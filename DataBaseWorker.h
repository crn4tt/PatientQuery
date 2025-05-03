#include "Patient.h"
#include <pqxx/pqxx>


class DataBaseWorker
{
private:
    pqxx::connection _conn;

public:
    DataBaseWorker();
    
    void AddVisit(Visit& vis, Patient& pat, int visit_id);
    void DeletePatient(Patient& pat);

    size_t GetPatientsCount();

    Patient GetPatient(int id);
    std::vector<Visit> GetHistory(int id);


};
