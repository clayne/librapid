#ifndef LIBRAPID_CORE_CONFIG_HPP
#define LIBRAPID_CORE_CONFIG_HPP

/*
 * This file defines a few macros and includes other files, depending on the
 * compiler/system being used.
 */

// Include the precompiled header
#include "librapidPch.hpp"

// Detect Release vs Debug builds
#if !defined(NDEBUG)
#	define LIBRAPID_DEBUG
#else
#	define LIBRAPID_RELEASE
#endif

// Detect the operating system
#if defined(_WIN32)
#	define LIBRAPID_WINDOWS // Windows
#	define LIBRAPID_OS_NAME "windows"
#elif defined(_WIN64)
#	define LIBRAPID_WINDOWS // Windows
#	define LIBRAPID_OS_NAME "windows"
#elif defined(__CYGWIN__) && !defined(_WIN32)
#	define LIBRAPID_WINDOWS // Windows (Cygwin POSIX under Microsoft Window)
#	define LIBRAPID_OS_NAME "windows"
#elif defined(__ANDROID__)
#	define LIBRAPID_ANDROID // Android (implies Linux, so it must come first)
#	define LIBRAPID_OS_NAME "android"
#elif defined(__linux__)
#	define LIBRAPID_LINUX // Debian, Ubuntu, Gentoo, Fedora, openSUSE, RedHat, Centos and other
#	define LIBRAPID_UNIX
#	define LIBRAPID_OS_NAME "linux"
#elif defined(__unix__) || !defined(__APPLE__) && defined(__MACH__)
#	include <sys/param.h>
#	if defined(BSD)
#		define LIBRAPID_BSD // FreeBSD, NetBSD, OpenBSD, DragonFly BSD
#		define LIBRAPID_UNIX
#		define LIBRAPID_OS_NAME "bsd"
#	endif
#elif defined(__hpux)
#	define LIBRAPID_HP_UX	 // HP-UX
#	define LIBRAPID_OS_NAME "hp-ux"
#elif defined(_AIX)
#	define LIBRAPID_AIX	 // IBM AIX
#	define LIBRAPID_OS_NAME "aix"
#elif defined(__APPLE__) && defined(__MACH__) // Apple OSX and iOS (Darwin)
#	define LIBRAPID_APPLE
#	define LIBRAPID_UNIX
#	include <TargetConditionals.h>
#	if TARGET_IPHONE_SIMULATOR == 1
#		define LIBRAPID_IOS	 // Apple iOS
#		define LIBRAPID_OS_NAME "ios"
#	elif TARGET_OS_IPHONE == 1
#		define LIBRAPID_IOS	 // Apple iOS
#		define LIBRAPID_OS_NAME "ios"
#	elif TARGET_OS_MAC == 1
#		define LIBRAPID_OSX	 // Apple OSX
#		define LIBRAPID_OS_NAME "osx"
#	endif
#elif defined(__sun) && defined(__SVR4)
#	define LIBRAPID_SOLARIS // Oracle Solaris, Open Indiana
#	define LIBRAPID_OS_NAME "solaris"
#else
#	define LIBRAPID_UNKNOWN
#	define LIBRAPID_OS_NAME "unknown"
#endif

// Compiler information
#if defined(__GNUC__) && !defined(__clang__) && !defined(__INTEL_COMPILER)
#	define LIBRAPID_GNU
#	define LIBRAPID_COMPILER_NAME "GNU C/C++ Compiler"
#endif

#if defined(__MINGW32__)
#	define LIBRAPID_MINGW
#	define LIBRAPID_COMPILER_NAME "Mingw or GNU C/C++ Compiler ported for Windows NT"
#endif

#if defined(__MINGW64__)
#	define LIBRAPID_MINGW
#	define LIBRAPID_COMPILER_NAME                                                                 \
		"Mingw or GNU C/C++ Compiler ported for Windows NT - 64 bits only"
#endif

#if defined(__GFORTRAN__)
#	define LIBRAPID_FORTRAN
#	define LIBRAPID_COMPILER_NAME "Fortran / GNU Fortran Compiler"
#endif

