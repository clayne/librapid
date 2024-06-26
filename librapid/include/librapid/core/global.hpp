#ifndef LIBRAPID_CORE_GLOBAL_HPP
#define LIBRAPID_CORE_GLOBAL_HPP

/*
 * Global variables required for LibRapid, such as version number, number of threads,
 * CUDA-related configuration, etc.
 */

namespace librapid {
    namespace global {
        // Should ASSERT functions print their message to stdout?
        extern bool printOnAssert;

        /// Arrays with more elements than this will run with multithreaded implementations
        extern size_t multithreadThreshold;

        // Number of columns required for a matrix to be parallelized in GEMM
        extern size_t gemmMultithreadThreshold;

        // Number of columns required for a matrix to be parallelized in GEMV
        extern size_t gemvMultithreadThreshold;

        // Number of threads used by LibRapid
        extern size_t numThreads;

        // Random seed used by LibRapid (when changed, the random number generator is reseeded)
        extern size_t randomSeed;

        // Should the random number generator be reseeded?
        extern bool reseed;

        // Size of a cache line in bytes
        extern size_t cacheLineSize;

#if defined(LIBRAPID_HAS_OPENCL)
        // OpenCL device list
        extern std::vector<cl::Device> openclDevices;

        // OpenCL context
        extern cl::Context openCLContext;

        // OpenCL device
        extern cl::Device openCLDevice;

        // OpenCL command queue
        extern cl::CommandQueue openCLQueue;

        // OpenCL program sources
        extern cl::Program::Sources openCLSources;

        // OpenCL program
        extern cl::Program openCLProgram;

        // True if OpenCL has been configured
        extern bool openCLConfigured;
#endif // LIBRAPID_HAS_OPENCL

#if defined(LIBRAPID_HAS_CUDA)
        // LibRapid's CUDA stream -- this removes the need for calling cudaDeviceSynchronize()
        extern cudaStream_t cudaStream;

        // LibRapid's CuBLAS handle
        extern cublasHandle_t cublasHandle;

        // LibRapid's CuBLASLt handle
        extern cublasLtHandle_t cublasLtHandle;

        extern uint64_t cublasLtWorkspaceSize;

        // LibRapid's CuBLASLt workspace
        extern void *cublasLtWorkspace;

        // Jitify cache for CUDA kernels
        extern jitify::JitCache jitCache;
#endif // LIBRAPID_HAS_CUDA
    }  // namespace global

    void setNumThreads(size_t numThreads);
    size_t getNumThreads();

    void setSeed(size_t seed);
    size_t getSeed();
} // namespace librapid

#endif // LIBRAPID_CORE_GLOBAL_HPP