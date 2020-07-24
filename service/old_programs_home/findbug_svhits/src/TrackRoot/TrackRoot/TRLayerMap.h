#if !defined(TRACKROOT_TRLAYERMAP_H)
#define TRACKROOT_TRLAYERMAP_H
// -*- C++ -*-
//
// Package:     <TrackRoot>
// Module:      TRLayerMap
// 
/**\class TRLayerMap TRLayerMap.h TrackRoot/TRLayerMap.h

 Description: Layer map for the TRTrackFitQuality object. Holds a bit
              map indicating what layers the track hit.

 Usage:
    <usage>

*/
//
// Author:      Adam Lyon
// Created:     Fri Dec 29 10:20:17 EST 2000
// $Id: TRLayerMap.h,v 1.1 2001/01/05 05:27:36 lyon Exp $
//
// Revision history
//
// $Log: TRLayerMap.h,v $
// Revision 1.1  2001/01/05 05:27:36  lyon
// Added LayerMap and other changes to quality objects
//

// system include files
#include <assert.h>

// user include files
#include "TrackRoot/TRSubdetectorLists.h"

// forward declarations

class TRLayerMap
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------

      // Default C'tor -- make an empty TRLayerMap
      TRLayerMap();

      // C'tor for storage helper
      TRLayerMap(UInt32 word1, UInt32 word2, UInt32 word3);

      // Copy constructor
      TRLayerMap( const TRLayerMap& );

      // Assignment operator
      const TRLayerMap& operator=( const TRLayerMap& );

      virtual ~TRLayerMap();

      // ---------- member functions ---------------------------
      void setLayerHit( TRSubdetectorLists::Detectors detector,
			TRSubdetectorLists::HitTypes aHitType, 
			unsigned int layerNum );

      // ---------- const member functions ---------------------
      DABoolean isLayerHit( TRSubdetectorLists::Detectors detector,
			    TRSubdetectorLists::HitTypes aHitType, 
			    unsigned int layerNum ) const;

      // Compare 
      DABoolean operator==( const TRLayerMap& rhs ) const;

      // Get the layer map array (for the storage helper)
      const UInt32* begin() const { return m_layerMap; };
      const UInt32* end() const { 
         return m_layerMap+sizeof(m_layerMap)/sizeof(UInt32); };

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------

      // ---------- assignment operator(s) ---------------------

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------
      UInt32 layerMapBitPattern(TRSubdetectorLists::Detectors detector,
				TRSubdetectorLists::HitTypes aHitType, 
				unsigned int layerNum, unsigned int& word) const;

      // ---------- data members -------------------------------
      UInt32 m_layerMap[ TRSubdetectorLists::kMaxLayerMapSize ] ;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "TrackRoot/Template/TRLayerMap.cc"
//#endif

#endif /* TRACKROOT_TRLAYERMAP_H */