#if defined(__clang__) && !defined(_MSC_VER)
#	define LIBRAPID_CLANG
#	define LIBRAPID_COMPILER_NAME "Clang / LLVM Compiler"
#endif

#if defined(_MSC_VER)
#	define LIBRAPID_MSVC
#	define LIBRAPID_COMPILER_NAME "Microsoft Visual Studio Compiler MSVC"
#endif

#if defined(_MANAGED) || defined(__cplusplus_cli)
#	define LIBRAPID_DOTNET
#	define LIBRAPID_COMPILER_NAME "Compilation to C++/CLI .NET (CLR) bytecode"
#endif

#if defined(__INTEL_COMPILER)
#	define LIBRAPID_INTEL
#	define LIBRAPID_COMPILER_NAME "Intel C/C++ Compiler"
#endif

#if defined(__PGI) || defined(__PGIC__)
#	define LIBRAPID_PORTLAND
#	define LIBRAPID_COMPILER_NAME "Portland Group C/C++ Compiler"
#endif

#if defined(__BORLANDC__)
#	define LIBRAPID_BORLAND
#	define LIBRAPID_COMPILER_NAME "Borland C++ Compiler"
#endif

#if defined(__EMSCRIPTEN__)
#	define LIBRAPID_EMSCRIPTEN
#	define LIBRAPID_COMPILER_NAME "emscripten (asm.js - web assembly)"
#endif

#if defined(__asmjs__)
#	define LIBRAPID_ASMJS
#	define LIBRAPID_COMPILER_NAME "asm.js"
#endif

#if defined(__wasm__)
#	define LIBRAPID_WASM
#	define LIBRAPID_COMPILER_NAME "WebAssembly"
#endif

#if defined(__NVCC__)
#	define LIBRAPID_NVCC
#	define LIBRAPID_COMPILER_NAME "NVIDIA NVCC CUDA Compiler"
#endif

#if defined(__CLING__)
#	define LIBRAPID_CLING
#	define LIBRAPID_COMPILER_NAME "CERN's ROOT Cling C++ Interactive Shell"
#endif

// Instruction sets
#define ARCH_AVX512_2 11
#define ARCH_AVX512	  10
#define ARCH_AVX2	  9
#define ARCH_AVX	  8
#define ARCH_SSE4_2	  7
#define ARCH_SSE4_1	  6
#define ARCH_SSSE3	  5
#define ARCH_SSE3	  4
#define ARCH_SSE2	  3
#define ARCH_SSE	  2
#define ARCH_NEON	  1
#define ARCH_NONE	  0

// Instruction set detection
#if defined(__AVX512VL__) && defined(__AVX512BW__) && defined(__AVX512DQ__)
#	define LIBRAPID_AVX512
#	define LIBRAPID_ARCH	   ARCH_AVX512_2
#	define LIBRAPID_ARCH_NAME "AVX512"

#	ifndef LIBRAPID_MEM_ALIGN
#		define LIBRAPID_MEM_ALIGN 64
#	endif
#elif defined(__AVX512F__) || defined(__AVX512__)
#	define LIBRAPID_AVX512
#	define LIBRAPID_ARCH	   ARCH_AVX512
#	define LIBRAPID_ARCH_NAME "AVX512"

#	ifndef LIBRAPID_MEM_ALIGN
#		define LIBRAPID_MEM_ALIGN 64
#	endif
#elif defined(__AVX2__)
#	define LIBRAPID_AVX2
#	define LIBRAPID_ARCH	   ARCH_AVX2
#	define LIBRAPID_ARCH_NAME "AVX2"

#	ifndef LIBRAPID_MEM_ALIGN
#		define LIBRAPID_MEM_ALIGN 32
#	endif
#elif defined(__AVX__)
#	define LIBRAPID_AVX
#	define LIBRAPID_ARCH	   ARCH_AVX
#	define LIBRAPID_ARCH_NAME "AVX"

