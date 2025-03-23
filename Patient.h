#include "Visit.h"
#include <iostream>
#include <sstream>

class Patient{
private:
    std::string _id;

public:
    std::vector<Visit> History;
    
    std::string Name;
    std::string LastName;
    std::string Patronomyc;

    std::string BornDate;

    std::string ToString();
};