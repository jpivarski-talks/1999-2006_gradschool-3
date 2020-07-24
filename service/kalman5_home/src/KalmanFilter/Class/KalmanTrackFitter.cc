// -*- C++ -*-
//
// Package:     KalmanFilter
// Module:      KalmanTrackFitter
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Tue Sep 29 17:00:02 EDT 1998
// $Id: KalmanTrackFitter.cc,v 1.57 2003/01/29 21:44:18 cleo3 Exp $
//
// Revision history
//
// $Log: KalmanTrackFitter.cc,v $
// Revision 1.57  2003/01/29 21:44:18  cleo3
// only include nan.h if _IEEE defined
//
// Revision 1.56  2002/03/22 00:50:17  wsun
// Copy seed track to inward fit when fit fails.
//
// Revision 1.55  2002/02/23 06:48:49  wsun
// * Added flag for applying B field corrections.
// * Updated hit addition/removal to new fit ordering.
//
// Revision 1.54  2002/02/22 03:32:43  wsun
// On inward fit, set first surface to be outermost hit surface.
//
// Revision 1.53  2002/02/21 07:18:57  wsun
// Reverse order of fits: first inward, then outward.
//
// Revision 1.52  2002/01/23 22:03:45  llh14
// Changes for NonUniform B field corrections
//
// Revision 1.51  2001/10/09 17:39:26  llh14
// Fixed a problem with link data ownership
//
// Revision 1.50  2001/03/27 06:37:50  wsun
// Mods for hit addition and removal.
//
// Revision 1.49  2001/03/04 20:42:38  wsun
// Check for NaN only if _IEEE is defined.
//
// Revision 1.48  2001/03/03 05:29:54  wsun
// Check seed track momentum is not NaN.
//
// Revision 1.47  2001/01/26 22:24:54  cleo3
// removed use of sqr and use fabs instead of abs when computing pull
//
// Revision 1.46  2001/01/05 05:33:01  lyon
// Added LayerMap to quality object
//
// Revision 1.45  2000/12/22 07:02:03  wsun
// Fixed handling of additional hits added to fit.
//
// Revision 1.44  2000/12/13 07:02:37  wsun
// Added option to not have inward fits propagated to the origin.
//
// Revision 1.43  2000/11/18 00:16:45  wsun
// Allow cathode fitting weight to be scaled by user-defined parameter.
//
// Revision 1.42  2000/10/29 02:34:20  wsun
// Get only hits to be fit from HIDRSurfaceFactory.
//
// Revision 1.41  2000/09/05 20:45:20  wsun
// Mods to allow user to force fitting weight to a given value.
//
// Revision 1.40  2000/08/16 20:13:10  wsun
// Count number of discarded and flipped hits.
//
// Revision 1.39  2000/07/25 21:31:56  wsun
// User can specify usage and and production tags for hits.
//
// Revision 1.38  2000/06/10 06:02:39  wsun
// Added parameters to dis/enable corrections to DR, SVR, SVZ, DRCA hits.
//
// Revision 1.37  2000/06/10 00:13:40  wsun
// Hit corrections now applied by HICalibratedHits instead of surfaces.
//
// Revision 1.36  2000/06/09 00:50:40  wsun
// * Allow hit list to be set by one mass hypothesis.
// * Allow selection of hypotheses to be fit.
//
// Revision 1.35  2000/06/06 22:21:58  wsun
// Prevent double deleting by setting m_outwardFit to null.
//
// Revision 1.34  2000/04/03 17:21:37  wsun
// Check if seed track has zero momentum.
//
// Revision 1.33  2000/03/02 08:36:11  wsun
// * Keep a list of all HICalibratedHits on surfaces because inward and
//   outward fits may have different hits.
// * Check quality of outward fit before using it as an inward seed.
// * Overload TrackFitter::event() functions to handle m_hypoFilled correctly.
//
// Revision 1.32  2000/02/04 19:19:04  wsun
// Fixed a bug that compiled on SunOS but not on OSF1.
//
// Revision 1.31  2000/02/03 18:48:04  wsun
// Use smoothed residuals to discard hits or flip drift sign.
//
// Revision 1.30  2000/01/11 20:35:59  wsun
// Added flags to control exception handling.
//
// Revision 1.29  1999/12/15 21:12:39  wsun
// Replaced HIDedxOperation and HIMultScatOperation with HIMSDedxOperation.
//
// Revision 1.28  1999/11/21 22:36:44  wsun
// Fixed bug in surface merging.
//
// Revision 1.27  1999/10/08 23:59:28  wsun
// Forgot to delete some debug output.
//
// Revision 1.26  1999/10/08 23:16:49  wsun
// * Added individual surface caches for Si, IR, DR.
// * Added surface cache for hit surfaces.
// * Allow outward fits to be used as seeds for the inward fits.
//
// Revision 1.25  1999/10/05 23:02:45  wsun
// Added functions for user to specify hits to fit.
//
// Revision 1.24  1999/09/30 20:03:15  wsun
// Forgot to apply Suez parameter for disabling DR cathode hits.
//
// Revision 1.23  1999/09/12 21:58:52  wsun
// Clear error matrix before outward swim for inward fits.
//
// Revision 1.22  1999/09/10 21:53:38  wsun
// Added cathode surface.
//
// Revision 1.21  1999/08/30 21:53:16  wsun
// Added flags for dE/dx straggling and surfaces from HIIRSurfaceFactory.
//
// Revision 1.20  1999/08/27 20:41:57  wsun
// Delay track parameter update for Si hits.
//
// Revision 1.19  1999/08/05 20:04:24  wsun
// Protect against uninitialized helix at penultimate hit.
//
// Revision 1.18  1999/08/03 22:30:22  wsun
// Mods for fitting curler back-halves.
//
// Revision 1.17  1999/07/22 23:12:21  wsun
// Reenable inadvertently disabled track fitting.
//
// Revision 1.16  1999/07/15 00:54:51  wsun
// Stupid CC for-loop variable scoping rules...
//
// Revision 1.15  1999/07/12 07:06:08  wsun
// * Forgot to initialize m_fixParameter in constructor.
// * Added surfaces for DR inner tube.
//
// Revision 1.14  1999/07/07 16:46:42  wsun
// Added mechanism for applying constraints in track fitter.
//
// Revision 1.13  1999/06/30 23:51:46  wsun
// Added cache of Si surfaces needed for every event.
//
// Revision 1.12  1999/06/24 22:44:26  wsun
// Added use of HIMagField and new SiHits.
//
// Revision 1.11  1999/06/21 23:04:10  pg
//  Modifications for new Si hits
//
// Revision 1.10  1999/06/18 19:28:00  wsun
// More improvements to low momentum fitting.
//
// Revision 1.9  1999/06/02 23:07:43  wsun
// Improved low-momentum fitting.
//
// Revision 1.7  1999/05/14 01:13:11  wsun
// Updated for use with KalmanFilter producer.
//
// Revision 1.6  1999/05/09 21:38:35  wsun
// Updated code to new Lattice interface.
//
// Revision 1.5  1999/04/27 22:20:52  wsun
// Major reorganization and structural changes.
//
// Revision 1.4  1999/02/11 21:13:36  wsun
// kPlacedDRName -> kDRName.
//
// Revision 1.3  1998/12/18 03:19:00  wsun
// Removed tag SV3HITS from extract for CalibratedSVHits.
//
// Revision 1.2  1998/12/16 23:51:11  wsun
// SeedTrack/SVHit lattices moved to TrackFinder.
//
// Revision 1.1.1.1  1998/12/07 04:59:01  wsun
// First submission.
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */
#if defined(_IEEE)
#include <nan.h>
#endif

// user include files
#include "Experiment/report.h"
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"

#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"
#include "FrameAccess/extract.h"

#include "TrackRoot/TRSeedTrack.h"

#include "DRGeom/DRGeometryParams.h"
#include "ADRGeom/ADRSenseWireStore.h"

#include "DetectorGeometry/DGMaterialStore.h"
#include "MaterialDelivery/material_identifiers.h"

#include "CalibratedData/CalibratedDRHit.h"
#include "CalibratedData/CalibratedCathodeHit.h"
#include "SiHits/CalibratedSVRphiHit.h"
#include "SiHits/CalibratedSVZHit.h"

#include "Lattice/Lattice.h"
#include "TrackFinder/SeedDRHitLink.h"
#include "TrackFinder/SeedTrackSVRHitLattice.h"
#include "TrackFinder/SeedTrackSVZHitLattice.h"

#include "HelixIntersection/HIHelixIntersector.h"
#include "HelixIntersection/HIIntersectionSurface.h"
#include "HelixIntersection/HIIntersectionCylinder.h"
#include "HelixIntersection/HIZBoundaryCondition.h"
#include "HelixIntersection/HIDedxCondition.h"
#include "HelixIntersection/HIMSDedxOperation.h"
#include "HelixIntersection/HICompositeOperation.h"
#include "HelixIntersection/HIMoveIterateOperation.h"
#include "HelixIntersection/HIDRSurfaceFactory.h"
#include "HelixIntersection/HICathodeSurfaceFactory.h"
#include "HelixIntersection/HISiSurfaceFactory.h"
#include "HelixIntersection/HICompositeSurface.h"

#include "HelixIntersection/HIFitConstraint.h"
#include "HelixIntersection/HITrackParameterConstraint.h"
#include "HelixIntersection/HICompositeFitConstraint.h"

#include "HelixIntersection/HIIRSurfaceFactory.h"

#include "HelixIntersection/HICorrectForBFieldOperation.h"
// // TEMPORARY
// #include "HelixIntersection/HIMomentumConstraint.h"

#include "KalmanFilter/KalmanAddHitOperation.h"
//#include "KalmanFilter/KalmanFitQualityCondition.h"
#include "KalmanFilter/KalmanUpdateParamsCondition.h"
#include "KalmanFilter/KalmanUpdateZParamsCondition.h"
#include "KalmanFilter/KalmanUpdateParamsOperation.h"
#include "KalmanFilter/KalmanTrackFitter.h"

#include "DAException/DANoDataException.h"

#include "CLHEP/Matrix/SymMatrix.h"

// STL classes
#include <vector>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "KalmanTrackFitter" ;

//
// static data member definitions
//

