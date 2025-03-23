#include "Patient.h"
#include "Visit.h"
#include <sys/socket.h>


class ConnectionHandler
{
private:
    class Doctor
    {
    private:
        Patient _patient;
        Visit _visit;

    public:
        void SetPatient(Patient patient);
        void AddHS(std::string hs);
        void AddDrug(std::string drug);
        void SetAnamnes(std::string an);
        Patient GetPatient();

    };

    Doctor _doc;
    sockaddr _connection;

public:

    Patient GetPatientReq();
    void Intention(Patient pat);
    void UpdateReq();
    void DeleteReq();

};
