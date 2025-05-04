#pragma once
#include "PatientQueue.h"

class Doctor {
private:
    Patient _patient;
    Visit _visit;

public:
    Doctor() = default;
    void SetPatient(const Patient& patient);
    Patient GetPat();
    std::vector<std::string> Working();
};