//
// constructors and destructor
//
KalmanTrackFitter::KalmanTrackFitter( HIFitConstraint* aConstraint,
				      InputTrackInfo aInputTrackInfo )
   : m_printDiagnostics( false ),
     m_useDRAxialHits( true ),
     m_useDRStereoUHits( true ),
     m_useDRStereoVHits( true ),
     m_correctDRHits( true ),
     m_useDRCathodeHits( true ),
     m_correctDRCathodeHits( true ),
     m_useSVRPhiHits( true ),
     m_useSVZHits( true ),
     m_correctSVRHits( true ),
     m_correctSVZHits( true ),
     m_applyDedx( true ),
     m_applyStraggling( true ),
     m_stragglingCoeff( 0.001 ),
     m_applyMultScat( true ),
     m_hitChisqCut( 50. ),
     m_maxFracDP( 1.1 ),
     m_fitOperation( 0 ),
     m_fitSiOperation( 0 ),
     m_noFitOperation( 0 ),
     m_noFitSiOperation( 0 ),
     m_globalCondition( 0 ),
     m_globalOperation( 0 ),
     m_siCacheFilled( false ),
     m_siCache( new STL_VECTOR( HIIntersectionSurface* ) ),
     m_irCacheFilled( false ),
     m_irCache( new STL_VECTOR( HIIntersectionSurface* ) ),
     m_drCacheFilled( false ),
     m_drCache( new STL_VECTOR( HIIntersectionSurface* ) ),
     m_hitSurfacesFilled( false ),
     m_hitSurfaces( new STL_VECTOR( HIIntersectionSurface* ) ),
     m_hitSurfacesTrack( 0 ),
     m_siHitSurfacesAtFrontOfList( false ),
     m_fitConstraint( aConstraint ),
     m_fixParamsConstraint( new HITrackParameterConstraint ),
     m_compositeConstraint( 0 ),
     m_inputTrackInfo( aInputTrackInfo ),
     m_inwardFit( 0 ),
     m_inwardFitIsOutwardSeed( true ),
     m_siSurfacesMerged( false ),
     m_continueIfHitsMissing( false ),
     m_continueIfAnyException( false ),
     m_maxHitDel( 3 ),
     m_maxHitPull( 3.5 ),
     m_deleteInwardFit( true ),
     m_keepPreviousHitList( false ),
     m_hyposToFit( new STL_VECTOR( DBCandidate::Hypo ) ),
     m_hitListFromOneHypo( true ),
     m_hitListHypo( DBCandidate::kChargedPion ),
     m_drHitUsageTag( UsageTag() ),
     m_drHitProdTag( ProductionTag() ),
     m_cathodeHitUsageTag( UsageTag() ),
     m_cathodeHitProdTag( ProductionTag() ),
     m_svrHitUsageTag( UsageTag() ),
     m_svrHitProdTag( ProductionTag() ),
     m_svzHitUsageTag( UsageTag() ),
     m_svzHitProdTag( ProductionTag() ),
     m_nHitsDropped( 0 ),
     m_nHitsFlipped( 0 ),
     m_moreBadHits( false ),
     m_pullOfNextWorstHit( 0. ),
     m_forceDRFittingWeight( false ),
     m_drFittingWeight( 0. ),
     m_forceSVRFittingWeight( false ),
     m_svrFittingWeight( 0. ),
     m_forceSVZFittingWeight( false ),
     m_svzFittingWeight( 0. ),
     m_forceCathodeFittingWeight( false ),
     m_cathodeFittingWeight( 0. ),
     m_scaleCathodeFittingWeight( false ),
     m_cathodeFittingWeightScale( 1. ),
     m_inwardFitsAtLastHit( false ),
     m_msdedxOperations( new STL_VECTOR( HIMSDedxOperation* ) ),
     m_addHitOperations( new STL_VECTOR( KalmanAddHitOperation* ) ),
     m_updateParamsOperations( new STL_VECTOR( KalmanUpdateParamsOperation* )),
     m_updateParamsConditions( new STL_VECTOR( KalmanUpdateParamsCondition* ) )
{
//    // TEMPORARY
//    m_fitConstraint = new HIMomentumConstraint ;

   for( int i = 0 ; i < DBCandidate::kMaxHypo ; ++i )
   {
      m_fittedHelices[ i ] = new STL_VECTOR( KalmanHelix* ) ;
   }

   if( m_fitConstraint != 0 )
   {
      m_compositeConstraint = new HICompositeFitConstraint ;
      m_compositeConstraint->setDtorDeletesChildren( false ) ;
      m_compositeConstraint->addChild( m_fitConstraint ) ;
      m_compositeConstraint->addChild( m_fixParamsConstraint ) ;
   }

   for( int j = 0 ; j < HIHelix::kZ0 + HIHelix::kCurvature ; ++j )
   {
      m_fixParameter[ j ] = false ;
   }

   m_hyposToFit->push_back( DBCandidate::kElectron ) ;
   m_hyposToFit->push_back( DBCandidate::kMuon ) ;
   m_hyposToFit->push_back( DBCandidate::kChargedPion ) ;
   m_hyposToFit->push_back( DBCandidate::kChargedKaon ) ;
   m_hyposToFit->push_back( DBCandidate::kProton ) ;
}


// KalmanTrackFitter::KalmanTrackFitter( const KalmanTrackFitter& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say
//    *this = rhs;
// }

KalmanTrackFitter::~KalmanTrackFitter()
{
   clearCache() ;
   for( int i = 0 ; i < DBCandidate::kMaxHypo ; ++i )
   {
      delete m_fittedHelices[ i ] ;
   }

   // Delete the Si surface cache.
   STL_VECTOR( HIIntersectionSurface* )::iterator itr = m_siCache->begin() ;
   STL_VECTOR( HIIntersectionSurface* )::iterator end = m_siCache->end() ;
   for( ; itr != end ; ++itr )
   {
      if( ( *itr )->isComposite() )
      {
	 ( ( HICompositeSurface* )( *itr ) )->deleteConditions() ;
      }
      delete *itr ;
   }
   delete m_siCache ;

   // Delete the IR surface cache.
   itr = m_irCache->begin() ;
   end = m_irCache->end() ;
   for( ; itr != end ; ++itr )
   {
      if( ( *itr )->isComposite() )
      {
	 ( ( HICompositeSurface* )( *itr ) )->deleteConditions() ;
      }
      delete *itr ;
   }
   delete m_irCache ;

   // Delete the DR surface cache.
   itr = m_drCache->begin() ;
   end = m_drCache->end() ;
   for( ; itr != end ; ++itr )
   {
      if( ( *itr )->isComposite() )
      {
	 ( ( HICompositeSurface* )( *itr ) )->deleteConditions() ;
      }
      delete *itr ;
   }
   delete m_drCache ;

   clearConditionsAndOperations() ;

   delete m_fixParamsConstraint ;
   delete m_compositeConstraint ;
   delete m_hitSurfaces ;
   delete m_hyposToFit ;
   delete m_msdedxOperations ;
   delete m_addHitOperations ;
   delete m_updateParamsOperations ;
   delete m_updateParamsConditions ;

//    // TEMPORARY!!!!!
//    delete m_fitConstraint ;
//    m_fitConstraint = 0 ;
}

void
KalmanTrackFitter::clearCache()
{
   // Delete the helices in the vectors.
   for( int i = 0 ; i < DBCandidate::kMaxHypo ; ++i )
   {
      STL_VECTOR( KalmanHelix* )::iterator helixItr =
	 m_fittedHelices[ i ]->begin() ;
      STL_VECTOR( KalmanHelix* )::iterator helixEnd =
	 m_fittedHelices[ i ]->end() ;

      for( ; helixItr != helixEnd ; ++helixItr )
      {
	 delete *helixItr ;
      }
      m_fittedHelices[ i ]->clear() ;
   }

   // Delete the conditions and the hit surfaces.
   STL_VECTOR( HIIntersectionSurface* )::iterator itr = m_hitSurfaces->begin();
   STL_VECTOR( HIIntersectionSurface* )::iterator end = m_hitSurfaces->end() ;

   for( ; itr != end ; ++itr )
   {
      ( *itr )->deleteCondition() ;
      delete *itr ;
   }
   m_hitSurfaces->clear() ;
   m_siSurfacesMerged = false ;

   m_hitSurfacesTrack = 0 ;
   m_siHitSurfacesAtFrontOfList = false ;

   TrackFitter::clearCache() ;
}

//
// assignment operators
//
// const KalmanTrackFitter& KalmanTrackFitter::operator=(
//    const KalmanTrackFitter& rhs )
// {
//   if( this != &rhs ) {
//      // do actual copying here, plus:
//      // "SuperClass"::operator=( rhs );
//   }
//   return *this;
// }

//
// member functions
//

void
KalmanTrackFitter::setHyposToFit( STL_VECTOR( DBCandidate::Hypo )& aVector )
{
   *m_hyposToFit = aVector ;
}

void
KalmanTrackFitter::makeConditionsAndOperations() 
{
   // Instantiate the dedx and mult. scat. helix operations.  These are
   // deleted after this track has been fitted.
   m_fitOperation     = new HICompositeOperation() ;
   m_fitSiOperation   = new HICompositeOperation() ;
   m_noFitOperation   = new HICompositeOperation() ;
   m_noFitSiOperation = new HICompositeOperation() ;

   if( m_applyMultScat || m_applyDedx )
   {
      HIMSDedxOperation* msdedx0 = new HIMSDedxOperation( false,
							  false,
							  m_applyStraggling, 
							  m_stragglingCoeff,
							  m_applyMultScat,
							  m_applyDedx ) ;

      m_fitOperation->addChild( msdedx0 ) ;      
      m_msdedxOperations->push_back( msdedx0 ) ;

      HIMSDedxOperation* msdedx1 = new HIMSDedxOperation( false,
							  false,
							  m_applyStraggling, 
							  m_stragglingCoeff,
							  m_applyMultScat,
							  m_applyDedx ) ;

      m_fitSiOperation->addChild( msdedx1 );
      m_msdedxOperations->push_back( msdedx1 ) ;


      HIMSDedxOperation* msdedx2 = new HIMSDedxOperation( false,
							  false,
							  m_applyStraggling, 
							  m_stragglingCoeff,
							  m_applyMultScat,
							  m_applyDedx ) ;
      m_noFitOperation->addChild( msdedx2 );
      m_msdedxOperations->push_back( msdedx2 ) ;

      HIMSDedxOperation* msdedx3 = new HIMSDedxOperation( false,
							  false,
							  m_applyStraggling, 
							  m_stragglingCoeff,
							  m_applyMultScat,
							  m_applyDedx ) ;

      m_noFitSiOperation->addChild( msdedx3 ) ;
      m_msdedxOperations->push_back( msdedx3 ) ;
   }

   if( m_applyBfieldCorrections )
   {
      m_fitOperation->addChild( new HICorrectForBFieldOperation( false ) );
      m_fitSiOperation->addChild( new HICorrectForBFieldOperation( false ) );
      m_noFitOperation->addChild(  new HICorrectForBFieldOperation( false ) );
      m_noFitSiOperation->addChild( new HICorrectForBFieldOperation( false ) );
   }

   // Instantiate operations for non-Si layers with hits.
   KalmanAddHitOperation* addHit0 = new KalmanAddHitOperation( m_hitChisqCut );

   m_fitOperation->addChild( addHit0 ) ;
   m_addHitOperations->push_back( addHit0 ) ;

   KalmanUpdateParamsCondition* updateCondition0 =
      new KalmanUpdateParamsCondition ;
   KalmanUpdateParamsOperation* update0 =
      new KalmanUpdateParamsOperation(
	 updateCondition0,
	 new KalmanUpdateZParamsCondition( m_updateErr2Z0,
					   m_updateErr2CotTheta,
					   m_updateDFracCotTheta ) ) ;

   m_fitOperation->addChild( update0 ) ;
   m_updateParamsOperations->push_back( update0 ) ;
   m_updateParamsConditions->push_back( updateCondition0 ) ;

   // Instantiate operations for Si layers with hits.
   KalmanAddHitOperation* addHit1 = new KalmanAddHitOperation( m_hitChisqCut );
   m_fitSiOperation->addChild( addHit1 ) ;
   m_addHitOperations->push_back( addHit1 ) ;

   // Instantiate operations for non-Si layers without hits.
   KalmanUpdateParamsCondition* updateCondition1 =
      new KalmanUpdateParamsCondition ;
   KalmanUpdateParamsOperation* update1 =
      new KalmanUpdateParamsOperation(
	 updateCondition1,
	 new KalmanUpdateZParamsCondition( m_updateErr2Z0,
					   m_updateErr2CotTheta,
					   m_updateDFracCotTheta ) ) ;

   m_noFitSiOperation->addChild( update1 ) ;
   m_updateParamsOperations->push_back( update1 ) ;
   m_updateParamsConditions->push_back( updateCondition1 ) ;


   // This is false by default.
   m_fitOperation->setPrintDiagnostics( m_printDiagnostics ) ;
   m_fitSiOperation->setPrintDiagnostics( m_printDiagnostics ) ;
   m_noFitOperation->setPrintDiagnostics( m_printDiagnostics ) ;
   m_noFitSiOperation->setPrintDiagnostics( m_printDiagnostics ) ;


   // Global condition and operation for HIHelixIntersector.
   if( m_applyDedx && m_maxFracDP < 1. )
   {
      // moveOp is the operation given to the HIMoveIterateOperation.
      HICompositeOperation* moveOp = new HICompositeOperation ;

      HIMSDedxOperation* msdedx4 = new HIMSDedxOperation( false,
							  false,
							  m_applyStraggling,
							  m_stragglingCoeff,
							  m_applyMultScat,
							  m_applyDedx ) ;

      moveOp->addChild( msdedx4 ) ;
      m_msdedxOperations->push_back( msdedx4 ) ;

      //Adding this for Bfield corrections
      moveOp->addChild(  new HICorrectForBFieldOperation( false ) );

      HIMoveIterateOperation* moveIterOp = 
	 new HIMoveIterateOperation( moveOp ) ;
      m_globalOperation = moveIterOp ;
      m_globalCondition = new HIDedxCondition( m_maxFracDP, moveIterOp ) ;

      moveOp->setPrintDiagnostics( m_printDiagnostics ) ;
      m_globalCondition->setPrintDiagnostics( m_printDiagnostics ) ;
      m_globalOperation->setPrintDiagnostics( m_printDiagnostics ) ;
   }
}

