#pragma once

#include <string>

namespace scenegraphdemo {
    // Logs a message of a specified log level to stdout.
    void log(std::string label, std::string message);

    // Ad-hoc log functions.

    void info(std::string message);
    void debug(std::string message);
    void warn(std::string message);
    void error(std::string message);
}
