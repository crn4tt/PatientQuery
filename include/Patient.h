#pragma once
#include "Visit.h"
#include <iostream>
#include <sstream>

class Patient{
private:
    int _id;

public:
    Patient();
    Patient(int id, std::string name, std::string surname, std::string patronomyc, std::string born_date, std::string gender);

    std::vector<Visit> History;

    std::string Name;
    std::string Surname;
    std::string Patronomyc;
    std::string BornDate;
    std::string Gender;

    void Print() const;

    int GetID() const;
};