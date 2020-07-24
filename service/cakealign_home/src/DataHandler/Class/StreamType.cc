// -*- C++ -*-
//
// Package:     DataHandler
// Module:      StreamType
// 
// Description: Specialization of UTIdentifier for Streams
//
// Implimentation:
//     <Notes on implimentation>
//
// Author:      Chris D. Jones
// Created:     Tue Apr 29 11:56:15 EDT 1997
// $Id: StreamType.cc,v 1.16 1998/09/22 16:44:00 bkh Exp $
//
// Revision history
//
// $Log: StreamType.cc,v $
// Revision 1.16  1998/09/22 16:44:00  bkh
// moved FrameLogger from ToolBox per mkl request
// typed a nonTyped variable in StreamType.cc (noticed as a warning msg)
//
// Revision 1.15  1998/02/03 21:28:26  mkl
// switch over to STLUtility library
//
// Revision 1.14  1997/09/18 21:10:22  cdj
// changed Boolean to DABoolean
//
// Revision 1.13  1997/09/04 19:59:45  sjp
// Used CPP macro for local STL container
//
// Revision 1.12  1997/09/03 20:06:17  sjp
// Uses CPP macros for STL containers
//
// Revision 1.11  1997/09/01 16:23:12  sjp
// Moved TBIdentifier to UTIdentifier
//
// Revision 1.10  1997/09/01 15:35:18  sjp
// Changed report include to be in Experiment
//
// Revision 1.9  1997/08/27 03:39:09  sjp
// Updated to new name for bug flags
//
// Revision 1.8  1997/08/14 11:42:46  sjp
// Mods to run under CW Pro 1
//
// Revision 1.7  1997/07/17 18:42:36  sjp
// Cleaned up problem with static data in template class
//
// Revision 1.6  1997/07/09 17:59:18  sjp
// New <Package>/<File>.h structure
// Modified so can also compile under cxx v5.5
//
// Revision 1.5  1997/07/01 18:42:48  sjp
// added missing `const' in argument list
//
// Revision 1.4  1997/06/29 20:17:46  cdj
// Added priority and isStandard capabilities
//
// Revision 1.3  1997/05/07 19:56:08  sjp
// Added "read from stream" function
//
// Revision 1.2  1997/05/07 16:57:49  sjp
// Cleaned up include file dependencies
//
// Revision 1.1  1997/04/29 19:46:02  cdj
// First submission
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <string>
#include <vector>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h" // For error report
#include "DataHandler/StreamType.h"


// STL classes
#if defined(OLD_CXX_STRING_CLASS_BUG)
#include <String.h>
#else
#include <string>
#endif /* OLD_CXX_STRING_CLASS_BUG */
#include <map>
#include <vector>
#include <algorithm>

//
// constants, enums and typedefs
//

const string kFacilityString = "DataHandler.Frame" ;

typedef _streamtype_priorities_ PriorityList ;
typedef _streamtype_arestandard_ DABooleanList ;

//
// static data member definitions
//

//
// constructors and destructor
//
StreamType::StreamType( void )
{
   m_priority=findPriority();
   findIsStandard(); //Set isStandard to default
}

StreamType::StreamType( const string& iString ) :
   UTIdentifier<string, Stream>( iString )
{
   m_priority=findPriority();
   findIsStandard();//Set isStandard to default
}

StreamType::StreamType( const string& iString, 
			StreamType::Priority iPriority,
			DABoolean iIsStandard ) :
   UTIdentifier<string, Stream>( iString ),
   m_priority( iPriority)
{
   setPriority(iPriority );
   setIsStandard(iIsStandard );
}

// StreamType::StreamType( const StreamType& )
// {
// }

StreamType::~StreamType()
{
}

//
// assignment operators
//
// const StreamType& StreamType::operator=( const StreamType& )
// {
// }

//
// member functions
//

//
// const member functions
//
StreamType::Priority
StreamType::priority( void ) const
{
   return m_priority;
}

DABoolean
StreamType::isStandard( void ) const
{
   return findIsStandard();
}

//
// static member functions
//

//Simple utility function which checks to see if an index to a vector
// is contained within that vector.
// This function is used to make the code more readable and because it
// guarantees that the same algorithm is used in four places.
template <class T>
inline
DABoolean 
isNoIndex( const STL_VECTOR( T )& ipVector, const Count& iIndex ) 
{
   return ( ipVector.size() < iIndex+1 );
} 

PriorityList&
StreamType::priorities( void )
{
   static PriorityList m_priorities ;
   static DABoolean m_first = !false ;

   if ( m_first ) {
      //There probably won't be more than 40 streams, but there will be
      // less than the 4000 that is normally reserved for vector.
      m_priorities.reserve(40);
      m_first = false ;
   }
   return ( m_priorities ) ;
}

DABooleanList&
StreamType::standards( void )
{
   static DABooleanList m_standards ;
   static DABoolean m_first = !false ;

   if ( m_first ) {
      //There probably won't be more than 40 streams, but there will be
      // less than the 4000 that is normally reserved for vector.
      m_standards.reserve(40);
      m_first = false ;
   }
   return ( m_standards ) ;
}

StreamType::Priority 
StreamType::findPriority( void ) const
{
   Count iIndex = index();
   Priority returnValue(kDefaultPriority);
   if( isNoIndex( priorities(), iIndex ) ){
      //add a new entry
      priorities().push_back( kDefaultPriority);
      returnValue= kDefaultPriority;
   } else {
      returnValue = priorities()[ iIndex ];
   }

   return returnValue;
}

   
void 
StreamType::setPriority( const Priority& iPriority)
{
   Count iIndex = index();

   if( isNoIndex( priorities(), iIndex )){
      //add a new entry
      priorities().push_back( iPriority );
   } else {
      //if new priority doesn't match old priority this is an error
      Priority truePriority;
      if( (truePriority = priorities()[iIndex] )
	  != iPriority ){
	 report( EMERGENCY ,
		 kFacilityString )<< "Priority for Stream "
				  << value()
				  <<" has already been set to "
				  << truePriority
				  <<" and you tried to change this to "
				  <<iPriority
				  << endl ;
	 exit( 1 ) ;
      }
   }

   return;
}

DABoolean 
StreamType::findIsStandard( void ) const
{
   Count iIndex = index();
   DABoolean returnValue(false);
   if( isNoIndex(standards(), iIndex) ){
      //default is not standard
      standards().push_back(false);
   } else {
      returnValue = standards()[iIndex];
   }
   return returnValue;
}

void
StreamType::setIsStandard( const DABoolean iIsStandard)
{
   Count iIndex = index();
   if( isNoIndex( standards(), iIndex )){
      //add a new entry
      standards().push_back( iIsStandard );
   } else {
      //if new isStandard doesn't match old isStandard this is an error
      DABoolean trueIsStandard;
      if( (trueIsStandard = standards()[iIndex] )
	  != iIsStandard ){
	 report( EMERGENCY ,
		 kFacilityString )<< "isStandard for Stream "
				  << value()
				  <<" has already been set to "
				  << trueIsStandard
				  <<" and you tried to change this to "
				  <<iIsStandard
				  << endl ;
	 exit( 1 ) ;
      }
   }
   return;
}


//Utility functions
ostream&
operator<<( ostream& iOStream, const StreamType& iStreamType )
{
   return ( iOStream << iStreamType.value() );
}

istream&
operator>>( istream& iIStream, StreamType& iStreamType )
{
   string tmp ;
   iIStream >> tmp ;
   iStreamType = StreamType( tmp ) ;
   return ( iIStream );
}
