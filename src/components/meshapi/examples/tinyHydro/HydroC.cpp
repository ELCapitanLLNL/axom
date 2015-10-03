#include <algorithm>
#include <stdio.h>
#include <string.h>

#include "HydroC.hpp"
#include "PolygonMeshXY.hpp"
#include "Part.hpp"
// #include "/usr/include/gperftools/profiler.h"

#include "slic/slic.hpp"

namespace tinyHydro {

//----------------------------------------------
// c'tor
Hydro::Hydro(State * s):
mesh(*(s->mesh)),
   state(*s),
   halfStep(*s),
   dState(*s),
   cycle(0),
   dtZone(-1),
   time(0.0),
   Cq(1.0),
   Cl(1.0),
   cfl(0.7)
{
   printf("using HydroC\n");

   SLIC_ASSERT(state.nParts > 0);
   SLIC_ASSERT(mesh.numZones() > 0);
   
   // make memory for Q calculation
   Q = ZonalScalarField( &mesh.zones);
   
   // make memory for Q calculation
   force = NodalVectorField(&mesh.nodes);

   /* cornerforce = new VectorXY * [nParts]; */
   /* for (int p = 0; p < nParts; p++) cornerforce[p] = new VectorXY[4*state.parts[p].nzones]; */
   
   // make memory for holding zone and node masses
   const int nParts = state.nParts;
   initialMass = new double * [nParts];
   for (int p = 0; p < nParts; p++)
       initialMass[p] = new double[state.parts[p].numZones()];



   totalMass        = ZonalScalarField( &mesh.zones);
   totalPressure    = ZonalScalarField( &mesh.zones);
   maxCs            = ZonalScalarField( &mesh.zones);

   nodeMass         = NodalScalarField( &mesh.nodes);
      
   // space for half-step velocity
   halfStepVelocity = NodalVectorField(& mesh.nodes);

   // for dt, Q, and some PdV work schemes
   divu = ZonalScalarField( &mesh.zones);


   // zone pressure
   pressure = new double * [nParts];
   for (int p = 0; p < nParts; p++)
       pressure[p] = new double[state.parts[p].numZones()];
   

   // velocity BC space.  BC's are implicitly ordered as 'bottom',
   // 'right', 'top', 'left', numbered 0 through 3.
   // initial BC's are 0xdeadbeef, which we take as a special value
   // meaning free boundary condition.

   bcVelocity = BoundaryEdgeVectorField(&boundaryEdgeSet, VectorXY(0xdeadbeef, 0xdeadbeef));

   double minX = 1e99;
   double maxX = -1e99;
   double minY = 1e99;
   double maxY = -1e99;
   for (int n = 0; n < mesh.numNodes(); n++)  // compute bounding box of domain
   {
      VectorXY & pos = mesh.nodePos[n];
      if (pos.x < minX) minX = pos.x;
      if (pos.x > maxX) maxX = pos.x;
      if (pos.y < minY) minY = pos.y;
      if (pos.y > maxY) maxY = pos.y;
   }

   std::vector<IndexType> bcNodeLists[NUM_DOMAIN_BOUNDARIES];
   for (int n = 0; n < mesh.numNodes(); n++)
   {
      VectorXY & pos = mesh.nodePos[n];
      if (fuzzyEqual(pos.y, minY)) bcNodeLists[0].push_back(n);
      if (fuzzyEqual(pos.x, maxX)) bcNodeLists[1].push_back(n);
      if (fuzzyEqual(pos.y, maxY)) bcNodeLists[2].push_back(n);
      if (fuzzyEqual(pos.x, minX)) bcNodeLists[3].push_back(n);
   }

   for(int i=0; i< boundaryEdgeSet.size(); ++i)
   {
       bcNodes[i] = NodeSubset( bcNodeLists[i].size() );
       std::vector<int>& bcVec = DataRegistry::setRegistry.addNamelessField( &bcNodes[i]).data();
       bcVec.swap( bcNodeLists[i]);
       bcNodes[i].data() = &bcVec;
   }
}
   
//----------------------------------------------
// d'tor
Hydro::~Hydro()
{

   /* delete [] cornerforce; */

   for (int p = 0; p < state.nParts; p++)
       delete [] initialMass[p];
   delete [] initialMass;

   for (int p = 0; p < state.nParts; p++)
       delete [] pressure[p];
   delete [] pressure;
}
//----------------------------------------------
// set hydro ready to begin taking steps
void Hydro::initialize(void)
{
   // calc new geometry data in mesh, including
   // new zone volumes
   mesh.computeNewGeometry();
   
   // get our state ready with initial mesh positions
   for (int i = 0; i < mesh.numNodes(); i++)
   {
      state.position[i] = mesh.nodePos[i];
   }

   // tuck away initial zone and node masses and never forget them
   totalMass.clear();
   nodeMass.clear();
   for (int p = 0; p < state.nParts; p++)  // loop over parts
   {
      ZoneSubset& zones = state.parts[p].zones;
      const ZonalScalarField& rho = state.parts[p].density;
      for (int iz = 0; iz < state.parts[p].numZones(); iz++) // loop over zones
      {
         initialMass[p][iz] = rho[iz]*mesh.zoneVolume[zones[iz]];
         totalMass[zones[iz]] += initialMass[p][iz];

         ZNodeSet zNodes = mesh.zoneToNodes[ zones[iz] ];
         for (int in = 0; in < zNodes.size(); in ++) // loop over nodes
         {
            int node = zNodes[in];
            nodeMass[node] += 0.25*initialMass[p][iz];
         }
      } // end loop over zones
   } // end loop over parts
}
   
//----------------------------------------------
// computes the time derivatives, stores them in 2nd arg
void Hydro::calcDerivs(const State & s, State & dState, double dt)
{
   // printf("Hydro:: blago calcDerivs 0\n");

   const int nnodes = mesh.numNodes();
   
   // calc forces
   calcForce(s);

   // Accelerations are the delta velocities for dState, so let's
   // label that explicitly.  Re-naming makes it less confusing, at
   // least to me.
   NodalVectorField& accel = dState.velocity;
   
   // calc accelerations:
   for (int n = 0; n < nnodes; n++)
   {
      accel[n].x = force[n].x/nodeMass[n];
      accel[n].y = force[n].y/nodeMass[n];
   }

   // halfStepVelocity = state.u + 0.5*dt*accel
   
   // Velocities must be anchored at the half-way point in time to get
   // the conservation properties we like. Note that state.u is the
   // beginning of timestep velocity, which we use even during the
   // second partial RK2 step.
   for (int i = 0; i < mesh.numNodes(); i++)
   {
      halfStepVelocity[i] = accel[i];
      halfStepVelocity[i] *= 0.5*dt;
      halfStepVelocity[i] += state.velocity[i]; 
   }
   
   // apply velocity BCs to half-step velocity to prevent
   // spurious position changes and PdV work
   applyVelocityBC(halfStepVelocity);
   
   // velocities are the delta of position for dState.
   for (int i = 0; i < mesh.numNodes(); i++)
   {
      dState.position[i] = halfStepVelocity[i];
   }
   
   // calc rate of (P+Q)dV work: to be energy conservative we want
   // the velocity divergence computed on the half-step velocity
   calcDivU(halfStepVelocity);

   ////// interestingly, in the finite volume approach, the
   ////// force.dot(velocity) energy update is algebraically identical
   ////// to the simpler P*div(u) calculation.  So we do the simpler one.
   /* const int kmax = mesh.kmax; */
   /* for (int z = 0; z < mesh.nzones; z++) */
   /* { */
   /*    dState.energyPerMass[z] = -(cornerforce[4*z].dot(halfStepVelocity[LL(z)]) */
   /*                                +cornerforce[4*z+1].dot(halfStepVelocity[LR(z)]) */
   /*                                +cornerforce[4*z+2].dot(halfStepVelocity[UR(z)]) */
   /*                                +cornerforce[4*z+3].dot(halfStepVelocity[UL(z)])) / */
   /*       initialMass[z]; */
   /* } */
   
   // the energy production rate is simply -(P+Q) div(u)
   for (int p = 0; p < s.nParts; p++)
   {
      const ZonalScalarField& rho = s.parts[p].density;
      const double * P = pressure[p];
      ZonalScalarField& de = dState.parts[p].energyPerMass;
      const int nzones = state.parts[p].numZones();
      const ZoneSubset& zones = state.parts[p].zones;
      for (int z = 0; z < nzones; z++)
      {
         de[z] = -(P[z] + Q[zones[z]]) * divu[zones[z]] / rho[z];
      }
   }
   // printf("Hydro:: blago calcDerivs 9\n");

}
//----------------------------------------------
// compute the total force that pushes the nodes
void Hydro::calcForce(const State & s)
{
   SLIC_ASSERT(s.mesh == &mesh);
   
   // compute grad of zone-centered (P+Q), return pointer to array of
   // node-centered VectorXY forces.
   force.clear();

   // zero out total before summing
   for (int z = 0; z < mesh.numZones(); z ++) totalPressure[z] = 0.0;

   // get pressure from EOS: gamma law gas, P = rho*e*(gamma-1)
   for (int p = 0; p < s.nParts; p++)
   {
      const int nzones = state.parts[p].numZones();
      const double gammaMinusOne = s.parts[p].gamma - 1.0;
      const ZonalScalarField& rho = s.parts[p].density;
      const ZonalScalarField& e = s.parts[p].energyPerMass;
      double * P = pressure[p];
      const ZoneSubset& zones = s.parts[p].zones;
      for (int z = 0; z < nzones; z++)
      {
         P[z] = gammaMinusOne*rho[z]*e[z];
         totalPressure[zones[z]] += P[z];
      }
   }
   
   // get Q
   calcQ(state);

   // compute grad via finite volume method
   // grad(P) = (1/Vn) sum(P*A) for P,A around node
   // for each zone, accumulate P*A to its nodes
   for (int iz = 0; iz < mesh.numZones(); iz++)
   {
      ZNodeSet zNodes = mesh.zoneToNodes[ iz];
      ZFaceSet zFaces = mesh.zoneToFaces[ iz];
      const int relnSize = zNodes.size();
      for (int in = 0; in < relnSize; in++)
      {
         const int node = zNodes[in];
         const int face0 = zFaces[(in-1 + relnSize) % relnSize];
         const int face1 = zFaces[in];

         VectorXY A = mesh.faceArea[face0] + mesh.faceArea[face1];
         // multiply areas by pressure + Q (and div 2 for area factor)
         double totP = 0.5 * (totalPressure[iz] + Q[iz]);
         A *= totP;
         force[node] += A;
      }
   }
}
//----------------------------------------------
// compute the artificial viscosity
void Hydro::calcQ(const State & s)
{
   // Q = rho((L*div(u))**2 - cs*L*div(u))

   // calculate divergence of velocity
   calcDivU(s.velocity);

   // set max sound speed
   calcMaxCs();

   // compute Q for each zone
   for (int z = 0; z < mesh.numZones(); z++)
   {
      if (divu[z] < 0.0)
      {
         const double vol = mesh.zoneVolume[z];
         const double L = sqrt(vol);
         const double deltaU = L*divu[z];
         double rho = totalMass[z]/vol;
         Q[z] = rho*(Cq*deltaU*deltaU - Cl*maxCs[z]*deltaU);
      }
      else
         Q[z] = 0.0;
   }
}

//----------------------------------------------
// update all the algebraically determinable stuff
void Hydro::updateConstitutives(State & s)
{
   // move mesh to new node positions
   mesh.moveNodesToPosition(s.position);

   // calc new geometry data in mesh, including
   // new zone volumes
   mesh.computeNewGeometry();
   for (int p = 0; p < s.nParts; p++)
   {
      const ZoneSubset& zones = s.parts[p].zones;
      const int nzones = s.parts[p].numZones();
      ZonalScalarField& rho = s.parts[p].density;
      for (int z = 0; z < nzones; z++)
      {
         rho[z] = initialMass[p][z]/mesh.zoneVolume[zones[z]];
      }
   }

   // apply BC's to newly computed velocity
   applyVelocityBC(s.velocity);
}

//----------------------------------------------
void Hydro::step(double dt)
{
   printf("cycle %d  time = %f  dt = %f\n", cycle, time, dt);
   // printf("Hydro::step blago 1\n");

//printf("\n -- Dumping state: --\n");
//state.dumpState();

   // ----------------------------------
   // update physics variables using RK2
   // ----------------------------------
   // printf("Hydro::step blago 2\n");
   time += 0.5*dt;
   calcDerivs(state, dState, 0.5*dt); // dState = time derivatives
   // halfstep = state + dt/2 * dState
   halfStep = state;

   dState *= 0.5*dt;
   halfStep += dState;

   // update all the algebraically determinable stuff
   updateConstitutives(halfStep);

   // second step of our RK2 integration
   calcDerivs(halfStep, dState, dt);
   // final state = state + dt * dState(halfStep)
   dState *= dt;
   state += dState;
   
   // update all the algebraically determinable stuff
   updateConstitutives(state);

   // update control info
   time += 0.5*dt; // full time update
   cycle++;

   // printf("Hydro::step blago 9\n");
      
}
//----------------------------------------------
// calc max sound speed in each zone
void Hydro::calcMaxCs(void)
{
   // zero to start with
   maxCs.clear();
   
   for (int p = 0; p < state.nParts; p++)
   {
      const ZoneSubset& zones = state.parts[p].zones;
      const int nzones = state.parts[p].numZones();
      const double gammaGammaMinusOne = state.parts[p].gamma*(state.parts[p].gamma - 1.0);
      const ZonalScalarField& e = state.parts[p].energyPerMass;
      for (int z = 0; z < nzones; z++)
      {
         double cs2 = gammaGammaMinusOne*e[z];
         maxCs[zones[z]] = maxCs[zones[z]] > cs2 ? maxCs[zones[z]] : cs2;
      }
   }
   // we saved all the sqrts for the very end
   for (int z = 0; z < mesh.numZones(); z++)
   {
      maxCs[z] = sqrt(maxCs[z]);
   }
}

//----------------------------------------------
double Hydro::newDT(void)
{
   double minDT = 1.0e99; // impossibly high dt
   dtZone = -1; // impossible zone index

   calcDivU(state.velocity);  // we need velocity divergence
   calcMaxCs(); // need max sound speed 
   
   for (int z = 0; z < mesh.numZones(); z++)
   {
      double L2 = mesh.zoneVolume[z];
      double dt2 = L2/(maxCs[z]*maxCs[z] + L2*divu[z]*divu[z]);
      if (minDT >= dt2)
      {
         minDT = dt2;
         dtZone = z;
      }
   }

   //printf("\n\t\tMin DT is %g -- computed in zone %i \n\n", minDT, dtZone);

   return cfl*sqrt(minDT);
}
//----------------------------------------------
// compute the divergence of velocity
void Hydro::calcDivU(const NodalVectorField& velocity)
{
   // div u = 1/V * sum(u dot dA)
   for (int iz = 0; iz < mesh.numZones(); iz++)
   {
      divu[iz] = 0.0;

      ZNodeSet zNodes = mesh.zoneToNodes[ iz];
      ZFaceSet zFaces = mesh.zoneToFaces[ iz];
      const int relnSize = zNodes.size();
      for (int in = 0; in < relnSize; in++)
      {
         const int node = zNodes[in];
         const int face0 = zFaces[(in-1 + relnSize) % relnSize];
         const int face1 = zFaces[in];

         VectorXY A = mesh.faceArea[face0] + mesh.faceArea[face1];
         divu[iz] += velocity[node].dot(A);
      }
      divu[iz] /= 2.0*mesh.zoneVolume[iz]; // the 2 normalizes the areas
   }
}

//----------------------------------------------
void Hydro::setBC(const char * boundary, double xVel, double yVel)
{
   switch (boundary[0] )
   {
   case 'b':
      bcVelocity[0] = VectorXY(xVel, yVel);
      break;
   case 'r':
      bcVelocity[1] = VectorXY(xVel, yVel);
      break;
   case 't':
      bcVelocity[2] = VectorXY(xVel, yVel);
      break;
   case 'l':
      bcVelocity[3] = VectorXY(xVel, yVel);
      break;
   default:
      printf("ERROR, bad value specified for BC\n");
      break;
   }
}
//----------------------------------------------
void Hydro::applyVelocityBC(NodalVectorField& u)
{
   for(int i= 0; i < boundaryEdgeSet.size(); ++i)
   {
       if (bcVelocity[i].x != 0xdeadbeef)
       {
          for(NodeSubset::iterator nIt = bcNodes[i].begin(), nItEnd = bcNodes[i].end(); nIt < nItEnd; ++nIt)
             u[*nIt].x = bcVelocity[i].x;
       }
       if (bcVelocity[i].y != 0xdeadbeef)
       {
          for(NodeSubset::iterator nIt = bcNodes[i].begin(), nItEnd = bcNodes[i].end(); nIt < nItEnd; ++nIt)
             u[*nIt].y = bcVelocity[i].y;
       }
   }
}
//----------------------------------------------
void Hydro::steps(int numSteps)
{
   for (int i = 0; i < numSteps; i++)
   {
      double dt = newDT();
      step(dt);
   }
}
//----------------------------------------------
void Hydro::advance(double stopTime)
{
   /* ProfilerStart("profile.out"); */
   /* struct ProfilerState ps; */
   
   while (time < stopTime)
   {
      double dt = newDT();
      step(dt);
   }
   
   /* ProfilerGetCurrentState(&ps); */
   /* printf("profiler: enabled = %d, samples gathered = %d\n", ps.enabled, ps.samples_gathered); */
   /* ProfilerStop(); */
   
}
//----------------------------------------------
double Hydro::totalEnergy(const State & s) const
{
   double totE = 0.0;
   for (int p = 0; p < s.nParts; p++)
   {
      const int nzones = s.parts[p].numZones();
      const ZonalScalarField& e = s.parts[p].energyPerMass;
      const double * m = initialMass[p];
      for (int z = 0; z < nzones; z++)
      {
         totE += e[z]*m[z];
      }
   }
   for (int n = 0; n < mesh.numNodes(); n++)
   {
      totE += 0.5 * nodeMass[n] * s.velocity[n].dot(s.velocity[n]);
   }
   return totE;
}
//----------------------------------------------
int Hydro::numBCnodes(int bc)
{
    SLIC_ASSERT_MSG( 0 <= bc && bc < NUM_DOMAIN_BOUNDARIES
               , "BC identifier must be between 0 and " << NUM_DOMAIN_BOUNDARIES);

    return bcNodes[bc].size();
}
//----------------------------------------------
int Hydro::bcNode(int bc, int node)
{
    SLIC_ASSERT_MSG( 0 <= bc && bc < NUM_DOMAIN_BOUNDARIES
               , "BC identifier must be between 0 and " << NUM_DOMAIN_BOUNDARIES);

    return bcNodes[bc][node];
}
//----------------------------------------------
double Hydro::totalEnergy(void) const
{
   return totalEnergy(state);
}
//----------------------------------------------


} // end namespace tinyHydro
