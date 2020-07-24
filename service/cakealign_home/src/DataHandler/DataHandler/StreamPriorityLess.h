#if !defined(DATAHANDLER_STREAMPRIORITYLESS_H)
#define DATAHANDLER_STREAMPRIORITYLESS_H
// -*- C++ -*-
//
// Package:     DataHandler
// Module:      StreamPriorityLess
// 
// Description: Compares two Stream::Types based on their Priorities
//
// Usage:
//    This functional object is used to compare two Stream::Types based on 
//     their priority.  The functional object returns true if the left 
//     hand side Stream has a lower priority than the right hand side Stream.
//
//    NOTE: See Stream.h for the meaning of a Stream::Type priority
//
// Author:      Chris D. Jones
// Created:     Wed Jun 25 14:24:48 EDT 1997
// $Id: StreamPriorityLess.h,v 1.3 1997/09/18 21:10:33 cdj Exp $
//
// Revision history
//
// $Log: StreamPriorityLess.h,v $
// Revision 1.3  1997/09/18 21:10:33  cdj
// changed Boolean to DABoolean
//
// Revision 1.2  1997/07/09 17:59:59  sjp
// New <Package>/<File>.h structure
// Modified so can also compile under cxx v5.5
//
// Revision 1.1  1997/06/29 20:24:11  cdj
// First sumbission
//


// system include files

// user include files
#include "DataHandler/Stream.h"


// forward declarations

class StreamPriorityLess
{
      // friend classses and functions

   public:
      // constants, enums and typedefs

      // Constructors and destructor
      //StreamPriorityLess();
      //virtual ~StreamPriorityLess();

      // member functions

      // const member functions
      DABoolean operator()( const Stream::Type& iLHS,
			  const Stream::Type& iRHS) const;

      // static member functions

   protected:
      // protected member functions

      // protected const member functions

   private:
      // Constructors and destructor
      //StreamPriorityLess( const StreamPriorityLess& );

      // assignment operator(s)
      //const StreamPriorityLess& operator=( const StreamPriorityLess& );

      // private member functions

      // private const member functions

      // data members

      // static data members

};

// inline function definitions

#endif /* DATAHANDLER_STREAMPRIORITYLESS_H */
