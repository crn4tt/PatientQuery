#include <iostream>
#include <cctype>
#include <cstdlib>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <queue>
#include <stdexcept>

struct Visit {
    std::vector<std::string> Drugs;
    std::vector<std::string> History;
    void Print() const;
};

void Visit::Print() const {
    std::cout << "Pills:\n";
    for (const auto& d : Drugs)
        std::cout << d << '\n';

    std::cout << "\nHistory:\n";
    for (const auto& h : History)
        std::cout << h << '\n';
}

class Patient {
private:
    int _id;

public:
    Patient();
    Patient(int id, std::string name, std::string surname,
            std::string patronomyc, std::string born_date,
            std::string gender);

    std::vector<Visit> History;

    std::string Name;
    std::string Surname;
    std::string Patronomyc;
    std::string BornDate;
    std::string Gender;

    void Print() const;
    int GetID() const;
};

Patient::Patient() : _id(0) {}

Patient::Patient(int id, std::string name, std::string surname,
                 std::string patronomyc, std::string born_date,
                 std::string gender)
    : _id(id), Name(std::move(name)), Surname(std::move(surname)),
      Patronomyc(std::move(patronomyc)), BornDate(std::move(born_date)),
      Gender(std::move(gender)) {}

void Patient::Print() const {
    std::cout << Name << ' ' << Surname << ' ' << Patronomyc << '\n';
    std::cout << BornDate << '\n';
    std::cout << Gender << '\n';
}

int Patient::GetID() const {
    return _id;
}

template <typename T>
class Queue {
private:
    std::queue<T> _data;

public:
    Queue() = default;

    bool IsEmpty() const { return _data.empty(); }

    void Push(const T& value) { _data.push(value); }

    void Pop() {
        if (IsEmpty())
            throw std::runtime_error("Queue is empty");
        _data.pop();
    }

    T& Front() { return _data.front(); }
    const T& Front() const { return _data.front(); }

    size_t Size() const { return _data.size(); }
};

class DataBaseWorker {
private:
    std::string _patientsFile;
    std::string _visitsFile;

public:
    DataBaseWorker(const std::string& patientsFile = "patients.csv",
                   const std::string& visitsFile = "visits.csv");

    DataBaseWorker(const DataBaseWorker&) = delete;
    DataBaseWorker& operator=(const DataBaseWorker&) = delete;

    void AddVisit(const std::string& drugs, const std::string& diagnosis,
                  const Patient& pat, int visit_id, const std::string& date);
    void DeletePatient(const Patient& pat);

    size_t GetVisitsCount();

    void GetPatients(Queue<Patient>& result);
    Visit GetHistory(const Patient& pat);
};

DataBaseWorker::DataBaseWorker(const std::string& patientsFile,
                               const std::string& visitsFile)
    : _patientsFile(patientsFile), _visitsFile(visitsFile) {}

void DataBaseWorker::AddVisit(const std::string& drugs, const std::string& diagnosis,
                              const Patient& pat, int visit_id, const std::string& date) {
    std::ofstream out(_visitsFile, std::ios::app);
    if (!out)
        throw std::runtime_error("Cannot open visits file");
    out << visit_id << ',' << pat.GetID() << ',' << date << ','
        << diagnosis << ',' << drugs << '\n';
}

void DataBaseWorker::GetPatients(Queue<Patient>& result) {
    std::ifstream in(_patientsFile);
    if (!in)
        throw std::runtime_error("Cannot open patients file");
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty())
            continue;
        std::stringstream ss(line);
        std::string id, name, surname, patronymic, born, gender;
        std::getline(ss, id, ',');
        std::getline(ss, name, ',');
        std::getline(ss, surname, ',');
        std::getline(ss, patronymic, ',');
        std::getline(ss, born, ',');
        std::getline(ss, gender, ',');
        Patient p(std::stoi(id), name, surname, patronymic, born, gender);
        result.Push(p);
    }
}

size_t DataBaseWorker::GetVisitsCount() {
    std::ifstream in(_visitsFile);
    if (!in)
        throw std::runtime_error("Cannot open visits file");
    size_t count = 0;
    std::string line;
    while (std::getline(in, line)) {
        if (!line.empty())
            ++count;
    }
    return count;
}

