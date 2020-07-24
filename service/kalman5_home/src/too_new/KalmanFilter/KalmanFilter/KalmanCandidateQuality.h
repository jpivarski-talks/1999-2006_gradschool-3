#if !defined(KALMANFILTER_KALMANCANDIDATEQUALITY_H)
#define KALMANFILTER_KALMANCANDIDATEQUALITY_H
// -*- C++ -*-
//
// Package:     KalmanFilter
// Module:      KalmanCandidateQuality
// 
// Description: templated quality object derived from
//              TRTrackCandidateFitQuality< H >
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Wed May 12 00:04:50 EDT 1999
// $Id: KalmanCandidateQuality.h,v 1.6 2001/01/05 05:33:08 lyon Exp $
//
// Revision history
//
// $Log: KalmanCandidateQuality.h,v $
// Revision 1.6  2001/01/05 05:33:08  lyon
// Added LayerMap to quality object
//
// Revision 1.5  2000/12/22 07:02:27  wsun
// Updated to new TRTrackFitQuality.
//
// Revision 1.4  2000/12/14 16:19:10  wsun
// Added #hits, avg residuals, and rms residuals by subdetector to fit quality.
//
// Revision 1.3  2000/12/02 01:33:07  wsun
// Added moreBadHits and pullOfNextWorstHit to KalmanCandidateQuality.
//
// Revision 1.2  2000/08/16 20:13:50  wsun
// Updated to new TRSeedTrackQuality.
//
// Revision 1.1  1999/05/14 01:11:01  wsun
// First submission.
//
//
//

// system include files

// user include files
#include "CleoDB/DBCandidate.h" // for Hypo
#include "TrackRoot/TRTrackCandidateFitQuality.h" // superclass

// forward declarations

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
class KalmanCandidateQuality : public TRTrackCandidateFitQuality< H >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef typename TRTrackCandidateFitQuality< H >::Identifier Identifier;

      // Constructors and destructor

#if defined(TYPEDEFS_IN_TEMPLATES_IN_ARGS_BUG)
      KalmanCandidateQuality( Identifier identifier,
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
			      const TRLayerMap& layerMap )
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
	 m_energyLoss(                     energyLoss ) ,
	 m_arcSwum(                        arcSwum ),
	 m_moreBadHits(                    moreBadHits ),
	 m_pullOfNextWorstHit(             pullOfNextWorstHit )
{
}
#else
      KalmanCandidateQuality( Identifier identifier,
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
			      const TRLayerMap& layerMap) ;
#endif /* TYPEDEFS_IN_TEMPLATE_IN_ARGS_BUG */

      virtual ~KalmanCandidateQuality() ;

      // member functions

      // const member functions
      float energyLoss( void ) const { return m_energyLoss; }
      float arcSwum(    void ) const { return m_arcSwum; }
      DABoolean moreBadHits( void ) const { return m_moreBadHits ; }
      float pullOfNextWorstHit( void ) const { return m_pullOfNextWorstHit ; }

      // static member functions

   protected:
      // protected member functions

      // protected const member functions

   private:
      // Constructors and destructor
      KalmanCandidateQuality();

      // assignment operator(s)
      const KalmanCandidateQuality& operator=( const KalmanCandidateQuality& );

      // private member functions

      // private const member functions

      // data members
      float m_energyLoss;
      float m_arcSwum;
      DABoolean m_moreBadHits ;
      float m_pullOfNextWorstHit ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "KalmanFilter/Template/KalmanCandidateQuality.cc"
//#endif

#endif /* KALMANFILTER_KALMANCANDIDATEQUALITY_H */
