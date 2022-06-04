#pragma once

#include "../../internal/config.hpp"
#include "../../internal/forward.hpp"
#include "../traits.hpp"

namespace librapid::functors {
	template<typename Derived, typename OtherDerived, bool evalBeforeAssign>
	struct AssignSelector;

	template<typename Derived, typename OtherDerived>
	struct AssignSelector<Derived, OtherDerived, false> {
		static Derived &run(Derived &left, const OtherDerived &right) {
			return left.assignLazy(right);
		}
	};

	template<typename Derived, typename OtherDerived>
	struct AssignOp {
		LR_FORCE_INLINE static void run(Derived &dst, const OtherDerived &src) {
			constexpr bool dstIsHost =
			  std::is_same_v<typename internal::traits<Derived>::Device, device::CPU>;
			constexpr bool srcIsHost =
			  std::is_same_v<typename internal::traits<OtherDerived>::Device, device::CPU>;
			using Scalar	 = typename internal::traits<Derived>::Scalar;
			using BaseScalar = typename internal::traits<Derived>::BaseScalar;
			using Packet	 = typename internal::traits<Scalar>::Packet;

#if !defined(LIBRAPID_HAS_CUDA)
			static_assert(dstIsHost && srcIsHost, "CUDA support was not enabled");
#endif

			if constexpr (dstIsHost && srcIsHost) {
				int64_t packetWidth = internal::traits<Scalar>::PacketWidth;
				int64_t len			= dst.extent().size();
				int64_t alignedLen	= len - (len % packetWidth);
				if (alignedLen < 0) alignedLen = 0;

				if constexpr (std::is_same_v<Scalar, bool>) {
					len += 512;
					len /= 512;
					packetWidth = 1;
					alignedLen	= len;
				}

				// Only use a Packet type if possible
				if constexpr (!std::is_same_v<Packet, std::false_type> &&
							  (internal::traits<OtherDerived>::Flags &
							   internal::flags::SupportsPacket)) {
					// Use the entire packet width where possible
					if (numThreads < 2 || len < 500) {
						for (int64_t i = 0; i < alignedLen; i += packetWidth) {
							dst.loadFrom(i, src);
						}
					} else {
						// Multi-threaded approach
#pragma omp parallel for shared(dst, src, alignedLen, packetWidth) default(none)                   \
  num_threads(numThreads)
						for (int64_t i = 0; i < alignedLen; i += packetWidth) {
							dst.loadFrom(i, src);
						}
					}
				}

				// Ensure the remaining values are filled
				int64_t start = alignedLen;
				if constexpr (!(internal::traits<OtherDerived>::Flags &
								internal::flags::SupportsPacket))
					start = 0;
				for (int64_t i = start < 0 ? 0 : start; i < len; ++i) {
					dst.loadFromScalar(i, src);
				}
			} else {
#if defined(LIBRAPID_HAS_CUDA)
				// LR_LOG_STATUS("Size of Type: {}", sizeof(OtherDerived));
				static_assert(sizeof(OtherDerived) < (1 << 15), // Defines the max op size
							  "Calculation is too large to be run in a single call. Please call "
							  "eval() somewhere earlier");

				int64_t elems = src.extent().size();

				if constexpr (std::is_same_v<Scalar, bool>) {
					elems += 512;
					elems /= 512;
				}

				std::vector<BaseScalar *> arrays = {dst.storage().heap()};
				std::string scalarName			 = internal::traits<BaseScalar>::Name;
				int64_t index					 = 0;
				std::string microKernel			 = src.genKernel(arrays, index);

				std::string mainArgs;
				for (int64_t i = 0; i < index; ++i) {
					mainArgs += fmt::format("{} *{}{}", scalarName, "arg", i);
					if (i + 1 < index) mainArgs += ", ";
				}

				std::string functionArgs;
				for (int64_t i = 0; i < index; ++i) {
					functionArgs += fmt::format("{} arg{}", scalarName, i);
					if (i + 1 < index) functionArgs += ", ";
				}

				std::string indArgs;
				for (int64_t i = 0; i < index; ++i) {
					indArgs += fmt::format("arg{}[kernelIndex]", i);
					if (i + 1 < index) indArgs += ", ";
				}

				std::string varExtractor;
				for (int64_t i = 0; i < index; ++i)
					varExtractor +=
					  fmt::format("{} *arg{} = pointers[{}];\n\t", scalarName, i, i + 1);

				std::string varArgs;
				for (int64_t i = 0; i < index; ++i) {
					varArgs += fmt::format("src{}", i);
					if (i + 1 < index) varArgs += ", ";
				}

				std::string kernel = fmt::format(R"V0G0N(kernelOp
#include<stdint.h>

__device__
{4} function({0}) {{
	return {3};
}}

__global__
void applyOp({4} **pointers, int64_t size) {{
	const int64_t kernelIndex = blockDim.x * blockIdx.x + threadIdx.x;
	{4} *dst = pointers[0];
	{5}

	if (kernelIndex < size) {{
		dst[kernelIndex] = function({2});
	}}
}}
				)V0G0N",
												 functionArgs,
												 mainArgs,
												 indArgs,
												 microKernel,
												 scalarName,
												 varExtractor,
												 varArgs);

				static jitify::JitCache kernelCache;
				jitify::Program program = kernelCache.program(kernel, cudaHeaders, nvccOptions);

				int64_t threadsPerBlock, blocksPerGrid;

				// Use 1 to 512 threads per block
				if (elems < 512) {
					threadsPerBlock = elems;
					blocksPerGrid	= 1;
				} else {
					threadsPerBlock = 512;
					blocksPerGrid	= ceil(double(elems) / double(threadsPerBlock));
				}

				dim3 grid(blocksPerGrid);
				dim3 block(threadsPerBlock);

				// Copy the pointer array to the device
				BaseScalar **devicePointers = memory::malloc<BaseScalar *, device::GPU>(index + 1);
				memory::memcpy<BaseScalar *, device::GPU, BaseScalar *, device::CPU>(
				  devicePointers, &arrays[0], index + 1);

				jitifyCall(program.kernel("applyOp")
							 .instantiate()
							 .configure(grid, block, 0, memory::cudaStream)
							 .launch(devicePointers, elems));

				// Free device pointers
				memory::free<BaseScalar *, device::GPU>(devicePointers);
#endif
			}
		}
	};
} // namespace librapid::functors
