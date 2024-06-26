#ifndef LIBRAPID_ARRAY_SIZETYPE_HPP
#define LIBRAPID_ARRAY_SIZETYPE_HPP

/*
 * This file defines the Shape class and some helper functions,
 * including stride operations.
 */

namespace librapid {
	namespace typetraits {
		LIBRAPID_DEFINE_AS_TYPE_NO_TEMPLATE(Shape);
		LIBRAPID_DEFINE_AS_TYPE_NO_TEMPLATE(MatrixShape);
		LIBRAPID_DEFINE_AS_TYPE_NO_TEMPLATE(VectorShape);
	} // namespace typetraits

	class Shape {
	public:
		using SizeType						  = uint32_t;
		using DimType						  = int16_t;
		static constexpr size_t MaxDimensions = LIBRAPID_MAX_ARRAY_DIMS;

		/// Default constructor
		LIBRAPID_ALWAYS_INLINE Shape() = default;

		/// Create a shape object from the dimensions of a FixedStorage object. This is used
		// mainly internally, but may serve some purpose I haven't yet thought of.
		/// \tparam Scalar Scalar type of the FixedStorage object
		/// \tparam Dimensions Dimensions of the FixedStorage object
		/// \param fixed The FixedStorage object
		template<typename Scalar, size_t... Dimensions>
		explicit LIBRAPID_ALWAYS_INLINE Shape(const FixedStorage<Scalar, Dimensions...> &fixed);

		/// Create a Shape object from a list of values
		/// \tparam V Scalar type of the values
		/// \param vals The dimensions for the object
		template<typename V>
		LIBRAPID_ALWAYS_INLINE Shape(const std::initializer_list<V> &vals);

		/// Create a Shape object from a vector of values
		/// \tparam V Scalar type of the values
		/// \param vals The dimensions for the object
		template<typename V>
		explicit LIBRAPID_ALWAYS_INLINE Shape(const std::vector<V> &vals);

		/// Create a copy of a Shape object
		/// \param other Shape object to copy
		LIBRAPID_ALWAYS_INLINE Shape(const Shape &other) = default;

		LIBRAPID_ALWAYS_INLINE Shape(const MatrixShape &other);
		LIBRAPID_ALWAYS_INLINE Shape(const VectorShape &other);

		/// Create a Shape from an RValue
		/// \param other Temporary Shape object to copy
		LIBRAPID_ALWAYS_INLINE Shape(Shape &&other) noexcept = default;

		/// Create a Shape object from one with a different type and number of dimensions, moving it
		/// instead of copying it.
		/// \tparam V Scalar type of the values
		/// \tparam Dim Number of dimensions
		/// \param other Temporary Shape object to move
		template<size_t Dim>
		LIBRAPID_ALWAYS_INLINE Shape(Shape &&other) noexcept;

		/// Assign a Shape object to this object
		/// \tparam V Scalar type of the Shape
		/// \param vals Dimensions of the Shape
		/// \return *this
		template<typename V>
		LIBRAPID_ALWAYS_INLINE auto operator=(const std::initializer_list<V> &vals) -> Shape &;

		/// Assign a Shape object to this object
		/// \tparam V Scalar type of the Shape
		/// \param vals Dimensions of the Shape
		/// \return *this
		template<typename V>
		LIBRAPID_ALWAYS_INLINE auto operator=(const std::vector<V> &vals) -> Shape &;

		/// Assign an RValue Shape to this object
		/// \param other RValue to move
		/// \return
		LIBRAPID_ALWAYS_INLINE auto operator=(Shape &&other) noexcept -> Shape & = default;

		/// Assign a Shape to this object
		/// \param other Shape to copy
		/// \return
		LIBRAPID_ALWAYS_INLINE auto operator=(const Shape &other) -> Shape & = default;

		/// Return a Shape object with \p dims dimensions, all initialized to zero.
		/// \param dims Number of dimensions
		/// \return New Shape object
		LIBRAPID_ALWAYS_INLINE static auto zeros(DimType dims) -> Shape;

		/// Return a Shape object with \p dims dimensions, all initialized to one.
		/// \param dims Number of dimensions
		/// \return New Shape object
		LIBRAPID_ALWAYS_INLINE static auto ones(DimType dims) -> Shape;

		/// Access an element of the Shape object
		/// \tparam Index Typename of the index
		/// \param index Index to access
		/// \return The value at the index
		template<typename Index>
		LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE auto operator[](Index index) const
		  -> const SizeType &;

		/// Access an element of the Shape object
		/// \tparam Index Typename of the index
		/// \param index Index to access
		/// \return A reference to the value at the index
		template<typename Index>
		LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE auto operator[](Index index) -> SizeType &;

