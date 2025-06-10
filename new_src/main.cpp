#include "Doctor.hpp"
#include "PatientController.hpp"
#include "ConnectionHandlerDoctor.hpp"
#include "ConnectionHandlerClient.hpp"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <sstream>

namespace {
constexpr int SERVER_PORT = 5555;
}

static void runServer() {
    PatientController controller;

    int server_fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        std::perror("socket");
        return;
    }
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERVER_PORT);
    addr.sin_addr.s_addr = INADDR_ANY;
    if (::bind(server_fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
        std::perror("bind");
        ::close(server_fd);
        return;
    }
    ::listen(server_fd, 1);
    std::cout << "Server listening on port " << SERVER_PORT << std::endl;
    int client = ::accept(server_fd, nullptr, nullptr);
    if (client < 0) {
        std::perror("accept");
        ::close(server_fd);
        return;
    }

    while (true) {
        char buf[512];
        int len = ::recv(client, buf, sizeof(buf) - 1, 0);
        if (len <= 0) break;
        buf[len] = '\0';

        if (std::strncmp(buf, "GET", 3) == 0) {
            if (!controller.hasPatients()) {
                const char* empty = "EMPTY\n";
                ::send(client, empty, std::strlen(empty), 0);
            } else {
                Patient p = controller.getPatient();
                std::stringstream ss;
                ss << p.getId() << ',' << p.getName() << ',' << p.getSurname() << ','
                   << p.getPatronymic() << ',' << p.getBornDate() << '\n';
                auto msg = ss.str();
                ::send(client, msg.c_str(), msg.size(), 0);
            }
        } else if (std::strncmp(buf, "VISIT", 5) == 0) {
            // Simplified: just print visit info
            std::cout << "Received visit data: " << (buf + 6) << std::endl;
        } else if (std::strncmp(buf, "REG", 3) == 0) {
            std::stringstream ss(buf + 4);
            std::string id, name, surname, patronymic, born;
            std::getline(ss, id, ',');
            std::getline(ss, name, ',');
            std::getline(ss, surname, ',');
            std::getline(ss, patronymic, ',');
            std::getline(ss, born, '\n');
            controller.addPatients({Patient(id, name, surname, patronymic, born)});
        } else if (std::strncmp(buf, "UPDATE", 6) == 0) {
            std::stringstream ss(buf + 7);
            std::string id, name, surname, patronymic, born;
            std::getline(ss, id, ',');
            std::getline(ss, name, ',');
            std::getline(ss, surname, ',');
            std::getline(ss, patronymic, ',');
            std::getline(ss, born, '\n');
            controller.updatePatient(Patient(id, name, surname, patronymic, born),
                                    Patient(id, name, surname, patronymic, born));
        } else if (std::strncmp(buf, "DELETE", 6) == 0) {
            std::string id(buf + 7);
            controller.replacePatient(Patient(id, "", "", "", ""), Patient());
        } else if (std::strncmp(buf, "EXIT", 4) == 0) {
            break;
        }
    }

    ::close(client);
    ::close(server_fd);
}

static void runDoctor() {
    ConnectionHandlerDoctor handler;
    while (true) {
        Patient p = handler.getPatientReq();
        if (p.getId().empty()) break;
        std::cout << "Patient: " << p.toString() << std::endl;
        handler.interaction(p);
        handler.updateReq();
    }
    handler.deleteReq();
}

int main(int argc, char* argv[]) {
    if (argc > 1 && std::strcmp(argv[1], "--server") == 0) {
        runServer();
    } else if (argc > 1 && std::strcmp(argv[1], "--doctor") == 0) {
        runDoctor();
    } else {
        std::cout << "Usage: " << argv[0] << " [--server|--doctor]" << std::endl;
    }
    return 0;
}

