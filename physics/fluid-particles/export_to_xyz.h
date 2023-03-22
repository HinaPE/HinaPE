#ifndef HINAPE_EXPORT_TO_XYZ_H
#define HINAPE_EXPORT_TO_XYZ_H

#include "common.h"
#include <fstream>

void save_particles_as_pos(real *ptr, size_t size, const std::string &filename)
{
	std::ofstream file(filename);
	for (size_t i = 0; i < size; ++i)
		file << ptr[i * 3 + 0] << " " << ptr[i * 3 + 1] << " " << ptr[i * 3 + 2] << std::endl;
	std::cout << "Saved " << size << " particles to " << filename << std::endl;
}

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

#ifdef WIN32
#define open _popen
#define close _pclose
#else
#define open popen
#define close pclose
#endif

// execute a terminal command
void exec(const char *cmd)
{
	std::array<char, 128> buffer{};
	std::string result;
	std::unique_ptr<FILE, decltype(&close)> pipe(open(cmd, "r"), close);
	if (!pipe)
		throw std::runtime_error("popen() failed!");
	while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
		result += buffer.data();
	std::cout << result << std::endl;
	std::cout << "complete!" << std::endl;
}

#endif //HINAPE_EXPORT_TO_XYZ_H
