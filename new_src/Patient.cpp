#include "Patient.hpp"
#include <sstream>

Patient::Patient(const std::string& id_,
                 const std::string& name_,
                 const std::string& surname_,
                 const std::string& patronymic_,
                 const std::string& born_)
    : id(id_), name(name_), surname(surname_), patronymic(patronymic_),
      bornDate(born_) {}

std::string Patient::toString() const {
    std::ostringstream ss;
    ss << surname << ' ' << name << ' ' << patronymic << " (" << bornDate << ")";
    return ss.str();
}

