// -*- C++ -*-
//
// Package:     DataHandler
// Module:      SyncValue
// 
// Description: Designates the position of a Record in the DataStream.
//
// Implimentation:
//     For now, the time field is not used.
//
// Author:      Chris D. Jones
// Created:     Mon Mar 17 11:54:17 EST 1997
// $Id: SyncValue.cc,v 1.12 2002/01/24 15:41:41 cdj Exp $
//
// Revision history
//
// $Log: SyncValue.cc,v $
// Revision 1.12  2002/01/24 15:41:41  cdj
// SyncValue no longer uses time do do comparisons.  This avoids problems with goto and other synchronizations.
//
// Revision 1.11  1998/03/26 19:40:33  mkl
// switched to UInt64/UInt32 for time/run-,event numbers; include time for comparison operators
//
// Revision 1.10  1997/11/19 15:16:11  sjp
// Added kBeginningOfTime constant
//
// Revision 1.9  1997/09/18 21:10:23  cdj
// changed Boolean to DABoolean
//
// Revision 1.8  1997/08/30 19:01:10  cdj
// kMaxValue is now a static const unsigned long (needed by HP compiler)
//
// Revision 1.7  1997/07/09 17:59:20  sjp
// New <Package>/<File>.h structure
// Modified so can also compile under cxx v5.5
//
// Revision 1.6  1997/06/29 20:12:57  cdj
// Added const SyncValue kUndefined
//
// Revision 1.5  1997/05/30 17:06:48  cdj
// added the accessor methods runNumber(), eventNumber(), and time()
//
// Revision 1.4  1997/05/06 20:25:50  mkl
// got rid of run(), event(), time() methods
//
// Revision 1.3  1997/05/06 18:35:13  mkl
// added run(), event(), and time() return functions
//
// Revision 1.2  1997/04/01 21:01:52  cdj
// Added operator!= and simplified some of the other operators
//
// Revision 1.1  1997/03/17 19:26:15  cdj
// First submission
//

#include "Experiment/Experiment.h"
// system include files
#include <limits.h>

// user include files
#include "DataHandler/SyncValue.h"


// STL classes

//
// constants, enums and typedefs
//
const UInt64 SyncValue::kMaxTimeValue( kMaxUInt64 );
const UInt32 SyncValue::kMaxRunEventValue( kMaxUInt32 );
const SyncValue SyncValue::kUndefined; //Uses the default values
const SyncValue SyncValue::kBeginningOfTime( 0 , 0 , 0 ) ;

//
// static data member definitions
//

//
// constructors and destructor
//
SyncValue::SyncValue( UInt32 iRun,
		      UInt32 iEvent,
		      UInt64 iTime )
   :
   m_run(iRun),
   m_event(iEvent),
   m_time(iTime)
{
}

// SyncValue::SyncValue( const SyncValue& )
// {
// }

SyncValue::~SyncValue()
{
}

//
// assignment operators
//
// const SyncValue& SyncValue::operator=( const SyncValue& )
// {
// }

//
// member functions
//
//
// const member functions
//
DABoolean 
SyncValue::operator<( const SyncValue& iRHS ) const
{
   if( m_run < iRHS.m_run ) {
      return true;
   }
   else if( m_run == iRHS.m_run ) {
      if( m_event < iRHS.m_event ) {
	 return true;
      }
   }
   return false;
}

DABoolean 
SyncValue::operator==( const SyncValue& iRHS) const
{ 
   if( m_run == iRHS.m_run ) {
      if( m_event == iRHS.m_event ) {
	 return true;
      }
   }
   return false;
}

DABoolean 
SyncValue::operator<=( const SyncValue& iRHS) const
{ 
   if( m_run > iRHS.m_run ) {
      return false;
   }
   else if( m_run == iRHS.m_run ) {
      if( m_event > iRHS.m_event ) {
	 return false;
      }
   }
   return true;
}

DABoolean
SyncValue::operator!=( const SyncValue& iRHS) const
{
   if( m_run != iRHS.m_run ){
      return true;
   } 
   else {
      if( m_event != iRHS.m_event ) {
	 return true;
      }
   }
   return false;
}

UInt32
SyncValue::runNumber( void ) const
{
   return m_run;
}

UInt32
SyncValue::eventNumber( void ) const
{
   return m_event;
}

UInt64
SyncValue::time( void ) const
{
   return m_time;
}

//
// static member functions
//


