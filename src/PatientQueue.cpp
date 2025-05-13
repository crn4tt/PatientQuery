#include "PatientQueue.h"



PatientQueue::PatientQueue(DataBaseWorker& dbw):_queuePatient(){   
    dbw.GetPatients(_queuePatient);

} 

Patient PatientQueue::GetPatient(DataBaseWorker& dbw){
    _queuePatient.Front().Print();

    Visit visits = dbw.GetHistory(_queuePatient.Front());
    visits.Print();
    _queuePatient.Front().History.emplace_back(visits);
    return _queuePatient.Front();
}

size_t PatientQueue::GetVisitsCount(DataBaseWorker& dbw){
    return dbw.GetVisitsCount();
}

void PatientQueue::FreePatient(DataBaseWorker& dbw){
    Patient pat = _queuePatient.Front();
    dbw.DeletePatient(pat);
    _queuePatient.Pop();
}

void PatientQueue::SendVisit(DataBaseWorker& dbw, std::string& drugs, std::string& diag, Patient pat, int visit_id, std::string& date){
    dbw.AddVisit(drugs, diag, pat, visit_id, date);
}

bool PatientQueue::IsEmpty(){
    return _queuePatient.IsEmpty();
}