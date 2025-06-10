// Single-file version of Patient Queue application
// Combines all headers and sources into one compilation unit

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <exception>
#include <fstream>
#include <iostream>
#include <limits>
#include <memory>
#include <mutex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

// ---- Node and Queue ----
template <typename T>
struct Node {
    T data;
    Node<T>* next = nullptr;
    explicit Node(const T& d) : data(d), next(nullptr) {}
};

template <typename T>
class Queue {
private:
    Node<T>* head = nullptr;
    Node<T>* tail = nullptr;
    std::size_t size = 0;
public:
    Queue() = default;
    ~Queue() {
        while (!isEmpty()) pop();
    }

    void push(const T& elem) {
        Node<T>* node = new Node<T>(elem);
        if (tail) {
            tail->next = node;
        } else {
            head = node;
        }
        tail = node;
        ++size;
    }

    void pop() {
        if (isEmpty()) throw std::runtime_error("Queue is empty");
        Node<T>* tmp = head;
        head = head->next;
        if (!head) tail = nullptr;
        delete tmp;
        --size;
    }

    T& front() const {
        if (isEmpty()) throw std::runtime_error("Queue is empty");
        return head->data;
    }

    bool isEmpty() const { return size == 0; }
    bool isFull() const { return false; }
};

// ---- Visit ----
struct Visit {
    std::vector<std::string> drugs;
    std::vector<std::string> hs;
    std::string anamnes;
    std::string date;
};

// ---- Patient ----
class Patient {
    std::string id;
    std::string name;
    std::string surname;
    std::string patronymic;
    std::string bornDate;
    std::string gender;
    std::vector<Visit> visitLog;
public:
    Patient() = default;
    Patient(const std::string& id_, const std::string& name_,
            const std::string& surname_, const std::string& patronymic_,
            const std::string& born_, const std::string& gender_ = "")
        : id(id_), name(name_), surname(surname_), patronymic(patronymic_),
          bornDate(born_), gender(gender_) {}

    const std::string& getId() const { return id; }
    const std::string& getName() const { return name; }
    const std::string& getSurname() const { return surname; }
    const std::string& getPatronymic() const { return patronymic; }
    const std::string& getBornDate() const { return bornDate; }
    const std::string& getGender() const { return gender; }

    void addVisit(const Visit& v) { visitLog.push_back(v); }
    const std::vector<Visit>& getVisits() const { return visitLog; }

    std::string toString() const {
        std::ostringstream ss;
        ss << surname << ' ' << name << ' ' << patronymic
           << " (" << bornDate << "," << gender << ")";
        return ss.str();
    }
};

// ---- Doctor ----
class Doctor {
    std::shared_ptr<Patient> patient;
    std::shared_ptr<Visit> visit;
public:
    Doctor() : patient(nullptr), visit(std::make_shared<Visit>()) {}

    void setPatient(const std::shared_ptr<Patient>& p) { patient = p; }
    void addDrugs(const std::string& drug) {
        if (!visit) visit = std::make_shared<Visit>();
        visit->drugs.push_back(drug);
    }
    void addHS(const std::string& hs) {
        if (!visit) visit = std::make_shared<Visit>();
        visit->hs.push_back(hs);
    }
    void setAnamnes(const std::string& anamnes) {
        if (!visit) visit = std::make_shared<Visit>();
        visit->anamnes = anamnes;
    }
    std::shared_ptr<Patient> getPatient() const { return patient; }

    Visit performExamination() {
        if (!visit) visit = std::make_shared<Visit>();
        visit->drugs.clear();
        visit->hs.clear();
        std::cout << "Enter visit date (yyyy-mm-dd): ";
        std::getline(std::cin, visit->date);
        std::string input;
        std::cout << "Enter diagnosis: ";
        std::getline(std::cin, visit->anamnes);

        std::cout << "Enter prescribed drugs (type 'done' to finish): ";
        while (std::getline(std::cin, input)) {
            if (input == "done") break;
            if (!input.empty()) visit->drugs.push_back(input);
            std::cout << "drug: ";
        }
        // Additional health services input removed
        return *visit;
    }
};

