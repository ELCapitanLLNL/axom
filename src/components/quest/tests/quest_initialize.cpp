

// Axom includes
#include "mint/UnstructuredMesh.hpp"
#include "slic/slic.hpp"
#include "slic/UnitTestLogger.hpp"
#include "quest_test_utilities.hpp"
#include "quest/quest.hpp"

#include "fmt/format.h"

#ifdef AXOM_USE_MPI
  #include <mpi.h>
#endif

// Google test include
#include "gtest/gtest.h"

typedef axom::mint::UnstructuredMesh< MINT_TRIANGLE > TriangleMesh;

TEST( quest_interface, pointer_initialize )
{
  const int IGNORE = -1;

  SLIC_INFO(fmt::format("Initializing InOutOctree over triangle mesh ..."));

  axom::mint::Mesh* input_mesh =
    axom::quest::utilities::make_tetrahedron_mesh();

#ifdef AXOM_USE_MPI
  axom::quest::initialize(MPI_COMM_WORLD, input_mesh, false, 3, IGNORE, IGNORE);
#else
  axom::quest::initialize(input_mesh, false, 3, IGNORE, IGNORE);
#endif

  EXPECT_TRUE(axom::quest::inside(3, 2, 0));
  EXPECT_TRUE(axom::quest::inside(-1, 2, -1));
  EXPECT_FALSE(axom::quest::inside(4, 4, -7));

  axom::quest::finalize();
  delete input_mesh;

}

int main( int argc, char** argv )
{
#ifdef AXOM_USE_MPI
  // Initialize MPI
  MPI_Init( &argc, &argv );
#endif
  ::testing::InitGoogleTest(&argc, argv);

  int result = RUN_ALL_TESTS();

#ifdef AXOM_USE_MPI
  MPI_Finalize();
#endif

  return result;
}
