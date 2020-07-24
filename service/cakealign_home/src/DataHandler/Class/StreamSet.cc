// -*- C++ -*-
//
// Package:     DataHandler
// Module:      StreamSet
// 
// Description: Implements a set of Streams
//
// Implimentation:
//     Most of the functionality comes from set<...>
//
// Author:      Chris D. Jones
// Created:     Tue Mar 18 14:13:14 EST 1997
// $Id: StreamSet.cc,v 1.9 1998/06/23 23:11:09 mkl Exp $
//
// Revision history
//
// $Log: StreamSet.cc,v $
// Revision 1.9  1998/06/23 23:11:09  mkl
// fixed assignment operator against self-assignment
//
// Revision 1.8  1997/09/18 21:10:20  cdj
// changed Boolean to DABoolean
//
// Revision 1.7  1997/09/03 20:06:15  sjp
// Uses CPP macros for STL containers
//
// Revision 1.6  1997/08/14 11:42:44  sjp
// Mods to run under CW Pro 1
//
// Revision 1.5  1997/07/09 17:59:17  sjp
// New <Package>/<File>.h structure
// Modified so can also compile under cxx v5.5
//
// Revision 1.4  1997/05/07 19:55:54  sjp
// Fixed less<Stream::Type> so can be used as usual
//
// Revision 1.3  1997/04/29 19:53:40  cdj
// replaced less< String::Type > with Stream::TypeLess
//
// Revision 1.2  1997/03/20 22:37:08  cdj
// Added add(...) methods to simplify concatenation of StreamSets
//
// Revision 1.1  1997/03/18 20:39:02  cdj
// First submission
//

#include "Experiment/Experiment.h"
// system include files

// user include files
#include "DataHandler/StreamSet.h"


// STL classes

//
// constants, enums and typedefs
//

typedef _streamset_types_ StreamSetSet;

//
// static data member definitions
//

//
// constructors and destructor
//
StreamSet::StreamSet( void )
{
}

// StreamSet::StreamSet( const StreamSet& )
// {
// }

//StreamSet::~StreamSet()
//{
//}

//
// assignment operators
//
const StreamSet& StreamSet::operator=( const StreamSet& iSet )
{
   if( &iSet != this ) {
      StreamSetSet::operator=( iSet );
   }

   return *this;
}

//
// member functions
//
StreamSet&
StreamSet::add( const StreamSet& iSet )
{
   //g++ 2.7.2 could not do this
   //insert( iSet.begin(), iSet.end() );

   StreamSetSet::const_iterator itEnd = iSet.end();
   for( StreamSetSet::const_iterator itType = iSet.begin();
	itType != itEnd;
	++itType ){
      insert( *itType );
   }
   return *this;
}

StreamSet&
StreamSet::add( const Stream::Type& iType )
{
   insert( iType );

   return *this;
}
//
// const member functions
//
DABoolean
StreamSet::contains( const Stream::Type& iType ) const
{
   return count( iType );
}

DABoolean
StreamSet::contains( const StreamSet& iSet ) const
{
   if( iSet.size() > size() ){
      return false;
   }

   if( 0 == iSet.size() ){
      return true;
   }

   DABoolean returnValue( true );
   //Loop through each item is iSet and see if we contain it
   StreamSetSet::const_iterator itEnd = iSet.end();
   for( StreamSetSet::const_iterator itStream = iSet.begin();
	itStream != itEnd;
	++itStream ){
      if( ! contains( *itStream ) ){
	 returnValue = false;
	 break;
      }
   }
   return returnValue;

   //NOTE: This algorithm does not make use of the fact that both sets are
   //      ordered.
}

   
//
// static member functions
//


