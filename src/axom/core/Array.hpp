// Copyright (c) 2017-2022, Lawrence Livermore National Security, LLC and
// other Axom Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)

#ifndef AXOM_ARRAY_HPP_
#define AXOM_ARRAY_HPP_

#include "axom/config.hpp"
#include "axom/core/Macros.hpp"
#include "axom/core/utilities/Utilities.hpp"
#include "axom/core/Types.hpp"
#include "axom/core/ArrayBase.hpp"
#include "axom/core/ArrayIteratorBase.hpp"
#include "axom/core/ArrayView.hpp"

// C/C++ includes
#include <algorithm>
#include <iostream>

namespace axom
{
namespace ArrayOptions
{
/// \brief A "tag type" for constructing an Array without initializing its memory
struct Uninitialized
{ };

}  // namespace ArrayOptions

// Forward declare the templated classes and operator function(s)
template <typename T, int DIM, MemorySpace SPACE>
class Array;

namespace detail
{
// Static information to pass to ArrayBase
template <typename T, int DIM, MemorySpace SPACE>
struct ArrayTraits<Array<T, DIM, SPACE>>
{
  constexpr static bool is_view = false;
};

}  // namespace detail

/*!
 * \class Array
 *
 * \brief Provides a generic multidimensional array container.
 *
 *  The Array class provides a generic multidimensional array 
 *  container with dynamic reallocation and insertion.  The dimensionality
 *  of the array must be known at compile time but the extents in each dimension
 *  are dynamic and can be changed at runtime.  Array elements are stored
 *  contiguously.
 *
 *  \note For a multi-component array container, where each element
 *  is a tuple of 1 or more components, Axom provides the MCArray alias, which
 *  corresponds to Array<T, 2>.
 *
 *  The Array class mirrors std::vector, with future support for GPUs
 *  in-development.  The class's multidimensional array functionality roughly
    mirrors the multidimensional array support provided by numpy's ndarray.
 * 
 *  \see https://numpy.org/doc/stable/reference/generated/numpy.ndarray.html
 * 
 *  This class is meant to be a drop-in replacement for std::vector.
 *  However, it differs in its memory management and construction semantics.
 *  Specifically, we do not require axom::Array to initialize/construct
 *  its memory at allocation time and we use axom's memory_management
 *  and allocator ID abstractions rather than std::allocator.
 *
 *  Array always retains exclusive ownership of its data and is responsible for
 *  freeing its memory.
 *
 *  \see ArrayView for non-owning views of one- or multi-dimensional data
 *  Depending on which constructor is used, the Array object can have two
 *  different underlying storage types:
 *
 * \tparam T the type of the values to hold.
 * \tparam DIM The dimension of the array.
 * 
 * \pre T must be CopyAssignable and Erasable
 * \see https://en.cppreference.com/w/cpp/named_req
 *
 * \pre When Array is allocated on the device, T must be relocatable, i.e.
 *  moving the object to a new index and destroying the original should be
 *  equivalent to a memcpy.
 * \see https://github.com/facebook/folly/blob/main/folly/docs/FBVector.md#object-relocation
 *
 */
template <typename T, int DIM = 1, MemorySpace SPACE = MemorySpace::Dynamic>
class Array : public ArrayBase<T, DIM, Array<T, DIM, SPACE>>
{
public:
  static constexpr double DEFAULT_RESIZE_RATIO = 2.0;
  static constexpr IndexType MIN_DEFAULT_CAPACITY = 32;
  using value_type = T;
  static constexpr MemorySpace space = SPACE;
  using ArrayIterator = ArrayIteratorBase<Array<T, DIM, SPACE>, T>;
  using ConstArrayIterator =
    ArrayIteratorBase<const Array<T, DIM, SPACE>, const T>;

  using ArrayViewType = ArrayView<T, DIM, SPACE>;
  using ConstArrayViewType = ArrayView<const T, DIM, SPACE>;

private:
  using OpHelper = detail::ArrayOps<T, SPACE>;

public:
  /// \name Native Storage Array Constructors
  /// @{

  /*! 
   * \brief Default constructor. Constructs an Array instance with no elements
   *  and default allocator ID. 
   *
   */
  Array();

  /*!
   * \brief Constructs a 1D Array instance with the given number of elements.
   *
   * \param [in] num_elements the number of elements the Array holds.
   * \param [in] capacity the number of elements to allocate space for.
   * \param [in] allocator_id the ID of the allocator to use (optional)
   *
   * \note If no capacity or capacity less than num_elements is specified
   *  then it will default to at least num_elements * DEFAULT_RESIZE_RATIO.
   * \note a capacity is specified for the number of elements to store in the
   *  array and does not correspond to the actual bytesize.
   * \note The option to select a capacity is only available for 1-dimensional Arrays
   * 
   * \note Some overloads have an `ArrayOptions::Uninitialized` first parameter.
   * These are intended for cases where the array data should not be initialized
   * when memory is allocated, e.g. if the code is known to initialize the data
   *
   * \pre num_elements >= 0
   *
   * \post capacity() >= size()
   * \post size() == num_elements
   * \post getResizeRatio() == DEFAULT_RESIZE_RATIO
   */
  template <IndexType SFINAE_DIM = DIM,
            MemorySpace SFINAE_SPACE = SPACE,
            typename std::enable_if<SFINAE_DIM == 1>::type* = nullptr>
  Array(IndexType num_elements,
        IndexType capacity = 0,
        int allocator_id = axom::detail::getAllocatorID<SPACE>());

  /// \overload
  template <IndexType SFINAE_DIM = DIM,
            MemorySpace SFINAE_SPACE = SPACE,
            typename std::enable_if<SFINAE_DIM == 1>::type* = nullptr>
  Array(ArrayOptions::Uninitialized,
        IndexType num_elements,
        IndexType capacity = 0,
        int allocator_id = axom::detail::getAllocatorID<SPACE>());

