#pragma once
#include "Doctor.hpp"
#include "Patient.hpp"

class ConnectionHandlerDoctor {
private:
    int connection{0};
    Doctor doc;
public:
    Patient getPatientReq();
    void interaction(const Patient& p);
    void updateReq();
    void deleteReq();
};

