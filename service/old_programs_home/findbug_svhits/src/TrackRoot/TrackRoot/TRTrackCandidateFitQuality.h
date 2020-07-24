#if !defined(TRACKROOT_TRTRACKCANDIDATEFITQUALITY_H)
#define TRACKROOT_TRTRACKCANDIDATEFITQUALITY_H
// -*- C++ -*-
//
// Package:     TrackRoot
// Module:      TRTrackCandidateFitQuality
// 
/**\class TRTrackCandidateFitQuality TRTrackCandidateFitQuality.h TrackRoot/TRTrackCandidateFitQuality.h

 Description: Templated wrapper for TRTrackFitQuality to distiguish hypotheses.

 Usage:
    This IS-A TRTrackFitQuality, which in turn IS-A TRTrackQuality.  It adds
    nothing to the interface, just type-separation for proxies.  Users
    should never see this class, since the base classes are all that is
    of interest.

    The constructor is protected, so only derived types can be
    instantiated.

*/
//
// Author:      Michael Marsh
// Created:     Fri Dec 18 10:17:56 EST 1998
// $Id: TRTrackCandidateFitQuality.h,v 1.7 2001/01/05 05:27:36 lyon Exp $
//
// Revision history
//
// $Log: TRTrackCandidateFitQuality.h,v $
// Revision 1.7  2001/01/05 05:27:36  lyon
// Added LayerMap and other changes to quality objects
//
// Revision 1.6  2000/12/22 06:56:12  wsun
// New hit and residual functions that can be used with CLEO II, II.V, or III.
//
// Revision 1.5  2000/12/14 16:01:25  wsun
// Added #hits, average residuals, and rms residuals by subdetector.
//
// Revision 1.4  2000/08/01 23:28:43  wsun
// Added data members for number of hits dropped and number of fit iterations.
//
// Revision 1.3  2000/02/25 01:09:45  wsun
// Added copy/default ctors and comparison operators for storage.
//
// Revision 1.2  1999/10/07 16:33:51  marsh
// Added doxygen-style comments.
//
// Revision 1.1  1998/12/18 19:37:27  marsh
// Added templated fit track quality object to provide type-differentiation
// for Frame storage.
//

// system include files

// user include files
#include "TrackRoot/TRTrackFitQuality.h" // superclass
#include "CleoDB/DBCandidate.h" // for Hypo

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
class TRTrackCandidateFitQuality : public TRTrackFitQuality
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      typedef typename TRTrackFitQuality::Identifier Identifier;

      // ---------- Constructors and destructor ----------------
#if defined(TYPEDEFS_IN_TEMPLATES_IN_ARGS_BUG)
      TRTrackCandidateFitQuality( Identifier identifier,
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
#else
      TRTrackCandidateFitQuality( Identifier identifier,
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
				  const TRLayerMap& layerMap );
#endif /* TYPEDEFS_IN_TEMPLATE_IN_ARGS_BUG */

      virtual ~TRTrackCandidateFitQuality();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- constructor --------------------------------

      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      // stop defaults -- defaults okay, needed for storage helpers.
      // TRTrackCandidateFitQuality();
      // TRTrackCandidateFitQuality( const TRTrackCandidateFitQuality& );

      // ---------- assignment operator(s) ---------------------
      const TRTrackCandidateFitQuality& operator=(
         const TRTrackCandidateFitQuality& );

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
# include "TrackRoot/Template/TRTrackCandidateFitQuality.cc"
#endif

#endif /* TRACKROOT_TRTRACKCANDIDATEFITQUALITY_H */