		/// Compare two Shape objects, returning true if and only if they are identical
		/// \param other Shape object to compare
		/// \return	true if the objects are identical
		LIBRAPID_ALWAYS_INLINE auto operator==(const Shape &other) const -> bool;

		/// Compare two Shape objects, returning true if and only if they are not identical
		/// \param other Shape object to compare
		/// \return true if the objects are not identical
		LIBRAPID_ALWAYS_INLINE auto operator!=(const Shape &other) const -> bool;

		/// Return the number of dimensions in the Shape object
		/// \return Number of dimensions
		LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE auto ndim() const -> DimType;

		/// Return a subshape of the Shape object
		/// \param start Starting index
		/// \param end Ending index
		/// \return Subshape
		LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE auto subshape(DimType start = -1,
																DimType end	  = -1) const -> Shape;

		/// Return the number of elements the Shape object represents
		/// \return Number of elements
		LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE auto size() const -> size_t;

		LIBRAPID_NODISCARD const std::array<SizeType, MaxDimensions> &data() const {
			return m_data;
		}

		LIBRAPID_NODISCARD std::array<SizeType, MaxDimensions> &data() { return m_data; }

		template<typename T_, typename Char, typename Ctx>
		void str(const fmt::formatter<T_, Char> &format, Ctx &ctx) const;

	protected:
		DimType m_dims;
		std::array<SizeType, MaxDimensions> m_data;
	};

	class MatrixShape {
	public:
		using SizeType						  = uint32_t;
		static constexpr size_t MaxDimensions = 2;

		LIBRAPID_ALWAYS_INLINE MatrixShape() = default;

		template<typename Scalar, size_t Rows, size_t Cols>
		LIBRAPID_ALWAYS_INLINE explicit MatrixShape(const FixedStorage<Scalar, Rows, Cols> &fixed);

		template<typename V>
		LIBRAPID_ALWAYS_INLINE MatrixShape(const std::initializer_list<V> &vals);

		template<typename V>
		LIBRAPID_ALWAYS_INLINE explicit MatrixShape(const std::vector<V> &vals);

		LIBRAPID_ALWAYS_INLINE MatrixShape(const Shape &other);
		LIBRAPID_ALWAYS_INLINE MatrixShape(const MatrixShape &other) = default;

		LIBRAPID_ALWAYS_INLINE MatrixShape(MatrixShape &&other) noexcept = default;

		template<typename V>
		LIBRAPID_ALWAYS_INLINE auto operator=(const std::initializer_list<V> &vals)
		  -> MatrixShape &;

		template<typename V>
		LIBRAPID_ALWAYS_INLINE auto operator=(const std::vector<V> &vals) -> MatrixShape &;

		LIBRAPID_ALWAYS_INLINE MatrixShape &operator=(const MatrixShape &other) = default;

		LIBRAPID_ALWAYS_INLINE MatrixShape &operator=(MatrixShape &&other) noexcept = default;

		static LIBRAPID_ALWAYS_INLINE auto zeros() -> MatrixShape;
		static LIBRAPID_ALWAYS_INLINE auto ones() -> MatrixShape;

		static LIBRAPID_ALWAYS_INLINE auto zeros(size_t) -> MatrixShape;
		static LIBRAPID_ALWAYS_INLINE auto ones(size_t) -> MatrixShape;

		template<typename Index>
		LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE auto operator[](Index index) const
		  -> const SizeType &;

		template<typename Index>
		LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE auto operator[](Index index) -> SizeType &;

		LIBRAPID_ALWAYS_INLINE auto operator<=>(const MatrixShape &other) const = default;

		LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE constexpr auto ndim() const -> int;

		LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE auto subshape(int start, int end) const -> Shape;

		LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE auto size() const -> size_t;

		template<typename T_, typename Char, typename Ctx>
		void str(const fmt::formatter<T_, Char> &format, Ctx &ctx) const;

	private:
		SizeType m_rows;
		SizeType m_cols;
	};

	class VectorShape {
	public:
		using SizeType						  = uint32_t;
		using DimType						  = int16_t;
		static constexpr size_t MaxDimensions = 1;

		LIBRAPID_ALWAYS_INLINE VectorShape() = default;

		template<typename Scalar, size_t Elements>
		LIBRAPID_ALWAYS_INLINE explicit VectorShape(const FixedStorage<Scalar, Elements> &fixed);

		template<typename V>
		LIBRAPID_ALWAYS_INLINE VectorShape(const std::initializer_list<V> &vals);

		template<typename V>
		LIBRAPID_ALWAYS_INLINE explicit VectorShape(const std::vector<V> &vals);

