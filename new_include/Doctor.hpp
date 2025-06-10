#pragma once
#include <memory>
#include <string>
#include "Patient.hpp"
#include "Visit.hpp"

class Doctor {
private:
    std::shared_ptr<Patient> patient;
    std::shared_ptr<Visit> visit;
public:
    Doctor();
    void setPatient(const std::shared_ptr<Patient>& p);
    void addDrugs(const std::string& drug);
    void addHS(const std::string& hs);
    void setAnamnes(const std::string& anamnes);
    std::shared_ptr<Patient> getPatient() const;
};

