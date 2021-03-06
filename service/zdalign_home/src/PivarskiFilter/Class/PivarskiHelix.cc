// -*- C++ -*-
//
// Package:     PivarskiFilter
// Module:      PivarskiHelix
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Fri Oct  2 18:55:20 EDT 1998
// $Id: PivarskiHelix.cc,v 1.18 2002/04/27 17:42:37 cleo3 Exp $
//
// Revision history
//
// $Log: PivarskiHelix.cc,v $
// Revision 1.18  2002/04/27 17:42:37  cleo3
// Compiler now writes the copy constructor, destructor and assigment operator.  Replaced pointer to vector with actual vector.
//
// Revision 1.17  2000/08/16 20:13:48  wsun
// Updated to new TRSeedTrackQuality.
//
// Revision 1.16  2000/06/09 00:41:07  wsun
// Updated to new HIFitHelix::HitAndLinkData.
//
// Revision 1.15  2000/02/03 18:48:51  wsun
// Added reset() and included use of HICalibratedHit.
//
// Revision 1.14  1999/12/15 21:14:41  wsun
// Sped up matrix manipulations & copy ctor, added transportCorrectionsDedx()
//
// Revision 1.13  1999/11/22 18:48:56  wsun
// Added updateHitsOnTrack().
//
// Revision 1.12  1999/11/03 03:30:37  wsun
// Changed Hep3Vector --> HepVector3D to match KTHelix changes.
//
// Revision 1.11  1999/10/08 23:12:29  wsun
// Added data member for HIIntersectionSurface with the last hit on the track.
//
// Revision 1.10  1999/08/30 21:50:15  wsun
// Added numberOfNewHitsInFit().
//
// Revision 1.9  1999/08/27 20:40:34  wsun
// Added resetHitsOnTrack() and otherErrorMatrix().
//
// Revision 1.8  1999/08/03 22:28:42  wsun
// Added implementation of transportCorrections(...).
//
// Revision 1.7  1999/07/07 16:46:41  wsun
// Added mechanism for applying constraints in track fitter.
//
// Revision 1.6  1999/06/02 23:07:42  wsun
// Improved low-momentum fitting.
//
// Revision 1.5  1999/05/09 21:38:34  wsun
// Updated code to new Lattice interface.
//
// Revision 1.4  1999/04/30 23:05:37  wsun
// Zero transport matrix at each move.
//
// Revision 1.3  1999/04/29 20:16:42  wsun
// Additional fixes for Solaris.
//
// Revision 1.2  1999/04/28 22:58:38  wsun
// Fixes for Solaris.
//
// Revision 1.1  1999/04/27 22:20:51  wsun
// Major reorganization and structural changes.
//
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "PivarskiFilter/PivarskiHelix.h"

#include "CLHEP/Geometry/Vector3D.h"
#include "CLHEP/Geometry/Point3D.h"

// STL classes
#include <vector>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "PivarskiFilter.PivarskiHelix" ;
const string PivarskiHelix::kHelixType = "PivarskiHelix" ;
//
// static data member definitions
//

//
// constructors and destructor
//
PivarskiHelix::PivarskiHelix()
   : HIFitHelix(),
     m_lastTransportMatrix(                   HepMatrix( 5, 5, 1 ) ),
     m_trackParameterCorrections(             HepVector( 5, 0 ) ),
     m_newTrackParameterCorrections(          HepVector( 5, 0 ) ),
     m_transportTrackParameterCorrections(    false ),
     m_transportNewTrackParameterCorrections( false ),
     m_newErrorMatrix( HepSymMatrix(          5, 0 ) ),
     m_transportNewErrorMatrix(               false ),
     m_deltaChisq(                            0. ),
     m_updateRphiTrackParameters(             false ),
     m_updateZTrackParameters(                false ),
     m_newHitsOnTrack(),
     m_lastHitSurface(                        0 ),
     m_moreBadHits(                           false ),
     m_pullOfNextWorstHit(                    0. )
{
}


