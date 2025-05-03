#include "Doctor.h"

void Doctor::SetPatient(Patient pat){
    _patient = pat;
}

void Doctor::AddDiagnosis(std::string diagnosis){
    _visit.History.emplace_back(diagnosis);
}

void Doctor::AddDrug(std::string drug){
    _visit.Drugs.emplace_back(drug);
    
}

