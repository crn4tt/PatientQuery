#include "DataBaseWorker.h"


DataBaseWorker::DataBaseWorker(){

    std::string connection = "host=85.192.29.145 "
    "port=5432 ";
    std::string tmp;

    std::cout << " Write database name: " << std::endl;

    std::cin >> tmp;
    std::cout << '\n';
    
    connection += tmp + " ";

    std::cout << " Write username: " << std::endl;
    std::cin >> tmp;
    std::cout << '\n';
    
    connection += tmp + " ";


    std::cout << " Password: " << std::endl;
    std::cin >> tmp;
    std::cout << '\n';
    
    connection += tmp + " ";

    _conn = pqxx::connection(connection);
    if(!_conn.is_open()){
        throw "db connection failed!";
    }

}



void DataBaseWorker::AddVisit(Visit& vis, Patient& pat, int visit_id) {
    try {
        std::string drugs;
        for(std::string str : vis.Drugs){
            drugs += str + ",";
        }
        drugs[drugs.size() - 1] = ' ';

        std::string diagnosis;
        std::cout << "Write diagnosis: \n";
        getline(std::cin, diagnosis);
        std::cout << "\n";

        std::string date;
        std::cout << "Write date(yyyy-mm-dd): \n";
        getline(std::cin, date);
        std::cout << " \n";


        pqxx::work txn(_conn);
        txn.exec_params(
            "INSERT INTO Patients (visit_id, pat_id, visit_date, diagnosis, dugs) "
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

Patient DataBaseWorker::GetPatient(int ID) {
    try {
        pqxx::work txn(_conn);
        pqxx::result result = txn.exec_params(
            "SELECT pat_id, name, surname, patronomic, born_date, gender, last_visit "
            "FROM patients "
            "WHERE pat_id = $1",
            ID
        );

        if (result.empty()) {
            throw std::runtime_error("Patient not found");
        }

        const auto& row = result[0];
        return Patient(
            row["pat_id"].as<int>(),
            row["name"].as<std::string>(),
            row["surname"].as<std::string>(),
            row["patronomic"].as<std::string>(),
            row["born_date"].as<std::string>(),
            row["gender"].as<std::string>(),
            row["last_visit"].as<int>()
        );
    }
    catch (const std::exception& e) {
        std::cerr << "Error getting patient: " << e.what() << std::endl;
        throw;
    }
}

size_t DataBaseWorker::GetPatientsCount() {
    try {
    pqxx::work txn(_conn);
    std::string query = "SELECT COUNT(*) FROM patients";
    pqxx::result result = txn.exec(query);
    return txn.query_value<size_t>(query);
    }
    catch(const std::exception& e) {
        std::cerr << "Error getting patients count: " << e.what() << std::endl;
        throw;
    }
}

//+ make getVisit to know about patient problem.