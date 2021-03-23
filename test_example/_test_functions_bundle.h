#ifndef ASSIGNMENT0__TEST_FUNCTIONS_BUNDLE_H
#define ASSIGNMENT0__TEST_FUNCTIONS_BUNDLE_H

#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <iterator>
#include <iostream>

#include "Shader.h"

template <typename T>
std::string vector_to_string(std::vector<T> vec) {
    std::ostringstream oss;
    oss << '[';
    if (!vec.empty())
    {
        // Convert all but the last element to avoid a trailing ","
        std::copy(vec.begin(), vec.end()-1,
                  std::ostream_iterator<T>(oss, ","));

        // Now add the last element with no delimiter
        oss << vec.back();
    }
    oss << ']';
    return oss.str();
};

#endif //ASSIGNMENT0__TEST_FUNCTIONS_BUNDLE_H
