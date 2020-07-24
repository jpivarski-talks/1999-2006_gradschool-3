#if !defined(KALMANFILTER_KALMANTRACKFITTER_H)
#define KALMANFILTER_KALMANTRACKFITTER_H
// -*- C++ -*-
//
// Package:     KalmanFilter
// Module:      KalmanTrackFitter
// 
// Description: Subclass of TrackFitter for the C++ Kalman filter.
//
// Usage:
//
//    The width of the Gaussian straggling is given by:
//
//        d( deltaE ) = coeff^2 * deltaE
//
//    where the coefficient is set by the user in the constructor.
//
//
// Author:      Werner Sun
// Created:     Tue Sep 29 16:59:51 EDT 1998
// $Id: KalmanTrackFitter.h,v 1.30 2002/02/23 06:47:24 wsun Exp $
//
// Revision history
//
// $Log: KalmanTrackFitter.h,v $
// Revision 1.30  2002/02/23 06:47:24  wsun
// Added flag for applying B field corrections.
//
// Revision 1.29  2002/02/22 03:33:12  wsun
// Change m_outwardFit to m_inwardFit.
//
// Revision 1.28  2001/03/27 06:39:41  wsun
// Mods for hit addition and removal.
//
// Revision 1.27  2000/12/22 07:01:56  wsun
// Fixed handling of additional hits added to fit.
//
// Revision 1.26  2000/12/13 07:02:41  wsun
// Added option to not have inward fits propagated to the origin.
//
// Revision 1.25  2000/11/18 00:16:48  wsun
// Allow cathode fitting weight to be scaled by user-defined parameter.
//
// Revision 1.24  2000/09/05 20:45:23  wsun
// Mods to allow user to force fitting weight to a given value.
//
// Revision 1.23  2000/08/16 20:13:13  wsun
// Count number of discarded and flipped hits.
//
// Revision 1.22  2000/07/25 21:31:51  wsun
// User can specify usage and and production tags for hits.
//
// Revision 1.21  2000/06/10 06:02:42  wsun
// Added parameters to dis/enable corrections to DR, SVR, SVZ, DRCA hits.
//
// Revision 1.20  2000/06/09 00:50:59  wsun
// * Allow hit list to be set by one mass hypothesis.
// * Allow selection of hypotheses to be fit.
//
// Revision 1.19  2000/06/06 22:22:32  wsun
// Added flag to enable DR hit correction.
//
// Revision 1.18  2000/03/02 08:38:27  wsun
// Overloaded TrackFitter::event() functions.
//
// Revision 1.17  2000/02/26 22:28:51  cdj
// added accessors for KalmanTrackFitter running options
//
// Revision 1.16  2000/02/03 18:49:18  wsun
// Use smoothed residuals to discard hits or flip drift sign.
//
// Revision 1.15  2000/01/11 20:35:56  wsun
// Added flags to control exception handling.
//
// Revision 1.14  1999/11/21 22:36:41  wsun
// Fixed bug in surface merging.
//
// Revision 1.13  1999/10/08 23:16:25  wsun
// * Added individual surface caches for Si, IR, DR.
// * Added surface cache for hit surfaces.
// * Allow outward fits to be used as seeds for the inward fits.
//
// Revision 1.12  1999/10/05 23:02:43  wsun
// Added functions for user to specify hits to fit.
//
// Revision 1.11  1999/08/30 21:53:12  wsun
// Added flags for dE/dx straggling and surfaces from HIIRSurfaceFactory.
//
// Revision 1.10  1999/08/27 20:42:26  wsun
// Delay track parameter update for Si hits.
//
// Revision 1.9  1999/08/03 22:29:22  wsun
// Cleaned up code.
//
// Revision 1.8  1999/07/07 16:46:28  wsun
// Added mechanism for applying constraints in track fitter.
//
// Revision 1.7  1999/06/30 23:51:43  wsun
// Added cache of Si surfaces needed for every event.
//
// Revision 1.6  1999/06/18 19:27:38  wsun
// More improvements to low momentum fitting.
//
// Revision 1.5  1999/05/28 22:01:40  wsun
// Use outward fit result as seed for inward fit.
//
// Revision 1.4  1999/05/14 01:11:27  wsun
// Updated for use with KalmanFilter producer.
//
// Revision 1.3  1999/04/27 22:19:53  wsun
// Major reorganization and structural changes.
//
// Revision 1.2  1999/01/30 01:26:37  wsun
// Implement new virtual TrackFitter::fitTracks().
//
// Revision 1.1.1.1  1998/12/07 04:59:00  wsun
// First submission.
//

