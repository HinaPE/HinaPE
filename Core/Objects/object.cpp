#include "object.h"

using namespace HinaPE;

unsigned int Object::ID_GLOBAL = 0u;

void Object::assignID()
{
    ID_ = ID_GLOBAL++;
}

void Object::to_string()
{
    std::stringstream ss;

    ss << "========== POSITION ==========" << '\n';
    ss << "LENGTH: " << location_.rows() << '\n';
    ss << location_ << '\n';
    ss << "========== END ==========" << '\n';

    info_ = ss.str();
}