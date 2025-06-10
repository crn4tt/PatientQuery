#include "ConnectionHandlerClient.hpp"

ConnectionHandlerClient::ConnectionHandlerClient(PatientController* ctrl)
    : patients(ctrl), connection(0) {}

void ConnectionHandlerClient::connect() {
    // TODO: establish connection
}

void ConnectionHandlerClient::regHandler() {
    // TODO: implement registration handling
}

Patient ConnectionHandlerClient::getPatientReq() {
    // TODO: implement patient request
    return Patient();
}

void ConnectionHandlerClient::updatePatientReq(const std::string& id) {
    // TODO: implement update patient request
}

void ConnectionHandlerClient::deletePatient(const std::string& id) {
    // TODO: implement delete patient request
}