		LIBRAPID_ALWAYS_INLINE VectorShape(const Shape &other);
		LIBRAPID_ALWAYS_INLINE VectorShape(const VectorShape &other) = default;

		LIBRAPID_ALWAYS_INLINE VectorShape(VectorShape &&other) noexcept = default;

		template<typename V>
		LIBRAPID_ALWAYS_INLINE auto operator=(const std::initializer_list<V> &vals)
		  -> VectorShape &;

		template<typename V>
		LIBRAPID_ALWAYS_INLINE auto operator=(const std::vector<V> &vals) -> VectorShape &;

		LIBRAPID_ALWAYS_INLINE VectorShape &operator=(const VectorShape &other) = default;

		LIBRAPID_ALWAYS_INLINE VectorShape &operator=(VectorShape &&other) noexcept = default;

		static LIBRAPID_ALWAYS_INLINE auto zeros() -> VectorShape;
		static LIBRAPID_ALWAYS_INLINE auto ones() -> VectorShape;

		static LIBRAPID_ALWAYS_INLINE auto zeros(DimType) -> VectorShape;
		static LIBRAPID_ALWAYS_INLINE auto ones(DimType) -> VectorShape;

		template<typename Index>
		LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE auto operator[](Index index) const
		  -> const SizeType &;

		template<typename Index>
		LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE auto operator[](Index index) -> SizeType &;

		LIBRAPID_ALWAYS_INLINE auto operator<=>(const VectorShape &other) const = default;

		LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE constexpr auto ndim() const -> int;

		LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE auto subshape(DimType start, DimType end) const
		  -> Shape;

		LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE auto size() const -> size_t;

		template<typename T_, typename Char, typename Ctx>
		void str(const fmt::formatter<T_, Char> &format, Ctx &ctx) const;

	private:
		SizeType m_elements;
	};

	namespace detail {
		template<typename T, size_t... Dims>
		Shape shapeFromFixedStorage(const FixedStorage<T, Dims...> &) {
			return Shape({Dims...});
		}
	} // namespace detail

	template<typename Scalar, size_t... Dimensions>
	LIBRAPID_ALWAYS_INLINE Shape::Shape(const FixedStorage<Scalar, Dimensions...> &) :
			m_dims(sizeof...(Dimensions)), m_data({Dimensions...}) {}

	template<typename V>
	LIBRAPID_ALWAYS_INLINE Shape::Shape(const std::initializer_list<V> &vals) :
			m_dims(vals.size()) {
		for (size_t i = 0; i < vals.size(); ++i) { m_data[i] = *(vals.begin() + i); }
	}

	template<typename V>
	LIBRAPID_ALWAYS_INLINE Shape::Shape(const std::vector<V> &vals) : m_dims(vals.size()) {
		for (size_t i = 0; i < vals.size(); ++i) { m_data[i] = vals[i]; }
	}

	LIBRAPID_ALWAYS_INLINE Shape::Shape(const MatrixShape &other) {
		m_dims	  = 2;
		m_data[0] = other[0];
		m_data[1] = other[1];
	}

	LIBRAPID_ALWAYS_INLINE Shape::Shape(const VectorShape &other) {
		m_dims	  = 1;
		m_data[0] = other[0];
	}

	template<typename V>
	LIBRAPID_ALWAYS_INLINE auto Shape::operator=(const std::initializer_list<V> &vals) -> Shape & {
		m_dims = vals.size();
		for (size_t i = 0; i < vals.size(); ++i) { m_data[i] = *(vals.begin() + i); }
		return *this;
	}

	template<typename V>
	LIBRAPID_ALWAYS_INLINE auto Shape::operator=(const std::vector<V> &vals) -> Shape & {
		m_dims = vals.size();
		for (size_t i = 0; i < vals.size(); ++i) { m_data[i] = vals[i]; }
		return *this;
	}

	LIBRAPID_ALWAYS_INLINE auto Shape::zeros(DimType dims) -> Shape {
		Shape res;
		res.m_dims = dims;
		for (int i = 0; i < dims; ++i) res.m_data[i] = 0;
		return res;
	}

	LIBRAPID_ALWAYS_INLINE auto Shape::ones(DimType dims) -> Shape {
		Shape res;
		res.m_dims = dims;
		for (int i = 0; i < dims; ++i) res.m_data[i] = 1;
		return res;
	}

