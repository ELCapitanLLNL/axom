/*
 * Copyright (c) 2015, Lawrence Livermore National Security, LLC.
 * Produced at the Lawrence Livermore National Laboratory.
 *
 * All rights reserved.
 *
 * This source code cannot be distributed without permission and further
 * review from Lawrence Livermore National Laboratory.
 */

#ifndef UNIFORMMESH_HXX_
#define UNIFORMMESH_HXX_

#include "mint/StructuredMesh.hpp"
#include "slic/slic.hpp"

namespace axom {
namespace mint {

class UniformMesh:public StructuredMesh
{
public:

  /*!
   * \brief Constructs a uniform mesh defined by the origin, spacing and extent.
   * \param [in] dimension the dimension of this mesh instance.
   * \param [in] origin the origin coordinates of the mesh
   * \param [in] h the spacing in each dimension.
   * \param [in] ext the extent of this mesh instance.
   */
  UniformMesh( int dimension,
               const double origin[3],
               const double h[3],
               const int ext[6] );

  /*!
   * \brief Constructs a uniform mesh defined by the extent of the mesh in each
   *  direction and an axis aligned bounding box.
   * \param [in] dimension the dimension of this mesh instance.
   * \param [in] ext the extent of this mesh instance.
   * \param [in] lower_bound a corner of the bounding box.
   * \param [in] upper_bound the corner opposite lower_bound of the bounding box.
   */
  UniformMesh( int dimension,
               const int ext[6],
               const double lower_bound[3],
               const double upper_bound[3] );

  /*!
   * \brief Constructs a uniform mesh defined by the origin, spacing and extent.
   * \param [in] dimension the dimension of this mesh instance.
   * \param [in] origin the origin coordinates of the mesh
   * \param [in] h the spacing in each dimension.
   * \param [in] ext the extent of this mesh instance.
   * \param [in] blockId the block ID of this mesh.
   * \param [in] partitionId the partition ID of this mesh.
   */
  UniformMesh( int dimension,
               const double origin[3],
               const double h[3],
               const int ext[6],
               int blockId,
               int partitionId );

  /*!
   * \brief Constructs a uniform mesh defined by the extent of the mesh in each
   *  direction and an axis aligned bounding box.
   * \param [in] dimension the dimension of this mesh instance.
   * \param [in] ext the extent of this mesh instance.
   * \param [in] lower_bound a corner of the bounding box.
   * \param [in] upper_bound the corner opposite lower_bound of the bounding box.
   * \param [in] blockId the block ID of this mesh.
   * \param [in] partitionId the partition ID of this mesh.
   */
  UniformMesh( int dimension,
               const int ext[6],
               const double lower_bound[3],
               const double upper_bound[3],
               int blockId,
               int partitionId );

  /*!
   * \brief Destructor.
   */
  virtual ~UniformMesh();

  /*!
   * \brief Returns the origin of the Uniform Mesh
   * \param [out] origin user-supplied buffer to store the mesh origin.
   */
  void getOrigin( double origin[3] ) const;

  /*!
   * \brief Returns the spacing of the Uniform Mesh.
   * \param [out] h user-supplied buffer to store the spacing of the mesh.
   */
  void getSpacing( double h[3] ) const;

  /*!
   * \brief Returns the coordinates of the given node.
   * \param [in] nodeIdx the index of the node in query.
   * \param [out] coordinates pointer to buffer to populate with coordinates.
   * \pre coordinates != AXOM_NULLPTR.
   * \pre nodeIdx >= 0 && nodeIdx < this->getNumberOfNodes().
   */
  virtual void getNode( int nodeIdx, double* coordinates ) const;

  /*!
   * \brief Returns the coordinates of the node at (i,j)
   * \param [in] i logical index of the node along the first dimension.
   * \param [in] j logical index of the node along the second dimension.
   * \param [out] coordinates pointer to buffer to populate with coordinates.
   * \pre this->getDimension() == 2
   */
  virtual void getNode( int i, int j, double* coordinates ) const;

  /*!
   * \brief Returns the coordinates of the node at (i,j)
   * \param [in] i logical index of the node along the first dimension.
   * \param [in] j logical index of the node along the second dimension.
   * \param [in] k logical index of the node along the third dimension.
   * \param [out] coordinates pointer to buffer to populate with coordinates.
   * \pre this->getDimension() == 3
   */
  virtual void getNode( int i, int j, int k, double* coordinates ) const;

  /*!
   * \brief Returns the coordinate of the given node.
   * \param [in] nodeIdx index of the node in query.
   * \param [in] idim requested coordinate dimension.
   * \return x the coordinate value of the node.
   * \pre nodeIdx >= 0 && nodeIdx < this->getNumberOfNodes()
   * \pre idim >= 0 && idim < m_ndims.
   */
  virtual double getNodeCoordinate( int nodeIdx, int idim  ) const;

