// -*- C++ -*-
//
// Package:     HelixIntersection
// Module:      HIHelix
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Fri Oct  2 18:55:20 EDT 1998
// $Id: HIHelix.cc,v 1.17 2002/04/27 16:56:54 cleo3 Exp $
//
// Revision history
//
// $Log: HIHelix.cc,v $
// Revision 1.17  2002/04/27 16:56:54  cleo3
// Have the compiler write the copy constructor, destructor and assignment operator
//
// Revision 1.16  2001/03/26 06:58:54  wsun
// Added errorMatrixOK().
//
// Revision 1.15  2000/08/16 20:00:57  wsun
// Updated to new TrackFitHitLink and TRTrackFitQuality.
//
// Revision 1.14  2000/06/19 05:59:41  wsun
// Removed use of TBThreeVector, TBThreePoint, and TBSymMatrix.
//
// Revision 1.13  2000/02/03 07:28:36  wsun
// Added use of HICalibratedHits.
//
// Revision 1.12  2000/01/11 20:27:28  wsun
// Added flag to indicate that HIHelix has intersected a surface.
//
// Revision 1.11  1999/12/15 20:49:33  wsun
// Sped up copy ctors.
//
// Revision 1.10  1999/11/21 22:25:04  wsun
// Got rid of string copy in helixType() to speed up code.
//
// Revision 1.9  1999/11/03 03:35:22  wsun
// Changed Hep3Vector to HepVector3D to match KTHelix changes.
//
// Revision 1.8  1999/08/30 21:35:49  wsun
// Added resetInitialHelix().
//
// Revision 1.7  1999/08/03 21:49:50  wsun
// Added copy constructor that does not copy error matrix.
//
// Revision 1.6  1999/07/07 16:41:12  wsun
// Added mechanism for applying constraints in track fitters.
//
// Revision 1.5  1999/06/24 22:24:57  wsun
// Added use of HIMagField and new SiHits.
//
// Revision 1.4  1999/06/21 17:26:46  wsun
// Added trackParameters().
//
// Revision 1.3  1999/06/18 19:25:57  wsun
// More improvements to low momentum fitting.
//
// Revision 1.2  1999/06/02 23:05:23  wsun
// Added features for low-momentum fitting.
//
// Revision 1.1  1999/04/27 22:06:25  wsun
// Major reorganization and structural changes.
//
// Revision 1.1.1.1  1998/12/07 04:58:27  wsun
// First submission.
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

#include <assert.h>

// user include files
#include "Experiment/report.h"
#include "HelixIntersection/HIHelix.h"
#include "HelixIntersection/HIMagField.h"

#include "CLHEP/Geometry/Vector3D.h"
#include "CLHEP/Geometry/Point3D.h"

// STL classes
#include <vector>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "HelixIntersection.HIHelix" ;

const string HIHelix::kHelixType = "HIHelix" ;

const int HIHelix::kNTrackParameters = 5 ;

//
// static data member definitions
//

//
// constructors and destructor
//
HIHelix::HIHelix()
   : KTHelix(),
     m_mass(                        0. ),
     m_lastArcLength(               0. ),
     m_totalArcLength(              0. ),
     m_identifier(                  0 ),
     m_updateErrorMatrix(           true ),
     m_savedCurvature(              0. ),
     m_savedEnergy(                 0. ),
     m_initHelix(                   *this ),
     m_reportFitResultsAtInitRefPt( false ),
     m_atSurface(                   false ),
     m_totalDeltaEnergy(                 0. )
{
}


HIHelix::HIHelix( const KTHelix& aHelix,
		  GeV aMass,
		  DABoolean reportFitResultsAtInitRefPt,
		  Meters anArcLength,
		  Identifier aId )
   : KTHelix( aHelix ),
     m_mass( aMass ),
     m_lastArcLength( anArcLength ),
     m_totalArcLength( anArcLength ),
     m_identifier( aId ),
     m_updateErrorMatrix( true ),
     m_savedCurvature( 0. ),
     m_savedEnergy( 0. ),
     m_initHelix( *this ),
     m_reportFitResultsAtInitRefPt( reportFitResultsAtInitRefPt ),
     m_atSurface( false ),
     m_totalDeltaEnergy( 0. )
{
}