	template<typename Index>
	LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE auto Shape::operator[](Index index) const
	  -> const SizeType & {
		static_assert(std::is_integral_v<Index>, "Index must be an integral type");
		LIBRAPID_ASSERT_WITH_EXCEPTION(std::out_of_range,
									   index < m_dims,
									   "Index {} out of bounds for Shape with {} dimensions",
									   index,
									   m_dims);
		LIBRAPID_ASSERT_WITH_EXCEPTION(std::out_of_range,
									   index >= 0,
									   "Index out of bounds. Must be greater than 0. Received {}",
									   index);
		return m_data[index];
	}

	template<typename Index>
	LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE auto Shape::operator[](Index index) -> SizeType & {
		static_assert(std::is_integral_v<Index>, "Index must be an integral type");
		LIBRAPID_ASSERT_WITH_EXCEPTION(std::out_of_range,
									   index < m_dims,
									   "Index {} out of bounds for Shape with {} dimensions",
									   index,
									   m_dims);
		LIBRAPID_ASSERT_WITH_EXCEPTION(
		  std::range_error, index >= 0, "Index {} out of bounds. Must be greater than 0", index);
		return m_data[index];
	}

	LIBRAPID_ALWAYS_INLINE auto Shape::operator==(const Shape &other) const -> bool {
		if (m_dims != other.m_dims) return false;
		for (int i = 0; i < m_dims; ++i) {
			if (m_data[i] != other.m_data[i]) return false;
		}
		return true;
	}

	LIBRAPID_ALWAYS_INLINE auto Shape::operator!=(const Shape &other) const -> bool {
		return !(*this == other);
	}

	LIBRAPID_NODISCARD auto Shape::ndim() const -> DimType { return m_dims; }

	LIBRAPID_NODISCARD auto Shape::subshape(DimType start, DimType end) const -> Shape {
		if (start < 0) start = 0;
		if (end < 0) end = m_dims;

		LIBRAPID_ASSERT_WITH_EXCEPTION(std::range_error,
									   start <= end,
									   "Start index ({}) must not be greater than end index ({})",
									   start,
									   end);
		LIBRAPID_ASSERT_WITH_EXCEPTION(
		  std::out_of_range,
		  end <= m_dims,
		  "End index ({}) must be less than or equal to the number of dimensions ({}).",
		  end,
		  m_dims);
		LIBRAPID_ASSERT_WITH_EXCEPTION(std::out_of_range,
									   start >= 0,
									   "Start index ({}) must be greater than or equal to 0",
									   start);

		Shape res;
		res.m_dims = end - start;
		for (int i = 0; i < res.m_dims; ++i) res.m_data[i] = m_data[i + start];
		return res;
	}

	LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE auto Shape::size() const -> size_t {
		size_t res = 1;
		for (int i = 0; i < m_dims; ++i) res *= m_data[i];
		return res;
	}

	template<typename T_, typename Char, typename Ctx>
	LIBRAPID_ALWAYS_INLINE void Shape::str(const fmt::formatter<T_, Char> &format, Ctx &ctx) const {
		fmt::format_to(ctx.out(), "Shape(");
		for (int i = 0; i < m_dims; ++i) {
			format.format(m_data[i], ctx);
			if (i != m_dims - 1) fmt::format_to(ctx.out(), ", ");
		}
		fmt::format_to(ctx.out(), ")");
	}

	template<typename Scalar, size_t Rows, size_t Cols>
	LIBRAPID_ALWAYS_INLINE MatrixShape::MatrixShape(const FixedStorage<Scalar, Rows, Cols> &) :
			m_rows(Rows), m_cols(Cols) {}

	template<typename V>
	LIBRAPID_ALWAYS_INLINE MatrixShape::MatrixShape(const std::initializer_list<V> &vals) {
		LIBRAPID_ASSERT_WITH_EXCEPTION(std::invalid_argument,
									   vals.size() <= 2,
									   "MatrixShape must be initialized with 2 values. Received {}",
									   vals.size());
		if (vals.size() == 2) {
			m_rows = *(vals.begin());
			m_cols = *(vals.begin() + 1);
		} else if (vals.size() == 1) {
			m_rows = *(vals.begin());
			m_cols = 1;
		} else {
			m_rows = 0;
			m_cols = 0;
		}
	}

	template<typename V>
	LIBRAPID_ALWAYS_INLINE MatrixShape::MatrixShape(const std::vector<V> &vals) {
		LIBRAPID_ASSERT_WITH_EXCEPTION(std::invalid_argument,
									   vals.size() <= 2,
									   "MatrixShape must be initialized with 2 values. Received {}",
									   vals.size());
		if (vals.size() == 2) {
			m_rows = vals[0];
			m_cols = vals[1];
		} else if (vals.size() == 1) {
			m_rows = vals[0];
			m_cols = 1;
		} else {
			m_rows = 0;
			m_cols = 0;
		}
	}

