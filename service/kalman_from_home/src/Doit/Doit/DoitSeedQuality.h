#if !defined(DOIT_DOITSEEDQUALITY_H)
#define DOIT_DOITSEEDQUALITY_H
// -*- C++ -*-
//
// Package:     <Doit>
// Module:      DoitSeedQuality
// 
// Description: Seed track quality for Doit tracks
//
// Usage:
//    <usage>
//
// Author:      Adam Lyon
// Created:     Wed Aug 18 13:37:51 EDT 1999
// $Id: DoitSeedQuality.h,v 1.4 2001/01/05 05:37:30 lyon Exp $
//
// Revision history
//
// $Log: DoitSeedQuality.h,v $
// Revision 1.4  2001/01/05 05:37:30  lyon
// Added Doit status words
//
// Revision 1.3  2000/11/01 01:23:29  lyon
// Add Fittable flag and new members to quality object
//
// Revision 1.2  2000/08/16 19:43:18  wsun
// Updated to new TRSeedTrackQuality.
//
// Revision 1.1  1999/08/31 21:16:41  lyon
// Doit seed quality
//

// system include files
#include "C++Std/fwd_ostream.h"

// user include files
#include "TrackRoot/TRSeedTrackQuality.h"
#include "Doit/DoitStatusInfo.h"

// forward declarations

/// Seed track quality for DOIT Seed Tracks.
/// Note that the chi squared saved in the TRTrackQuality (base class) and 
/// obtained with chiSquare() is the chiSquare of the r-phi fit. 
/// Other chi Squares are available.
class DoitSeedQuality : public TRSeedTrackQuality
{
   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      DoitSeedQuality( Identifier id,
		       double chiSquare,
		       int degreesOfFreedom,
		       int numberHits,
		       int numberHitsExpected,
		       int numberLayers,
		       int numberMultipleHits,
		       DABoolean zEndpointUsed,
		       DABoolean originUsed,
		       DABoolean useForExtrapolation,
		       DABoolean goodZ,
		       DABoolean inwardDoitCurler,
		       float chiSquareZ,
		       float chiSquare3,
		       float resicd,
		       const DoitStatusInfo& status
		       )
	: TRSeedTrackQuality( id, chiSquare, degreesOfFreedom,
			      numberHits, numberHitsExpected, 
			      numberLayers, numberMultipleHits,
			      zEndpointUsed, originUsed, 
			      useForExtrapolation),
	  m_goodZ(goodZ),
	  m_inwardDoitCurler(inwardDoitCurler),
	  m_chiSquareZ(chiSquareZ),
	  m_chiSquare3(chiSquare3),
          m_resicd(resicd),
          m_doitStatus( status )
      {}


    DoitSeedQuality( const DoitSeedQuality& rhs )
      : TRSeedTrackQuality( rhs.identifier(),
			    rhs.chiSquare(),
			    rhs.degreesOfFreedom(),
			    rhs.numberHits(),
			    rhs.numberHitsExpected(),
			    rhs.numberLayers(),
			    rhs.numberMultipleHits(),
			    rhs.zEndpointUsed(),
			    rhs.originUsed(),
			    rhs.useForExtrapolation()),
	m_goodZ( rhs.goodZ() ),
	m_inwardDoitCurler( rhs.inwardDoitCurler() ),
        m_chiSquareZ( rhs.chiSquareZ() ),
	m_chiSquare3( rhs.chiSquare3() ),
        m_resicd( rhs.resicd() ),
        m_doitStatus( rhs.doitStatus() )
    {}
  
      virtual ~DoitSeedQuality(){};

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------
  
      /// goodZ - Returns true if Z information is good
      ///         (false is equivalent to kincd = -1)
      DABoolean goodZ() const { return m_goodZ; }

      /// inwardDoitCurler - Returns true is the track is
      ///                    recognized by Doit as the inward
      ///                    part of a curler (don't use since 
      ///                    the track is better represented by the outward
      ///                    part). Note that other algorithms may disagree
      ///                    with Doit's decision.
      DABoolean inwardDoitCurler() const { return m_inwardDoitCurler; }

      /// chiSquareZ - Chi**2 per D.O.F. for the Z fit
      float chiSquareZ() const { return m_chiSquareZ; }

      /// chiSquare3 - Total chi**2 for 3D fit (probably not done)
      float chiSquare3() const { return m_chiSquare3; }

      /// resicd - Fit residuals scaled by D.O.F.
      float resicd() const { return m_resicd; }

      /// Get Doit status object
      const DoitStatusInfo& doitStatus() const { return m_doitStatus; }

      /// Print out nicely
      void printout(ostream& s) const;

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
  //      DoitSeedQuality( const DoitSeedQuality& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const DoitSeedQuality& operator=( const DoitSeedQuality& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      DABoolean m_goodZ;
      DABoolean m_inwardDoitCurler;
      float m_chiSquareZ;
      float m_chiSquare3;
      float m_resicd;
      DoitStatusInfo m_doitStatus;

      // ---------- static data members ------------------------

};

// inline function definitions

// operator<<
inline ostream& operator<<(ostream& stream, 
			   const DoitSeedQuality& doitSeedQuality)
{
  doitSeedQuality.printout(stream);
  return stream;
};


// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "Doit/Template/DoitSeedQuality.cc"
//#endif

#endif /* DOIT_DOITSEEDQUALITY_H */