PivarskiHelix::PivarskiHelix( const KTHelix& aHelix,
			  GeV aMass,
			  Meters anArcLength,
			  Identifier aId,
			  const HepMatrix& aTransportMatrix,
			  DABoolean aUpdateRphiParameters,
			  DABoolean aUpdateZParameters )
   : HIFitHelix( aHelix, aMass, false, anArcLength, aId ),
     m_lastTransportMatrix(                   aTransportMatrix ),
     m_trackParameterCorrections(             HepVector( 5, 0 ) ),
     m_newTrackParameterCorrections(          HepVector( 5, 0 ) ),
     m_transportTrackParameterCorrections(    false ),
     m_transportNewTrackParameterCorrections( false ),
     m_newErrorMatrix(                        HepSymMatrix( 5, 0 ) ),
     m_transportNewErrorMatrix(               false ),
     m_deltaChisq(                            0. ),
     m_updateRphiTrackParameters(             aUpdateRphiParameters ),
     m_updateZTrackParameters(                aUpdateZParameters ),
     m_newHitsOnTrack(),
     m_lastHitSurface(                        0 ),
     m_moreBadHits(                           false ),
     m_pullOfNextWorstHit(                    0. )
{
}

//
// member functions
//

void
PivarskiHelix::setNewHitsOnTrack( const STL_VECTOR( HitAndLinkData )& aHits )
{
   m_newHitsOnTrack = aHits ;
}

void
PivarskiHelix::updateHitsOnTrack()
{
   STL_VECTOR( HitAndLinkData )::iterator hitItr = m_newHitsOnTrack.begin() ;
   STL_VECTOR( HitAndLinkData )::iterator hitEnd = m_newHitsOnTrack.end() ;

   for( ; hitItr != hitEnd ; ++hitItr )
   {
      if( hitItr->hiCalibratedHit() != 0 )
      {
	 addHitOnTrack( *hitItr ) ;
      }
   }
}

void
PivarskiHelix::updateFitInfo()
{
   if( m_transportNewTrackParameterCorrections )
   {
      m_trackParameterCorrections = m_newTrackParameterCorrections ;
   }

   if( m_transportNewErrorMatrix )
   {
      setErrorMatrix( m_newErrorMatrix ) ;
   }

   incrementChisq( m_deltaChisq ) ;

   updateHitsOnTrack() ;
   clearFitInfoChanges() ;

   if( m_updateRphiTrackParameters )
   {
      setCurvature( curvature() + m_trackParameterCorrections( kCurvature ) ) ;
      m_trackParameterCorrections( kCurvature ) = 0. ;

      setPhi0( phi0() + m_trackParameterCorrections( kPhi0 ) ) ;
      m_trackParameterCorrections( kPhi0 ) = 0. ;

      setD0( d0() + m_trackParameterCorrections( kD0 ) ) ;
      m_trackParameterCorrections( kD0 ) = 0. ;
   }

   if( m_updateZTrackParameters )
   {
      setCotTheta( cotTheta() + m_trackParameterCorrections( kCotTheta ) ) ;
      m_trackParameterCorrections( kCotTheta ) = 0. ;

      setZ0( z0() + m_trackParameterCorrections( kZ0 ) ) ;
      m_trackParameterCorrections( kZ0 ) = 0. ;
   }

   m_transportTrackParameterCorrections =
      !( m_updateRphiTrackParameters && m_updateZTrackParameters ) ;
}


void
PivarskiHelix::updateTrackParameters()
{
   setTrackParameters( trackParameters() + m_trackParameterCorrections );

   // Faster than m_trackParameterCorrections = HepVector( 5, 0 ) ;
   m_trackParameterCorrections *= 0. ;

   m_transportTrackParameterCorrections = false ;
}


void
PivarskiHelix::clearFitInfoChanges()
{
   // Faster than m_newTrackParameterCorrections = HepVector( 5, 0 ) ;
   m_newTrackParameterCorrections *= 0. ;
   m_transportNewTrackParameterCorrections = false ;

   // Faster then m_newErrorMatrix = HepSymMatrix( 5, 0 ) ;
   m_newErrorMatrix *= 0. ;
   m_transportNewErrorMatrix = false ;

   m_deltaChisq = 0. ;
   m_newHitsOnTrack.clear() ;
}

void
PivarskiHelix::resetHitsOnTrack()
{
   HIFitHelix::resetHitsOnTrack() ;
   m_newHitsOnTrack.clear() ;
}

