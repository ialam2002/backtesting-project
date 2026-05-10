#include "quant/data/data_loader.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

namespace quant {

std::vector<Price> load_prices_from_csv(const std::string& file_path) {
    std::ifstream input(file_path);
    if (!input.is_open()) {
        throw std::runtime_error("Unable to open CSV file: " + file_path);
    }

    std::vector<Price> prices;
    std::string line;
    while (std::getline(input, line)) {
        if (line.empty()) {
            continue;
        }

        std::stringstream row(line);
        std::string cell;
        if (std::getline(row, cell, ',')) {
            try {
                prices.push_back(std::stod(cell));
            } catch (...) {
                // Skip header or malformed row.
            }
        }
    }

    return prices;
}

}  // namespace quant
