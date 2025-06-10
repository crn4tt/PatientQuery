#pragma once
#include <string>
#include "PatientController.hpp"
#include "Patient.hpp"

class ConnectionHandlerClient {
private:
    PatientController* patients{nullptr};
    int connection{0};
public:
    explicit ConnectionHandlerClient(PatientController* ctrl);
    void connect();
    void regHandler();
    Patient getPatientReq();
    void updatePatientReq(const std::string& id);
    void deletePatient(const std::string& id);
};