void
PivarskiHelix::reset()
{
   HIFitHelix::reset() ;
   resetHitsOnTrack() ;
   clearFitInfoChanges() ;
   m_updateRphiTrackParameters = false ;
   m_updateZTrackParameters = false ;
}

KTHelix::MoveStatus
PivarskiHelix::moveToReferencePoint( const HepPoint3D& aPoint,
				   const MoveDirection aDirection )
{
   KTHelix oldHelix( *this, false ) ;
   KTHelix::MoveStatus status = HIHelix::moveToReferencePoint( aPoint,
							       aDirection );

   // If the transport was successful, then the error matrix transport
   // was also successful, which means the transport matrix is well
   // defined.
   if( status == KTMoveControl::kMoveOK && 
       ( m_transportTrackParameterCorrections ||
	 m_transportNewTrackParameterCorrections ||
	 m_transportNewErrorMatrix ) )
   {
      m_lastTransportMatrix = HepMatrix( 5, 5, 0 ) ;
      transportMatrix( oldHelix, m_lastTransportMatrix ) ;
      transportCorrections() ;
   }
   return status ;
}

KTHelix::MoveStatus
PivarskiHelix::moveToLine( const HepPoint3D& aPoint,
			 const HepVector3D& aDirCosine,
			 const MoveDirection aDirection )
{
   KTHelix oldHelix( *this, false ) ;
   KTHelix::MoveStatus status = HIHelix::moveToLine( aPoint,
						     aDirCosine,
						     aDirection ) ;
   if( status == KTMoveControl::kMoveOK && 
       ( m_transportTrackParameterCorrections ||
	 m_transportNewTrackParameterCorrections ||
	 m_transportNewErrorMatrix ) )
   {
      m_lastTransportMatrix = HepMatrix( 5, 5, 0 ) ;
      transportMatrix( oldHelix, m_lastTransportMatrix ) ;
      transportCorrections() ;
   }
   return status ;
}

KTHelix::MoveStatus
PivarskiHelix::moveToZPosition( const Meters aZPosition,
			      const MoveDirection aDirection )
{
   KTHelix oldHelix( *this, false ) ;
   KTHelix::MoveStatus status = HIHelix::moveToZPosition( aZPosition,
							  aDirection ) ;
   if( status == KTMoveControl::kMoveOK && 
       ( m_transportTrackParameterCorrections ||
	 m_transportNewTrackParameterCorrections ||
	 m_transportNewErrorMatrix ) )
   {
      m_lastTransportMatrix = HepMatrix( 5, 5, 0 ) ;
      transportMatrix( oldHelix, m_lastTransportMatrix ) ;
      transportCorrections() ;
   }
   return status ;
}


KTHelix::MoveStatus
PivarskiHelix:: moveToPlane( const HepPoint3D&  aPoint,
			   const HepVector3D& aNormal,
			   const MoveDirection aDirection )
{
   KTHelix oldHelix( *this, false ) ;
   KTHelix::MoveStatus status = HIHelix::moveToPlane( aPoint,
						      aNormal,
						      aDirection ) ;
   if( status == KTMoveControl::kMoveOK && 
       ( m_transportTrackParameterCorrections ||
	 m_transportNewTrackParameterCorrections ||
	 m_transportNewErrorMatrix ) )
   {
      m_lastTransportMatrix = HepMatrix( 5, 5, 0 ) ;
      transportMatrix( oldHelix, m_lastTransportMatrix ) ;
      transportCorrections() ;
   }
   return status ;
}


KTHelix::MoveStatus
PivarskiHelix::moveToRadius( const Meters aRadius,
			   const MoveDirection aDirection )
{
   KTHelix oldHelix( *this, false ) ;
   KTHelix::MoveStatus status = HIHelix::moveToRadius( aRadius,
						       aDirection ) ;
   if( status == KTMoveControl::kMoveOK && 
       ( m_transportTrackParameterCorrections ||
	 m_transportNewTrackParameterCorrections ||
	 m_transportNewErrorMatrix ) )
   {
      m_lastTransportMatrix = HepMatrix( 5, 5, 0 ) ;
      transportMatrix( oldHelix, m_lastTransportMatrix ) ;
      transportCorrections() ;
   }
   return status ;
}


