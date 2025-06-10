#include "Visit.h"
#include <iostream>


void Visit::Print() const {
    std::cout << "--- Previous Visits ---" << std::endl;
    if (Drugs.empty() && History.empty()) {
        std::cout << "No visit history" << std::endl << std::endl;
        return;
    }

    if (!Drugs.empty()) {
        std::cout << "Medications:" << std::endl;
        for (const auto& d : Drugs)
            std::cout << "  - " << d << std::endl;
    }

    if (!History.empty()) {
        std::cout << "Diagnoses:" << std::endl;
        for (const auto& h : History)
            std::cout << "  - " << h << std::endl;
    }

    std::cout << std::endl;
}