// -*- C++ -*-
//
// Package:     <TrackRoot>
// Module:      TRTrackFitQuality
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Tue Dec 19 18:44:17 EST 2000
// $Id: TRTrackFitQuality.cc,v 1.2 2001/01/05 05:27:32 lyon Exp $
//
// Revision history
//
// $Log: TRTrackFitQuality.cc,v $
// Revision 1.2  2001/01/05 05:27:32  lyon
// Added LayerMap and other changes to quality objects
//
// Revision 1.1  2000/12/22 06:54:12  wsun
// First submission.
//

#include "Experiment/Experiment.h"

// system include files
#include <stdlib.h>   // for ::exit
#include <assert.h>
#include <iostream.h>
#include <string.h>
#include <ctype.h>


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

// user include files
//#include "Experiment/report.h"
#include "TrackRoot/TRTrackFitQuality.h"

// STL classes
// You may have to uncomment some of these or other
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

static const char* const kFacilityString = "TrackRoot.TRTrackFitQuality" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: TRTrackFitQuality.cc,v 1.2 2001/01/05 05:27:32 lyon Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
TRTrackFitQuality::TRTrackFitQuality(
   Identifier identifier,
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
   : TRTrackQuality( identifier, numberHits ),
     m_chiSquare(                chiSquare ),
     m_degreesOfFreedom(         degreesOfFreedom ),
     m_numberHitsExpected(       numberHitsExpected ),
     m_numberHitsInput(          numberHitsInput ),
     m_numberHitsDropped(        numberHitsDropped ),
     m_numberIterations(         numberIterations ),
     m_fit(                      fit ),
     m_fitAbort(                 fitAbort ),
     m_detector(                 detector ),
     m_layerMap( layerMap )
{
   assert( numberHitsVec.size() ==
	   TRSubdetectorLists::numberOfHitTypes( detector ) ) ;
   assert( averageResiduals.size() ==
	   TRSubdetectorLists::numberOfHitTypes( detector ) ) ;
   assert( rmsResiduals.size() ==
	   TRSubdetectorLists::numberOfHitTypes( detector ) ) ;

   STL_VECTOR( int )::const_iterator numberHitsItr = numberHitsVec.begin() ;
   STL_VECTOR( double )::const_iterator averageResidualItr =
      averageResiduals.begin() ;
   STL_VECTOR( double )::const_iterator rmsResidualItr = rmsResiduals.begin() ;
   
   for( int i = 0 ; i < TRSubdetectorLists::kNumberOfHitTypes ; ++i )
   {
      if( TRSubdetectorLists::subdetectorValid(
	 m_detector,
	 TRSubdetectorLists::HitTypes( i ) ) )
      {
	 m_numberHitsArray[ i ] = *numberHitsItr ;
	 m_averageResiduals[ i ] = *averageResidualItr ;
	 m_rmsResiduals[ i ] = *rmsResidualItr ;

	 ++numberHitsItr ;
	 ++averageResidualItr ;
	 ++rmsResidualItr ;
      }
      else
      {
	 m_numberHitsArray[ i ] = 0 ;
	 m_averageResiduals[ i ] = 0. ;
	 m_rmsResiduals[ i ] = 0. ;
      }
   }
}

// TRTrackFitQuality::TRTrackFitQuality( const TRTrackFitQuality& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

TRTrackFitQuality::~TRTrackFitQuality()
{
}

//
// assignment operators
//
// const TRTrackFitQuality& TRTrackFitQuality::operator=( const TRTrackFitQuality& rhs )
// {
//   if( this != &rhs ) {
//      // do actual copying here, plus:
//      // "SuperClass"::operator=( rhs );
//   }
//
//   return *this;
// }

DABoolean
TRTrackFitQuality::operator==( const TRTrackFitQuality& rhs ) const
{
   return( TRTrackQuality::operator==( rhs ) &&
	   m_chiSquare                == rhs.chiSquare() &&
	   m_degreesOfFreedom         == rhs.degreesOfFreedom() &&
	   m_numberHitsExpected       == rhs.m_numberHitsExpected &&
	   m_numberHitsInput          == rhs.m_numberHitsInput &&
	   m_numberHitsDropped        == rhs.m_numberHitsDropped &&
	   m_numberIterations         == rhs.m_numberIterations &&
	   m_fit                      == rhs.m_fit &&
	   m_fitAbort                 == rhs.m_fitAbort &&
	   m_numberHitsArray          == rhs.m_numberHitsArray &&
	   m_averageResiduals         == rhs.m_averageResiduals &&
	   m_rmsResiduals             == rhs.m_rmsResiduals &&
	   m_layerMap                 == rhs.m_layerMap
      ) ;
}

//
// member functions
//

//
// const member functions
//

unsigned int
TRTrackFitQuality::numberSubdetectorHits(
   TRSubdetectorLists::HitTypes aHitType ) const
{
   assert( TRSubdetectorLists::subdetectorValid( m_detector, aHitType ) ) ;
   return m_numberHitsArray[ aHitType ] ;
}

double
TRTrackFitQuality::averageResidual(
   TRSubdetectorLists::HitTypes aHitType ) const
{
   assert( TRSubdetectorLists::subdetectorValid( m_detector, aHitType ) ) ;
   return m_averageResiduals[ aHitType ] ;
}

double
TRTrackFitQuality::rmsResidual( TRSubdetectorLists::HitTypes aHitType ) const
{
   assert( TRSubdetectorLists::subdetectorValid( m_detector, aHitType ) ) ;
   return m_rmsResiduals[ aHitType ] ;
}

const STL_VECTOR( int ) TRTrackFitQuality::numberHitsVector() const
{
   STL_VECTOR( int ) numberHitsVector ;

   for( int i = 0 ; i < TRSubdetectorLists::kNumberOfHitTypes ; ++i )
   {
      if( TRSubdetectorLists::subdetectorValid(
	 m_detector,
	 TRSubdetectorLists::HitTypes( i ) ) )
      {
	 numberHitsVector.push_back( m_numberHitsArray[ i ] ) ;
      }
   }

   return numberHitsVector ;
}


unsigned int TRTrackFitQuality::numberSubdetectorHitLayers(
			     TRSubdetectorLists::HitTypes aHitType) const
{
  assert( TRSubdetectorLists::subdetectorValid( m_detector, aHitType ) ) ;

  // How many layers are there that where actually hit
  unsigned int nLayersHit = 0;

  // How many layers are there in the detector?
  const int nLayers = 
    TRSubdetectorLists::layerMapNLayers( m_detector, aHitType );

  // Let's count
  for ( unsigned int i = 1; i <= nLayers; ++i )
  {
    if ( isLayerHit(aHitType, i) )
      ++nLayersHit;
  }

  return nLayersHit;
}

int TRTrackFitQuality::layersHitInside( TRSubdetectorLists::HitTypes aHitType,
					 unsigned int layerNum ) const
{
   int sum( 0 );
   int outermost_layer( TRSubdetectorLists::layerMapNLayers( m_detector, aHitType ) );
   for ( int layer = 1;
	 layer < layerNum  &&  layer <= outermost_layer;
	 layer++ )
      sum += m_layerMap.isLayerHit( m_detector, aHitType, layerNum );
   return sum;
}

int TRTrackFitQuality::layersHitOutside( TRSubdetectorLists::HitTypes aHitType,
					 unsigned int layerNum ) const
{
   int sum( 0 );
   int outermost_layer( TRSubdetectorLists::layerMapNLayers( m_detector, aHitType ) );
   for ( int layer = ( layerNum > 0 ? ( layerNum + 1 ) : 1 );
	 layer <= outermost_layer;
	 layer++ )
      sum += m_layerMap.isLayerHit( m_detector, aHitType, layerNum );
   return sum;
}

DABoolean TRTrackFitQuality::isLayerHit( TRSubdetectorLists::HitTypes aHitType,
					 unsigned int layerNum ) const
{
  return m_layerMap.isLayerHit(m_detector, aHitType, layerNum);
}
  

const STL_VECTOR( double ) TRTrackFitQuality::averageResidualsVector() const
{
   STL_VECTOR( double ) avgResidualsVector ;

   for( int i = 0 ; i < TRSubdetectorLists::kNumberOfHitTypes ; ++i )
   {
      if( TRSubdetectorLists::subdetectorValid(
	 m_detector,
	 TRSubdetectorLists::HitTypes( i ) ) )
      {
	 avgResidualsVector.push_back( m_averageResiduals[ i ] ) ;
      }
   }

   return avgResidualsVector ;
}

const STL_VECTOR( double ) TRTrackFitQuality::rmsResidualsVector() const
{
   STL_VECTOR( double ) rmsResidualsVector ;

   for( int i = 0 ; i < TRSubdetectorLists::kNumberOfHitTypes ; ++i )
   {
      if( TRSubdetectorLists::subdetectorValid(
	 m_detector,
	 TRSubdetectorLists::HitTypes( i ) ) )
      {
	 rmsResidualsVector.push_back( m_rmsResiduals[ i ] ) ;
      }
   }

   return rmsResidualsVector ;
}

const TRLayerMap& TRTrackFitQuality::layerMap() const
{
  return m_layerMap;
}

  
void TRTrackFitQuality::reportFlag(ostream& s, const char* text, 
				   DABoolean flag) const
{
  
  s << " " << text << "=";

  if ( flag ) 
  {
    s << "TRUE";
  }
  else
  {
    s << "false";
  }
}

void TRTrackFitQuality::printout(ostream& s) const
{
  // Print out stuff nicely
  s << "TRTrackQuality: \n";

  s << "   ";
  reportFlag(s, "fitted", fit() );
  reportFlag(s, "fit_aborted", fitAbort());
  s << "\n";

  s << "  ChiSquare = " << chiSquare() << "   DOF = " << degreesOfFreedom();
  s << "  # Hits = " << numberHits() << "    # hits expected = "
    << numberHitsExpected() << "   Ratio = " << ratioNumberHitsToExpected()
    << endl;
  s << "  # hits input = " << numberHitsInput() << "  # hits dropped " 
    << numberHitsDropped() 
    << "  # hits iterations = " << numberIterations() <<"\n";
    
  // Now go through the sub-detectors
  for ( unsigned int i = 0; i < TRSubdetectorLists::kNumberOfHitTypes; ++i )
  {
    // Convert to the enum to make things easy
    TRSubdetectorLists::HitTypes h = (TRSubdetectorLists::HitTypes) i;

    // Do we have this detector?
    if ( ! TRSubdetectorLists::subdetectorValid( detector(), h ) )
      continue;

    // We do! Print it out
    s << "      " << TRSubdetectorLists::subdetectorName(h) << ":  ";
    s << " # hits = " << numberSubdetectorHits(h) << "  "
      << " # hit layers = " << numberSubdetectorHitLayers(h) << " "
      << " rmsResidual = " << rmsResidual(h) << "  "
      << " avgResidual = " << averageResidual(h) << "  " << "\n";
   
    
    // Now we do the layer map
    s << "       Layers hit = ";

    // Get the number of layers
    int nLayers = TRSubdetectorLists::layerMapNLayers( detector(), h );
    
    // Loop through them
    for ( unsigned int j = 1; j <= nLayers; ++j )
    {
      if ( isLayerHit(h, j) )
      {
	s << j << " ";
      }
    }

    // End the line
    s << "\n\n";

  } // for (over sub-detectors)
}
	 
//
// static member functions
//