HIHelix::HIHelix( const double aCurvature,
		  const double aPhi0,
		  const double aD0,
		  const double aCotTheta,
		  const double aZ0,
		  const HepPoint3D& aReferencePoint,
		  GeV aMass,
		  DABoolean reportFitResultsAtInitRefPt,
		  Meters anArcLength,
		  Identifier aId )
   : KTHelix( aCurvature, aPhi0, aD0, aCotTheta, aZ0, aReferencePoint ),
     m_mass( aMass ),
     m_lastArcLength( anArcLength ),
     m_totalArcLength( anArcLength ),
     m_identifier( aId ),
     m_updateErrorMatrix( true ),
     m_savedCurvature( 0. ),
     m_savedEnergy( 0. ),
     m_initHelix( *this ),
     m_reportFitResultsAtInitRefPt( reportFitResultsAtInitRefPt ),
     m_atSurface( false ),
     m_totalDeltaEnergy( 0. )
{
}

HIHelix::HIHelix( const double aCurvature,
		  const double aPhi0,
		  const double aD0,
		  const double aCotTheta,
		  const double aZ0,
		  const HepSymMatrix& aErrorMatrix,
		  const HepPoint3D& aReferencePoint,
		  GeV aMass,
		  DABoolean reportFitResultsAtInitRefPt,
		  Meters anArcLength,
		  Identifier aId )
   : KTHelix( aCurvature, aPhi0, aD0, aCotTheta, aZ0,
	      aErrorMatrix, aReferencePoint ),
   m_mass( aMass ),
   m_lastArcLength( anArcLength ),
   m_totalArcLength( anArcLength ),
   m_identifier( aId ),
   m_updateErrorMatrix( true ),
   m_savedCurvature( 0. ),
   m_savedEnergy( 0. ),
   m_initHelix( *this ),
   m_reportFitResultsAtInitRefPt( reportFitResultsAtInitRefPt ),
   m_atSurface( false ),
   m_totalDeltaEnergy( 0. )
{
}


HIHelix::HIHelix( const HIHelix& rhs, const DABoolean aCopyErrorMatrix )
   : KTHelix( rhs, aCopyErrorMatrix ),
     m_mass(              rhs.m_mass ),
     m_lastArcLength(     rhs.m_lastArcLength ),
     m_totalArcLength(    rhs.m_totalArcLength ),
     m_identifier(        rhs.m_identifier ),
     m_updateErrorMatrix( rhs.m_updateErrorMatrix ),
     m_savedCurvature(    rhs.m_savedCurvature ),
     m_savedEnergy(       rhs.m_savedEnergy ),
     m_initHelix(         rhs.m_initHelix ),
     m_reportFitResultsAtInitRefPt( rhs.m_reportFitResultsAtInitRefPt ),
     m_atSurface(         rhs.m_atSurface ),
     m_totalDeltaEnergy(  rhs.m_totalDeltaEnergy )
{
}

//
// assignment operators
//

//
// member functions
//
void
HIHelix::setTrackParameters( const HepVector& aParams )
{
   assert( aParams.num_row() == kNTrackParameters ) ;

   setCurvature( aParams( kCurvature ) ) ;
   setPhi0(      aParams( kPhi0 ) ) ;
   setD0(        aParams( kD0 ) ) ;
   setCotTheta(  aParams( kCotTheta ) ) ;
   setZ0(        aParams( kZ0 ) ) ;
}

void
HIHelix::resetInitialHelix()
{
   m_initHelix.setCurvature(      curvature() ) ;
   m_initHelix.setPhi0(           phi0() ) ;
   m_initHelix.setD0(             d0() ) ;
   m_initHelix.setCotTheta(       cotTheta() ) ;
   m_initHelix.setZ0(             z0() ) ;
   m_initHelix.setReferencePoint( referencePoint() ) ;

   if( hasNullErrorMatrix() )
   {
      m_initHelix.clearErrorMatrix() ;
   }
   else
   {
      m_initHelix.setErrorMatrix( errorMatrix() ) ;
   }

   m_totalArcLength = 0. ;
   m_totalDeltaEnergy = 0. ;
}

void
HIHelix::reset()
{
   m_lastArcLength = 0. ;
   m_totalArcLength = 0. ;
   m_updateErrorMatrix = true ;
   m_savedCurvature = 0. ;
   m_savedEnergy = 0. ;
   m_atSurface = false ;
   m_totalDeltaEnergy = 0. ;
}

KTHelix::MoveStatus
HIHelix::moveToReferencePoint( const HepPoint3D& aPoint,
			       const MoveDirection aDirection )
{
   KTHelix::MoveStatus status = KTHelix::moveToReferencePoint( aPoint,
							       m_lastArcLength,
							       aDirection ) ;
   m_totalArcLength += m_lastArcLength ;
   return status ;
}

