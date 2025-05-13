#include "Doctor.h"

std::string ToString(std::vector<std::string> vec){
    std::string tmp;
    for(std::string str : vec){
        tmp += str + ",";
    }
    if (!tmp.empty()) {
        tmp.pop_back();
    }

    return tmp;
}


Patient Doctor::GetPat(){
    return _patient;
}

void Doctor::SetPatient(const Patient& pat) { 
    _patient = pat;
}

std::vector<std::string> Doctor::Working(){

    std::string _diag;
    std::string tmp;
    std::string _drug;

    std::cout << "Write diagnosis:\n";
    getline(std::cin, _diag);
    std::cout << "\n";


    std::cout << "Write drugs(insert 0 to stop):\n";
    std::cin >> tmp;
    
    while (tmp[0] != '0') {
        _drug += tmp + " ";
        std::cin >> tmp;
    } 
    
    std::cin.ignore(); 
    
    std::cout << "Write date(yyyy-mm-dd): \n";
    std::string _date;
    getline(std::cin, _date);
    std::cout << "\n";

    std::vector<std::string> result(3);

    result[0] = _drug;
    result[1] = _diag;
    result[2] = _date;

    return result;
}