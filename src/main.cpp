#include <iostream>
#include <cctype>
#include <cstdlib>
#include <limits>
#include "PatientQueue.h"
#include "Doctor.h"

int main() {
    Doctor doc;
    DataBaseWorker dbw;
    PatientQueue queue(dbw);
    size_t visit_num = queue.GetVisitsCount(dbw);
    std::vector<std::string> cur_work;

    while (true) {
        std::cout << "\n=== Patient Queue Menu ===" << std::endl;
        std::cout << "1. Serve next patient" << std::endl;
        std::cout << "2. Add patient" << std::endl;
        std::cout << "3. Exit" << std::endl;
        std::cout << "Choose option: ";

        char choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == '3')
            break;
        if (choice == '2') {
            std::string name, surname, patronymic, born, gender;
            std::cout << "Name: ";
            std::getline(std::cin, name);
            std::cout << "Surname: ";
            std::getline(std::cin, surname);
            std::cout << "Patronymic: ";
            std::getline(std::cin, patronymic);
            std::cout << "Date of birth (yyyy-mm-dd): ";
            std::getline(std::cin, born);
            std::cout << "Gender (M/F): ";
            std::getline(std::cin, gender);
            Patient new_pat = dbw.AddPatient(name, surname, patronymic, born, gender);
            queue.AddPatient(new_pat);
            continue;
        }
        if (choice != '1') {
            std::cout << "Unknown option" << std::endl;
            continue;
        }

        if (queue.IsEmpty()) {
            std::cout << "Queue is empty." << std::endl;
            break;
        }

        Patient current_patient = queue.GetPatient(dbw);
        doc.SetPatient(current_patient);
        cur_work = doc.Working();
        queue.SendVisit(dbw, cur_work[0], cur_work[1], current_patient,
                        visit_num + 1, cur_work[2]);
        queue.FreePatient(dbw);
        visit_num++;
    }

    return 0;
}
