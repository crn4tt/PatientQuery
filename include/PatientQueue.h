#pragma once
#include "Patient.h"
#include "Queue.h"
#include "DataBaseWorker.h"

class PatientQueue {
private:
    Queue<Patient> _queuePatient;

public:
    explicit PatientQueue(DataBaseWorker& dbw); 

    Patient GetPatient(DataBaseWorker& dbw);
    void FreePatient(DataBaseWorker& dbw);
    size_t GetVisitsCount(DataBaseWorker& dbw); 
    void SendVisit(DataBaseWorker& dbw, std::string& drugs, std::string& diag, Patient pat, int visit_id, std::string& date);

    bool IsEmpty();

    ~PatientQueue() = default;
};