void
KalmanTrackFitter::clearConditionsAndOperations()
{
   delete m_globalCondition ;
   m_globalCondition = 0 ;

   delete m_globalOperation ;
   m_globalOperation = 0 ;

   delete m_fitOperation ;
   m_fitOperation = 0 ;

   delete m_fitSiOperation ;
   m_fitSiOperation = 0 ;

   delete m_noFitOperation ;
   m_noFitOperation = 0 ;

   delete m_noFitSiOperation ;
   m_noFitSiOperation = 0 ;
}



DABoolean
KalmanTrackFitter::fitTrack( const TRSeedTrack* theSeed,
			     Frame& theFrame,
			     DBCandidate::Hypo aHypo )
{
   // Check that this track and hypothesis has not been fit already.
   STL_VECTOR( KalmanHelix* )::iterator itr = m_fittedHelices[aHypo]->begin();
   STL_VECTOR( KalmanHelix* )::iterator end = m_fittedHelices[aHypo]->end() ;

   for( ; itr != end ; ++itr )
   {
      if( ( *itr )->identifier() == theSeed->identifier() )
      {
	 return true ;
      }
   }

   // Now fit the given hypothesis.
   KalmanHelix* fittedHelix =
      ( KalmanHelix* ) fittedTrack( theSeed,
				    theFrame,
				    DBCandidate::mass( aHypo ) ) ;


   if( fittedHelix != 0 )
   {
      m_fittedHelices[ DBCandidate::outwardHypo( aHypo ) ]->push_back(
	 fittedHelix ) ;

      // NavTrack matches seeds and fits by identifier.
      fittedHelix->setIdentifier( theSeed->identifier() ) ;
   }

   if( m_inwardFit != 0 )
   {
      m_fittedHelices[ DBCandidate::inwardHypo( aHypo ) ]->push_back(
	 m_inwardFit ) ;

      // NavTrack matches seeds and fits by identifier.
      m_inwardFit->setIdentifier( theSeed->identifier() ) ;

      // fittedTrack() should not delete the outward fit.
      m_deleteInwardFit = false ;
   }

   return true ;
}


KTHelix*
KalmanTrackFitter::fittedTrack( const TRSeedTrack* theSeed,
				Frame& theFrame,
				DBCandidate::Hypo aHypo )
{
   KTHelix* outwardFit = fittedTrack( theSeed,
				     theFrame,
				     DBCandidate::mass( aHypo ) ) ;
   
   if( !DBCandidate::hypoIsInward( aHypo ) )
   {
      delete m_inwardFit ;
      m_inwardFit = 0 ;  // prevents fittedTrack() from deleting again.
      return outwardFit ;
   }
   else
   {
      delete outwardFit ;
      return m_inwardFit ;
   }
}

KTHelix*
KalmanTrackFitter::fittedTrack(
   const TRSeedTrack* theSeed,
   Frame& theFrame,
   const STL_VECTOR(
      HIDRSurfaceFactory::DRHitAndDriftDistance )* aDRVector,
   const STL_VECTOR( const CalibratedCathodeHit* )* aCathodeVector,
   const STL_VECTOR( const CalibratedSVRphiHit* )* aRphiVector,
   const STL_VECTOR( const CalibratedSVZHit* )* aZVector,
   DBCandidate::Hypo aHypo,
   DABoolean aExtractHitsIfNull )
{
   KTHelix* outwardFit = fittedTrack( theSeed,
				      theFrame,
				      aDRVector,
				      aCathodeVector,
				      aRphiVector,
				      aZVector,
				      DBCandidate::mass( aHypo ),
				      aExtractHitsIfNull ) ;

   if( !DBCandidate::hypoIsInward( aHypo ) )
   {
      delete m_inwardFit ;
      m_inwardFit = 0 ;  // prevents fittedTrack() from deleting again.
      return outwardFit ;
   }
   else
   {
      delete outwardFit ;
      return m_inwardFit ;
   }
}

KTHelix*
KalmanTrackFitter::fittedTrack( const TRSeedTrack* theSeed,
				Frame& theFrame,
				GeV aMass,
				FitDirection aFitDirection )
{
   return fittedTrack( theSeed, theFrame, 0, 0, 0, 0, aMass ) ;
}


KTHelix*
KalmanTrackFitter::removeHitsFromFit(
   FitDirection aDirection,
   const TRSeedTrack* theSeed,
   Frame& theFrame,
   const STL_VECTOR(
      HIDRSurfaceFactory::DRHitAndDriftDistance )* aDRVector,
   const STL_VECTOR( const CalibratedCathodeHit* )* aCathodeVector,
   const STL_VECTOR( const CalibratedSVRphiHit* )* aRphiVector,
   const STL_VECTOR( const CalibratedSVZHit* )* aZVector,
   GeV aMass )
{
   int nHitsToRemove =
      ( aDRVector == 0 ? 0 : aDRVector->size() ) +
      ( aCathodeVector == 0 ? 0 : aCathodeVector->size() ) +
      ( aRphiVector == 0 ? 0 : aRphiVector->size() ) +
      ( aZVector == 0 ? 0 : aZVector->size() ) ;

   // Make a non-const copy of the seed track.
   KalmanHelix* aHelix = new KalmanHelix( *theSeed, aMass ) ;
   KalmanHelix copyOfOriginal = *aHelix ;

   GeV pmag2 = aHelix->momentum().mag2() ;
#if _IEEE
   if( pmag2 == 0. || IsNANorINF( pmag2 ) )
#else
   if( pmag2 == 0. )
#endif
   {
      aHelix->setFitFailed( true ) ;
      return aHelix ;
   }

   // m_fitOperation is nonnull if makeConditionsAndOperations()
   // has been called.
   if( m_fitOperation == 0 )
   {
      makeConditionsAndOperations() ;
   }

   // Generate surfaces for entire detector, including hit surfaces.
   STL_VECTOR( HIIntersectionSurface* ) surfaces =
      generateSurfaces( theSeed, theFrame,
			aDRVector, aCathodeVector, aRphiVector, aZVector,
			aMass,
			false ) ; // extract hits if null

   if( surfaces.size() == 0 )
   {
      aHelix->setFitFailed( true ) ;
      return aHelix ;
   }


   //~~~~~~~~~ Set up track parameter constraints, if any ~~~~~~~~~~

   DABoolean parametersFixed = false ;

   // If no constraints other than fixing parameters are to be applied,
   // then simply remove the free parameters from the helix.  Otherwise,
   // fixing the parameters is applied after the unconstrained fit as
   // another constraint.  This is to avoid inverting a singular matrix
   // in solving the constraint.
   m_fixParamsConstraint->clearFixedParameters() ;

   for( int i = HIHelix::kCurvature ; i <= HIHelix::kZ0 ; ++i )
   {
      if( m_fixParameter[ i ] )
      {
	 m_fixParamsConstraint->setParameterToFix( i ) ;
	 parametersFixed = true ;
      }
   }

   if( parametersFixed )
   {
      m_fixParamsConstraint->setConstraintParametersOne( *aHelix ) ;

      if( m_fitConstraint != 0 )
      {
	 m_compositeConstraint->recalculateNumberOfConstraints() ;
      }
   }


   // ~~~~~~~~~~~ Set operations for hit removal ~~~~~~~~~~~

   STL_VECTOR( HIMSDedxOperation* )::const_iterator msdedxItr =
      m_msdedxOperations->begin() ;
   STL_VECTOR( HIMSDedxOperation* )::const_iterator msdedxEnd =
      m_msdedxOperations->end() ;

   for( ; msdedxItr != msdedxEnd ; ++msdedxItr )
   {
      ( *msdedxItr )->setReverseTransport( true ) ;
   }

   STL_VECTOR( KalmanAddHitOperation* )::const_iterator addHitItr =
      m_addHitOperations->begin() ;
   STL_VECTOR( KalmanAddHitOperation* )::const_iterator addHitEnd =
      m_addHitOperations->end() ;

   for( ; addHitItr != addHitEnd ; ++addHitItr )
   {
      ( *addHitItr )->setRemoveHitsFromFit( true ) ;
   }

   //~~~~~~~~~~~~ Use reverse hit order in surfaces ~~~~~~~~~~~~~~~

   STL_VECTOR( HIIntersectionSurface* )::iterator surfaceItr =
      surfaces.begin();
   STL_VECTOR( HIIntersectionSurface* )::iterator surfaceEnd =
      surfaces.end();

   for( ; surfaceItr != surfaceEnd ; ++surfaceItr )
   {
      ( *surfaceItr )->setUseReverseHitOrder( true ) ;
   }

   //~~~~~~~~~~~~ Always update parameters after adding a hit ~~~~~~~~~~~~~~~

   STL_VECTOR( KalmanUpdateParamsCondition* )::const_iterator updateCondItr =
      m_updateParamsConditions->begin() ;
   STL_VECTOR( KalmanUpdateParamsCondition* )::const_iterator updateCondEnd =
      m_updateParamsConditions->end() ;

   for( ; updateCondItr != updateCondEnd ; ++updateCondItr )
   {
      ( *updateCondItr )->setIgnoreNumberOfHits( true ) ;
   }


   //~~~~~~~~~~~~ Instantiate a new HIHelixIntersector ~~~~~~~~~~~~~~~

   HIHelixIntersector intersector( surfaces,
				   HIHelixIntersector::kIncreasingRadius,
				   aHelix ) ;

   if( m_printDiagnostics )
   {
      intersector.setPrintDiagnostics( true ) ;

      report( DEBUG, kFacilityString )
	 << "r "
	 << intersector.currentSurface()->radius()
	 << endl << "x " << aHelix->position()
	 << endl << "p "
	 << aHelix->momentum()
	 << " mag " << aHelix->momentum().mag()
	 << endl << "ref pt " << aHelix->referencePoint()
	 << " mass " << aHelix->mass()
	 << endl;
   }

   // Set global condition and operation.
   intersector.setGlobalCondition( m_globalCondition ) ;
   intersector.setGlobalOperation( m_globalOperation ) ;

   // Swim to first surface and fit (if applicable).
   KTMoveControl::MoveDirection moveDir = ( aDirection == kInwardFit ) ?
      KTMoveControl::kDirectionEither :
      KTMoveControl::kDirectionForward ;

   if( aDirection == kOutwardFit )
   {
      intersector.setClosestSurfaceToCurrent( moveDir ) ;
   }

   HIHelixIntersector::IntersectionStatus status =
      intersector.swimToCurrentSurface( moveDir ) ;

   // Save helix and surface at last transport.
   KalmanHelix lastHelix( *aHelix ) ;
   const HIIntersectionSurface* oldSurface = intersector.currentSurface() ;
   const HIIntersectionSurface* lastSurface = oldSurface ;

   KTMoveControl::MoveDirection swimDirection =
      aDirection == kInwardFit ?
      KTMoveControl::kDirectionBackward :
      KTMoveControl::kDirectionForward ;

   DABoolean fitFailed ;

   int lastNHits = 0 ;
   int nHitsOnTrack = 0 ;
   int nHitsRemoved = 0 ;

   nHitsOnTrack = aHelix->hitsOnTrack().size() ;
   nHitsRemoved += nHitsOnTrack ;

   // Save the pointer to the current surface if it has a hit.
   if( nHitsOnTrack > 0 )
   {
      lastSurface = intersector.currentSurface() ;
      lastHelix = *aHelix ;
   }

   if( nHitsRemoved < nHitsToRemove )
   {
      do
      {
	 status = intersector.swimToNextIntersection( swimDirection ) ;

	 fitFailed =
	    status == HIHelixIntersector::kGlobalOperationError ||
	    status == HIHelixIntersector::kSurfaceOperationError ;

	 lastNHits = nHitsOnTrack ;
	 nHitsOnTrack = aHelix->hitsOnTrack().size() ;
	 nHitsRemoved += nHitsOnTrack - lastNHits ;

	 // Save the pointer to the current surface if it has a hit.
	 if( nHitsOnTrack - lastNHits > 0 )
	 {
	    lastSurface = intersector.currentSurface() ;
	    lastHelix = *aHelix ;
	 }
      }
      while( status != HIHelixIntersector::kNoMoreSurfaces &&
	     !fitFailed &&
	     nHitsRemoved < nHitsToRemove &&
	     aHelix->errorMatrixOK() ) ;
   }

   // ~~~~~~~~~~~ Reset operations ~~~~~~~~~~~

   if( m_fitOperation != 0 ) m_fitOperation->setExecuteForward() ;
   if( m_fitSiOperation != 0 ) m_fitSiOperation->setExecuteForward() ;
   if( m_noFitOperation != 0 ) m_noFitOperation->setExecuteForward() ;
   if( m_noFitSiOperation != 0 ) m_noFitSiOperation->setExecuteForward() ;

   msdedxItr = m_msdedxOperations->begin() ;
   msdedxEnd = m_msdedxOperations->end() ;

   for( ; msdedxItr != msdedxEnd ; ++msdedxItr )
   {
      ( *msdedxItr )->setReverseTransport( false ) ;
   }

   addHitItr = m_addHitOperations->begin() ;
   addHitEnd = m_addHitOperations->end() ;

   for( ; addHitItr != addHitEnd ; ++addHitItr )
   {
      ( *addHitItr )->setRemoveHitsFromFit( false ) ;
   }

   updateCondItr = m_updateParamsConditions->begin() ;
   updateCondEnd = m_updateParamsConditions->end() ;

   for( ; updateCondItr != updateCondEnd ; ++updateCondItr )
   {
      ( *updateCondItr )->setIgnoreNumberOfHits( false ) ;
   }

   //~~~~~~~~~~~~ Reset surfaces ~~~~~~~~~~~~~~~

   for( surfaceItr = surfaces.begin() ;
	surfaceItr != surfaceEnd ;
	++surfaceItr )
   {
      ( *surfaceItr )->setUseReverseHitOrder( false ) ;
   }

   //~~~~~~~~~~~~ Finish up ~~~~~~~~~~~~~~~

   if( !fitFailed && aHelix->errorMatrixOK() )
   {
      *aHelix = lastHelix ;
      aHelix->setLastHitSurface( lastSurface ) ;
      aHelix->updateTrackParameters() ;
      aHelix->setFitFailed( false ) ;
      return aHelix ;
   }
   else
   {
      delete aHelix ;
      return 0 ;
   }
}
  

