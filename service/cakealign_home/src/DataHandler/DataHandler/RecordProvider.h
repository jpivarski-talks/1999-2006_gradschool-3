#if !defined(DATADELIVERY_RECORDPROVIDER_H)
#define DATADELIVERY_RECORDPROVIDER_H
// -*- C++ -*-
//
// Package:     DataHandler
// Module:      RecordProvider
// 
/**\class RecordProvider RecordProvider.h DataHandler/RecordProvider.h

 Description: Creates and manages Records.

 Usage:
 This class manages a Record and ensures that it is "up to date".
    FrameProvider uses this class to provide Records that are placed
    in the Frame.

 To be able to fill the Record with durable DataFields, the
    RecordProvider needs to maintain a list of ProxyProviders that can
    provide Proxies for the appropraite Stream.  The 'add' function is
    used to add a ProxyProvider to a RecordProvider, while the
    'remove' function is used to remove a ProxyProvider.  The
    'isEmpty' function is true if a RecordProvider has no //
    ProxyProviders.  Whenever the set of ProxyProviders is changes the
    Record has to be reconfigured, i.e. the durable DataFields are
    emptied and re-filled so that the Record reflects the new
    configuration.  It is possible to force a reconfiguation without
    changing the ProxyProvider set by calling the 'reconfigure'
    function.

 Whenever a record is requested by a call to the 'record' function,
    the RecordProvider polls all its ProxyProviders to find the
    "latest" SyncValue they can provide that is not higher than the
    requested SyncValue, using the ProxyProviders 'latest' function.
    The result of this poll is the SyncValue used from the returned
    Record.  If this SyncValue is the same are the current,
    i.e. previous, SyncValue for that Record then the RecordProvider
    does nothing else.  However if the SyncValue is different then the
    RecordProvider clears the Record of its transient DataFields,
    using the Record's 'removeTransients' function, and uses each
    ProxyProvider's 'align' function to align their DataFields to the
    this new SyncValue.

 Note: The Record pointer returned by 'record' is not const, this
    allows transient DataFields to be added to the Record at a later
    time.
 Note: The Record pointer returned by 'record' may be 0 if the record
    fails to syncrhonize or the RecordProvider has no DataProviders.
    Switched from using exceptions because if exception thrown often
    the job is extremely slowed down (OSF cxx 6.2 and Sun CC 4.2)
*/
//
// Author:      Simon Patton
// Created:     Thu Oct 30 16:41:02 EST 1997
// $Id: RecordProvider.h,v 1.7 2002/03/08 20:25:02 cdj Exp $
//
// Revision history
//
// $Log: RecordProvider.h,v $
// Revision 1.7  2002/03/08 20:25:02  cdj
// speed improvement: under perverse conditions, was throwing many exceptions per event
//
// Revision 1.6  2000/06/03 21:20:56  cdj
// now issue warning and continue if a Record that is supposed to be in the Frame fails to synchronize
//
// Revision 1.5  2000/03/17 15:28:00  cdj
// Added resetAllCachesOfProxies and unsynchronizedRecord. Needed for FrameIterate
//
// Revision 1.4  2000/01/22 02:34:24  cdj
// added contains method to RecordProvider
//
// Revision 1.3  1999/10/09 21:51:15  cdj
// updated to doxygen style comments
//
// Revision 1.2  1998/02/03 21:28:32  mkl
// switch over to STLUtility library
//
// Revision 1.1  1997/11/19 15:23:20  sjp
// Class to replace RecordDeliverer in DataDelivery
//

// system include files

// user include files
#include "DataHandler/Stream.h"

// forward declarations
#include "STLUtility/fwd_vector.h"
class SyncValue ;
class Record ;
class Frame ;
class ProxyProvider ;

class RecordProvider
{
      // friend classses and functions

   public:
      // constants, enums and typedefs
      typedef STL_VECTOR( ProxyProvider* ) Providers ;

      // Constructors and destructor
      RecordProvider( const Stream::Type& aStream ) ;
      virtual ~RecordProvider() ;

      // member functions
      void add( ProxyProvider& aProvider ) ;
      void remove( ProxyProvider& aProvider ) ;
      void reconfigure() ;
      ///causes all the Proxies to clear all of their different caches
      void resetAllCachesOfProxies();

      // const member functions
      const Stream::Type& stream() const ;
      DABoolean isEmpty() const;

      ///returns 0 if synchronization fails
      Record* record( const SyncValue& aNow ) const ;

      ///use this method if you can use an unsynchronized Record
      const Record& unsynchronizedRecord() const;

      DABoolean contains( const ProxyProvider& aProvider ) const;

      // static member functions
      static Record* createRecord( const Stream::Type& aStream ) ;

   protected:
      // protected member functions

      // protected const member functions

   private:
      // Constructors and destructor
      RecordProvider(); // stop default
      RecordProvider( const RecordProvider& ) ; // stop default

      // assignment operator(s)
      const RecordProvider& operator=( const RecordProvider& ) ; // stop default

      // private member functions
      void fill( const ProxyProvider& aProvider ) ;

      // private const member functions

      // data members

      // static data members
      Stream::Type m_stream ;
      Record& m_record ;
      Providers& m_providers ;
      DABoolean m_needReconfigure ;

};

// inline function definitions

#endif /* DATADELIVERY_RECORDPROVIDER_H */
