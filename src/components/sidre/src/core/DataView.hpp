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
 * \brief A DataView object describes a "view" into data, which may be
 *        owned by the view object (via an attached DataBuffer) or
 *        owned externally.
 *
 * The DataView class has the following properties:
 *
 *    - DataView objects can only be created via the DataGroup interface,
 *      not constructed directly. A view object is owned by the DataGroup
 *      object that creates it.
 *    - A DataView object has a unique name (string) within the DataGroup
 *      that owns it.
 *    - A DataView holds a pointer to the DataGroup that created it and which
 *      owns it.
 *    - A DataView object can describe and provide access to data in one of
 *      four ways:
 *        # A view can decribe (a subset of) data owned by an existing
 *          DataBuffer. In this case, the data can be (re)allocated or
 *          deallocated by the view if and only if it is the only view
 *          attached to the buffer.
 *        # A view can describe and allocate data using semantics similar
 *          to DataBuffer data description and allocation. In this case, no
 *          other view is allowed to (re)allocate or deallocate the data held
 *          by the associated data buffer.
 *        # A view can describe data associated with a pointer to an
 *          "external" data object. In this case, the view cannot (re)allocate
 *          or deallocate the data. However, all other view operations are
 *          essentially the same as the previous two cases.
 *        # It can hold a pointer to an undescribed (i.e., "opaque") data
 *          object. In this case, the view knows nothing about the type or
 *          structure of the data; it is essentially just a handle to the data.
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
//!  @name DataView query and accessor methods methods

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
   * \brief Return true if view has a an associated DataBuffer object
   *        (e.g., view is not opaque, it has been created with a buffer,
   *         it has been allocated, etc.); false otherwise.
   */
  bool  hasBuffer() const
  {
    return m_data_buffer != ATK_NULLPTR;
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
   * \brief Return true if view holds external data; false otherwise.
   */
  bool isExternal() const
  {
    return m_state == EXTERNAL;
  }

  /*!
   * \brief Return true if view holds data that has been allocated.
   */
  // TODO - Would like to make this a const function.  Need to have conduit element_ptr() be const to do this.
  bool isAllocated();

  /*!
   * \brief Return true if data description (schema) has been applied to data
   *        in buffer associated with view; false otherwise.
   */
  bool isApplied() const
  {
    return m_is_applied;
  }

  /*!
   * \brief Return true if data description exists.  It may/may not have been
   * applied to the data yet.  ( Check isApplied() for that. )
   */
  bool isDescribed() const
  {
    return !m_schema.dtype().is_empty();
  }

  /*!
   * \brief Return true if view is empty.
   */
  bool isEmpty() const
  {
    return m_state == EMPTY;
  }

  /*!
   * \brief Convenience function that returns true if view is opaque
   *        (i.e., has access to data but has no knowledge of the data
   *        type or structure); false otherwise.
   */
  bool  isOpaque() const
  {
    return m_state == EXTERNAL && !isApplied();
  }

  /*!
   * \brief Return true if view contains a scalar value.
   */
  bool isScalar() const
  {
    return m_state == SCALAR;
  }

  /*!
   * \brief Return true if view contains a string value.
   */
  bool isString() const
  {
    return m_state == STRING;
  }

  /*!
   * \brief Return type of data for this DataView object.
   */
  TypeID getTypeID() const
  {
    return static_cast<TypeID>(m_schema.dtype().id());
  }

  /*!
   * \brief Return total number of bytes associated with this DataView object.
   *
   * IMPORTANT: This is the total bytes described by the view; they may not
   *            yet be allocated.
   */
  SidreLength getTotalBytes() const
  {
    return m_schema.total_bytes();
  }

  /*!
   * \brief Return total number of elements described by this DataView object.
   *
   * IMPORTANT: This is the number of elements described by the view;
   *            they may not yet be allocated.
   */
  SidreLength getNumElements() const
  {
    return m_schema.dtype().number_of_elements();
  }

  /*!
   * \brief Return number of dimensions in data view.
   */
  int getNumDimensions() const
  {
    return m_shape.size();
  }

  /*!
   * \brief Return number of dimensions in data view and fill in shape
   *        information of this data view object.
   *
   *  ndims - maximum number of dimensions to return.
   *  shape - user supplied buffer assumed to be ndims long.
   *
   *  Return the number of dimensions of the view.
   *  Return -1 if shape is too short to hold all dimensions.
   */
  int getShape(int ndims, SidreLength * shape) const;

  /*!
   * \brief Return const reference to schema describing data.
   */
  const Schema& getSchema() const
  {
    return m_node.schema();
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
//!  @name DataView allocation methods

  /*!
   * \brief Allocate data for a view, previously described.
   *
   * NOTE: Allocation from a view is allowed only if it is the only
   *       view associated with its buffer (when it has one), or the view
   *       is not external, not a string view, or not a scalar view.
   *       If none of these condition is true, this method does nothing.
   *
   * \return pointer to this DataView object.
   */
  DataView * allocate();

  /*!
   * \brief Allocate data for view given type and number of elements.
   *
   * NOTE: The allocate() method describes conditions where view
   *       allocation is allowed. If none of those is true, or given number
   *       of elements is < 0, this method does nothing.
   *
   * \return pointer to this DataView object.
   */
  DataView * allocate( TypeID type, SidreLength num_elems);

  /*!
   * \brief Allocate data for view described by a Conduit data type object.
   *
   * NOTE: The allocate() method describes conditions where view
   *       allocation is allowed. If none of those is true,
   *       this method does nothing.
   *
   * \return pointer to this DataView object.
   */
  DataView * allocate(const DataType& dtype);

  /*!
   * \brief  Reallocate data for view to given number of elements (type
   *         stays the same).
   *
   * NOTE: Reallocation from a view is only allowed under that same conditions
   *       for the allocate() method. If none of those is true,
   *       or given number of elements is < 0, this method does nothing.
   *
   * \return pointer to this DataView object.
   */
//
// RDH -- Should calling reallocate with 0 elems deallocate the data??
//
  DataView * reallocate(SidreLength num_elems);

  /*!
   * \brief  Reallocate data for view as specified by Conduit data type object.
   *
   * NOTE: Reallocation from a view is only allowed under that same conditions
   *       described by the allocate() method. If none of those is true,
   *       or data type is undefined, this method does nothing.
   *
   * NOTE: The given data type object must match the view type, if it is
   *       defined. If not, the method does nothing.
   *
   * \return pointer to this DataView object.
   */
  DataView * reallocate(const DataType& dtype);

  /*!
   * \brief  Deallocate data for view.
   *
   * NOTE: Deallocation from a view is only allowed under that same conditions
   *       described by the allocate() method. If none of those is true,
   *       or a DataBuffer is not attached, this method does nothing.
   *
   * \return pointer to this DataView object.
   */
  DataView * deallocate();

//@}


  /*!
   * \brief Attach DataBuffer object to data view.
   *
   * Note that, in general, the view cannot be used to access data in
   * buffer until one of the apply() methods is called. However, if
   * the view has a valid data description with a total number of bytes
   * that is <= number of bytes held in the buffer, then apply() will
   * be called internally.
   *
   * If data view already has a buffer, or it is an external view,
   * a scalar view, or a string view, this method does nothing.
   *
   * \return pointer to this DataView object.
   */
  DataView * attachBuffer( DataBuffer * buff );


//@{
//!  @name Methods to apply DataView description to data.

  /*!
   * \brief Apply view description to data.
   *
   * If view holds a scalar or a string, the method does nothing.
   *
   * \return pointer to this DataView object.
   */
  DataView * apply();

  /*!
   * \brief Apply data description defined by number of elements, and
   *        optionally offset and stride to data view (type remains the same).
   *
   * NOTE: The units for offset and stride are in number of elements, which
   *       is different than the Conduit DataType usage below where offset
   *       and stride are in number of bytes.
     //
     // RDH -- will this be changed for consistency?
     //
   *
   * IMPORTANT: If view has been previously described (or applied), this
   *            operation will apply the new data description to the view.
   *
   * If view holds a scalar or a string, is external and does not have a
   * sufficient data description to get type information, or given number
   * of elements < 0, or offset < 0, the method does nothing.
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
   *       is different than the Conduit DataType usage below where offset
   *       and stride are in number of bytes.
   *
   * IMPORTANT: If view has been previously described (or applied), this
   *            operation will apply the new data description to the view.
   *
   * If view holds a scalar or a string, or given number of elements < 0,
   * or offset < 0, the method does nothing.
   *
   * \return pointer to this DataView object.
   */
  DataView * apply( TypeID type, SidreLength num_elems,
                    SidreLength offset = 0,
                    SidreLength stride = 1);

  /*!
   * \brief Apply data description defined by type and shape information
   *        to data view.
   *
   * NOTE: The units for the shape are in number of elements.
   *
   * IMPORTANT: If view has been previously described (or applied), this
   *            operation will apply the new data description to the view.
   *
   * If view holds a scalar or a string, or given number of dimensions < 0,
   * or pointer to shape is null, the method does nothing.
   *
   * \return pointer to this DataView object.
   */
  DataView * apply( TypeID type, int ndims, SidreLength * shape );

  /*!
   * \brief Apply data description of given Conduit data type to data view.
   *
   * If view holds a scalar or a string, the method does nothing.
   *
   * \return pointer to this DataView object.
   */
  DataView * apply(const DataType& dtype);

//@}


//@{
//!  @name Methods to set data in the view (scalar, string, or external data).

  /*!
   * \brief Set the view to hold the given scalar.
   *
   * \return pointer to this DataView object.
   */
  template<typename ScalarType>
  DataView * setScalar(ScalarType value)
  {
//
// RDH -- This will change when scalar pool is added. Also, the following
//        check will not be needed because a scalar view will not have
//        allocated data.
//
    // Check that parameter type provided matches what type is stored in the node.
#if defined(ATK_DEBUG)
    if (m_state != EMPTY)
    {
      DataTypeId arg_id = detail::SidreTT<ScalarType>::id;
      SLIC_CHECK_MSG( arg_id == m_node.dtype().id(),
                      "Mismatch between setScalar()" <<
                      DataType::id_to_name(
                        arg_id ) << ") and type contained in the buffer (" << m_node.dtype().name() <<
                      ").");
    }
#endif

    m_node.set(value);
    m_schema.set(m_node.schema());
    m_is_applied = true;
    m_state = SCALAR;
    describeShape();
    return this;
  }

//
// RDH -- Add an overload of the following that takes a const char *.
//
/*!
 * \brief Set the view to hold the given string.
 *
 * \return pointer to this DataView object.
 */
  DataView * setString(const std::string& value)
  {
    m_node.set_string(value);
    m_schema.set(m_node.schema());
    m_state = STRING;
    m_is_applied = true;
    describeShape();
    return this;
  };

  /*!
   * \brief Set view to hold external data.
   *
   * Data is undescribed (i.e., view is opaque) until an apply methods
   * is called on the view.
   *
   * If external_ptr is NULL, the view will be EMPTY.
   * Any existing description is unchanged.
   *
   * \return pointer to this DataView object.
   */
  DataView * setExternalDataPtr(void * external_ptr);

//@}


//@{
//! @name Methods to retrieve data in a view.

  /*!
   * \brief Return a pointer or conduit array object to the view's array data.
   *
   * Return value depends on variable type caller assigns it to. For example,
   * if view holds an integer array, the following usage is possible:
   *
   * \verbatim
   *    int* a = view->getArray();      // Get array as int pointer
   *    int_array a = view->getArray(); // Get array as Conduit array struct.
   * \endverbatim
   */
  Node::Value getArray()
  {
    //TODO add check that view holds array data.  Will be added in later commit.
    //If debug, should trigger assert.  If release, issue warning.
    return getData();
  }

  /*!
   * \brief Returns a copy of the string contained in the view.
     //
     // RDH -- Should we also provide an overload that returns a const char *?
     //        It seems excessive to create copies of strings for most usage.
     //
   */
//
// RDH -- What happens if the view does not hold a scalar?
//
  Node::Value getString()
  {
    //TODO add check that view holds array data.  Will be added in later commit.
    //If debug, should trigger assert.  If release, issue warning.
    return getData();
  }

  /*!
   * \brief Returns a copy of the scalar value contained in the view.
   */
//
// RDH -- What happens if the view does not hold a scalar?
//
  Node::Value getScalar()
  {
    return getData();
  }

  /*!
   * \brief Return data held by view and cast it to any compatible type
   *  allowed by Conduit (return type depends on type caller assigns it to).
   *
   *  If view does not contain allocated data, an empty Node::Value will be
   *  returned.
   */
  Node::Value getData()
  {
    if ( !isAllocated() || !isDescribed())
    {
      SLIC_CHECK_MSG( isAllocated(),
                      "No view data present, memory has not been allocated.");
      SLIC_CHECK_MSG( isApplied(),
                      "View data description not present.");
      return Node().value();
    }
    return m_node.value();
  }

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

  /*!
   * \brief Returns a void pointer to data in the view (if described, it will take into account any
   * offset, stride, schema, etc. applied).
   */
  // TODO - Would like this to be a const function, but it calls a conduit function which is not const.
  void * getVoidPtr() const;

//@}


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
//!  @name Private DataView ctor and dtor
//!        (callable only by DataGroup and DataView methods).

  /*!
   *  \brief Private ctor that creates a DataView with given name
   *         in given parent group and which has no data associated with it.
   */
  DataView( const std::string& name,
            DataGroup * const owning_group );

  /*!
   * \brief Private copy ctor.
   */
  DataView(const DataView& source);

  /*!
   * \brief Private dtor.
   */
  ~DataView();

//@}


//@{
//!  @name Private DataView declaration methods.
//!        (callable only by DataGroup and DataView methods).

  /*!
   * \brief Describe a data view with given type and number of elements.
   *
   *
   * IMPORTANT: If view has been previously described, this operation will
   *            re-describe the view. To have the new description take effect,
   *            the apply() method must be called.
   *
   * If given number of elements < 0, or view is opaque, method does nothing.
   *
   */
  void describe( TypeID type, SidreLength num_elems);

  /*!
   * \brief Describe a data view with given type, number of dimensions, and
   *        number of elements per dimension.
   *
   *
   * IMPORTANT: If view has been previously described, this operation will
   *            re-describe the view. To have the new description take effect,
   *            the apply() method must be called.
   *
   * If given number of dimensions or total number of elements < 0,
   * or view is opaque, method does nothing.
   *
   * \return pointer to this DataView object.
   */
  void describe(TypeID type, int ndims, SidreLength * shape);

  /*!
   * \brief Declare a data view with a Conduit data type object.
   *
   * IMPORTANT: If view has been previously described, this operation will
   *            re-describe the view. To have the new description take effect,
   *            the apply() method must be called.
   *
   * If view is opaque, the method does nothing.
   *
   * \return pointer to this DataView object.
   */
  void describe(const DataType& dtype);

  /*!
   * \brief Set the shape to be a one dimension with the described number of elements.
   */
  void describeShape();

  /*!
   * \brief Set the shape to be a ndims dimensions with shape.
   */
  void describeShape(int ndims, SidreLength * shape);

//@}


//@{
//!  @name Private method that determine whether some view operations are valid.

  /*!
   *  \brief Private method returns true if data allocation on view is a
   *         valid operation; else false
   */
  bool isAllocateValid() const;

  /*!
   *  \brief Private method returns true if attaching buffer to view is a
   *         valid operation; else false
   */
  bool isAttachBufferValid() const;

  /*!
   *  \brief Private method returns true if apply is a valid operation on
   *         view; else false
   */
  bool isApplyValid() const;

  /*!
   *  \brief Private method to remove any applied description;
   *         but preserves user provided description.
   *
   */
  void unapply()
  {
    m_node.reset();
    m_is_applied = false;
  }

//@}


  ///
  /// Enum with constants that identify the state of a view.
  ///
  /// Note that these states are not mutually-exclusive. These constants
  /// combined with the boolean m_is_applied uniquely identify the view
  /// state, or how it was created and defined.
  ///
  enum State
  {
    EMPTY,           // View created with name only :
                     //    has no data or data description
    BUFFER,          // View has a buffer attached explicitly. :
                     //    applied may be true or false
    EXTERNAL,        // View holds pointer to external data (no buffer) :
                     //    applied may be true or false
    SCALAR,          // View holds scalar data (via setScalar()):
                     //    applied is true
    STRING           // View holds string data (view setString()):
                     //    applied is true
  };

  /*!
   *  \brief Private method returns string name of given view state enum value.
   */
  char const * getStateStringName(State state) const;

  /// Name of this DataView object.
  std::string m_name;

  /// DataGroup object that owns this DataView object.
  DataGroup * m_owning_group;

  /// DataBuffer associated with this DataView object.
  DataBuffer * m_data_buffer;

  /// Data description (schema) that describes the view's data.
  Schema m_schema;

  /// Conduit node used to access the data in this DataView.
  Node m_node;

  /// Shape information
  std::vector<SidreLength> m_shape;

  /// Pointer to external memory
  void * m_external_ptr;

  /// State of view.
  State m_state;

  /// Has data description been applied to the view's data?
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
