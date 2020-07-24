#if !defined(KALMANFILTER_KALMANHELIX_H)
#define KALMANFILTER_KALMANHELIX_H
// -*- C++ -*-
//
// Package:     KalmanFilter
// Module:      KalmanHelix
// 
// Description: A KTHelix bundled with other information pertinent to track
//              finding and fitting.
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Fri Oct  2 18:55:13 EDT 1998
// $Id: KalmanHelix.h,v 1.18 2002/04/27 17:42:39 cleo3 Exp $
//
// Revision history
//
// $Log: KalmanHelix.h,v $
// Revision 1.18  2002/04/27 17:42:39  cleo3
// Compiler now writes the copy constructor, destructor and assigment operator.  Replaced pointer to vector with actual vector.
//
// Revision 1.17  2000/08/16 20:13:51  wsun
// Updated to new TRSeedTrackQuality.
//
// Revision 1.16  2000/06/09 00:41:12  wsun
// Updated to new HIFitHelix::HitAndLinkData.
//
// Revision 1.15  2000/02/03 18:48:28  wsun
// Added reset().
//
// Revision 1.14  1999/12/15 21:14:38  wsun
// Sped up matrix manipulations & copy ctor, added transportCorrectionsDedx()
//
// Revision 1.13  1999/11/22 18:48:53  wsun
// Added updateHitsOnTrack().
//
// Revision 1.12  1999/11/21 22:35:58  wsun
// Updated to new HIHelix interface.
//
// Revision 1.11  1999/11/03 03:30:34  wsun
// Changed Hep3Vector --> HepVector3D to match KTHelix changes.
//
// Revision 1.10  1999/10/08 23:12:27  wsun
// Added data member for HIIntersectionSurface with the last hit on the track.
//
// Revision 1.9  1999/08/30 21:50:17  wsun
// Added numberOfNewHitsInFit().
//
// Revision 1.8  1999/08/27 20:40:13  wsun
// Added resetHitsOnTrack() and otherErrorMatrix().
//
// Revision 1.7  1999/08/03 22:28:23  wsun
// Added implementation of transportCorrections(...).
//
// Revision 1.6  1999/07/07 16:46:27  wsun
// Added mechanism for applying constraints in track fitter.
//
// Revision 1.5  1999/06/18 19:27:37  wsun
// More improvements to low momentum fitting.
//
// Revision 1.4  1999/06/02 23:07:30  wsun
// Improved low-momentum fitting.
//
// Revision 1.3  1999/05/09 21:38:20  wsun
// Updated code to new Lattice interface.
//
// Revision 1.2  1999/04/29 20:16:33  wsun
// Additional fixes for Solaris.
//
// Revision 1.1  1999/04/27 22:19:52  wsun
// Major reorganization and structural changes.
//
//

// system include files
#include <vector>

// user include files

#include "HelixIntersection/HIFitHelix.h"

#include "CLHEP/Matrix/Matrix.h"
#include "CLHEP/Matrix/SymMatrix.h"
#include "CLHEP/Matrix/Vector.h"

// forward declarations
class HepVector3D ;
class HepPoint3D ;
class HIIntersectionSurface ;


class KalmanHelix : public HIFitHelix
{
      // ---------- friend classes and functions ---------------
      
   public:
      // ---------- constants, enums and typedefs --------------
      static const string kHelixType ;

      // ---------- Constructors and destructor ----------------
      KalmanHelix();
      //KalmanHelix( const KalmanHelix& );
      KalmanHelix( const KTHelix& aHelix,
		   GeV aMass = 0.13957,  // pion mass
		   Meters anArcLength = 0.,
		   Identifier id = 0,
		   const HepMatrix& aTransportMatrix = HepMatrix( 5, 5, 1 ),
		   DABoolean aUpdateRphiParameters = false,
		   DABoolean aUpdateZParameters = false ) ;

      //const KalmanHelix& operator=( const KalmanHelix& );

      //virtual ~KalmanHelix();

      // ---------- member functions ---------------------------

      // *********** IMPORTANT ************
      // Subclasses need to reimplement these two functions so that the
      // snapshot is of the right type.
      virtual HIHelix* newSnapshot() { return new KalmanHelix( *this ) ; }
      virtual void revertToSnapshot( const HIHelix* aSnapshot )
      {
	 if( aSnapshot->helixType() == helixType() )
	    *this = *( ( KalmanHelix* )aSnapshot ) ;
      }

      //~~~~~ Accessors for data members ~~~~~

      void setLastTransportMatrix( const HepMatrix& aMatrix )
      { m_lastTransportMatrix = aMatrix ; }

      void setTrackParameterCorrections( const HepVector& aVector )
      {
	 m_trackParameterCorrections = aVector ;
	 m_transportTrackParameterCorrections = true ;
      }

      void setNewTrackParameterCorrections( const HepVector& aVector )
      {
	 m_newTrackParameterCorrections = aVector ;
	 m_transportNewTrackParameterCorrections = true ;
      }

      void setNewErrorMatrix( const HepSymMatrix& aMatrix )
      {
	 m_newErrorMatrix = aMatrix ;
	 m_transportNewErrorMatrix = true ;
      }

      void setDeltaChisq( double aDeltaChisq )
      { m_deltaChisq = aDeltaChisq ; }

      void setUpdateRphiTrackParametersFlag( DABoolean aUpdate )
      { m_updateRphiTrackParameters = aUpdate ; }

      void setUpdateZTrackParametersFlag( DABoolean aUpdate )
      { m_updateZTrackParameters = aUpdate ; }

      void setNewHitsOnTrack( const STL_VECTOR( HitAndLinkData )& aHits ) ;

