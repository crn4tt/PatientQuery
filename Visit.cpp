#include "Visit.h"
#include <iostream>


void Visit::Print(){

    std::cout << "Pills:\n";
    for (size_t i = 0; i < Drugs.size(); i++)
        std::cout << Drugs[i] << '\n';


    std::cout << "\nHistory:\n";
    for (size_t i = 0; i < History.size(); i++)
        std::cout << History[i] << '\n';
     
}