KTHelix::MoveStatus
HIHelix::moveToLine( const HepPoint3D&  aPoint,
		     const HepVector3D& aDirCosine,
		     const MoveDirection aDirection )
{
   KTHelix::MoveStatus status = KTHelix::moveToLine( aPoint,
						     aDirCosine,
						     m_lastArcLength,
						     aDirection ) ;
   m_totalArcLength += m_lastArcLength ;
   return status ;
}

KTHelix::MoveStatus
HIHelix::moveToZPosition( const Meters aZPosition,
			  const MoveDirection aDirection )
{
   KTHelix::MoveStatus status = KTHelix::moveToZPosition( aZPosition,
							  m_lastArcLength,
							  aDirection ) ;
   m_totalArcLength += m_lastArcLength ;
   return status ;
}


KTHelix::MoveStatus
HIHelix:: moveToPlane( const HepPoint3D&  aPoint,
		       const HepVector3D& aNormal,
		       const MoveDirection aDirection )
{
   KTHelix::MoveStatus status = KTHelix::moveToPlane( aPoint,
						      aNormal,
						      m_lastArcLength,
						      aDirection ) ;
   m_totalArcLength += m_lastArcLength ;
   return status ;
}


KTHelix::MoveStatus
HIHelix::moveToRadius( const Meters aRadius,
		       const MoveDirection aDirection )
{
   KTHelix::MoveStatus status = KTHelix::moveToRadius( aRadius,
						       m_lastArcLength,
						       aDirection ) ;
   m_totalArcLength += m_lastArcLength ;
   return status ;
}


KTHelix::MoveStatus
HIHelix::moveToCylinder( const HepPoint3D& aCenter,
			 const HepVector3D& aDirCosine,
			 const Meters aRadius,
			 const MoveDirection aDirection )
{
   KTHelix::MoveStatus status = KTHelix::moveToCylinder( aCenter,
							 aDirCosine,
							 aRadius,
							 m_lastArcLength,
							 aDirection ) ;
   m_totalArcLength += m_lastArcLength ;
   return status ;
}

KTHelix::MoveStatus
HIHelix::moveByArcLength( Meters aArcLength )
{
   KTHelix::MoveStatus status = KTHelix::moveByArcLength( aArcLength ) ;

   if( status == KTMoveControl::kMoveOK )
   {
      m_lastArcLength = aArcLength ;
   }
   else
   {
      m_lastArcLength = 0. ;
   }

   m_totalArcLength += m_lastArcLength ;
   return status ;
}

//
// const member functions
//
HepVector3D
HIHelix::momentum() const
{
   return KTHelix::momentum(
      HIMagField::instance()->bfield( position() ) ) ;
}

const double kBFieldConstant = -0.0299792458;

double
HIHelix::momentumMag2() const
{
   if( curvature() == 0. )
   {
      return 0. ;

   }
   else
   {
      return sqr( 0.5 * kBFieldConstant *
		   HIMagField::instance()->bfield( position() ).z()
		   / curvature() ) *
	 ( 1. + sqr( cotTheta() ) ) ;
   }
}

GeV
HIHelix::momentumMag() const
{
   return sqrt( momentumMag2() ) ;
}


HepVector
HIHelix::trackParameters() const
{
   HepVector tmpVector( 5, 0 ) ;
   tmpVector( kCurvature ) = curvature() ;
   tmpVector( kPhi0 )      = phi0() ;
   tmpVector( kD0 )        = d0() ;
   tmpVector( kCotTheta )  = cotTheta() ;
   tmpVector( kZ0 )        = z0() ;
   return tmpVector ;
}

DABoolean
HIHelix::errorMatrixOK( const HepSymMatrix& aMatrix ) const
{
   for( int i = 1 ; i < kNTrackParameters+1 ; ++i )
   {
      if( aMatrix.fast( i, i ) < 0. )
      {
	 report( DEBUG, kFacilityString )
	    << "Error matrix diagonal element " << i << " negative." << endl ;
	 return false ;
      }
   }

   for( int j = 1 ; j < kNTrackParameters ; ++j )
   {
      for( int k = j + 1 ; k < kNTrackParameters + 1 ; ++k )
      {
	 if( aMatrix.fast( k, j ) * aMatrix.fast( k, j ) >
	     aMatrix.fast( k, k ) * aMatrix.fast( j, j ) )
	 {
	    report( DEBUG, kFacilityString )
	       << "Error matrix element ("
	       << k << "," << j << ") too large." << endl ;
	    return false ;
	 }
      }
   }

   return true ;
}

//
// static member functions
//
