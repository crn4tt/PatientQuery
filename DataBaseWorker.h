#include "Patient.h"
#include <pqxx/pqxx>


class DataBaseWorker
{
private:
    pqxx::connection _conn;

public:
    
    DataBaseWorker();
    void AddVisit(Visit& vis);
    Patient GetPatient(int id);


};
