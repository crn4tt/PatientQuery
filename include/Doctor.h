#pragma once
#include "Patient.h"
#include "Visit.h"
#include <vector>

class Doctor {
private:
    Patient _patient;
    Visit _visit;

public:
    Doctor() = default;
    void SetPatient(const Patient& patient);
    const Patient& GetPat() const;
    std::vector<std::string> Working();
};