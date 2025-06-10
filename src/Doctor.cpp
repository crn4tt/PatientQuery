#include "Doctor.h"
#include <limits>

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


const Patient& Doctor::GetPat() const{
    return _patient;
}

void Doctor::SetPatient(const Patient& pat) { 
    _patient = pat;
}

std::vector<std::string> Doctor::Working() {
    std::string _diag;
    std::string tmp;
    std::string _drug;

    std::cout << "Enter diagnosis: ";
    std::getline(std::cin, _diag);

    std::cout << "Enter drugs (0 to stop): ";
    std::cin >> tmp;
    while (tmp != "0") {
        _drug += tmp + " ";
        std::cin >> tmp;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Enter visit date (yyyy-mm-dd): ";
    std::string _date;
    std::getline(std::cin, _date);

    std::vector<std::string> result(3);
    result[0] = _drug;
    result[1] = _diag;
    result[2] = _date;

    return result;
}