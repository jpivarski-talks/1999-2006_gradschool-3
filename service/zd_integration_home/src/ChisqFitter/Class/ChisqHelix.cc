// -*- C++ -*-
//
// Package:     <ChisqFitter>
// Module:      ChisqHelix
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Wed Jun  9 02:11:13 EDT 1999
// $Id: ChisqHelix.cc,v 1.7 2000/01/11 20:13:56 wsun Exp $
//
// Revision history
//
// $Log: ChisqHelix.cc,v $
// Revision 1.7  2000/01/11 20:13:56  wsun
// setTrackParamsAtInitRefPt() now resets error matrix as well.
//
// Revision 1.6  1999/12/15 21:57:56  wsun
// Sped up copy ctor.
//
// Revision 1.5  1999/07/07 16:44:13  wsun
// Added mechanism for applying constraints in track fitter.
//
// Revision 1.4  1999/06/24 22:53:06  wsun
// * Added use of HIMagField and new SiHits.
// * Added ability to fix any combination of parameters in track fitting.
//
// Revision 1.3  1999/06/20 21:17:09  wsun
// Allow user to fix parameters.
//
// Revision 1.2  1999/06/18 19:16:29  wsun
// Allow user to fix parameters.
//
// Revision 1.1.1.1  1999/06/16 02:46:47  wsun
// First submission.
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

#include <assert.h>

// user include files
//#include "Experiment/report.h"
#include "ChisqFitter/ChisqHelix.h"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "ChisqFitter.ChisqHelix" ;

const string ChisqHelix::kHelixType = "ChisqHelix" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: ChisqHelix.cc,v 1.7 2000/01/11 20:13:56 wsun Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
ChisqHelix::ChisqHelix()
   : m_infoMatrix( HepSymMatrix( 5, 0 ) ),
     m_residualVector( HepVector( 5, 0 ) )
{
}

ChisqHelix::ChisqHelix( const KTHelix& aHelix,
			GeV aMass,
			DABoolean reportFitResultsAtInitRefPt,
			Meters anArcLength,
			Identifier id )
   : HIFitHelix( aHelix, aMass, reportFitResultsAtInitRefPt, anArcLength, id ),
     m_infoMatrix( HepSymMatrix( 5, 0 ) ),
     m_residualVector( HepVector( 5, 0 ) ),
     m_helixAtStart( aHelix, false ) // don't copy error matrix
{
   for( int i = 0 ; i < kZ0 + kCurvature ; ++i )
   {
      m_free[ i ] = true ;
      m_paramsFixedTo[ i ] = 0. ;
   }
}

ChisqHelix::ChisqHelix( const ChisqHelix& rhs )
   : HIFitHelix( rhs ),
     m_infoMatrix( rhs.m_infoMatrix ),
     m_residualVector( rhs.m_residualVector ),
     m_helixAtStart( rhs.m_helixAtStart )
{
   // do actual copying here; if you implemented
   // operator= correctly, you may be able to use just say      
//    *this = rhs;

     for( int i = 0 ; i < kZ0 + kCurvature ; ++i )
     {
	m_free[ i ] = rhs.m_free[ i ] ;
	m_paramsFixedTo[ i ] = rhs.m_paramsFixedTo[ i ] ;
     }
}

const ChisqHelix& ChisqHelix::operator=( const ChisqHelix& rhs )
{
  if( this != &rhs ) {
     HIFitHelix::operator=( rhs ) ;
     m_infoMatrix        = rhs.m_infoMatrix ;
     m_residualVector    = rhs.m_residualVector ;
     m_helixAtStart      = rhs.m_helixAtStart ;

     for( int i = 0 ; i < kZ0 + kCurvature ; ++i )
     {
	m_free[ i ] = rhs.m_free[ i ] ;
	m_paramsFixedTo[ i ] = rhs.m_paramsFixedTo[ i ] ;
     }
  }
  return *this;
}

ChisqHelix::~ChisqHelix()
{
}

//
// member functions
//

void
ChisqHelix::fixParameter( int aParam )
{
   if( !m_free[ aParam ] )
   {
      return ;
   }

   m_paramsFixedTo[ aParam ] = HIHelix::trackParameters()( aParam ) ;
   incrementNumberOfFreeParameters( -1 ) ;

   HepSymMatrix tmpMatrix( numberOfFreeParameters(), 0 ) ;
   HepVector    tmpVector( numberOfFreeParameters(), 0 ) ;

   int readCol = kCurvature ;
   int fillCol = kCurvature ;
   for( int col = kCurvature ; col <= kZ0 ; ++col )
   {
      if( m_free[ col ] )
      {
	 if( col != aParam )
	 {
	    tmpVector( fillCol ) = m_residualVector( readCol ) ;

	    int readRow = readCol ;
	    int fillRow = fillCol ;
	    for( int row = col ; row <= kZ0 ; ++row )
	    {
	       if( m_free[ row ] )
	       {
		  if( row != aParam )
		  {
		     tmpMatrix.fast( fillRow, fillCol ) =
			m_infoMatrix.fast( readRow, readCol ) ;
		     ++fillRow ;
		  }
		  ++readRow ;
	       }
	    }
	    ++fillCol ;
	 }
	 ++readCol ;
      }
   }

   m_free[ aParam ] = false ;
   m_infoMatrix = tmpMatrix ;
   m_residualVector = tmpVector ;
}


