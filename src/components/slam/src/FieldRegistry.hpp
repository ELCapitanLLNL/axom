/*
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Copyright (c) 2017, Lawrence Livermore National Security, LLC.
 *
 * Produced at the Lawrence Livermore National Laboratory
 *
 * LLNL-CODE-741217
 *
 * All rights reserved.
 *
 * This file is part of Axom.
 *
 * For details about use and distribution, please read axom/LICENSE.
 *
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

#ifndef SLAM_FIELD_REGISTRY_H_
#define SLAM_FIELD_REGISTRY_H_


#include <sstream>

#include "axom/Macros.hpp"
#include "slic/slic.hpp"

#include "slam/Utilities.hpp"
#include "slam/Set.hpp"
#include "slam/Map.hpp"

namespace axom {
namespace slam {



/**
 * \brief Very simple container for fields of a given type DataType with minimal error checking.
 * \note
 *         We are using concrete instances for int and double in the code below.
 *         This should eventually be replaced with the sidre datastore.
 */
  template<typename TheDataType>
  class FieldRegistry
  {
  public:
    typedef TheDataType                   DataType;
    typedef std::string                   KeyType;
    typedef axom::slam::Map<DataType>     MapType;
    typedef typename MapType::OrderedMap  BufferType;

    typedef std::map<KeyType, MapType>    DataVecMap;
    typedef std::map<KeyType, BufferType> DataBufferMap;
    typedef std::map<KeyType, DataType>   DataAttrMap;

  public:


    bool      hasField(const KeyType & key) const 
    { 
        return m_maps.find(key) != m_maps.end(); 
    }

    MapType&  addField(KeyType key, Set const* theSet) 
    { 
        return m_maps[key] = MapType(theSet); 
    }

    MapType&  addNamelessField(Set const* theSet)
    {
      static int cnt = 0;
      std::stringstream key;

      key << "__field_" << cnt++;
      return m_maps[key.str()] = MapType(theSet);
    }

    MapType& getField(KeyType key)
    {
      verifyFieldsKey(key);
      return m_maps[key];
    }

    const MapType& getField(KeyType key) const
    {
      verifyFieldsKey(key);
      return m_maps[key];
    }


    bool        hasBuffer(const KeyType & key) const 
    { 
        return m_buff.find(key) != m_buff.end(); 
    }

    BufferType& addBuffer(KeyType key, int size = 0)     
    { 
       return m_buff[key] = BufferType(size); 
    }

    BufferType& addNamelessBuffer(int size = 0)
    {
      static int cnt = 0;
      std::stringstream key;

      key << "__buffer_" << cnt++;
      return m_buff[key.str()] = BufferType(size);
    }

    BufferType& getBuffer(KeyType key)
    {
      verifyBufferKey(key);
      return m_buff[key];
    }
    const BufferType & getBuffer(KeyType key) const
    {
      verifyBufferKey(key);
      return m_buff[key];
    }


    bool      hasScalar(const KeyType & key) const 
    { 
        return m_scal.find(key) != m_scal.end(); 
    }

    DataType& addScalar(KeyType key, DataType val)     
    { 
        return m_scal[key] = val; 
    }

    DataType& getScalar(KeyType key)
    {
      verifyScalarKey(key);
      return m_scal[key];
    }

    const DataType& getScalar(KeyType key) const
    {
      verifyScalarKey(key);
      return m_scal[key];
    }

  private:

    std::string dataTypeString() const {
      return axom::slam::util::TypeToString<DataType>::to_string();
    }

    inline void verifyFieldsKey(const KeyType& AXOM_DEBUG_PARAM(key)) const 
    {
      SLIC_ASSERT_MSG( hasField(key),
          "Didn't find " << dataTypeString() << " field named " << key );
    }

    inline void verifyBufferKey(const KeyType & AXOM_DEBUG_PARAM(key)) const 
    {
      SLIC_ASSERT_MSG( hasBuffer(key),
          "Didn't find " << dataTypeString() << " buffer named " << key );
    }

    inline void verifyScalarKey(const KeyType & AXOM_DEBUG_PARAM(key)) const 
    {
      SLIC_ASSERT_MSG( hasScalar(key),
          "Didn't find " << dataTypeString() << " scalar named " << key );
    }

  private:

    DataVecMap m_maps;
    DataBufferMap m_buff;
    DataAttrMap m_scal;
  };

} // end namespace slam
} // end namespace axom

#endif // SLAM_FIELD_REGISTRY_H_
