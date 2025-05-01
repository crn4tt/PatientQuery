#include "DataBaseWorker.h"


DataBaseWorker::DataBaseWorker(){

    std::string connection = "host=192.168.1.100 "
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



// void DataBaseWorker::AddVisit(Visit& vis) {
//     try {
//         std::string drugs, his;
//         for(std::string str : vis.Drugs){
//             drugs += str + ",";
//         }
//         drugs[drugs.size() - 1] = ' ';

//         pqxx::work txn(_conn);
//         txn.exec_params(
//             "INSERT INTO Patients (ID, Name, Surname, Problem) "
//             "VALUES ($1, $2, $3, $4)",



//         );
//         txn.commit();
//     } catch (const std::exception& e) {
//         std::cerr << "Error adding patient: " << e.what() << std::endl;
//         throw;
//     }
// }

// Patient DataBaseWorker::GetPatient(int ID) {
//     try {
//         pqxx::work txn(_conn);
//         pqxx::result result = txn.exec_params(
//             "SELECT ID, Name, Surname, Problem "
//             "FROM patients "
//             "WHERE ID = $1",
//             ID
//         );

//         if (result.empty()) {
//             throw std::runtime_error("Patient not found");
//         }

//         const auto& row = result[0];
//         return Patient{
//             row["ID"].as<int>(),
//             row["Name"].as<std::string>(),
//             row["Surname"].as<std::string>(),
//             row["Problem"].as<std::string>(),

//         };
//     }
//     catch (const std::exception& e) {
//         std::cerr << "Error getting patient: " << e.what() << std::endl;
//         throw;
//     }
// }

//+ make getVisit to know about patient problem.