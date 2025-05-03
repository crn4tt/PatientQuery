#include "PatientQueue.h"

PatientQueue::PatientQueue():_queuePatient(), _dataBase(){    int num = _dataBase.GetPatientsCount();
    for (int i = 0; i < num; i++)
        _queuePatient.Push(_dataBase.GetPatient(i));    

} 


Patient PatientQueue::GetPatients(){
    _queuePatient.Front().Print();

    std::vector<Visit> visits = _dataBase.GetHistory(_queuePatient.Front().GetID());
    int i = visits.size();
    
    while (i--){
        visits[i].Print();
    }

    return _queuePatient.Front();
}
