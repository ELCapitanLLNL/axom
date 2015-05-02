/*!
 ******************************************************************************
 *
 * \file
 *
 * \brief   Header file containing definition of DataBuffer class.
 *
 ******************************************************************************
 */

#ifndef DATABUFFER_HPP_
#define DATABUFFER_HPP_

// Standard C++ headers
#include <map>
#include <set>
#include <vector>

// Other library headers
#include "conduit/conduit.h"

// SiDRe project headers
#include "Types.hpp"


// using directives to make Conduit usage easier and less visible
using conduit::DataType;
using conduit::Node;
using conduit::Schema;


namespace sidre
{

class DataStore;
class DataView;

/*!
 * \class DataBuffer
 *
 * \brief DataBuffer holds a data object, which it owns (and allocates!)
 *
 * A DataBuffer instance has the following properties:
 *
 *    - DataBuffer objects can only be created via the DataStore interface,
 *      not directly. 
 *    - A DataBuffer object has a unique identifier within a DataStore,
 *      which is assigned by the DataStore when the buffer is created.
 *    - The data object owned by a DataBuffer is unique to that DataDuffer
 *      object; i.e.,  DataBuffers do not share data.
 *    - A DataBuffer object maintains a collection of DataViews that
 *      refer to its data.
 *
 */
class DataBuffer
{
public:

    //
    // Friend declarations to constrain usage via controlled access to
    // private members.
    //
    friend class DataStore;
    friend class DataGroup;
    friend class DataView;


    /*!
     * \brief Return the unique id of this buffer object.
     */
    IDType GetUID() const
    {
        return m_uid;
    }

    /*!
     * \brief Return number of views attached to this buffer.
     */
    size_t CountViews() const
    {
      return m_views.size();
    }
   
 
//@{
//!  @name Data declaration and allocation methods
 
    /*!
     * \brief Declare a data object as a Conduit schema.
     *
     * \return pointer to this DataBuffer object.
     */
    DataBuffer* Declare(const Schema& schema)
    {
        m_schema.set(schema);
        return this;
    }
    
    /*!
     * \brief Declare a data object as a pre-defined Conduit data type.
     *
     * \return pointer to this DataBuffer object.
     */
    DataBuffer* Declare(const DataType& dtype)
    {
        m_schema.set(dtype);
        return this;
    }

    /*!
     * \brief Allocate data previously declared using a Declare() method.
     *
     * \return pointer to this DataBuffer object.
     */
    DataBuffer* Allocate();
  
    /*!
     * \brief Declare and allocate data described as a Conduit schema.
     *
     *        Equivalent to calling Declare(schema), then Allocate().
     *
     * \return pointer to this DataBuffer object.
     */
    DataBuffer* Allocate(const Schema &schema);

    /*!
     * \brief Declare and allocate data described as a pre-defined 
     *        Conduit data type.
     *
     *        Equivalent to calling Declare(dtype), then Allocate().
     *
     * \return pointer to this DataBuffer object.
     */
    DataBuffer* Allocate(const DataType& dtype);

//@}


//@{
//!  @name Accessor methods
 
    /*!
     * \brief Return void-pointer to data held by DataBuffer.
     */
    void* GetData()
    { 
       return m_data;
    }

    /*!
     * \brief Return non-const reference to Conduit node holding data.
     */
    Node& GetNode()
    {
       return m_node; 
    }

    /*!
     * \brief Return const reference to Conduit node holding data.
     */
    const Node& GetNode() const
    { 
       return m_node; 
    }

    /*!
     * \brief Return const reference to Conduit schema describing data.
     */
    const Schema& getDescriptor() const
    { 
       return m_schema; 
    }

    /*!
     * \brief Return pointer to view attached to this buffer identified
     *        by the given index.
     */
    DataView* GetView(IDType idx)
    { 
       return m_views[idx]; 
    }

//@}


    /*!
     * \brief Copy data buffer description to given Conduit node.
     */
    void Info(Node& n) const;

    /*!
     * \brief Print JSON description of data buffer to stdout.
     */
    void Print() const;


private:
    //
    // Default ctor is not implemented.
    //
    DataBuffer();

    /*!
     *  \brief Private ctor that assigns unique id.
     */
    DataBuffer( const IDType uid );

    /*!
     * \brief Private copy ctor.
     */
    DataBuffer(const DataBuffer& source );

    /*!
     * \brief Private dtor.
     */
    ~DataBuffer();

    /*!
     * \brief Private methods to attach/detach data view to buffer.
     */
    void attachView( DataView* dataView );
    ///
    void detachView( DataView* dataView );


    /// universal identifier - unique within a dataStore
    IDType m_uid;

    /// container of views that attached with this buffer
    std::vector<DataView *> m_views;

    /// pointer to the data owned by data buffer
    void* m_data;
  
    ///
    /// vector used to allocate data
    /// 
    /// IMPORTANT: This is temorary until we implement an appropriate 
    ///            allocator interface.
    ///
    std::vector<char> m_memblob;

    /// Conduit Node that holds buffer data 
    Node   m_node;

    /// Conduit Schema that describes buffer data
    Schema m_schema;

};


} /* namespace sidre */
#endif /* DATABUFFER_HPP_ */
