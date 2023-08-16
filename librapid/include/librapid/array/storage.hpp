#ifndef LIBRAPID_ARRAY_STORAGE_HPP
#define LIBRAPID_ARRAY_STORAGE_HPP

/*
 * This file defines the Storage class, which contains a contiguous
 * block of memory of a single data type.
 */

namespace librapid {
    namespace typetraits {
        template<typename Scalar_>
        struct TypeInfo<Storage<Scalar_>> {
            static constexpr bool isLibRapidType = true;
            using Scalar                         = Scalar_;
            using Backend                        = backend::CPU;
        };

        template<typename Scalar_, size_t... Dims>
        struct TypeInfo<FixedStorage<Scalar_, Dims...>> {
            static constexpr bool isLibRapidType = true;
            using Scalar                         = Scalar_;
            using Backend                        = backend::CPU;
        };

        LIBRAPID_DEFINE_AS_TYPE(typename Scalar, Storage<Scalar>);
    } // namespace typetraits

    template<typename Scalar_>
    class Storage {
    public:
        using Scalar                          = Scalar_;
        using Packet                          = typename typetraits::TypeInfo<Scalar>::Packet;
        static constexpr uint64_t packetWidth = typetraits::TypeInfo<Scalar>::packetWidth;
        using RawPointer                      = Scalar *;
        using ConstRawPointer                 = const Scalar *;
        using Pointer                         = std::shared_ptr<Scalar>;
        using ConstPointer                    = std::shared_ptr<const Scalar>;
        using Reference                       = Scalar &;
        using ConstReference                  = const Scalar &;
        using SizeType                        = size_t;
        using DifferenceType                  = ptrdiff_t;
        using Iterator                        = RawPointer;
        using ConstIterator                   = ConstRawPointer;
        using ReverseIterator                 = std::reverse_iterator<Iterator>;
        using ConstReverseIterator            = std::reverse_iterator<ConstIterator>;

        /// Default constructor
        Storage() = default;

        /// Create a Storage object with \p size elements
        /// \param size Number of elements to allocate
        LIBRAPID_ALWAYS_INLINE explicit Storage(SizeType size);

        LIBRAPID_ALWAYS_INLINE explicit Storage(Scalar *begin, Scalar *end, bool ownsData);

        /// Create a Storage object with \p size elements, each initialized
        /// to \p value.
        /// \param size Number of elements to allocate
        /// \param value Value to initialize each element to
        LIBRAPID_ALWAYS_INLINE Storage(SizeType size, ConstReference value);

        /// Create a Storage object from another Storage object. Additionally
        /// a custom allocator can be used. The data is **NOT** copied -- it is referenced
        /// by the new Storage object. For a deep copy, use the ``copy()`` method.
        /// \param other Storage object to copy
        LIBRAPID_ALWAYS_INLINE Storage(const Storage &other);

        /// Move a Storage object into this object.
        /// \param other Storage object to move
        LIBRAPID_ALWAYS_INLINE Storage(Storage &&other) noexcept;

        /// Create a Storage object from an std::initializer_list
        /// \tparam V Type of the elements in the initializer list
        /// \param list Initializer list to copy
        /// \param alloc Allocator to use
        template<typename V>
        LIBRAPID_ALWAYS_INLINE Storage(const std::initializer_list<V> &list);

        /// Create a Storage object from a std::vector
        /// \tparam V Type of the elements in the vector
        /// \param vec Vector to copy
        template<typename V>
        LIBRAPID_ALWAYS_INLINE explicit Storage(const std::vector<V> &vec);

        template<typename V>
        static Storage fromData(const std::initializer_list<V> &vec);

        template<typename V>
        static Storage fromData(const std::vector<V> &vec);

        /// Assignment operator for a Storage object
        /// \param other Storage object to copy
        /// \return *this
        LIBRAPID_ALWAYS_INLINE Storage &operator=(const Storage &other);

        /// Move assignment operator for a Storage object
        /// \param other Storage object to move
        /// \return *this
        LIBRAPID_ALWAYS_INLINE Storage &operator=(Storage &&other) LIBRAPID_RELEASE_NOEXCEPT;