KTHelix*
KalmanTrackFitter::fittedTrack(
   const TRSeedTrack* theSeed,
   Frame& theFrame,
   const STL_VECTOR(
      HIDRSurfaceFactory::DRHitAndDriftDistance )* aDRVector,
   const STL_VECTOR( const CalibratedCathodeHit* )* aCathodeVector,
   const STL_VECTOR( const CalibratedSVRphiHit* )* aRphiVector,
   const STL_VECTOR( const CalibratedSVZHit* )* aZVector,
   GeV aMass,
   DABoolean aExtractHitsIfNull )
{
   // Reset inward seed.
   if( m_deleteInwardFit )
   {
      delete m_inwardFit ;
   }
   m_deleteInwardFit = true ;
   m_inwardFit = 0 ;

   // Make a non-const copy of the seed track.
   KalmanHelix* aHelix = new KalmanHelix( *theSeed, aMass ) ;
   KalmanHelix copyOfOriginal = *aHelix ;

   GeV pmag2 = aHelix->momentum().mag2() ;
#if _IEEE
   if( pmag2 == 0. || IsNANorINF( pmag2 ) )
#else
   if( pmag2 == 0. )
#endif
   {
      if( m_inwardFit == 0 )
      {
	 m_inwardFit = new KalmanHelix( *theSeed, aMass ) ;
      }
      m_inwardFit->setFitFailed( true ) ;
      aHelix->setFitFailed( true ) ;
      return aHelix ;
   }

   // m_fitOperation is nonnull if makeConditionsAndOperations()
   // has been called.
   if( m_fitOperation == 0 )
   {
      makeConditionsAndOperations() ;
   }

   // Generate surfaces for entire detector, including hit surfaces.
   STL_VECTOR( HIIntersectionSurface* ) surfaces =
      generateSurfaces( theSeed, theFrame,
			aDRVector, aCathodeVector, aRphiVector, aZVector,
			aMass, aExtractHitsIfNull ) ;

   if( surfaces.size() == 0 )
   {
      if( m_inwardFit == 0 )
      {
	 m_inwardFit = new KalmanHelix( *theSeed, aMass ) ;
      }
      m_inwardFit->setFitFailed( true ) ;
      aHelix->setFitFailed( true ) ;
      return aHelix ;
   }


   //~~~~~~~~~ Set up track parameter constraints, if any ~~~~~~~~~~

   DABoolean parametersFixed = false ;

//    // TEMPORARY!!!
//    m_fitConstraint->setConstraintParametersOne( *aHelix ) ;
//    m_fixParameter[ HIHelix::kD0 ] = true ;
//    m_fixParameter[ HIHelix::kZ0 ] = true ;

   // If no constraints other than fixing parameters are to be applied,
   // then simply remove the free parameters from the helix.  Otherwise,
   // fixing the parameters is applied after the unconstrained fit as
   // another constraint.  This is to avoid inverting a singular matrix
   // in solving the constraint.
   m_fixParamsConstraint->clearFixedParameters() ;

   for( int i = HIHelix::kCurvature ; i <= HIHelix::kZ0 ; ++i )
   {
      if( m_fixParameter[ i ] )
      {
	 m_fixParamsConstraint->setParameterToFix( i ) ;
	 parametersFixed = true ;
      }
   }

   if( parametersFixed )
   {
      m_fixParamsConstraint->setConstraintParametersOne( *aHelix ) ;

      if( m_fitConstraint != 0 )
      {
	 m_compositeConstraint->recalculateNumberOfConstraints() ;
      }
   }


   //~~~~~~~~~~~~ Instantiate a new HIHelixIntersector. ~~~~~~~~~~~~~~~

   HIHelixIntersector intersector( surfaces,
//				   HIHelixIntersector::kIncreasingRadius,
				   HIHelixIntersector::kDecreasingRadius,
				   aHelix ) ;

   // TEMPORARY!!!!
//   intersector.setAttemptCurling( false ) ;

   if( m_printDiagnostics )
   {
      // This is false by default.
      intersector.setPrintDiagnostics( true ) ;

      report( DEBUG, kFacilityString )
	 << "r "
	 << intersector.currentSurface()->radius()
	 << endl << "x " << aHelix->position()
	 << endl << "p "
	 << aHelix->momentum()
	 << " mag " << aHelix->momentum().mag()
	 << endl << "ref pt " << aHelix->referencePoint()
	 << " mass " << aHelix->mass()
	 << endl;
   }

   // Set global condition and operation.
   intersector.setGlobalCondition( m_globalCondition ) ;
   intersector.setGlobalOperation( m_globalOperation ) ;

   //~~~~~~~~~~~~~~~~~~~~~~ Fit the track. ~~~~~~~~~~~~~~~~~~~~~~~

   // Do inward fit first.  This is to avoid failures due to rangeout.
   // The seed track parameters are dominated by the drift chamber, where
   // energy loss in the beampipe and Si have already taken place.
   // So, swimming a track out from the origin using the lower momentum
   // may cause a track to range out when it shouldn't have.  Therefore,
   // start with the inward fit, and use that as the seed for the outward fit.

   // Based on smoothed residuals, discard hit
   // with largest pull above threshold.  Iterate until hit list stabilizes
   // or m_maxHitDel hits have been discarded.

   // Find radius of outermost surface with hit and swim there for inward fit.
   // This won't work for curlers, should we ever fit them as single tracks.
      

   // Form vector of HICalibratedHits.
   STL_VECTOR( HICalibratedHit* ) hiHitsOnSeed ;
   const HIIntersectionSurface* outermostHitSurface = 0 ;
   const HIIntersectionSurface* secondOutermostHitSurface = 0 ;

   STL_VECTOR( HIIntersectionSurface* )::const_iterator itr =
      m_hitSurfaces->begin() ;
   STL_VECTOR( HIIntersectionSurface* )::const_iterator end =
      m_hitSurfaces->end() ;

   for( ; itr != end ; ++itr )
   {
      ( *itr )->addHICalibratedHitsToVector( hiHitsOnSeed ) ;

      if( outermostHitSurface == 0 ||
	  ( *itr )->radius() > outermostHitSurface->radius() )
      {
	 secondOutermostHitSurface = outermostHitSurface ;
	 outermostHitSurface = *itr ;
      }
   }

   // Clear the HICalibratedHits -- if the previous inward fit failed, they
   // will be left in an uncleared state.
   // Apply track- and mass-dependent corrections to the hits on surfaces.
   STL_VECTOR( HICalibratedHit* )::iterator hiitr = hiHitsOnSeed.begin() ;
   STL_VECTOR( HICalibratedHit* )::iterator hiend = hiHitsOnSeed.end() ;

   for( ; hiitr != hiend ; ++hiitr )
   {
      ( *hiitr )->setInfoOnTrack( false ) ;
      ( *hiitr )->clear() ;
      ( *hiitr )->applyTrackCorrections( *aHelix ) ;

      if( !m_keepPreviousHitList )
      {
	 ( *hiitr )->setUseInFit( true ) ;
      }
   }


   // Loop until hit list stabilizes.
   int imax = m_keepPreviousHitList ? 0 : m_maxHitDel ;

   if( !m_keepPreviousHitList )
   {
      // Reset quantities for the Kalman quality object.
      m_nHitsDropped = 0 ;
      m_nHitsFlipped = 0 ;

      // Could more than imax hits have been deleted?
      m_moreBadHits = false ;
      // If so, what is the residual pull of the worst hit?
      m_pullOfNextWorstHit = 0. ;
   }

   DABoolean refit = true ;
   for( int iHitDel = 0 ; iHitDel <= imax ; ++iHitDel )
   {
      if( !refit )
      {
	 break ;
      }

      if( m_printDiagnostics )
      {
	 cout << "Starting iteration "
	      << m_nHitsDropped + m_nHitsFlipped + 1 << endl ;
      }

      // Reset the HIHelixIntersector.
      intersector.resetSurfacePointer() ;

      // Reset HICalibratedHits and clear the hits on track.
      STL_VECTOR( HIFitHelix::HitAndLinkData )::iterator hitItr =
	 aHelix->hitsOnTrack().begin() ;
      STL_VECTOR( HIFitHelix::HitAndLinkData )::iterator hitEnd =
	 aHelix->hitsOnTrack().end() ;

      for( ; hitItr != hitEnd ; ++hitItr )
      {
	 hitItr->hiCalibratedHit()->clear() ;
      }

      if( m_inwardFit != 0 )
      {
	 hitItr = m_inwardFit->hitsOnTrack().begin() ;
	 hitEnd = m_inwardFit->hitsOnTrack().end() ;

	 for( ; hitItr != hitEnd ; ++hitItr )
	 {
	    hitItr->hiCalibratedHit()->clear() ;
	 }

	 m_inwardFit->reset() ;
      }


      //~~~~~~~~~~~~ First perform inward fit.~~~~~~~~~~~~

      // Clear previous fit info, reset error matrix, and do fit.

      // If hits are being added to the track, the input track aHelix will
      // be at the right starting point.  In this case, first do the fit
      // away from the center of the track: inward fit for hits added to
      // the inside, outward fit for this added to the outside.  This should
      // be the fit returned by the function, but it will be stored in
      // m_inwardFit (which is actually the outward fit if m_inputTrackInfo
      // is kAddHitsToInputTrackOutside).  So, later on, m_inwardFit and
      // aHelix need to be swapped.

      DABoolean fitFailed ;

      if( m_inputTrackInfo == kNoHitsOnInputTrack ||
	  m_inputTrackInfo == kAddHitsToInputTrackInside )
      {
	 // First, swim track to radius of outermost hit.
	 if( ( outermostHitSurface == 0 ||
	       aHelix->moveToRadius( outermostHitSurface->radius(),
				     KTMoveControl::kDirectionForward )
	       != KTMoveControl::kMoveOK ||
	       !intersector.setCurrentSurface( outermostHitSurface ) ) &&
	     ( secondOutermostHitSurface == 0 ||
	       aHelix->moveToRadius( secondOutermostHitSurface->radius(),
				     KTMoveControl::kDirectionForward )
	       != KTMoveControl::kMoveOK ||
	       !intersector.setCurrentSurface( secondOutermostHitSurface ) ) )
	 {
	    if( m_inwardFit == 0 )
	    {
	       m_inwardFit = new KalmanHelix( copyOfOriginal ) ;
	    }
	    else
	    {
	       *m_inwardFit = copyOfOriginal ;
	    }

	    m_inwardFit->setFitFailed( true ) ;

	    *aHelix = copyOfOriginal ;
	    aHelix->setFitFailed( true ) ;
	    return aHelix ;
	 }

	 fitFailed = !fitTrackOneDirection( intersector,
					    aHelix,
					    kInwardFit,
					    m_inwardFitsAtLastHit) ;
      }
      else
      {
	 fitFailed = !fitTrackOneDirection( intersector,
					    aHelix,
					    kOutwardFit ) ;
      }

      if( !fitFailed )
      {
	 // Transport the helix back to the origin in the direction that
	 // minimizes the z-distance from the IP.
	 if( !m_inwardFitsAtLastHit &&
	     ( m_inputTrackInfo == kNoHitsOnInputTrack ||
	       m_inputTrackInfo == kAddHitsToInputTrackInside ) )
	 {
	    aHelix->moveToReferencePoint(
	       HepPoint3D( 0., 0., 0. ),
	       KTMoveControl::kDirectionBackward ) ;
	 }

	 if( m_inwardFit == 0 )
	 {
	    m_inwardFit = new KalmanHelix( *aHelix ) ;
	 }
	 else
	 {
	    *m_inwardFit = *aHelix ;
	 }

	 m_inwardFit->updateTrackParameters() ;
	 m_inwardFit->setFitFailed( false ) ;


	 // Bad hits (especially in stereo layers) will tend to swing the
	 // dip angle wildly.  Since the z-parameters are not updated
	 // throughout the fit, before calling updateTrackParameters(), they
	 // will be the same as the seed track parameters.

	 // If t has strayed too far from the seed, then use the pre-updated
	 // track as the inward seed.

	 double err2t =
	    aHelix->newErrorMatrix()( KTHelix::kCotTheta, KTHelix::kCotTheta );
	 if( err2t <= 0. )
	 {
	    err2t =
	       aHelix->errorMatrix()( KTHelix::kCotTheta, KTHelix::kCotTheta );
	 }

	 double deltaT =
	    fabs( aHelix->trackParameterCorrections()( KTHelix::kCotTheta ) ) ;

	 // Initial error on t = sqrt(5) = 2.24.
	 DABoolean updateOutwardFit = ( deltaT*deltaT ) < 25. * err2t ||
	    deltaT < 3. ;

	 if( updateOutwardFit )
	 {
	    aHelix->updateTrackParameters() ;
	 }

	 // aHelix has its reset() function called in fitTrackOneDirection().
      }
      else
      {
	 if( m_inwardFit == 0 )
	 {
	    m_inwardFit = new KalmanHelix( copyOfOriginal ) ;
	 }
	 else
	 {
	    *m_inwardFit = copyOfOriginal ;
	 }

	 m_inwardFit->setFitFailed( true ) ;

	 // Return aHelix to original and non-Kalman transport it to the
	 // radius of the HIHelixIntersector's current surface.
	 *aHelix = copyOfOriginal ;

	 // No need for transport now that inward is done first.
//	 aHelix->moveToRadius( intersector.currentSurface()->radius() ) ;
      }

      // Reset the initial helix to current helix.
      aHelix->resetInitialHelix() ;

      // Don't clear the HICalibratedHits if inward fit fails because
      // discarding a hit with a bad smoothed residual may fix the fit next
      // time around.


      //~~~~~~~~~~~~~~~~~ Now do outward fit.~~~~~~~~~~~~~~~~~

      if( m_inputTrackInfo == kNoHitsOnInputTrack ||
	  m_inputTrackInfo == kAddHitsToInputTrackInside )
      {
	 fitFailed = !fitTrackOneDirection( intersector,
					    aHelix,
					    kOutwardFit ) ;
      }
      else
      {
	 fitFailed = !fitTrackOneDirection( intersector,
					    aHelix,
					    kInwardFit,
					    m_inwardFitsAtLastHit ) ;
      }

      if( fitFailed )
      {
	 *aHelix = copyOfOriginal ;
	 aHelix->setFitFailed( true ) ;

         //Store the link data of the inward fit if that succeeded
	 STL_VECTOR( HIFitHelix::HitAndLinkData )::iterator hitItr =
	    m_inwardFit->hitsOnTrack().begin() ;
	 STL_VECTOR( HIFitHelix::HitAndLinkData )::iterator hitEnd =
	    m_inwardFit->hitsOnTrack().end() ;         

	 for( ; hitItr != hitEnd ; ++hitItr )
	 {
	    hitItr->takeOwnershipOfLinkData() ;
	 }

	 return aHelix ;
      }

      aHelix->updateTrackParameters() ;
      aHelix->setFitFailed( false ) ;

      // If this is inward fit, transport the helix back to the origin in
      // the direction that minimizes the z-distance from the IP.
      if( !m_inwardFitsAtLastHit &&
	  m_inputTrackInfo == kAddHitsToInputTrackOutside )
      {
	 aHelix->moveToReferencePoint( HepPoint3D( 0., 0., 0. ),
				       KTMoveControl::kDirectionBackward ) ;
      }

      //~~~~~~~~ Calculate smoothed residuals and their errors.~~~~~~~~~

      if( m_printDiagnostics )
      {
	
	int tmpId = -999;
	if ( theSeed != 0 ) 
	{
	  tmpId = theSeed->identifier();
	}

	 cout << "id " << tmpId
	      << " mass " << aHelix->mass() << " iter " << iHitDel ;

	 if( m_inwardFit != 0 )
	 {
	    cout << " chisq/ndof out " << m_inwardFit->chisq()
		 << "/" << m_inwardFit->ndof() ;
	 }
	 if( aHelix != 0 )
	 {
	    cout << " in " << aHelix->chisq() << "/" << aHelix->ndof() << endl;
	 }
      }

      // This block is executed only if m_keepPreviousHitList is false.
//       if( iHitDel < imax )
      {
	 double maxPull = 0. ;
	 HICalibratedHit* worstHit = 0 ;

	 STL_VECTOR( HICalibratedHit* )::iterator hitItr =hiHitsOnSeed.begin();
	 STL_VECTOR( HICalibratedHit* )::iterator hitEnd = hiHitsOnSeed.end() ;

	 for( ; hitItr != hitEnd ; ++hitItr )
	 {
	    ( *hitItr )->calculateSmoothedResidualAndError() ;	    

	    if( ( *hitItr )->useInFit() &&
		( *hitItr )->residualsSmoothed() )
	    {
	       double resid = ( *hitItr )->smoothedResidual() ;
	       double error = ( *hitItr )->smoothedResidualError() ;
	       double pull = fabs( resid / error ) ;

	       if( m_printDiagnostics )
	       {
		  cout << ( *hitItr )->calibratedHit()->deviceType()
		       << " hit ID "
		       << ( *hitItr )->calibratedHit()->identifier()
		       << " resid/error " << resid << "/" << error
		       << " pull " << pull
		       << " avg of " << ( *hitItr )->numberOfResiduals()
		       << endl;
	       }

	       if( pull > maxPull )
	       {
		  maxPull = pull ;
		  worstHit = *hitItr ;
	       }
	    }
	 }

	 //~~~~ Discard the worst hit if its pull is above the threshold.~~~~~
	 if( worstHit != 0 && maxPull > m_maxHitPull )
	 {
	    // This block is executed only if m_keepPreviousHitList is false.
	    if( iHitDel < imax )
	    {
	       // First try flipping the drift distance, if applicable.
	       if( worstHit->reverseMeasurementSign() )
	       {
		  // Increment counter if hits *could* have been flipped.
		  if( !m_keepPreviousHitList )
		  {
		     ++m_nHitsFlipped ;
		  }

		  if( m_printDiagnostics )
		  {
		     cout << "TrackFitter flipped drift sign "
			  << *( worstHit->calibratedHit());
		  }
	       }
	       else  // Otherwise, discard the hit.
	       {
		  // Increment counter if hits *could* have been dropped.
		  if( !m_keepPreviousHitList )
		  {
		     ++m_nHitsDropped ;
		  }

		  if( m_printDiagnostics )
		  {

		     cout << "TrackFitter removed hit "
			  << *( worstHit->calibratedHit());
		  }

		  worstHit->setUseInFit( false ) ;
	       }

	       *aHelix = copyOfOriginal ;
	    }
	    else if( !m_keepPreviousHitList )
	    {
	       // To be stored in Kalman quality object.
	       m_moreBadHits = true ;
	       m_pullOfNextWorstHit = maxPull ;
	    }
	 }
	 else
	 {
	    // Hit list has stabilized -- break out of fit loop.
	    refit = false ;
	 }
      }
   }
   

   // Store info to be transfered to quality objects later.
   aHelix->setNumberHitsDropped(  m_nHitsDropped ) ;
   aHelix->setNumberHitsFlipped(  m_nHitsFlipped ) ;
   aHelix->setFitQualityInfo(     m_moreBadHits, m_pullOfNextWorstHit ) ;

   m_inwardFit->setNumberHitsDropped(  m_nHitsDropped ) ;
   m_inwardFit->setNumberHitsFlipped(  m_nHitsFlipped ) ;
   m_inwardFit->setFitQualityInfo(     m_moreBadHits, m_pullOfNextWorstHit ) ;

   // Set the fits' link data residual and error to the smoothed quantities.
   // Also copy link data from HICalibratedHit to link data.
   STL_VECTOR( HIFitHelix::HitAndLinkData )::iterator hitItr =
      aHelix->hitsOnTrack().begin() ;
   STL_VECTOR( HIFitHelix::HitAndLinkData )::iterator hitEnd =
      aHelix->hitsOnTrack().end() ;

   for( ; hitItr != hitEnd ; ++hitItr )
   {
      hitItr->calculateSmoothedResidualAndError() ;
      hitItr->takeOwnershipOfLinkData() ;
   }

   if( m_inwardFit != 0 )
   {
      hitItr = m_inwardFit->hitsOnTrack().begin() ;
      hitEnd = m_inwardFit->hitsOnTrack().end() ;

      for( ; hitItr != hitEnd ; ++hitItr )
      {
	 hitItr->calculateSmoothedResidualAndError() ;
	 hitItr->takeOwnershipOfLinkData() ;
      }
   }

   // Clear the HICalibratedHits.
   hiitr = hiHitsOnSeed.begin() ;
   hiend = hiHitsOnSeed.end() ;

   for( ; hiitr != hiend ; ++hiitr )
   {
      ( *hiitr )->setInfoOnTrack( false ) ;
      ( *hiitr )->clear() ;
   }


   //~~~~~~~~~~~~~~~~~~~~~~~~ Apply fit constraints ~~~~~~~~~~~~~~~~~~~~~~~~~

   if( m_fitConstraint != 0 )
   {
      if( parametersFixed )
      {
	 m_compositeConstraint->applyConstraint( aHelix ) ;
	 m_compositeConstraint->applyConstraint( m_inwardFit ) ;
      }
      else
      {
	 m_fitConstraint->applyConstraint( aHelix ) ;
	 m_fitConstraint->applyConstraint( m_inwardFit ) ;
      }
   }
   else if( parametersFixed )
   {
      m_fixParamsConstraint->applyConstraint( aHelix ) ;
      m_fixParamsConstraint->applyConstraint( m_inwardFit ) ;
   }


   if( m_printDiagnostics )
   {
      Hep3Vector mom = aHelix->momentum() ;

      cout
	 << "At exit, mass " << aHelix->mass()
	 << " id " << theSeed->identifier() << ": "
	 << "\n" << "x " << aHelix->position()
	 << "\n" << "p " << mom.mag()
	 << " / costheta " << mom.z() / mom.mag()
	 << "\n" << "chi2/ndof "
	 << aHelix->chisq() << "/"
	 << aHelix->ndof() << "\n"
	 << "params "
	 << aHelix->trackParameters()[0] << " "
	 << aHelix->trackParameters()[1] << " "
	 << aHelix->trackParameters()[2] << " "
	 << aHelix->trackParameters()[3] << " "
	 << aHelix->trackParameters()[4] << " " << "\n"
	 << "errors "
	 << sqrt( aHelix->errorMatrix()[0][0] ) << " "
	 << sqrt( aHelix->errorMatrix()[1][1] ) << " "
	 << sqrt( aHelix->errorMatrix()[2][2] ) << " "
	 << sqrt( aHelix->errorMatrix()[3][3] ) << " "
	 << sqrt( aHelix->errorMatrix()[4][4] ) << " " << endl ;
      cout << aHelix->errorMatrix()
	 << endl ;

      mom = m_inwardFit->momentum() ;

      cout
	 << "At origin, mass " << m_inwardFit->mass()
	 << " id " << theSeed->identifier() << ": "
	 << "\n" << "x " << m_inwardFit->position()
	 << "\n" << "p " << mom.mag()
	 << " / costheta " << mom.z() / mom.mag()
	 << "\n" << "chi2/ndof "
	 << m_inwardFit->chisq() << "/"
	 << m_inwardFit->ndof() << "\n"
	 << "params "
	 << m_inwardFit->trackParameters()[0] << " "
	 << m_inwardFit->trackParameters()[1] << " "
	 << m_inwardFit->trackParameters()[2] << " "
	 << m_inwardFit->trackParameters()[3] << " "
	 << m_inwardFit->trackParameters()[4] << " " << "\n"
	 << "errors "
	 << sqrt( m_inwardFit->errorMatrix()[0][0] ) << " "
	 << sqrt( m_inwardFit->errorMatrix()[1][1] ) << " "
	 << sqrt( m_inwardFit->errorMatrix()[2][2] ) << " "
	 << sqrt( m_inwardFit->errorMatrix()[3][3] ) << " "
	 << sqrt( m_inwardFit->errorMatrix()[4][4] ) << " " << endl ;
      cout << m_inwardFit->errorMatrix()
	 << endl ;

      cout << "# hits dropped " << m_nHitsDropped
	   << ", # hits flipped " << m_nHitsFlipped
	   << ", more bad hits " << ( m_moreBadHits ? 1 : 0 )
	   << "/" << m_pullOfNextWorstHit << endl;
   }

   // Swap m_inwardFit and aHelix if hits were added to a prefit track.
   if( m_inputTrackInfo == kAddHitsToInputTrackInside ||
       m_inputTrackInfo == kAddHitsToInputTrackOutside )
   {
      delete aHelix ;
      aHelix = m_inwardFit ;
      m_inwardFit = 0 ;
   }

   return aHelix ;
}