  /*!
   * \brief Generic constructor for an Array of arbitrary dimension
   *
   * \param [in] args The parameter pack containing the "shape" of the Array
   * \see https://numpy.org/doc/stable/reference/generated/numpy.empty.html#numpy.empty
   *
   * \pre sizeof...(Args) == DIM
   *
   * \post capacity() >= size()
   * \post size() == num_elements
   * \post getResizeRatio() == DEFAULT_RESIZE_RATIO
   */
  template <
    typename... Args,
    typename Enable = typename std::enable_if<
      sizeof...(Args) == DIM && detail::all_types_are_integral<Args...>::value>::type>
  Array(Args... args);

  /// \overload
  template <
    typename... Args,
    typename Enable = typename std::enable_if<
      sizeof...(Args) == DIM && detail::all_types_are_integral<Args...>::value>::type>
  Array(ArrayOptions::Uninitialized, Args... args);

  /*!
   * \brief Initializer list constructor for a one-dimensional Array
   *
   * \param [in] elems The elements to initialize the array with
   * \param [in] allocator_id the ID of the allocator to use (optional)
   */
  template <int UDIM = DIM, typename Enable = typename std::enable_if<UDIM == 1>::type>
  Array(std::initializer_list<T> elems,
        int allocator_id = axom::detail::getAllocatorID<SPACE>());

  /*! 
   * \brief Copy constructor for an Array instance 
   */
  Array(const Array& other);

  /*! 
   * \brief Move constructor for an Array instance 
   */
  Array(Array&& other);

  /*!
   * \brief Constructor for transferring between memory spaces
   *
   * \param [in] other The array in a different memory space to copy from
   *
   * \note The new Array will be constructed with the allocator ID of the
   *  copied-from array, if compatible with the memory space of the target
   *  array. Otherwise, the new Array will be constructed with the default
   *  allocator for the memory space.
   *
   *  An Array specified with the default Dynamic memory space will always
   *  propagate the allocator ID from the source array.
   */
  template <typename OtherArrayType>
  Array(const ArrayBase<T, DIM, OtherArrayType>& other);

  /// \overload
  template <typename OtherArrayType>
  Array(const ArrayBase<const T, DIM, OtherArrayType>& other);

  /*!
   * \brief Constructor for transferring between memory spaces, with a user-
   *  specified allocator
   *
   * \param [in] other The array in a different memory space to copy from
   * \param [in] allocator_id the ID of the allocator to use
   *
   * \note The specified allocator ID must be compatible with the memory space
   *  of the new Array.
   */
  template <typename OtherArrayType>
  Array(const ArrayBase<T, DIM, OtherArrayType>& other, int allocator_id);

  /// \overload
  template <typename OtherArrayType>
  Array(const ArrayBase<const T, DIM, OtherArrayType>& other, int allocator_id);

  /// @}

  /// \name Array copy and move operators
  /// @{

  /*! 
   * \brief Copy assignment operator for Array
   * 
   * \pre T must be TriviallyCopyable
   */
  Array& operator=(const Array& other)
  {
    if(this != &other)
    {
      static_cast<ArrayBase<T, DIM, Array<T, DIM, SPACE>>&>(*this) = other;
      m_allocator_id = other.m_allocator_id;
      m_resize_ratio = other.m_resize_ratio;
      initialize(other.size(), other.capacity());
      // Use fill_range to ensure that copy constructors are invoked for each
      // element.
      MemorySpace srcSpace = SPACE;
      if(srcSpace == MemorySpace::Dynamic)
      {
        srcSpace = axom::detail::getAllocatorSpace(other.m_allocator_id);
      }
      OpHelper::fill_range(m_data,
                           0,
                           m_num_elements,
                           m_allocator_id,
                           other.data(),
                           srcSpace);
    }

    return *this;
  }

  /*! 
   * \brief Move assignment operator for Array
   */
  Array& operator=(Array&& other)
  {
    if(this != &other)
    {
      if(m_data != nullptr)
      {
        axom::deallocate(m_data);
      }
      static_cast<ArrayBase<T, DIM, Array<T, DIM, SPACE>>&>(*this) =
        std::move(other);

      m_data = other.m_data;
      m_num_elements = other.m_num_elements;
      m_capacity = other.m_capacity;
      m_resize_ratio = other.m_resize_ratio;
      m_allocator_id = other.m_allocator_id;

      other.m_data = nullptr;
      other.m_num_elements = 0;
      other.m_capacity = 0;
      other.m_resize_ratio = DEFAULT_RESIZE_RATIO;
      other.m_allocator_id = INVALID_ALLOCATOR_ID;
    }

    return *this;
  }

  /*!
   * \brief Initializer list assignment operator for Array.
   *
   * \param [in] elems the elements to set the array to.
   */
  template <int UDIM = DIM, typename Enable = typename std::enable_if<UDIM == 1>::type>
  Array& operator=(std::initializer_list<T> elems)
  {
    clear();
    insert(0, elems.size(), elems.begin());
    return *this;
  }

  /// @}

  /*!
   * Destructor. Frees the associated buffer.
   */
  virtual ~Array();

  /// \name Array element access operators
  /// @{

  // TODO: Implement View class for the case where sizeof...(Args) < DIM (i.e., where the indexing results in a nonscalar)

  /*!
   * \brief Return a pointer to the array of data.
   */
  /// @{

  AXOM_HOST_DEVICE inline T* data() { return m_data; }
  AXOM_HOST_DEVICE inline const T* data() const { return m_data; }

  /// @}

  /// @}

  /// \name Array methods to modify the data.
  /// @{

  /*!
   * \brief Set all the values of the array.
   *
   * \param [in] value the value to set to.
   */
  void fill(const T& value);

  /*!
   * \brief Modify the values of existing elements.
   *
   * \param [in] elements the new elements to write.
   * \param [in] n the number of elements to write.
   * \param [in] pos the position at which to begin writing.
   *
   * \note It's assumed that elements is of length n.
   * \note The size is unchanged by calls to set.
   *
   * \pre pos + n <= m_num_elements.
   */
  void set(const T* elements, IndexType n, IndexType pos);

