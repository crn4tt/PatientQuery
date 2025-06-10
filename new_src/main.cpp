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
#include <limits>

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
                   << p.getPatronymic() << ',' << p.getBornDate() << ',' << p.getGender() << '\n';
                auto msg = ss.str();
                ::send(client, msg.c_str(), msg.size(), 0);
            }
        } else if (std::strncmp(buf, "VISIT", 5) == 0) {
            std::stringstream ss(buf + 6);
            std::string id, date, anamnes, drugs, hs;
            std::getline(ss, id, ',');
            std::getline(ss, date, ',');
            std::getline(ss, anamnes, ',');
            std::getline(ss, drugs, ',');
            std::getline(ss, hs, '\n');
            Visit v;
            v.anamnes = anamnes;
            v.date = date;
            std::stringstream ds(drugs);
            std::string token;
            while (std::getline(ds, token, '|')) {
                if (!token.empty()) v.drugs.push_back(token);
            }
            controller.saveVisit(v, Patient(id, "", "", "", ""), controller.visitsCount() + 1, date);
        } else if (std::strncmp(buf, "REG", 3) == 0) {
            std::stringstream ss(buf + 4);
            std::string id, name, surname, patronymic, born, gender;
            std::getline(ss, id, ',');
            std::getline(ss, name, ',');
            std::getline(ss, surname, ',');
            std::getline(ss, patronymic, ',');
            std::getline(ss, born, ',');
            std::getline(ss, gender, '\n');
            controller.addPatient(Patient(id, name, surname, patronymic, born, gender));
        } else if (std::strncmp(buf, "UPDATE", 6) == 0) {
            std::stringstream ss(buf + 7);
            std::string id, name, surname, patronymic, born, gender;
            std::getline(ss, id, ',');
            std::getline(ss, name, ',');
            std::getline(ss, surname, ',');
            std::getline(ss, patronymic, ',');
            std::getline(ss, born, ',');
            std::getline(ss, gender, '\n');
            controller.addPatient(Patient(id, name, surname, patronymic, born, gender));
        } else if (std::strncmp(buf, "DELETE", 6) == 0) {
            std::string id(buf + 7);
            // remove patient from queue
        } else if (std::strncmp(buf, "EXIT", 4) == 0) {
            break;
        }
    }

    ::close(client);
    ::close(server_fd);
}

static void runDoctor() {
    ConnectionHandlerDoctor docHandler;
    ConnectionHandlerClient regHandler(nullptr);
    regHandler.connect();
    while (true) {
        std::cout << "\n=== Patient Queue Menu ===" << std::endl;
        std::cout << "1. Serve next patient" << std::endl;
        std::cout << "2. Add patient" << std::endl;
        std::cout << "3. Exit" << std::endl;
        std::cout << "Choose option: ";

        char choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == '3') {
            break;
        } else if (choice == '2') {
            regHandler.regHandler();
            continue;
        } else if (choice != '1') {
            std::cout << "Unknown option" << std::endl;
            continue;
        }

        Patient p = docHandler.getPatientReq();
        if (p.getId().empty()) {
            std::cout << "Queue is empty." << std::endl;
            break;
        }
        std::cout << "Patient: " << p.toString() << std::endl;
        docHandler.interaction(p);
        docHandler.updateReq();
    }
    docHandler.deleteReq();
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

