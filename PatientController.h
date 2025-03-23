#include "Patient.h"
#include "Queue.h"
#include "DataBaseWorker.h"

class PatientController
{
private:
    DataBaseWorker _dataBase;
    Queue<Patient> _queuePatient;

public:
    PatientController();
    Patient GetPatient();
    void AddPatient();
    void UpdatePatient(Patient patient);
    void ReplacePatient(Patient patient);
    

    ~PatientController();
};