  /*!
   * \brief Clears the contents of the array
   * 
   * \post size of Array is 0
   * \post capacity is unchanged
   */
  void clear();

  /*!
   * \brief Insert an element into the array at the given position.
   *
   * \param [in] pos the position at which to insert.
   * \param [in] value the element value to insert.
   *
   * \note Reallocation is done if the new size will exceed the capacity.
   * \note The size increases by 1.
   *
   */
  void insert(IndexType pos, const T& value);

  /*!
   * \brief Insert an element into the array at the value before pos.
   *
   * \param [in] pos the ArrayIterator before which value will be inserted.
   * \param [in] value the element value to insert.
   *
   * \note Reallocation is done if the new size will exceed the capacity.
   * \note The size increases by 1.
   *
   * \return ArrayIterator to inserted value
   */
  ArrayIterator insert(ArrayIterator pos, const T& value);

  /*!
   * \brief Insert elements into the array at the given position.
   *
   * \param [in] pos the position at which to begin the insertion.
   * \param [in] n the number of elements to insert.
   * \param [in] values the element values to insert.
   *
   * \note It's assumed that elements is of length n.
   * \note Reallocation is done if the new size will exceed the capacity.
   * \note The size increases by n.
   *
   * \pre pos <= m_num_elements.
   */
  void insert(IndexType pos, IndexType n, const T* values);

  /*!
   * \brief Insert elements into the array at the value before pos.
   *
   * \param [in] pos the ArrayIterator before which value will be inserted.
   * \param [in] n the number of elements to insert.
   * \param [in] values the element values to insert.
   *
   * \note It's assumed that elements is of length n.
   * \note Reallocation is done if the new size will exceed the capacity.
   * \note The size increases by n.
   *
   * \pre pos <= end()
   *
   * \return ArrayIterator to first element inserted (pos if n == 0)
   */
  ArrayIterator insert(ArrayIterator pos, IndexType n, const T* values);

  /*!
   * \brief Insert n copies of element into the array at the given position.
   *
   * \param [in] pos the position at which to begin the insertion.
   * \param [in] n the number of elements to insert.
   * \param [in] value the element value to insert.
   *
   * \note Reallocation is done if the new size will exceed the capacity.
   * \note The size increases by n.
   * \note This method is used to create space for elements in the middle of
   *  the array.
   *
   * \pre pos <= m_num_elements.
   */
  void insert(IndexType pos, IndexType n, const T& value);

  /*!
   * \brief Insert n copies of element into the array at the value before pos.
   *
   * \param [in] pos the ArrayIterator before which value will be inserted.
   * \param [in] n the number of elements to insert.
   * \param [in] value the element value to insert.
   *
   * \note Reallocation is done if the new size will exceed the capacity.
   * \note The size increases by n.
   * \note This method is used to create space for elements in the middle of
   *  the array.
   *
   * \pre pos <= end()
   *
   * \return ArrayIterator to first element inserted (pos if n == 0)
   */
  ArrayIterator insert(ArrayIterator pos, IndexType n, const T& value);

  /*!
   * \brief Inserts an Array to the end of the calling object
   *
   * \param [in] other The Array to append
   *
   * \pre The shapes of the calling Array and @a other are the same
   * (excluding the leading dimension), i.e., shape()[1:] == other.shape()[1:]
   *
   * \note Reallocation is done if the new size will exceed the capacity.
   */
  template <MemorySpace OtherSpace>
  void insert(IndexType pos, ArrayView<const T, DIM, OtherSpace> other);

  /// \overload
  template <MemorySpace OtherSpace>
  void insert(IndexType pos, ArrayView<T, DIM, OtherSpace> other)
  {
    insert(pos, ArrayView<const T, DIM, OtherSpace>(other));
  }

  /*!
   * \brief Appends an Array to the end of the calling object
   *
   * \param [in] other The Array to append
   * \tparam OtherArrayType The underlying type of the other array
   *
   * \note Reallocation is done if the new size will exceed the capacity.
   */
  template <MemorySpace OtherSpace>
  void append(ArrayView<const T, DIM, OtherSpace> other)
  {
    insert(size(), other);
  }

  /// \overload
  template <MemorySpace OtherSpace>
  void append(ArrayView<T, DIM, OtherSpace> other)
  {
    insert(size(), other);
  }

  /*!
   * \brief Erases an element from the Array 
   *
   * \param [in] pos the ArrayIterator to the element in the Array
   *
   * \return An ArrayIterator following the last element removed.
   */
  ArrayIterator erase(ArrayIterator pos);

  /*!
   * \brief Erases elements in the range [first, last) from the Array
   *
   * \param [in] first the ArrayIterator to the beginning of the range.
   * \param [in] last the ArrayIterator to end of range.
   *
   * \return An ArrayIterator following the last element removed. 
   */
  ArrayIterator erase(ArrayIterator first, ArrayIterator last);

  /*!
   * \brief Inserts new element into Array at the given position.
   *
   * \param [in] pos the position to insert element at.
   * \param [in] args the arguments to forward to constructor of the element.
   *
   * \note Reallocation is done if the new size will exceed the capacity.
   * \note The size increases by 1.
   *
   * \pre T must be MoveAssignable
   */
  template <typename... Args>
  void emplace(IndexType pos, Args&&... args);

  /*!
   * \brief Inserts new element into Array before pos.
   *
   * \param [in] pos the ArrayIterator to insert element before.
   * \param [in] args the arguments to forward to constructor of the element.
   *
   * \note Reallocation is done if the new size will exceed the capacity.
   * \note The size increases by 1.
   *
   * \pre T must be MoveAssignable
   *
   * \return An ArrayIterator to the emplaced element.
   */
  template <typename... Args>
  ArrayIterator emplace(ArrayIterator pos, Args&&... args);

  /*!
   * \brief Push a value to the back of the array.
   *
   * \param [in] value the value to be added to the back.
   *
   * \note Reallocation is done if the new size will exceed the capacity.
   * 
   * \pre DIM == 1
   */
  void push_back(const T& value);