        /// Free a Storage object
        ~Storage();

        /// \brief Set this storage object to reference the same data as \p other
        /// \param other Storage object to reference
        void set(const Storage &other);

        /// \brief Return a Storage object on the host with the same data as this Storage object
        /// (mainly for use with CUDA or OpenCL)
        /// \return
        Storage toHostStorage() const;

        /// \brief Same as `toHostStorage()` but does not necessarily copy the data
        /// \return Storage object on the host
        Storage toHostStorageUnsafe() const;

        /// \brief Create a deep copy of this Storage object
        /// \return Deep copy of this Storage object
        Storage copy() const;

        template<typename ShapeType>
        static ShapeType defaultShape();

        /// Resize a Storage object to \p size elements. Existing elements
        /// are preserved.
        /// \param size New size of the Storage object
        LIBRAPID_ALWAYS_INLINE void resize(SizeType newSize);

        /// Resize a Storage object to \p size elements. Existing elements
        /// are not preserved
        /// \param size New size of the Storage object
        LIBRAPID_ALWAYS_INLINE void resize(SizeType newSize, int);

        /// Return the number of elements in the Storage object
        /// \return
        LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE SizeType size() const noexcept;

        /// Const access to the element at index \p index
        /// \param index Index of the element to access
        /// \return Const reference to the element at index \p index
        LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE ConstReference operator[](SizeType index) const;

        /// Access to the element at index \p index
        /// \param index Index of the element to access
        /// \return Reference to the element at index \p index
        LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE Reference operator[](SizeType index);

        LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE Pointer data() const noexcept;

        LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE RawPointer begin() noexcept;
        LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE RawPointer end() noexcept;

        LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE ConstIterator begin() const noexcept;
        LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE ConstIterator end() const noexcept;

        LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE ConstIterator cbegin() const noexcept;
        LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE ConstIterator cend() const noexcept;

        LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE ReverseIterator rbegin() noexcept;
        LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE ReverseIterator rend() noexcept;

        LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE ConstReverseIterator rbegin() const noexcept;
        LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE ConstReverseIterator rend() const noexcept;

        LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE ConstReverseIterator crbegin() const noexcept;
        LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE ConstReverseIterator crend() const noexcept;

    private:
        /// Copy data from \p begin to \p end into this Storage object
        /// \tparam P Pointer type
        /// \param begin Beginning of data to copy
        /// \param end End of data to copy
        template<typename P>
        LIBRAPID_ALWAYS_INLINE void initData(P begin, P end);

        template<typename P>
        LIBRAPID_ALWAYS_INLINE void initData(P begin, SizeType size);

        /// Resize the Storage Object to \p newSize elements, retaining existing
        /// data.
        /// \param newSize New size of the Storage object
        LIBRAPID_ALWAYS_INLINE void resizeImpl(SizeType newSize, int);

        /// Resize the Storage object to \p newSize elements. Note this does not
        /// initialize the new elements or maintain existing data.
        /// \param newSize New size of the Storage object
        LIBRAPID_ALWAYS_INLINE void resizeImpl(SizeType newSize);

        // #if defined(LIBRAPID_NATIVE_ARCH) && !defined(LIBRAPID_APPLE)
        //		alignas(LIBRAPID_DEFAULT_MEM_ALIGN) Pointer m_begin = nullptr;
        // #else
        //		Pointer m_begin = nullptr; // Pointer to the beginning of the data
        // #endif

        Pointer m_begin = nullptr;

        SizeType m_size = 0;    // Number of elements in the Storage object
        bool m_ownsData = true; // Whether this Storage object owns the data it points to
    };