// Returns false if fit failed.
DABoolean
KalmanTrackFitter::fitTrackOneDirection( HIHelixIntersector& aIntersector,
					 KalmanHelix* aHelix,
					 FitDirection aDirection,
					 DABoolean aReturnTrackAtLastHit )
   const
{
   // Reset the helix.  Also resets infoOnTrack for each HICalibratedHit.
   aHelix->reset() ;

   // Reset the surfaces.
   aIntersector.resetSurfaces() ;

   Meters oneRevolution = M_PI / fabs( aHelix->curvature() ) *
      sqrt( 1. + ( aHelix->cotTheta()*aHelix->cotTheta() ) ) ;

   if( m_inputTrackInfo == kNoHitsOnInputTrack ||
       ( m_inputTrackInfo == kAddHitsToInputTrackInside &&
	 aDirection == kOutwardFit ) ||
       ( m_inputTrackInfo == kAddHitsToInputTrackOutside &&
	 aDirection == kInwardFit ) )
   {
      // Initialize error matrix.
      HepSymMatrix errorMatrix( 5, 0 ) ;
      errorMatrix( HIHelix::kCurvature, HIHelix::kCurvature ) = 0.5 ;
      errorMatrix( HIHelix::kPhi0, HIHelix::kPhi0 )           = 0.1 ;
      errorMatrix( HIHelix::kD0, HIHelix::kD0 )               = 0.1 ;
      errorMatrix( HIHelix::kCotTheta, HIHelix::kCotTheta )   = 5. ;
      errorMatrix( HIHelix::kZ0, HIHelix::kZ0 )               = 5. ;
      aHelix->setErrorMatrix( errorMatrix ) ;
   }


   if( m_printDiagnostics )
   {
      report( DEBUG, kFacilityString )
	 << "r "
	 << aIntersector.currentSurface()->radius()
	 << endl << "x " << aHelix->position()
	 << endl << "p "
	 << aHelix->momentum()
	 << endl << "ref pt "
	 << aHelix->referencePoint()
	 << " mass " << aHelix->mass()
	 << endl;
      report( DEBUG, kFacilityString )
	 << "init pos/mom " << aHelix->position() << " / "
	 << aHelix->momentum() << endl
	 << "arc length " << aHelix->lastArcLength() << endl << endl ;
   }

   // If hits are being added on the inside, the first swim should be inward
   // or else the energy loss and error matrix will be wrong.

   // Swim to first surface and fit (if applicable).
   KTMoveControl::MoveDirection moveDir = ( aDirection == kInwardFit ) ?
      KTMoveControl::kDirectionEither :
      KTMoveControl::kDirectionForward ;

   if( m_inputTrackInfo == kNoHitsOnInputTrack &&
       m_inputTrackInfo == kAddHitsToInputTrackOutside )
   {
      moveDir = ( aDirection == kInwardFit ) ?
	 KTMoveControl::kDirectionEither : KTMoveControl::kDirectionForward ;
   }
   else if( m_inputTrackInfo == kAddHitsToInputTrackInside )
   {
      moveDir = ( aDirection == kInwardFit ) ?
	 KTMoveControl::kDirectionBackward : KTMoveControl::kDirectionForward ;
   }

   // Use setClosestSurfaceToCurrentByRadius() for pass2 running because
   // it is much faster, and the slower version is unnecessary, since the
   // outward fit will always start outside the Si, where all the surfaces
   // are concentric cylinders.
   if( m_inputTrackInfo == kNoHitsOnInputTrack &&
       aDirection == kOutwardFit )
   {
      aIntersector.setClosestSurfaceToCurrentByRadius( moveDir ) ;
   }
   else if( m_inputTrackInfo == kAddHitsToInputTrackInside ||
	    m_inputTrackInfo == kAddHitsToInputTrackOutside )
   {
      aIntersector.setClosestSurfaceToCurrent( moveDir ) ;
   }


   HIHelixIntersector::IntersectionStatus status =
      aIntersector.swimToCurrentSurface( moveDir ) ;

   // Keep track of total arc length transported during fit.
   DABoolean fitFailed = false ;

   // Save the surface pointer at the last hit.
   const HIIntersectionSurface* lastHitSurface = 0 ;

   // Swim through surfaces until an error occurs.
   KTMoveControl::MoveDirection swimDirection =
      aDirection == kInwardFit ?
      KTMoveControl::kDirectionBackward :
      KTMoveControl::kDirectionForward ;

   KalmanHelix lastHitHelix( *aHelix ) ;
   int nHitsOnTrack = 0 ;
   int lastNHits = 0 ;

   unsigned int loopCounter = 0 ;

   do
   {
      const HIIntersectionSurface* oldSurface = aIntersector.currentSurface() ;

      // Save the pointer to the current surface if it has a hit.
      if( nHitsOnTrack - lastNHits > 0 &&
	  ( aDirection == kOutwardFit || aReturnTrackAtLastHit ) )
      {
	 lastHitSurface = oldSurface ;
	 lastHitHelix = *aHelix ;
      }
      lastNHits = nHitsOnTrack ;

      // Proceed to next surface.
      status = aIntersector.swimToNextIntersection( swimDirection ) ;

      // Prevent infinite loops.
      if( ++loopCounter > 100000 ||
	  ( status != HIHelixIntersector::kIntersectionOK &&
	    aIntersector.currentSurface() == oldSurface ) )
      {
	 break ;
      }

      fitFailed =
	 status == HIHelixIntersector::kGlobalOperationError ||
	 status == HIHelixIntersector::kSurfaceOperationError ;

      // Place the fitted helix at the last hit whether or not it has been
      // discarded -- if this track is used as the seed for the inward fit,
      // this hit may not be discarded.
      nHitsOnTrack = aHelix->hitsOnTrack().size() ;
   }
   while( status != HIHelixIntersector::kNoMoreSurfaces &&
	  fabs( aHelix->totalArcLength() ) < oneRevolution &&
	  !fitFailed ) ;

   if( aDirection == kOutwardFit || aReturnTrackAtLastHit )
   {
      *aHelix = lastHitHelix ;
      aHelix->setLastHitSurface( lastHitSurface ) ;
      return aIntersector.setCurrentSurface( lastHitSurface ) ;
   }

   return !fitFailed ;
}


