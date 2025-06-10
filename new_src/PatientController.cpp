#include "PatientController.hpp"

PatientController::PatientController()
    : queuePat(), database(std::make_unique<DatabaseWorker>()) {
    database->getPatients(queuePat);
}

Patient PatientController::getPatient() {
    if (queuePat.isEmpty()) {
        return Patient();
    }
    Patient p = queuePat.front();
    queuePat.pop();
    return p;
}

void PatientController::addPatient(const Patient& p) {
    queuePat.push(p);
}

void PatientController::saveVisit(const Visit& v, const Patient& p, int visitId, const std::string& date) {
    std::string drugsJoined;
    for (size_t i = 0; i < v.drugs.size(); ++i) {
        drugsJoined += v.drugs[i];
        if (i + 1 < v.drugs.size()) drugsJoined += '|';
    }
    database->addVisit(drugsJoined, v.anamnes, p, visitId, date);
    database->deletePatient(p);
}

size_t PatientController::visitsCount() const {
    return database->getVisitsCount();
}