Visit DataBaseWorker::GetHistory(const Patient& pat) {
    std::ifstream in(_visitsFile);
    if (!in)
        throw std::runtime_error("Cannot open visits file");
    Visit result;
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty())
            continue;
        std::stringstream ss(line);
        std::string visit_id_str, pat_id_str, date, diagnosis, drugs;
        std::getline(ss, visit_id_str, ',');
        std::getline(ss, pat_id_str, ',');
        std::getline(ss, date, ',');
        std::getline(ss, diagnosis, ',');
        std::getline(ss, drugs, ',');
        if (std::stoi(pat_id_str) == pat.GetID()) {
            result.Drugs.emplace_back(drugs);
            result.History.emplace_back(diagnosis);
        }
    }
    return result;
}

void DataBaseWorker::DeletePatient(const Patient& pat) {
    std::ifstream in(_patientsFile);
    if (!in)
        throw std::runtime_error("Cannot open patients file");
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty())
            continue;
        std::stringstream ss(line);
        std::string id;
        std::getline(ss, id, ',');
        if (std::stoi(id) != pat.GetID())
            lines.push_back(line);
    }
    in.close();

    std::ofstream out(_patientsFile, std::ios::trunc);
    for (const auto& l : lines)
        out << l << '\n';
}

class Doctor {
private:
    Patient _patient;
    Visit _visit;

public:
    Doctor() = default;
    void SetPatient(const Patient& patient);
    const Patient& GetPat() const;
    std::vector<std::string> Working();
};

std::string ToString(std::vector<std::string> vec) {
    std::string tmp;
    for (const std::string& str : vec) {
        tmp += str + ",";
    }
    if (!tmp.empty()) {
        tmp.pop_back();
    }
    return tmp;
}

const Patient& Doctor::GetPat() const {
    return _patient;
}

void Doctor::SetPatient(const Patient& pat) {
    _patient = pat;
}

std::vector<std::string> Doctor::Working() {
    std::string _diag;
    std::string tmp;
    std::string _drug;

    std::cout << "Write diagnosis:\n";
    std::getline(std::cin, _diag);
    std::cout << "\n";

    std::cout << "Write drugs(insert 0 to stop):\n";
    std::cin >> tmp;

    while (tmp[0] != '0') {
        _drug += tmp + " ";
        std::cin >> tmp;
    }

    std::cin.ignore();

    std::cout << "Write date(yyyy-mm-dd): \n";
    std::string _date;
    std::getline(std::cin, _date);
    std::cout << "\n";

    std::vector<std::string> result(3);
    result[0] = _drug;
    result[1] = _diag;
    result[2] = _date;

    return result;
}

class PatientQueue {
private:
    Queue<Patient> _queuePatient;

public:
    explicit PatientQueue(DataBaseWorker& dbw);

    Patient GetPatient(DataBaseWorker& dbw);
    void FreePatient(DataBaseWorker& dbw);
    size_t GetVisitsCount(DataBaseWorker& dbw);
    void SendVisit(DataBaseWorker& dbw, const std::string& drugs, const std::string& diag,
                   const Patient& pat, int visit_id, const std::string& date);

    bool IsEmpty() const;
};

PatientQueue::PatientQueue(DataBaseWorker& dbw) : _queuePatient() {
    dbw.GetPatients(_queuePatient);
}

Patient PatientQueue::GetPatient(DataBaseWorker& dbw) {
    _queuePatient.Front().Print();

    Visit visits = dbw.GetHistory(_queuePatient.Front());
    visits.Print();
    _queuePatient.Front().History.emplace_back(visits);
    return _queuePatient.Front();
}

size_t PatientQueue::GetVisitsCount(DataBaseWorker& dbw) {
    return dbw.GetVisitsCount();
}

void PatientQueue::FreePatient(DataBaseWorker& dbw) {
    Patient pat = _queuePatient.Front();
    dbw.DeletePatient(pat);
    _queuePatient.Pop();
}

void PatientQueue::SendVisit(DataBaseWorker& dbw, const std::string& drugs, const std::string& diag,
                             const Patient& pat, int visit_id, const std::string& date) {
    dbw.AddVisit(drugs, diag, pat, visit_id, date);
}

bool PatientQueue::IsEmpty() const {
    return _queuePatient.IsEmpty();
}

int main() {
    Doctor _doc;
    DataBaseWorker _dbw;
    PatientQueue _queue(_dbw);
    size_t visit_num = _queue.GetVisitsCount(_dbw);
    std::vector<std::string> cur_work;

    while(!_queue.IsEmpty()){
        Patient current_patient = _queue.GetPatient(_dbw);
        _doc.SetPatient(current_patient);
        cur_work = _doc.Working();

        _queue.SendVisit(_dbw, cur_work[0], cur_work[1], current_patient, visit_num + 1, cur_work[2]);
        _queue.FreePatient(_dbw);
        visit_num++;
    }

    return 0;
}