void
KalmanTrackFitter::generateSiSurfaceCache( Frame& theFrame )
{
   // Generate the cache of sensor boundaries if necessary.
   if( m_siCacheFilled || m_noFitSiOperation->numberOfChildren() == 0 )
   {
      return ;
   }

   // First, clear the existing cache.
   STL_VECTOR( HIIntersectionSurface* )::iterator itr = m_siCache->begin() ;
   STL_VECTOR( HIIntersectionSurface* )::iterator end = m_siCache->end() ;
   for( ; itr != end ; ++itr )
   {
      if( ( *itr )->isComposite() )
      {
	 ( ( HICompositeSurface* )( *itr ) )->deleteConditions() ;
      }
      delete *itr ;
   }
   m_siCache->clear() ;

   // Generate the Si sensor, diamond Vs, and carbon fiber tube boundaries.
   HISiSurfaceFactory siFactory( theFrame ) ;
   if( siFactory.geometryAvailable() &&
       m_noFitSiOperation->numberOfChildren() > 0 )
   {
      siFactory.generateLadderBoundarySurfaces( m_noFitSiOperation,
						m_noFitSiOperation );
      siFactory.transferSurfacesTo( *m_siCache ) ;
   }

   m_siCacheFilled = true ;
}

void
KalmanTrackFitter::generateIRSurfaceCache( Frame& theFrame )
{
   // Generate the cache of IR surfaces if necessary.
   if( m_irCacheFilled || m_noFitOperation->numberOfChildren() == 0 )
   {
      return ;
   }

   // First, clear the existing cache.
   STL_VECTOR( HIIntersectionSurface* )::iterator itr = m_irCache->begin() ;
   STL_VECTOR( HIIntersectionSurface* )::iterator end = m_irCache->end() ;
   for( ; itr != end ; ++itr )
   {
      if( ( *itr )->isComposite() )
      {
	 ( ( HICompositeSurface* )( *itr ) )->deleteConditions() ;
      }
      delete *itr ;
   }
   m_irCache->clear() ;

   // IR cylinders.
   HIIRSurfaceFactory irFactory( theFrame ) ;
   if( irFactory.geometryAvailable() )
   {
      irFactory.generateSurfaces( *m_irCache, m_noFitOperation ) ;
   }

   m_irCacheFilled = true ;
}

