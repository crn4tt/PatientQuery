#include "Visit.h"
#include <iostream>
#include <sstream>

class Patient{
private:
    int _id;

public:
    Patient(int id, std::string name, std::string surname, std::string patronomyc, std::string born_date, std::string gender, int last_visit);

    std::vector<Visit> History;

    std::string Name;
    std::string Surname;
    std::string Patronomyc;
    std::string BornDate;
    std::string Gender;
    int LastVisit;

    void Print();
    
    int GetID();
};