    template<typename Scalar_, size_t... Size_>
    class FixedStorage {
    public:
        using Scalar                   = Scalar_;
        using Pointer                  = Scalar *;
        using ConstPointer             = const Scalar *;
        using Reference                = Scalar &;
        using ConstReference           = const Scalar &;
        using SizeType                 = size_t;
        using DifferenceType           = ptrdiff_t;
        static constexpr SizeType Size = product<Size_...>();
        using Iterator                 = typename std::array<Scalar, product<Size_...>()>::iterator;
        using ConstIterator   = typename std::array<Scalar, product<Size_...>()>::const_iterator;
        using ReverseIterator = std::reverse_iterator<Iterator>;
        using ConstReverseIterator = std::reverse_iterator<ConstIterator>;

        /// Default constructor
        FixedStorage();

        /// Create a FixedStorage object filled with \p value
        /// \param value Value to fill the FixedStorage object with
        LIBRAPID_ALWAYS_INLINE explicit FixedStorage(const Scalar &value);

        /// Create a FixedStorage object from another FixedStorage object
        /// \param other FixedStorage object to copy
        LIBRAPID_ALWAYS_INLINE FixedStorage(const FixedStorage &other);

        /// Move constructor for a FixedStorage object
        /// \param other FixedStorage object to move
        LIBRAPID_ALWAYS_INLINE FixedStorage(FixedStorage &&other) noexcept;

        /// Create a FixedStorage object from a std::initializer_list
        /// \tparam V Type of the elements in the initializer list
        /// \param list Initializer list to copy
        LIBRAPID_ALWAYS_INLINE explicit FixedStorage(const std::initializer_list<Scalar> &list);

        /// Create a FixedStorage object from a std::vector
        /// \tparam V Type of the elements in the vector
        /// \param vec Vector to copy
        LIBRAPID_ALWAYS_INLINE explicit FixedStorage(const std::vector<Scalar> &vec);

        /// Assignment operator for a FixedStorage object
        /// \param other FixedStorage object to copy
        /// \return *this
        LIBRAPID_ALWAYS_INLINE FixedStorage &operator=(const FixedStorage &other);

        /// Move assignment operator for a FixedStorage object
        /// \param other FixedStorage object to move
        /// \return *this
        LIBRAPID_ALWAYS_INLINE FixedStorage &operator=(FixedStorage &&other) noexcept;

        /// Free a FixedStorage object
        ~FixedStorage() = default;

        template<typename ShapeType>
        static ShapeType defaultShape();

        /// Resize a Storage object to \p size elements. Existing elements
        /// are preserved.
        /// \param size New size of the Storage object
        LIBRAPID_ALWAYS_INLINE void resize(SizeType newSize);

        /// Resize a Storage object to \p size elements. Existing elements
        /// are not preserved
        /// \param size New size of the Storage object
        LIBRAPID_ALWAYS_INLINE void resize(SizeType newSize, int);

        /// Return the number of elements in the FixedStorage object
        /// \return Number of elements in the FixedStorage object
        LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE SizeType size() const noexcept;

        /// \brief Create a copy of the FixedStorage object
        /// \return Copy of the FixedStorage object
        LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE FixedStorage copy() const;

        /// Const access to the element at index \p index
        /// \param index Index of the element to access
        /// \return Const reference to the element at index \p index
        LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE ConstReference operator[](SizeType index) const;

        /// Access to the element at index \p index
        /// \param index Index of the element to access
        /// \return Reference to the element at index \p index
        LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE Reference operator[](SizeType index);

        LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE Pointer data() const noexcept;

        LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE Iterator begin() noexcept;
        LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE Iterator end() noexcept;

        LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE ConstIterator begin() const noexcept;
        LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE ConstIterator end() const noexcept;

        LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE ConstIterator cbegin() const noexcept;
        LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE ConstIterator cend() const noexcept;

        LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE ReverseIterator rbegin() noexcept;
        LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE ReverseIterator rend() noexcept;

        LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE ConstReverseIterator rbegin() const noexcept;
        LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE ConstReverseIterator rend() const noexcept;

        LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE ConstReverseIterator crbegin() const noexcept;
        LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE ConstReverseIterator crend() const noexcept;

    private:
#if defined(LIBRAPID_NATIVE_ARCH) && !defined(LIBRAPID_APPLE)
        alignas(LIBRAPID_DEFAULT_MEM_ALIGN) std::array<Scalar, Size> m_data;
#else
        // No memory alignment on Apple platforms or if it is disabled
        std::array<Scalar, Size> m_data;
#endif
    };

