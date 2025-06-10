#include "PatientController.hpp"

PatientController::PatientController()
    : queuePat(), database(std::make_unique<DatabaseWorker>("")) {}

Patient PatientController::getPatient() {
    if (queuePat.isEmpty()) {
        return Patient();
    }
    Patient p = queuePat.front();
    queuePat.pop();
    return p;
}

void PatientController::addPatients(const std::vector<Patient>& list) {
    for (const auto& p : list) {
        queuePat.push(p);
    }
}

void PatientController::updatePatient(const Patient& oldP, const Patient& newP) {
    // TODO: implement patient update logic
}

void PatientController::replacePatient(const Patient& oldP, const Patient& newP) {
    // TODO: implement patient replace logic
}

