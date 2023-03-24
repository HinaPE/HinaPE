#ifndef HINAPE_EXPORT_TO_VOL_H
#define HINAPE_EXPORT_TO_VOL_H

#include "common.h"

inline auto save_volume_as_tga(const HinaPE::Geom::ScalarGridField3 &grid, const std::string &filename)
{
	std::ofstream file(filename.c_str(), std::ofstream::binary);
	if (file)
	{
		auto size = grid.data_center.size();

		std::array<char, 18> header;
		header.fill(0);

		int img_width = static_cast<int>(size.x);
		int img_height = static_cast<int>(size.y);

		header[2] = 2; // uncompressed RGB
		header[12] = static_cast<char>(img_width & 0xFF);
		header[13] = static_cast<char>((img_width & 0xFF00) >> 8);
		header[14] = static_cast<char>(img_height & 0xFF);
		header[15] = static_cast<char>((img_height & 0xFF00) >> 8);
		header[16] = 24; // 24 bit bitmap

		file.write(header.data(), header.size());

		std::vector<std::vector<real>> hdr_img;
		hdr_img.resize(size.y);
		for (auto &row: hdr_img)
			row.resize(size.x);

		HinaPE::Util::parallelFor(HinaPE::Constant::ZeroSize, size.y, [&](size_t j)
		{
			for (size_t i = 0; i < size.x; ++i)
			{
				real sum = 0;
				for (size_t k = 0; k < size.z; ++k)
					sum += grid.data_center(i, j, k);
				hdr_img[j][i] = 2 * sum / static_cast<real>(size.z);
			}
		});

		std::vector<char> img(size.x * size.y * 3);
		for (size_t j = 0; j < size.y; ++j)
		{
			for (size_t i = 0; i < size.x; ++i)
			{
				real value = hdr_img[j][i];
				value = std::clamp(value, 0.0, 1.0);
				char gray = static_cast<char>(std::round(value * 255.0));
				img[(j * size.x + i) * 3 + 0] = gray;
				img[(j * size.x + i) * 3 + 1] = gray;
				img[(j * size.x + i) * 3 + 2] = gray;
			}
		}

		file.write(img.data(), static_cast<std::streamsize>(img.size()));
		file.close();
	}
}

//inline auto smooth_step(float edge0, float edge1, float x) -> float
//{
//	// Scale, bias and saturate x to 0..1 range
//	x = std::clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
//	// Evaluate polynomial
//	return x * x * (3 - 2 * x);
//}
//
//void save_volume_as_vol(const HinaPE::Geom::ScalarGrid3Ptr &grid, const std::string &filename)
//{
//	std::ofstream file(filename.c_str(), std::ofstream::binary);
//	if (file)
//	{
//		// Mitsuba 0.5.0 gridvolume format
//		char header[48];
//		memset(header, 0, sizeof(header));
//
//		header[0] = 'V';
//		header[1] = 'O';
//		header[2] = 'L';
//		header[3] = 3;
//		auto *encoding = reinterpret_cast<int32_t *>(header + 4);
//		encoding[0] = 1;  // 32-bit float
//		encoding[1] = static_cast<int32_t>(grid->data_size().x);
//		encoding[2] = static_cast<int32_t>(grid->data_size().y);
//		encoding[3] = static_cast<int32_t>(grid->data_size().z);
//		encoding[4] = 1;  // number of channels
//		mBBox3 domain = grid->bbox();
//		auto *bbox = reinterpret_cast<float *>(encoding + 5);
//		bbox[0] = static_cast<float>(domain._lower_corner.x());
//		bbox[1] = static_cast<float>(domain._lower_corner.y());
//		bbox[2] = static_cast<float>(domain._lower_corner.z());
//		bbox[3] = static_cast<float>(domain._upper_corner.x());
//		bbox[4] = static_cast<float>(domain._upper_corner.y());
//		bbox[5] = static_cast<float>(domain._upper_corner.z());
//
//		file.write(header, sizeof(header));
//		HinaPE::Math::Array3<float> data;
//		data.resize(grid->data_size());
//
//		const size_t edge_blur = 3;
//		const auto edge_blur_f = static_cast<float>(edge_blur);
//		data.parallel_for_each_index(
//				[&](size_t i, size_t j, size_t k)
//				{
//					auto d = static_cast<float>((*grid)(i, j, k));
//
//					// Blur the edge for less-noisy rendering
//					if (i < edge_blur)
//						d *= smooth_step(0.0f, edge_blur_f, static_cast<float>(i));
//					if (j < edge_blur)
//						d *= smooth_step(0.0f, edge_blur_f, static_cast<float>(j));
//					if (k < edge_blur)
//						d *= smooth_step(0.0f, edge_blur_f, static_cast<float>(k));
//					if (i > grid->data_size().x - 1 - edge_blur)
//						d *= smooth_step(0.0f, edge_blur_f, static_cast<float>(grid->data_size().x - 1 - i));
//					if (j > grid->data_size().y - 1 - edge_blur)
//						d *= smooth_step(0.0f, edge_blur_f, static_cast<float>(grid->data_size().y - 1 - j));
//					if (k > grid->data_size().z - 1 - edge_blur)
//						d *= smooth_step(0.0f, edge_blur_f, static_cast<float>(grid->data_size().z - 1 - k));
//
//					data(i, j, k) = d;
//				});
//
//		file.write(reinterpret_cast<const char *>(data.data()), static_cast<std::streamsize>(sizeof(float) * data.size().x * data.size().y * data.size().z));
//		file.close();
//	}
//}

#endif //HINAPE_EXPORT_TO_VOL_H