KTHelix::MoveStatus
PivarskiHelix::moveToCylinder( const HepPoint3D& aCenter,
			     const HepVector3D& aDirCosine,
			     const Meters aRadius,
			     const MoveDirection aDirection )
{
   KTHelix oldHelix( *this, false ) ;
   KTHelix::MoveStatus status = HIHelix::moveToCylinder( aCenter,
							 aDirCosine,
							 aRadius,
							 aDirection ) ;
   if( status == KTMoveControl::kMoveOK && 
       ( m_transportTrackParameterCorrections ||
	 m_transportNewTrackParameterCorrections ||
	 m_transportNewErrorMatrix ) )
   {
      m_lastTransportMatrix = HepMatrix( 5, 5, 0 ) ;
      transportMatrix( oldHelix, m_lastTransportMatrix ) ;
      transportCorrections() ;
   }
   return status ;
}


KTHelix::MoveStatus
PivarskiHelix::moveByArcLength( Meters aArcLength )
{
   KTHelix oldHelix( *this, false ) ;
   KTHelix::MoveStatus status = HIHelix::moveByArcLength( aArcLength ) ;

   if( status == KTMoveControl::kMoveOK && 
       ( m_transportTrackParameterCorrections ||
	 m_transportNewTrackParameterCorrections ||
	 m_transportNewErrorMatrix ) )
   {
      m_lastTransportMatrix = HepMatrix( 5, 5, 0 ) ;
      transportMatrix( oldHelix, m_lastTransportMatrix ) ;
      transportCorrections() ;
   }
   return status ;
}

void
PivarskiHelix::transportCorrections()
{
   // m_trackParameterCorrections =
   // m_lastTransportMatrix * m_trackParameterCorrections
   if( m_transportTrackParameterCorrections )
   {
      double newCorrK    = 0. ;
      double newCorrPhi0 = 0. ;
      double newCorrD0   = 0. ;
      double newCorrT    = 0. ;
      double newCorrZ0   = 0. ;

      for( int i = 1 ; i <= kNTrackParameters ; ++i )
      {
	 double oldCorrI =  m_trackParameterCorrections( i ) ;
	 newCorrK    += m_lastTransportMatrix( kCurvature, i ) * oldCorrI ;
	 newCorrPhi0 += m_lastTransportMatrix( kPhi0, i ) * oldCorrI ;
	 newCorrD0   += m_lastTransportMatrix( kD0, i ) * oldCorrI ;
	 newCorrT    += m_lastTransportMatrix( kCotTheta, i ) * oldCorrI ;
	 newCorrZ0   += m_lastTransportMatrix( kZ0, i ) * oldCorrI ;
      }

      m_trackParameterCorrections( kCurvature ) = newCorrK ;
      m_trackParameterCorrections( kPhi0 )      = newCorrPhi0 ;
      m_trackParameterCorrections( kD0 )        = newCorrD0 ;
      m_trackParameterCorrections( kCotTheta )  = newCorrT ;
      m_trackParameterCorrections( kZ0 )        = newCorrZ0 ;
   }

   // m_newTrackParameterCorrections =
   // m_lastTransportMatrix * m_newTrackParameterCorrections
   if( m_transportNewTrackParameterCorrections )
   {
      double newCorrK    = 0. ;
      double newCorrPhi0 = 0. ;
      double newCorrD0   = 0. ;
      double newCorrT    = 0. ;
      double newCorrZ0   = 0. ;

      for( int i = 1 ; i <= kNTrackParameters ; ++i )
      {
	 double oldCorrI =  m_newTrackParameterCorrections( i ) ;
	 newCorrK    += m_lastTransportMatrix( kCurvature, i ) * oldCorrI ;
	 newCorrPhi0 += m_lastTransportMatrix( kPhi0, i ) * oldCorrI ;
	 newCorrD0   += m_lastTransportMatrix( kD0, i ) * oldCorrI ;
	 newCorrT    += m_lastTransportMatrix( kCotTheta, i ) * oldCorrI ;
	 newCorrZ0   += m_lastTransportMatrix( kZ0, i ) * oldCorrI ;
      }

      m_newTrackParameterCorrections( kCurvature ) = newCorrK ;
      m_newTrackParameterCorrections( kPhi0 )      = newCorrPhi0 ;
      m_newTrackParameterCorrections( kD0 )        = newCorrD0 ;
      m_newTrackParameterCorrections( kCotTheta )  = newCorrT ;
      m_newTrackParameterCorrections( kZ0 )        = newCorrZ0 ;
   }

   // m_newErrorMatrix = m_newErrorMatrix( m_lastTransportMatrix )
   if( m_transportNewErrorMatrix )
   {
      HepMatrix errorTransportT( kNTrackParameters, kNTrackParameters ) ;

      for( int i = 1 ; i <= kNTrackParameters ; ++i )
      {
	 for( int j = 1 ; j <= kNTrackParameters ; ++j )
	 {
	    double tmp = 0. ;
	    for( int k = 1 ; k <= kNTrackParameters ; ++k )
	    {
	       tmp += m_newErrorMatrix( i, k ) * m_lastTransportMatrix( j, k );
	    }
	    errorTransportT( i, j ) = tmp ;
	 }
      }

      for( int r = 1 ; r <= kNTrackParameters ; ++r )
      {
	 for( int s = r ; s <= kNTrackParameters ; ++s )
	 {
	    double tmp = 0. ;
	    for( int t = 1 ; t <= kNTrackParameters ; ++t )
	    {
	       tmp += errorTransportT( t, r ) * m_lastTransportMatrix( s, t );
	    }
	    m_newErrorMatrix.fast( s, r ) = tmp ;
	 }
      }
   }
}