    // Trait implementations
    namespace typetraits {
        template<typename T>
        struct IsStorage : std::false_type {};

        template<typename Scalar>
        struct IsStorage<Storage<Scalar>> : std::true_type {};

        template<typename T>
        struct IsFixedStorage : std::false_type {};

        template<typename Scalar, size_t... Size>
        struct IsFixedStorage<FixedStorage<Scalar, Size...>> : std::true_type {};
    } // namespace typetraits

    namespace detail {
        /// Safely deallocate memory for \p size elements, using an std::allocator \p alloc. If the
        /// object cannot be trivially destroyed, the destructor will be called on each element of
        /// the data, ensuring that it is safe to free the allocated memory.
        /// \tparam A The allocator type
        /// \param alloc The allocator object
        /// \param ptr The pointer to free
        /// \param size The number of elements of type \p in the memory block
        template<typename T>
        void safeDeallocate(T *ptr, size_t size) {
            if constexpr (!std::is_trivially_destructible_v<T>) {
                for (size_t i = 0; i < size; ++i) { ptr[i].~T(); }
            }

#if defined(LIBRAPID_BLAS_MKLBLAS)
            mkl_free(ptr);
#elif defined(LIBRAPID_APPLE)
            free(ptr);
#elif defined(LIBRAPID_NATIVE_ARCH) && defined(LIBRAPID_MSVC)
            _aligned_free(ptr);
#else
            free(ptr);
#endif
        }

        /// Safely allocate memory for \p size elements using the allocator \p alloc. If the data
        /// can be trivially default constructed, then the constructor is not called and no data
        /// is initialized. Otherwise, the correct default constructor will be called for each
        /// element in the data, making sure the returned pointer is safe to use.
        /// \tparam A The allocator type to use
        /// \param alloc The allocator object to use
        /// \param size Number of elements to allocate
        /// \return Pointer to the first element
        /// \see safeDeallocate
        template<typename T>
        std::shared_ptr<T> safeAllocate(size_t size) {
            using RawPointer = T *;
            using Pointer    = std::shared_ptr<T>;

#if defined(LIBRAPID_BLAS_MKLBLAS)
            // MKL has its own memory allocation function
            auto ptr = static_cast<RawPointer>(mkl_malloc(size * sizeof(T), 64));
#elif defined(LIBRAPID_APPLE)
            // Use posix_memalign
            void *_ptr;
            auto err = posix_memalign(&_ptr, global::memoryAlignment, size * sizeof(T));
            LIBRAPID_ASSERT(err == 0, "posix_memalign failed with error code {}", err);
            auto ptr = static_cast<RawPointer>(_ptr);
#elif defined(LIBRAPID_MSVC) || defined(LIBRAPID_MINGW)
            auto ptr =
              static_cast<RawPointer>(_aligned_malloc(size * sizeof(T), global::memoryAlignment));
#else
            auto ptr = static_cast<RawPointer>(
              std::aligned_alloc(global::memoryAlignment, size * sizeof(T)));
#endif

            LIBRAPID_ASSERT(
              ptr != nullptr, "Failed to allocate {} bytes of memory", size * sizeof(T));

            // If the type cannot be trivially constructed, we need to
            // initialize each value
            if constexpr (!typetraits::TriviallyDefaultConstructible<T>::value &&
                          !std::is_array<T>::value) {
                for (RawPointer p = ptr; p != ptr + size; ++p) { new (p) T(); }
            }

            return Pointer(ptr, [size](RawPointer ptr) { safeDeallocate(ptr, size); });
        }

