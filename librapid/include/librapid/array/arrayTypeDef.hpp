#ifndef LIBRAPID_ARRAY_TYPE_DEF_HPP
#define LIBRAPID_ARRAY_TYPE_DEF_HPP

namespace librapid {
	namespace detail {
		template<typename Scalar, typename T>
		struct TypeDefStorageEvaluator {
			using Type = T;
		};

		template<typename Scalar>
		struct TypeDefStorageEvaluator<Scalar, backend::CPU> {
			using Type = Storage<Scalar>;
		};

		template<typename Scalar>
		struct TypeDefStorageEvaluator<Scalar, backend::OpenCL> {
			using Type = OpenCLStorage<Scalar>;
		};

		template<typename Scalar>
		struct TypeDefStorageEvaluator<Scalar, backend::CUDA> {
			using Type = CudaStorage<Scalar>;
		};
	} // namespace detail

	/// An easier to use definition than ArrayContainer. In this case, StorageType can be
	/// `backend::CPU`, `backend::CUDA` or any Storage interface
	/// \tparam Scalar The scalar type of the array.
	/// \tparam StorageType The storage type of the array.
	template<typename Scalar, typename StorageType = backend::CPU>
	using Array =
	  array::ArrayContainer<Shape,
							typename detail::TypeDefStorageEvaluator<Scalar, StorageType>::Type>;

	/// A definition for fixed-size array objects.
	/// \tparam Scalar The scalar type of the array.
	/// \tparam Dimensions The dimensions of the array.
	/// \see Array
	template<typename Scalar, size_t... Dimensions>
	using ArrayF = array::ArrayContainer<Shape, FixedStorage<Scalar, Dimensions...>>;

	/// A reference type for Array objects. Use this to accept Array objects as parameters since
	/// the compiler cannot determine the templates for the Array typedef. For more
	/// granularity, you can also accept a raw ArrayContainer object. \tparam StorageType The
	/// storage type of the array. \see Array \see ArrayF \see Function \see FunctionRef
	template<typename ShapeType, typename StorageType>
	using ArrayRef = array::ArrayContainer<ShapeType, StorageType>;

	template<typename Scalar, typename Backend = backend::CPU>
	using Matrix =
	  array::ArrayContainer<MatrixShape,
							typename detail::TypeDefStorageEvaluator<Scalar, Backend>::Type>;

	template<typename Scalar, size_t... Dimensions>
	using MatrixF = array::ArrayContainer<MatrixShape, FixedStorage<Scalar, Dimensions...>>;

	template<typename Scalar, typename Backend = backend::CPU>
	using Array1D =
	  array::ArrayContainer<VectorShape,
							typename detail::TypeDefStorageEvaluator<Scalar, Backend>::Type>;

	template<typename Scalar, size_t... Dimensions>
	using Array1DF = array::ArrayContainer<VectorShape, FixedStorage<Scalar, Dimensions...>>;

	/// A reference type for Array Function objects. Use this to accept Function objects as
	/// parameters since the compiler cannot determine the templates for the typedef by default.
	/// Additionally, this can be used to store references to Function objects.
	/// \tparam Inputs The argument types to the function (template...)
	/// \see Array
	/// \see ArrayF
	/// \see ArrayRef
	/// \see Function
	template<typename... Inputs>
	using FunctionRef = detail::Function<Inputs...>;

	namespace array {
		/// An intermediate type to represent a slice or view of an array.
		/// \tparam T The type of the array.
		template<typename T, typename ShapeType_>
		class GeneralArrayView;

		template<typename T>
		class Transpose;
	} // namespace array

	namespace linalg {
		template<typename ShapeTypeA, typename StorageTypeA, typename ShapeTypeB,
				 typename StorageTypeB, typename Alpha = typename StorageTypeA::Scalar,
				 typename Beta = typename StorageTypeB::Scalar>
		class ArrayMultiply;
	}

	template<typename T>
	using IsArrayType = std::integral_constant<
	  bool, (typetraits::TypeInfo<T>::type == detail::LibRapidType::ArrayContainer) ||
			  (typetraits::TypeInfo<T>::type == detail::LibRapidType::GeneralArrayView) ||
			  (typetraits::TypeInfo<T>::type == detail::LibRapidType::ArrayFunction)>;

#define ARRAY_TYPE_FMT_IML(TEMPLATE_, TYPE_)                                                       \
	template<TEMPLATE_>                                                                            \
	struct fmt::formatter<TYPE_> {                                                                 \
		using Type		= TYPE_;                                                                   \
		using Scalar	= typename librapid::typetraits::TypeInfo<Type>::Scalar;                   \
		using Formatter = fmt::formatter<Scalar>;                                                  \
		Formatter m_formatter;                                                                     \
		char m_bracket	 = 's';                                                                    \
		char m_separator = ' ';                                                                    \
		char m_formatString[16] {};                                                                \
                                                                                                   \
		template<typename ParseContext>                                                            \
		FMT_CONSTEXPR auto parse(ParseContext &ctx) -> const char * {                              \
			/* Custom format options:               */                                             \
			/*  - "~r" for round brackets           */                                             \
			/*  - "~s" for square brackets          */                                             \
			/*  - "~c" for curly brackets           */                                             \
			/*  - "~a" for angle brackets           */                                             \
			/*  - "~p" for pipe brackets            */                                             \
			/*  - "-," for comma separator          */                                             \
			/*  - "-;" for semicolon separator      */                                             \
			/*  - "-:" for colon separator          */                                             \
			/*  - "-|" for pipe separator           */                                             \
			/*  - "-_" for underscore separator     */                                             \
                                                                                                   \
			auto it			= ctx.begin();                                                         \
			const char *ret = m_formatter.parse(ctx);                                              \
			int index		= 0;                                                                   \
			for (auto itt = it; itt != ret; itt++) { m_formatString[index++] = *itt; }             \
			return ret;                                                                            \
		}                                                                                          \
                                                                                                   \
		template<typename FormatContext>                                                           \
		FMT_CONSTEXPR auto format(const Type &val, FormatContext &ctx) const                       \
		  -> decltype(ctx.out()) {                                                                 \
			val.str(m_formatter, m_bracket, m_separator, m_formatString, ctx);                     \
			return ctx.out();                                                                      \
		}                                                                                          \
	};                                                                                             \
                                                                                                   \
	template<TEMPLATE_>                                                                            \
	auto operator<<(std::ostream &os, const TYPE_ &object) -> std::ostream & {                     \
		os << fmt::format("{}", object);                                                           \
		return os;                                                                                 \
	}
} // namespace librapid

#endif // LIBRAPID_ARRAY_TYPE_DEF_HPP