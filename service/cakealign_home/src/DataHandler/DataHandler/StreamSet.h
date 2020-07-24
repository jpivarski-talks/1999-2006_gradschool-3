#if !defined(DATAHANDLER_STREAMSET_H)
#define DATAHANDLER_STREAMSET_H
// -*- C++ -*-
//
// Package:     DataHandler
// Module:      StreamSet
// 
// Description: Implements a Set of Streams
//
// Usage:
//    <usage>
//
// Author:      Chris D. Jones
// Created:     Tue Mar 18 13:59:05 EST 1997
// $Id: StreamSet.h,v 1.11 1998/02/03 21:28:33 mkl Exp $
//
// Revision history
//
// $Log: StreamSet.h,v $
// Revision 1.11  1998/02/03 21:28:33  mkl
// switch over to STLUtility library
//
// Revision 1.10  1997/09/18 21:10:35  cdj
// changed Boolean to DABoolean
//
// Revision 1.9  1997/09/03 20:06:27  sjp
// Uses CPP macros for STL containers
//
// Revision 1.8  1997/08/27 03:38:49  sjp
// Updated to new name for bug flags
//
// Revision 1.7  1997/08/14 11:42:56  sjp
// Mods to run under CW Pro 1
//
// Revision 1.6  1997/07/09 18:00:03  sjp
// New <Package>/<File>.h structure
// Modified so can also compile under cxx v5.5
//
// Revision 1.5  1997/05/07 19:00:07  sjp
// Fixed less<Stream::Type> so can be used as usual
//
// Revision 1.4  1997/04/29 19:53:42  cdj
// replaced less< String::Type > with Stream::TypeLess
//
// Revision 1.3  1997/04/10 01:05:10  sjp
// Removed extra include file that is not required
//
// Revision 1.2  1997/03/20 22:32:41  cdj
// Added the add(...) methods to simplify concatenating two StreamSets
//
// Revision 1.1  1997/03/18 20:21:41  cdj
// First submission
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <set>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "DataHandler/Stream.h"

// STL include files
#include <set>

// forward declarations
#include "STLUtility/fwd_set.h" // for CPP macros

#if !defined(_streamset_types_)
#  define _streamset_types_ STL_SET( Stream::Type )
#endif /* _streamset_types_ */


class StreamSet : public _streamset_types_
{
      // friend classses and functions

   public:
      // constants, enums and typedefs

      // Constructors and destructor
      StreamSet( void );
      //virtual ~StreamSet();

      // assignment operator(s)
      const StreamSet& operator=( const StreamSet& iSet );

      // member functions
      StreamSet& add( const StreamSet& iSet );
      StreamSet& add( const Stream::Type& iType );

      // const member functions
      DABoolean contains( const Stream::Type& iType ) const;
      DABoolean contains( const StreamSet& iSet ) const;

      // static member functions

   protected:
      // protected member functions

      // protected const member functions

   private:
      // Constructors and destructor
      //StreamSet( const StreamSet& );

      // private member functions

      // private const member functions

      // data members

      // static data members
      
};

// inline function definitions

#endif /* DATAHANDLER_STREAMSET_H */
