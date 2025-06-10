#pragma once
#include <memory>
#include <vector>
#include <string>
#include "Queue.hpp"
#include "DatabaseWorker.hpp"
#include "Patient.hpp"

class PatientController {
private:
    Queue<Patient> queuePat;
    std::unique_ptr<DatabaseWorker> database;
public:
    PatientController();
    bool hasPatients() const { return !queuePat.isEmpty(); }
    Patient getPatient();
    void addPatient(const Patient& p);
    void saveVisit(const Visit& v, const Patient& p, int visitId, const std::string& date);
    size_t visitsCount() const;
};

