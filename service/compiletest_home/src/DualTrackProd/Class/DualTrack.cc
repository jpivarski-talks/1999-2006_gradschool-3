// -*- C++ -*-
//
// Package:     <DualTrackProd>
// Module:      DualTrack
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Wed Dec 10 11:06:31 EST 2003
// $Id: DualTrack.cc,v 1.2 2003/12/15 17:53:05 mccann Exp $
//
// Revision history
//
// $Log: DualTrack.cc,v $
// Revision 1.2  2003/12/15 17:53:05  mccann
// adding hit lattices for dual-constrained residuals
//
// Revision 1.1.1.1  2003/12/11 17:14:28  mccann
// imported DualTrackProd sources
//

#include "Experiment/Experiment.h"

// system include files
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

// user include files
//#include "Experiment/report.h"
#include "DualTrackProd/DualTrack.h"


//
// constants, enums and typedefs
//

static const char* const kFacilityString = "DualTrackProd.DualTrack" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DualTrack.cc,v 1.2 2003/12/15 17:53:05 mccann Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
DualTrack::DualTrack()
   : m_positive()
   , m_negative()
   , m_pVirtualPhoton(0., 0., 0.)
   , m_pointConstraint(false)
   , m_ptConstraint(false)
   , m_pzConstraint(false)
   , m_constraint(0., m_pVirtualPhoton, m_pointConstraint, m_ptConstraint, m_pzConstraint)
   , m_results(false)
{
}

DualTrack::DualTrack(unsigned int posid,
		     const KTHelix* positive,
		     unsigned int negid,
		     const KTHelix* negative,
		     double magneticField,
		     HepVector3D& pVirtualPhoton,
		     DABoolean pointConstraint,
		     DABoolean ptConstraint,
		     DABoolean pzConstraint)
   : m_positive(posid, *positive)
   , m_negative(negid, *negative)
   , m_pVirtualPhoton(pVirtualPhoton)
   , m_pointConstraint(pointConstraint)
   , m_ptConstraint(ptConstraint)
   , m_pzConstraint(pzConstraint)
   , m_constraint(magneticField, pVirtualPhoton, pointConstraint, ptConstraint, pzConstraint)
{
   if (m_pointConstraint  ||  m_ptConstraint  ||  m_pzConstraint) {
      vector<HIFitHelix*> vect;
      vect.push_back(&m_positive);
      vect.push_back(&m_negative);
      m_results = m_constraint.applyConstraint(vect);
   }
   else {
      m_results = HIFitConstraint::Results(true);
   }
}

// DualTrack::DualTrack( const DualTrack& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

DualTrack::~DualTrack()
{
}

//
// assignment operators
//
// const DualTrack& DualTrack::operator=( const DualTrack& rhs )
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
