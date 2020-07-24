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
// $Id$
//
// Revision history
//
// $Log$

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
static const char* const kIdString  = "$Id: skeleton.cc,v 1.6 2001/01/03 16:25:23 cdj Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
DualTrack::DualTrack()
   : m_positive(KTHelix(0., 0., 0., 0., 0.))
   , m_negative(KTHelix(0., 0., 0., 0., 0.))
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
   : m_positive(*positive, true)
   , m_negative(*negative, true)
   , m_pVirtualPhoton(pVirtualPhoton)
   , m_pointConstraint(pointConstraint)
   , m_ptConstraint(ptConstraint)
   , m_pzConstraint(pzConstraint)
   , m_constraint(magneticField, pVirtualPhoton, pointConstraint, ptConstraint, pzConstraint)
{
   m_positive.setIdentifier(posid);
   m_negative.setIdentifier(negid);
   m_positive.setReferencePoint(HepPoint3D(0.,0.,0.));
   m_negative.setReferencePoint(HepPoint3D(0.,0.,0.));

   if (m_pointConstraint  ||  m_ptConstraint  ||  m_pzConstraint) {
      vector<HIFitHelix*> vect;
      vect.push_back(&m_positive);
      vect.push_back(&m_negative);
      m_results = m_constraint.applyConstraint(vect);
   }
   else {
      m_results = HIFitConstraint::Results(false);
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
