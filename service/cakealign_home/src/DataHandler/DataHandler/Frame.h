#if !defined(DATAHANDLER_FRAME_H)
#define DATAHANDLER_FRAME_H
// -*- C++ -*-
//
// Package:     DataHandler
// Module:      Frame
// 
/**\class Frame Frame.h DataHandler/Frame.h

 Description: A Container of Records

 Usage:
 This class contains all available and "valid" Records.  A "valid"
    Records has two criteria:
      - it is the most recent one in a given stream;
      - it has not been invalidated by the arrival of a Record in
        another Stream.  
    To recover a Records from the Frame the client uses the 'record'
    function, specifying the Stream of the Records they require.  The
    client can check that the Frame hold the Records of interest by
    using the 'contains' function.

 The Frame in this design has been designed to be reused time after
    time, rather than it being destroyed and a new one created every
    time a new Frame is requested.  To encapsulate this reuse, a
    client should only ever get a Frame from a FrameProvider, they
    should never create their own Frame. If this design does change to
    a create/destroy pattern the client will then be insulated behind
    the FrameProviders interface.

 The FrameProvider class also handles the addition of Records to a
    Frame, using the 'add' function, a task that should not be done
    directly by a client.  Similarly the FrameProvider class is the
    only class that can remove Records from a Frame.  It was decided
    that it is easier to completely refill the Frame at every
    SyncValue rather than try to work out which Records should be add
    at a given Stop and which should be removed.  The 'removeAll'
    function is used by the FrameProvider to remove all Records in a
    Frame ready to be refilled with the Records that are appropriate
    for that requested Stop.  The FrameProvider also uses the
    'setStop' and 'setSyncValue' functions to set the appropriate
    values for the Frame.

 The 'contains' function is provided to see if a Frame contains a
    valid Record for a particular Stream.  If such a Record does exist
    then it can be recovered from the Frame by using the 'record'
    function.  A client can use the 'syncValue' function to recover
    the SyncValue that was used to build a Frame, while the
    'stopStream' function allows the client to tell which Stream was
    the Frame's Stop Stream.  These values are set by the
    FrameProvider calling the 'setValue' and 'setStop' functions.
*/
//
// Author:      Simon Patton
// Created:     Thu Sep 26 10:53:49 EDT 1996
// $Id: Frame.h,v 1.15 2001/11/06 18:45:38 cdj Exp $
//
// Invariants:
//   can only contain one record for each Stream
//
// Revision history
//
// $Log: Frame.h,v $
// Revision 1.15  2001/11/06 18:45:38  cdj
// can now iterator over Records in a Frame
//
// Revision 1.14  1999/10/09 21:51:12  cdj
// updated to doxygen style comments
//
// Revision 1.13  1998/02/03 21:28:30  mkl
// switch over to STLUtility library
//
// Revision 1.12  1997/11/19 15:02:04  sjp
// Updated doc and interface for FrameProvider use
//
// Revision 1.11  1997/09/18 21:10:28  cdj
// changed Boolean to DABoolean
//
// Revision 1.10  1997/09/03 20:06:21  sjp
// Uses CPP macros for STL containers
//
// Revision 1.9  1997/08/14 11:42:51  sjp
// Mods to run under CW Pro 1
//
// Revision 1.8  1997/07/09 17:59:51  sjp
// New <Package>/<File>.h structure
// Modified so can also compile under cxx v5.5
//
// Revision 1.7  1997/05/07 19:00:13  sjp
// Fixed less<Stream::Type> so can be used as usual
//
// Revision 1.6  1997/04/29 19:48:49  cdj
// replaced less<Stream::Type> with Stream::TypeLess and changed initial value
// of m_stopStream to Stream::kNone
//
// Revision 1.5  1997/04/04 16:55:21  sjp
// Added Stream include
//
// Revision 1.4  1997/04/01 16:08:25  sjp
// used new names for functions
//
// Revision 1.3  1997/03/29 16:20:32  sjp
// Renamed some functions, improved docuemtation
//
// Revision 1.2  1997/03/19 22:16:57  sjp
// Changed string to be Stream::Type
//

// system include files

// user include files
#include "DataHandler/Stream.h" 
#include "DataHandler/SyncValue.h" 

// forward declarations
#include "STLUtility/fwd_map.h"
class Record ;
class FrameRecordItr;

#if !defined(_frame_records_)
#  define _frame_records_ STL_MAP( Stream::Type , Record* )
#endif /* _frame_records_ */


class Frame
{
      // friend classses and functions
      friend class FrameProvider ;

   public:
      // constants, enums and typedefs
      typedef FrameRecordItr const_iterator;

      // Constructors and destructor
      virtual ~Frame() ;

      // member functions
      Record& record( const Stream::Type& aRecordStream ) ;

      // const member functions
      const Stream::Type& stopStream() const ;
      const SyncValue& syncValue() const ;
      DABoolean contains( const Stream::Type& aRecordStream ) const ;
      const Record& record( const Stream::Type& aRecordStream ) const ;

      const_iterator begin() const;
      const_iterator end() const;
      // static member functions

   protected:
      // protrected constructor (only used by FrameProvider)
      Frame() ;

      // protected member functions
//
// The following member functions should only be used by FrameProvider
      void add( Record& aRecord ) ;
      void setStop( const Stream::Type& aStop ) ;
      void setSyncValue( const SyncValue& aSyncValue ) ;
      void removeAll() ;

      // protected const member functions

   private:
      // Constructors and destructor
      Frame( const Frame& ) ; // stop default

      // assignment operator(s)
      const Frame& operator=( const Frame& ) ; // stop default

      // private member functions

      // private const member functions
      const Record* findRecord( const Stream::Type& aRecordStream ) const ;

      // data members
      Stream::Type m_stop ;
      SyncValue m_syncValue ;
      _frame_records_& m_records ;
      Record* m_latestRecord ;

      // static data members

};

// inline function definitions

#endif /* DATAHANDLER_FRAME_H */
