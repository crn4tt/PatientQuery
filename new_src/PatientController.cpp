#include "PatientController.hpp"

PatientController::PatientController()
    : queuePat(), database(std::make_unique<DatabaseWorker>()) {
    database->getPatients(queuePat);
}

Patient PatientController::getPatient() {
    std::lock_guard<std::mutex> lock(mtx);
    if (queuePat.isEmpty()) {
        return Patient();
    }
    Patient p = queuePat.front();
    queuePat.pop();
    return p;
}

void PatientController::addPatient(const Patient& p) {
    std::lock_guard<std::mutex> lock(mtx);
    queuePat.push(p);
}

Patient PatientController::registerPatient(const std::string& name,
                                           const std::string& surname,
                                           const std::string& patronymic,
                                           const std::string& born,
                                           const std::string& gender) {
    std::lock_guard<std::mutex> lock(mtx);
    Patient p = database->addPatient(name, surname, patronymic, born, gender);
    queuePat.push(p);
    return p;
}

void PatientController::saveVisit(const Visit& v, const Patient& p, int visitId, const std::string& date) {
    std::string drugsJoined;
    for (size_t i = 0; i < v.drugs.size(); ++i) {
        drugsJoined += v.drugs[i];
        if (i + 1 < v.drugs.size()) drugsJoined += '|';
    }
    std::lock_guard<std::mutex> lock(mtx);
    database->addVisit(drugsJoined, v.anamnes, p, visitId, date);
    database->deletePatient(p);
}

size_t PatientController::visitsCount() const {
    std::lock_guard<std::mutex> lock(mtx);
    return database->getVisitsCount();
}

