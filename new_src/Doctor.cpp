#include "Doctor.hpp"
#include <utility>

Doctor::Doctor() : patient(nullptr), visit(std::make_shared<Visit>()) {}

void Doctor::setPatient(const std::shared_ptr<Patient>& p) {
    patient = p;
}

void Doctor::addDrugs(const std::string& drug) {
    if (!visit) visit = std::make_shared<Visit>();
    visit->drugs.push_back(drug);
}

void Doctor::addHS(const std::string& hs) {
    if (!visit) visit = std::make_shared<Visit>();
    visit->hs.push_back(hs);
}

void Doctor::setAnamnes(const std::string& anamnes) {
    if (!visit) visit = std::make_shared<Visit>();
    visit->anamnes = anamnes;
}

std::shared_ptr<Patient> Doctor::getPatient() const {
    return patient;
}

