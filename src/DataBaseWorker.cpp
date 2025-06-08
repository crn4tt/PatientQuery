#include "DataBaseWorker.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <stdexcept>

DataBaseWorker::DataBaseWorker(const std::string& patientsFile,
                               const std::string& visitsFile)
    : _patientsFile(patientsFile), _visitsFile(visitsFile) {}

void DataBaseWorker::AddVisit(const std::string& drugs, const std::string& diagnosis,
                              const Patient& pat, int visit_id, const std::string& date)
{
    std::ofstream out(_visitsFile, std::ios::app);
    if (!out)
        throw std::runtime_error("Cannot open visits file");
    out << visit_id << ',' << pat.GetID() << ',' << date << ','
        << diagnosis << ',' << drugs << '\n';
}

void DataBaseWorker::GetPatients(Queue<Patient>& result)
{
    std::ifstream in(_patientsFile);
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
        Patient p(std::stoi(id), name, surname, patronymic, born, gender);
        result.Push(p);
    }
}

size_t DataBaseWorker::GetVisitsCount()
{
    std::ifstream in(_visitsFile);
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

Visit DataBaseWorker::GetHistory(const Patient& pat)
{
    std::ifstream in(_visitsFile);
    if (!in)
        throw std::runtime_error("Cannot open visits file");
    Visit result;
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty())
            continue;
        std::stringstream ss(line);
        std::string visit_id_str, pat_id_str, date, diagnosis, drugs;
        std::getline(ss, visit_id_str, ',');
        std::getline(ss, pat_id_str, ',');
        std::getline(ss, date, ',');
        std::getline(ss, diagnosis, ',');
        std::getline(ss, drugs, ',');
        if (std::stoi(pat_id_str) == pat.GetID()) {
            result.Drugs.emplace_back(drugs);
            result.History.emplace_back(diagnosis);
        }
    }
    return result;
}

void DataBaseWorker::DeletePatient(const Patient& pat)
{
    std::ifstream in(_patientsFile);
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
        if (std::stoi(id) != pat.GetID())
            lines.push_back(line);
    }
    in.close();

    std::ofstream out(_patientsFile, std::ios::trunc);
    for (const auto& l : lines)
        out << l << '\n';
}