#	ifndef LIBRAPID_MEM_ALIGN
#		define LIBRAPID_MEM_ALIGN 32
#	endif
#elif defined(__SSE4_2__)
#	define LIBRAPID_SSE42
#	define LIBRAPID_ARCH	   ARCH_SSE4_2
#	define LIBRAPID_ARCH_NAME "SSE4.2"

#	ifndef LIBRAPID_MEM_ALIGN
#		define LIBRAPID_MEM_ALIGN 16
#	endif
#elif defined(__SSE4_1__)
#	define LIBRAPID_SSE41
#	define LIBRAPID_ARCH	   ARCH_SSE4_1
#	define LIBRAPID_ARCH_NAME "SSE4.1"

#	ifndef LIBRAPID_MEM_ALIGN
#		define LIBRAPID_MEM_ALIGN 16
#	endif
#elif defined(__SSSE3__)
#	define LIBRAPID_SSSE3
#	define LIBRAPID_ARCH	   ARCH_SSSE3
#	define LIBRAPID_ARCH_NAME "SSSE3"

#	ifndef LIBRAPID_MEM_ALIGN
#		define LIBRAPID_MEM_ALIGN 16
#	endif
#elif defined(__SSE3__)
#	define LIBRAPID_SSE3
#	define LIBRAPID_ARCH	   ARCH_SSE3
#	define LIBRAPID_ARCH_NAME "SSE3"

#	ifndef LIBRAPID_MEM_ALIGN
#		define LIBRAPID_MEM_ALIGN 16
#	endif
#elif defined(__SSE2__) || defined(__x86_64__)
#	define LIBRAPID_SSE2
#	define LIBRAPID_ARCH	   ARCH_SSE2
#	define LIBRAPID_ARCH_NAME "SSE2"

#	ifndef LIBRAPID_MEM_ALIGN
#		define LIBRAPID_MEM_ALIGN 16
#	endif
#elif defined(__SSE__)
#	define LIBRAPID_SSE
#	define LIBRAPID_ARCH	   ARCH_SSE
#	define LIBRAPID_ARCH_NAME "SSE"

#	ifndef LIBRAPID_MEM_ALIGN
#		define LIBRAPID_MEM_ALIGN 16
#	endif
#elif defined(__ARM_NEON) || defined(__ARM_NEON__)
#	define LIBRAPID_NEON
#	define LIBRAPID_ARCH	   ARCH_NEON
#	define LIBRAPID_ARCH_NAME "NEON"

#	ifndef LIBRAPID_MEM_ALIGN
#		define LIBRAPID_MEM_ALIGN 16
#	endif
#elif defined(__ARM_FEATURE_SVE)
#	define LIBRAPID_SVE
#	define LIBRAPID_ARCH	   ARCH_SVE
#	define LIBRAPID_ARCH_NAME "SVE"

#	ifndef LIBRAPID_MEM_ALIGN
#		define LIBRAPID_MEM_ALIGN 128 // SVE has a max vector length of 2048 bits
#	endif
#elif defined(_M_IX86_FP) // Defined in MS compiler. 1: SSE, 2: SSE2
#	if _M_IX86_FP == 1
#		define LIBRAPID_SSE
#		define LIBRAPID_ARCH	   ARCH_SSE
#		define LIBRAPID_ARCH_NAME "SSE"

#		ifndef LIBRAPID_MEM_ALIGN
#			define LIBRAPID_MEM_ALIGN 16
#		endif
#	elif _M_IX86_FP == 2
#		define LIBRAPID_SSE2
#		define LIBRAPID_ARCH	   ARCH_SSE2
#		define LIBRAPID_ARCH_NAME "SSE2"

#		ifndef LIBRAPID_MEM_ALIGN
#			define LIBRAPID_MEM_ALIGN 16
#		endif
#	endif // _M_IX86_FP
#else
#	define LIBRAPID_ARCH	   ARCH_NONE
#	define LIBRAPID_ARCH_NAME "None"

