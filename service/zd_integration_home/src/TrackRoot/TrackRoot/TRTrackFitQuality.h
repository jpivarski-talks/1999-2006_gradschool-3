#if !defined(TRACKROOT_TRTRACKFITQUALITY_H)
#define TRACKROOT_TRTRACKFITQUALITY_H
// -*- C++ -*-
//
// Package:     TrackRoot
// Module:      TRTrackFitQuality
// 
/**\class TRTrackFitQuality TRTrackFitQuality.h TrackRoot/TRTrackFitQuality.h

 Description: Track quality object for fit tracks

 Usage:
    This object stores the basic information for the quality of tracks
    coming out of a track fitter.  Specific fitters will create their
    own quality objects by inheriting this.

    The constructor is protected, so only derived types can be
    instantiated.

    zEndpointUsed() and originUsed() come from the seed track, and
    are included because they are still relevant pieces of information
    for the fit track.

*/
//
// Author:      Michael Marsh
// Created:     Tue Nov 10 09:51:12 EST 1998
// $Id: TRTrackFitQuality.h,v 1.14 2001/03/06 21:01:29 wsun Exp $
//
// Revision history
//
// $Log: TRTrackFitQuality.h,v $
// Revision 1.14  2001/03/06 21:01:29  wsun
// Fixed ratioNumberHitsToExpected(): use number hit LAYERS on track.
//
// Revision 1.13  2001/01/05 05:27:37  lyon
// Added LayerMap and other changes to quality objects
//
// Revision 1.12  2000/12/22 06:56:12  wsun
// New hit and residual functions that can be used with CLEO II, II.V, or III.
//
// Revision 1.11  2000/12/14 16:01:26  wsun
// Added #hits, average residuals, and rms residuals by subdetector.
//
// Revision 1.10  2000/08/06 00:42:14  wsun
// Wrong return type for numberHitsDropped() and numberIterations().
//
// Revision 1.9  2000/08/01 23:26:49  wsun
// TrackRoot/TRTrackCandidateFitQuality.h
//
// Revision 1.8  2000/04/05 15:38:03  marsh
// Let the compiler make the destructor.
//
// Revision 1.7  2000/02/25 01:09:45  wsun
// Added copy/default ctors and comparison operators for storage.
//
// Revision 1.6  1999/10/23 17:43:27  cdj
// fixed improper use of typename
//
// Revision 1.5  1999/10/07 16:33:52  marsh
// Added doxygen-style comments.
//
// Revision 1.4  1998/11/20 15:42:00  marsh
// New pieces of information added, including hit counting and additional
// boolean flags for "z-escapes".
//
// Revision 1.3  1998/11/15 16:58:05  marsh
// Added success() to base quality classes.  Made classes concrete, but
// with protected constructors to limit instances to derived classes.
//
// Revision 1.2  1998/11/12 16:06:37  marsh
// Stop default empty ctor().
//
// Revision 1.1  1998/11/12 15:07:53  marsh
// New track quality object.
//

// system include files

// user include files
#include "TrackRoot/TRTrackQuality.h" // superclass
#include "TrackRoot/TRSubdetectorLists.h"
#include "TrackRoot/TRLayerMap.h"

// forward declarations
#include "STLUtility/fwd_vector.h"
#include "C++Std/fwd_ostream.h"

