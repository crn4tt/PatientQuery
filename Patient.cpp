#include "Patient.h"


Patient::Patient(int id, std::string name, std::string surname, std::string patronomyc, std::string born_date, std::string gender, int last_visit){

    _id = id;
    Name = name;
    Surname = surname;
    Patronomyc = patronomyc;
    BornDate = born_date;
    Gender = gender;
    LastVisit = last_visit;

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
