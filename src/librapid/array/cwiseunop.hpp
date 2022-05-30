#pragma once

#include "../internal/config.hpp"
#include "../internal/forward.hpp"
#include "arrayBase.hpp"

namespace librapid {
	namespace internal {
		template<typename Unop, typename TYPE>
		struct traits<unop::CWiseUnop<Unop, TYPE>> {
			using Valid						= std::true_type;
			using Type						= unop::CWiseUnop<Unop, TYPE>;
			using Scalar					= typename Unop::RetType;
			using BaseScalar				= typename traits<Scalar>::BaseScalar;
			using Packet					= typename traits<Scalar>::Packet;
			using Device					= typename traits<TYPE>::Device;
			using StorageType				= memory::DenseStorage<Scalar, Device>;
			static constexpr uint64_t Flags = Unop::Flags;
		};
	} // namespace internal

	namespace unop {
		template<typename Unop, typename TYPE>
		class CWiseUnop
				: public ArrayBase<CWiseUnop<Unop, TYPE>, typename internal::traits<TYPE>::Device> {
		public:
			using Operation					= Unop;
			using Scalar					= typename Unop::RetType;
			using Packet					= typename internal::traits<Scalar>::Packet;
			using ValType					= typename internal::StripQualifiers<TYPE>;
			using Device					= typename internal::traits<TYPE>::Device;
			using Type						= CWiseUnop<Unop, TYPE>;
			using Base						= ArrayBase<Type, Device>;
			static constexpr uint64_t Flags = internal::traits<Type>::Flags;

			CWiseUnop() = delete;

			explicit CWiseUnop(const ValType &value) : Base(value.extent(), 0), m_value(value) {}

			CWiseUnop(const Type &op) :
					Base(op.extent(), 0), m_value(op.m_value), m_operation(op.m_operation) {}

			template<typename T>
			CWiseUnop &operator=(const T &op) {
				static_assert(
				  std::is_same_v<T, Type>,
				  "Lazy-evaluated result cannot be assigned a different type. Please either "
				  "evaluate the result (using 'eval()') or create a new variable");

				if (this == &op) return *this;

				Base::m_extent = op.m_extent;

				m_value		= op.m_value;
				m_operation = op.m_operation;

				return *this;
			}

			LR_NODISCARD("Do not ignore the result of an evaluated calculation")
			Array<Scalar, Device> eval() const {
				Array<Scalar, Device> res(Base::extent());
				res.assign(*this);
				return res;
			}

			LR_FORCE_INLINE Packet packet(int64_t index) const {
				return m_operation.packetOp(m_value.packet(index));
			}

			LR_FORCE_INLINE Scalar scalar(int64_t index) const {
				return m_operation.scalarOp(m_value.scalar(index));
			}

			template<typename T>
			std::string genKernel(std::vector<T> &vec, int64_t &index) const {
				std::string kernel = m_value.genKernel(vec, index);
				std::string op	   = m_operation.genKernel();
				return fmt::format("({}{})", op, kernel);
			}

		protected:
			ValType m_value;
			Operation m_operation {};
		};
	} // namespace unop
} // namespace librapid