	LIBRAPID_ALWAYS_INLINE MatrixShape::MatrixShape(const Shape &other) {
		LIBRAPID_ASSERT_WITH_EXCEPTION(
		  std::invalid_argument,
		  other.ndim() <= 2,
		  "MatrixShape must be initialized with 2 dimension, but received {}",
		  other.ndim());
		if (other.ndim() == 2) {
			m_rows = other[0];
			m_cols = other[1];
		} else if (other.ndim() == 1) {
			m_rows = other[0];
			m_cols = 1;
		} else {
			m_rows = 0;
			m_cols = 0;
		}
	}

	template<typename V>
	LIBRAPID_ALWAYS_INLINE auto MatrixShape::operator=(const std::initializer_list<V> &vals)
	  -> MatrixShape & {
		LIBRAPID_ASSERT_WITH_EXCEPTION(
		  std::invalid_argument,
		  vals.size() <= 2,
		  "MatrixShape must be initialized with 2 values, but received {}",
		  vals.size());
		if (vals.size() == 2) {
			m_rows = *(vals.begin());
			m_cols = *(vals.begin() + 1);
		} else if (vals.size() == 1) {
			m_rows = *(vals.begin());
			m_cols = 1;
		} else {
			m_rows = 0;
			m_cols = 0;
		}
		return *this;
	}

	template<typename V>
	LIBRAPID_ALWAYS_INLINE auto MatrixShape::operator=(const std::vector<V> &vals)
	  -> MatrixShape & {
		LIBRAPID_ASSERT_WITH_EXCEPTION(
		  std::invalid_argument,
		  vals.size() <= 2,
		  "MatrixShape must be initialized with 2 values, but received {}",
		  vals.size());
		if (vals.size() == 2) {
			m_rows = vals[0];
			m_cols = vals[1];
		} else if (vals.size() == 1) {
			m_rows = vals[0];
			m_cols = 1;
		} else {
			m_rows = 0;
			m_cols = 0;
		}
		return *this;
	}

	LIBRAPID_ALWAYS_INLINE auto MatrixShape::zeros() -> MatrixShape { return MatrixShape({0, 0}); }
	LIBRAPID_ALWAYS_INLINE auto MatrixShape::ones() -> MatrixShape { return MatrixShape({1, 1}); }

	LIBRAPID_ALWAYS_INLINE auto MatrixShape::zeros(size_t) -> MatrixShape {
		return MatrixShape({0, 0});
	}

	LIBRAPID_ALWAYS_INLINE auto MatrixShape::ones(size_t) -> MatrixShape {
		return MatrixShape({1, 1});
	}

	template<typename Index>
	LIBRAPID_ALWAYS_INLINE auto MatrixShape::operator[](Index index) const -> const SizeType & {
		static_assert(std::is_integral_v<Index>, "Index must be an integral type");
		LIBRAPID_ASSERT_WITH_EXCEPTION(std::out_of_range,
									   index < 2,
									   "Index {} out of bounds for MatrixShape with 2 dimensions",
									   index);
		LIBRAPID_ASSERT_WITH_EXCEPTION(
		  std::out_of_range, index >= 0, "Index {} out of bounds. Must be greater than 0", index);

		return index == 0 ? m_rows : m_cols;
	}

	template<typename Index>
	LIBRAPID_ALWAYS_INLINE auto MatrixShape::operator[](Index index) -> SizeType & {
		static_assert(std::is_integral_v<Index>, "Index must be an integral type");
		LIBRAPID_ASSERT_WITH_EXCEPTION(std::out_of_range,
									   index < 2,
									   "Index {} out of bounds for MatrixShape with 2 dimensions",
									   index);
		LIBRAPID_ASSERT_WITH_EXCEPTION(
		  std::out_of_range, index >= 0, "Index {} out of bounds. Must be greater than 0", index);

		return index == 0 ? m_rows : m_cols;
	}

	LIBRAPID_ALWAYS_INLINE constexpr auto MatrixShape::ndim() const -> int { return 2; }

	LIBRAPID_ALWAYS_INLINE auto MatrixShape::subshape(int start, int end) const -> Shape {
		LIBRAPID_ASSERT_WITH_EXCEPTION(std::invalid_argument,
									   start <= end,
									   "Start index ({}) must not be greater than end index ({})",
									   start,
									   end);
		LIBRAPID_ASSERT_WITH_EXCEPTION(
		  std::out_of_range,
		  end <= 2,
		  "End index ({}) must be less than or equal to the number of dimensions (2).",
		  end);
		LIBRAPID_ASSERT_WITH_EXCEPTION(std::out_of_range,
									   start >= 0,
									   "Start index ({}) must be greater than or equal to 0",
									   start);

		Shape res = Shape::zeros(2);
		res[0]	  = m_rows;
		res[1]	  = m_cols;
		return res.subshape(start, end);
	}

