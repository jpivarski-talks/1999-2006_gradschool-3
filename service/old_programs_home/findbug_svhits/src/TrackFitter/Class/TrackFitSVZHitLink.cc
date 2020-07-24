// -*- C++ -*-
//
// Package:     TrackFitter
// Module:      TrackFitSVZHitLink
// 
// Description: Link data for fit track <--> SVZ hit Lattices
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Michael Marsh
// Created:     Mon Dec 14 13:45:48 EST 1998
// $Id: TrackFitSVZHitLink.cc,v 1.8 2000/08/16 19:34:23 wsun Exp $
//
// Revision history
//
// $Log: TrackFitSVZHitLink.cc,v $
// Revision 1.8  2000/08/16 19:34:23  wsun
// Removed curvature from TrackFitHitLink.
//
// Revision 1.7  2000/02/24 17:37:44  wsun
// Updated for storage helpers.
//
// Revision 1.6  1999/06/21 22:53:12  pg
//  Modifications for new Si hits.
//
// Revision 1.5  1999/05/09 21:32:19  wsun
// Implemented operator=()
//
// Revision 1.4  1999/04/26 20:35:17  marsh
// Made link data classes inherit a common base class.
//
// Revision 1.3  1999/01/26 22:44:36  marsh
// Added error on residual, curvature, and momentum.
//
// Revision 1.2  1998/12/24 14:28:42  marsh
// Added include statements for vector.
//
// Revision 1.1  1998/12/15 20:32:53  marsh
// Added definitions of fit track to hit Lattices and link data.
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "TrackFitter/TrackFitSVZHitLink.h"

// STL classes
#include <vector>
#include <map>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "TrackFitter.TrackFitSVZHitLink" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: TrackFitSVZHitLink.cc,v 1.8 2000/08/16 19:34:23 wsun Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
TrackFitSVZHitLink::TrackFitSVZHitLink() :
//   TrackFitHitLink( false, 0, 1E10, 100, 0 )
   TrackFitHitLink( false, 0, 1E10, 0 )
{
}

TrackFitSVZHitLink::TrackFitSVZHitLink( DABoolean used,
                                        double residual,
                                        double resError,
//                                        double curvature,
                                        double momentum ) :
//   TrackFitHitLink( used, residual, resError, curvature, momentum )
   TrackFitHitLink( used, residual, resError, momentum )
{
}

TrackFitSVZHitLink::TrackFitSVZHitLink( const TrackFitSVZHitLink& rhs )
   : TrackFitHitLink( rhs )
{
}

const TrackFitSVZHitLink& TrackFitSVZHitLink::operator=(
   const TrackFitSVZHitLink& rhs )
{
   if( this != &rhs )
   {
      TrackFitHitLink::operator=( rhs ) ;
   }
   return *this ;
}

TrackFitSVZHitLink::~TrackFitSVZHitLink()
{
}

//
// member functions
//

//
// const member functions
//

//
// static member functions
//
