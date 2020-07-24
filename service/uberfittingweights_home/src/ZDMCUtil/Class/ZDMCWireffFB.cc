// -*- C++ -*-
//
// Package:     ZDMCUtil
// Module:      ZDMCWireffFB
// 
// Description: Frame Broker object for ZD MC Constants - Wireff
//              for Fortran access to these constants.  
//              Based on Mike Marsh's K3FFrameBroker object.

//
// Implementation:
//     <Notes on implementation>
//
// Author:      Guangshun Huang
// Created:     Tue Dec 17, 2002
//
// Revision history
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <string>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "Experiment/fortran_types.h"

#include "ZDMCUtil/ZDMCWireffFB.h"
#include "DBZDMCWireff.hh"

#include "FrameAccess/extract.h"
#include "FrameAccess/FAConstants.h"

#include <string>

// STL classes
//#include <string>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "ZDMCUtil.ZDMCWireffFB" ;

//
// static data member definitions
//
ZDMCWireffFB* ZDMCWireffFB::m_instance = 0;


//
// constructors and destructor
//
ZDMCWireffFB::ZDMCWireffFB()
{
}


// ZDMCWireffFB::ZDMCWireffFB( const ZDMCWireffFB& )
// {
// }


ZDMCWireffFB::~ZDMCWireffFB()
{
}


//
// assignment operators
//
// const ZDMCWireffFB& ZDMCWireffFB::operator=( const ZDMCWireffFB& )
// {
// }


//
// member functions
//
void ZDMCWireffFB::setWireff( const FAConstants< DBZDMCWireff > aWireff ) 
{
   //-> Save FAConstans container of DBZDMCWireff constants
   m_wireff = aWireff;
   return;
}


//
// const member functions
//
//
const DBZDMCWireff& ZDMCWireffFB::getDBZDMCWireff( unsigned int entry) const
{
   //-> Return a DBZDMCWireff object for a given entry

   // Error checking valid entry # (unsigned int requires entry > 0)
   if ( entry > ZDMCWireffFB::kNumberOfEntries  ) {
      report( EMERGENCY, kFacilityString )
	 << "Invalid constant entry = " << entry
	 << " ZDMCWireffFB::kNumberOfEntries = "
	 << ZDMCWireffFB::kNumberOfEntries << endl;
      ::exit(-1);
   }

   return m_wireff[entry];
}


//
// static member functions
//
ZDMCWireffFB*
ZDMCWireffFB::instance()
{
   //-> Creates instance of singleton object
   if ( 0 == m_instance )
   {
      report( DEBUG, kFacilityString ) << "Creating singleton object" << endl;
      m_instance = new ZDMCWireffFB;
   }

   return m_instance;
}


void
ZDMCWireffFB::destroy()
{
   //-> Destroys instance of singleton object
   if ( 0 != m_instance )
   {
      delete m_instance;
      m_instance = 0;
   }
   
   return;
}


void ZDMCWireffFB::fillArrays( ZDMCWireffFB* pFrameBroker,
			       FReal* Effi,
			       const FInteger& arraySize )
{
   //-> Function fills arrays of constants 
   //  iterate over collection of FAConstants and fill arrays
   FAConstants<DBZDMCWireff>::const_iterator itEnd   = m_wireff.end();
   FAConstants<DBZDMCWireff>::const_iterator itBegin = m_wireff.begin();
   // Require arraySize = length of FAConstant container
   if ( arraySize != (itEnd-itBegin) ) {
      assert( arraySize == (itEnd - itBegin) );
      ::exit( -1 );
   }
   for( FAConstants<DBZDMCWireff>::const_iterator itConstant = itBegin;
	itConstant != itEnd;
	++itConstant ) {
      // Need getIndex()-1 because constants start with index=1, not 0
      // Fortran access works normal - starts with 1
      Effi[(*itConstant).getIndex()-1] = (*itConstant).get_Effi();
   }
}


//
// F77 interface functions
//
extern "C" void
zdmc_wireff_( FReal* Effi,
	      FInteger* arraySize )
{
   report( INFO, kFacilityString ) << "ZDMC_WIREFF- F77 access to constants" << endl;

   // Get pointer to ZDMC Wireff Frame Broker object
   ZDMCWireffFB* pFrameBroker = ZDMCWireffFB::instance();

   // Check array size
   if( *arraySize != ZDMCWireffFB::kNumberOfEntries ) {
      report( EMERGENCY, kFacilityString )
	 << "Input Array wrong size:"
	 << " arraySize = " << *arraySize
	 << " ZDMCWireffFB::kNumberOfEntries = " << ZDMCWireffFB::kNumberOfEntries
	 << endl;
					   
      assert( *arraySize == ZDMCWireffFB::kNumberOfEntries );
      ::exit(-1);
   }

   // Fill arrays via fillArrays function
   pFrameBroker->fillArrays( pFrameBroker,
			     Effi,
			     *arraySize );
}   
