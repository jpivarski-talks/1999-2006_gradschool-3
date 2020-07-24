#if !defined(CHISQFITPRODUCER_CHISQCANDIDATEQUALITY_H)
#define CHISQFITPRODUCER_CHISQCANDIDATEQUALITY_H
// -*- C++ -*-
//
// Package:     ChisqFitProducer
// Module:      ChisqCandidateQuality
// 
// Description: templated quality object derived from
//              TRTrackCandidateFitQuality< H >
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Wed May 12 00:04:50 EDT 1999
// $Id: ChisqCandidateQuality.h,v 1.5 2001/01/05 05:30:55 lyon Exp $
//
// Revision history
//
// $Log: ChisqCandidateQuality.h,v $
// Revision 1.5  2001/01/05 05:30:55  lyon
// Added TRLayerMap to constructor
//
// Revision 1.4  2000/12/22 07:03:12  wsun
// Updated to new TRTrackFitQuality.
//
// Revision 1.3  2000/12/14 20:23:55  wsun
// Added #hits, avg residuals, and rms residuals by subdetector to fit quality.
//
// Revision 1.2  2000/08/16 20:31:43  wsun
// Updated to new TRTrackFitQuality.
//
// Revision 1.1  1999/09/10 22:10:51  wsun
// Transfered from ChisqFitProducer.
//
// Revision 1.2  1999/06/20 22:44:48  wsun
// Added proxies for all ten fit hypotheses.
//
// Revision 1.1.1.1  1999/06/16 02:48:25  wsun
// First submission.
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
class ChisqCandidateQuality : public TRTrackCandidateFitQuality< H >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef typename
         TRTrackCandidateFitQuality< H >::Identifier
         Identifier;

      // Constructors and destructor
      ChisqCandidateQuality( Identifier identifier,
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
			     const TRLayerMap& layerMap) :
         TRTrackCandidateFitQuality< H >(
	    identifier,
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
	    layerMap) {}

      ChisqCandidateQuality( const ChisqCandidateQuality< H >& aQuality ) :
         TRTrackCandidateFitQuality< H >(
	    aQuality.identifier(),
	    aQuality.chiSquare(),
	    aQuality.degreesOfFreedom(),
	    aQuality.numberHits(),
	    aQuality.numberHitsExpected(),
	    aQuality.numberHitsInput(),
	    aQuality.numberHitsDropped(),
	    aQuality.numberIterations(),
	    aQuality.fit(),
	    aQuality.fitAbort(),
	    aQuality.detector(),
	    aQuality.numberHitsVector(),
	    aQuality.averageResidualsVector(),
	    aQuality.rmsResidualsVector(),
	    aQuality.layerMap()
	    ) {}

      virtual ~ChisqCandidateQuality() {}

      // member functions

      // const member functions

      // static member functions

   protected:
      // protected member functions

      // protected const member functions

   private:
      // Constructors and destructor
      ChisqCandidateQuality();

      // assignment operator(s)
      const ChisqCandidateQuality& operator=( const ChisqCandidateQuality& );

      // private member functions

      // private const member functions

      // data members

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "ChisqFitProducer/Template/ChisqCandidateQuality.cc"
//#endif

#endif /* CHISQFITPRODUCER_CHISQCANDIDATEQUALITY_H */
