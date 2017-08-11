#include "logging.hpp"
#include <iostream>

namespace scenegraphdemo {
    // Logs a message of a specified log level to stdout.
    void log(std::string label, std::string message) {
        std::cout << "[" << label << "] - " << message << std::endl;
    }

    // Ad-hoc log functions.

    void info(std::string message) {
        log("INFO", message);
    }

    void debug(std::string message) {
        log("DEBUG", message);
    }

    void warn(std::string message) {
        log("WARN", message);
    }

    void error(std::string message) {
        log("ERROR", message);
    }
}
