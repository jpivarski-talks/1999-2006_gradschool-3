// -*- C++ -*-
//
// Package:     ZDMCUtil
// Module:      ZDMCPhwireFB
// 
// Description: Frame Broker object for ZD MC Constants - Phwire
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

#include "ZDMCUtil/ZDMCPhwireFB.h"
#include "DBZDMCPhwire.hh"

#include "FrameAccess/extract.h"
#include "FrameAccess/FAConstants.h"

#include <string>

// STL classes
//#include <string>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "ZDMCUtil.ZDMCPhwireFB" ;

//
// static data member definitions
//
ZDMCPhwireFB* ZDMCPhwireFB::m_instance = 0;

//
// constructors and destructor
//
ZDMCPhwireFB::ZDMCPhwireFB()
{
}

// ZDMCPhwireFB::ZDMCPhwireFB( const ZDMCPhwireFB& )
// {
// }

ZDMCPhwireFB::~ZDMCPhwireFB()
{
}

//
// assignment operators
//
// const ZDMCPhwireFB& ZDMCPhwireFB::operator=( const ZDMCPhwireFB& )
// {
// }

//
// member functions
//


void ZDMCPhwireFB::setPhwire( const FAConstants< DBZDMCPhwire > aPhwire ) 
{
   //-> Save FAConstans container of DBZDMCPhwire constants
   m_phwire = aPhwire;
   return;
}


//
// const member functions
//
//
const DBZDMCPhwire& ZDMCPhwireFB::getDBZDMCPhwire( unsigned int entry) const
{
   //-> Return a DBZDMCPhwire object for a given entry

   // Error checking valid entry # (unsigned int requires entry > 0)
   if ( entry > ZDMCPhwireFB::kNumberOfEntries  ) {
      report( EMERGENCY, kFacilityString )
	 << "Invalid constant entry = "
	 << entry
	 << " ZDMCPhwireFB::kNumberOfEntries = "
	 << ZDMCPhwireFB::kNumberOfEntries << endl;
      ::exit(-1);
   }

   return m_phwire[entry];
}


//
// static member functions
//
ZDMCPhwireFB*
ZDMCPhwireFB::instance()
{
   //-> Creates instance of singleton object
   if ( 0 == m_instance )
   {
      report( DEBUG, kFacilityString ) << "Creating singleton object" << endl;
      m_instance = new ZDMCPhwireFB;
   }

   return m_instance;
}

void
ZDMCPhwireFB::destroy()
{
   //-> Destroys instance of singleton object
   if ( 0 != m_instance )
   {
      delete m_instance;
      m_instance = 0;
   }
   return;
}


void ZDMCPhwireFB::fillArrays( ZDMCPhwireFB* pFrameBroker,
			       FReal* Wircor,
			       const FInteger& arraySize )
{
   //-> Function fills arrays of constants 
   //  iterate over collection of FAConstants and fill arrays
   FAConstants<DBZDMCPhwire>::const_iterator itEnd   = m_phwire.end();
   FAConstants<DBZDMCPhwire>::const_iterator itBegin = m_phwire.begin();
   // Require arraySize = length of FAConstant container
   if ( arraySize != (itEnd-itBegin) ) {
      assert( arraySize == (itEnd - itBegin) );
      ::exit( -1 );
   }
   for( FAConstants<DBZDMCPhwire>::const_iterator itConstant = itBegin;
	itConstant != itEnd;
	++itConstant ) {
      // Need getIndex()-1 because constants start with index=1, not 0
      // Fortran access works normal - starts with 1
      Wircor[(*itConstant).getIndex()-1] = (*itConstant).get_Wircor();
   }
}


//
// F77 interface functions
//
extern "C" void
zdmc_phwire_( FReal* Wircor,
	      FInteger* arraySize )
{
   report( INFO, kFacilityString ) << "ZDMC_PHWIRE-F77 access to constants" << endl;

   // Get pointer to ZDMC Phwire Frame Broker object
   ZDMCPhwireFB* pFrameBroker = ZDMCPhwireFB::instance();

   // Check array size
   if( *arraySize != ZDMCPhwireFB::kNumberOfEntries ) {
      report( EMERGENCY, kFacilityString )
	 << "Input Array wrong size:"
	 << " arraySize = " << *arraySize
	 << " ZDMCPhwireFB::kNumberOfEntries = " << ZDMCPhwireFB::kNumberOfEntries
	 << endl;
					   
      assert( *arraySize == ZDMCPhwireFB::kNumberOfEntries );
      ::exit(-1);
   }

   // Fill arrays via fillArrays function
   pFrameBroker->fillArrays( pFrameBroker,
			     Wircor,
			     *arraySize );
}   
