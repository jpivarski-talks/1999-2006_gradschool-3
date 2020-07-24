// -*- C++ -*-
//
// Package:     <DualTrackHelices>
// Module:      DualTrackHelices
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Lauren Hsu
// Created:     Fri Feb 25 17:13:02 EST 2000
// $Id: DualTrackHelices.cc,v 1.3 2001/08/31 20:35:35 mccann Exp $
//
// Revision history
//
// $Log: DualTrackHelices.cc,v $
// Revision 1.3  2001/08/31 20:35:35  mccann
// added some diagnostics to DualTrackHelices object
//
// Revision 1.2  2001/08/16 16:44:11  mccann
// Added chisq() and ndof() of constraint to DualTrackHelices object for testing for final-state radiation
//
// Revision 1.1.1.1  2001/01/30 23:57:19  llh14
// imported DualTrackHelices sources
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "DualTrackHelices/DualTrackHelices.h"
#include "FrameAccess/FATable.h"

#include "KinematicTrajectory/KTHelix.h"
#include "KinematicTrajectory/KTKinematicData.h"

#include "HelixIntersection/HIHelix.h"
#include "HelixIntersection/HIFitHelix.h"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "DualTrackHelices.DualTrackHelices" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DualTrackHelices.cc,v 1.3 2001/08/31 20:35:35 mccann Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//

DualTrackHelices::DualTrackHelices(const Identifier& identifier, 
	double curvature, double phi0, double d0, 
	double cotTheta, double z0, STL_VECTOR(double) errVector) 
   : m_identifier(identifier)
   , m_curvature(curvature)
   , m_phi0(phi0)
   , m_d0(d0)
   , m_cotTheta(cotTheta)
   , m_z0(z0)
   , m_errVector(errVector)
   , m_results()
   , m_chisq(0.)
   , m_ndof(0)
   , m_deltaTrackParam()
{
}

DualTrackHelices::DualTrackHelices(Identifier identifier,
	 const HIFitConstraint::Results& results, 
	 const HIFitHelix& modifiedHelix )
   : m_identifier(identifier)
   , m_curvature(modifiedHelix.curvature())
   , m_phi0(modifiedHelix.phi0())
   , m_d0(modifiedHelix.d0())
   , m_cotTheta(modifiedHelix.cotTheta())
   , m_z0(modifiedHelix.z0())
   , m_errVector(vector<double>())
   , m_results(results)
   , m_chisq(results.chisq())
   , m_ndof(results.ndof())
   , m_deltaTrackParam(results.deltaTrackParameters())
{
   // compress SYMMETRIC matrix into an STL_VECTOR for storage
   // Note that:  SymMatrix numbering starts at 1,1
   // and elements are stored in lower triangle so that row >=col
   const HepSymMatrix errMatrix = modifiedHelix.errorMatrix();

   for(Count row=1; row<= HIHelix::kNTrackParameters; row++)
      for(Count col=1; col <= row; col++)
         m_errVector.push_back( (errMatrix(row, col)) );
}

// DualTrackHelices::DualTrackHelices( const DualTrackHelices& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

DualTrackHelices::~DualTrackHelices()
{
}

DABoolean
DualTrackHelices::operator==(const DualTrackHelices& rhs) const

{  
   return ( ( identifier()==rhs.identifier() ) &&  
	    ( curvature()==rhs.curvature() ) && 
	    ( phi0()==rhs.phi0() ) &&
            ( d0()==rhs.d0() ) &&
            ( cotTheta()==rhs.cotTheta() ) &&
	    ( z0()==rhs.z0() ) &&	    
	    ( storeErrMatrix() == rhs.storeErrMatrix() )
      );
}


DABoolean
DualTrackHelices::operator!=(const DualTrackHelices& rhs) const

{  
   return !( ( identifier()==rhs.identifier() ) &&  
	     ( curvature()==rhs.curvature() ) && 
	     ( phi0()==rhs.phi0() ) &&
	     ( d0()==rhs.d0() ) &&
	     ( cotTheta()==rhs.cotTheta() ) &&
	     ( z0()==rhs.z0() ) &&	    
	     ( storeErrMatrix() == rhs.storeErrMatrix() )
      );
}


/////////////////////////////////////////////////////////////////////////////
double
DualTrackHelices::curvature() const
{ 
   return m_curvature;   

}

double
DualTrackHelices::phi0() const
{ 
   return m_phi0;

}

double
DualTrackHelices::d0() const
{ 
   return m_d0;

}

double
DualTrackHelices::cotTheta() const
{ 
   return m_cotTheta;

}

double
DualTrackHelices::z0() const
{ 
   return m_z0;

}

double
DualTrackHelices::chisq() const
{ 
   return m_chisq;

}

unsigned int
DualTrackHelices::ndof() const
{ 
   return m_ndof;

}

STL_VECTOR(double)
DualTrackHelices::storeErrMatrix() const
{ 

   return m_errVector;

}

HepSymMatrix 
DualTrackHelices::errMatrix() const
{ 

   // reload the vector into our SymMatrix
   // Note that:  SymMatrix numbering starts at 1,1 
   // and elements are stored in lower triangle so that row >=col
   Count i=0;
   HepSymMatrix errMatrix(HIHelix::kNTrackParameters);

   for(Count row=1; row<= HIHelix::kNTrackParameters; row++)
   {
      for(Count col=1; col <= row; col++)
       
      { 
	 errMatrix(row, col) = (m_errVector[i]); 
	 i++;
      }

   } 

   return errMatrix;

}

HepSymMatrix
DualTrackHelices::trackParamErrMatrix() const
{
   return m_results.trackParameterErrorMatrix();
}

HepVector
DualTrackHelices::deltaTrackParam() const
{
   return m_deltaTrackParam;
}

HIHelix
DualTrackHelices::helix() const

{

   // reload the vector into our SymMatrix
   // Note that:  SymMatrix numbering starts at 1,1 
   // and elements are stored in lower triangle so that row >=col
   Count i=0;
   HepSymMatrix errMatrix(5);

   for(Count row=1; row<= 5; row++)
   {
      for(Count col=1; col <= row; col++)
       
      { 
	 errMatrix(row, col) = (m_errVector[i]); 
	 i++;
      }

   } 

   
   KTHelix newHelix(m_curvature, m_phi0, m_d0, m_cotTheta, m_z0, errMatrix);

// Make an HIHelix from the helix
   HIHelix helix( newHelix, DBCandidate::mass(DBCandidate::kMuon) );

   return helix;

}
//
// assignment operators
//
// const DualTrackHelices& DualTrackHelices::operator=( const DualTrackHelices& rhs )
// {
//   if( this != &rhs ) {
//      // do actual copying here, plus:
//      // "SuperClass"::operator=( rhs );
//   }
//
//   return *this;
// }

//
// member functions
//

//
// const member functions
//

//
// static member functions
//
