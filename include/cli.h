#pragma once

#include <string>
#include <vector>
#include "restaurant.h"

namespace geo {

// CLI blueprint: parse user input and present options.
class CLI {
public:
    CLI() = default;

    // Run the command-line interface. (obviously)
    void run();

private:
    // helper methods for drawing menu/input parsing
    std::string prompt(const std::string& msg) const;
};

} // namespace geo