void
ChisqHelix::releaseParameter( int aParam )
{
   if( m_free[ aParam ] )
   {
      return ;
   }

   m_paramsFixedTo[ aParam ] = 0. ;
   m_free[ aParam ] = true ;
   incrementNumberOfFreeParameters() ;

   HepSymMatrix tmpMatrix( numberOfFreeParameters(), 0 ) ;
   HepVector    tmpVector( numberOfFreeParameters(), 0 ) ;

   int readCol = kCurvature ;
   int fillCol = kCurvature ;
   for( int col = kCurvature ;	col <= kZ0 ; ++col )
   {
      if( m_free[ col ] )
      {
	 if( col != aParam )
	 {
	    tmpVector( fillCol ) = m_residualVector( readCol ) ;

	    int readRow = readCol ;
	    int fillRow = fillCol ;
	    for( int row = col ; row <= kZ0 ; ++row )
	    {
	       if( m_free[ row ] )
	       {
		  if( row != aParam )
		  {
		     tmpMatrix.fast( fillRow, fillCol ) =
			m_infoMatrix.fast( readRow, readCol ) ;
		     ++readRow ;
		  }
		  ++fillRow ;
	       }
	    }
	    ++readCol ;
	 }
	 ++fillCol ;
      }
   }

   m_infoMatrix = tmpMatrix ;
   m_residualVector = tmpVector ;
}

void
ChisqHelix::setTrackParamsAtInitRefPt( const HepVector& aParams )
{
   assert( aParams.num_row() == numberOfFreeParameters() ) ;

   setReferencePoint( initialHelix().referencePoint() ) ;

   if( !hasNullErrorMatrix() && !initialHelix().hasNullErrorMatrix() )
   {
      setErrorMatrix( initialHelix().errorMatrix() ) ;
   }

   int readIndex = kCurvature ;

   if( m_free[ kCurvature ] )
   {
      setCurvature( aParams( readIndex ) ) ;
      ++readIndex ;
   }
   else
   {
      setCurvature( m_paramsFixedTo[ kCurvature ] ) ;
   }

   if( m_free[ kPhi0 ] )
   {
      setPhi0( aParams( readIndex ) ) ;
      ++readIndex ;
   }
   else
   {
      setPhi0( m_paramsFixedTo[ kPhi0 ] ) ;
   }

   if( m_free[ kD0 ] )
   {
      setD0( aParams( readIndex ) ) ;
      ++readIndex ;
   }
   else
   {
      setD0( m_paramsFixedTo[ kD0 ] ) ;
   }

   if( m_free[ kCotTheta ] )
   {
      setCotTheta( aParams( readIndex ) ) ;
      ++readIndex ;
   }
   else
   {
      setCotTheta( m_paramsFixedTo[ kCotTheta ] ) ;
   }

   if( m_free[ kZ0 ] )
   {
      setZ0( aParams( readIndex ) ) ;
   }
   else
   {
      setZ0( m_paramsFixedTo[ kZ0 ] ) ;
   }

   // Don't copy the error matrix.
   KTHelix tmpHelix( *this, false ) ;
   m_helixAtStart = tmpHelix ;
}

void
ChisqHelix::setErrorMatrix( const HepSymMatrix& aMatrix )
{
   assert( aMatrix.num_row() == numberOfFreeParameters() ) ;

   if( numberOfFreeParameters() == 5 )
   {
      KTHelix::setErrorMatrix( aMatrix ) ;
   }
   else
   {
      // Fill columns and rows of fixed parameters with 0.
      HepSymMatrix tmpError( 5, 0 ) ;

      int readCol = kCurvature ;
      for( int col = kCurvature ; col <= kZ0 ; ++col )
      {
	 if( m_free[ col ] )
	 {
	    int readRow = readCol ;
	    for( int row = col ; row <= kZ0 ; ++row )
	    {
	       if( m_free[ row ] )
	       {
		  tmpError.fast( row, col ) = aMatrix.fast( readRow, readCol );
		  ++readRow ;
	       }
	    }
	    ++readCol ;
	 }
      }

      KTHelix::setErrorMatrix( tmpError ) ;
   }
}

void
ChisqHelix::returnToStart()
{
   KTHelix::operator=( m_helixAtStart ) ;

   setLastArcLength( 0. ) ;
   setTotalArcLength( 0. ) ;
}

//
// const member functions
//

HepVector
ChisqHelix::freeTrackParameters() const
{
   if( numberOfFreeParameters() == 5 )
   {
      return HIHelix::trackParameters() ;
   }

   HepVector tmpVector( numberOfFreeParameters(), 0 ) ;
   int vectorIndex = kCurvature ;

   if( m_free[ kCurvature ] )
   {
      tmpVector( vectorIndex ) = curvature() ;
      ++vectorIndex ;
   }
   if( m_free[ kPhi0 ] )
   {
      tmpVector( vectorIndex ) = phi0() ;
      ++vectorIndex ;
   }
   if( m_free[ kD0 ] )
   {
      tmpVector( vectorIndex ) = d0() ;
      ++vectorIndex ;
   }
   if( m_free[ kCotTheta ] )
   {
      tmpVector( vectorIndex ) = cotTheta() ;
      ++vectorIndex ;
   }
   if( m_free[ kZ0 ] )
   {
      tmpVector( vectorIndex ) = z0() ;
   }

   return tmpVector ;
}

//
// static member functions
//
