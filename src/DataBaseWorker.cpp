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
    std::string full_name = pat.Name + ' ' + pat.Surname + ' ' + pat.Patronomyc;
    out << visit_id << ',' << full_name << ',' << date << ','
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
        std::string visit_id_str, pat_name, date, diagnosis, drugs;
        std::getline(ss, visit_id_str, ',');
        std::getline(ss, pat_name, ',');
        std::getline(ss, date, ',');
        std::getline(ss, diagnosis, ',');
        std::getline(ss, drugs, ',');
        std::string full_name = pat.Name + " " + pat.Surname + " " + pat.Patronomyc;
        if (pat_name == full_name) {
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

Patient DataBaseWorker::AddPatient(const std::string& name, const std::string& surname,
                                   const std::string& patronomyc,
                                   const std::string& born_date,
                                   const std::string& gender)
{
    // Determine next ID
    size_t next_id = 1;
    std::ifstream in(_patientsFile);
    if (in) {
        std::string line;
        while (std::getline(in, line)) {
            if (line.empty())
                continue;
            std::stringstream ss(line);
            std::string id;
            std::getline(ss, id, ',');
            size_t num = std::stoul(id);
            if (num >= next_id)
                next_id = num + 1;
        }
        in.close();
    }

    std::ofstream out(_patientsFile, std::ios::app);
    if (!out)
        throw std::runtime_error("Cannot open patients file");
    out << next_id << ',' << name << ',' << surname << ',' << patronomyc << ','
        << born_date << ',' << gender << '\n';

    return Patient(static_cast<int>(next_id), name, surname, patronomyc,
                   born_date, gender);
}
