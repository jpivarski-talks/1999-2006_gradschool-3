#if !defined(TRACKROOT_TRSUBDETECTORLISTS_H)
#define TRACKROOT_TRSUBDETECTORLISTS_H
// -*- C++ -*-
//
// Package:     <TrackRoot>
// Module:      TRSubdetectorLists
// 
/**\class TRSubdetectorLists TRSubdetectorLists.h TrackRoot/TRSubdetectorLists.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Werner Sun
// Created:     Tue Dec 19 17:32:38 EST 2000
// $Id: TRSubdetectorLists.h,v 1.2 2001/01/05 05:27:36 lyon Exp $
//
// Revision history
//
// $Log: TRSubdetectorLists.h,v $
// Revision 1.2  2001/01/05 05:27:36  lyon
// Added LayerMap and other changes to quality objects
//
// Revision 1.1  2000/12/22 06:54:18  wsun
// First submission.
//

// system include files

// user include files

// forward declarations

class TRSubdetectorLists
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      enum Detectors { kCLEOII,
		       kCLEOIIV,
		       kCLEOIII,
		       kNumberOfDetectors } ;

      enum HitTypes { kPT,
		      kVD,
		      kSVR,
		      kSVZ,
		      kDR2,      // This is the CLEO II/II.V DR
		      kDR3Axial,  // This is for the CLEO III DR only
		      kDR3Stereo, // This is for the CLEO III DR only
		      kVDCathode,
		      kDRCathode,
		      kEntireDetector,
		      kNumberOfHitTypes } ;


      // Make sure this corresponds to the biggest layer map size
      // (see TRSubdetectorLists.cc)
      enum {kMaxLayerMapSize = 3};

      // ---------- Constructors and destructor ----------------
      TRSubdetectorLists();
      virtual ~TRSubdetectorLists();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------
      static DABoolean subdetectorValid(
	 TRSubdetectorLists::Detectors aDetector,
	 TRSubdetectorLists::HitTypes aHitType )
      {
	 return m_subdetectors[ aDetector ][ aHitType ] ;
      }

      static int numberOfHitTypes( TRSubdetectorLists::Detectors aDetector )
      {
	 return m_numberOfTypes[ aDetector ] ;
      }

      static int layerMapSize( TRSubdetectorLists::Detectors aDetector )
      {
	return m_layerMapSizes[ aDetector ] ; 
      }

      static int layerMapOffset( TRSubdetectorLists::Detectors aDetector,
				 TRSubdetectorLists::HitTypes aHitType)
      {
	return m_layerMapOffsets[ aDetector ][ aHitType ]; 
      }

      static int layerMapNLayers( TRSubdetectorLists::Detectors aDetector,
				  TRSubdetectorLists::HitTypes aHitType)
      {
	return m_layerMapNLayers[ aDetector ][ aHitType ]; 
      }

      static char* subdetectorName( TRSubdetectorLists::HitTypes aHitType )
      {
	return m_subdetectorNames[aHitType];
      }

//       static DABoolean* hitTypesArray(
// 	 TRSubdetectorLists::Detectors aDetector )
//       {
// 	 return m_subdetectors[ aDetector ] ;
//       }

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      TRSubdetectorLists( const TRSubdetectorLists& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const TRSubdetectorLists& operator=( const TRSubdetectorLists& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------
      static DABoolean m_subdetectors[kNumberOfDetectors][kNumberOfHitTypes] ;
      static int m_numberOfTypes[ kNumberOfDetectors ] ;
      static int m_layerMapSizes[ kNumberOfDetectors ] ;
      static int m_layerMapOffsets[ kNumberOfDetectors ][ kNumberOfHitTypes ] ;
      static int m_layerMapNLayers[ kNumberOfDetectors ][ kNumberOfHitTypes ] ;

      static char* m_subdetectorNames[ kNumberOfHitTypes ];
};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "TrackRoot/Template/TRSubdetectorLists.cc"
//#endif

#endif /* TRACKROOT_TRSUBDETECTORLISTS_H */
