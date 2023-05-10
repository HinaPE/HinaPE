//! # Example 5: ND-Array
#include <cassert>
#include <cstdint>
#include <ios>
#include <iostream>
#include <iomanip>
#include <taichi/cpp/taichi.hpp>

int main(int argc, const char **argv)
{
	setenv("TI_LIB_DIR", "/opt/homebrew/lib/python3.10/site-packages/taichi/_lib/runtime", 1);

	ti::Runtime runtime(ti::get_available_archs().front());

	/// Compared with `ti::Memory`, ND-array provides a more structured view over
	/// the underlying memory.
	///
	/// ND-array is a multi dimensional dense matrix, similar to `ndarray` in
	/// NumPy or `Tensor` in PyTorch. But they are not exactly the same. Because
	/// in simulation and graphics we not only operate on scalars but more often
	/// vectors and matrices, Taichi's ND-array thus have generalized the concept
	/// of Tensor element. A Tensor can not only contain scalar elements but also
	/// vectors and matrices, leading to its `elem_shape` attribute to represent
	/// the dimension of the shape of each element.
	///
	/// You can allocate a host-accessible 4x4 array of 2-component vectors like
	/// this:
	ti::NdArray<float> arr = runtime.allocate_ndarray<float>({4, 4}, {2}, true);

	{
		/// You can access host-accessible ND-array data from the CPU with `read()`
		/// and `write()`.
		///
		/// Note that ND-array has the same host-accessibility restriction as raw
		/// memory allocations. Accessing non-host-accessible ND-arrays from the CPU
		/// can lead to unrecoverable program termination (usually segfault).
		std::vector<float> canvas(4 * 4 * 2);
		for (size_t h = 0; h < 4; ++h)
		{
			for (size_t w = 0; w < 4; ++w)
			{
				canvas[(h * 4 + w) * 2 + 0] = (w + 0.5f) / 4.0f;
				canvas[(h * 4 + w) * 2 + 1] = (h + 0.5f) / 4.0f;
			}
		}
		arr.write(canvas);
	}

	/// To help communicating vectors and matrices between the CPU and the GPU,
	/// you can directly `read()` and `write()` composite types (structures) from
	/// `ti::NdArray`, as long as the composite type sizes are multiples of ND-
	/// array scalar types.
	struct Vec2
	{
		float x;
		float y;
	};
	static_assert(sizeof(Vec2) == sizeof(uint32_t) * 2, "mismatched size");

	std::cout << "ndarray data:" << std::setprecision(3) << std::fixed << std::endl;
	std::vector<Vec2> canvas2(4 * 4);
	arr.read(canvas2);
	for (size_t h = 0; h < 4; ++h)
	{
		for (size_t w = 0; w < 4; ++w)
		{
			const Vec2 &vec = canvas2[h * 4 + w];
			std::cout << "(" << vec.x << "," << vec.y << ")" << " ";
		}
		std::cout << std::endl;
	}
}
