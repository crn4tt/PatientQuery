#include "DatabaseWorker.hpp"

DatabaseWorker::DatabaseWorker(const std::string& dbPath) : path(dbPath) {}

std::vector<Patient> DatabaseWorker::getList() {
    // TODO: load patient list from storage
    return {};
}

Patient DatabaseWorker::getPatient(const std::string& id) {
    // TODO: fetch patient by id
    return Patient();
}

void DatabaseWorker::deleteRecord(const std::string& id) {
    // TODO: delete patient record
}

void DatabaseWorker::updateRecord(const Patient& entity) {
    // TODO: update patient data
}

