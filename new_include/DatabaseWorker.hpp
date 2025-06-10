#pragma once
#include <string>
#include <vector>
#include "Patient.hpp"

class DatabaseWorker {
private:
    std::string path;
public:
    explicit DatabaseWorker(const std::string& dbPath);
    std::vector<Patient> getList();
    Patient getPatient(const std::string& id);
    void deleteRecord(const std::string& id);
    void updateRecord(const Patient& entity);
};

