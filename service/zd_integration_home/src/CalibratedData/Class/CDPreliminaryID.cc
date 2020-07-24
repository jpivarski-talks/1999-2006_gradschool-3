// -*- C++ -*-
//
// Package:     <CalibratedData>
// Module:      CDPreliminaryID
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Adam Lyon
// Created:     Tue Feb  8 11:28:49 EST 2000
// $Id: CDPreliminaryID.cc,v 1.1 2000/02/08 18:26:12 lyon Exp $
//
// Revision history
//
// $Log: CDPreliminaryID.cc,v $
// Revision 1.1  2000/02/08 18:26:12  lyon
// Moved from CalibratedDRHitProd
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "CalibratedData/CDPreliminaryID.h"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "CalibratedData.CDPreliminaryID" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: CDPreliminaryID.cc,v 1.1 2000/02/08 18:26:12 lyon Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//
const char* const CDPreliminaryID::kSecondaryLabel = 
               "Preliminary";
//
// constructors and destructor
//
CDPreliminaryID::CDPreliminaryID()
{
}

// CDPreliminaryID::CDPreliminaryID( const CDPreliminaryID& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

CDPreliminaryID::~CDPreliminaryID()
{
}

//
// assignment operators
//
// const CDPreliminaryID& CDPreliminaryID::operator=( const CDPreliminaryID& rhs )
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
