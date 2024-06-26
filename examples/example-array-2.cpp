#include <librapid>
namespace lrc = librapid;
using namespace lrc::literals;

auto main() -> int {
	/*
	 * 1. Create two arrays. One 2x3 and one 3x1
	 * 2. Perform a matrix multiplication
	 * 3. Print the results
	 */

	std::vector<std::vector<float>> firstData  = {{1.0f, 2.0f, 3.0f}, {4.0f, 5.0f, 6.0f}};
	std::vector<std::vector<float>> secondData = {{1.0f}, {2.0f}, {3.0f}};

	auto firstMatrix  = lrc::Array<float>(firstData);
	auto secondMatrix = lrc::Array<float>(secondData);
	auto firstResult  = lrc::dot(firstMatrix, secondMatrix);
	fmt::print("Left:\n{}\n", firstMatrix);
	fmt::print("Right:\n{}\n", secondMatrix);
	fmt::print("Result:\n{}\n", firstResult);

	/*
	 * 4. Create a new array of shape 4x3, filled with random numbers in the range [-1, 1)
	 * 5. Perform a matrix multiplication with the transpose of `firstMatrix`
	 * 6. Print the results
	 */

	auto thirdMatrix  = lrc::random<float>(lrc::Shape({4, 3}), -1, 1);
	auto secondResult = lrc::dot(thirdMatrix, lrc::transpose(firstMatrix));
	fmt::print("Left:\n{:.2f}\n", thirdMatrix); // Format values to 2 decimal places
	fmt::print("Right:\n{}\n", lrc::transpose(firstMatrix));
	fmt::print("Result:\n{:.2f}\n", secondResult);

	return 0;
}