        /// Safely copy a pointer to a shared pointer. If \p ownsData is true, then the shared
        /// pointer will be initialized with a custom deleter that will call safeDeallocate on the
        /// pointer. Otherwise, the shared pointer will be initialized with a no-op deleter.
        /// \tparam T Type of the pointer
        /// \param ptr Raw pointer to copy
        /// \param ownsData Whether the shared pointer should own the data
        /// \return Shared pointer to the data
        template<typename T>
        std::shared_ptr<T> safePointerCopy(T *ptr, size_t size, bool ownsData) {
            using RawPointer = T *;
            using Pointer    = std::shared_ptr<T>;

            if (ownsData) {
                return Pointer(ptr, [size](RawPointer ptr) { safeDeallocate(ptr, size); });
            } else {
                return Pointer(ptr, [](RawPointer) {});
            }
        }

        template<typename T>
        std::shared_ptr<T> safePointerCopy(const std::shared_ptr<T> &ptr, size_t size,
                                           bool ownsData = true) {
            using RawPointer = T *;
            using Pointer    = std::shared_ptr<T>;

            if (ownsData) {
                return Pointer(ptr.get(), [size](RawPointer ptr) { safeDeallocate(ptr, size); });
            } else {
                return Pointer(ptr.get(), [](RawPointer) {});
            }
        }
    } // namespace detail

    template<typename T>
    Storage<T>::Storage(SizeType size) :
            m_begin(detail::safeAllocate<T>(size)), m_size(size), m_ownsData(true) {}

    template<typename T>
    Storage<T>::Storage(Scalar *begin, Scalar *end, bool ownsData) :
            m_begin(detail::safePointerCopy(begin, std::distance(begin, end), ownsData)),
            m_size(std::distance(begin, end)), m_ownsData(ownsData) {}

    template<typename T>
    Storage<T>::Storage(SizeType size, ConstReference value) :
            m_begin(detail::safeAllocate<T>(size)), m_size(size), m_ownsData(true) {
        for (SizeType i = 0; i < size; ++i) { m_begin.get()[i] = value; }
    }

    template<typename T>
    Storage<T>::Storage(const Storage &other) :
            m_begin(other.m_begin), m_size(other.m_size), m_ownsData(other.m_ownsData) {}

    template<typename T>
    Storage<T>::Storage(Storage &&other) noexcept :
            m_begin(std::move(other.m_begin)), m_size(std::move(other.m_size)),
            m_ownsData(std::move(other.m_ownsData)) {
        other.m_begin    = nullptr;
        other.m_size     = 0;
        other.m_ownsData = false;
    }

    template<typename T>
    template<typename V>
    Storage<T>::Storage(const std::initializer_list<V> &list) :
            m_begin(nullptr), m_size(0), m_ownsData(true) {
        initData(list.begin(), list.end());
    }

    template<typename T>
    template<typename V>
    Storage<T>::Storage(const std::vector<V> &vector) :
            m_begin(nullptr), m_size(0), m_ownsData(true) {
        initData(vector.begin(), vector.end());
    }

    template<typename T>
    template<typename V>
    auto Storage<T>::fromData(const std::initializer_list<V> &list) -> Storage {
        return Storage(list);
    }

    template<typename T>
    template<typename V>
    auto Storage<T>::fromData(const std::vector<V> &vec) -> Storage {
        return Storage(vec);
    }

    template<typename T>
    Storage<T> &Storage<T>::operator=(const Storage &other) {
        if (this != &other) {
            if (m_ownsData) {
                // If we own the data already, we can just copy the pointer since we know it won't
                // affect anything else. The shared pointer deals with the reference counting, so
                // we don't need to worry about other arrays that might be using the same data.
                m_begin = other.m_begin;
                m_size  = other.m_size;
            } else {
                LIBRAPID_ASSERT(m_size == other.m_size,
                                "Cannot copy storage with {} elements to dependent storage with "
                                "{} elements",
                                other.m_size,
                                m_size);

                // If we don't own the data, the size must be the same since it is being used
                // elsewhere, and we can't change it

                if (typetraits::TriviallyDefaultConstructible<T>::value) {
                    // Use a slightly faster memcpy if the type is trivially default constructible
                    std::uninitialized_copy(other.begin(), other.end(), m_begin.get());
                } else {
                    // Otherwise, use the standard copy algorithm
                    std::copy(other.begin(), other.end(), m_begin.get());
                }
            }
        }
        return *this;
    }