void
PivarskiHelix::transportCorrections( const HepMatrix& aTransportMatrix )
{
   // It is faster (on Solaris, at least) to copy aTransportMatrix to
   // m_lastTransportMatrix and then call transportCorrections() than to
   // do the matrix*vector multiplication in this function.  Apparently,
   // since HepMatrix::operation()(int,int) is inlined, the compiler can
   // optimize out calls to m_lastTransportMatrix(int,int) since it is a
   // data member, but not calls to aTransportMatrix(int,int).  This makes
   // the execution almost three times faster.

   m_lastTransportMatrix = aTransportMatrix ;

   // Don't transport the new error matrix.
   DABoolean savedFlag = m_transportNewErrorMatrix ;
   m_transportNewErrorMatrix = false ;
   transportCorrections() ;
   m_transportNewErrorMatrix = savedFlag ;
}

// The track parameter transport for dE/dx only affects the curvature.
void
PivarskiHelix::transportCorrectionsDedx( const HepVector& aTransportVector )
{
   // m_trackParameterCorrections( kCurvature ) =
   // m_lastTransportMatrix * m_trackParameterCorrections( kCurvature )
   if( m_transportTrackParameterCorrections )
   {
      double newCorrK = 0. ;

      for( int i = 1 ; i <= kNTrackParameters ; ++i )
      {
	 newCorrK += aTransportVector( i ) *
	    m_trackParameterCorrections( i ) ;
      }

      m_trackParameterCorrections( kCurvature ) = newCorrK ;
   }

   // m_newTrackParameterCorrections( kCurvature ) =
   // m_lastTransportMatrix * m_newTrackParameterCorrections( kCurvature )
   if( m_transportNewTrackParameterCorrections )
   {
      double newCorrK = 0. ;

      for( int i = 1 ; i <= kNTrackParameters ; ++i )
      {
	 newCorrK += aTransportVector( i ) * 
	    m_newTrackParameterCorrections( i ) ;
      }

      m_newTrackParameterCorrections( kCurvature ) = newCorrK ;
   }
}

//
// const member functions
//

int
PivarskiHelix::numberOfNewHitsInFit() const
{
   STL_VECTOR( HitAndLinkData )::const_iterator hitItr = m_newHitsOnTrack.begin() ;
   STL_VECTOR( HitAndLinkData )::const_iterator hitEnd = m_newHitsOnTrack.end() ;

   int nUsedHits = 0 ;
   for( ; hitItr != hitEnd ; ++hitItr )
   {
      if( ( *hitItr ).linkData()->used() )
      {
	 ++nUsedHits ;
      }
   }

   return nUsedHits ;
}

//
// static member functions
//
