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
    std::string gender;
    std::vector<Visit> visitLog;
public:
    Patient() = default;
    Patient(const std::string& id,
            const std::string& name,
            const std::string& surname,
            const std::string& patronymic,
            const std::string& bornDate,
            const std::string& gender = "");

    const std::string& getId() const { return id; }
    const std::string& getName() const { return name; }
    const std::string& getSurname() const { return surname; }
    const std::string& getPatronymic() const { return patronymic; }
    const std::string& getBornDate() const { return bornDate; }
    const std::string& getGender() const { return gender; }

    void addVisit(const Visit& v) { visitLog.push_back(v); }
    const std::vector<Visit>& getVisits() const { return visitLog; }

    std::string toString() const;
};