  /*!
   * \brief Returns the coordinate value of the node at (i,j)
   * \param [in] i logical index of the node along the first dimension.
   * \param [in] j logical index of the node along the second dimension.
   * \param [in] idim requested coordinate dimension.
   * \return x the coordinate value of the node.
   * \pre this->getDimension()==2.
   * \pre idim >= 0 && idim < m_ndims.
   */
  virtual double getNodeCoordinate( int i, int j, int idim ) const;

  /*!
   * \brief Returns the coordinate value of the node at (i,j,k)
   * \param [in] i logical index of the node along the first dimension.
   * \param [in] j logical index of the node along the second dimension.
   * \param [in] k logical index of the node along the third dimension.
   * \param [in] idim requested coordinate dimension.
   * \return x the coordinate value of the node.
   * \pre this->getDimension()==3.
   * \pre idim >= 0 && idim < m_ndims.
   */
  virtual double getNodeCoordinate( int i, int j, int k, int idim ) const;

  /// @}

private:

  /*!
   * \brief Default constructor.
   * \note Made private to prevent users from calling it.
   */
  UniformMesh();

  double m_origin[3];
  double m_h[3];

  DISABLE_COPY_AND_ASSIGNMENT(UniformMesh);
  DISABLE_MOVE_AND_ASSIGNMENT(UniformMesh);
};

} /* namespace mint */
} /* namespace axom */

//------------------------------------------------------------------------------
//          In-lined Method Implementations
//------------------------------------------------------------------------------
namespace axom {
namespace mint {

inline void UniformMesh::getOrigin( double origin[3] ) const
{
  SLIC_ASSERT( origin != AXOM_NULLPTR );
  memcpy( origin, m_origin, 3*sizeof(double) );
}

//------------------------------------------------------------------------------
inline void UniformMesh::getSpacing( double h[3] ) const
{
  SLIC_ASSERT( h != AXOM_NULLPTR );
  memcpy( h, m_h, 3*sizeof(double) );
}

//------------------------------------------------------------------------------
inline void UniformMesh::getNode(int nodeIdx, double* coordinates) const
{
  SLIC_ASSERT(  coordinates != AXOM_NULLPTR );
  SLIC_ASSERT(  nodeIdx >= 0 && nodeIdx < this->getNumberOfNodes() );

  int ijk[3];
  m_extent->getGridIndex( nodeIdx, ijk[0], ijk[1], ijk[2] );

  for ( int i=0; i < this->getDimension(); ++i ) {
    coordinates[ i ] = m_origin[ i ] + m_h[ i ]*ijk[ i ];
  }
}

//------------------------------------------------------------------------------
inline void UniformMesh::getNode( int i, int j, double* coordinates ) const
{
  SLIC_ASSERT(  coordinates != AXOM_NULLPTR );
  SLIC_ASSERT(  this->getDimension()==2 );

  int ijk[2] = { i, j };
  for ( int d=0; d < 2; ++d ) {
    coordinates[ d ] = m_origin[ d ] + m_h[ d ]*ijk[ d ];
  }

}

//------------------------------------------------------------------------------
inline
void UniformMesh::getNode(int i, int j, int k, double* coordinates) const
{
  SLIC_ASSERT(  coordinates !=  AXOM_NULLPTR );
  SLIC_ASSERT(  this->getDimension()==3 );

  int ijk[3] = { i, j, k };
  for ( int d=0; d < 3; ++d ) {
    coordinates[ d ] = m_origin[ d ] + m_h[ d ]*ijk[ d ];
  }
}

//------------------------------------------------------------------------------
inline double UniformMesh::getNodeCoordinate( int nodeIdx, int idim ) const
{
  SLIC_ASSERT(  nodeIdx >= 0 && nodeIdx < this->getNumberOfNodes() );
  SLIC_ASSERT(  idim >= 0 && idim < this->getDimension() );

  int ijk[3];
  m_extent->getGridIndex( nodeIdx, ijk[0], ijk[1], ijk[2] );
  return ( (m_origin[ idim ] + m_h[ idim ]*ijk[ idim ]) );
}

//------------------------------------------------------------------------------
inline double UniformMesh::getNodeCoordinate( int i, int j, int idim ) const
{
  SLIC_ASSERT(  this->getDimension()==2 );
  SLIC_ASSERT(  idim >= 0 && idim < 2 );

  int ijk[2] = { i, j };
  return ( (m_origin[ idim ] + m_h[ idim ]*ijk[ idim ]) );
}

//------------------------------------------------------------------------------
inline
double UniformMesh::getNodeCoordinate( int i, int j, int k, int idim ) const
{
  SLIC_ASSERT(  this->getDimension()==3 );
  SLIC_ASSERT(  idim >= 0 && idim < 3 );

  int ijk[3] = { i, j, k };
  return ( (m_origin[ idim ] + m_h[ idim ]*ijk[ idim ]) );
}

} /* namespace mint */
} /* namespace axom */

#endif /* UNIFORMMESH_HXX_ */
