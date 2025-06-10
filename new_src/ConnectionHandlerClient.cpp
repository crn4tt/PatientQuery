#include "ConnectionHandlerClient.hpp"
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

ConnectionHandlerClient::ConnectionHandlerClient(PatientController* ctrl)
    : patients(ctrl), connection(0) {}

void ConnectionHandlerClient::connect() {
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

void ConnectionHandlerClient::regHandler() {
    if (connection < 0) return;
    std::string id, name, surname, patronymic, born, gender;
    std::cout << "Enter id: ";
    std::getline(std::cin, id);
    std::cout << "Enter name: ";
    std::getline(std::cin, name);
    std::cout << "Enter surname: ";
    std::getline(std::cin, surname);
    std::cout << "Enter patronymic: ";
    std::getline(std::cin, patronymic);
    std::cout << "Enter birth date: ";
    std::getline(std::cin, born);
    std::cout << "Enter gender: ";
    std::getline(std::cin, gender);

    std::stringstream ss;
    ss << "REG," << id << ',' << name << ',' << surname << ',' << patronymic << ',' << born << ',' << gender << '\n';
    auto msg = ss.str();
    ::send(connection, msg.c_str(), msg.size(), 0);
}

Patient ConnectionHandlerClient::getPatientReq() {
    if (connection < 0) return {};
    const char* msg = "GET\n";
    ::send(connection, msg, std::strlen(msg), 0);
    char buf[512];
    int len = ::recv(connection, buf, sizeof(buf) - 1, 0);
    if (len <= 0) return {};
    buf[len] = '\0';
    std::stringstream ss(buf);
    std::string id, name, surname, patronymic, born, gender;
    std::getline(ss, id, ',');
    std::getline(ss, name, ',');
    std::getline(ss, surname, ',');
    std::getline(ss, patronymic, ',');
    std::getline(ss, born, ',');
    std::getline(ss, gender, '\n');
    return Patient(id, name, surname, patronymic, born, gender);
}

void ConnectionHandlerClient::updatePatientReq(const std::string& id) {
    if (connection < 0) return;
    std::string name, surname, patronymic, born, gender;
    std::cout << "Enter new name: ";
    std::getline(std::cin, name);
    std::cout << "Enter new surname: ";
    std::getline(std::cin, surname);
    std::cout << "Enter new patronymic: ";
    std::getline(std::cin, patronymic);
    std::cout << "Enter new birth date: ";
    std::getline(std::cin, born);
    std::cout << "Enter new gender: ";
    std::getline(std::cin, gender);
    std::stringstream ss;
    ss << "UPDATE," << id << ',' << name << ',' << surname << ',' << patronymic << ',' << born << ',' << gender << '\n';
    auto msg = ss.str();
    ::send(connection, msg.c_str(), msg.size(), 0);
}

void ConnectionHandlerClient::deletePatient(const std::string& id) {
    if (connection < 0) return;
    std::stringstream ss;
    ss << "DELETE," << id << '\n';
    auto msg = ss.str();
    ::send(connection, msg.c_str(), msg.size(), 0);
}