class TRTrackFitQuality : public TRTrackQuality
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      typedef TRTrackQuality::Identifier Identifier;

      // ---------- Constructors and destructor ----------------

      TRTrackFitQuality( Identifier identifier,
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
			 const TRLayerMap& layerMap
			 ) ;

      virtual ~TRTrackFitQuality() ;

      DABoolean operator==( const TRTrackFitQuality& rhs ) const ;
      DABoolean operator!=( const TRTrackFitQuality& rhs ) const
      {
	 return !( *this == rhs ) ;
      }

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------
      double chiSquare() const {
         return m_chiSquare;
      }
      int degreesOfFreedom() const {
         return m_degreesOfFreedom;
      }

      /**
       * \return number of hit LAYERS expected given curvature and
       * dip angle of track
       */
      int numberHitsExpected() const {
         return m_numberHitsExpected;
      }


      /**
       * \Return the ratio of number of hit layers in the fit to the number
       * of hit layers expected (this does the conversion to float for you).
       */
      float ratioNumberHitsToExpected() const {

	 // Must convert to float to avoid integer division (and always
	 // returning zero!)
//	return ((float)numberHits()) / ((float)m_numberHitsExpected);

	 // numberOfHitsExpected is really number of hit layers expected.
	 return (
	    (float)numberSubdetectorHitLayers(
	       TRSubdetectorLists::kEntireDetector) ) /
	    ((float)m_numberHitsExpected);
      }

      /**
       * \return number of hits coming in from the seed track.
       */
      unsigned int numberHitsInput() const {
	return m_numberHitsInput;
      }

      /**
       * \return true if the track was passed to the fitter.
       * Used to flag mocked-up fits that are really seed tracks.
       */
      DABoolean fit() const {
         return m_fit;
      }

      /**
       * \return true if the track failed in the fitter.
       */
      DABoolean fitAbort() const {
         return m_fitAbort;
      }

      /// Number of hits that were dropped by the fitter
      unsigned int numberHitsDropped() const {
	return m_numberHitsDropped;
      }

  
      /// Number of iterations the fitter went through
      unsigned int numberIterations() const {
	return m_numberIterations;
      }

      /// detector code (used internally)
      TRSubdetectorLists::Detectors detector() const {
	 return m_detector ;
      }

      /// Number of hits that occured in a particular subdetector
      ///   -- note multiple hits on layers ARE counted ---
      unsigned int numberSubdetectorHits( 
			    TRSubdetectorLists::HitTypes aHitType ) const ;

      /// Number of layers in subdetector that were hit
      ///  --- note multiple hits are counted as one hit ---
      unsigned int numberSubdetectorHitLayers(
			   TRSubdetectorLists::HitTypes aHitType) const;

      /// Average residual for hits in a subdetector
      double averageResidual( TRSubdetectorLists::HitTypes aHitType ) const ;

      /// RMS residual for hits in a subdetector
      double rmsResidual( TRSubdetectorLists::HitTypes aHitType ) const ;

      /// Return true if a particular layer in a subdetector was hit
      ///    (layer# always starts at 1)
      DABoolean isLayerHit( TRSubdetectorLists::HitTypes aHitType, 
			    unsigned int layerNum ) const;
    
      // Report a flag 
      void reportFlag(ostream& s, const char* text, 
		      DABoolean flag) const;

      /// Print out quality information nicely   
      void printout(ostream& s) const;


      // For storage helper.
      const STL_VECTOR( int ) numberHitsVector() const ;
      const STL_VECTOR( double ) averageResidualsVector() const ;
      const STL_VECTOR( double ) rmsResidualsVector() const ;

//      const int* beginNumberHits() const { return m_numberHitsArray; };
//      const int* endNumberHits() const { 
//         return m_numberHitsArray+sizeof(m_numberHitsArray)/sizeof(int); };

//      const double* beginAverageResiduals() const { 
//         return m_averageResudials; };
//      const double* endAverageResiduals() const {
//         return m_averageResiduals+sizeof(m_averageResiduals)/sizeof(double); };

//      const double* beginRmsResiduals() const { 
//        return m_rmsResudials; };
//      const double* endRmsResiduals() const {
//        return m_rmsResiduals+sizeof(m_rmsResiduals)/sizeof(double); };

      const TRLayerMap& layerMap() const;

      // ---------- static member functions --------------------

   protected:
      // ---------- constructor --------------------------------

      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      // defaults okay -- needed for storage helpers.
      // TRTrackFitQuality();
      // TRTrackFitQuality( const TRTrackFitQuality& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const TRTrackFitQuality& operator=( const TRTrackFitQuality& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      double    m_chiSquare;
      int       m_degreesOfFreedom;
      int       m_numberHitsExpected;
      int       m_numberHitsInput;
      DABoolean m_fit;
      DABoolean m_fitAbort;

      int m_numberHitsDropped ;
      int m_numberIterations ;

      TRSubdetectorLists::Detectors m_detector ;
      int m_numberHitsArray[ TRSubdetectorLists::kNumberOfHitTypes ] ;
      double m_averageResiduals[ TRSubdetectorLists::kNumberOfHitTypes ] ;
      double m_rmsResiduals[ TRSubdetectorLists::kNumberOfHitTypes ] ; 
      TRLayerMap m_layerMap;


      // ---------- static data members ------------------------

};

// inline function definitions

#endif /* TRACKROOT_TRTRACKFITQUALITY_H */
