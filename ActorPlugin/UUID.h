#pragma once
#include <sstream>
#include <random>
#include <string>

class UUID
{
private:
    static unsigned int random_char()
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 255);
        return dis(gen);
    }

    static std::string generate_hex(const unsigned int len)
    {
        std::stringstream ss;
        for (unsigned int i = 0; i < len; i++) {
            const auto rc = random_char();
            std::stringstream hexstream;
            hexstream << std::hex << rc;
            auto hex = hexstream.str();
            ss << (hex.length() < 2 ? '0' + hex : hex);
        }
        return ss.str();
    }
public:
    static std::string CreateUUID()
    {
        return generate_hex(16);
    }
};

