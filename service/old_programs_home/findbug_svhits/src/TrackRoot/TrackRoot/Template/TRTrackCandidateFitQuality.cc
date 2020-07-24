#if !defined(TRACKROOT_TRTRACKCANDIDATEFITQUALITY_CC)
#define TRACKROOT_TRTRACKCANDIDATEFITQUALITY_CC
// -*- C++ -*-
//
// Package:     TrackRoot
// Module:      TRTrackCandidateFitQuality
// 
// Description: Type-differentiated fit track quality object
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Michael Marsh
// Created:     Fri Dec 18 10:18:07 EST 1998
// $Id: TRTrackCandidateFitQuality.cc,v 1.6 2001/01/05 05:27:43 lyon Exp $
//
// Revision history
//
// $Log: TRTrackCandidateFitQuality.cc,v $
// Revision 1.6  2001/01/05 05:27:43  lyon
// Added LayerMap and other changes to quality objects
//
// Revision 1.5  2000/12/22 06:56:18  wsun
// New hit and residual functions that can be used with CLEO II, II.V, or III.
//
// Revision 1.4  2000/12/14 16:01:34  wsun
// Added #hits, average residuals, and rms residuals by subdetector.
//
// Revision 1.3  2000/08/01 23:29:43  wsun
// Updated to new TRTrackCandidateFitQuality.
//
// Revision 1.2  1998/12/20 21:56:48  marsh
// Protected against multiple definition of constructor.
//
// Revision 1.1  1998/12/18 19:37:29  marsh
// Added templated fit track quality object to provide type-differentiation
// for Frame storage.
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "TrackRoot/TRTrackCandidateFitQuality.h"

// STL classes

//
// constants, enums and typedefs
//

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: TRTrackCandidateFitQuality.cc,v 1.6 2001/01/05 05:27:43 lyon Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
#if !defined(TYPEDEFS_IN_TEMPLATES_IN_ARGS_BUG)
#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
TRTrackCandidateFitQuality< H >::TRTrackCandidateFitQuality(
   Identifier identifier,
   double chiSquare,
   int degreesOfFreedom,
   int numberHits,
   int numberHitsExpected,
   int numberHitsInput,
   int numberHitsDropped,
   int numberIterations,
   DABoolean fit,
   DABoolean fitAbort,
   TRSubdetectorLists::Detectors detector,
   const STL_VECTOR( int )& numberHitsVec,
   const STL_VECTOR( double )& averageResiduals,
   const STL_VECTOR( double )& rmsResiduals,
   const TRLayerMap& layerMap )
   : TRTrackFitQuality( identifier,
			chiSquare,
			degreesOfFreedom,
			numberHits,
			numberHitsExpected,
			numberHitsInput,
			numberHitsDropped,
			numberIterations,
			fit,
			fitAbort,
			detector,
			numberHitsVec,
			averageResiduals,
			rmsResiduals,
			layerMap)
{
}
#endif /* TYPEDEFS_IN_TEMPLATES_IN_ARGS_BUG */

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
TRTrackCandidateFitQuality< H >::~TRTrackCandidateFitQuality()
{
}

#endif /* TRACKROOT_TRTRACKCANDIDATEFITQUALITY_CC */
