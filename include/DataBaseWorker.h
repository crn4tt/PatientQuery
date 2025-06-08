#pragma once
#include "Patient.h"
#include "Queue.h"
#include <string>

class DataBaseWorker
{
private:
    std::string _patientsFile;
    std::string _visitsFile;

public:
    DataBaseWorker(const std::string& patientsFile = "patients.csv",
                   const std::string& visitsFile = "visits.csv");
    
    DataBaseWorker(const DataBaseWorker&) = delete;
    DataBaseWorker& operator=(const DataBaseWorker&) = delete;

    void AddVisit(const std::string& drugs, const std::string& diagnosis,
                  const Patient& pat, int visit_id, const std::string& date);
    void DeletePatient(const Patient& pat);

    size_t GetVisitsCount();

    void GetPatients(Queue<Patient>& result);
    Visit GetHistory(const Patient& pat);

};
