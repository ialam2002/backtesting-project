#include <string>
#include <vector>

namespace quant {
namespace {

std::vector<std::string> split_csv_row(const std::string& line) {
    std::vector<std::string> out;
    std::string current;
    for (char ch : line) {
        if (ch == ',') {
            out.push_back(current);
            current.clear();
        } else {
            current.push_back(ch);
        }
    }
    out.push_back(current);
    return out;
}

}  // namespace
}  // namespace quant
