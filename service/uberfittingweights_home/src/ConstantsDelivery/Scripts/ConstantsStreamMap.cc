// -*- C++ -*-
//
// Package:     <ConstantsDelivery>
// Module:      ConstantsStreamMap
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Wed Apr 12 15:35:03 EDT 2000
// $Id: ConstantsStreamMap.cc,v 1.3 2000/06/20 18:41:04 cdj Exp $
//
// Revision history
//
// $Log: ConstantsStreamMap.cc,v $
// Revision 1.3  2000/06/20 18:41:04  cdj
// when using Online MetaVersion tag, use pregenerated list of Constants types
//
// Revision 1.2  2000/04/27 21:14:15  cdj
// now works on OSF using MICO ORB
//
// Revision 1.1  2000/04/14 20:52:33  cdj
// now looks up what constants are in the MetaVersion tag
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <set>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "ConstantsDelivery/ConstantsStreamMap.h"

_stream_incs_

#include "DBConstants.hxx"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "ConstantsDelivery.ConstantsStreamMap" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: ConstantsStreamMap.cc,v 1.3 2000/06/20 18:41:04 cdj Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
ConstantsStreamMap::ConstantsStreamMap()
{
}

// ConstantsStreamMap::ConstantsStreamMap( const ConstantsStreamMap& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

//ConstantsStreamMap::~ConstantsStreamMap()
//{
//}

//
// assignment operators
//
// const ConstantsStreamMap& ConstantsStreamMap::operator=( const ConstantsStreamMap& rhs )
// {
//   if( this != &rhs ) {
//      // do actual copying here, plus:
//      // "SuperClass"::operator=( rhs );
//   }
//
//   return *this;
// }

//
// member functions
//

//
// const member functions
//

//
// static member functions
//
Stream::Type
ConstantsStreamMap::streamWithConstant( const string& iConstantName ) {
   typedef STL_MAP( string, Stream::Type ) NameStreamMap;
   static  NameStreamMap s_nameStreamMap;
   if( 0 == s_nameStreamMap.size() ) {
      s_nameStreamMap.insert( NameStreamMap::value_type("_constant_name_",_stream_name_) );
   }

   NameStreamMap::iterator itFound = s_nameStreamMap.find( iConstantName );
   if( itFound != s_nameStreamMap.end() ) {
      return (*itFound).second;
   }
   return Stream::kNone;
}

STL_VECTOR(string)
ConstantsStreamMap::constantsInStream( const Stream::Type& iStream ) {
   STL_VECTOR(string) returnValue;
   returnValue.reserve(4);

   typedef STL_MULTIMAP( Stream::Type, string ) StreamNameMap;
   static StreamNameMap s_streamNameMap;

   if( 0 == s_streamNameMap.size() ) {
      s_streamNameMap.insert( StreamNameMap::value_type(_stream_name_,"_constant_name_") );
   }

   pair< StreamNameMap::iterator, StreamNameMap::iterator> itRange = 
      s_streamNameMap.equal_range( iStream );

   for( StreamNameMap::iterator itPair = itRange.first;
	itPair != itRange.second;
	++itPair ) {
      returnValue.push_back( (*itPair).second );
   }
   return returnValue;
}

const STL_VECTOR(string)&
ConstantsStreamMap::onlineConstants() {
   static STL_VECTOR(string) s_returnValue;
   
   if ( 0 == s_returnValue.size() ) {
      s_returnValue.push_back( "_onlineconstant_name_");
   }

   return s_returnValue;
}
