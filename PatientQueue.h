#include "Patient.h"
#include "Queue.h"
#include "DataBaseWorker.h"

class PatientQueue
{
private:
    DataBaseWorker _dataBase;
    Queue<Patient> _queuePatient;

public:
    PatientQueue(); 

    Patient GetPatients();
    void FreePatient(Patient pat);


    ~PatientQueue() = default;
};

