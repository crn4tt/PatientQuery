#include "Visit.h"
#include <iostream>

namespace clinic {

void Visit::Print() const {

    std::cout << "Pills:\n";
    for (size_t i = 0; i < Drugs.size(); i++)
        std::cout << Drugs[i] << '\n';


    std::cout << "\nHistory:\n";
    for (size_t i = 0; i < History.size(); i++)
        std::cout << History[i] << '\n';
     
}

} // namespace clinic