  /*!
   * \brief Push a value to the back of the array.
   *
   * \param [in] value the value to move to the back.
   *
   * \note Reallocation is done if the new size will exceed the capacity.
   * 
   * \pre DIM == 1
   */
  void push_back(T&& value);

  /*!
   * \brief Inserts new element at the end of the Array.
   *
   * \param [in] args the arguments to forward to constructor of the element.
   *
   * \note Reallocation is done if the new size will exceed the capacity.
   * \note The size increases by 1.
   * 
   * \pre DIM == 1
   */
  template <typename... Args>
  void emplace_back(Args&&... args);

  /// @}

  /// \name Array methods to query and set attributes
  /// @{

  /*!
   * \brief Return the number of elements allocated for the data array.
   */
  IndexType capacity() const { return m_capacity; }

  /*!
   * \brief Increase the capacity. Does nothing if the new capacity is less
   *  than the current capacity.
   *
   * \param [in] capacity the new number of elements to allocate.
   */
  void reserve(IndexType capacity)
  {
    if(capacity > m_capacity)
    {
      setCapacity(capacity);
    }
  }

  /*!
   * \brief Returns an ArrayIterator to the first element of the Array
   */
  ArrayIterator begin() { return ArrayIterator(0, this); }

  /// \overload
  ConstArrayIterator begin() const { return ConstArrayIterator(0, this); }

  /*!
   * \brief Returns an ArrayIterator to the element following the last
   *  element of the Array.
   */
  ArrayIterator end() { return ArrayIterator(size(), this); }

  /// \overload
  ConstArrayIterator end() const { return ConstArrayIterator(size(), this); }

  /*!
   * \brief Shrink the capacity to be equal to the size.
   */
  void shrink() { setCapacity(m_num_elements); }

  /*!
   * \brief Returns true iff the Array stores no elements.
   *
   * \note If the Array is empty the capacity can still be greater than zero.
   */
  bool empty() const { return m_num_elements == 0; }

  /*!
   * \brief Return the number of elements stored in the data array.
   */
  AXOM_HOST_DEVICE inline IndexType size() const { return m_num_elements; }

  /*!
   * \brief Update the number of elements stored in the data array.
   *
   * \note Reallocation is done if the new size will exceed the capacity.
   */
  template <typename... Args>
  void resize(Args... args)
  {
    static_assert(sizeof...(Args) == DIM,
                  "Array size must match number of dimensions");
    const StackArray<IndexType, DIM> dims {static_cast<IndexType>(args)...};
    resize(dims, true);
  }

  /// \overload
  template <typename... Args>
  void resize(ArrayOptions::Uninitialized, Args... args)
  {
    static_assert(sizeof...(Args) == DIM,
                  "Array size must match number of dimensions");
    const StackArray<IndexType, DIM> dims {static_cast<IndexType>(args)...};
    resize(dims, false);
  }

  /*!
   * \brief Exchanges the contents of this Array with the other.
   */
  void swap(Array<T, DIM, SPACE>& other);

  /*!
   * \brief Get the ratio by which the capacity increases upon dynamic resize.
   */
  double getResizeRatio() const { return m_resize_ratio; }

  /*!
   * \brief Set the ratio by which the capacity increases upon dynamic resize.
   *
   * \param [in] ratio the new resize ratio.
   */
  void setResizeRatio(double ratio) { m_resize_ratio = ratio; }

  /*!
   * \brief Get the ID for the umpire allocator
   */
  int getAllocatorID() const { return m_allocator_id; }

  /*!
   * \brief Returns a view of the array
   * \sa ArrayView
   */
  ArrayViewType view() { return ArrayViewType(*this); }
  /// \overload
  ConstArrayViewType view() const { return ConstArrayViewType(*this); }

  /// @}

protected:
  /*!
   * \brief Initialize an Array instance with the given number of elements.
   *
   * \param [in] num_elements the number of elements the Array holds.
   * \param [in] capacity the number of elements to allocate space for.
   * \param [in] should_default_construct whether to create default-constructed
   *  objects in the region [0, num_elements). Defaults to true.
   *
   * \note If no capacity or capacity less than num_elements is specified
   *  then it will default to at least num_elements * DEFAULT_RESIZE_RATIO.
   * \note a capacity is specified for the number of elements to store in the
   *  array and does not correspond to the actual bytesize.
   *
   * \pre num_elements >= 0
   *
   * \post capacity() >= size()
   * \post size() == num_elements
   * \post getResizeRatio() == DEFAULT_RESIZE_RATIO
   */
  void initialize(IndexType num_elements,
                  IndexType capacity,
                  bool should_default_construct = true);

  /*!
   * \brief Helper function for initializing an Array instance with an existing
   *  range of elements.
   *
   * \param [in] data pointer to the existing array of elements
   * \param [in] num_elements the number of elements in the existing array
   * \param [in] data_space the memory space in which data has been allocated
   * \param [in] user_provided_allocator true if the Array's allocator ID was
   *  provided by the user
   */
  void initialize_from_other(const T* data,
                             IndexType num_elements,
                             MemorySpace data_space,
                             bool user_provided_allocator);

  /*!
   * \brief Updates the number of elements stored in the data array.
   *
   * \param [in] dims the number of elements to allocate in each dimension
   * \param [in] default_construct if true, default-constructs any new elements
   *  in the array
   */
  void resize(const StackArray<IndexType, DIM>& dims, bool default_construct);

  /*!
   * \brief Make space for a subsequent insertion into the array.
   *
   * \param [in] n the number of elements to insert.
   * \param [in] pos the position at which to begin the insertion.
   *
   * \return a pointer to the beginning of the insertion space.
   *
   * \note Reallocation is done if the new size will exceed the capacity.
   */
  T* reserveForInsert(IndexType n, IndexType pos);

  /*!
   * \brief Update the number of elements.
   *
   * \param [in] new_num_elements the new number of elements.
   */
  virtual void updateNumElements(IndexType new_num_elements);

