/*
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Copyright (c) 2017, Lawrence Livermore National Security, LLC.
 *
 * Produced at the Lawrence Livermore National Laboratory
 *
 * LLNL-CODE-738930
 *
 * All rights reserved.
 *
 * This file is part of the RAJA Performance Suite.
 *
 * For details about use and distribution, please read raja-perfsuite/LICENSE.
 *
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

#include "mint/Mesh.hpp"

// axom includes
#include "axom/Types.hpp"
#include "mint/FieldData.hpp"

// C/C++ includes
#include <cstddef>

namespace axom {
namespace mint {

Mesh::Mesh():
  m_ndims(2),
  m_type(MINT_UNDEFINED_MESH),
  m_block_idx(0),
  m_part_idx(0),
  m_node_data( new FieldData() ),
  m_cell_data( new FieldData() ),
  m_face_data( new FieldData() )
{}

//------------------------------------------------------------------------------
Mesh::Mesh( int ndims, int type, int blockId, int partId ):
  m_ndims( ndims ),
  m_type( type ),
  m_block_idx( blockId ),
  m_part_idx( partId ),
  m_node_data( new FieldData() ),
  m_cell_data( new FieldData() ),
  m_face_data( new FieldData() )
{}

//------------------------------------------------------------------------------
Mesh::~Mesh()
{
  m_node_data->clear();
  delete m_node_data;
  m_node_data = AXOM_NULLPTR;

  m_cell_data->clear();
  delete m_cell_data;
  m_cell_data = AXOM_NULLPTR;

  m_face_data->clear();
  delete m_face_data;
  m_face_data = AXOM_NULLPTR;
}

} /* namespace mint */
} /* namespace axom */
