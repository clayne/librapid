#ifndef LIBRAPID_CORE_LIBRAPID_PCH_HPP
#define LIBRAPID_CORE_LIBRAPID_PCH_HPP

/*
 * Include standard library headers and precompile them as part of
 * librapid. This reduces compile times dramatically.
 *
 * Additionally, include the header files of dependencies.
 */

// Standard Library
#include <algorithm>
#include <array>
#include <atomic>
#include <cfenv>
#include <cfloat>
#include <cmath>
#include <cmath>
#include <complex>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstdlib>
#include <fstream>
#include <future>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <memory>
#include <mutex>
#include <numeric>
#include <queue>
#include <random>
#include <regex>
#include <set>

#if defined(_WIN32) || defined(_WIN64)
#	define WIN32_LEAN_AND_MEAN
#	include <Windows.h>
#endif

// Remove a few macros
#undef min
#undef max

// fmtlib
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fmt/chrono.h>
#include <fmt/compile.h>
#include <fmt/color.h>
#include <fmt/os.h>
#include <fmt/ostream.h>
#include <fmt/printf.h>
#include <fmt/xchar.h>

// scnlib
#include <scn/scn.h>
#include <scn/tuple_return/tuple_return.h>

// Vc -- SIMD instructions
#include <Vc/Vc>
#include <Vc/algorithm>
#include <Vc/cpuid.h>

#endif // LIBRAPID_CORE_LIBRAPID_PCH_HPP