
#include "SessionManager.h"

#include <iomanip>
#include <sstream>
#include <stdexcept>

std::filesystem::path SessionManager::createSessionDirectory()
{
    const std::filesystem::path root = "results";
    std::filesystem::create_directories(root);

    for (int id = 1; id <= 9999; ++id)
    {
        std::ostringstream name;
        name << "session_" << std::setw(3) << std::setfill('0') << id;

        const auto path = root / name.str();

        if (std::filesystem::create_directory(path))
        {
            return path;
        }
    }

    throw std::runtime_error("Cannot create a new session directory.");
}
