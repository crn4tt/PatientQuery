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
    void SendVisit(DataBaseWorker& dbw, const std::string& drugs, const std::string& diag,
                   const Patient& pat, int visit_id, const std::string& date);

    bool IsEmpty() const;

    ~PatientQueue() = default;
};