	LIBRAPID_ALWAYS_INLINE auto MatrixShape::size() const -> size_t { return m_rows * m_cols; }

	template<typename T_, typename Char, typename Ctx>
	LIBRAPID_ALWAYS_INLINE void MatrixShape::str(const fmt::formatter<T_, Char> &format,
												 Ctx &ctx) const {
		fmt::format_to(ctx.out(), "MatrixShape(");
		format.format(m_rows, ctx);
		fmt::format_to(ctx.out(), ", ");
		format.format(m_cols, ctx);
		fmt::format_to(ctx.out(), ")");
	}

	template<typename Scalar, size_t Elements>
	LIBRAPID_ALWAYS_INLINE VectorShape::VectorShape(const FixedStorage<Scalar, Elements> &) :
			m_elements(Elements) {}

	template<typename V>
	LIBRAPID_ALWAYS_INLINE VectorShape::VectorShape(const std::initializer_list<V> &vals) {
		LIBRAPID_ASSERT_WITH_EXCEPTION(std::invalid_argument,
									   vals.size() == 1,
									   "MatrixShape must be initialized with 1 value. Received {}",
									   vals.size());
		m_elements = *(vals.begin());
	}

	template<typename V>
	LIBRAPID_ALWAYS_INLINE VectorShape::VectorShape(const std::vector<V> &vals) {
		LIBRAPID_ASSERT_WITH_EXCEPTION(std::invalid_argument,
									   vals.size() == 1,
									   "MatrixShape must be initialized with 1 value. Received {}",
									   vals.size());
		m_elements = vals[0];
	}

	LIBRAPID_ALWAYS_INLINE VectorShape::VectorShape(const Shape &other) {
		LIBRAPID_ASSERT_WITH_EXCEPTION(
		  std::invalid_argument,
		  other.ndim() == 1,
		  "VectorShape must be initialized with 1 dimension, but received {}",
		  other.ndim());
		m_elements = other[0];
	}

	template<typename V>
	LIBRAPID_ALWAYS_INLINE auto VectorShape::operator=(const std::initializer_list<V> &vals)
	  -> VectorShape & {
		LIBRAPID_ASSERT_WITH_EXCEPTION(std::invalid_argument,
									   vals.size() == 1,
									   "MatrixShape must be initialized with 1 value. Received {}",
									   vals.size());
		m_elements = *(vals.begin());
		return *this;
	}

	template<typename V>
	LIBRAPID_ALWAYS_INLINE auto VectorShape::operator=(const std::vector<V> &vals)
	  -> VectorShape & {
		LIBRAPID_ASSERT_WITH_EXCEPTION(std::runtime_error,
									   vals.size() == 1,
									   "MatrixShape must be initialized with 1 value. Received {}",
									   vals.size());
		m_elements = vals[0];
		return *this;
	}

	LIBRAPID_ALWAYS_INLINE auto VectorShape::zeros() -> VectorShape { return VectorShape({0}); }
	LIBRAPID_ALWAYS_INLINE auto VectorShape::ones() -> VectorShape { return VectorShape({1}); }

	LIBRAPID_ALWAYS_INLINE auto VectorShape::zeros(DimType) -> VectorShape {
		return VectorShape({0});
	}

	LIBRAPID_ALWAYS_INLINE auto VectorShape::ones(DimType) -> VectorShape {
		return VectorShape({1});
	}

	template<typename Index>
	LIBRAPID_ALWAYS_INLINE auto VectorShape::operator[](Index index) const -> const SizeType & {
		static_assert(std::is_integral_v<Index>, "Index must be an integral type");
		LIBRAPID_ASSERT_WITH_EXCEPTION(std::range_error,
									   index < 1,
									   "Index {} out of bounds for VectorShape with 1 dimension",
									   index);
		LIBRAPID_ASSERT_WITH_EXCEPTION(
		  std::range_error, index >= 0, "Index {} out of bounds. Must be greater than 0", index);

		return m_elements;
	}

	template<typename Index>
	LIBRAPID_ALWAYS_INLINE auto VectorShape::operator[](Index index) -> SizeType & {
		static_assert(std::is_integral_v<Index>, "Index must be an integral type");
		LIBRAPID_ASSERT_WITH_EXCEPTION(std::range_error,
									   index < 1,
									   "Index {} out of bounds for VectorShape with 1 dimension",
									   index);
		LIBRAPID_ASSERT_WITH_EXCEPTION(
		  std::range_error, index >= 0, "Index {} out of bounds. Must be greater than 0", index);

		return m_elements;
	}

