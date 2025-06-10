#include "DatabaseWorker.hpp"
#include <fstream>
#include <sstream>
#include <vector>

DatabaseWorker::DatabaseWorker(const std::string& dbPath) : path(dbPath) {}

static std::string patientFile(const std::string& base) {
    if (base.empty()) return "patients.csv";
    return base + "/patients.csv";
}

std::vector<Patient> DatabaseWorker::getList() {
    std::vector<Patient> result;
    std::ifstream in(patientFile(path));
    if (!in)
        return result;

    std::string line;
    while (std::getline(in, line)) {
        if (line.empty())
            continue;
        std::stringstream ss(line);
        std::string id, name, surname, patronymic, born;
        std::getline(ss, id, ',');
        std::getline(ss, name, ',');
        std::getline(ss, surname, ',');
        std::getline(ss, patronymic, ',');
        std::getline(ss, born, ',');
        Patient p(id, name, surname, patronymic, born);
        result.push_back(p);
    }
    return result;
}

Patient DatabaseWorker::getPatient(const std::string& id) {
    std::ifstream in(patientFile(path));
    if (!in)
        return {};
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty())
            continue;
        std::stringstream ss(line);
        std::string pid, name, surname, patronymic, born;
        std::getline(ss, pid, ',');
        std::getline(ss, name, ',');
        std::getline(ss, surname, ',');
        std::getline(ss, patronymic, ',');
        std::getline(ss, born, ',');
        if (pid == id)
            return Patient(pid, name, surname, patronymic, born);
    }
    return {};
}

void DatabaseWorker::deleteRecord(const std::string& id) {
    std::ifstream in(patientFile(path));
    if (!in)
        return;
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty())
            continue;
        std::stringstream ss(line);
        std::string pid;
        std::getline(ss, pid, ',');
        if (pid != id)
            lines.push_back(line);
    }
    in.close();
    std::ofstream out(patientFile(path), std::ios::trunc);
    for (const auto& l : lines)
        out << l << '\n';
}

void DatabaseWorker::updateRecord(const Patient& entity) {
    std::ifstream in(patientFile(path));
    std::vector<std::string> lines;
    std::string line;
    bool updated = false;
    if (in) {
        while (std::getline(in, line)) {
            if (line.empty())
                continue;
            std::stringstream ss(line);
            std::string pid;
            std::getline(ss, pid, ',');
            if (pid == entity.getId()) {
                std::stringstream newline;
                newline << entity.getId() << ',' << entity.getName() << ','
                       << entity.getSurname() << ',' << entity.getPatronymic()
                       << ',' << entity.getBornDate();
                lines.push_back(newline.str());
                updated = true;
            } else {
                lines.push_back(line);
            }
        }
        in.close();
    }
    if (!updated) {
        std::stringstream newline;
        newline << entity.getId() << ',' << entity.getName() << ','
               << entity.getSurname() << ',' << entity.getPatronymic() << ','
               << entity.getBornDate();
        lines.push_back(newline.str());
    }

    std::ofstream out(patientFile(path), std::ios::trunc);
    for (const auto& l : lines)
        out << l << '\n';
}

