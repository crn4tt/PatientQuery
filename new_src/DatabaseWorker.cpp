#include "DatabaseWorker.hpp"
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>

DatabaseWorker::DatabaseWorker(const std::string& patFile,
                               const std::string& visFile)
    : patientsFile(patFile), visitsFile(visFile) {}

void DatabaseWorker::addVisit(const std::string& drugs, const std::string& diagnosis,
                              const Patient& pat, int visitId, const std::string& date) {
    std::ofstream out(visitsFile, std::ios::app);
    if (!out)
        throw std::runtime_error("Cannot open visits file");
    std::string fullName = pat.getName() + ' ' + pat.getSurname() + ' ' + pat.getPatronymic();
    out << visitId << ',' << fullName << ',' << date << ','
        << diagnosis << ',' << drugs << '\n';
}

void DatabaseWorker::getPatients(Queue<Patient>& result) {
    std::ifstream in(patientsFile);
    if (!in)
        throw std::runtime_error("Cannot open patients file");
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty())
            continue;
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

size_t DatabaseWorker::getVisitsCount() {
    std::ifstream in(visitsFile);
    if (!in)
        throw std::runtime_error("Cannot open visits file");
    size_t count = 0;
    std::string line;
    while (std::getline(in, line)) {
        if (!line.empty())
            ++count;
    }
    return count;
}

Visit DatabaseWorker::getHistory(const Patient& pat) {
    std::ifstream in(visitsFile);
    if (!in)
        throw std::runtime_error("Cannot open visits file");
    Visit result;
    std::string line;
    std::string fullName = pat.getName() + " " + pat.getSurname() + " " + pat.getPatronymic();
    while (std::getline(in, line)) {
        if (line.empty())
            continue;
        std::stringstream ss(line);
        std::string vid, name, date, diag, drugs;
        std::getline(ss, vid, ',');
        std::getline(ss, name, ',');
        std::getline(ss, date, ',');
        std::getline(ss, diag, ',');
        std::getline(ss, drugs, ',');
        if (name == fullName) {
            result.date = date;
            result.anamnes = diag;
            std::stringstream ds(drugs);
            std::string token;
            while (std::getline(ds, token, '|')) {
                if (!token.empty()) result.drugs.push_back(token);
            }
        }
    }
    return result;
}

void DatabaseWorker::deletePatient(const Patient& pat) {
    std::ifstream in(patientsFile);
    if (!in)
        throw std::runtime_error("Cannot open patients file");
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty())
            continue;
        std::stringstream ss(line);
        std::string id;
        std::getline(ss, id, ',');
        if (id != pat.getId())
            lines.push_back(line);
    }
    in.close();
    std::ofstream out(patientsFile, std::ios::trunc);
    for (const auto& l : lines)
        out << l << '\n';
}

Patient DatabaseWorker::addPatient(const std::string& name, const std::string& surname,
                                   const std::string& patronymic,
                                   const std::string& bornDate, const std::string& gender) {
    size_t nextId = 1;
    std::ifstream in(patientsFile);
    if (in) {
        std::string line;
        while (std::getline(in, line)) {
            if (line.empty())
                continue;
            std::stringstream ss(line);
            std::string id;
            std::getline(ss, id, ',');
            size_t num = std::stoul(id);
            if (num >= nextId)
                nextId = num + 1;
        }
        in.close();
    }

    std::ofstream out(patientsFile, std::ios::app);
    if (!out)
        throw std::runtime_error("Cannot open patients file");
    out << nextId << ',' << name << ',' << surname << ',' << patronymic << ','
        << bornDate << ',' << gender << '\n';

    return Patient(std::to_string(nextId), name, surname, patronymic, bornDate, gender);
}