// system include files

// user include files
#include "TrackFitter/TrackFitter.h"
#include "KalmanFilter/KalmanHelix.h"
#include "HelixIntersection/HIDRSurfaceFactory.h"
#include "DataHandler/DataKeyTags.h"

// forward declarations
#include "STLUtility/fwd_vector.h"

class HIHelixIntersector ;
class HIIntersectionSurface ;
class HIHelixOperation ;
class HICompositeOperation ;
class CalibratedDRHit ;
class CalibratedCathodeHit ;
class CalibratedSVRphiHit ;
class CalibratedSVZHit ;
class HICondition ;
class HIFitConstraint ;
class HITrackParameterConstraint ;
class HICompositeFitConstraint ;
class HIMSDedxOperation ;
class KalmanAddHitOperation ;
class KalmanUpdateParamsOperation ;
class KalmanUpdateParamsCondition ;

class KalmanTrackFitter : public TrackFitter
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      enum InputTrackInfo { kNoHitsOnInputTrack,
			    kAddHitsToInputTrackInside,
			    kAddHitsToInputTrackOutside } ;

      // ---------- Constructors and destructor ----------------
      KalmanTrackFitter( HIFitConstraint* aConstraint = 0,
			 InputTrackInfo aTrackInfo = kNoHitsOnInputTrack ) ;
      virtual ~KalmanTrackFitter();

      // ---------- member functions ---------------------------

      // This function is called by event(), and, in turn, calls fittedTrack().
      // The new fitted track is kept in the TrackFitter's cache, which is
      // emptied by clearCache().
      virtual DABoolean fitTrack(
         const TRSeedTrack* theSeed,
         Frame& theFrame,
         DBCandidate::Hypo aHypo = DBCandidate::kChargedPion ) ;

      // The following four functions return a new KTHelix which the user
      // (or the Frame) is responsible for deleting.  The returned KTHelix will
      // hold the fitted track parameters, and the reference point will be
      // the current reference point of the seed.  The fitted helix is NOT
      // inserted into TrackFitter's internal vector.
      //
      // If the constraint pointer is non-null, it will be applied to the
      // helix parameters after the unconstrained fit has been performed
      // successfully.  The HIFitConstraint object is modified in the process.
      virtual KTHelix* fittedTrack(
	 const TRSeedTrack* theSeed,
	 Frame& theFrame,
         DBCandidate::Hypo aHypo = DBCandidate::kChargedPion ) ;

      virtual KTHelix* fittedTrack(
	 const TRSeedTrack* theSeed,
	 Frame& theFrame,
	 GeV aMass = DBCandidate::mass( DBCandidate::kChargedPion ),
	 FitDirection aFitDirection = kInwardFit ) ;

      // FITTING NON-DEFAULT HITS:
      // -------------------------
      // If the user supplies the hits by hand, the fitter will ignore
      // the ones attached to the seeds (via Lattice).
      //
      // If a hit vector pointer is null (default), then if
      // aExtractHitsIfNull is:
      //   * true, the Lattice-connected hits will be used.
      //   * false, then no hits of that type will be included in the fit.

      // ADDING HITS TO A PREVIOUS FIT:
      // ------------------------------
      // If hits are to be added to the track, the returned helix will be at
      // the edge of the track: inward fit if hits are added to the inside,
      // outward fit if hits are added to the outside.  The other fit will
      // be deleted.
      //
      // If hits are to be added to the input track, they must extend the
      // the track inward or outward.  Hits cannot be added in the middle
      // of the track -- all these hits would have to be refit.
      //
      // The track given to fittedTrack(...) should have a reference point
      // at the last hit just before the new hits to be added.  I.e. if hits
      // are to be added to the inner edge, the input track should be at the
      // innermost hit from the previous fit.
      //
      // IMPORTANT:
      //    * Use setInputTrackInfo() to tell the fitter which side
      //      to add the hits to.
      //    * The user is responsible for deleting the returned helix.

      virtual KTHelix* fittedTrack(
         const TRSeedTrack* theSeed,
         Frame& theFrame,
         const STL_VECTOR(
            HIDRSurfaceFactory::DRHitAndDriftDistance )* aDRVector = 0,
         const STL_VECTOR( const CalibratedCathodeHit* )* aCathodeVector = 0,
         const STL_VECTOR( const CalibratedSVRphiHit* )* aRphiVector = 0,
         const STL_VECTOR( const CalibratedSVZHit* )* aZVector = 0,
         DBCandidate::Hypo aHypo = DBCandidate::kChargedPion,
         DABoolean aExtractHitsIfNull = true ) ;

      virtual KTHelix* fittedTrack(
         const TRSeedTrack* theSeed,
         Frame& theFrame,
         const STL_VECTOR(
            HIDRSurfaceFactory::DRHitAndDriftDistance )* aDRVector = 0,
         const STL_VECTOR( const CalibratedCathodeHit* )* aCathodeVector = 0,
         const STL_VECTOR( const CalibratedSVRphiHit* )* aRphiVector = 0,
         const STL_VECTOR( const CalibratedSVZHit* )* aZVector = 0,
         GeV aMass = DBCandidate::mass( DBCandidate::kChargedPion ),
         DABoolean aExtractHitsIfNull = true ) ;

      // REMOVING HITS FROM A PREVIOUS FIT:
      // ----------------------------------
      // Hits are removed by adding the hits to the track with negative
      // fitting weight.  Energy loss and multiple scattering operations
      // are performed in the reverse order as in the original fit.
      //
      // (Anti-)fits are performed in one direction only, keeping the
      // original error matrix.  This direction should be given by the user
      // as kOutwardFit for hits removed from the inside edge and kInwardFit
      // for hits removed from the outside edge.
      //
      // Fit results are given at the last hit.  The user can then use
      // the KalmanHelixTransporter to move the track to the desired location.
      // If the fit fails, returned pointer is null.  In this case, the entire
      // track should be refit (use fittedTrack() above).

      virtual KTHelix* removeHitsFromFit(
	 FitDirection aDirection,
         const TRSeedTrack* theSeed,
         Frame& theFrame,
         const STL_VECTOR(
            HIDRSurfaceFactory::DRHitAndDriftDistance )* aDRVector = 0,
         const STL_VECTOR( const CalibratedCathodeHit* )* aCathodeVector = 0,
         const STL_VECTOR( const CalibratedSVRphiHit* )* aRphiVector = 0,
         const STL_VECTOR( const CalibratedSVZHit* )* aZVector = 0,
         GeV aMass = DBCandidate::mass( DBCandidate::kChargedPion ) ) ;

      virtual TDKinematicFit* hypothesis( DBCandidate::Hypo hypo ) const
      { return 0 ; }


      void setPrintDiagnostics( DABoolean flag ) { m_printDiagnostics = flag;}

      void setUseDRAxialHits( DABoolean flag )   { m_useDRAxialHits = flag; }
      void setUseDRStereoUHits( DABoolean flag ) { m_useDRStereoUHits = flag;}
      void setUseDRStereoVHits( DABoolean flag ) { m_useDRStereoVHits = flag;}
      void setCorrectDRHits( DABoolean flag )    { m_correctDRHits = flag ; }

      void setUseDRCathodeHits( DABoolean flag ) { m_useDRCathodeHits = flag;}
      void setCorrectDRCathodeHits( DABoolean flag )
      { m_correctDRCathodeHits = flag ; }

      void setUseSVRPhiHits( DABoolean flag )    { m_useSVRPhiHits = flag ; }
      void setUseSVZHits( DABoolean flag )       { m_useSVZHits = flag ; }
      void setCorrectSVRHits( DABoolean flag )   { m_correctSVRHits = flag ; }
      void setCorrectSVZHits( DABoolean flag )   { m_correctSVZHits = flag ; }

      void setApplyDedx( DABoolean flag )        { m_applyDedx = flag ; }
      void setApplyStraggling( DABoolean flag )  { m_applyStraggling = flag; }
      void setStragglingCoeff( double aCoeff )   { m_stragglingCoeff = aCoeff;}
      void setApplyMultScat( DABoolean flag )    { m_applyMultScat = flag ; }
      void setApplyBfieldCorrections( DABoolean flag )
      { m_applyBfieldCorrections = flag ; }
      void setHitChisqCut( double aCut )         { m_hitChisqCut = aCut ; }
      void setMaxFractionalDeltaP( double aFrac ){ m_maxFracDP = aFrac ; }
      void setUpdateErr2Z0( double aErr )        { m_updateErr2Z0 = aErr ; }
      void setUpdateErr2CotTheta( double aErr )  { m_updateErr2CotTheta=aErr;}
      void setUpdateDFracCotTheta( double aFrac )
      { m_updateDFracCotTheta = aFrac ; }
      void setMaxHitsToDelete( int aMax ) { m_maxHitDel = aMax ; }
      void setMaxHitResidualPull( double aMaxPull ) { m_maxHitPull = aMaxPull;}
      void setKeepPreviousHitList( DABoolean flag )
      { m_keepPreviousHitList = flag ; }

      virtual void clearCache() ;

      void setInputTrackInfo( InputTrackInfo aInputTrackInfo )
      { m_inputTrackInfo = aInputTrackInfo ; }

      // Call this when changing parameters.
      void clearConditionsAndOperations() ;

      // Force the caches to be regenerated the next time
      // generateXXSurfaceCache() is called.
      void updateSiSurfaceCache() { m_siCacheFilled = false ; }
      void updateIRSurfaceCache() { m_irCacheFilled = false ; }
      void updateDRSurfaceCache() { m_drCacheFilled = false ; }

      // Use these functions to constrain a parameter to its initial value.
      void fixParameter( int aParam ) { m_fixParameter[ aParam ] = true ; }
      void fixCurvature()             { fixParameter( HIHelix::kCurvature ) ; }
      void fixPhi0()                  { fixParameter( HIHelix::kPhi0 ) ; }
      void fixD0()                    { fixParameter( HIHelix::kD0 ) ; }
      void fixCotTheta()              { fixParameter( HIHelix::kCotTheta ) ; }
      void fixZ0()                    { fixParameter( HIHelix::kZ0 ) ; }

      // Use these functions to release a constrained parameter.
      void releaseParameter( int aParam ) { m_fixParameter[ aParam ] = false; }
      void releaseCurvature()     { releaseParameter( HIHelix::kCurvature ) ; }
      void releasePhi0()          { releaseParameter( HIHelix::kPhi0 ) ; }
      void releaseD0()            { releaseParameter( HIHelix::kD0 ) ; }
      void releaseCotTheta()      { releaseParameter( HIHelix::kCotTheta ) ; }
      void releaseZ0()            { releaseParameter( HIHelix::kZ0 ) ; }

      // By default, calling fitTrack() with an inward hypothesis entails
      // doing the outward hypothesis as well.  So to speed things up, the
      // outward fit can be disabled by setting this flag to false.
      void setInwardFitIsOutwardSeed( DABoolean aFlag )
      { m_inwardFitIsOutwardSeed = aFlag ; }

      void setContinueIfHitsMissing( DABoolean aFlag )
      { m_continueIfHitsMissing = aFlag ; }
      void setContinueIfAnyException( DABoolean aFlag )
      { m_continueIfAnyException = aFlag ; }

      void setHyposToFit( STL_VECTOR( DBCandidate::Hypo )& aVector ) ;
      void setHitListFromOneHypo( DABoolean aFlag )
      { m_hitListFromOneHypo = aFlag ; }
      void setHitListHypo( DBCandidate::Hypo aHypo )
      { m_hitListHypo = aHypo ; }

      void setDRHitUsageTag( const string& aTag )
      { m_drHitUsageTag = UsageTag( aTag.c_str() ) ; }
      void setDRHitProductionTag( const string& aTag )
      { m_drHitProdTag = ProductionTag( aTag.c_str() ) ; }
      void setCathodeHitUsageTag( const string& aTag )
      { m_cathodeHitUsageTag = UsageTag( aTag.c_str() ) ; }
      void setCathodeHitProductionTag( const string& aTag )
      { m_cathodeHitProdTag = ProductionTag( aTag.c_str() ) ; }
      void setSVRHitUsageTag( const string& aTag )
      { m_svrHitUsageTag = UsageTag( aTag.c_str() ) ; }
      void setSVRHitProductionTag( const string& aTag )
      { m_svrHitProdTag = ProductionTag( aTag.c_str() ) ; }
      void setSVZHitUsageTag( const string& aTag )
      { m_svzHitUsageTag = UsageTag( aTag.c_str() ) ; }
      void setSVZHitProductionTag( const string& aTag )
      { m_svzHitProdTag = ProductionTag( aTag.c_str() ) ; }

      void setDRFittingWeight( double aWeight )
      { m_forceDRFittingWeight = true ; m_drFittingWeight = aWeight ; }
      void setSVRFittingWeight( double aWeight )
      { m_forceSVRFittingWeight = true ; m_svrFittingWeight = aWeight ; }
      void setSVZFittingWeight( double aWeight )
      { m_forceSVZFittingWeight = true ; m_svzFittingWeight = aWeight ; }
      void setCathodeFittingWeight( double aWeight )
      { m_forceCathodeFittingWeight = true ; m_cathodeFittingWeight=aWeight ; }
      void setCathodeFittingWeightScale( double aScale )
      {
	 m_scaleCathodeFittingWeight = true ;
	 m_cathodeFittingWeightScale = aScale ;
      }

      void setInwardFitsAtLastHit( DABoolean aFlag )
      { m_inwardFitsAtLastHit = aFlag ; }

      // ---------- const member functions ---------------------
      DABoolean event( Frame& theFrame ) const ;
      DABoolean event( Frame& theFrame, HypoType aHypo ) const ;

      STL_VECTOR( KalmanHelix* )* fittedHelices( HypoType aHypo ) const
      { return m_fittedHelices[ (int) aHypo ] ; }

      virtual DABoolean inwardFitIsOutwardSeed() const
      { return m_inwardFitIsOutwardSeed ; }

      DABoolean continueIfHitsMissing() const {
	 return m_continueIfHitsMissing; }

      DABoolean continueIfAnyException() const {
	 return m_continueIfAnyException; }
      
      DABoolean useDRAxialHits() const   { return  m_useDRAxialHits; }
      DABoolean useDRStereoUHits() const { return m_useDRStereoUHits; }
      DABoolean useDRStereoVHits() const { return m_useDRStereoVHits; }
      DABoolean correctDRHits() const    { return m_correctDRHits ; }
      DABoolean useDRCathodeHits() const { return m_useDRCathodeHits; }
      DABoolean correctDRCathodeHits() const{ return m_correctDRCathodeHits ; }
      DABoolean useSVRPhiHits() const    { return m_useSVRPhiHits; }
      DABoolean useSVZHits() const       { return m_useSVZHits; }
      DABoolean correctSVRHits() const    { return m_correctSVRHits ; }
      DABoolean correctSVZHits() const    { return m_correctSVZHits ; }

      DABoolean inwardFitsAtLastHit() const
      { return m_inwardFitsAtLastHit ; }

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      KalmanTrackFitter( const KalmanTrackFitter& ) ; // stop default

      // ---------- assignment operator(s) ---------------------
      const KalmanTrackFitter& operator=( const KalmanTrackFitter& );
      // stop default

      // ---------- private member functions -------------------
      void generateSiSurfaceCache( Frame& ) ;
      void generateIRSurfaceCache( Frame& ) ;
      void generateDRSurfaceCache( Frame& ) ;

      // Returns true if the Si surfaces are at the beginning of the list.
      void generateHitSurfaces(
	 const TRSeedTrack* theSeed,
         Frame& theFrame,
         const STL_VECTOR(
            HIDRSurfaceFactory::DRHitAndDriftDistance )* aDRVector = 0,
         const STL_VECTOR( const CalibratedCathodeHit* )* aCathodeVector = 0,
         const STL_VECTOR( const CalibratedSVRphiHit* )* aRphiVector = 0,
         const STL_VECTOR( const CalibratedSVZHit* )* aZVector = 0,
         GeV aMass = 0.13957,
         DABoolean aExtractHitsIfNull = true ) ;

      // Generates surfaces for entire detector, including hit surfaces
      // for a given track.  Give null pointer for first argument if no
      // track.
      STL_VECTOR( HIIntersectionSurface* ) generateSurfaces(
	 const TRSeedTrack* theSeed,
         Frame& theFrame,
         const STL_VECTOR(
            HIDRSurfaceFactory::DRHitAndDriftDistance )* aDRVector = 0,
         const STL_VECTOR( const CalibratedCathodeHit* )* aCathodeVector = 0,
         const STL_VECTOR( const CalibratedSVRphiHit* )* aRphiVector = 0,
         const STL_VECTOR( const CalibratedSVZHit* )* aZVector = 0,
         GeV aMass = 0.13957,
         DABoolean aExtractHitsIfNull = true ) ;

      DABoolean fitTrackOneDirection( HIHelixIntersector& aIntersector,
				      KalmanHelix* aHelix,
				      FitDirection aDirection,
				      DABoolean aReturnTrackAtLastHit = false )
	 const ;


      // ---------- private const member functions -------------
      void makeConditionsAndOperations() ;

      // ---------- data members -------------------------------
      DABoolean m_printDiagnostics ;

      DABoolean m_useDRAxialHits ;
      DABoolean m_useDRStereoUHits ;
      DABoolean m_useDRStereoVHits ;
      DABoolean m_correctDRHits ;

      DABoolean m_useDRCathodeHits ;
      DABoolean m_correctDRCathodeHits ;

      DABoolean m_useSVRPhiHits ;
      DABoolean m_useSVZHits ;
      DABoolean m_correctSVRHits ;
      DABoolean m_correctSVZHits ;

      DABoolean m_applyDedx ;
      DABoolean m_applyStraggling ;
      double    m_stragglingCoeff ;
      DABoolean m_applyMultScat ;
      DABoolean m_applyBfieldCorrections ;

      double m_hitChisqCut ;
      double m_maxFracDP ;

      double m_updateErr2Z0 ;
      double m_updateErr2CotTheta ;
      double m_updateDFracCotTheta ;

      int m_maxHitDel ;
      double m_maxHitPull ;

      STL_VECTOR( KalmanHelix* )* m_fittedHelices[ DBCandidate::kMaxHypo ] ;

      HICompositeOperation* m_fitOperation ;
      HICompositeOperation* m_fitSiOperation ;
      HICompositeOperation* m_noFitOperation ;
      HICompositeOperation* m_noFitSiOperation ;
      HICondition*      m_globalCondition ;
      HIHelixOperation* m_globalOperation ;

      DABoolean m_siCacheFilled ;
      STL_VECTOR( HIIntersectionSurface* )* m_siCache ;
      DABoolean m_irCacheFilled ;
      STL_VECTOR( HIIntersectionSurface* )* m_irCache ;
      DABoolean m_drCacheFilled ;
      STL_VECTOR( HIIntersectionSurface* )* m_drCache ;
      DABoolean m_hitSurfacesFilled ;
      STL_VECTOR( HIIntersectionSurface* )* m_hitSurfaces ;
      const TRSeedTrack* m_hitSurfacesTrack ;
      DABoolean m_siHitSurfacesAtFrontOfList ;

      DABoolean m_fixParameter[ HIHelix::kZ0 + HIHelix::kCurvature ] ;
      HIFitConstraint*            m_fitConstraint ;
      HITrackParameterConstraint* m_fixParamsConstraint ;
      HICompositeFitConstraint*   m_compositeConstraint ;

      InputTrackInfo m_inputTrackInfo ;

      // Pointer set by fitTrack() to avoid passing it as an argument
      // to fittedTrack().  Gets set to null at the end of fitTrack().
      KalmanHelix* m_inwardFit ;

      DABoolean m_inwardFitIsOutwardSeed ;

      // Si boundary surfaces are duplicated if they are merged into the
      // hit surfaces list more than once.
      DABoolean m_siSurfacesMerged ;

      DABoolean m_continueIfHitsMissing ;
      DABoolean m_continueIfAnyException ;

      DABoolean m_deleteInwardFit ;

      DABoolean m_keepPreviousHitList ;

      STL_VECTOR( DBCandidate::Hypo )* m_hyposToFit ;
      DABoolean m_hitListFromOneHypo ;
      DBCandidate::Hypo m_hitListHypo ;

      UsageTag m_drHitUsageTag ;
      ProductionTag m_drHitProdTag ;
      UsageTag m_cathodeHitUsageTag ;
      ProductionTag m_cathodeHitProdTag ;
      UsageTag m_svrHitUsageTag ;
      ProductionTag m_svrHitProdTag ;
      UsageTag m_svzHitUsageTag ;
      ProductionTag m_svzHitProdTag ;

      DABoolean m_forceDRFittingWeight ;
      double m_drFittingWeight ;
      DABoolean m_forceSVRFittingWeight ;
      double m_svrFittingWeight ;
      DABoolean m_forceSVZFittingWeight ;
      double m_svzFittingWeight ;
      DABoolean m_forceCathodeFittingWeight ;
      double m_cathodeFittingWeight ;
      DABoolean m_scaleCathodeFittingWeight ;
      double m_cathodeFittingWeightScale ;

      // When m_keepPreviousHitList is true, these quantities are not
      // determined for every fit.  But the quality objects should record
      // the same number of hits dropped and flipped for each mass
      // hypothesis.  Therefore, the KalmanTrackFitter needs to save these
      // numbers from fit to fit and reset them only when the hit list
      // is cleared.
      int m_nHitsDropped ;
      int m_nHitsFlipped ;
      DABoolean m_moreBadHits ;
      double m_pullOfNextWorstHit ;

      // By default, inward fits are propagated to the coordinate origin
      // after the last hit.  If this flag is true, the reference point of
      // the inward fits will be at the innermost hit.
      DABoolean m_inwardFitsAtLastHit ;

      // Keep list of pointers to HIMSDedxOperations and KalmanAddHitOperations
      // because they need to be reset when undoing the fit.
      STL_VECTOR( HIMSDedxOperation* )* m_msdedxOperations ;
      STL_VECTOR( KalmanAddHitOperation* )* m_addHitOperations ;
      STL_VECTOR( KalmanUpdateParamsOperation* )* m_updateParamsOperations ;
      STL_VECTOR( KalmanUpdateParamsCondition* )* m_updateParamsConditions ;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "KalmanFilter/Template/KalmanTrackFitter.cc"
//#endif

#endif /* KALMANFILTER_KALMANTRACKFITTER_H */
