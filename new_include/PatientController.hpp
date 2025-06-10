#pragma once
#include <memory>
#include <vector>
#include <string>
#include <mutex>
#include "Queue.hpp"
#include "DatabaseWorker.hpp"
#include "Patient.hpp"

class PatientController {
private:
    Queue<Patient> queuePat;
    std::unique_ptr<DatabaseWorker> database;
    mutable std::mutex mtx;
public:
    PatientController();
    bool hasPatients() const {
        std::lock_guard<std::mutex> lock(mtx);
        return !queuePat.isEmpty();
    }
    Patient getPatient();
    void addPatient(const Patient& p);
    Patient registerPatient(const std::string& name,
                            const std::string& surname,
                            const std::string& patronymic,
                            const std::string& born,
                            const std::string& gender);
    void saveVisit(const Visit& v, const Patient& p, int visitId, const std::string& date);
    size_t visitsCount() const;
};

