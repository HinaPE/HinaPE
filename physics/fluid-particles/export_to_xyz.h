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

#endif //HINAPE_EXPORT_TO_XYZ_H
