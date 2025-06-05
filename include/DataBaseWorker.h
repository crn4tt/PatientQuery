#pragma once
#include "Patient.h"
#include "Queue.h"
#include <pqxx/pqxx>

namespace clinic {

class DataBaseWorker final {
private:
    pqxx::connection _conn;

public:
    DataBaseWorker(const std::string& connection_string);
    
    DataBaseWorker(const DataBaseWorker&) = delete;
    DataBaseWorker& operator=(const DataBaseWorker&) = delete;

    void AddVisit(const std::string& drugs, const std::string& diagnosis,
                  const Patient& pat, int visit_id, const std::string& date);
    void DeletePatient(const Patient& pat);

    size_t GetVisitsCount();

    void GetPatients(Queue<Patient>& result);
    Visit GetHistory(const Patient& pat);

};

} // namespace clinic