#	ifndef LIBRAPID_MEM_ALIGN
#		define LIBRAPID_MEM_ALIGN 1
#	endif
#endif // Instruction set detection

// Check for 32bit vs 64bit
#if _WIN32 || _WIN64 // Check windows
#	if _WIN64
#		define LIBRAPID_64BIT
#	else
#		define LIBRAPID_32BIT
#	endif
#elif __GNUC__
#	if __x86_64__ || __ppc64__
#		define LIBRAPID_64BIT
#	else
#		define LIBRAPID_32BIT
#	endif
#else
#	LIBRAPID_64BIT // Default to 64bit
#endif

// C++ Version
#if defined(_MSVC_LANG) && _MSVC_LANG >= 202004L
#	define LIBRAPID_CPP23
#	define LIBRAPID_CPP_VERSION 23
#elif defined(__cplusplus) && __cplusplus >= 202004L
#	define LIBRAPID_CPP23
#	define LIBRAPID_CPP_VERSION 23
#elif defined(_MSVC_LANG) && _MSVC_LANG >= 202002L
#	define LIBRAPID_CPP20
#	define LIBRAPID_CPP_VERSION 20
#elif defined(__cplusplus) && __cplusplus >= 202002L
#	define LIBRAPID_CPP20
#	define LIBRAPID_CPP_VERSION 20
#elif defined(_MSVC_LANG) && _MSVC_LANG >= 201703L
#	define LIBRAPID_CPP17
#	define LIBRAPID_CPP_VERSION 17
#elif defined(__cplusplus) && __cplusplus >= 201703L
#	define LIBRAPID_CPP17
#	define LIBRAPID_CPP_VERSION 17
#elif defined(_MSVC_LANG) && _MSVC_LANG >= 201402L
#	define LIBRAPID_CPP14
#	define LIBRAPID_CPP_VERSION 14
#elif defined(__cplusplus) && __cplusplus >= 201402L
#	define LIBRAPID_CPP14
#	define LIBRAPID_CPP_VERSION 14
#elif defined(_MSVC_LANG) && _MSVC_LANG >= 201103L
#	define LIBRAPID_CPP11
#	define LIBRAPID_CPP_VERSION 11
#elif defined(__cplusplus) && __cplusplus >= 201103L
#	define LIBRAPID_CPP11
#	define LIBRAPID_CPP_VERSION 11
#endif

// Branch prediction hints
#if LIBRAPID_CPP_VERSION >= 20
#	define LIBRAPID_LIKELY	  [[likely]]
#	define LIBRAPID_UNLIKELY [[unlikely]]
#else
#	define LIBRAPID_LIKELY
#	define LIBRAPID_UNLIKELY
#endif

#if LIBRAPID_CPP_VERSION >= 23
#	if defined(LIBRAPID_MSVC)
#		define LIBRAPID_ASSUME(COND_) __assume(COND_)
#	elif defined(LIBRAPID_GCC)
#		define LIBRAPID_ASSUME(COND_) __attribute__((assume(COND_)))
#	elif defined(LIBRAPID_CLANG)
#		define LIBRAPID_ASSUME(COND_) __builtin_assume(COND_)
#	else
#		define LIBRAPID_ASSUME(COND_)
#	endif
#else
#	define LIBRAPID_ASSUME(COND_)
#endif

#if LIBRAPID_CPP_VERSION >= 20
#	define LIBRAPID_ASSUME_ALIGNED(PTR_) std::assume_aligned<LIBRAPID_MEM_ALIGN>(PTR_)
#else
#	define LIBRAPID_ASSUME_ALIGNED(PTR_) PTR_
#endif

// [[nodiscard]] macro
#define LIBRAPID_NODISCARD [[nodiscard]]

// Nicer FILENAME macro
#if defined(FILENAME)
#	warning                                                                                        \
	  "The macro 'FILENAME' is already defined. LibRapid's logging system might not function correctly as a result"
#else
#	ifdef LIBRAPID_OS_WINDOWS
#		define FILENAME (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#	else
#		define FILENAME (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#	endif
#endif

