/*
 * Copyright (c) 2015, Lawrence Livermore National Security, LLC.
 * Produced at the Lawrence Livermore National Laboratory.
 *
 * All rights reserved.
 *
 * This source code cannot be distributed without permission and
 * further review from Lawrence Livermore National Laboratory.
 */

/*!
 ******************************************************************************
 *
 * \file
 *
 * \brief   Header file containing definition of DataView class.
 *
 ******************************************************************************
 */

#ifndef DATAVIEW_HPP_
#define DATAVIEW_HPP_

// Standard C++ headers
#include <string>

// Other CS Toolkit headers
#include "common/CommonTypes.hpp"
#include "slic/slic.hpp"

// SiDRe project headers
#include "sidre/SidreTypes.hpp"

namespace asctoolkit
{
namespace sidre
{

// using directives to make Conduit usage easier and less visible
using conduit::Node;
using conduit::Schema;

class DataBuffer;
class DataGroup;
class DataStore;

/*!
 * \class DataView
 *
 * \brief DataView provides a "view" into data, which may be owned by a
 *        DataBuffer object or owned externally.
 *
 * The DataView class has the following properties:
 *
 *    - DataView objects can only be created via the DataGroup interface,
 *      not directly. The view object is owned by the DataGroup object that
 *      creates it.
 *    - A DataView object has a unique name (string) within the DataGroup
 *      that owns it.
 *    - A DataView holds a pointer to the DataGroup that created it and which
 *      owns it.
 *    - A DataView object can refer to data in one of four ways:
 *        # It can decribe a view into data owned by a DataBuffer.
 *        # A view can be created to describe and allocate data, using 
 *          semantics similar to DataBuffer data declaration and allocation. 
 *          In this case, no other view is allowed to allocate, reallocate,
 *          or deallocate the buffer.
 *        # It can hold a pointer to an "external" data object. In this case,
 *          the view can describe the data as though it owns it and view
 *          operations are essentially the same as the previous two cases.
 *        # It can hold a pointer to an "opaque" data object. In this case,
 *          the view knows nothing about the structure of the data; the view
 *          is essentially a handle to the data.
 *    - For any DataView object that is "external" or associated with a 
 *      DataBuffer, the data description of the view may be specified, or
 *      changed, by calling one of the apply() methods.  
 *
 */
class DataView
{
public:

  //
  // Friend declaration to constrain usage via controlled access to
  // private members.
  //
  friend class DataGroup;

//@{
//!  @name DataView allocation methods

  /*!
   * \brief Allocate data for a view, previously defined.
   *
   * NOTE: Allocation from a view is only allowed if it is the only
   *       view associated with its buffer, or if it is not opaque or
   *       external. If either condition is true, this method does nothing.
   *
   * \return pointer to this DataView object.
   */
  DataView * allocate();

  /*!
   * \brief Allocate data for view with given type and number of elements.
   *
   * NOTE: Allocation from a view is only allowed if it is the only
   *       view associated with its buffer, or if it is not opaque or
   *       external. If either condition is true, or given number of 
   *       elements is < 0, this method does nothing.
   *
   * \return pointer to this DataView object.
   */
  DataView * allocate( TypeID type, SidreLength num_elems);

  /*!
   * \brief Allocate data for view described by a Conduit data type object.
   *
   * NOTE: Allocation from a view is only allowed if it is the only
   *       view associated with its buffer, or if it is not opaque or
   *       external. If either condition is true, this method does nothing.
   *
   * \return pointer to this DataView object.
   */
  DataView * allocate(const DataType& dtype);

  /*!
   * \brief Allocate data for view described by a Conduit schema object.
   *
   * NOTE: Allocation from a view is only allowed if it is the only
   *       view associated with its buffer, or if it is not opaque or
   *       external. If either condition is true, this method does nothing.
   *
   * \return pointer to this DataView object.
   */
  DataView * allocate(const Schema& schema);

  /*!
   * \brief  Reallocate data for view to given number of elements (type
   *         stays the same).
   *
   * NOTE: Reallocation from a view is only allowed if it is the only
   *       view associated with its buffer, or if it is not opaque or
   *       external. If either condition is true, or given number of 
   *       elements is < 0, this method does nothing.
   *
   * \return pointer to this DataView object.
   */
  DataView * reallocate(SidreLength num_elems);

  /*!
   * \brief  Reallocate data for view as specified by Conduit data type object.
   *
   * NOTE: Reallocation from a view is only allowed if it is the only
   *       view associated with its buffer, or if it is not opaque or
   *       external. If either condition is true, this method does nothing.
   *
   * NOTE: The current type of the view data must match that of the given 
   *       data type object. If not, the method does nothing.
   *
   * \return pointer to this DataView object.
   */
  DataView * reallocate(const DataType& dtype);

