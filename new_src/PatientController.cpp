#include "PatientController.hpp"

PatientController::PatientController()
    : queuePat(), database(std::make_unique<DatabaseWorker>("")) {
    auto list = database->getList();
    for (const auto& p : list)
        queuePat.push(p);
}

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
        database->updateRecord(p);
    }
}

void PatientController::updatePatient(const Patient& oldP, const Patient& newP) {
    database->updateRecord(newP);
}

void PatientController::replacePatient(const Patient& oldP, const Patient& newP) {
    database->deleteRecord(oldP.getId());
    database->updateRecord(newP);
}