void
KalmanTrackFitter::generateDRSurfaceCache( Frame& theFrame )
{
   // Generate the cache of DR surfaces if necessary.
   if( m_drCacheFilled || m_noFitOperation->numberOfChildren() == 0 )
   {
      return ;
   }

   // First, clear the existing cache.
   STL_VECTOR( HIIntersectionSurface* )::iterator itr = m_drCache->begin() ;
   STL_VECTOR( HIIntersectionSurface* )::iterator end = m_drCache->end() ;
   for( ; itr != end ; ++itr )
   {
      if( ( *itr )->isComposite() )
      {
	 ( ( HICompositeSurface* )( *itr ) )->deleteConditions() ;
      }
      delete *itr ;
   }
   m_drCache->clear() ;

   // DR inner tube.
   HIDRSurfaceFactory drFactory( theFrame,
				 m_useDRAxialHits,
				 m_useDRStereoUHits,
				 m_useDRStereoVHits,
				 m_correctDRHits ) ;

   if( drFactory.geometryAvailable() )
   {
      drFactory.generateInnerTubeCylinders( *m_drCache, m_noFitOperation ) ;
      m_drCacheFilled = true ;
   }
}


STL_VECTOR( HIIntersectionSurface* )
KalmanTrackFitter::generateSurfaces(
   const TRSeedTrack* theSeed,
   Frame& theFrame,
   const STL_VECTOR(
      HIDRSurfaceFactory::DRHitAndDriftDistance )* aDRVector,
   const STL_VECTOR( const CalibratedCathodeHit* )* aCathodeVector,
   const STL_VECTOR( const CalibratedSVRphiHit* )* aRphiVector ,
   const STL_VECTOR( const CalibratedSVZHit* )* aZVector,
   GeV aMass,
   DABoolean aExtractHitsIfNull )
{
   if( !m_keepPreviousHitList )
   {
      generateHitSurfaces( theSeed, theFrame,
			   aDRVector, aCathodeVector, aRphiVector, aZVector,
			   aMass, aExtractHitsIfNull ) ;
   }


   // Copy surfaces to the vector to be passed to HIHelixIntersector.
   STL_VECTOR( HIIntersectionSurface* ) surfaces = *m_hitSurfaces ;

   // Generate the surface caches.
   generateSiSurfaceCache( theFrame ) ;
   generateIRSurfaceCache( theFrame ) ;
   generateDRSurfaceCache( theFrame ) ;

   // Merge the cached surfaces with the surfaces with hits.
   // If the Si geometry is available, then the first four surfaces
   // in the list will be the Si composite surfaces, whether or not
   // there are any Si hits.
   STL_VECTOR( HIIntersectionSurface* )::iterator cacheItr =m_siCache->begin();
   STL_VECTOR( HIIntersectionSurface* )::iterator cacheEnd = m_siCache->end() ;

   if( m_siHitSurfacesAtFrontOfList )
   {
      int siHitSurfaceCounter = 0 ;

      for( ; cacheItr != cacheEnd ; ++cacheItr )
      {
	 // Use an index to add to begin instead of incrementing the
	 // iterator because as non-composite surfaces get merged into
	 // the list, the vector may overflow it memory allotment, in
	 // which case, it gets copied to another larger chunk of memory.
	 // But when this happens, the pointer to the current composite
	 // surface in the list no longer points to a valid object.
	 STL_VECTOR( HIIntersectionSurface* )::iterator surfItr =
	    surfaces.begin() + siHitSurfaceCounter ;

	 if( !( *cacheItr )->isComposite() || !( *surfItr )->isComposite() )
	 {
	    surfaces.push_back( *cacheItr ) ;
	 }
	 else if( !m_siSurfacesMerged )
	 {
	    ( ( HICompositeSurface* )( *surfItr ) )->addChildren(
	       *( ( HICompositeSurface* ) *cacheItr ) ) ;

	    ++siHitSurfaceCounter ;
	 }
      }

      m_siSurfacesMerged = true ;
   }
   else
   {
      for( ; cacheItr != cacheEnd ; ++cacheItr )
      {
	 surfaces.push_back( *cacheItr ) ;
      }
   }

   // Merge the IR cache.
   cacheItr = m_irCache->begin();
   cacheEnd = m_irCache->end() ;
   for( ; cacheItr != cacheEnd ; ++cacheItr )
   {
      surfaces.push_back( *cacheItr ) ;
   }

   // Merge the DR cache.
   cacheItr = m_drCache->begin();
   cacheEnd = m_drCache->end() ;
   for( ; cacheItr != cacheEnd ; ++cacheItr )
   {
      surfaces.push_back( *cacheItr ) ;
   }

   if( m_printDiagnostics )
   {
      STL_VECTOR( HIIntersectionSurface* )::const_iterator surfaceItr =
	 surfaces.begin() ;
      STL_VECTOR( HIIntersectionSurface* )::const_iterator lastSurface =
	 surfaces.end() ;

      for( ; surfaceItr != lastSurface ; ++surfaceItr )
      {
	 ( *surfaceItr )->setPrintDiagnostics( true ) ;
      }
   }

   if( m_fitOperation != 0 )
   {
      m_fitOperation->setPrintDiagnostics( m_printDiagnostics ) ;
   }

   if( m_fitSiOperation != 0 )
   {
      m_fitSiOperation->setPrintDiagnostics( m_printDiagnostics ) ;
   }

   if( m_noFitOperation != 0 )
   {
      m_noFitOperation->setPrintDiagnostics( m_printDiagnostics ) ;
   }

   if( m_noFitSiOperation != 0 )
   {
      m_noFitSiOperation->setPrintDiagnostics( m_printDiagnostics ) ;
   }

   if( m_globalCondition != 0 )
   {
      m_globalCondition->setPrintDiagnostics( m_printDiagnostics ) ;
   }

   if( m_globalOperation != 0 )
   {
      m_globalOperation->setPrintDiagnostics( m_printDiagnostics ) ;
   }

   return surfaces ;
}

