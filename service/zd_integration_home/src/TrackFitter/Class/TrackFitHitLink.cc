// -*- C++ -*-
//
// Package:     TrackFitter
// Module:      TrackFitHitLink
// 
// Description: Link data base class for fit track <--> hit Lattices
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Michael Marsh
// Created:     Mon Apr 26 15:44:00 EDT 1999
// $Id: TrackFitHitLink.cc,v 1.5 2002/06/12 20:47:37 cleo3 Exp $
//
// Revision history
//
// $Log: TrackFitHitLink.cc,v $
// Revision 1.5  2002/06/12 20:47:37  cleo3
// let the compiler automatically write what member functions it can
//
// Revision 1.4  2000/08/16 19:34:22  wsun
// Removed curvature from TrackFitHitLink.
//
// Revision 1.3  2000/02/24 17:37:43  wsun
// Updated for storage helpers.
//
// Revision 1.2  1999/05/09 21:32:17  wsun
// Implemented operator=()
//
// Revision 1.1  1999/04/26 20:35:16  marsh
// Made link data classes inherit a common base class.
//
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "TrackFitter/TrackFitHitLink.h"

// STL classes
#include <vector>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "TrackFitter.TrackFitHitLink" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: TrackFitHitLink.cc,v 1.5 2002/06/12 20:47:37 cleo3 Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
TrackFitHitLink::TrackFitHitLink() :
   m_used( false ),
   m_residual( 0 ),
   m_residualError( 1E10 ),
//   m_curvature( 100 ),
   m_momentum( 0 )
{
}

TrackFitHitLink::TrackFitHitLink( DABoolean used,
                                  double residual,
                                  double resError,
//                                  double curvature,
                                  double momentum ) :
   m_used( used ),
   m_residual( residual ),
   m_residualError( resError ),
//   m_curvature( curvature ),
   m_momentum( momentum )
{
}

DABoolean TrackFitHitLink::operator==( const TrackFitHitLink& aLinkData ) const
{
   return( m_used          == aLinkData.m_used &&
	   m_residual      == aLinkData.m_residual &&
	   m_residualError == aLinkData.m_residualError &&
//	   m_curvature     == aLinkData.m_curvature &&
	   m_momentum      == aLinkData.m_momentum ) ;
}

DABoolean TrackFitHitLink::operator!=( const TrackFitHitLink& aLinkData ) const
{
   return !( *this == aLinkData ) ;
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
