#include "ConnectionHandlerDoctor.hpp"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <sstream>
#include <iostream>

namespace {
constexpr int SERVER_PORT = 5555;
}

ConnectionHandlerDoctor::ConnectionHandlerDoctor() {
    connection = ::socket(AF_INET, SOCK_STREAM, 0);
    if (connection < 0) {
        std::perror("socket");
        return;
    }
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERVER_PORT);
    addr.sin_addr.s_addr = ::inet_addr("127.0.0.1");
    if (::connect(connection, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
        std::perror("connect");
        ::close(connection);
        connection = -1;
    }
}

Patient ConnectionHandlerDoctor::getPatientReq() {
    if (connection < 0) return {};
    const char* msg = "GET\n";
    ::send(connection, msg, std::strlen(msg), 0);
    char buf[512];
    int len = ::recv(connection, buf, sizeof(buf) - 1, 0);
    if (len <= 0) return {};
    buf[len] = '\0';
    if (std::strncmp(buf, "EMPTY", 5) == 0) return {};

    std::stringstream ss(buf);
    std::string id, name, surname, patronymic, born;
    std::getline(ss, id, ',');
    std::getline(ss, name, ',');
    std::getline(ss, surname, ',');
    std::getline(ss, patronymic, ',');
    std::getline(ss, born, '\n');
    Patient p(id, name, surname, patronymic, born);
    doc.setPatient(std::make_shared<Patient>(p));
    return p;
}

void ConnectionHandlerDoctor::interaction(const Patient& p) {
    if (connection < 0) return;
    doc.setPatient(std::make_shared<Patient>(p));
    Visit v = doc.performExamination();

    std::stringstream ss;
    ss << "VISIT," << p.getId() << ',' << v.anamnes << ',';
    for (size_t i = 0; i < v.drugs.size(); ++i) {
        ss << v.drugs[i];
        if (i + 1 < v.drugs.size()) ss << '|';
    }
    ss << ',';
    for (size_t i = 0; i < v.hs.size(); ++i) {
        ss << v.hs[i];
        if (i + 1 < v.hs.size()) ss << '|';
    }
    ss << '\n';

    auto msg = ss.str();
    ::send(connection, msg.c_str(), msg.size(), 0);
}

void ConnectionHandlerDoctor::updateReq() {
    if (connection >= 0) {
        const char* msg = "DONE\n";
        ::send(connection, msg, std::strlen(msg), 0);
    }
}

void ConnectionHandlerDoctor::deleteReq() {
    if (connection >= 0) {
        const char* msg = "EXIT\n";
        ::send(connection, msg, std::strlen(msg), 0);
        ::close(connection);
        connection = -1;
    }
}