  /*!
   * \brief  Reallocate data for view as specified by Conduit schema object.
   *
   * NOTE: Reallocation from a view is only allowed if it is the only
   *       view associated with its buffer, or if it is not opaque or
   *       external. If either condition is true, this method does nothing.
   *
   * NOTE: The current type of the view data must match that of the given 
   *       schema object. If not, the method does nothing.
   *
   * \return pointer to this DataView object.
   */
  DataView * reallocate(const Schema& schema);

//@}

  /*!
   * \brief Attach DataBuffer object to data view.
   *
   * Note that view cannot be used to access data in buffer until it
   * is described by calling an apply() method.
   *
   * If data view is opaque, already associated with a buffer, or given 
   * buffer pointer is null, method does nothing.
   *
   * \return pointer to this DataView object.
   */
  DataView * attachBuffer( DataBuffer * buff );


//@{
//!  @name DataView apply (data description) methods

  /*!
   * \brief Apply data description of a previously declared to data view.
   *
   * If view is opaque, the method does nothing.
   *
   * \return pointer to this DataView object.
   */
  DataView * apply();

  /*!
   * \brief Apply data description defined by number of elements, and
   *        optionally offset and stride to data view (type remains the same).
   *
   * NOTE: The units for offset and stride are in number of elements, which
   *       is different than the DataType usage below where offset and stride
   *       are in number of bytes.
   *
   * IMPORTANT: If view has been previously declared (or applied), this 
   *            operation will apply the new data description to the view.
   *
   * IMPORTANT: If view has no data buffer object attached, this method 
   *            does nothing because it doesn't know type information.
   *
   * If given number of elements < 0, offset < 0, or view is opaque, the
   * method also does nothing.
   *
   * \return pointer to this DataView object.
   */
  DataView * apply( SidreLength num_elems,
                    SidreLength offset = 0,
                    SidreLength stride = 1);

  /*!
   * \brief Apply data description defined by type and number of elements, and 
   *        optionally offset and stride to data view.
   *
   * NOTE: The units for offset and stride are in number of elements, which
   *       is different than the DataType usage below where offset and stride
   *       are in number of bytes.
   *
   * IMPORTANT: If view has been previously declared (or applied), this 
   *            operation will apply the new data description to the view.
   *
   * If given number of elements < 0, offset < 0, or view is opaque, the
   * method does nothing.
   *
   * \return pointer to this DataView object.
   */
  DataView * apply( TypeID type, SidreLength num_elems,
                                 SidreLength offset = 0,
                                 SidreLength stride = 1);

  /*!
   * \brief Apply data description of given Conduit data type to data view.
   *
   * If view is opaque, the method does nothing.
   *
   * \return pointer to this DataView object.
   */
  DataView * apply(const DataType& dtype);

  /*!
   * \brief Apply data description of given Conduit schema to data view.
   *        this DataView object
   *
   * If view is opaque, the method does nothing.
   *
   * \return pointer to this DataView object.
   */
  DataView * apply(const Schema& schema);

//@}

//@{
//!  @name DataView query methods

  /*!
   * \brief Return true if view has a an associated DataBuffer object
   *        (e.g., view is not opaque, it has been created with a buffer,
   *         it has been allocated, etc.); false otherwise.
   */
  bool  hasBuffer() const
  {
    return m_data_buffer != ATK_NULLPTR;
  }

  /*!
   * \brief Return true if view is opaque (i.e., view has no knowledge 
   *        of the data it holds and can only return a void* pointer to it);
   *        false otherwise.
   */
  bool  isOpaque() const
  {
    return m_is_opaque;
  }

  /*!
   * \brief Return true if data declaration has been applied to data
   *        in buffer associated with view; false otherwise.
   */
  bool isApplied() const
  {
    return m_is_applied;
  }

  /*!
   * \brief Return total number of bytes associated with this DataView object.
   *
   * IMPORTANT: This is the total bytes described by the view; they may not 
   *            yet be allocated.
   */
  size_t getTotalBytes() const
  {
    return m_schema.total_bytes();
  }

  /*!
   * \brief Return total number of elements described by this DataView object.
   *
   * IMPORTANT: This is the number of elements described by the view; 
   *            they may not yet be allocated.
   */
  size_t getNumElements() const
  {
    return m_schema.dtype().number_of_elements();
  }

//@}


//@{
//!  @name Accessor methods

