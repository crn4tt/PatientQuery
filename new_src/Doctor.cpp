#include "Doctor.hpp"
#include <utility>
#include <iostream>

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

Visit Doctor::performExamination() {
    if (!visit) visit = std::make_shared<Visit>();
    visit->drugs.clear();
    visit->hs.clear();

    std::string input;
    std::cout << "Enter diagnosis: ";
    std::getline(std::cin, visit->anamnes);

    std::cout << "Enter prescribed drugs (type 'done' to finish): ";
    while (std::getline(std::cin, input)) {
        if (input == "done") break;
        if (!input.empty()) visit->drugs.push_back(input);
        std::cout << "drug: ";
    }

    std::cout << "Enter health services (type 'done' to finish): ";
    while (std::getline(std::cin, input)) {
        if (input == "done") break;
        if (!input.empty()) visit->hs.push_back(input);
        std::cout << "service: ";
    }

    return *visit;
}

