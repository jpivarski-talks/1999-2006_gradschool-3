#if !defined(HELIXINTERSECTION_HIHELIX_H)
#define HELIXINTERSECTION_HIHELIX_H
// -*- C++ -*-
//
// Package:     HelixIntersection
// Module:      HIHelix
// 
// Description: A KTHelix bundled with other information pertinent to track
//              finding and fitting.
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Fri Oct  2 18:55:13 EDT 1998
// $Id: HIHelix.h,v 1.18 2001/03/27 05:29:03 wsun Exp $
//
// Revision history
//
// $Log: HIHelix.h,v $
// Revision 1.18  2001/03/27 05:29:03  wsun
// Added errorMatrixOK()
//
// Revision 1.17  2001/01/26 21:43:40  cleo3
// removed used of sqr since not available on linux
//
// Revision 1.16  2000/08/16 20:01:02  wsun
// Updated to new TrackFitHitLink and TRTrackFitQuality.
//
// Revision 1.15  2000/06/19 05:59:46  wsun
// Removed use of TBThreeVector, TBThreePoint, and TBSymMatrix.
//
// Revision 1.14  2000/02/03 07:16:56  wsun
// Added track parameter ctors.
//
// Revision 1.13  2000/01/11 20:27:31  wsun
// Added flag to indicate that HIHelix has intersected a surface.
//
// Revision 1.12  1999/12/15 20:49:04  wsun
// Added transportCorrectionsDedx(), which only changes curvatures.
//
// Revision 1.11  1999/11/21 22:25:00  wsun
// Got rid of string copy in helixType() to speed up code.
//
// Revision 1.10  1999/11/03 03:35:20  wsun
// Changed Hep3Vector to HepVector3D to match KTHelix changes.
//
// Revision 1.9  1999/08/30 21:35:51  wsun
// Added resetInitialHelix().
//
// Revision 1.8  1999/08/27 20:18:34  wsun
// Added otherErrorMatrix().
//
// Revision 1.7  1999/08/03 21:47:47  wsun
// Added transportCorrections(...).
//
// Revision 1.6  1999/07/07 16:40:15  wsun
// Added mechanism for applying constraints in track fitters.
//
// Revision 1.5  1999/06/24 22:18:22  wsun
// Added use of HIMagField and new SiHits.
//
// Revision 1.4  1999/06/21 17:26:33  wsun
// Added trackParameters().
//
// Revision 1.3  1999/06/18 19:25:35  wsun
// More improvements to low momentum fitting.
//
// Revision 1.2  1999/06/02 23:04:21  wsun
// Added features for low-momentum fitting.
//
// Revision 1.1  1999/04/27 22:03:59  wsun
// Major reorganization and structural changes.
//
// Revision 1.2  1998/12/15 00:13:17  wsun
// New interface for HIDRHitInfo.
//
// Revision 1.1.1.1  1998/12/07 04:58:20  wsun
// First submission.
//

// system include files

// user include files
#include "KinematicTrajectory/KTHelix.h"
#include "CLHEP/Matrix/Vector.h"
#include "CLHEP/Matrix/Matrix.h"

// forward declarations
#include "C++Std/fwd_string.h"
#include "STLUtility/fwd_vector.h"

class HepVector3D ;
class HepPoint3D ;
class HepSymMatrix ;


class HIHelix : public KTHelix
{
      // ---------- friend classes and functions ---------------
      
   public:
      // ---------- constants, enums and typedefs --------------
      typedef double Meters ;
      typedef double GeV ;
      typedef Count Identifier ;

      static const string kHelixType ;
      static const int kNTrackParameters ;

      // ---------- Constructors and destructor ----------------
      HIHelix();
      HIHelix( const HIHelix& );
      HIHelix( const HIHelix&, const DABoolean aCopyErrorMatrix );

      // The derivatives given by HIWireLayerCylinder and HISiSensorPlane
      // depend on the reportFitResultsAtInitRefPt flag.
      HIHelix( const KTHelix& aHelix,
	       GeV aMass = 0.13957,  // pion mass
	       DABoolean reportFitResultsAtInitRefPt = false,
	       Meters anArcLength = 0.,
	       Identifier id = 0 ) ;

      HIHelix( const double aCurvature,
               const double aPhi0,
               const double aD0,
               const double aCotTheta,
               const double aZ0,
               const HepPoint3D& aReferencePoint = HepPoint3D( 0.,0.,0. ),
	       GeV aMass = 0.13957,  // pion mass
	       DABoolean reportFitResultsAtInitRefPt = false,
	       Meters anArcLength = 0.,
	       Identifier id = 0 ) ;

      HIHelix( const double aCurvature,
               const double aPhi0,
               const double aD0,
               const double aCotTheta,
               const double aZ0,
               const HepSymMatrix& aErrorMatrix,
               const HepPoint3D& aReferencePoint = HepPoint3D( 0.,0.,0. ),
	       GeV aMass = 0.13957,  // pion mass
	       DABoolean reportFitResultsAtInitRefPt = false,
	       Meters anArcLength = 0.,
	       Identifier id = 0 ) ;

      const HIHelix& operator=( const HIHelix& );

      virtual ~HIHelix();

      // ---------- member functions ---------------------------

      // *********** IMPORTANT ************
      // Subclasses need to reimplement these two functions so that the
      // snapshot is of the right type.
      virtual HIHelix* newSnapshot() { return new HIHelix( *this ) ; }
      virtual void revertToSnapshot( const HIHelix* aSnapshot )
      {
	 if( aSnapshot->helixType() == helixType() )
	    *this = *( ( HIHelix* )aSnapshot ) ;
      }

