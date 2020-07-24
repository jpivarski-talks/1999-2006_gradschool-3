#if !defined(TRACKFINDER_SEEDDRHITLINK_H)
#define TRACKFINDER_SEEDDRHITLINK_H
// -*- C++ -*-
//
// Package:     <TrackFinder>
// Module:      SeedDRHitLink
// 
/**\class SeedDRHitLink SeedDRHitLink.h TrackFinder/SeedDRHitLink.h

 Description: Link data for the Seed Tracks \<--\> DRHit lattice.

 Usage:
    This defines the link data between seed tracks and drift chamber hits,
    the information about a hit as it relates to a specific track.

    There is also a typedef to the appropriate Lattice instantiation,
    SeedTrackDRHitLattice.

*/
//
// Author:      Adam Lyon
// Created:     Tue Oct 13 17:24:33 EDT 1998
// $Id: SeedDRHitLink.h,v 1.8 2000/10/29 02:43:35 wsun Exp $
//
// Revision history
//
// $Log: SeedDRHitLink.h,v $
// Revision 1.8  2000/10/29 02:43:35  wsun
// kFITABLE --> kFITTABLE
//
// Revision 1.7  2000/10/25 18:35:07  lyon
// Remove fittingweights from SeedDRHitLink and its storage helper
//
// Revision 1.6  2000/10/25 15:07:21  lyon
// Added new fitFlag to SeedDRHitLink link data
//
// Revision 1.5  2000/09/29 19:53:53  mccann
// Added fittingweight as member function of linkdata.
//
// Revision 1.4  2000/02/08 18:28:00  lyon
// Add stuff necessary for Storage Helpers
//
// Revision 1.3  1999/10/07 18:12:48  marsh
// Added doxygen-style comments.
//
// Revision 1.2  1998/11/09 16:59:04  lyon
// Changed class name: LinkData -> SeedDRhitLink and updated typedefs
//
// Revision 1.1  1998/10/17 20:37:16  lyon
// Link data
//

// system include files

// user include files
#include "TrackRoot/TRSeedTrack.h"
#include "CalibratedData/CalibratedDRHit.h"
#include "Lattice/Lattice.h"

// forward declarations

class SeedDRHitLink
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      enum fitFlagEnum {kFITTABLE, kDO_NOT_FIT};

      // ---------- Constructors and destructor ----------------
      
      // Default constructor
      SeedDRHitLink();

      SeedDRHitLink(float signedDriftDistance, float pull);

      SeedDRHitLink(float signedDriftDistance, float pull, 
		    fitFlagEnum fitFlag);

      virtual ~SeedDRHitLink();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------
      inline float signedDriftDistance() const;
      inline float pull() const;

      inline fitFlagEnum fitFlag() const;

      inline DABoolean operator==( const SeedDRHitLink& rhs ) const;
      inline DABoolean operator!=( const SeedDRHitLink& rhs ) const;

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      //SeedDRHitLink( const SeedDRHitLink& ); // -- WE WANT THE DEFAULT COPY CONSTRUCTOR

      // ---------- assignment operator(s) ---------------------
      //const SeedDRHitLink& operator=( const SeedDRHitLink& ); // stop default

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
float SeedDRHitLink::signedDriftDistance() const 
  { return m_signedDriftDistance; }
      
float SeedDRHitLink::pull() const 
  { return m_pull; }

SeedDRHitLink::fitFlagEnum SeedDRHitLink::fitFlag() const
{ return m_fitFlag; }

DABoolean SeedDRHitLink::operator==( const SeedDRHitLink& rhs ) const
{  return ( rhs.signedDriftDistance() == signedDriftDistance() &&
            rhs.pull() == pull() &&
	    rhs.fitFlag() == fitFlag() ); }

// This is peculiar--- the operator "==" returns true iff you have two
// exact copies (possibly the same object) and the operator "!="
// returns false iff lhs and rhs are the same object. This is not what
// I would have expected. (JM 9/18/00)
DABoolean SeedDRHitLink::operator!=( const SeedDRHitLink& rhs ) const
{  return !((*this) == rhs ); }

typedef Lattice< TRSeedTrack, CalibratedDRHit, SeedDRHitLink >  
                                                      SeedTrackDRHitLattice;

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "TrackFinder/Template/SeedDRHitLink.cc"
//#endif

#endif /* TRACKFINDER_SEEDDRHITLINK_H */
