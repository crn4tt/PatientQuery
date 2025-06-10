#include "Patient.hpp"
#include <sstream>

std::string Patient::toString() const {
    std::ostringstream ss;
    ss << surname << ' ' << name << ' ' << patronymic << " (" << bornDate << ")";
    return ss.str();
}