// Nicer FUNCTION macro
#if defined(FUNCTION)
#	warning                                                                                        \
	  "The macro 'FUNCTION' is already defined. LibRapid's logging system might not function correctly as a result"
#else
#	if defined(LIBRAPID_MSVC)
#		define FUNCTION __FUNCSIG__
#	elif defined(LIBRAPID_GNU) || defined(LIBRAPID_CLANG) || defined(LIBRAPID_CLING)
#		define FUNCTION __PRETTY_FUNCTION__
#	else
#		define FUNCTION "Function Signature Unknown"
#	endif
#endif

// STRINGIFY
#define STRINGIFY(a) STR_IMPL_(a)
#define STR_IMPL_(a) #a

// Assertions, warnings and errors
#if defined(LIBRAPID_DEBUG) && !defined(LIBRAPID_ENABLE_ASSERT)
#	define LIBRAPID_ENABLE_ASSERT
#endif // LIBRAPID_DEBUG && !LIBRAPID_ASSERT

// Warn the user the first time this is called, but never again
#if defined(LIBRAPID_ASSERT)
#	define LIBRAPID_WARN_ONCE(msg, ...)                                                           \
		do {                                                                                       \
			static bool _alerted = false;                                                          \
			if (!_alerted) {                                                                       \
				LIBRAPID_WARN(msg, __VA_ARGS__);                                                   \
				_alerted = true;                                                                   \
			}                                                                                      \
		} while (false)
#endif // LIBRAPID_ASSERT

#if defined(LIBRAPID_ENABLE_ASSERT) || defined(LIBRAPID_DEBUG)
#	define LIBRAPID_NOT_IMPLEMENTED LIBRAPID_ASSERT(false, "Not implemented");
#else
#	define LIBRAPID_NOT_IMPLEMENTED throw std::runtime_error("Not implemented");
#endif

// Compiler-specific attributes
#if defined(LIBRAPID_MSVC)
#	include "msvcConfig.hpp"
#elif defined(LIBRAPID_GNU) || defined(LIBRAPID_CLANG) || defined(LIBRAPID_CLING)
#	include "gnuConfig.hpp"
#else
#	include "genericConfig.hpp"
#endif

#if defined(LIBRAPID_HAS_CUDA)
#	include "cudaConfig.hpp"
#else
namespace librapid::typetraits {
	template<typename T>
	struct IsCudaStorage : std::false_type {};
} // namespace librapid::typetraits
#endif

#if defined(LIBRAPID_HAS_OPENCL)
#	include "openclConfig.hpp"
#else
namespace librapid::typetraits {
	template<typename T>
	struct IsOpenCLStorage : std::false_type {};
} // namespace librapid::typetraits
#endif

namespace librapid::backend {
	// Use the CPU for computation (default)
	struct CPU {};

	// Use the GPU via CUDA
	struct CUDA {};

	// Use OpenCL
	struct OpenCL {};

	// Use the fastest device for computation
#if defined(LIBRAPID_HAS_CUDA)
	using Fastest = CUDA;
#elif defined(LIBRAPID_HAS_OPENCL)
	using Fastest = OpenCL;
#else
	using Fastest = CPU;
#endif

	// GPU if available, CPU otherwise
#if defined(LIBRAPID_HAS_CUDA)
	using CUDAIfAvailable = CUDA;
#else
	using CUDAIfAvailable = CPU;
#endif

	// OpenCL if available, CPU otherwise
#if defined(LIBRAPID_HAS_OPENCL)
	using OpenCLIfAvailable = OpenCL;
#else
	using OpenCLIfAvailable = CPU;
#endif
} // namespace librapid::backend

#ifndef LIBRAPID_MAX_ARRAY_DIMS
#	define LIBRAPID_MAX_ARRAY_DIMS 16
#endif // LIBRAPID_MAX_ARRAY_DIMS

// Code to be run *before* main()
#include "preMain.hpp"

#endif // LIBRAPID_CORE_CONFIG_HPP