      void setMass( GeV aMass )
      { m_mass = aMass ; }

      virtual void setTrackParameters( const HepVector& aParams ) ;

      void setLastArcLength( Meters anArcLength )
      { m_lastArcLength = anArcLength ; }

      void setTotalArcLength( Meters anArcLength )
      { m_totalArcLength = anArcLength ; }

      void setIdentifier( Count aId )
      {  m_identifier = aId ; }

      void setAtSurface( DABoolean aFlag ) { m_atSurface = aFlag ; }

      void incrementDeltaEnergy( GeV aDeltaEnergy )
      { m_totalDeltaEnergy += aDeltaEnergy ; }

      virtual void transportCorrections( const HepMatrix& aTransportMatrix ) {}

      // The track parameter transport for dE/dx only affects the curvature.
      virtual void transportCorrectionsDedx(
	 const HepVector& aTransportVector ) {}

      // Save current helix state as the initial helix.
      void resetInitialHelix() ;

      // Set all flags and accumulated quantities to initial values.
      virtual void reset() ;
	 

      // New move functions that call the KTHelix functions with
      // m_lastArcLength.  If subclasses overload these functions, they
      // should not modify the basic functionality of the KTHelix functions.
      // Some HIIntersectionsSurface classes depend on the fact that the
      // KTHelix and HIHelix transport routines move tracks identically.

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


      void setReportFitResultsAtInitRefPt( DABoolean aFlag )
      { m_reportFitResultsAtInitRefPt = aFlag ; }

      void setUpdateErrorMatrix( DABoolean aFlag )
      { m_updateErrorMatrix = aFlag ; }

      // Function for HIMultScatOperation to access the other error
      // matrices associated with the helix.
      virtual HepSymMatrix* otherErrorMatrix()
      { return 0 ; }

      void saveCurvatureAndEnergy()
      {
	 m_savedCurvature = curvature() ;
	 m_savedEnergy = sqrt( momentumMag2() +  m_mass*m_mass ) ;
      }

      void unsaveCurvatureAndEnergy()
      { m_savedCurvature = 0. ; m_savedEnergy = 0. ; }

      DABoolean otherErrorMatrixOK()
      { return otherErrorMatrix() == 0 ||
	   errorMatrixOK( *otherErrorMatrix() ) ; }

      // ---------- const member functions ---------------------

      // *********** IMPORTANT ************
      // Subclasses should reimplement helixType(), which is used to
      // test for the helix' actual type when casting base class pointers
      // into the appropriate subclass.
      virtual const string& helixType() const { return HIHelix::kHelixType ; }

      // This function uses the singleton HIMagField to get the magnetic
      // field to be passed to KTHelix::momentum( HepVector3D ).
      HepVector3D momentum() const ;

      // Return the magnitude (squared) of the momentum.
      // Faster than momentum().mag().
      double momentumMag2() const ;
      GeV momentumMag() const ;

      // This function saves time by not calling KTHelix::position(),
      // since the Bfield is not needed for the tangent unit vector.
      HepVector3D momentumUnit() const
      {
	 HepVector3D tmp( cos( phi0() ), sin( phi0() ), cotTheta() ) ;
	 return ( 1. / sqrt( 1. + cotTheta()*cotTheta() ) ) * tmp ;
      }

      // This function calls KTHelix::momentum( HepVector3D ) because the
      // above function partially overloads momentum(), making
      // KTHelix::momentum() unavailable.
      HepVector3D momentum( const HepVector3D bfield ) const
      { return KTHelix::momentum( bfield ) ; }

      GeV mass() const { return m_mass ; }
      Meters lastArcLength() const { return m_lastArcLength ; }
      Meters totalArcLength() const { return m_totalArcLength ; }
      Identifier identifier() const { return m_identifier ; }
      DABoolean atSurface() const { return m_atSurface ; }

      const KTHelix& initialHelix() const { return m_initHelix ; }

      DABoolean reportFitResultsAtInitRefPt() const
      { return m_reportFitResultsAtInitRefPt ; }

      DABoolean updateErrorMatrix() const { return m_updateErrorMatrix ; }

      HepVector trackParameters() const ;

      double savedCurvature() const { return m_savedCurvature ; }
      GeV savedEnergy() const { return m_savedEnergy ; }

      GeV totalDeltaEnergy() const { return m_totalDeltaEnergy ; }

      DABoolean errorMatrixOK() const
      { return errorMatrixOK( errorMatrix() ) ; }

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------

      // ---------- assignment operator(s) ---------------------

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------
      DABoolean errorMatrixOK( const HepSymMatrix& aMatrix ) const ;

      // ---------- data members -------------------------------
      GeV m_mass ;
      Meters m_lastArcLength ;
      Meters m_totalArcLength ;
      Identifier m_identifier ;

      DABoolean m_updateErrorMatrix ;
      double m_savedCurvature ;
      GeV m_savedEnergy ;

      KTHelix m_initHelix ;

      DABoolean m_reportFitResultsAtInitRefPt ;

      // This flag is false until the helix intersects a surface.
      DABoolean m_atSurface ;

      GeV m_totalDeltaEnergy ;

      // ---------- static data members ------------------------
};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HelixIntersection/Template/HIHelix.cc"
//#endif

#endif /* HELIXINTERSECTION_HIHELIX_H */
