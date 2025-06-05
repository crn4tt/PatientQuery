#include <iostream>
#include <cctype>
#include <cstdlib>
#include "PatientQueue.h"
#include "Doctor.h"
#include <termios.h> 
#include <unistd.h>


using namespace clinic;

std::string getPassword() {
    termios oldt;
    tcgetattr(STDIN_FILENO, &oldt);
    termios newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    std::string password;
    std::getline(std::cin, password);

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return password;
}


int main() {
    const std::string host = "85.192.29.145";
    const std::string port = "5432";
    std::string dbname, user, password;

    std::cout << "Database name: ";
    std::cin >> dbname;

    std::cout << "Username: ";
    std::cin >> user;

    std::cout << "Password: ";
    std::cin.ignore();
    password = getPassword();
    
    const std::string connection_str =
        "host=" + host + " " +
        "port=" + port + " " +
        "dbname=" + dbname + " " +
        "user=" + user + " " +
        "password=" + password;

    std::cout << "\n";
    Doctor _doc;
    DataBaseWorker _dbw(connection_str);
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
