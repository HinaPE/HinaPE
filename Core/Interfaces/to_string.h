#ifndef HINAPE_TO_STRING_H
#define HINAPE_TO_STRING_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

class ToString
{
public:
    std::string info_{};

    inline void log();

protected:
    ToString() = default;

    virtual void to_string() = 0;
};

void ToString::log()
{
    to_string();
    std::ofstream o(std::string(LOG_DIR) + std::string(typeid(*this).name()) + ".txt",
                    std::ios::binary | std::ios::trunc);
    o << info_ << std::endl;
    o.close();
}

#endif //HINAPE_TO_STRING_H
