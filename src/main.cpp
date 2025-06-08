#include <iostream>
#include <cctype>
#include <cstdlib>
#include "PatientQueue.h"
#include "Doctor.h"

int main() {
    Doctor _doc;
    DataBaseWorker _dbw;
    PatientQueue _queue(_dbw); 
    size_t visit_num = _queue.GetVisitsCount(_dbw);
    std::vector<std::string> cur_work;
    
    while(!_queue.IsEmpty()){
        Patient current_patient = _queue.GetPatient(_dbw);
        _doc.SetPatient(current_patient);
        cur_work = _doc.Working();

        _queue.SendVisit(_dbw, cur_work[0], cur_work[1], current_patient, visit_num + 1, cur_work[2]);
        
        _queue.FreePatient(_dbw); 

        visit_num++;
    }
    


    return 0;
}