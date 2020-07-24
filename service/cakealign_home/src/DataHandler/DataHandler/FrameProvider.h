#if !defined(DATAHANDLER_FRAMEPROVIDER_H)
#define DATAHANDLER_FRAMEPROVIDER_H
// -*- C++ -*-
//
// Package:     DataHandler
// Module:      FrameProvider
// 
/**\class FrameProvider FrameProvider.h DataHandler/FrameProvider.h

 Description: Creates and manages a Frame

 Usage:
 This class manages a Frame and ensures that it is "up to date".  A
    client must never create their own Frame but must always get one
    from a FrameProvider using the 'frame' function.  A call to this
    function, with no arguments, will cause the FrameProvider to
    return the last Frame it built.  A call to this function passing a
    Stop Stream and a SyncValue will cause the Frame to be rebuilt
    using these new values.  If a Stream::kNone provided to this
    function call then an empty Frame is built.

 It should be noted that it is currently felt that the overhead on
    copying a Frame (and the knock-on effect of copying the entire
    state of the detector) is prohibitive so no copy of assignment
    functions should be provided.  This does mean that only
    references/pointers to a FrameProvider's Frame can currently be
    used by the client to access a Frame, and that a Frame accessed by
    either technique will change whenever the FrameProvider builds a
    new Frame, a client needs to be aware of the "feature".  To have
    two different Frames a client must have two FrameProviders.

 As the Frame is designed for reuse. It is the FrameProviders job to
    manage this reuse and make sure that each time a Frame is
    requested that it is "up to date".  The set of Records that can
    appear in a Frame is determined by the set of RecordProviders that
    are handed to the FrameProvider.  Therefore FrameProvider
    maintains a list of RecordProviders that are used to provide "up
    to date" Records that are placed in the Frame.  The 'insert'
    function is used to add a RecordProvider to a FrameProvider, while
    the 'erase' function is used to remove a single RecordProvider and
    the 'eraseAll' function removes all RecordProviders.  The
    'contains' function can be used to see if a particular Stream has
    a matching RecordProvider inside the FrameProvider.  It should be
    noted that there can only be one RecordProvider for each Stream.

 Note: In this version of FrameProvider it owns the RecordProviders
    handed to it.  When a RecordProvider is `erase'd it is deleted.
*/
//
// Author:      Simon Patton
// Created:     Sat Nov  8 15:24:08 EST 1997
// $Id: FrameProvider.h,v 1.5 2000/03/17 15:28:50 cdj Exp $
//
// Revision history
//
// $Log: FrameProvider.h,v $
// Revision 1.5  2000/03/17 15:28:50  cdj
// Added contents and made recordProvider public. Needed for FrameIterate
//
// Revision 1.4  1999/10/09 21:51:13  cdj
// updated to doxygen style comments
//
// Revision 1.3  1998/02/03 21:28:30  mkl
// switch over to STLUtility library
//
// Revision 1.2  1998/01/23 05:18:14  mkl
// added reconfigureCurrentStop method to clear transient proxies
//
// Revision 1.1  1997/11/19 15:22:45  sjp
// New class for basic Frame Management
//

// system include files

// user include files
#include "DataHandler/Stream.h"
#include "DataHandler/Frame.h"

// forward declarations
#include "STLUtility/fwd_map.h"
class SyncValue ;
class RecordProvider ;

#  define _frameprovider_recordproviders_ STL_MAP( Stream::Type , RecordProvider* )

class FrameProvider
{
      // friend classes and functions

   public:
      // constants, enums and typedefs

      // Constructors and destructor
      FrameProvider();
      virtual ~FrameProvider();

      // member functions
      virtual Frame& currentFrame() ;

      void reconfigureCurrentStop(); // reconfigure proxies at current stop
      RecordProvider& recordProvider( const Stream::Type& aStream ) ;

      // const member functions
      virtual const Frame& currentFrame() const ;
      DABoolean contains( const Stream::Type& aStream ) const ;
      Stream::Set contents() const;

      // static member functions

   protected:
      // protected member functions
      Frame& frame( const Stream::Type& aStop ,
		    const SyncValue& aSyncValue ) ;
      Frame& emptyFrame() ;
      void insert( RecordProvider* pProvider ) ;
      void erase( const Stream::Type& aStream ) ;
      void eraseAll() ;

      // protected const member functions

   private:
      // Constructors and destructor
      FrameProvider( const FrameProvider& ); // stop default

      // assignment operator(s)
      const FrameProvider& operator=( const FrameProvider& ); // stop default

      // private member functions

      // private const member functions

      // data members
      Frame m_frame ;
      _frameprovider_recordproviders_& m_providers;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "DataHandler/Template/FrameProvider.cc"
//#endif

#endif /* DATAHANDLER_FRAMEPROVIDER_H */
