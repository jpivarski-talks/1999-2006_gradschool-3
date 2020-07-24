#if !defined(TRACKROOT_TRTRACK_H)
#define TRACKROOT_TRTRACK_H
// -*- C++ -*-
//
// Package:     TrackRoot
// Module:      TRTrack
//
/**\class TRTrack TRTrack.h TrackRoot/TRTrack.h

 Description: Namespace for Identifier

 Usage:
    The TRTrack class is acting as a namespace to define an
    identifier that relates all information, spread in different
    DataFields, to a single entity.  It also contains the
    enumerator to differentiate between fit hypothesis.

    This class is specifically designed to be the minimal information
    needed to determine that a track exists without having to depend
    on the presence of any other particular data.  This is especially
    important for navigational objects such as TDTrack and NavTrack,
    which can exist with only partial information.

*/
//
// Author:      Simon Patton
// Created:     Fri May 17 08:38:02 EDT 1996
// $Id: TRTrack.h,v 1.4 1999/11/04 14:44:59 mkl Exp $
//
// Revision history
//
// $Log: TRTrack.h,v $
// Revision 1.4  1999/11/04 14:44:59  mkl
// added storage helper for TRTrack
//
// Revision 1.3  1999/10/07 16:33:51  marsh
// Added doxygen-style comments.
//
// Revision 1.2  1999/06/24 13:55:26  marsh
// And the CleoDB purge continues...
//
// DBCandidate::Hypo is now TRTrack::Hypo .  Man, I can't wait to change
// this everywhere in the repository.
//
// Revision 1.1  1999/06/24 12:43:54  marsh
// Moved DBTrack into TrackRoot.
//
// Revision 1.6  1997/09/17 19:26:39  sjp
// Modified definition if Identifier
//
// Revision 1.5  1997/08/22 17:58:45  sjp
// Removed redundant enumerator
//
// Revision 1.4  1997/08/06 18:08:36  sjp
// Removed dependence on DataField
//
// Revision 1.3  1997/07/09 18:01:48  sjp
// New <Package>/<File>.h structure
// Modified so can also compile under cxx v5.5
//
// Revision 1.2  1997/06/27 18:55:24  sjp
// extended for use a anchor to composite track class
//
// Revision 1.1  1997/04/16 18:43:04  sjp
// New header for Tracking DataFields
//

// system include files

// user include files
#include "DataHandler/Stream.h" // for Stream::Type

// STL include

// forward declarations

class TRTrack
{
      // friend classses and functions

   public:
      // constants, enums and typedefs
      enum Hypo {
         kElectron ,
         kMuon ,
         kChargedPion ,
         kChargedKaon ,
         kProton ,
         kExitElectron ,
         kExitMuon ,
         kExitPion ,
         kExitKaon ,
         kExitProton ,
         kMaxHypo
      } ;

// following must be a reference to avoid confusion when static
//   objects are being created.
      static const Stream::Type& kStream ;

      typedef Count Identifier ;

      // Constructors and destructor
      TRTrack( const Identifier& aId ) ;
      virtual ~TRTrack() ;

      // member functions

      // const member functions
      /**
       * \return unique (distinct) track number.
       * This is the identifier shared by all representations of a track.
       */
      const Identifier& identifier() const ;

      // static member functions

   protected:
      // protected member functions

      // protected const member functions

   private:
      // Constructors and destructor
      TRTrack() ; // stop default
      //TRTrack( const TRTrack& ) ; // stop default

      // assignment operator(s)
      const TRTrack& operator=( const TRTrack& ) ; // stop default

      // private member functions

      // private const member functions

      // data members
      Identifier m_id ;

      // static data members

};

// inline function definitions

#endif /* TRACKROOT_TRTRACK_H */