  /*!
   * \brief Set the number of elements allocated for the data array.
   *
   * \param [in] capacity the new number of elements to allocate.
   */
  virtual void setCapacity(IndexType new_capacity);

  /*!
   * \brief Reallocates the data array when the size exceeds the capacity.
   *
   * \param [in] new_num_elements the number of elements which exceeds the
   *  current capacity.
   */
  virtual void dynamicRealloc(IndexType new_num_elements);

  T* m_data = nullptr;
  /// \brief The full number of elements in the array
  ///  i.e., 3 for a 1D Array of size 3, 9 for a 3x3 2D array, etc
  IndexType m_num_elements = 0;
  IndexType m_capacity = 0;
  double m_resize_ratio = DEFAULT_RESIZE_RATIO;
  int m_allocator_id;
};

/// \brief Helper alias for multi-component arrays
template <typename T>
using MCArray = Array<T, 2>;

//------------------------------------------------------------------------------
//                            Array IMPLEMENTATION
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template <typename T, int DIM, MemorySpace SPACE>
Array<T, DIM, SPACE>::Array()
  : m_allocator_id(axom::detail::getAllocatorID<SPACE>())
{ }

//------------------------------------------------------------------------------
template <typename T, int DIM, MemorySpace SPACE>
template <typename... Args, typename Enable>
Array<T, DIM, SPACE>::Array(Args... args)
  : ArrayBase<T, DIM, Array<T, DIM, SPACE>>({args...})
  , m_allocator_id(axom::detail::getAllocatorID<SPACE>())
{
  static_assert(sizeof...(Args) == DIM,
                "Array size must match number of dimensions");
  // Intel hits internal compiler error when casting as part of function call
  const IndexType tmp_args[] = {args...};
  assert(detail::allNonNegative(tmp_args));
  initialize(detail::packProduct(tmp_args), 0);
}

//------------------------------------------------------------------------------
template <typename T, int DIM, MemorySpace SPACE>
template <typename... Args, typename Enable>
Array<T, DIM, SPACE>::Array(ArrayOptions::Uninitialized, Args... args)
  : ArrayBase<T, DIM, Array<T, DIM, SPACE>>({args...})
  , m_allocator_id(axom::detail::getAllocatorID<SPACE>())
{
  static_assert(sizeof...(Args) == DIM,
                "Array size must match number of dimensions");
  // Intel hits internal compiler error when casting as part of function call
  const IndexType tmp_args[] = {args...};
  assert(detail::allNonNegative(tmp_args));
  initialize(detail::packProduct(tmp_args), 0, false);
}

//------------------------------------------------------------------------------
template <typename T, int DIM, MemorySpace SPACE>
template <IndexType SFINAE_DIM,
          MemorySpace SFINAE_SPACE,
          typename std::enable_if<SFINAE_DIM == 1>::type*>
Array<T, DIM, SPACE>::Array(IndexType num_elements,
                            IndexType capacity,
                            int allocator_id)
  : m_allocator_id(allocator_id)
{
  // If a memory space has been explicitly set for the Array object, check that
  // the space of the user-provided allocator matches the explicit space.
  if(SPACE != MemorySpace::Dynamic &&
     SPACE != axom::detail::getAllocatorSpace(m_allocator_id))
  {
#ifdef AXOM_DEBUG
    std::cerr << "Incorrect allocator ID was provided for an Array object with "
                 "explicit memory space - using default for space\n";
#endif
    m_allocator_id = axom::detail::getAllocatorID<SPACE>();
  }
  initialize(num_elements, capacity);
}

//------------------------------------------------------------------------------
template <typename T, int DIM, MemorySpace SPACE>
template <IndexType SFINAE_DIM,
          MemorySpace SFINAE_SPACE,
          typename std::enable_if<SFINAE_DIM == 1>::type*>
Array<T, DIM, SPACE>::Array(ArrayOptions::Uninitialized,
                            IndexType num_elements,
                            IndexType capacity,
                            int allocator_id)
  : m_allocator_id(allocator_id)
{
  // If a memory space has been explicitly set for the Array object, check that
  // the space of the user-provided allocator matches the explicit space.
  if(SPACE != MemorySpace::Dynamic &&
     SPACE != axom::detail::getAllocatorSpace(m_allocator_id))
  {
#ifdef AXOM_DEBUG
    std::cerr << "Incorrect allocator ID was provided for an Array object with "
                 "explicit memory space - using default for space\n";
#endif
    m_allocator_id = axom::detail::getAllocatorID<SPACE>();
  }
  initialize(num_elements, capacity, false);
}

//------------------------------------------------------------------------------
template <typename T, int DIM, MemorySpace SPACE>
template <int UDIM, typename Enable>
Array<T, DIM, SPACE>::Array(std::initializer_list<T> elems, int allocator_id)
  : m_allocator_id(allocator_id)
{
  initialize_from_other(elems.begin(), elems.size(), MemorySpace::Dynamic, true);
}

//------------------------------------------------------------------------------
template <typename T, int DIM, MemorySpace SPACE>
Array<T, DIM, SPACE>::Array(const Array& other)
  : ArrayBase<T, DIM, Array<T, DIM, SPACE>>(
      static_cast<const ArrayBase<T, DIM, Array<T, DIM, SPACE>>&>(other))
  , m_allocator_id(other.m_allocator_id)
{
  initialize(other.size(), other.capacity());
  // Use fill_range to ensure that copy constructors are invoked for each
  // element.
  MemorySpace srcSpace = SPACE;
  if(srcSpace == MemorySpace::Dynamic)
  {
    srcSpace = axom::detail::getAllocatorSpace(other.m_allocator_id);
  }
  OpHelper::fill_range(m_data,
                       0,
                       m_num_elements,
                       m_allocator_id,
                       other.data(),
                       srcSpace);
}

//------------------------------------------------------------------------------
template <typename T, int DIM, MemorySpace SPACE>
Array<T, DIM, SPACE>::Array(Array&& other)
  : ArrayBase<T, DIM, Array<T, DIM, SPACE>>(
      static_cast<ArrayBase<T, DIM, Array<T, DIM, SPACE>>&&>(std::move(other)))
  , m_resize_ratio(0.0)
{
  m_data = other.m_data;
  m_num_elements = other.m_num_elements;
  m_capacity = other.m_capacity;
  m_resize_ratio = other.m_resize_ratio;
  m_allocator_id = other.m_allocator_id;

  other.m_data = nullptr;
  other.m_num_elements = 0;
  other.m_capacity = 0;
  other.m_resize_ratio = DEFAULT_RESIZE_RATIO;
  other.m_allocator_id = INVALID_ALLOCATOR_ID;
}

//------------------------------------------------------------------------------
template <typename T, int DIM, MemorySpace SPACE>
template <typename OtherArrayType>
Array<T, DIM, SPACE>::Array(const ArrayBase<T, DIM, OtherArrayType>& other)
  : ArrayBase<T, DIM, Array<T, DIM, SPACE>>(other)
  , m_allocator_id(static_cast<const OtherArrayType&>(other).getAllocatorID())
{
  initialize_from_other(static_cast<const OtherArrayType&>(other).data(),
                        static_cast<const OtherArrayType&>(other).size(),
                        axom::detail::getAllocatorSpace(m_allocator_id),
                        false);
}

//------------------------------------------------------------------------------
template <typename T, int DIM, MemorySpace SPACE>
template <typename OtherArrayType>
Array<T, DIM, SPACE>::Array(const ArrayBase<const T, DIM, OtherArrayType>& other)
  : ArrayBase<T, DIM, Array<T, DIM, SPACE>>(other)
  , m_allocator_id(static_cast<const OtherArrayType&>(other).getAllocatorID())
{
  initialize_from_other(static_cast<const OtherArrayType&>(other).data(),
                        static_cast<const OtherArrayType&>(other).size(),
                        axom::detail::getAllocatorSpace(m_allocator_id),
                        false);
}

//------------------------------------------------------------------------------
template <typename T, int DIM, MemorySpace SPACE>
template <typename OtherArrayType>
Array<T, DIM, SPACE>::Array(const ArrayBase<T, DIM, OtherArrayType>& other,
                            int allocatorId)
  : ArrayBase<T, DIM, Array<T, DIM, SPACE>>(other)
  , m_allocator_id(allocatorId)
{
  int src_allocator = static_cast<const OtherArrayType&>(other).getAllocatorID();

  initialize_from_other(static_cast<const OtherArrayType&>(other).data(),
                        static_cast<const OtherArrayType&>(other).size(),
                        axom::detail::getAllocatorSpace(src_allocator),
                        true);
}

//------------------------------------------------------------------------------
template <typename T, int DIM, MemorySpace SPACE>
template <typename OtherArrayType>
Array<T, DIM, SPACE>::Array(const ArrayBase<const T, DIM, OtherArrayType>& other,
                            int allocatorId)
  : ArrayBase<T, DIM, Array<T, DIM, SPACE>>(other)
  , m_allocator_id(allocatorId)
{
  int src_allocator = static_cast<const OtherArrayType&>(other).getAllocatorID();

  initialize_from_other(static_cast<const OtherArrayType&>(other).data(),
                        static_cast<const OtherArrayType&>(other).size(),
                        axom::detail::getAllocatorSpace(src_allocator),
                        true);
}

//------------------------------------------------------------------------------
template <typename T, int DIM, MemorySpace SPACE>
Array<T, DIM, SPACE>::~Array()
{
  clear();
  if(m_data != nullptr)
  {
    axom::deallocate(m_data);
  }

  m_data = nullptr;
}

//------------------------------------------------------------------------------
template <typename T, int DIM, MemorySpace SPACE>
inline void Array<T, DIM, SPACE>::fill(const T& value)
{
  OpHelper::destroy(m_data, 0, m_num_elements, m_allocator_id);
  OpHelper::fill(m_data, 0, m_num_elements, m_allocator_id, value);
}

//------------------------------------------------------------------------------
template <typename T, int DIM, MemorySpace SPACE>
inline void Array<T, DIM, SPACE>::set(const T* elements, IndexType n, IndexType pos)
{
  assert(elements != nullptr);
  assert(pos >= 0);
  assert(pos + n <= m_num_elements);

  OpHelper::destroy(m_data, pos, n, m_allocator_id);
  OpHelper::fill_range(m_data, pos, n, m_allocator_id, elements, MemorySpace::Dynamic);
}

//------------------------------------------------------------------------------
template <typename T, int DIM, MemorySpace SPACE>
inline void Array<T, DIM, SPACE>::clear()
{
  OpHelper::destroy(m_data, 0, m_num_elements, m_allocator_id);

  updateNumElements(0);
}

//------------------------------------------------------------------------------
template <typename T, int DIM, MemorySpace SPACE>
inline void Array<T, DIM, SPACE>::insert(IndexType pos, const T& value)
{
  static_assert(DIM == 1, "Insertion not supported for multidimensional Arrays");
  reserveForInsert(1, pos);

  OpHelper::emplace(m_data, pos, m_allocator_id, value);
}

//------------------------------------------------------------------------------
template <typename T, int DIM, MemorySpace SPACE>
inline typename Array<T, DIM, SPACE>::ArrayIterator Array<T, DIM, SPACE>::insert(
  Array<T, DIM, SPACE>::ArrayIterator pos,
  const T& value)
{
  static_assert(DIM == 1, "Insertion not supported for multidimensional Arrays");
  assert(pos >= begin() && pos <= end());
  insert(pos - begin(), value);
  return pos;
}

//------------------------------------------------------------------------------
template <typename T, int DIM, MemorySpace SPACE>
inline void Array<T, DIM, SPACE>::insert(IndexType pos, IndexType n, const T* values)
{
  assert(values != nullptr);
  reserveForInsert(n, pos);
  OpHelper::fill_range(m_data, pos, n, m_allocator_id, values, MemorySpace::Dynamic);
}

//------------------------------------------------------------------------------
template <typename T, int DIM, MemorySpace SPACE>
inline typename Array<T, DIM, SPACE>::ArrayIterator Array<T, DIM, SPACE>::insert(
  Array<T, DIM, SPACE>::ArrayIterator pos,
  IndexType n,
  const T* values)
{
  static_assert(DIM == 1, "Insertion not supported for multidimensional Arrays");
  assert(pos >= begin() && pos <= end());
  insert(pos - begin(), n, values);
  return pos;
}

//------------------------------------------------------------------------------
template <typename T, int DIM, MemorySpace SPACE>
inline void Array<T, DIM, SPACE>::insert(IndexType pos, IndexType n, const T& value)
{
  static_assert(DIM == 1, "Insertion not supported for multidimensional Arrays");
  reserveForInsert(n, pos);
  OpHelper::fill(m_data, pos, n, m_allocator_id, value);
}

//------------------------------------------------------------------------------
template <typename T, int DIM, MemorySpace SPACE>
inline typename Array<T, DIM, SPACE>::ArrayIterator Array<T, DIM, SPACE>::insert(
  Array<T, DIM, SPACE>::ArrayIterator pos,
  IndexType n,
  const T& value)
{
  static_assert(DIM == 1, "Insertion not supported for multidimensional Arrays");
  assert(pos >= begin() && pos <= end());
  insert(pos - begin(), n, value);
  return pos;
}

//------------------------------------------------------------------------------
template <typename T, int DIM, MemorySpace SPACE>
template <MemorySpace OtherSpace>
inline void Array<T, DIM, SPACE>::insert(IndexType pos,
                                         ArrayView<const T, DIM, OtherSpace> other)
{
  // First update the dimensions
  this->updateShapeOnInsert(other.shape());
  // Then add the raw data to the buffer
  insert(pos, other.size(), other.data());
}

//------------------------------------------------------------------------------
template <typename T, int DIM, MemorySpace SPACE>
inline typename Array<T, DIM, SPACE>::ArrayIterator Array<T, DIM, SPACE>::erase(
  Array<T, DIM, SPACE>::ArrayIterator pos)
{
  assert(pos >= begin() && pos < end());

  IndexType posIdx = pos - begin();

  // Destroy element at posIdx and shift elements over by 1
  OpHelper::destroy(m_data, posIdx, 1, m_allocator_id);
  OpHelper::move(m_data, posIdx + 1, m_num_elements, posIdx, m_allocator_id);
  updateNumElements(m_num_elements - 1);

  return ArrayIterator(posIdx, this);
}

//------------------------------------------------------------------------------
template <typename T, int DIM, MemorySpace SPACE>
inline typename Array<T, DIM, SPACE>::ArrayIterator Array<T, DIM, SPACE>::erase(
  Array<T, DIM, SPACE>::ArrayIterator first,
  Array<T, DIM, SPACE>::ArrayIterator last)
{
  assert(first >= begin() && first < end());
  assert(last >= first && last <= end());

  // Empty range, return last
  if(first == last)
  {
    return last;
  }

  // Erase [first,last) elements
  IndexType firstIdx = first - begin();
  IndexType lastIdx = last - begin();
  IndexType nelems = last - first;
  OpHelper::destroy(m_data, firstIdx, nelems, m_allocator_id);

  // Shift [last, end) elements over
  OpHelper::move(m_data, lastIdx, m_num_elements, firstIdx, m_allocator_id);

  IndexType count = lastIdx - firstIdx;
  updateNumElements(m_num_elements - count);
  return ArrayIterator(firstIdx, this);
}

//------------------------------------------------------------------------------
template <typename T, int DIM, MemorySpace SPACE>
template <typename... Args>
inline void Array<T, DIM, SPACE>::emplace(IndexType pos, Args&&... args)
{
  reserveForInsert(1, pos);
  OpHelper::emplace(m_data, pos, m_allocator_id, std::forward<Args>(args)...);
}

//------------------------------------------------------------------------------
template <typename T, int DIM, MemorySpace SPACE>
template <typename... Args>
inline typename Array<T, DIM, SPACE>::ArrayIterator Array<T, DIM, SPACE>::emplace(
  Array<T, DIM, SPACE>::ArrayIterator pos,
  Args&&... args)
{
  assert(pos >= begin() && pos <= end());
  emplace(pos - begin(), std::forward<Args>(args)...);
  return pos;
}

//------------------------------------------------------------------------------
template <typename T, int DIM, MemorySpace SPACE>
inline void Array<T, DIM, SPACE>::push_back(const T& value)
{
  static_assert(DIM == 1, "push_back is only supported for 1D arrays");
  emplace_back(value);
}

//------------------------------------------------------------------------------
template <typename T, int DIM, MemorySpace SPACE>
inline void Array<T, DIM, SPACE>::push_back(T&& value)
{
  static_assert(DIM == 1, "push_back is only supported for 1D arrays");
  emplace_back(std::move(value));
}

//------------------------------------------------------------------------------
template <typename T, int DIM, MemorySpace SPACE>
template <typename... Args>
inline void Array<T, DIM, SPACE>::emplace_back(Args&&... args)
{
  static_assert(DIM == 1, "emplace_back is only supported for 1D arrays");
  emplace(size(), std::forward<Args>(args)...);
}

//------------------------------------------------------------------------------
template <typename T, int DIM, MemorySpace SPACE>
inline void Array<T, DIM, SPACE>::resize(const StackArray<IndexType, DIM>& dims,
                                         bool default_construct)
{
  assert(detail::allNonNegative(dims.m_data));
  const auto new_num_elements = detail::packProduct(dims.m_data);

  static_cast<ArrayBase<T, DIM, Array<T, DIM, SPACE>>&>(*this) =
    ArrayBase<T, DIM, Array<T, DIM, SPACE>> {dims};

  const IndexType prev_num_elements = m_num_elements;

  if(new_num_elements > m_capacity)
  {
    dynamicRealloc(new_num_elements);
  }

  if(prev_num_elements < new_num_elements && default_construct)
  {
    // Default-initialize the new elements
    OpHelper::init(m_data,
                   prev_num_elements,
                   new_num_elements - prev_num_elements,
                   m_allocator_id);
  }
  else if(prev_num_elements > new_num_elements)
  {
    // Destroy any elements above new_num_elements
    OpHelper::destroy(m_data,
                      new_num_elements,
                      prev_num_elements - new_num_elements,
                      m_allocator_id);
  }

  updateNumElements(new_num_elements);
}

//------------------------------------------------------------------------------
template <typename T, int DIM, MemorySpace SPACE>
inline void Array<T, DIM, SPACE>::swap(Array<T, DIM, SPACE>& other)
{
  ArrayBase<T, DIM, Array<T, DIM, SPACE>>::swap(other);
  axom::utilities::swap(m_data, other.m_data);
  axom::utilities::swap(m_num_elements, other.m_num_elements);
  axom::utilities::swap(m_capacity, other.m_capacity);
  axom::utilities::swap(m_resize_ratio, other.m_resize_ratio);
  axom::utilities::swap(m_allocator_id, other.m_allocator_id);
}

//------------------------------------------------------------------------------
template <typename T, int DIM, MemorySpace SPACE>
inline void Array<T, DIM, SPACE>::initialize(IndexType num_elements,
                                             IndexType capacity,
                                             bool default_construct)
{
  assert(num_elements >= 0);

  if(capacity < 0 || num_elements > capacity)
  {
    capacity = 0;
  }

  if(capacity == 0)
  {
    capacity = (num_elements > MIN_DEFAULT_CAPACITY) ? num_elements
                                                     : MIN_DEFAULT_CAPACITY;
  }
  setCapacity(capacity);
  if(default_construct)
  {
    OpHelper::init(m_data, 0, num_elements, m_allocator_id);
  }
  updateNumElements(num_elements);

  // quick checks
  assert(m_data != nullptr);
  assert(m_num_elements >= 0);
  assert(m_capacity >= m_num_elements);
}

//------------------------------------------------------------------------------
template <typename T, int DIM, MemorySpace SPACE>
inline void Array<T, DIM, SPACE>::initialize_from_other(
  const T* other_data,
  IndexType num_elements,
  MemorySpace other_data_space,
  bool AXOM_DEBUG_PARAM(user_provided_allocator))
{
  // If a memory space has been explicitly set for the Array object, check that
  // the space of the user-provided allocator matches the explicit space.
  if(SPACE != MemorySpace::Dynamic &&
     SPACE != axom::detail::getAllocatorSpace(m_allocator_id))
  {
#ifdef AXOM_DEBUG
    if(user_provided_allocator)
    {
      std::cerr << "Incorrect allocator ID was provided for an Array object "
                   "with explicit memory space - using default for space\n";
    }
#endif
    m_allocator_id = axom::detail::getAllocatorID<SPACE>();
  }
  initialize(num_elements, num_elements);
  // Use fill_range to ensure that copy constructors are invoked for each
  // element.
  OpHelper::fill_range(m_data,
                       0,
                       m_num_elements,
                       m_allocator_id,
                       other_data,
                       other_data_space);
}

//------------------------------------------------------------------------------
template <typename T, int DIM, MemorySpace SPACE>
inline T* Array<T, DIM, SPACE>::reserveForInsert(IndexType n, IndexType pos)
{
  assert(n >= 0);
  assert(pos >= 0);
  assert(pos <= m_num_elements);

  if(n == 0)
  {
    return m_data + pos;
  }

  IndexType new_size = m_num_elements + n;
  if(new_size > m_capacity)
  {
    dynamicRealloc(new_size);
  }

  OpHelper::move(m_data, pos, m_num_elements, pos + n, m_allocator_id);

  updateNumElements(new_size);
  return m_data + pos;
}

//------------------------------------------------------------------------------
template <typename T, int DIM, MemorySpace SPACE>
inline void Array<T, DIM, SPACE>::updateNumElements(IndexType new_num_elements)
{
  assert(new_num_elements >= 0);
  assert(new_num_elements <= m_capacity);

  m_num_elements = new_num_elements;
}

//------------------------------------------------------------------------------
template <typename T, int DIM, MemorySpace SPACE>
inline void Array<T, DIM, SPACE>::setCapacity(IndexType new_capacity)
{
  assert(new_capacity >= 0);

  if(new_capacity < m_num_elements)
  {
    // No need to default-initialize here because this is only
    // when the array is being shrunk
    updateNumElements(new_capacity);
  }

  m_data = axom::reallocate<T>(m_data, new_capacity, m_allocator_id);
  m_capacity = new_capacity;

  assert(m_data != nullptr || m_capacity <= 0);
}

//------------------------------------------------------------------------------
template <typename T, int DIM, MemorySpace SPACE>
inline void Array<T, DIM, SPACE>::dynamicRealloc(IndexType new_num_elements)
{
  assert(m_resize_ratio >= 1.0);
  IndexType new_capacity = new_num_elements * m_resize_ratio + 0.5;
  const IndexType block_size = this->blockSize();
  const IndexType remainder = new_capacity % block_size;
  if(remainder != 0)
  {
    new_capacity += block_size - remainder;
  }

  if(m_resize_ratio < 1.0)
  {
    std::cerr << "ERROR: resize ratio must be greater than 1.0.\n";
    std::cerr << "Set a valid resize ratio via calling setResizeRatio() with "
              << "an appropriate value.\n";

    utilities::processAbort();
  }

  m_data = axom::reallocate<T>(m_data, new_capacity, m_allocator_id);
  m_capacity = new_capacity;

  assert(m_data != nullptr || m_capacity <= 0);
}

} /* namespace axom */

#endif /* AXOM_ARRAY_HPP_ */
