// -*- C++ -*-
//
// Package:     TrackFitter
// Module:      TrackFitDRHitLink
// 
// Description: Link data for fit track <--> DR hit Lattices
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Michael Marsh
// Created:     Mon Dec 14 13:45:48 EST 1998
// $Id: TrackFitDRHitLink.cc,v 1.15 2002/06/12 20:47:19 cleo3 Exp $
//
// Revision history
//
// $Log: TrackFitDRHitLink.cc,v $
// Revision 1.15  2002/06/12 20:47:19  cleo3
// inlined the constructors and let the compiler write the methods it can
//
// Revision 1.14  2001/09/26 08:01:15  wsun
// Added signed drift distance as data member.
//
// Revision 1.13  2000/10/11 06:31:14  wsun
// Added fittingWeight() to TrackFitDRHitLink.
//
// Revision 1.12  2000/08/16 19:34:22  wsun
// Removed curvature from TrackFitHitLink.
//
// Revision 1.11  2000/06/16 19:30:19  marsh
// Purged tool box typedefs.
//
// Revision 1.10  2000/06/09 00:19:36  wsun
// Added driftTime data member.
//
// Revision 1.9  2000/02/24 17:37:43  wsun
// Updated for storage helpers.
//
// Revision 1.8  1999/11/03 03:21:00  wsun
// Added signedDcaError(), signedDriftDistance(), and signedDriftError().
//
// Revision 1.7  1999/08/27 19:59:53  wsun
// Added signed dca to wire and sin(entrance angle).
//
// Revision 1.6  1999/05/09 21:32:16  wsun
// Implemented operator=()
//
// Revision 1.5  1999/04/26 20:35:15  marsh
// Made link data classes inherit a common base class.
//
// Revision 1.4  1999/03/13 16:16:43  marsh
// Added m_spacePoint
//
// Revision 1.3  1999/01/26 22:44:35  marsh
// Added error on residual, curvature, and momentum.
//
// Revision 1.2  1998/12/24 14:28:40  marsh
// Added include statements for vector.
//
// Revision 1.1  1998/12/15 20:32:52  marsh
// Added definitions of fit track to hit Lattices and link data.
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */
#include "C++Std/iostream.h"

// user include files
//#include "Experiment/report.h"
#include "TrackFitter/TrackFitDRHitLink.h"

// STL classes
#include <vector>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "TrackFitter.TrackFitDRHitLink" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: TrackFitDRHitLink.cc,v 1.15 2002/06/12 20:47:19 cleo3 Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//

DABoolean TrackFitDRHitLink::operator==(
   const TrackFitDRHitLink& rhs ) const
{
   return( TrackFitHitLink::operator==( rhs ) &&
	   m_trackRefPt       == rhs.m_trackRefPt &&
	   m_dca              == rhs.m_dca &&
	   m_dcaError         == rhs.m_dcaError &&
	   m_sinTrackToRadial == rhs.m_sinTrackToRadial &&
	   m_driftTime        == rhs.m_driftTime &&
	   m_fittingWeight    == rhs.m_fittingWeight &&
	   m_signedDriftDistance == rhs.m_signedDriftDistance ) ;
}

DABoolean TrackFitDRHitLink::operator!=(
   const TrackFitDRHitLink& rhs ) const
{
   return !( *this == rhs ) ;
}

/*
TrackFitDRHitLink::~TrackFitDRHitLink()
{
}
*/
//
// member functions
//

//
// const member functions
//

//
// static member functions
//
