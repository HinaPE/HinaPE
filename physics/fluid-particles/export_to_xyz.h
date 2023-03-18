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

auto exec(const char *cmd) -> std::string
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
	return result;
}

//if (argc < 2 || argc > 4) return EXIT_FAILURE;
//
//std::string inputFilename = std::string(argv[1]);
//size_t lastindex = inputFilename.find_last_of(".");
//std::string rawname = inputFilename.substr(0, lastindex);
//std::string outputFilename = rawname + ".obj";
//if (argc == 3) {
//outputFilename = std::string(argv[2]);
//}
//Size3 resolution(200, 200, 200);
//Vector3D gridSpacing(0.01, 0.01, 0.01);
//Vector3D origin = Vector3D(-1, -1, -1);
//std::string method = "anisotropic";
//double kernelRadius = 0.1;

#endif //HINAPE_EXPORT_TO_XYZ_H