// ---- DatabaseWorker ----
class DatabaseWorker {
    std::string patientsFile;
    std::string visitsFile;
    std::string allPatientsFile;
public:
    DatabaseWorker(const std::string& patFile = "patients.csv",
                   const std::string& visFile = "visits.csv",
                   const std::string& allPatFile = "patients_all.csv")
        : patientsFile(patFile), visitsFile(visFile), allPatientsFile(allPatFile) {}
    DatabaseWorker(const DatabaseWorker&) = delete;
    DatabaseWorker& operator=(const DatabaseWorker&) = delete;

    void addVisit(const std::string& drugs, const std::string& diagnosis,
                  const Patient& pat, int visitId, const std::string& date) {
        std::ofstream out(visitsFile, std::ios::app);
        if (!out) throw std::runtime_error("Cannot open visits file");
        std::string fullName = pat.getName() + ' ' + pat.getSurname() + ' ' + pat.getPatronymic();
        out << visitId << ',' << fullName << ',' << date << ','
            << diagnosis << ',' << drugs << '\n';
    }

    void deletePatient(const Patient& pat) {
        std::ifstream in(patientsFile);
        if (!in) throw std::runtime_error("Cannot open patients file");
        std::vector<std::string> lines;
        std::string line;
        while (std::getline(in, line)) {
            if (line.empty()) continue;
            std::stringstream ss(line);
            std::string id;
            std::getline(ss, id, ',');
            if (id != pat.getId()) lines.push_back(line);
        }
        in.close();
        std::ofstream out(patientsFile, std::ios::trunc);
        for (const auto& l : lines) out << l << '\n';
    }

    Patient addPatient(const std::string& name, const std::string& surname,
                       const std::string& patronymic, const std::string& bornDate,
                       const std::string& gender) {
        size_t nextId = 1;
        std::ifstream in(allPatientsFile);
        if (in) {
            std::string line;
            while (std::getline(in, line)) {
                if (line.empty()) continue;
                std::stringstream ss(line);
                std::string id;
                std::getline(ss, id, ',');
                size_t num = std::stoul(id);
                if (num >= nextId) nextId = num + 1;
            }
            in.close();
        }

        std::ofstream outCur(patientsFile, std::ios::app);
        if (!outCur) throw std::runtime_error("Cannot open patients file");
        outCur << nextId << ',' << name << ',' << surname << ',' << patronymic << ','
               << bornDate << ',' << gender << '\n';

        std::ofstream outAll(allPatientsFile, std::ios::app);
        if (!outAll) throw std::runtime_error("Cannot open all patients file");
        outAll << nextId << ',' << name << ',' << surname << ',' << patronymic << ','
               << bornDate << ',' << gender << '\n';

        return Patient(std::to_string(nextId), name, surname, patronymic, bornDate, gender);
    }

    size_t getVisitsCount() {
        std::ifstream in(visitsFile);
        if (!in) throw std::runtime_error("Cannot open visits file");
        size_t count = 0;
        std::string line;
        while (std::getline(in, line)) {
            if (!line.empty()) ++count;
        }
        return count;
    }

    void getPatients(Queue<Patient>& result) {
        std::ifstream in(patientsFile);
        if (!in) throw std::runtime_error("Cannot open patients file");
        std::string line;
        while (std::getline(in, line)) {
            if (line.empty()) continue;
            std::stringstream ss(line);
            std::string id, name, surname, patronymic, born, gender;
            std::getline(ss, id, ',');
            std::getline(ss, name, ',');
            std::getline(ss, surname, ',');
            std::getline(ss, patronymic, ',');
            std::getline(ss, born, ',');
            std::getline(ss, gender, ',');
            result.push(Patient(id, name, surname, patronymic, born, gender));
        }
    }

    Visit getHistory(const Patient& pat) {
        std::ifstream in(visitsFile);
        if (!in) throw std::runtime_error("Cannot open visits file");
        Visit res;
        std::string line;
        std::string fullName = pat.getName() + " " + pat.getSurname() + " " + pat.getPatronymic();
        while (std::getline(in, line)) {
            if (line.empty()) continue;
            std::stringstream ss(line);
            std::string vid, name, date, diag, drugs;
            std::getline(ss, vid, ',');
            std::getline(ss, name, ',');
            std::getline(ss, date, ',');
            std::getline(ss, diag, ',');
            std::getline(ss, drugs, ',');
            if (name == fullName) {
                res.date = date;
                res.anamnes = diag;
                std::stringstream ds(drugs);
                std::string token;
                while (std::getline(ds, token, '|')) {
                    if (!token.empty()) res.drugs.push_back(token);
                }
            }
        }
        return res;
    }
};