	LIBRAPID_ALWAYS_INLINE constexpr auto VectorShape::ndim() const -> int { return 1; }

	LIBRAPID_ALWAYS_INLINE auto VectorShape::subshape(DimType start, DimType end) const -> Shape {
		LIBRAPID_ASSERT_WITH_EXCEPTION(std::invalid_argument,
									   start <= end,
									   "Start index ({}) must not be greater than end index ({})",
									   start,
									   end);
		LIBRAPID_ASSERT_WITH_EXCEPTION(
		  std::range_error,
		  end <= 1,
		  "End index ({}) must be less than or equal to the number of dimensions (1).",
		  end);
		LIBRAPID_ASSERT_WITH_EXCEPTION(std::range_error,
									   start >= 0,
									   "Start index ({}) must be greater than or equal to 0",
									   start);

		return Shape::zeros(1);
	}

	LIBRAPID_ALWAYS_INLINE auto VectorShape::size() const -> size_t { return m_elements; }

	template<typename T_, typename Char, typename Ctx>
	LIBRAPID_ALWAYS_INLINE void VectorShape::str(const fmt::formatter<T_, Char> &format,
												 Ctx &ctx) const {
		fmt::format_to(ctx.out(), "VectorShape(");
		format.format(m_elements, ctx);
		fmt::format_to(ctx.out(), ")");
	}

	LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE auto operator==(const Shape &lhs,
															  const MatrixShape &rhs) -> bool {
		return lhs.ndim() == 2 && lhs[0] == rhs[0] && lhs[1] == rhs[1];
	}

	LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE auto operator==(const MatrixShape &lhs,
															  const Shape &rhs) -> bool {
		return rhs == lhs;
	}

	LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE auto operator!=(const Shape &lhs,
															  const MatrixShape &rhs) -> bool {
		return !(lhs == rhs);
	}

	LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE auto operator!=(const MatrixShape &lhs,
															  const Shape &rhs) -> bool {
		return !(lhs == rhs);
	}

	LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE auto operator==(const Shape &lhs,
															  const VectorShape &rhs) -> bool {
		return lhs.ndim() == 1 && lhs[0] == rhs[0];
	}

	LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE auto operator==(const VectorShape &lhs,
															  const Shape &rhs) -> bool {
		return rhs == lhs;
	}

	LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE auto operator!=(const Shape &lhs,
															  const VectorShape &rhs) -> bool {
		return !(lhs == rhs);
	}

	LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE auto operator!=(const VectorShape &lhs,
															  const Shape &rhs) -> bool {
		return !(lhs == rhs);
	}

	LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE auto operator==(const MatrixShape &,
															  const VectorShape &) -> bool {
		// A vector cannot have the same shape as a matrix since it has a different number of
		// dimensions by definition
		return false;
	}

	LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE auto operator==(const VectorShape &,
															  const MatrixShape &) -> bool {
		return false;
	}

	LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE auto operator!=(const MatrixShape &lhs,
															  const VectorShape &rhs) -> bool {
		return !(lhs == rhs);
	}

	LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE auto operator!=(const VectorShape &lhs,
															  const MatrixShape &rhs) -> bool {
		return !(lhs == rhs);
	}

	namespace typetraits {
		template<typename T>
		struct IsSizeType : std::false_type {};

		template<>
		struct IsSizeType<Shape> : std::true_type {};

		template<>
		struct IsSizeType<MatrixShape> : std::true_type {};
	} // namespace typetraits

	/// Returns true if all inputs have the same shape
	/// \tparam T1 Type of the first input
	/// \tparam N1 Number of dimensions of the first input
	/// \tparam T2 Type of the second input
	/// \tparam N2 Number of dimensions of the second input
	/// \tparam Tn Type of the remaining (optional) inputs
	/// \tparam Nn Number of dimensions of the remaining (optional) inputs
	/// \param first First input
	/// \param second Second input
	/// \param shapes Remaining (optional) inputs
	/// \return True if all inputs have the same shape, false otherwise
	template<typename First, typename Second, typename... Rest>
		requires(typetraits::IsSizeType<First>::value && typetraits::IsSizeType<Second>::value &&
				 (typetraits::IsSizeType<Rest>::value && ...))
	LIBRAPID_NODISCARD LIBRAPID_INLINE bool
	shapesMatch(const std::tuple<First, Second, Rest...> &shapes) {
		if constexpr (sizeof...(Rest) == 0) {
			return std::get<0>(shapes) == std::get<1>(shapes);
		} else {
			return std::get<0>(shapes) == std::get<1>(shapes) &&
				   shapesMatch(std::apply(
					 [](auto, auto, auto... rest) { return std::make_tuple(rest...); }, shapes));
		}
	}