void
KalmanTrackFitter::generateHitSurfaces(
   const TRSeedTrack* theSeed,
   Frame& theFrame,
   const STL_VECTOR(
      HIDRSurfaceFactory::DRHitAndDriftDistance )* aDRVector,
   const STL_VECTOR( const CalibratedCathodeHit* )* aCathodeVector,
   const STL_VECTOR( const CalibratedSVRphiHit* )* aRphiVector ,
   const STL_VECTOR( const CalibratedSVZHit* )* aZVector,
   GeV aMass,
   DABoolean aExtractHitsIfNull )
{
   // Don't regenerate the cache only if the arguments are the same as
   // when called from fitTrack().  Otherwise, the hits to be fitted may
   // have been changed by the user.
   if( theSeed == m_hitSurfacesTrack &&
       aDRVector == 0 && aCathodeVector == 0 &&
       aRphiVector == 0 && aZVector == 0 &&
       aExtractHitsIfNull == true )
   {
      return ;
   }

   // Delete the conditions and the hit surfaces.
   STL_VECTOR( HIIntersectionSurface* )::iterator itr = m_hitSurfaces->begin();
   STL_VECTOR( HIIntersectionSurface* )::iterator end = m_hitSurfaces->end() ;

   for( ; itr != end ; ++itr )
   {
      ( *itr )->deleteCondition() ;
      delete *itr ;
   }
   m_hitSurfaces->clear() ;
   m_siSurfacesMerged = false ;

   // Generate Si surfaces for all sensors with hits.
   HISiSurfaceFactory siFactory( theFrame ) ;

   m_siHitSurfacesAtFrontOfList = false ;

   if( aRphiVector != 0 || aZVector != 0 || aExtractHitsIfNull )
   {
      if( siFactory.geometryAvailable() )
      {
	 STL_VECTOR( const CalibratedSVRphiHit* ) svrHits ;
	 STL_VECTOR( const CalibratedSVZHit* ) svzHits ;

         if( aRphiVector == 0 )
         {
            if( aExtractHitsIfNull && m_useSVRPhiHits )
            {
	       // Catch exceptions from extracting SVR hits on track.
	       try
	       {
		  siFactory.getSVRHits( theSeed,
					svrHits,
					m_svrHitUsageTag,
					m_svrHitProdTag ) ;
	       }
	       catch( NO_TABLE_EXCEPTION( CalibratedSVRphiHit )& aException )
	       {
		  // Exception is caught if specified by user.  Otherwise,
		  // it is rethrown.
		  if( m_continueIfHitsMissing )
		  {
		     report( WARNING, kFacilityString )
			<< "Continuing without SVRphi hits." << endl;
		  }
		  else
		  {
		     // This rethrows the exception currently being handled.
		     throw ;
		  }
	       }
	       catch( DAExceptionBase& aException )
	       {
		  // Exception is caught if specified by user.  Otherwise,
		  // it is rethrown.
		  if( m_continueIfAnyException )
		  {
		     report( WARNING, kFacilityString )
			<< "Continuing despite exception: \n "
			<< aException.what() << endl ;
		  }
		  else
		  {
		     throw ;
		  }
	       }
            }
         }
         else
         {
            svrHits = *aRphiVector ;
         }

         if( aZVector == 0 )
         {
            if( aExtractHitsIfNull && m_useSVZHits )
            {
	       // Catch exceptions from extracting SVZ hits on track.
	       try
	       {
		  siFactory.getSVZHits( theSeed,
					svzHits,
					m_svzHitUsageTag,
					m_svzHitProdTag ) ;
	       }
	       catch( NO_TABLE_EXCEPTION( CalibratedSVZHit )& aException )
	       {
		  // Exception is caught if specified by user.  Otherwise,
		  // it is rethrown.
		  if( m_continueIfHitsMissing )
		  {
		     report( WARNING, kFacilityString )
			<< "Continuing without SVZ hits." << endl;
		  }
		  else
		  {
		     // This rethrows the exception currently being handled.
		     throw ;
		  }
	       }
	       catch( DAExceptionBase& aException )
	       {
		  // Exception is caught if specified by user.  Otherwise,
		  // it is rethrown.
		  if( m_continueIfAnyException )
		  {
		     report( WARNING, kFacilityString )
			<< "Continuing despite exception: \n "
			<< aException.what() << endl ;
		  }
		  else
		  {
		     throw ;
		  }
	       }
            }
         }
         else
         {
            svzHits = *aZVector ;
         }

	 if( svrHits.size() > 0 || svzHits.size() > 0 )
	 {
	    siFactory.generateCenterSurfacesWithHits( svrHits,
						      svzHits,
						      m_fitSiOperation,
						      m_forceSVRFittingWeight,
						      m_svrFittingWeight,
						      m_forceSVZFittingWeight,
						      m_svzFittingWeight ) ;
	    siFactory.transferSurfacesTo( *m_hitSurfaces ) ;
	    m_siHitSurfacesAtFrontOfList = true ;
	 }
      }
   }

   // Generate surfaces for DR layers up to the last layer with a hit.
   if( aDRVector != 0 || aExtractHitsIfNull )
   {
      HIDRSurfaceFactory drFactory( theFrame,
				    m_useDRAxialHits,
				    m_useDRStereoUHits,
				    m_useDRStereoVHits,
				    m_correctDRHits ) ;

      if( drFactory.geometryAvailable() )
      {
	 STL_VECTOR( HIDRSurfaceFactory::DRHitAndDriftDistance ) drHitInfos ;
	 int maxHitDRLayer = 0 ;

         if( aDRVector == 0 )
         {
	    // Catch exceptions from extracting DR hits on track.
	    try
	    {
	       maxHitDRLayer =
		  drFactory.getDRHits( theSeed,
				       drHitInfos,
				       m_drHitUsageTag,
				       m_drHitProdTag,
				       true // fittable hits only
		     ) ;
	    }
	    catch( NO_TABLE_EXCEPTION( CalibratedDRHit )& aException )
	    {
	       // Exception is caught if specified by user.  Otherwise,
	       // it is rethrown.
	       if( m_continueIfHitsMissing )
	       {
		  report( WARNING, kFacilityString )
		     << "Continuing without DR hits." << endl;
	       }
	       else
	       {
		  // This rethrows the exception currently being handled.
		  throw ;
	       }
	    }
	    catch( DAExceptionBase& aException )
	    {
	       // Exception is caught if specified by user.  Otherwise,
	       // it is rethrown.
	       if( m_continueIfAnyException )
	       {
		  report( WARNING, kFacilityString )
		     << "Continuing despite exception: \n "
		     << aException.what() << endl ;
	       }
	       else
	       {
		  throw ;
	       }
	    }
         }
         else
         {
	    // Find the largest DR layer with a hit.
	    STL_VECTOR(
	       HIDRSurfaceFactory::DRHitAndDriftDistance )::const_iterator
	       itr = aDRVector->begin() ;
	    STL_VECTOR(
	       HIDRSurfaceFactory::DRHitAndDriftDistance )::const_iterator
	       end = aDRVector->end() ;

	    for( ; itr != end ; ++itr )
	    {
	       if( itr->calibratedDRHit()->layer() > maxHitDRLayer )
	       {
		  maxHitDRLayer = itr->calibratedDRHit()->layer() ;
	       }
	    }

	    // Copy the user-provided vector.
            drHitInfos = *aDRVector ;
         }

	 if( drHitInfos.size() > 0 )
	 {
	    drFactory.generateAllSurfacesMulti( *m_hitSurfaces,
						drHitInfos,
						m_fitOperation,
						m_noFitOperation,
						ADRSenseWireStore::kFirstLayer,
						maxHitDRLayer,
						true, // check dir switch
						m_forceDRFittingWeight,
						m_drFittingWeight) ;
	 }
	 else if( m_noFitOperation->numberOfChildren() > 0 )
	 {
	    drFactory.generateSurfacesWithoutHits( *m_hitSurfaces,
						   m_noFitOperation ) ;
	 }
      }
   }

   // Generate surfaces for cathode hits.
   if( ( aCathodeVector != 0 || aExtractHitsIfNull ) && m_useDRCathodeHits )
   {
      HICathodeSurfaceFactory cathodeFactory( theFrame ) ;

      if( cathodeFactory.geometryAvailable() )
      {
	 STL_VECTOR( const CalibratedCathodeHit* ) cathodeHits ;

         if( aCathodeVector == 0 )
         {
	    // Catch exceptions from extracting cathode hits on track.
	    try
	    {
	       cathodeFactory.getCathodeHits( theSeed,
					      cathodeHits,
					      m_cathodeHitUsageTag,
					      m_cathodeHitProdTag ) ;
	    }
	    catch( NO_TABLE_EXCEPTION( CalibratedCathodeHit )& aException )
	    {
	       // Exception is caught if specified by user.  Otherwise,
	       // it is rethrown.
	       if( m_continueIfHitsMissing )
	       {
		  report( WARNING, kFacilityString )
		     << "Continuing without cathode hits." << endl;
	       }
	       else
	       {
		  // This rethrows the exception currently being handled.
		  throw ;
	       }
	    }
	    catch( DAExceptionBase& aException )
	    {
	       // Exception is caught if specified by user.  Otherwise,
	       // it is rethrown.
	       if( m_continueIfAnyException )
	       {
		  report( WARNING, kFacilityString )
		     << "Continuing despite exception: \n "
		     << aException.what() << endl ;
	       }
	       else
	       {
		  throw ;
	       }
	    }
         }
         else
         {
            cathodeHits = *aCathodeVector ;
         }

	 if( cathodeHits.size() > 0 )
	 {
	    cathodeFactory.generateCylinderWithHits(
	       *m_hitSurfaces,
	       cathodeHits,
	       m_fitOperation,
	       m_forceCathodeFittingWeight,
	       m_cathodeFittingWeight,
	       m_scaleCathodeFittingWeight,
	       m_cathodeFittingWeightScale ) ;
	 }
      }
   }

   m_hitSurfacesTrack = theSeed ;
}

//
// const member functions
//
DABoolean
KalmanTrackFitter::event( Frame& theFrame ) const
{
   // Cast away the const on *this.
   KalmanTrackFitter* thisNonConst = const_cast< KalmanTrackFitter* >( this ) ;

   // Get the record from the frame
   const Record& theRecord = theFrame.record( Stream::kEvent ) ;

   // Filter the Seed Tracks
   thisNonConst->filterSeeds( theRecord ) ;

   DABoolean success;

   // Do fitting if there are seed tracks
   if ( ( 0 != seedTracks() ) && ( seedTracks()->size() > 0 ) )
   {
      // Loop over seed tracks.
      for( STL_VECTOR( const TRSeedTrack* )::iterator seedIter =
              seedTracks()->begin() ;
           seedIter != seedTracks()->end() ;
           ++seedIter )
      {
	 thisNonConst->m_keepPreviousHitList = false ;

	 if( m_hitListFromOneHypo )
	 {
	    if( success = thisNonConst->fitTrack( *seedIter,
						  theFrame,
						  m_hitListHypo ) )
	    {
	       thisNonConst->m_keepPreviousHitList = true ;
	    }
	 }

         // Loop over hypotheses.
	 STL_VECTOR( DBCandidate::Hypo )::const_iterator hypoItr =
	    m_hyposToFit->begin() ;
	 STL_VECTOR( DBCandidate::Hypo )::const_iterator hypoEnd =
	    m_hyposToFit->end() ;

	 for( ; hypoItr != hypoEnd ; ++hypoItr )
         {
            if( !hypoFilled( DBCandidate::Hypo( *hypoItr ) ) &&
		DBCandidate::Hypo( *hypoItr ) != m_hitListHypo )
            {
               success = thisNonConst->fitTrack(
		  *seedIter,
		  theFrame,
		  DBCandidate::Hypo( *hypoItr ) ) ;
            }
         }
      }
   }
   else
   {
      success = true;   // Fitter didn't fail, just no tracks to fit
   }


   // Set flags for fitted hypotheses.
   if( m_hitListFromOneHypo )
   {
      thisNonConst->setHypoFilled( DBCandidate::inwardHypo(  m_hitListHypo ) );
      thisNonConst->setHypoFilled( DBCandidate::outwardHypo( m_hitListHypo ) );
   }

   STL_VECTOR( DBCandidate::Hypo )::const_iterator hypoItr =
      m_hyposToFit->begin() ;
   STL_VECTOR( DBCandidate::Hypo )::const_iterator hypoEnd =
      m_hyposToFit->end() ;

   for( ; hypoItr != hypoEnd ; ++hypoItr )
   {
      thisNonConst->setHypoFilled( DBCandidate::inwardHypo(  *hypoItr ) ) ;
      thisNonConst->setHypoFilled( DBCandidate::outwardHypo( *hypoItr ) ) ;
   }

   thisNonConst->m_keepPreviousHitList = false ;
   return success;
}


DABoolean KalmanTrackFitter::event( Frame& theFrame, HypoType aHypo ) const
{
   DABoolean success = TrackFitter::event( theFrame, aHypo ) ;

   KalmanTrackFitter* thisNonConst =
      const_cast< KalmanTrackFitter* >( this ) ;
   thisNonConst->setHypoFilled( DBCandidate::inwardHypo(  aHypo ) ) ;
   thisNonConst->setHypoFilled( DBCandidate::outwardHypo( aHypo ) ) ;

   return success;
}

//
// static member functions
//