// ---- PatientController ----
class PatientController {
    Queue<Patient> queuePat;
    std::unique_ptr<DatabaseWorker> database;
    mutable std::mutex mtx;
public:
    PatientController() : queuePat(), database(std::make_unique<DatabaseWorker>()) {
        database->getPatients(queuePat);
    }
    bool hasPatients() const {
        std::lock_guard<std::mutex> lock(mtx);
        return !queuePat.isEmpty();
    }
    Patient getPatient() {
        std::lock_guard<std::mutex> lock(mtx);
        if (queuePat.isEmpty()) return Patient();
        Patient p = queuePat.front();
        queuePat.pop();
        return p;
    }
    void addPatient(const Patient& p) {
        std::lock_guard<std::mutex> lock(mtx);
        queuePat.push(p);
    }
    Patient registerPatient(const std::string& name, const std::string& surname,
                            const std::string& patronymic, const std::string& born,
                            const std::string& gender) {
        std::lock_guard<std::mutex> lock(mtx);
        Patient p = database->addPatient(name, surname, patronymic, born, gender);
        queuePat.push(p);
        return p;
    }
    void saveVisit(const Visit& v, const Patient& p, int visitId, const std::string& date) {
        std::string drugsJoined;
        for (size_t i = 0; i < v.drugs.size(); ++i) {
            drugsJoined += v.drugs[i];
            if (i + 1 < v.drugs.size()) drugsJoined += '|';
        }
        std::lock_guard<std::mutex> lock(mtx);
        database->addVisit(drugsJoined, v.anamnes, p, visitId, date);
        database->deletePatient(p);
    }
    size_t visitsCount() const {
        std::lock_guard<std::mutex> lock(mtx);
        return database->getVisitsCount();
    }
};

