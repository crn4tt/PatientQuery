#pragma once
#include <memory>
#include <vector>
#include "Queue.hpp"
#include "DatabaseWorker.hpp"
#include "Patient.hpp"

class PatientController {
private:
    Queue<Patient> queuePat;
    std::unique_ptr<DatabaseWorker> database;
public:
    PatientController();
    Patient getPatient();
    void addPatients(const std::vector<Patient>& list);
    void updatePatient(const Patient& oldP, const Patient& newP);
    void replacePatient(const Patient& oldP, const Patient& newP);
};

