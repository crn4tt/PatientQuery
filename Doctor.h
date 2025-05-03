#include "Patient.h"

class Doctor
{
private:
    Patient _patient;
    Visit _visit;
    
public:
    void SetPatient(Patient patient);
    void AddDiagnosis(std::string diagnosis);
    void AddDrug(std::string drug);
    Patient GetPatient();

};