  /*!
   * \brief Return const reference to name of DataView.
   */
  const std::string& getName() const
  {
    return m_name;
  }

  /*!
   * \brief Return pointer to non-const DataGroup that owns DataView object.
   */
  DataGroup * getOwningGroup()
  {
    return m_owning_group;
  }

  /*!
   * \brief Return pointer to const DataGroup that owns DataView object.
   */
  const DataGroup * getOwningGroup() const
  {
    return m_owning_group;
  }

  /*!
   * \brief Return type of data for this DataView object.
   */
  TypeID getTypeID() const
  {
    return static_cast<TypeID>(m_node.dtype().id());
  }

  /*!
   * \brief Return pointer to non-const DataBuffer associated with DataView.
   */
  DataBuffer * getBuffer()
  {
    return m_data_buffer;
  }

  /*!
   * \brief Return pointer to const DataBuffer associated with DataView.
   */
  const DataBuffer * getBuffer() const
  {
    return m_data_buffer;
  }
  /*!
   * \brief Return const reference to Conduit schema describing data.
   */
  const Schema& getSchema() const
  {
    return m_schema;
  }

  /*!
   * \brief Return non-const reference to Conduit node holding data.
   */
  Node& getNode()
  {
    return m_node;
  }

  /*!
   * \brief Return const reference to Conduit node holding data.
   */
  const Node& getNode() const
  {
    return m_node;
  }

  //@}

  //@{
  //! @name Accessor methods for retrieving data from a view.  These require the caller to assign the return value
  //! to a variable.
  //! Examples:
  //! int my_int = getData();
  //! int* my_ptr = getData();

  /*!
   * \brief Return a pointer or conduit int_array object to the view's array data.
   */
  Node::Value getArray()
  {
    //TODO add check that view holds array data.  Will be added in later commit.
    //If debug, should trigger assert.  If release, issue warning.
    return getData();
  }

  /*!
   * \brief Returns a copy of the string contained in the view.
   */
  Node::Value getString()
  {
    //TODO add check that view holds array data.  Will be added in later commit.
    //If debug, should trigger assert.  If release, issue warning.
    return getData();
  }

  /*!
   * \brief Returns a void pointer to the view's data.
   * The returned pointer value will take into account any offsets or strides applied to described data.
   */
  void * getVoidPtr()
  {
    // This needs to be changed to check if data is owned, not whether it's described (ie opaque or not)
    if (isOpaque())
    {
      return (void *)m_node.as_uint64();
    }
    // else, view contains data store owned data so retrieve pointer to node's data.
    else
    {
      return m_node.element_ptr(0);
    }
  }

  /*!
   * \brief Returns a copy of the scalar value contained in the view.
   */
  Node::Value getScalar()
  {
    return getData();
  }

  /*!
   * \brief This is an advanced accessor function for retrieving the data in a view that allows a developer more
   * flexibility with casting the data to another type.  It will allow a developer to cast held data to any compatible
   * type allowed by the conduit Node::Value return overloading mechanism.
   */
  Node::Value getData()
  {
    //TODO - add check that data is described (not opaque)
    SLIC_ASSERT_MSG( !isOpaque(), "This view contains data that is not described to the data store (opaque).  You "
      << "must use getVoidPtr() to retrieve a pointer to opaque data.");

    return m_node.value();
  }

  //@}

  //@{
  //!  @name Set methods for setting data in the view.  Set methods are provided for simple scalars, strings, and
  //!  void pointers for undescribed (opaque) data.
  //!  are stored directly in the view's node (or in the case of an opaque pointer, in the class member m_data_ptr.

  // Developer note: Strings, Scalars, and pointers to unowned described (external) or undescribed (opaque) data can
  // all be handled by the view's conduit node.  Having a data buffer is unnecessary, so there is room for refactoring
  // here.  (However, scalars will be refactored in the future to be in a scalar pool).

  /*!
   * \brief Set view to point to undescribed data not owned by the data store (ie, opaque data).
   *
   * \return pointer to this DataView object.
   */
  DataView* setVoidPtr(void * value);

  /*!
   * \brief Set node's data (should be a string).
   */
  DataView* setString(const std::string& value)
  {
    // TODO: Will add check to verify that view holds a string in later commit (need enum set up first).
    // TODO: Check with Cyrus that the set_string function is the right call (should be).
    m_node.set_string(value);
    return this;
  }

