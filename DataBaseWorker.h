#pragma once
#include "Patient.h"
#include "Queue.h"
#include <pqxx/pqxx>
#include <memory>

class DataBaseWorker
{
private:
    std::shared_ptr<pqxx::connection> _conn;

public:
    DataBaseWorker(const std::string& connection_string);
    
    DataBaseWorker(const DataBaseWorker&) = delete;
    DataBaseWorker& operator=(const DataBaseWorker&) = delete;

    void AddVisit(const std::string& drugs, const std::string& diagnosis, Patient& pat, int visit_id, const std::string& date); 
    void DeletePatient(Patient& pat);

    size_t GetVisitsCount();

    void GetPatients(Queue<Patient>& result);
    Visit GetHistory(Patient pat); 

};
