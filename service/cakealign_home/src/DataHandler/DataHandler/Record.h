#if !defined(DATAHANDLER_RECORD_H)
#define DATAHANDLER_RECORD_H
// -*- C++ -*-
//
// Package:     DataHandler
// Module:      Record
// 
/**\class Record Record.h DataHandler/Record.h

 Description: Container for all DataProxies that have the same lifetime

 Usage:
 This class contains the DataFields that make up a given Record.  It
    is designed to be reused time after time, rather than it being
    destroyed and a new one created every time a new Record is
    required.  Transient Proxies can be added at any time using the
    'insert' function provided by RecordMethods, which in turn called the
    'insert' function of the Record.  On the other hand durable
    DataFields can only be added by the RecordProvider class which
    uses the 'add' function to do this.  The reason for this is
    that the RecordProvider/ProxyProvider pair are responsible for
    "invalidating" all durable DataFields in a Record.  When a Record
    becomes "invalid" the RecordProvider must remove all transient
    Proxies using the 'removeTransients' function, and invalidate
    all the durable Proxies which it does using the ProxyProvider.
    To recover a Proxy from a Record, independent of whether it is
    transient or durable, the 'find' function of RecordMethods should be
    used, which calls the 'find' of the Record.

 When the set of durable Proxies for a Records changes, i.e. a
    ProxyProvider is added of removed from the system, then the
    durable Proxies in a Record need to be changes as appropriate.
    In this design it was decided the easiest way to achieve this was
    to erase all durable Proxies in a Record.  When this happens
    the should never be any transient Proxies in the Record, but to
    be on the safe side all Proxies should be removed so the
    'eraseAll' function does this task.

 It is important for the management of the Records that each Record
    know the Stream it is in and the SyncValue that represents the
    time it was recorded.  The Stream is defined at a Records creation
    time, while its SyncValue is set by its RecordProvider using the
    'setSyncValue' function.  These quantities can be recovered
    through 'stream' and 'syncValue' functions respectively.

 It is important for the management of the Records that each Record
    know the Stream it is in and the SyncValue that represents the
    time it was recorded.  These quantities can be accessed through
    'stream' and 'syncValue' functions respectively.

 For a DataField to be able to derive its contents from a Frame, it
    must be able to access any DataField (and thus any Record) in the
    Frame.  The 'faultHandler' function of a Proxy provides its
    containing Record as one of its arguments.  To be able to
    access the rest of the Frame, it is necessary for a Record to be
    able to access its containing Frame.  This task is handled by the
    'frame' function.  The Frame is responsible for managing this
    using the 'setFrame' and 'clearFrame'functions.

*/
//
// Author:      Simon Patton
// Created:     Wed Oct 30 10:36:08 EST 1996
// $Id: Record.h,v 1.20 2003/05/31 23:46:41 cdj Exp $
//
// Revision history
//
// $Log: Record.h,v $
// Revision 1.20  2003/05/31 23:46:41  cdj
// Using change in syncValue() to determine if a Record has changed is insufficient.  I added the method Record::cacheIdentifier() which returns a unique value each time a Record is cleared.
//
// Revision 1.19  1999/10/09 21:51:14  cdj
// updated to doxygen style comments
//
// Revision 1.18  1999/04/05 17:49:13  cdj
// now holds a non-const pointer to the Frame
//
// Revision 1.17  1998/09/30 21:07:48  cdj
// added iterator
//
// Revision 1.16  1998/09/25 19:31:50  cdj
// no longer use ProxyDict
//
// Revision 1.15  1998/02/03 21:28:32  mkl
// switch over to STLUtility library
//
// Revision 1.14  1997/11/19 15:02:25  sjp
// Updated doc and interface for RecordProvider use
//
// Revision 1.13  1997/09/18 21:10:31  cdj
// changed Boolean to DABoolean
//
// Revision 1.12  1997/09/03 20:06:25  sjp
// Uses CPP macros for STL containers
//
// Revision 1.11  1997/08/14 11:42:54  sjp
// Mods to run under CW Pro 1
//
// Revision 1.10  1997/08/06 14:51:21  sjp
// Changed to handle Proxies, and be a subclass of ProxyDict
//
// Revision 1.9  1997/07/24 16:17:50  sjp
// Modified ready for conversion to Ifd:
//      datafield -> get
//      add -> insert
//      replace -> add
//      xxxContainer -> xxxFrame
//   removed "canContains", whose function is now taken by "find".
//
// Revision 1.8  1997/07/09 17:59:52  sjp
// New <Package>/<File>.h structure
// Modified so can also compile under cxx v5.5
//
// Revision 1.7  1997/06/27 18:36:00  sjp
// Modified so DataField knows in which Frame it is living
//
// Revision 1.6  1997/06/03 17:25:53  sjp
// Fixed potential bug in Record by adding canContain
//
// Revision 1.5  1997/04/19 20:46:00  sjp
// Updated documentation
//
// Revision 1.4  1997/04/04 16:39:22  sjp
// Added SyncValue to Record
//
// Revision 1.3  1997/04/01 16:08:26  sjp
// used new names for functions
//
// Revision 1.2  1997/03/29 16:22:11  sjp
// Renamed some functions, improved docuemtation
//
// Revision 1.1  1997/03/19 22:18:23  sjp
// New header file for class
//

