#if !defined(TRACKROOT_TRSEEDTRACKQUALITY_H)
#define TRACKROOT_TRSEEDTRACKQUALITY_H
// -*- C++ -*-
//
// Package:     <TrackRoot>
// Module:      TRSeedTrackQuality
// 
/**\class TRSeedTrackQuality TRSeedTrackQuality.h TrackRoot/TRSeedTrackQuality.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Werner Sun
// Created:     Thu Jul 20 15:10:46 EDT 2000
// $Id: TRSeedTrackQuality.h,v 1.3 2001/01/05 05:27:36 lyon Exp $
//
// Revision history
//
// $Log: TRSeedTrackQuality.h,v $
// Revision 1.3  2001/01/05 05:27:36  lyon
// Added LayerMap and other changes to quality objects
//
// Revision 1.2  2000/10/28 20:34:49  ajm36
// added numberLayers, numberMultipleHits, useForExtrapolation to seed qualities
//
// Revision 1.1  2000/08/01 23:25:02  wsun
// First submission.
//

// system include files

// user include files
#include "TrackRoot/TRTrackQuality.h"

#include "C++Std/fwd_ostream.h"

// forward declarations

class TRSeedTrackQuality : public TRTrackQuality
{
  
   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      TRSeedTrackQuality() ;

      TRSeedTrackQuality( Identifier identifier,
			  double chiSquare,
			  int degreesOfFreedom,
			  int numberHits,
			  int numberHitsExpected,
                          int numberLayers,
                          int numberMultipleHits,
			  DABoolean zEndpointUsed,
			  DABoolean originUsed,
                          DABoolean useForExtrapolation) ;

      virtual ~TRSeedTrackQuality() ;

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------
      double chiSquare() const {
         return m_chiSquare;
      }
      int degreesOfFreedom() const {
         return m_degreesOfFreedom;
      }
      /**
       * \return number of hits we would expect based on the turnover/exit
       * point.
       */
      int numberHitsExpected() const {
         return m_numberHitsExpected;
      }
      /**
       * \return number of layers hit
       */
      int numberLayers() const {
         return m_numberLayers;
      }
      /**
       * \return number of multiple hits in a layer
       */
      int numberMultipleHits() const {
         return m_numberMultipleHits;
      }
      /**
       * \return true if the z-position of the endcap was used for some
       * layer because the track left the detector.
       */
      DABoolean zEndpointUsed() const {
         return m_zEndpointUsed;
      }
      /**
       * \return true if the origin was used as a point in the helix fit.
       */
      DABoolean originUsed() const {
         return m_originUsed;
      }
      /**
       * \return true if track can be extrapolated to RICH, CC
       */
      DABoolean useForExtrapolation() const {
         return m_useForExtrapolation;
      }

      /**
       * Used by operator<<
       * -- You can do report(INFO, kFacilityString) << seedTrackQual << endl;
       */
      void printout(ostream& s) const;

      // Print out a status flag
      void reportFlag(ostream& s, const char* text, DABoolean flag) const;

      // Operator==
      DABoolean operator==(const TRSeedTrackQuality& rhs) const
      {
	 return ( TRTrackQuality::operator==( rhs ) &&
		  chiSquare()          == rhs.chiSquare() &&
		  degreesOfFreedom()   == rhs.degreesOfFreedom() &&
		  numberHitsExpected() == rhs.numberHitsExpected() &&
                  numberLayers()       == rhs.numberLayers() &&
                  numberMultipleHits() == rhs.numberMultipleHits() &&
		  zEndpointUsed()      == rhs.zEndpointUsed() &&
		  originUsed()         == rhs.originUsed() &&
                  useForExtrapolation()== rhs.useForExtrapolation() );
      }

      // Operator!=
      DABoolean operator!=(const TRSeedTrackQuality& rhs ) const
      {
	return !( (*this) == rhs );
      }

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      //TRSeedTrackQuality( const TRSeedTrackQuality& ); // default OK

      // ---------- assignment operator(s) ---------------------
      const TRSeedTrackQuality& operator=( const TRSeedTrackQuality& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      double      m_chiSquare;
      int         m_degreesOfFreedom;
      int         m_numberHitsExpected;
      int         m_numberLayers;
      int         m_numberMultipleHits;
      DABoolean   m_zEndpointUsed;
      DABoolean   m_originUsed;
      DABoolean   m_useForExtrapolation;

      // ---------- static data members ------------------------

};

// inline function definitions

// operator<<
inline ostream& operator<<(ostream& stream, 
			   const TRSeedTrackQuality& seedTrackQuality)
{
  seedTrackQuality.printout(stream);
  return stream;
};


// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "TrackRoot/Template/TRSeedTrackQuality.cc"
//#endif

#endif /* TRACKROOT_TRSEEDTRACKQUALITY_H */
