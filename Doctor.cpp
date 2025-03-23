#include "ConnectionHandler.h"

void ConnectionHandler::Doctor::SetPatient(Patient pat){
    _patient = pat;
}

void ConnectionHandler::Doctor::AddHS(std::string hs){
    _visit.HS.emplace_back(hs);
}

void ConnectionHandler::Doctor::AddDrug(std::string drug){
    _visit.Drugs.emplace_back(drug);
}

void ConnectionHandler::Doctor::SetAnamnes(std::string an){
    _visit._anamnes += "\n" + an;
}

Patient ConnectionHandler::Doctor::GetPatient(){
    
}