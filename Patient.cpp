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

void Patient::Print(){
    std::cout << Name << ' ';
    std::cout << Surname << ' ';
    std::cout << Patronomyc << '\n';
    std::cout << BornDate << '\n';
    std::cout << Gender << '\n';
}

int Patient::GetID(){
    return _id;
}
