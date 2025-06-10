#pragma once
#include "Patient.hpp"
#include "Queue.hpp"
#include <string>

class DatabaseWorker {
private:
    std::string patientsFile;
    std::string visitsFile;
public:
    DatabaseWorker(const std::string& patientsFile = "patients.csv",
                   const std::string& visitsFile = "visits.csv");
    DatabaseWorker(const DatabaseWorker&) = delete;
    DatabaseWorker& operator=(const DatabaseWorker&) = delete;

    void addVisit(const std::string& drugs, const std::string& diagnosis,
                  const Patient& pat, int visitId, const std::string& date);
    void deletePatient(const Patient& pat);
    Patient addPatient(const std::string& name, const std::string& surname,
                       const std::string& patronymic,
                       const std::string& bornDate, const std::string& gender);
    size_t getVisitsCount();
    void getPatients(Queue<Patient>& result);
    Visit getHistory(const Patient& pat);
};