    template<typename T>
    Storage<T> &Storage<T>::operator=(Storage &&other) LIBRAPID_RELEASE_NOEXCEPT {
        if (this != &other) {
            if (m_ownsData) {
                std::swap(m_begin, other.m_begin);
                std::swap(m_size, other.m_size);
                m_ownsData = other.m_ownsData;
            } else {
                LIBRAPID_ASSERT(
                  size() == other.size(),
                  "Mismatched storage sizes. Cannot assign storage with {} elements to "
                  "dependent storage with {} elements",
                  other.size(),
                  size());

                if (typetraits::TriviallyDefaultConstructible<T>::value) {
                    // Use a slightly faster memcpy if the type is trivially default constructible
                    std::uninitialized_copy(other.begin(), other.end(), m_begin.get());
                } else {
                    // Otherwise, use the standard copy algorithm
                    std::copy(other.begin(), other.end(), m_begin.get());
                }
            }
        }
        return *this;
    }

    template<typename T>
    Storage<T>::~Storage() {
        // All deallocation is handled by the shared pointer, which has a custom deleter which
        // depends on whether the data is owned by the storage object or not. If it is owned, the
        // data is deallocated, otherwise it is left alone.
    }

    template<typename T>
    template<typename P>
    void Storage<T>::initData(P begin, P end) {
        m_size  = static_cast<SizeType>(std::distance(begin, end));
        m_begin = detail::safeAllocate<T>(m_size);

        if constexpr (typetraits::TypeInfo<T>::canMemcpy) {
            if constexpr (typetraits::TriviallyDefaultConstructible<T>::value) {
                // Use a slightly faster memcpy if the type is trivially default constructible
                std::uninitialized_copy(begin, end, m_begin.get());
            } else {
                // Otherwise, use the standard copy algorithm
                std::copy(begin, end, m_begin.get());
            }
        } else {
            // Since we can't memcpy, we have to copy each element individually
            for (SizeType i = 0; i < m_size; ++i) { m_begin.get()[i] = begin[i]; }
        }
    }

    template<typename T>
    template<typename P>
    void Storage<T>::initData(P begin, SizeType size) {
        initData(begin, begin + size);
    }

    template<typename T>
    void Storage<T>::set(const Storage<T> &other) {
        // We can simply copy the shared pointers across
        m_begin    = other.m_begin;
        m_size     = other.m_size;
        m_ownsData = other.m_ownsData;
    }

    template<typename T>
    auto Storage<T>::toHostStorage() const -> Storage {
        return copy();
    }

    template<typename T>
    auto Storage<T>::toHostStorageUnsafe() const -> Storage {
        return copy();
    }

    template<typename T>
    auto Storage<T>::copy() const -> Storage {
        Storage ret;
        ret.initData(m_begin.get(), m_size);
        return ret;
    }

    template<typename T>
    template<typename ShapeType>
    auto Storage<T>::defaultShape() -> ShapeType {
        return ShapeType({0});
    }

    template<typename T>
    auto Storage<T>::size() const noexcept -> SizeType {
        return m_size;
    }

    template<typename T>
    void Storage<T>::resize(SizeType newSize) {
        resizeImpl(newSize);
    }

    template<typename T>
    void Storage<T>::resize(SizeType newSize, int) {
        resizeImpl(newSize, 0);
    }

    template<typename T>
    LIBRAPID_ALWAYS_INLINE void Storage<T>::resizeImpl(SizeType newSize) {
        // Resize and retain data

        if (newSize == size()) return;
        LIBRAPID_ASSERT(m_ownsData, "Dependent storage cannot be resized");

        // Copy the existing data to a new location
        Pointer oldBegin = m_begin;
        SizeType oldSize = m_size;

        // Allocate a new block of memory
        m_begin = detail::safeAllocate<T>(newSize);
        m_size  = newSize;

        // Copy the data across
        if constexpr (typetraits::TriviallyDefaultConstructible<T>::value) {
            // Use a slightly faster memcpy if the type is trivially default constructible
            std::uninitialized_copy(
              oldBegin.get(), oldBegin.get() + ::librapid::min(oldSize, newSize), m_begin.get());
        } else {
            // Otherwise, use the standard copy algorithm
            std::copy(
              oldBegin.get(), oldBegin.get() + ::librapid::min(oldSize, newSize), m_begin.get());
        }
    }

