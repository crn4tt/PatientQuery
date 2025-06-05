#include "DataBaseWorker.h"

namespace clinic {

DataBaseWorker::DataBaseWorker(const std::string& connection_string) try
    : _conn(connection_string)
{
}
catch (const std::exception& e) {
    std::cerr << "Connection failed: " << e.what() << std::endl;
    throw;
}



void DataBaseWorker::AddVisit(const std::string& drugs, const std::string& diagnosis,
                              const Patient& pat, int visit_id, const std::string& date) {
    try {
        pqxx::work txn(_conn);
        txn.exec_params(
            "INSERT INTO visits (visit_id, pat_id, visit_date, diagnosis, drugs) "
            "VALUES ($1, $2, $3, $4, $5)",
            visit_id,
            pat.GetID(),
            date,
            diagnosis,
            drugs
        );

        txn.commit();
    } catch (const std::exception& e) {
        std::cerr << "Error adding patient: " << e.what() << std::endl;
        throw;
    }
}

void DataBaseWorker::GetPatients(Queue<Patient>& result) {
    try {
        pqxx::work txn(_conn);
        pqxx::result res = txn.exec("SELECT * FROM patients");
        for (const auto& row : res) {
            Patient current_row = Patient(
                row["pat_id"].as<int>(),
                row["name"].as<std::string>(),
                row["surname"].as<std::string>(),
                row["patronomic"].as<std::string>(),
                row["born_date"].as<std::string>(),
                row["gender"].as<std::string>()
            );
            result.Push(current_row);
        }
        
        txn.commit();
    }
    catch (const std::exception& e) {
        std::cerr << "Error getting patient: " << e.what() << std::endl;
        throw;
    }
}

size_t DataBaseWorker::GetVisitsCount() {
    try {
        pqxx::work txn(_conn);
        pqxx::result result = txn.exec("SELECT COUNT(*) FROM visits");
        return result[0][0].as<size_t>();
    }
    catch(const std::exception& e) {
        std::cerr << "Error getting patients: " << e.what() << std::endl;
        throw;
    }
}

Visit DataBaseWorker::GetHistory(const Patient& pat){
   try{ 
        pqxx::work txn(_conn);
        
        Visit result;
        pqxx::result res = txn.exec_params("SELECT * FROM visits WHERE pat_id=$1", pat.GetID());


        for (const auto& row : res) {
            
            std::string drugs = row["drugs"].is_null() ? "" : row["drugs"].as<std::string>();
            result.Drugs.emplace_back(drugs);

            std::string hist = row["diagnosis"].is_null() ? "" : row["diagnosis"].as<std::string>();
            result.History.emplace_back(hist);
        }

        txn.commit();
        return result;
    }
    catch(const std::exception& e) {
        std::cerr << "Error getting patient's history: " << e.what() << std::endl;
        throw;
    }
}


void DataBaseWorker::DeletePatient(const Patient& pat){
    try {
        pqxx::work txn(_conn);
        
        txn.exec_params(
            "DELETE FROM patients WHERE pat_id=$1",
            pat.GetID()
        );

        txn.commit();
        
    }
    catch(const std::exception& e) {
        std::cerr << "Error delete patient: " << e.what() << std::endl;
        throw;
    }
}

} // namespace clinic