// ---- ConnectionHandlerDoctor ----
class ConnectionHandlerDoctor {
    int connection{0};
    Doctor doc;
public:
    ConnectionHandlerDoctor() {
        connection = ::socket(AF_INET, SOCK_STREAM, 0);
        if (connection < 0) {
            std::perror("socket");
            return;
        }
        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(5555);
        addr.sin_addr.s_addr = ::inet_addr("127.0.0.1");
        if (::connect(connection, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
            std::perror("connect");
            ::close(connection);
            connection = -1;
        }
    }
    Patient getPatientReq() {
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
    void interaction(const Patient& p) {
        if (connection < 0) return;
        doc.setPatient(std::make_shared<Patient>(p));
        Visit v = doc.performExamination();
        std::stringstream ss;
        ss << "VISIT," << p.getId() << ',' << v.date << ',' << v.anamnes << ',';
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
    void updateReq() {
        if (connection >= 0) {
            const char* msg = "DONE\n";
            ::send(connection, msg, std::strlen(msg), 0);
        }
    }
    void deleteReq() {
        if (connection >= 0) {
            const char* msg = "EXIT\n";
            ::send(connection, msg, std::strlen(msg), 0);
            ::close(connection);
            connection = -1;
        }
    }
};

// ---- ConnectionHandlerClient ----
class ConnectionHandlerClient {
    PatientController* patients{nullptr};
    int connection{0};
public:
    explicit ConnectionHandlerClient(PatientController* ctrl) : patients(ctrl), connection(0) {}
    void connectServer() {
        connection = ::socket(AF_INET, SOCK_STREAM, 0);
        if (connection < 0) {
            std::perror("socket");
            return;
        }
        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(5555);
        addr.sin_addr.s_addr = ::inet_addr("127.0.0.1");
        if (::connect(connection, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
            std::perror("connect");
            ::close(connection);
            connection = -1;
        }
    }
    void regHandler() {
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
        ss << "REG," << id << ',' << name << ',' << surname << ',' << patronymic << ','
           << born << ',' << gender << '\n';
        auto msg = ss.str();
        ::send(connection, msg.c_str(), msg.size(), 0);
    }
    Patient getPatientReq() {
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
    void updatePatientReq(const std::string& id) {
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
        ss << "UPDATE," << id << ',' << name << ',' << surname << ',' << patronymic << ','
           << born << ',' << gender << '\n';
        auto msg = ss.str();
        ::send(connection, msg.c_str(), msg.size(), 0);
    }
    void deletePatient(const std::string& id) {
        if (connection < 0) return;
        std::stringstream ss;
        ss << "DELETE," << id << '\n';
        auto msg = ss.str();
        ::send(connection, msg.c_str(), msg.size(), 0);
    }
};

// ---- Main server/doctor logic ----
namespace {
constexpr int SERVER_PORT = 5555;
}

static void handleDoctor(int client, PatientController* controller) {
    while (true) {
        char buf[512];
        int len = ::recv(client, buf, sizeof(buf) - 1, 0);
        if (len <= 0) break;
        buf[len] = '\0';
        if (std::strncmp(buf, "GET", 3) == 0) {
            if (!controller->hasPatients()) {
                const char* empty = "EMPTY\n";
                ::send(client, empty, std::strlen(empty), 0);
            } else {
                Patient p = controller->getPatient();
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
            controller->saveVisit(v, Patient(id, "", "", "", ""), controller->visitsCount() + 1, date);
        } else if (std::strncmp(buf, "REG", 3) == 0) {
            std::stringstream ss(buf + 4);
            std::string id, name, surname, patronymic, born, gender;
            std::getline(ss, id, ',');
            std::getline(ss, name, ',');
            std::getline(ss, surname, ',');
            std::getline(ss, patronymic, ',');
            std::getline(ss, born, ',');
            std::getline(ss, gender, '\n');
            controller->addPatient(Patient(id, name, surname, patronymic, born, gender));
        } else if (std::strncmp(buf, "UPDATE", 6) == 0) {
            std::stringstream ss(buf + 7);
            std::string id, name, surname, patronymic, born, gender;
            std::getline(ss, id, ',');
            std::getline(ss, name, ',');
            std::getline(ss, surname, ',');
            std::getline(ss, patronymic, ',');
            std::getline(ss, born, ',');
            std::getline(ss, gender, '\n');
            controller->addPatient(Patient(id, name, surname, patronymic, born, gender));
        } else if (std::strncmp(buf, "DELETE", 6) == 0) {
            // deletion not implemented
        } else if (std::strncmp(buf, "EXIT", 4) == 0) {
            break;
        }
    }
    ::close(client);
}

static void registerPatients(PatientController* controller) {
    while (true) {
        std::string name;
        std::cout << "Enter patient name (or 'exit' to stop): ";
        if (!std::getline(std::cin, name)) break;
        if (name == "exit") break;
        std::string surname, patronymic, born, gender;
        std::cout << "Enter surname: ";
        std::getline(std::cin, surname);
        std::cout << "Enter patronymic: ";
        std::getline(std::cin, patronymic);
        std::cout << "Enter birth date: ";
        std::getline(std::cin, born);
        std::cout << "Enter gender: ";
        std::getline(std::cin, gender);
        controller->registerPatient(name, surname, patronymic, born, gender);
    }
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
    ::listen(server_fd, 5);
    std::cout << "Server listening on port " << SERVER_PORT << std::endl;

    std::thread regThr(registerPatients, &controller);
    std::vector<std::thread> docThreads;
    while (true) {
        int client = ::accept(server_fd, nullptr, nullptr);
        if (client < 0) {
            std::perror("accept");
            break;
        }
        docThreads.emplace_back(handleDoctor, client, &controller);
    }
    for (auto& t : docThreads) t.join();
    if (regThr.joinable()) regThr.join();
    ::close(server_fd);
}

static void runDoctor() {
    ConnectionHandlerDoctor docHandler;
    while (true) {
        std::cout << "\n=== Patient Queue Menu ===" << std::endl;
        std::cout << "1. Serve next patient" << std::endl;
        std::cout << "2. Exit" << std::endl;
        std::cout << "Choose option: ";
        char choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (choice == '2') {
            break;
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

