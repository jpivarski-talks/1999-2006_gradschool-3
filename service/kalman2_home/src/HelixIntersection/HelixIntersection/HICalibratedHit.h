#if !defined(HELIXINTERSECTINO_HICALIBRATEDHIT_H)
#define HELIXINTERSECTINO_HICALIBRATEDHIT_H
// -*- C++ -*-
//
// Package:     <HelixIntersectino>
// Module:      HICalibratedHit
// 
/**\class HICalibratedHit HICalibratedHit.h HelixIntersectino/HICalibratedHit.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Werner Sun
// Created:     Sun Jan 30 16:15:02 EST 2000
// $Id: HICalibratedHit.h,v 1.8 2002/04/27 17:12:23 cleo3 Exp $
//
// Revision history
//
// $Log: HICalibratedHit.h,v $
// Revision 1.8  2002/04/27 17:12:23  cleo3
// Have the compiler write the copy constructor and assignment operator. Inlined the destructor
//
// Revision 1.7  2002/02/17 17:21:41  wsun
// Don't erase track corrections in clear().
//
// Revision 1.6  2000/10/11 05:54:08  wsun
// Added data members for corrected fitting weight.
//
// Revision 1.5  2000/10/08 23:38:19  wsun
// Added dca smoothing.
//
// Revision 1.4  2000/06/09 23:53:17  wsun
// * Added applyTrackCorrections().
// * TrackFitHitLinks stored temporarily by HICalibratedHits to avoid new-ing
//   them every time the track is copied during fitting.  The TrackFitHitLinks
//   are copied to the helix after fitting is done.  Saves A LOT of time.
// * Renamed makeNewLinkData() -->
//   instantiateLinkData() and makeNewCopyOfLinkData().
//
// Revision 1.3  2000/06/09 00:29:34  wsun
// Added makeNewLinkData().
//
// Revision 1.2  2000/02/05 21:39:07  wsun
// Moved sqrt from .h to .cc file to avoid cleo_math.h dependency.
//
// Revision 1.1  2000/02/03 07:12:37  wsun
// First submission.
//

// system include files

// user include files

// forward declarations
class CalibratedHit ;
class TrackFitHitLink;
class HIFitHelix ;
class HIHelix ;
class HIIntersectionSurface ;

class HICalibratedHit
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      HICalibratedHit( DABoolean aUseInFit = true,
		       DABoolean aInfoOnTrack = false ) ;

      //HICalibratedHit( const HICalibratedHit& rhs ) ;

      //const HICalibratedHit& operator=( const HICalibratedHit& rhs ) ;

      virtual ~HICalibratedHit() {}

      // ---------- member functions ---------------------------
      void setUseInFit( DABoolean aFlag ) { m_useInFit = aFlag ; }
      void setInfoOnTrack( DABoolean aFlag ) { m_infoOnTrack = aFlag ; }
      void addResidualAndErrorSquared( const Meters& aResidual,
				       const Meters& aErrorSquared,
				       const Meters& aDistPred,
				       const Meters& aDistPredErrsq ) ;

      // clear() does not undo the track corrections.  It just clears
      // the residuals and smoothed quantities.
      virtual void clear() ;

      // Fills the data members m_smoothedResidual and m_smoothedErrsq.
      void calculateSmoothedResidualAndError() ;

      // This function instantiates a TrackFitHitLink and adds it to an
      // internal vector.  This link data does not need to be deleted.
      virtual TrackFitHitLink* instantiateLinkData(
	 const HIFitHelix& aHelix,
	 const HIIntersectionSurface& aSurface,
	 Meters aResidual,
	 Meters aResidualError,
	 Meters aDistPred,
	 Meters aDistPredError )
      { return 0 ; }

      // WARNING: this function does not check the type of the input
      // TrackFitHitLink before casting it to a subclass.  This link data
      // is owned by the caller and needs to be deleted.
      virtual TrackFitHitLink* makeNewCopyOfLinkData(
	 const TrackFitHitLink* aLinkData )
      { return 0 ; }

      // Apply track- and mass-dependent corrections, like entrance angle
      // corrections for Si and cathodes, signal propagation and time of
      // flight for DR anodes.  Fitting weight is also corrected.
      virtual void applyTrackCorrections( const HIHelix& aHelix ) {}

      // ---------- const member functions ---------------------
      virtual double measurementErrorSquared() const = 0 ;
      virtual const CalibratedHit* calibratedHit() const = 0 ;

      DABoolean residualsSmoothed() const { return m_residualsSmoothed ; }
      Meters smoothedResidual() const { return m_smoothedResidual ; }
      Meters smoothedResidualError() const ;
      Meters smoothedDistPred() const { return m_smoothedDistPred ; }
      Meters smoothedDistPredError() const ;

      // This function returns false if the measurement is zero (e.g. for
      // cathodes and SVX) or if flipping the sign of the measurement
      // increases the smoothed residual.
      virtual DABoolean reverseMeasurementSign() { return false ; }

      int numberOfResiduals() const { return m_nResiduals ; }

      // useInFit  && !infoOnTrack = hit should be added but has not been yet.
      // useInFit  && infoOnTrack  = hit has been added to the fit.
      // !useInFit && !infoOnTrack = hit should NOT be added, and hit/track
      //                              link data has not been added yet.
      // !useInFit && infoOnTrack  = hit was NOT added to the fit, but
      //                              hit/track link data has been added.
      DABoolean useInFit() const { return m_useInFit ; }
      DABoolean infoOnTrack() const { return m_infoOnTrack ; }

      DABoolean fittingWeightCorrected() const
      { return m_fittingWeightCorrected ; }
      double correctedFittingWeight() const
      { return m_correctedFittingWeight ; }

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------
      Meters m_smoothedResidual ;
      Meters m_smoothedErrsq ;
      Meters m_smoothedDistPred ;
      Meters m_smoothedDistPredErrsq ;

      DABoolean m_residualsSmoothed ;

      DABoolean m_fittingWeightCorrected ;
      double m_correctedFittingWeight ;

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------

      // ---------- assignment operator(s) ---------------------

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      DABoolean m_useInFit ;
      DABoolean m_infoOnTrack ;

      Meters m_residuals[2] ;
      Meters m_errsqs[2] ;
      Meters m_distPreds[2] ;
      Meters m_distPredErrsqs[2] ;

      int m_nResiduals ;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HelixIntersectino/Template/HICalibratedHit.cc"
//#endif

#endif /* HELIXINTERSECTINO_HICALIBRATEDHIT_H */