    template<typename T>
    LIBRAPID_ALWAYS_INLINE void Storage<T>::resizeImpl(SizeType newSize, int) {
        // Resize and discard data

        if (size() == newSize) return;
        LIBRAPID_ASSERT(m_ownsData, "Dependent storage cannot be resized");

        // Allocate a new block of memory
        m_begin = detail::safeAllocate<T>(newSize);
        m_size  = newSize;
    }

    template<typename T>
    auto Storage<T>::operator[](Storage<T>::SizeType index) const -> ConstReference {
        LIBRAPID_ASSERT(index < size(), "Index {} out of bounds for size {}", index, size());
        return m_begin.get()[index];
    }

    template<typename T>
    auto Storage<T>::operator[](Storage<T>::SizeType index) -> Reference {
        LIBRAPID_ASSERT(index < size(), "Index {} out of bounds for size {}", index, size());
        return m_begin.get()[index];
    }

    template<typename T>
    auto Storage<T>::data() const noexcept -> Pointer {
        return m_begin;
    }

    template<typename T>
    auto Storage<T>::begin() noexcept -> RawPointer {
        return m_begin.get();
    }

    template<typename T>
    auto Storage<T>::end() noexcept -> RawPointer {
        return m_begin.get() + m_size;
    }

    template<typename T>
    auto Storage<T>::begin() const noexcept -> ConstIterator {
        return m_begin.get();
    }

    template<typename T>
    auto Storage<T>::end() const noexcept -> ConstIterator {
        return m_begin.get() + m_size;
    }

    template<typename T>
    auto Storage<T>::cbegin() const noexcept -> ConstIterator {
        return begin();
    }

    template<typename T>
    auto Storage<T>::cend() const noexcept -> ConstIterator {
        return end();
    }

    template<typename T>
    auto Storage<T>::rbegin() noexcept -> ReverseIterator {
        return ReverseIterator(m_begin.get() + m_size);
    }

    template<typename T>
    auto Storage<T>::rend() noexcept -> ReverseIterator {
        return ReverseIterator(m_begin.get());
    }

    template<typename T>
    auto Storage<T>::rbegin() const noexcept -> ConstReverseIterator {
        return ConstReverseIterator(m_begin.get() + m_size);
    }

    template<typename T>
    auto Storage<T>::rend() const noexcept -> ConstReverseIterator {
        return ConstReverseIterator(m_begin.get());
    }

    template<typename T>
    auto Storage<T>::crbegin() const noexcept -> ConstReverseIterator {
        return rbegin();
    }

    template<typename T>
    auto Storage<T>::crend() const noexcept -> ConstReverseIterator {
        return rend();
    }

    template<typename T, size_t... D>
    FixedStorage<T, D...>::FixedStorage() = default;

    template<typename T, size_t... D>
    FixedStorage<T, D...>::FixedStorage(const Scalar &value) {
        for (size_t i = 0; i < Size; ++i) { m_data[i] = value; }
    }

    template<typename T, size_t... D>
    FixedStorage<T, D...>::FixedStorage(const FixedStorage &other) = default;

    template<typename T, size_t... D>
    FixedStorage<T, D...>::FixedStorage(FixedStorage &&other) noexcept = default;

    template<typename T, size_t... D>
    FixedStorage<T, D...>::FixedStorage(const std::initializer_list<Scalar> &list) {
        LIBRAPID_ASSERT(list.size() == size(), "Initializer list size does not match storage size");
        for (size_t i = 0; i < Size; ++i) { m_data[i] = list.begin()[i]; }
    }

    template<typename T, size_t... D>
    FixedStorage<T, D...>::FixedStorage(const std::vector<Scalar> &vec) {
        LIBRAPID_ASSERT(vec.size() == size(), "Initializer list size does not match storage size");
        for (size_t i = 0; i < Size; ++i) { m_data[i] = vec[i]; }
    }

