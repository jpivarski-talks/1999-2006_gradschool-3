// -*- C++ -*-
//
// Package:     <TrackRoot>
// Module:      TRLayerMap
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Adam Lyon
// Created:     Fri Dec 29 10:57:29 EST 2000
// $Id: TRLayerMap.cc,v 1.1 2001/01/05 05:27:31 lyon Exp $
//
// Revision history
//
// $Log: TRLayerMap.cc,v $
// Revision 1.1  2001/01/05 05:27:31  lyon
// Added LayerMap and other changes to quality objects
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "TrackRoot/TRLayerMap.h"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "TrackRoot.TRLayerMap" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: TRLayerMap.cc,v 1.1 2001/01/05 05:27:31 lyon Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
TRLayerMap::TRLayerMap()
{
  // Just make a blank one
  for ( unsigned int i = 0; i < TRSubdetectorLists::kMaxLayerMapSize; ++i )
  {
    m_layerMap[i] = 0;
  }
}

// C'tor for storage helper
TRLayerMap::TRLayerMap(UInt32 word1, UInt32 word2, UInt32 word3)
{

  assert( sizeof(m_layerMap)/sizeof(UInt32) >= 3 );

  m_layerMap[0] = word1;
  m_layerMap[1] = word2;
  m_layerMap[2] = word3;
}

TRLayerMap::TRLayerMap( const TRLayerMap& rhs )
{
   // do actual copying here; if you implemented
   // operator= correctly, you may be able to use just say      
   *this = rhs;
}

TRLayerMap::~TRLayerMap()
{
}

//
// assignment operators
//
const TRLayerMap& TRLayerMap::operator=( const TRLayerMap& rhs )
{
  if( this != &rhs ) 
  {
    // Copy the UInt32 words from rhs (the slick way)
    UInt32* i = m_layerMap;
    for ( const UInt32* j = rhs.begin(); j != rhs.end(); ++i, ++j)
    {
      *i = *j;
    }
  }

  return *this;
}


//
// const member functions
//


DABoolean TRLayerMap::operator==(const TRLayerMap& rhs) const
{
  DABoolean theSame = true;

  for ( unsigned int i = 0; i < TRSubdetectorLists::kMaxLayerMapSize;
	++i )
  {
    if ( m_layerMap[i] != rhs.m_layerMap[i] ) 
    {
      theSame = false;
      break;
    }
  }
  
  return theSame;
}
	  

UInt32 TRLayerMap::layerMapBitPattern(
		       TRSubdetectorLists::Detectors detector,
		       TRSubdetectorLists::HitTypes aHitType, 
		       unsigned int layerNum,
		       unsigned int& word) const
{

  // Get the offset for this layer
  unsigned int offset = TRSubdetectorLists::layerMapOffset( 
							   detector, aHitType );

  // Add the desired layer and subtract to start at bit 0
  offset = offset + layerNum - 1 - 1;

  // Offset is the bit number
  word = 0;

  while( offset >= 32 ) {
    ++word;
    offset -= 32;
  }

  // Form the bit pattern (bit shift 1 by offset's)
  UInt32 bitpat =  1 << offset;

  return bitpat;
}

DABoolean TRLayerMap::isLayerHit( TRSubdetectorLists::Detectors detector,
					 TRSubdetectorLists::HitTypes aHitType,
					 unsigned int layerNum ) const
{
  // Can we do this detector?
  assert( TRSubdetectorLists::subdetectorValid( detector, aHitType ) );

  // Is the layer number valid?
  assert(  layerNum > 0 &&
	   layerNum <= TRSubdetectorLists::layerMapNLayers(detector, aHitType)
	  );
  
  // Get the bit pattern and the word number
  unsigned int word;
  UInt32 ANDword = layerMapBitPattern(detector, aHitType, layerNum, word);

  // AND to get the bit
  return ( (m_layerMap[word] & ANDword) > 0 );
}


//
// member functions
//


void TRLayerMap::setLayerHit( TRSubdetectorLists::Detectors detector,
				     TRSubdetectorLists::HitTypes aHitType, 
				     unsigned int layerNum )
{
  // Can we do this detector?
  assert( TRSubdetectorLists::subdetectorValid( detector, aHitType ) );
  
  // Is the layer number valid?
  assert(layerNum > 0 && 
	 layerNum <= TRSubdetectorLists::layerMapNLayers(detector, 
							  aHitType ) );

  // Get the bit pattern and the word number
  unsigned int word;
  UInt32 ORword = layerMapBitPattern(detector, aHitType, layerNum, word);

  // OR to set the bit
  m_layerMap[word] |= ORword;
}
  
//
// static member functions
//