      void setLastHitSurface( const HIIntersectionSurface* aSurface )
      { m_lastHitSurface = aSurface ; }

      // The Kalman filter is allowed to throw away only so many hits.
      // aMoreBadHits tells whether any more hits *could* have been discarded,
      // i.e. with a smoothed residual pull larger than the cut (default 3.5).
      // If so, the pull of the next worst hit is given by the last argument.
      void setFitQualityInfo( DABoolean aMoreBadHits,
			      double    aPullOfNextWorstHit )
      {
	 m_moreBadHits        = aMoreBadHits ;
	 m_pullOfNextWorstHit = aPullOfNextWorstHit ;
      }

      //~~~~~ Functions that actually do something ~~~~~

      void updateHitsOnTrack() ;
      void updateFitInfo() ;
      void updateTrackParameters() ;
      void clearFitInfoChanges() ;
      virtual void resetHitsOnTrack() ;
      virtual void reset() ;

      // This function does NOT transport the new error matrix, which should
      // be handled separately using otherErrorMatrix().
      virtual void transportCorrections( const HepMatrix& aTransportMatrix ) ;

      // The track parameter transport for dE/dx only affects the curvature.
      virtual void transportCorrectionsDedx(
	 const HepVector& aTransportVector ) ;

      // Function for HIMultScatOperation to access the other error
      // matrices associated with the helix.
      virtual HepSymMatrix* otherErrorMatrix()
      { return ( m_transportNewErrorMatrix ? &m_newErrorMatrix : 0 ) ; }

      // New move functions that call the KTHelix functions
      // with m_lastArcLength.

      virtual
      MoveStatus moveToReferencePoint( const HepPoint3D& aPoint,
				       const MoveDirection aDirection =
				       KTMoveControl::kDirectionEither ) ;
      virtual
      MoveStatus moveToLine( const HepPoint3D&  aPoint,
			     const HepVector3D& aDirCosine,
			     const MoveDirection aDirection =
			     KTMoveControl::kDirectionEither ) ;
      virtual
      MoveStatus moveToZPosition( const Meters aZPosition,
				  const MoveDirection aDirection = 
				  KTMoveControl::kDirectionEither ) ;
      virtual
      MoveStatus moveToPlane( const HepPoint3D&  aPoint,
			      const HepVector3D& aNormal,
			      const MoveDirection aDirection =
			      KTMoveControl::kDirectionEither ) ;
      virtual
      MoveStatus moveToRadius( const Meters aRadius,
			       const MoveDirection aDirection =
			       KTMoveControl::kDirectionEither ) ;
      virtual
      MoveStatus moveToCylinder( const HepPoint3D& aCenter,
				 const HepVector3D& aDirCosine,
				 const Meters aRadius,
				 const MoveDirection aDirection =
				 KTMoveControl::kDirectionEither ) ;

      virtual
      MoveStatus moveByArcLength( Meters aArcLength ) ;


      // ---------- const member functions ---------------------

      // *********** IMPORTANT ************
      // Subclasses should reimplement helixType(), which is used to
      // test for the helix' actual type when casting base class pointers
      // into the appropriate subclass.
      virtual const string& helixType() const
      { return KalmanHelix::kHelixType ; }

      const HepMatrix& lastTransportMatrix() const
      { return m_lastTransportMatrix ; }

      const HepVector& trackParameterCorrections() const
      { return m_trackParameterCorrections ; }

      DABoolean transportTrackParameterCorrections() const
      { return m_transportTrackParameterCorrections ; }

      const HepVector& newTrackParameterCorrections() const
      { return m_newTrackParameterCorrections ; }

      const HepSymMatrix& newErrorMatrix() const
      { return m_newErrorMatrix ; }

      double deltaChisq() const
      { return m_deltaChisq ; }

      DABoolean updateRphiTrackParametersFlag() const
      { return m_updateRphiTrackParameters ; }

      DABoolean updateZTrackParametersFlag() const
      { return m_updateZTrackParameters ; }

      const STL_VECTOR( HitAndLinkData )& newHitsOnTrack() const
      { return m_newHitsOnTrack ; }

      virtual int numberOfNewHitsInFit() const ;

      const HIIntersectionSurface* lastHitSurface() const
      { return m_lastHitSurface ; }

      // Info for quality object.

      DABoolean moreBadHits() const
      { return m_moreBadHits ; }

      // This is meaningful only of moreBadHits() is true.
      double pullOfNextWorstHit() const
      { return m_pullOfNextWorstHit ; }

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------

      // ---------- assignment operator(s) ---------------------

      // ---------- private member functions -------------------
      void transportCorrections() ;

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      HepMatrix m_lastTransportMatrix ;

      HepVector m_trackParameterCorrections ;
      HepVector m_newTrackParameterCorrections ;
      DABoolean m_transportTrackParameterCorrections ;
      DABoolean m_transportNewTrackParameterCorrections ;

      HepSymMatrix m_newErrorMatrix ;
      DABoolean m_transportNewErrorMatrix ;

      double m_deltaChisq ;

      DABoolean m_updateRphiTrackParameters ;
      DABoolean m_updateZTrackParameters ;

      STL_VECTOR( HitAndLinkData ) m_newHitsOnTrack ;

      const HIIntersectionSurface* m_lastHitSurface ;

      DABoolean m_moreBadHits ;
      double    m_pullOfNextWorstHit ;

      // ---------- static data members ------------------------
};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "KalmanFilter/Template/HIFitHelix.cc"
//#endif

#endif /* KALMANFILTER_KALMANHELIX_H */
