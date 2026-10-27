#pragma once

#include <filesystem>

class SessionManager
{
public:
    static std::filesystem::path createSessionDirectory();
};
