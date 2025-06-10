#include "Patient.h"


Patient::Patient(){
    _id = 0;
    Name = "";
    Surname = "";
    Patronomyc = "";
    BornDate = "";
    Gender = "";
}

Patient::Patient(int id, std::string name, std::string surname, std::string patronomyc, std::string born_date, std::string gender){

    _id = id;
    Name = name;
    Surname = surname;
    Patronomyc = patronomyc;
    BornDate = born_date;
    Gender = gender;

}

void Patient::Print() const {
    std::cout << "\n==== Patient Information ====" << std::endl;
    std::cout << "Name: " << Name << ' ' << Surname << ' ' << Patronomyc
              << std::endl;
    std::cout << "Date of birth: " << BornDate << std::endl;
    std::cout << "Gender: " << Gender << std::endl << std::endl;
}

int Patient::GetID() const{
    return _id;
}
