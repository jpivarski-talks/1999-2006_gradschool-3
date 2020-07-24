// -*- C++ -*-
//
// Package:     ZDMCUtil
// Module:      ZDMCPhlayrFB
// 
// Description: Frame Broker object for ZD MC Constants - Phlayr
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

#include "ZDMCUtil/ZDMCPhlayrFB.h"
#include "DBZDMCPhlayr.hh"

#include "FrameAccess/extract.h"
#include "FrameAccess/FAConstants.h"

#include <string>

// STL classes
//#include <string>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "ZDMCUtil.ZDMCPhlayrFB" ;

//
// static data member definitions
//
ZDMCPhlayrFB* ZDMCPhlayrFB::m_instance = 0;

//
// constructors and destructor
//
ZDMCPhlayrFB::ZDMCPhlayrFB()
{
}

// ZDMCPhlayrFB::ZDMCPhlayrFB( const ZDMCPhlayrFB& )
// {
// }

ZDMCPhlayrFB::~ZDMCPhlayrFB()
{
}

//
// assignment operators
//
// const ZDMCPhlayrFB& ZDMCPhlayrFB::operator=( const ZDMCPhlayrFB& )
// {
// }

//
// member functions
//


void ZDMCPhlayrFB::setPhlayr( const FAConstants< DBZDMCPhlayr > aPhlayr ) 
{
   //-> Save FAConstans container of DBZDMCPhlayr constants
   m_phlayr = aPhlayr;
   return;
}


//
// const member functions
//
//
const DBZDMCPhlayr& ZDMCPhlayrFB::getDBZDMCPhlayr( unsigned int entry) const
{
   //-> Return a DBZDMCPhlayr object for a given entry

   // Error checking valid entry # (unsigned int requires entry > 0)
   if ( entry > ZDMCPhlayrFB::kNumberOfEntries  ) {
      report( EMERGENCY, kFacilityString )
	 << "Invalid constant entry = "
	 << entry
	 << " ZDMCPhlayrFB::kNumberOfEntries = "
	 << ZDMCPhlayrFB::kNumberOfEntries << endl;
      ::exit(-1);
   }

   return m_phlayr[entry];
}


//
// static member functions
//
ZDMCPhlayrFB*
ZDMCPhlayrFB::instance()
{
   //-> Creates instance of singleton object
   report( DEBUG, kFacilityString ) << "instance - begining of routine " << endl;

   if ( 0 == m_instance )
   {
      report( DEBUG, kFacilityString ) << "Creating singleton object" << endl;
      m_instance = new ZDMCPhlayrFB;

   }

   return m_instance;
}

void
ZDMCPhlayrFB::destroy()
{
   //-> Destroys instance of singleton object
   if ( 0 != m_instance )
   {
      delete m_instance;
      m_instance = 0;
   }
   return;
}


void ZDMCPhlayrFB::fillArrays( ZDMCPhlayrFB* pFrameBroker,
			       FReal* Lyrcor,
			       const FInteger& arraySize )
{
   //-> Function fills arrays of constants 
   //  iterate over collection of FAConstants and fill arrays
   FAConstants<DBZDMCPhlayr>::const_iterator itEnd   = m_phlayr.end();
   FAConstants<DBZDMCPhlayr>::const_iterator itBegin = m_phlayr.begin();
   // Require arraySize = length of FAConstant container
   if ( arraySize != (itEnd-itBegin) ) {
      assert( arraySize == (itEnd - itBegin) );
      ::exit( -1 );
   }
   for( FAConstants<DBZDMCPhlayr>::const_iterator itConstant = itBegin;
	itConstant != itEnd;
	++itConstant ) {
      // Need getIndex()-1 because constants start with index=1, not 0
      // Fortran access works normal - starts with 1
      Lyrcor[(*itConstant).getIndex()-1] = (*itConstant).get_Lyrcor();
   }
}


//
// F77 interface functions
//
extern "C" void
zdmc_phlayr_( FReal* Lyrcor,
	      FInteger* arraySize )
{
   report( INFO, kFacilityString ) << "ZDMC_PHLAYR-F77 access to constants" << endl;

   // Get pointer to ZDMC Phlayr Frame Broker object
   ZDMCPhlayrFB* pFrameBroker = ZDMCPhlayrFB::instance();

   // Check array size
   if( *arraySize != ZDMCPhlayrFB::kNumberOfEntries ) {
      report( EMERGENCY, kFacilityString )
	 << "Input Array wrong size:"
	 << " arraySize = " << *arraySize
	 << " ZDMCPhlayrFB::kNumberOfEntries = " << ZDMCPhlayrFB::kNumberOfEntries
	 << endl;
					   
      assert( *arraySize == ZDMCPhlayrFB::kNumberOfEntries );
      ::exit(-1);      
   }

   // Fill arrays via fillArrays function
   pFrameBroker->fillArrays( pFrameBroker,
			     Lyrcor,
			     *arraySize );
}   