    template<typename T, size_t... D>
    auto FixedStorage<T, D...>::operator=(const FixedStorage &other) -> FixedStorage & {
        if (this != &other) {
            for (size_t i = 0; i < Size; ++i) { m_data[i] = other.m_data[i]; }
        }
        return *this;
    }

    template<typename T, size_t... D>
    auto FixedStorage<T, D...>::operator=(FixedStorage &&other) noexcept
      -> FixedStorage & = default;

    template<typename T, size_t... D>
    template<typename ShapeType>
    auto FixedStorage<T, D...>::defaultShape() -> ShapeType {
        return ShapeType({D...});
    }

    template<typename T, size_t... D>
    void FixedStorage<T, D...>::resize(SizeType newSize) {
        LIBRAPID_ASSERT(newSize == size(), "FixedStorage cannot be resized");
    }

    template<typename T, size_t... D>
    void FixedStorage<T, D...>::resize(SizeType newSize, int) {
        LIBRAPID_ASSERT(newSize == size(), "FixedStorage cannot be resized");
    }

    template<typename T, size_t... D>
    auto FixedStorage<T, D...>::size() const noexcept -> SizeType {
        return Size;
    }

    template<typename T, size_t... D>
    auto FixedStorage<T, D...>::copy() const -> FixedStorage {
        return FixedStorage<T, D...>();
    }

    template<typename T, size_t... D>
    auto FixedStorage<T, D...>::operator[](SizeType index) const -> ConstReference {
        LIBRAPID_ASSERT(index < size(), "Index out of bounds");
        return m_data[index];
    }

    template<typename T, size_t... D>
    auto FixedStorage<T, D...>::operator[](SizeType index) -> Reference {
        LIBRAPID_ASSERT(index < size(), "Index out of bounds");
        return m_data[index];
    }

    template<typename T, size_t... D>
    auto FixedStorage<T, D...>::data() const noexcept -> Pointer {
        return const_cast<Pointer>(m_data.data());
    }

    template<typename T, size_t... D>
    auto FixedStorage<T, D...>::begin() noexcept -> Iterator {
        return m_data.begin();
    }

    template<typename T, size_t... D>
    auto FixedStorage<T, D...>::end() noexcept -> Iterator {
        return m_data.end();
    }

    template<typename T, size_t... D>
    auto FixedStorage<T, D...>::begin() const noexcept -> ConstIterator {
        return m_data.begin();
    }

    template<typename T, size_t... D>
    auto FixedStorage<T, D...>::end() const noexcept -> ConstIterator {
        return m_data.end();
    }

    template<typename T, size_t... D>
    auto FixedStorage<T, D...>::cbegin() const noexcept -> ConstIterator {
        return begin();
    }

    template<typename T, size_t... D>
    auto FixedStorage<T, D...>::cend() const noexcept -> ConstIterator {
        return end();
    }

    template<typename T, size_t... D>
    auto FixedStorage<T, D...>::rbegin() noexcept -> ReverseIterator {
        return ReverseIterator(end());
    }

    template<typename T, size_t... D>
    auto FixedStorage<T, D...>::rend() noexcept -> ReverseIterator {
        return ReverseIterator(begin());
    }

    template<typename T, size_t... D>
    auto FixedStorage<T, D...>::rbegin() const noexcept -> ConstReverseIterator {
        return ConstReverseIterator(end());
    }

    template<typename T, size_t... D>
    auto FixedStorage<T, D...>::rend() const noexcept -> ConstReverseIterator {
        return ConstReverseIterator(begin());
    }

    template<typename T, size_t... D>
    auto FixedStorage<T, D...>::crbegin() const noexcept -> ConstReverseIterator {
        return rbegin();
    }

    template<typename T, size_t... D>
    auto FixedStorage<T, D...>::crend() const noexcept -> ConstReverseIterator {
        return rend();
    }
} // namespace librapid

#endif // LIBRAPID_ARRAY_STORAGE_HPP