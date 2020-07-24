#if !defined(DATAHANDLER_PROXYPROVIDER_H)
#define DATAHANDLER_PROXYPROVIDER_H
// -*- C++ -*-
//
// Package:     DataHandler
// Module:      ProxyProvider
// 
/**\class ProxyProvider ProxyProvider.h DataHandler/ProxyProvider.h

 Description: Manages the creation and provision of non-tranient Proxies

 Usage:
 This class manages durable DataFields and ensures that they are "up
    to date".  It was expected that classes that can provide DataField
    may provide them for more than one Stream.  For example, a
    database class the provides the contents of two or more Streams,
    or a class may wish to provide DataFields to the "event" Stream
    and also provide, when run online, a summary DataField and part of
    the "end run" Stream.  Therefore, it was decided that a
    ProxyProvider should be able to provide DataFields for more than a
    single stream.  While is would have been possible to have a
    ProxyProvider for each Stream, and collect these together in a
    separate class, it was not felt that this held any advantage, so
    this possible design was not used.  One obvious artifact of this
    decision is that most member function of a ProxyProvider take a
    Stream argument to state upon which Stream the function should be
    executed.

 This class is an abstract class and so a concrete subclass must be
    created for each object that wishes to provide durable DataFields.
    This concrete subclass must provide the necessary implementations
    to tell a ProxyProvider which Stream and DataFields it can supply.

 As a ProxyProvider can supply DataFields to more that one Stream, it
    is necessary for the programmer of any concrete subclass to
    register with this class the Streams for which the subclass can
    provide DataFields.  This is done by calling the 'registerStream'
    function for each Stream in the constructor of the subclass.  Once
    a Stream has been registered it is included in the return value of
    the 'supplies' function which returns all Streams the
    ProxyProvider can supply, and the return value of the 'canSupply'
    function will be "true" for that Stream.

 The ProxyProvider is responsibility is to maintain a set of durable
    DataFields and their appropriate keys (this combination is called
    a KeyedProxy) for each Stream so that they can be placed into a
    Record when it is configured, and invalidated them every time that
    Records changes. This set of KeyedProxies is filled by the
    concrete subclass providing an implementation of the
    'registerProxies' function in which the DataFields appropriate to
    the provided Stream are added to the provided set.  This function
    is called the first time the relevant Record is configured.  Once
    this function has been executed, it is not normally executed again
    (this avoids repeated creating of Proxies which would be
    expensive).  If, for some reason, there is a need to change the
    set of durable DataFields after the function has executed, then a
    call to 'expireProxies' will force 'registerProxies' to be
    re-called the next time the appropriate Record is configured.  A
    RecordProvider uses the 'keyedProxies' function to recover the
    list of durable DataFields for its particular Stream.

 Whenever a record is requested from a RecordProvider, it polls all
    its ProxyProviders to find the "latest" SyncValue they can provide
    that is not higher than the requested SyncValue, using the
    ProxyProviders 'latest' function.  The concrete subclass need to
    provide an implementation of this function.  There are two
    standard choices that depend on the behavior of each Stream
    provided by the ProxyProvider.
 - If one or more DataFields in a particular Stream are "tied" to a
      particular SyncValue, i.e. the contents of that DataField have
      already been determined and are only correct for a particular
      SyncValue, then this means that the Stream itself tied to that
      SyncValue.  Note that each Stream in a ProxyProvider can only be
      tied to one SyncValue.  In this case if the SyncValue to which
      the Stream is tied is between the 'aCurrentLatest' and the
      'aLimit' arguments then the function should return the SyncValue
      to which it is tied, otherwise it should simply return the
      'aCurrentLatest' value.
 - If all the DataFields in a particular Stream are not "tied" to a
      particular SyncValue, i.e. the contents is determine only once
      the Record's SyncValue is known then the Stream is not tied to a
      particular SyncValue, but will use whatever SyncValue is
      assigned to the Record.  In this case the implementation of the
      function should simply return the 'aCurrentLatest' value.

 Once the latest SyncValue has been determined it will be used for the
    RecordProvider's Record.  If this SyncValue is the same are the
    RecordProvider's current, i.e. previous, SyncValue for that Record
    then the RecordProvider does nothing else.  However if the
    SyncValue is different then the RecordProvider calls each
    ProxyProvider's 'align' function for that Stream to align their
    DataFields to the this new SyncValue.  Again there are two
    standard options, though they are subtly different from those for
    the 'latest' function.
 - DataFields that already have a SyncValue associated with them,
      e.g. their contents have been stored outside the current
      instance of the Frame, should be either "enabled" or
      "disabled" depending on whether their SyncValue matches that
      to which they are being aligned.
 - DataFields that only use whatever the SyncValue is assigned to the
      Record simply need to be invalidated.  The
      'invalidateProxies' function does this task for all
      DataFields associated with a particular Stream.
 Note that it is the responsibility the concrete subclass must take
    care of invalidating its Proxies at the appropriate time.

*/
//
// Author:      Simon Patton
// Created:     Thu Mar 13 09:26:26 EST 1997
// $Id: ProxyProvider.h,v 1.18 2000/03/17 15:24:43 cdj Exp $
//
// Revision history
//
// $Log: ProxyProvider.h,v $
// Revision 1.18  2000/03/17 15:24:43  cdj
// RecordProvider is now a friend. Needed for FrameIterate
//
// Revision 1.17  1999/10/09 21:16:10  cdj
// changes needed for production_tag usage
//   -moved eraseAll() from private to protected so we can make new Proxies
//     with the correct production_tag when the tag changes
// updated documentation to work with doxygen
//
// Revision 1.16  1998/09/25 19:31:47  cdj
// no longer use ProxyDict
//
// Revision 1.15  1998/07/18 20:04:11  mkl
// solaris gets confused by vector in map
//
// Revision 1.14  1998/04/01 15:44:20  mkl
// switched to C++Std standard include area: fwd_string/istream/ostream
//
// Revision 1.13  1998/02/03 21:28:31  mkl
// switch over to STLUtility library
//
// Revision 1.12  1997/11/19 15:04:13  sjp
// Added facility to expire Proxies allowing reloading
//
// Revision 1.11  1997/09/18 21:10:29  cdj
// changed Boolean to DABoolean
//
// Revision 1.10  1997/09/03 20:06:23  sjp
// Uses CPP macros for STL containers
//
// Revision 1.9  1997/08/27 03:38:47  sjp
// Updated to new name for bug flags
//
// Revision 1.8  1997/08/14 11:42:52  sjp
// Mods to run under CW Pro 1
//
// Revision 1.7  1997/08/06 15:27:39  sjp
// Updated documentation to deal with Proxies
//
// Revision 1.6  1997/08/06 14:50:40  sjp
// Changed to handle Proxies
//
// Revision 1.5  1997/07/22 17:38:32  sjp
// Include <vector> as it is required
//
// Revision 1.4  1997/07/15 03:19:16  mkl
// added fwd_string.h
//
// Revision 1.3  1997/07/10 16:22:03  sjp
// fix spelling error in bug flag
//
// Revision 1.2  1997/07/09 18:00:49  sjp
// New <Package>/<File>.h structure
// Modified so can also compile under cxx v5.5
//
// Revision 1.1  1997/06/03 17:32:03  sjp
// new class to add DataFields to Records
//

