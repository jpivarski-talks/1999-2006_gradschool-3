// -*- C++ -*-
//
// Package:     <DualTrackProd>
// Module:      DualTrackFitHelix
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Thu Dec 11 15:13:44 EST 2003
// $Id: DualTrackFitHelix.cc,v 1.1 2003/12/15 17:53:08 mccann Exp $
//
// Revision history
//
// $Log: DualTrackFitHelix.cc,v $
// Revision 1.1  2003/12/15 17:53:08  mccann
// adding hit lattices for dual-constrained residuals
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
#include <assert.h>

// user include files
//#include "Experiment/report.h"
#include "DualTrackProd/DualTrackFitHelix.h"


//
// constants, enums and typedefs
//

static const char* const kFacilityString = "DualTrackProd.DualTrackFitHelix" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DualTrackFitHelix.cc,v 1.1 2003/12/15 17:53:08 mccann Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//

DualTrackFitHelix::DualTrackFitHelix(unsigned int id, const KTHelix& copy_helix)
   : HIFitHelix(copy_helix, 0., true, 0., id)
{
   assert(identifier() == id);
   assert(!hasNullErrorMatrix());

   setReferencePoint(HepPoint3D(0., 0., 0.));
}

DualTrackFitHelix::DualTrackFitHelix()
   : HIFitHelix(KTHelix(0., 0., 0., 0., 0.), 0., true, 0., 0)
{
}

DualTrackFitHelix::DualTrackFitHelix(const DualTrackFitHelix& copy_helix)
   : HIFitHelix(copy_helix, 0., true, 0., copy_helix.identifier())
{
}

DualTrackFitHelix::~DualTrackFitHelix()
{
}

//
// assignment operators
//
// const DualTrackFitHelix& DualTrackFitHelix::operator=( const DualTrackFitHelix& rhs )
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