// system include files

// user include files

#include "DataHandler/Stream.h" // Stream::Type
#include "DataHandler/SyncValue.h"  // data member
#include "DataHandler/DataKey.h" //data member

// forward declarations
#include "STLUtility/fwd_map.h"
#include "STLUtility/fwd_set.h"
class Frame ;
class ProxyBase;
class RecordKeyItr;

class Record 
{
      // friend classses and functions
//
// only Providers can `insert' to Records
//
      friend class RecordProvider ;
      friend class Frame ;

   public:
      // constants, enums and typedefs
      typedef RecordKeyItr const_key_iterator;
      enum { kCacheIdentifierNotSet=0};

      // Constructors and destructor
      virtual ~Record() ;

      // member functions
      DABoolean insert( const DataKey& aKey ,
			const ProxyBase* pProxy ) ;
      // adds a transient Proxy to the record

      // const member functions
      const Stream::Type& stream() const ;
      Frame& frame() const ;
      const SyncValue& syncValue() const ;

      //If you are caching data from the Record, you should also keep
      // this number.  If this number changes then you know that
      // the data you have cached is invalid. This is NOT true if
      // if the syncValue() hasn't changed since it is possible that
      // the job has gone to a new Record and then come back to the
      // previous SyncValue and your algorithm didn't see the intervening
      // Record.
      unsigned long cacheIdentifier() const { return m_cacheIdentifier;}

      const ProxyBase* find( const DataKey& aKey ) const ;

      const_key_iterator begin_key() const;
      const_key_iterator end_key() const;
      // static member functions

   protected:
      // protrected constructor (only used by RecordProvider)
      Record( const Stream::Type& aStream ) ;

      // protected member functions
//
// The following member functions should only be used by RecordProvider
      DABoolean add( const DataKey& aKey ,
		     const ProxyBase* pProxy ) ;
      void eraseTransients() ;
      void removeAll() ;
      void setSyncValue( const SyncValue& aSyncValue ) ;
//
// The following member functions should only be used by Frame
      DABoolean setFrame( Frame& aFrame ) ;
      void clearFrame() ;
//

      // protected const member functions

   private:
      // Constructors and destructor
      Record() ; // stop default
      Record( const Record& ) ; // stop default

      // assignment operator(s)
      const Record& operator=( const Record& ) ; // stop default

      // private member functions

      // private const member functions

      // data members
      Stream::Type m_stream ;
      SyncValue m_syncValue ;
      STL_MAP( DataKey , const ProxyBase* )& m_visible ;
      STL_MAP( DataKey , const ProxyBase* )& m_displaced ;
      STL_SET( DataKey )& m_extras ;
      Frame* m_frame ;
      const ProxyBase* m_latestProxy ;
      DataKey m_latestKey ;
      unsigned long m_cacheIdentifier;

      // static data members

};

// inline function definitions

#endif /* DATAHANDLER_RECORD_H */
