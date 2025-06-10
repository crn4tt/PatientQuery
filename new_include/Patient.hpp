#pragma once
#include "Visit.hpp"
#include <string>
#include <vector>


class Patient {
private:
    std::string id;
    std::string name;
    std::string surname;
    std::string patronymic;
    std::string bornDate;
    std::vector<Visit> visitLog;
public:
    Patient() = default;
    std::string toString() const;
};

