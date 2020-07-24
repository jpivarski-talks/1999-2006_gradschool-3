#if !defined(DATAHANDLER_STREAMTYPE_H)
#define DATAHANDLER_STREAMTYPE_H
// -*- C++ -*-
//
// Package:     DataHandler
// Module:      StreamType
// 
/**\class StreamType StreamType.h DataHandler/StreamType.h

 Description: Class to encapsulate the concept of a Stream type.
              

 Usage:
    Used by Stream.

    When declaring a new StreamType instance, we strongly suggest that
     you declare it as a globally accessable const (either as a global
     variable or as a static const member data to a class) and then only
     refer to that Stream using the const value.

     Code example
\code
     // Somewhere in your .cc file
     //   Make a "mystream" with priority equal to an Event
     const StreamType kMyStream("mystream", Stream::kEventPriority );
     
     ...
 
     //Somewhere in your code
     if (kMyStream == inputStream) {...}
\endcode
    NOTE: You can not do
\code
     const StreamType kMyStream("mystream",
                                Stream::kEvent.priority() );
\endcode
    Because the order in which global (or static) data is initialized
    is NOT guaranteed.

    There are two reasons to make your Stream a globally accessible const
    1) Efficiency: Every time you instantiate a new StreamType the system
                   does a number of string comparisons.  This can be 
                   computationally expensive
    2) Don't have to worry about processing order.
        Say you use your new StreamType in two different places in your code
        but you only set the priority in one of those places.  But now
        you must be very careful to make sure that the routine containing
        the StreamType which sets the priority must be executed 
        before the routine containing the StreamType which does not set
        the priority.  If this is not the case, the StreamType will get
        the default priority and when you try to set the priority later,
        the program will exit with an error.


   STREAM PRIORITIES

   What is priority?
    Stream priorities are used when Records in different Active Streams
    have the same SyncValue.  When that occurs, we need to produce a Frame
    which stops at each Record, but in what order should we produce the
    Frames?  The answer is we start with the highest priority Stream and
    work our way down.  If two Streams have identical priority then we do
    not guarantee the order in which their Frames are produced.

   Assigning priorities
    A rule of thumb for assigning priorities is "the longer a Record lives
    the higher its priority".  For example, the Record containing the
    description of the perfectly aligned detector geometry probably has
    the longest lifetime (i.e. for the duration of the experiment) and 
    therefore should have the highest priority.  On the other hand an
    Event Record only has meaning for the exact instant at which it was
    recorded and therefore it should probably have the lowest priority.

*/
//
// Author:      Chris D. Jones
// Created:     Tue Apr 29 11:47:04 EDT 1997
// $Id: StreamType.h,v 1.20 2002/12/09 16:22:11 cleo3 Exp $
//
// Revision history
//
// $Log: StreamType.h,v $
// Revision 1.20  2002/12/09 16:22:11  cleo3
// declare specialization of less from within the std namespace
//
// Revision 1.19  2001/09/07 16:52:13  cleo3
// used proper template specialization syntax
//
// Revision 1.18  1999/12/31 18:59:23  cdj
// fixed doxygen header
//
// Revision 1.17  1999/10/09 21:51:15  cdj
// updated to doxygen style comments
//
// Revision 1.16  1998/06/24 17:13:21  cdj
// changed less<StreamType> to be a struct (needed by cxx 6.0)
//
// Revision 1.15  1998/04/01 15:44:21  mkl
// switched to C++Std standard include area: fwd_string/istream/ostream
//
// Revision 1.14  1998/02/03 21:28:33  mkl
// switch over to STLUtility library
//
// Revision 1.13  1997/09/18 21:10:36  cdj
// changed Boolean to DABoolean
//
// Revision 1.12  1997/09/03 20:35:35  sjp
// Added functional header for HP to work
//
// Revision 1.11  1997/09/03 20:06:28  sjp
// Uses CPP macros for STL containers
//
// Revision 1.10  1997/09/01 16:23:11  sjp
// Moved TBIdentifier to UTIdentifier
//
// Revision 1.9  1997/08/30 18:59:13  cdj
// Bug fix: #define for vector now checks NO_ALLOCATOR_IN_STL_DECLARATION_BUG
//
// Revision 1.8  1997/08/27 03:38:52  sjp
// Updated to new name for bug flags
//
// Revision 1.7  1997/08/14 11:42:57  sjp
// Mods to run under CW Pro 1
//
// Revision 1.6  1997/07/17 18:42:40  sjp
// Cleaned up problem with static data in template class
//
// Revision 1.5  1997/07/09 18:00:04  sjp
// New <Package>/<File>.h structure
// Modified so can also compile under cxx v5.5
//
// Revision 1.4  1997/06/29 20:17:48  cdj
// Added priority and isStandard capabilities
//
// Revision 1.3  1997/05/07 19:00:05  sjp
// Fixed less<Stream::Type> so can be used as usual
//
// Revision 1.2  1997/05/07 16:57:52  sjp
// Cleaned up include file dependencies
//
// Revision 1.1  1997/04/29 19:46:04  cdj
// First submission
//

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <string>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Utility/UTIdentifier.h"

// STL classes
#if defined(OLD_CXX_STRING_CLASS_BUG)
#include <C++Std/fwd_string.h>
#include <String.h>
#else
#include <string>
#endif /* OLD_CXX_STRING_CLASS_BUG */

// forward declarations
#include "C++Std/fwd_ostream.h"
#include "STLUtility/fwd_vector.h"
class Stream ;

#if !defined(_streamtype_priorities_)
#define _streamtype_priorities_ STL_VECTOR( StreamType::Priority )
#define _streamtype_arestandard_ STL_VECTOR( DABoolean )
#endif /* _streamtype_priorities_ */


class StreamType : public UTIdentifier< string, Stream >
{
      // friend classses and functions

   public:
      // constants, enums and typedefs
      typedef Integer Priority;
      enum { kDefaultPriority=0 };

      // Constructors and destructor
      StreamType( void );
      StreamType( const string& iString );
      StreamType( const string& iString, 
		  Priority iPriority,
		  DABoolean iIsStandard=false);
      virtual ~StreamType();

      // member functions

      // const member functions
      Priority priority( void ) const;
      DABoolean isStandard( void ) const;

   protected:
      // protected member functions

      // protected const member functions

   private:
      // Constructors and destructor
      //StreamType( const StreamType& );

      // assignment operator(s)
      //const StreamType& operator=( const StreamType& );

      // private member functions
      Priority findPriority( void ) const;
      DABoolean findIsStandard( void ) const;

      // private const member functions
      void setPriority( const Priority& iPriority);
      void setIsStandard( const DABoolean iIsStandard);

      // static member functions
      static _streamtype_priorities_& priorities( void ) ;
      static _streamtype_arestandard_& standards( void ) ;

      // data members
      Priority m_priority;

      // static data members

};

// inline function definitions

ostream& operator<<( ostream& iOStream, const StreamType& iStreamType );
istream& operator>>( istream& iIStream, StreamType& iStreamType ) ;
      
// needed so forward decleration in Stream.h can be done

// system include files
#include <functional>

// user include files
#include "Utility/UTIdentifierLess.h"

#if !defined( NO_STD_NAMESPACE_BUG)
namespace std {
#endif
template<>
struct less< StreamType> : public UTIdentifierLess< string, Stream >
{
};
#if !defined( NO_STD_NAMESPACE_BUG)
}
#endif

#endif /* DATAHANDLER_STREAMTYPE_H */