	template<typename First>
		requires(typetraits::IsSizeType<First>::value)
	LIBRAPID_NODISCARD LIBRAPID_INLINE bool shapesMatch(const std::tuple<First> &shapes) {
		return true;
	}

	namespace detail {
		template<typename First, typename Second>
		struct ShapeTypeHelperImpl {
			using Type = std::false_type;
		};

		template<>
		struct ShapeTypeHelperImpl<Shape, Shape> {
			using Type = Shape;
		};

		template<>
		struct ShapeTypeHelperImpl<Shape, MatrixShape> {
			using Type = Shape;
		};

		template<>
		struct ShapeTypeHelperImpl<MatrixShape, Shape> {
			using Type = Shape;
		};

		template<>
		struct ShapeTypeHelperImpl<Shape, VectorShape> {
			using Type = Shape;
		};

		template<>
		struct ShapeTypeHelperImpl<VectorShape, Shape> {
			using Type = Shape;
		};

		template<>
		struct ShapeTypeHelperImpl<MatrixShape, MatrixShape> {
			using Type = MatrixShape;
		};

		template<>
		struct ShapeTypeHelperImpl<MatrixShape, VectorShape> {
			using Type = Shape;
		};

		template<>
		struct ShapeTypeHelperImpl<VectorShape, MatrixShape> {
			using Type = Shape;
		};

		template<>
		struct ShapeTypeHelperImpl<VectorShape, VectorShape> {
			using Type = VectorShape;
		};

		template<typename NonFalseType>
		struct ShapeTypeHelperImpl<NonFalseType, std::false_type> {
			using Type = NonFalseType;
		};

		template<typename NonFalseType>
		struct ShapeTypeHelperImpl<std::false_type, NonFalseType> {
			using Type = Shape;
		};

		template<>
		struct ShapeTypeHelperImpl<std::false_type, std::false_type> {
			using Type = VectorShape; // Fastest
		};

		template<typename... Args>
		struct ShapeTypeHelper;

		template<typename First>
		struct ShapeTypeHelper<First> {
			using Type = First;
		};

		template<typename First, typename Second>
		struct ShapeTypeHelper<First, Second> {
			using Type = typename ShapeTypeHelperImpl<First, Second>::Type;
		};

		template<typename First, typename Second, typename... Rest>
		struct ShapeTypeHelper<First, Second, Rest...> {
			using FirstResult = typename ShapeTypeHelperImpl<First, Second>::Type;
			using Type		  = typename ShapeTypeHelper<FirstResult, Rest...>::Type;
		};

		template<typename T>
		struct SubscriptShapeType {
			using Type = Shape;
		};

		template<>
		struct SubscriptShapeType<MatrixShape> {
			using Type = VectorShape;
		};

		template<>
		struct SubscriptShapeType<VectorShape> {
			using Type = Shape;
		};
	} // namespace detail
} // namespace librapid

// Support FMT printing
#ifdef FMT_API

template<>
struct fmt::formatter<librapid::Shape> {
private:
	using Type	   = librapid::Shape;
	using SizeType = librapid::Shape::SizeType;
	using Base	   = fmt::formatter<SizeType, char>;
	Base m_base;

public:
	template<typename ParseContext>
	FMT_CONSTEXPR auto parse(ParseContext &ctx) -> const char * {
		return m_base.parse(ctx);
	}

	template<typename FormatContext>
	FMT_CONSTEXPR auto format(const Type &val, FormatContext &ctx) const -> decltype(ctx.out()) {
		val.str(m_base, ctx);
		return ctx.out();
	}
};

template<>
struct fmt::formatter<librapid::MatrixShape> {
private:
	using Type	   = librapid::MatrixShape;
	using SizeType = librapid::MatrixShape::SizeType;
	using Base	   = fmt::formatter<SizeType, char>;
	Base m_base;

public:
	template<typename ParseContext>
	FMT_CONSTEXPR auto parse(ParseContext &ctx) -> const char * {
		return m_base.parse(ctx);
	}

	template<typename FormatContext>
	FMT_CONSTEXPR auto format(const Type &val, FormatContext &ctx) const -> decltype(ctx.out()) {
		val.str(m_base, ctx);
		return ctx.out();
	}
};
#endif // FMT_API

#endif // LIBRAPID_ARRAY_SIZETYPE_HPP