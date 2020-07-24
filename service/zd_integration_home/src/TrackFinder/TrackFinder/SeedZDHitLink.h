#if !defined(TRACKFINDER_SEEDZDHITLINK_H)
#define TRACKFINDER_SEEDZDHITLINK_H
// -*- C++ -*-
//
// Package:     <TrackFinder>
// Module:      SeedZDHitLink
// 
/**\class SeedZDHitLink SeedZDHitLink.h TrackFinder/SeedZDHitLink.h

 Description: Link data for the Seed Tracks \<--\> ZDHit lattice.

 Usage:
    This defines the link data between seed tracks and drift chamber hits,
    the information about a hit as it relates to a specific track.

    There is also a typedef to the appropriate Lattice instantiation,
    SeedTrackZDHitLattice.

*/
//
// Author:      Adam Lyon
// Created:     Tue Oct 13 17:24:33 EDT 1998
// $Id: SeedZDHitLink.h,v 1.1 2002/09/27 21:03:06 bkh Exp $
//
// Revision history
//
// $Log: SeedZDHitLink.h,v $
// Revision 1.1  2002/09/27 21:03:06  bkh
// Add ZD capability
//
//

// system include files

// user include files
#include "TrackRoot/TRSeedTrack.h"
#include "CalibratedData/CalibratedZDHit.h"
#include "Lattice/Lattice.h"

// forward declarations

class SeedZDHitLink
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      enum fitFlagEnum {kFITTABLE, kDO_NOT_FIT};

      // ---------- Constructors and destructor ----------------
      
      // Default constructor
      SeedZDHitLink();

      SeedZDHitLink(float signedDriftDistance, float pull);

      SeedZDHitLink(float signedDriftDistance, float pull, 
		    fitFlagEnum fitFlag);

      virtual ~SeedZDHitLink();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------
      inline float signedDriftDistance() const;
      inline float pull() const;

      inline fitFlagEnum fitFlag() const;

      inline DABoolean operator==( const SeedZDHitLink& rhs ) const;
      inline DABoolean operator!=( const SeedZDHitLink& rhs ) const;

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      //SeedZDHitLink( const SeedZDHitLink& ); // -- WE WANT THE DEFAULT COPY CONSTRUCTOR

      // ---------- assignment operator(s) ---------------------
      //const SeedZDHitLink& operator=( const SeedZDHitLink& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      float m_signedDriftDistance;
      float m_pull;

      // Add fit flag
      fitFlagEnum m_fitFlag;

      // 

      // ---------- static data members ------------------------

};

// inline function definitions
float SeedZDHitLink::signedDriftDistance() const 
  { return m_signedDriftDistance; }
      
float SeedZDHitLink::pull() const 
  { return m_pull; }

SeedZDHitLink::fitFlagEnum SeedZDHitLink::fitFlag() const
{ return m_fitFlag; }

DABoolean SeedZDHitLink::operator==( const SeedZDHitLink& rhs ) const
{  return ( rhs.signedDriftDistance() == signedDriftDistance() &&
            rhs.pull() == pull() &&
	    rhs.fitFlag() == fitFlag() ); }

// This is peculiar--- the operator "==" returns true iff you have two
// exact copies (possibly the same object) and the operator "!="
// returns false iff lhs and rhs are the same object. This is not what
// I would have expected. (JM 9/18/00)
DABoolean SeedZDHitLink::operator!=( const SeedZDHitLink& rhs ) const
{  return !((*this) == rhs ); }

typedef Lattice< TRSeedTrack, CalibratedZDHit, SeedZDHitLink >  
                                                      SeedTrackZDHitLattice;

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "TrackFinder/Template/SeedZDHitLink.cc"
//#endif

#endif /* TRACKFINDER_SEEDZDHITLINK_H */
