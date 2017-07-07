/*
 * Copyright (c) 2017, Lawrence Livermore National Security, LLC.
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
 * \file AttrValues.cpp
 *
 * \brief   Implementation file for AttrValues class.
 *
 ******************************************************************************
 */

// Associated header file
#include "AttrValues.hpp"

// Other axom headers
#include "axom/Types.hpp"
#include "slic/slic.hpp"

// Sidre component headers

namespace axom
{
namespace sidre
{

/*
 *************************************************************************
 *
 * Return true if the attribute has been explicitly set; else false.
 *
 *************************************************************************
 */
bool AttrValues::hasValue( IndexType iattr ) const
{
  if (! indexIsValid(iattr))
  {
    SLIC_CHECK_MSG(indexIsValid(iattr),
		   "hasValue: called with invalid index");
    return false;
  }

  if (m_values == AXOM_NULLPTR)
  {
    // No attributes have been set in this View.
    return false;
  }

  if ((size_t) iattr >= m_values->size())
  {
    // This attribute has not been set for this View.
    return false;
  }

  Node & value = (*m_values)[iattr];
  
  if (isEmpty(value))
  {
    return false;
  }

  return true;
}

/*
 *************************************************************************
 *
 * Set Attribute to its default value. If the Attribute has not been
 * set yet, then it is already the default value.  Otherwise, reset
 * the Node to empty.  Future calls to hasAttributeValue will return
 * false for the attribute.
 *
 *************************************************************************
 */
bool AttrValues::setToDefault( const Attribute * attr )
{
  if (attr == AXOM_NULLPTR)
  {
    SLIC_CHECK_MSG(attr != AXOM_NULLPTR,
		   "setDefault: called without an Attribute");
    return false;
  }

  if (m_values == AXOM_NULLPTR)
  {
    // No attributes have been set in this View, already default.
    return true;
  }

  IndexType iattr = attr->getIndex();

  if ((size_t) iattr >= m_values->size())
  {
    // This attribute has not been set for this View, already default.
    return true;
  }

  Node & value = (*m_values)[iattr];
  value.reset();

  return true;
}

/*
 *************************************************************************
 *
 * PRIVATE Create a Node for the Attribute.
 *
 * Create vector of Nodes and push empty nodes up to attr's index.
 *
 *************************************************************************
 */
bool AttrValues::createNode(const Attribute * attr)
{
  if (attr == AXOM_NULLPTR)
  {
    SLIC_CHECK_MSG(attr != AXOM_NULLPTR,
		   "createNode: called without an Attribute");
    return false;
  }

  if (m_values == AXOM_NULLPTR)
  {
    m_values = new(std::nothrow) Values( );
  }

  IndexType iattr = attr->getIndex();

  if ((size_t) iattr >= m_values->size())
  {
    // Create all attributes up to iattr, push back empty Nodes
    m_values->reserve(iattr + 1);
    for(int n=m_values->size(); n < iattr + 1; ++n)
    {
      m_values->push_back(Node());
    }
  }
   
  return true;
}
bool AttrValues::createNode(IndexType iattr)
{
  if (m_values == AXOM_NULLPTR)
  {
    m_values = new(std::nothrow) Values( );
  }

  if ((size_t) iattr >= m_values->size())
  {
    // Create all attributes up to iattr, push back empty Nodes
    m_values->reserve(iattr + 1);
    for(int n=m_values->size(); n < iattr + 1; ++n)
    {
      m_values->push_back(Node());
    }
  }
   
  return true;
}

/*
 *************************************************************************
 *
 * Return a scalar attribute value.
 *
 * The caller must ensure that attr is not NULL.
 *
 *************************************************************************
 */
Node::ConstValue AttrValues::getScalar( const Attribute * attr ) const
{
  if (attr == AXOM_NULLPTR)
  {
    SLIC_CHECK_MSG(attr != AXOM_NULLPTR,
		   "getScalar: called without an Attribute");
    return getEmptyNodeRef().value();
  }

  const Node & node = getValueNodeRef(attr);
  return node.value();
}

/*
 *************************************************************************
 *
 * Return a string attribute value.
 *
 * The caller must ensure that attr is not NULL.
 *
 *************************************************************************
 */
const char * AttrValues::getString( const Attribute * attr ) const
{
  if (attr->getTypeID() != CHAR8_STR_ID)
  {
    SLIC_CHECK_MSG(attr->getTypeID() == CHAR8_STR_ID,
		   "getString: Called on attribute '"
		   << attr->getName() 
		   << "' which is type "
		   << DataType::id_to_name(attr->getTypeID())
                   << ".");
    return AXOM_NULLPTR;
  }

  const Node & node = getValueNodeRef(attr);
  return node.as_char8_str();
}

/*
 *************************************************************************
 *
 * Return Reference to Node.
 *
 *************************************************************************
 */
const Node & AttrValues::getValueNodeRef( const Attribute * attr ) const
{
  if (attr == AXOM_NULLPTR)
  {
    SLIC_CHECK_MSG(attr != AXOM_NULLPTR,
		   "getValueNodeRef: called without an Attribute");
    return getEmptyNodeRef();
  }

  if (m_values == AXOM_NULLPTR)
  {
    // No attributes have been set in this View;
    return attr->getDefaultNodeRef();
  }

  IndexType iattr = attr->getIndex();

  if ((size_t) iattr >= m_values->size())
  {
    // This attribute has not been set for this View
    return attr->getDefaultNodeRef();
  }

  Node & value = (*m_values)[iattr];
  
  if (isEmpty(value))
  {
    return attr->getDefaultNodeRef();
  }

  return value;
}

/*
 *************************************************************************
 * Return first valid Attribute index for a set Attribute.
 * (i.e., smallest index over all Attributes).
 *
 * sidre::InvalidIndex is returned if DataStore has no Attributes.
 *************************************************************************
 */
IndexType AttrValues::getFirstValidAttrValueIndex() const
{
  if (m_values == AXOM_NULLPTR)
  {
    // No attributes have been set in this View.
    return InvalidIndex;
  }

  for(size_t iattr = 0; iattr < m_values->size(); ++iattr)
  {
    // Find first, non-empty attribute.
    Node & value = (*m_values)[iattr];
    if (! isEmpty(value))
    {
      return iattr;
    }
  }

  return InvalidIndex;
}

/*
 *************************************************************************
 * Return next valid Attribute index for a set Attribute after given
 * index (i.e., smallest index over all Attribute indices larger than
 * given one).
 *
 * sidre::InvalidIndex is returned if there is no valid index greater
 * than given one.
 * getNextAttrValueIndex(InvalidIndex) returns InvalidIndex.
 *************************************************************************
 */
IndexType AttrValues::getNextValidAttrValueIndex(IndexType idx) const
{
  if (idx == InvalidIndex)
  {
    return InvalidIndex;
  }

  idx++;
  while ( static_cast<unsigned>(idx) < m_values->size() &&
          isEmpty((*m_values)[idx]))
  {
    idx++;
  }
  return ( (static_cast<unsigned>(idx) < m_values->size()) ? idx : InvalidIndex );
}

/*
 *************************************************************************
 *
 * PRIVATE ctor for AttrValues
 *
 *************************************************************************
 */
AttrValues::AttrValues() :
  m_values(AXOM_NULLPTR)
{}

/*
 *************************************************************************
 *
 * PRIVATE dtor.
 *
 *************************************************************************
 */
AttrValues::~AttrValues()
{
}

} /* end namespace sidre */
} /* end namespace axom */
