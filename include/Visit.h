#pragma once
#include <vector>
#include <string>

namespace clinic {

struct Visit final {
    std::vector<std::string> Drugs;
    std::vector<std::string> History;
    void Print() const;
};

} // namespace clinic