// system include files
#include <utility> // for pair
#if defined(AMBIGUOUS_STRING_FUNCTIONS_BUG)
#include <string>
#endif /* AMBIGUOUS_STRING_FUNCTIONS_BUG */
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <string>
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */ 

// user include files
#include "DataHandler/DataKey.h" // needed as class is actually typedefs
#include "DataHandler/ProxyBase.h" // needed as class is actually typedefs
#include "DataHandler/Stream.h" // for Stream::Type
#include "DataHandler/StreamSet.h" // return type for `supplies'
#include "DataHandler/KeyedProxy.h"

// STL include
#if defined(OLD_CXX_STRING_CLASS_BUG)
#include "C++Std/fwd_string.h"
#include <String.h>
#else
#include <string>
#endif /* OLD_CXX_STRING_CLASS_BUG */
#include <vector>

// forward declarations
#include "STLUtility/fwd_vector.h"
#include "STLUtility/fwd_map.h"
class Record ;
class SyncValue ;
class RecordProvider;

#if !defined(_proxyprovider_proxies_)
typedef STL_VECTOR( KeyedProxy ) KeyedProxyVector;
#  define _proxyprovider_proxies_ STL_MAP( Stream::Type , KeyedProxyVector )
//#  define _proxyprovider_proxies_ STL_MAP( Stream::Type , STL_VECTOR( KeyedProxy )  )
#  define _proxyprovider_expired_ STL_MAP( Stream::Type , DABoolean )
#endif /* _proxyprovider_proxies_ */

class ProxyProvider
{
      // friend classses and functions
      friend class RecordProvider;  //can call expireProxies

   public:
      // constants, enums and typedefs
      typedef string Identifier ;
      
      typedef STL_VECTOR( KeyedProxy ) KeyedProxies ;

      enum { kDefaultNumberOfStreams = 1 ,
	     kDefaultNumberOfProxies = 4
      } ;

      // Constructors and destructor
      ProxyProvider( const Identifier& aId ) ;
      virtual ~ProxyProvider() ;

      // member functions
      virtual void align( const Stream::Type& aStream ,
			  const SyncValue& aSyncValue ) = 0 ;

      // const member functions
      const Identifier& identifier() const ;
      DABoolean canSupply( const Stream::Type& aStream ) const ;
      Stream::Set supplies( void ) const;
      const KeyedProxies& keyedProxies( const Stream::Type& aStream ) const ;
      virtual const SyncValue& latest( const Stream::Type& aStream ,
				       SyncValue& aCurrentLatest ,
				       const SyncValue& aLimit ) const = 0 ;

      // static member functions

   protected:
      // protected member functions
      void invalidateProxies( const Stream::Type& aStream ) ;
      void registerStream( const Stream::Type& aStream ) ;
      void expireProxies( const Stream::Type& aStream ) ;
      virtual void registerProxies( const Stream::Type& aStream ,
				    KeyedProxies& aProxyList ) = 0 ;

      ///deletes all the Proxies in aStream
      void eraseAll( const Stream::Type& aStream ) ;

      // protected const member functions

   private:
      // Constructors and destructor
      ProxyProvider() ; // stop default
      ProxyProvider( const ProxyProvider& ) ; // stop default

      // assignment operator(s)
      const ProxyProvider& operator=( const ProxyProvider& ) ; // stop default

      // private member functions
      
      // private const member functions
      DABoolean isExpired( const Stream::Type& aStream ) const ;
      
      // data members
      Identifier m_id ;
      _proxyprovider_proxies_& m_streamProxies ;
      _proxyprovider_expired_& m_expired ;

      // static data members

};

// inline function definitions

#endif /* DATAHANDLER_PROXYPROVIDER_H */
