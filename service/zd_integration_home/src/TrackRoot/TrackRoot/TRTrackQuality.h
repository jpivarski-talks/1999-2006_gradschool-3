#if !defined(TRACKROOT_TRTRACKQUALITY_H)
#define TRACKROOT_TRTRACKQUALITY_H
// -*- C++ -*-
//
// Package:     TrackRoot
// Module:      TRTrackQuality
// 
/**\class TRTrackQuality TRTrackQuality.h TrackRoot/TRTrackQuality.h

 Description: Track quality object for pattern-recognition tracks.

 Usage:
    This object stores the basic information for the quality of tracks
    coming out of a track finder.  Specific finders will create their
    own quality objects by inheriting this.

    The constructor is protected, so only derived types can be
    instantiated.

*/
//
// Author:      Michael Marsh
// Created:     Tue Nov 10 09:51:06 EST 1998
// $Id: TRTrackQuality.h,v 1.13 2000/08/01 23:29:09 wsun Exp $
//
// Revision history
//
// $Log: TRTrackQuality.h,v $
// Revision 1.13  2000/08/01 23:29:09  wsun
// Moved extraneous data members into TRSeedTrackQuality.
//
// Revision 1.12  2000/07/13 22:38:17  cdj
// Can now properly read by TRTrackQuality's
//
// Revision 1.11  2000/04/05 15:38:58  marsh
// Defer virtual destructor definition to TRTrackQuality.cc
//
// Revision 1.10  2000/02/25 01:09:46  wsun
// Added copy/default ctors and comparison operators for storage.
//
// Revision 1.9  2000/02/24 22:41:15  lyon
// Added code for default constructor and comparison operators
//
// Revision 1.8  2000/02/23 18:29:06  lyon
// Added copy constructors for storage helpers
//
// Revision 1.7  2000/02/23 18:00:50  lyon
// Storage helper for TRTrackQuality
//
// Revision 1.6  1999/10/23 17:43:27  cdj
// fixed improper use of typename
//
// Revision 1.5  1999/10/07 16:33:53  marsh
// Added doxygen-style comments.
//
// Revision 1.4  1998/11/20 15:42:01  marsh
// New pieces of information added, including hit counting and additional
// boolean flags for "z-escapes".
//
// Revision 1.3  1998/11/15 16:58:06  marsh
// Added success() to base quality classes.  Made classes concrete, but
// with protected constructors to limit instances to derived classes.
//
// Revision 1.2  1998/11/12 16:06:37  marsh
// Stop default empty ctor().
//
// Revision 1.1  1998/11/12 15:07:53  marsh
// New track quality object.
//

// system include files

// user include files
#include "TrackRoot/TRSeedTrack.h" // for Identifier

// forward declarations

class TRTrackQuality
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      typedef TRSeedTrack::Identifier Identifier;

      // ---------- Constructors and destructor ----------------
      virtual ~TRTrackQuality();

      // Needed for storage helpers
      TRTrackQuality() :
	m_id( 0 ),
	m_numberHits( 0 ) {}

      TRTrackQuality( Identifier identifier,
                      int numberHits )
         : m_id(                 identifier ) ,
	   m_numberHits(         numberHits ) {}

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------
      /**
       * \return unique (distinct) track number.
       * This identifier is shared by all representations of a track.
       */
      const Identifier& identifier() const {
         return m_id;
      }
      /**
       * \return number of hits attached to the track.
       */
      int numberHits() const {
         return m_numberHits;
      }

      // Operator==
      DABoolean operator==(const TRTrackQuality& rhs) const
      {
	return ( identifier() == rhs.identifier() &&
		 numberHits() == rhs.numberHits() ) ;
      }

      // Operator!=
      DABoolean operator!=(const TRTrackQuality& rhs ) const
      {
	return !( (*this) == rhs );
      }

      // ---------- static member functions --------------------

   protected:
      // ---------- constructor --------------------------------

      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      //TRTrackQuality( const TRTrackQuality& ); // stop default

      // ---------- assignment operator(s) ---------------------
      //const TRTrackQuality& operator=( const TRTrackQuality& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      Identifier  m_id;
      int         m_numberHits;

      // ---------- static data members ------------------------

};

#endif /* TRACKROOT_TRTRACKQUALITY_H */