  /*!
   * \brief Set  node's data ( should be a scalar ).
   */
  template<typename ScalarType>
  DataView* setScalar(ScalarType value)
  {
    // Check that parameter type provided matches what type is stored in the node.
#if defined(ATK_DEBUG)
    DataTypeId arg_id = SidreTT<ScalarType>::id();
    SLIC_ASSERT_MSG( arg_id == m_node.dtype().id(),
      "Mismatch between setScalar()" << DataType::id_to_name( arg_id ) << ") and type contained in the buffer (" << m_node.dtype().name() << ").");
#endif

    m_node.set(value);
    return this;
  }

  //@}

  /*!
   * \brief Lightweight templated wrapper around getData() that can be used when you are calling getData(), but not
   * assigning the return.
   *
   */
  // TODO - Will a app code ever use this?  We are just using it for internal tests, so maybe we can move this function
  // to a 'test helper' source file and remove it from the core API.
  template<typename DataType>
  DataType getData()
  {
    DataType data = m_node.value();
    return data;
  }

//@{
//!  @name DataView print methods.

  /*!
   * \brief Print JSON description of data view to stdout.
   */
  void print() const;

  /*!
   * \brief Print JSON description of data view to an ostream.
   */
  void print(std::ostream& os) const;

//@}

  /*!
   * \brief Copy data view description to given Conduit node.
   */
  void info(Node& n) const;

private:

//@{
//!  @name Private DataView ctors and dtors 
//!        (callable only by DataGroup and DataView methods).

  /*!
   *  \brief Private ctor that creates a DataView with given name
   *         in given parent group and which has no data associated with it.
   */
  DataView( const std::string& name,
            DataGroup * const owning_group );

  /*!
   *  \brief Private ctor that creates a DataView with given name
   *         in given parent group and which is associated with given
   *         DataBuffer object.
   */
  DataView( const std::string& name,
            DataGroup * const owning_group,
            DataBuffer * const data_buffer );

  /*!
   *  \brief Private ctor that creates an opaque DataView with given name
   *         in given parent group.
   */
  DataView( const std::string& name,
            DataGroup * const owning_group,
            void * opaque_ptr);

  /*!
   * \brief Private copy ctor.
   */
  DataView(const DataView& source);

  /*!
   * \brief Private dtor.
   */
  ~DataView();

//@}


  /*!
   *  \brief Private method returns true if data allocation on view is a
   *         valid operation; else false
   */
  bool allocationIsValid() const; 

//@{
//!  @name Private DataView declaration methods.
//!        (callable only by DataGroup and DataView methods).

  /*!
   * \brief Declare a data view with given type and number of elements, and
   *        
   *
   * IMPORTANT: If view has been previously declared, this operation will
   *            re-declare the view. To have the new declaration take effect,
   *            the apply() method must be called.
   *
   * If given number of elements < 0, or view is opaque, method does nothing.
   *
   * \return pointer to this DataView object.
   */
  DataView * declare( TypeID type, SidreLength num_elems);

  /*!
   * \brief Declare a data view with a Conduit data type object.
   *
   * IMPORTANT: If view has been previously declared, this operation will
   *            re-declare the view. To have the new declaration take effect,
   *            the apply() method must be called.
   *
   * If view is opaque, the method does nothing.
   *
   * \return pointer to this DataView object.
   */
  DataView * declare(const DataType& dtype);

  /*!
   * \brief Declare a data view with a Conduit schema object.
   *
   * IMPORTANT: If view has been previously declared, this operation will
   *            re-declare the view. To have the new declaration take effect,
   *            the apply() method must be called.
   *
   * If view is opaque, the method does nothing.
   *
   * \return pointer to this DataView object.
   */
  DataView * declare(const Schema& schema);

//@}

  /// Name of this DataView object.
  std::string m_name;

  /// DataGroup object that owns this DataView object.
  DataGroup * m_owning_group;

  /// DataBuffer associated with this DataView object.
  DataBuffer * m_data_buffer;

  /// Conduit Schema that describes this DataView.
  Schema m_schema;

  /// Conduit node used to access the data in this DataView.
  Node m_node;

  // TODO - these will get replaced with enum types for the different states of a view (described, applied, allocated, etc)

  /// Is this DataView opaque?  Will get refactored.
  bool m_is_opaque;

  /// Has Schema been applied data? Will get refactored.
  bool m_is_applied;

  /*!
   *  Unimplemented ctors and copy-assignment operators.
   */
#ifdef USE_CXX11
  DataView() = delete;
  DataView( DataView&& ) = delete;

  DataView& operator=( const DataView& ) = delete;
  DataView& operator=( DataView&& ) = delete;
#else
  DataView();
  DataView& operator=( const DataView& );
#endif

};


} /* end namespace sidre */
} /* end namespace asctoolkit */

#endif /* DATAVIEW_HPP_ */
