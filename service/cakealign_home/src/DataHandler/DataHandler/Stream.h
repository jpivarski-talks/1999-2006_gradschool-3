#if !defined(DATAHANDLER_STREAM_H)
#define DATAHANDLER_STREAM_H
// -*- C++ -*-
//
// Package:     DataHandler
// Module:      Stream
// 
// Description: Class used to define a name space for Stream objects
//
// Usage:
//    Stream::Type : denotes which stream you are refering to 
//                    Ex.  "event" or "geometry"
//                   
//    Stream::Set  : denotes a collection of Stream::Types
//
// Author:      Chris D Jones
// Created:     Wed Mar 12 15:52:25 EST 1997
// $Id: Stream.h,v 1.11 1997/11/19 15:16:54 sjp Exp $
//
// Revision history
//
// $Log: Stream.h,v $
// Revision 1.11  1997/11/19 15:16:54  sjp
// Moved Experiment dependent info to Experiment dir
//
// Revision 1.10  1997/07/09 17:59:56  sjp
// New <Package>/<File>.h structure
// Modified so can also compile under cxx v5.5
//
// Revision 1.9  1997/06/29 20:22:45  cdj
// 1) Removed Stream::kNoStops
// 2) Added priorities to Standard Streams
// 3) moved isStandard() to Stream::Type
//
// Revision 1.8  1997/05/07 19:00:09  sjp
// Fixed less<Stream::Type> so can be used as usual
//
// Revision 1.7  1997/05/07 16:57:51  sjp
// Cleaned up include file dependencies
//
// Revision 1.6  1997/04/29 19:50:48  cdj
// redefined Stream::Type and added Stream::TypeLess
//
// Revision 1.5  1997/03/19 23:05:36  cdj
// Added isStandard method at the request of mkl
//
// Revision 1.4  1997/03/19 22:15:42  sjp
// Corrected include files to work with other compilers
//
// Revision 1.3  1997/03/18 20:26:35  cdj
// Stream::Set is now defined in terms of class StreamSet
//
// Revision 1.2  1997/03/17 22:03:05  cdj
// The constants to use to denote standard CLEO streams have been made
// static constants within the Stream class.
//
// Revision 1.1  1997/03/13 21:51:46  cdj
// First submission.
// NOTE: No compilation has been attempted on this file so syntax errors are
// possible/likely.
//

// system include files

// user include files
#include "DataHandler/StreamType.h"

// forward declerations
class NonExistantFriend; //Used to avoid compiler warnings
class StreamSet;
class StreamType;
//class StreamTypeLess;

// class definition
class Stream
{
      // friend classses and functions
      friend class NonExistantFriend;

   public:
      // constants, enums and typedefs
      typedef StreamType Type;
      typedef StreamSet Set;

#include "Experiment/StreamConstants_decl.h"

      // Constructors and destructor

      // member functions

      // const member functions

      // static member functions

   protected:
      // protected member functions
      
      // protected const member functions

   private:
      // Constructors and destructor
      Stream( void );
      Stream( const Stream& );

      // assignment operator(s)
      const Stream& operator=(const Stream& );

      // private member functions

      // private const member functions

      // data members

      // static data members

};

#endif /* DATAHANDLER_STREAM_H */

