// -*- C++ -*-
//
// Package:     TrackFitter
// Module:      TrackFitCathodeHitLink
// 
// Description: Link data for fit track <--> Cathode hit Lattices
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Mon Sep  6 14:36:22 EDT 1999
// $Id: TrackFitCathodeHitLink.cc,v 1.3 2000/08/16 19:34:21 wsun Exp $
//
// Revision history
//
// $Log: TrackFitCathodeHitLink.cc,v $
// Revision 1.3  2000/08/16 19:34:21  wsun
// Removed curvature from TrackFitHitLink.
//
// Revision 1.2  2000/02/24 17:37:42  wsun
// Updated for storage helpers.
//
// Revision 1.1  1999/09/10 20:23:41  wsun
// First submission.
//
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "TrackFitter/TrackFitCathodeHitLink.h"

// STL classes
#include <vector>
#include <map>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "TrackFitter.TrackFitCathodeHitLink" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: TrackFitCathodeHitLink.cc,v 1.3 2000/08/16 19:34:21 wsun Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
TrackFitCathodeHitLink::TrackFitCathodeHitLink() :
//   TrackFitHitLink( false, 0, 1E10, 100, 0 )
   TrackFitHitLink( false, 0, 1E10, 0 )
{
}

TrackFitCathodeHitLink::TrackFitCathodeHitLink( DABoolean used,
						double residual,
						double resError,
//						double curvature,
						double momentum ) :
//   TrackFitHitLink( used, residual, resError, curvature, momentum )
   TrackFitHitLink( used, residual, resError, momentum )
{
}

TrackFitCathodeHitLink::TrackFitCathodeHitLink(
   const TrackFitCathodeHitLink& rhs )
   : TrackFitHitLink( rhs )
{
}

const TrackFitCathodeHitLink& TrackFitCathodeHitLink::operator=(
   const TrackFitCathodeHitLink& rhs )
{
   if( this != &rhs )
   {
      TrackFitHitLink::operator=( rhs ) ;
   }
   return *this ;
}

TrackFitCathodeHitLink::~TrackFitCathodeHitLink()
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
