#if !defined(KALMANFILTER_KALMANCANDIDATEQUALITY_CC)
#define KALMANFILTER_KALMANCANDIDATEQUALITY_CC
// -*- C++ -*-
//
// Package:     KalmanFilter
// Module:      KalmanCandidateQuality
// 
// Description: Type-differentiated fit track quality object
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Sat Aug  5 23:42:10 EDT 2000
// $Id: KalmanCandidateQuality.cc,v 1.5 2001/01/05 05:33:13 lyon Exp $
//
// Revision history
//
// $Log: KalmanCandidateQuality.cc,v $
// Revision 1.5  2001/01/05 05:33:13  lyon
// Added LayerMap to quality object
//
// Revision 1.4  2000/12/22 07:02:32  wsun
// Updated to new TRTrackFitQuality.
//
// Revision 1.3  2000/12/15 20:00:08  wsun
// Forgot to update KalmanCandidateQuality.cc to new TRTrackFitQuality.
//
// Revision 1.2  2000/12/02 01:33:14  wsun
// Added moreBadHits and pullOfNextWorstHit to KalmanCandidateQuality.
//
// Revision 1.1  2000/08/16 20:08:28  wsun
// First submission.
//
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "KalmanFilter/KalmanCandidateQuality.h"

// STL classes

//
// constants, enums and typedefs
//

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: KalmanCandidateQuality.cc,v 1.5 2001/01/05 05:33:13 lyon Exp $";
static const char* const kTagString = "$Name: v12_04_00 $";

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
KalmanCandidateQuality< H >::KalmanCandidateQuality(
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
   float energyLoss,
   float arcSwum,
   DABoolean moreBadHits,
   float pullOfNextWorstHit,
   TRSubdetectorLists::Detectors detector,
   const STL_VECTOR( int )& numberHitsVec,
   const STL_VECTOR( double )& averageResiduals,
   const STL_VECTOR( double )& rmsResiduals,
   const TRLayerMap& layerMap)
   : TRTrackCandidateFitQuality< H >( identifier,
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
				      layerMap),
   m_energyLoss(energyLoss ) ,
   m_arcSwum(arcSwum ),
   m_moreBadHits(moreBadHits ),
   m_pullOfNextWorstHit(pullOfNextWorstHit )
{
}
#endif /* TYPEDEFS_IN_TEMPLATES_IN_ARGS_BUG */

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
KalmanCandidateQuality< H >::~KalmanCandidateQuality()
{
}

#endif /* KALMANFILTER_KalmanCandidateQuality_CC */
