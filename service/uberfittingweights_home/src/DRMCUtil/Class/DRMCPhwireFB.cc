// -*- C++ -*-
//
// Package:     DRMCUtil
// Module:      DRMCPhwireFB
// 
// Description: Frame Broker object for DR MC Constants - Phwire
//              for Fortran access to these constants.  
//              Based on Mike Marsh's K3FFrameBroker object.

//
// Implementation:
//     <Notes on implementation>
//
// Author:      John J O'Neill
// Created:     Wed Jun 16 22:11:32 EDT 1999
// $Id: DRMCPhwireFB.cc,v 1.3 1999/10/10 14:33:08 cdj Exp $
//
// Revision history
//
// $Log: DRMCPhwireFB.cc,v $
// Revision 1.3  1999/10/10 14:33:08  cdj
// removed BDLclient dirctory from include
//
// Revision 1.2  1999/09/20 22:02:49  jjo
// Improve error checking
//
// Revision 1.1  1999/08/10 19:45:35  jjo
// Add Fortran access for DR Monte Carlo constants
//
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <string>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "Experiment/fortran_types.h"

#include "DRMCUtil/DRMCPhwireFB.h"
#include "DBDRMCPhwire.hh"

#include "FrameAccess/extract.h"
#include "FrameAccess/FAConstants.h"

#include <string>

// STL classes
//#include <string>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "DRMCUtil.DRMCPhwireFB" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DRMCPhwireFB.cc,v 1.3 1999/10/10 14:33:08 cdj Exp $";
static const char* const kTagString = "$Name:  $";


//
// static data member definitions
//
DRMCPhwireFB* DRMCPhwireFB::m_instance = 0;

//
// constructors and destructor
//
DRMCPhwireFB::DRMCPhwireFB()
{
}

// DRMCPhwireFB::DRMCPhwireFB( const DRMCPhwireFB& )
// {
// }

DRMCPhwireFB::~DRMCPhwireFB()
{
}

//
// assignment operators
//
// const DRMCPhwireFB& DRMCPhwireFB::operator=( const DRMCPhwireFB& )
// {
// }

//
// member functions
//


void DRMCPhwireFB::setPhwire( const FAConstants< DBDRMCPhwire > aPhwire ) 
{
   //-> Save FAConstans container of DBDRMCPhwire constants
   m_phwire = aPhwire;
   return;
}


//
// const member functions
//
//
const DBDRMCPhwire& DRMCPhwireFB::getDBDRMCPhwire( unsigned int entry) const
{
   //-> Return a DBDRMCPhwire object for a given entry

   // Error checking valid entry # (unsigned int requires entry > 0)
   if ( entry > DRMCPhwireFB::kNumberOfEntries  ) {
      report( EMERGENCY, kFacilityString )
	 << "Invalid constant entry = "
	 << entry
	 << " DRMCPhwireFB::kNumberOfEntries = "
	 << DRMCPhwireFB::kNumberOfEntries << endl;
      ::exit(-1);
   }

   return m_phwire[entry];
}


//
// static member functions
//
DRMCPhwireFB*
DRMCPhwireFB::instance()
{
   //-> Creates instance of singleton object
   if ( 0 == m_instance )
   {
      report( DEBUG, kFacilityString ) << "Creating singleton object" << endl;
      m_instance = new DRMCPhwireFB;
   }

   return m_instance;
}

void
DRMCPhwireFB::destroy()
{
   //-> Destroys instance of singleton object
   if ( 0 != m_instance )
   {
      delete m_instance;
      m_instance = 0;
   }
   return;
}


void DRMCPhwireFB::fillArrays( DRMCPhwireFB* pFrameBroker,
			       FReal* Wircor,
			       const FInteger& arraySize )
{
   //-> Function fills arrays of constants 
   //  iterate over collection of FAConstants and fill arrays
   FAConstants<DBDRMCPhwire>::const_iterator itEnd   = m_phwire.end();
   FAConstants<DBDRMCPhwire>::const_iterator itBegin = m_phwire.begin();
   // Require arraySize = length of FAConstant container
   if ( arraySize != (itEnd-itBegin) ) {
      assert( arraySize == (itEnd - itBegin) );
      ::exit( -1 );
   }
   for( FAConstants<DBDRMCPhwire>::const_iterator itConstant = itBegin;
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
drmc_phwire_( FReal* Wircor,
	      FInteger* arraySize )
{
   report( INFO, kFacilityString ) << "DRMC_PHWIRE-F77 access to constants" << endl;

   // Get pointer to DRMC Phwire Frame Broker object
   DRMCPhwireFB* pFrameBroker = DRMCPhwireFB::instance();

   // Check array size
   if( *arraySize != DRMCPhwireFB::kNumberOfEntries ) {
      report( EMERGENCY, kFacilityString )
	 << "Input Array wrong size:"
	 << " arraySize = " << *arraySize
	 << " DRMCPhwireFB::kNumberOfEntries = " << DRMCPhwireFB::kNumberOfEntries
	 << endl;
					   
      assert( *arraySize == DRMCPhwireFB::kNumberOfEntries );
      ::exit(-1);
   }

   // Fill arrays via fillArrays function
   pFrameBroker->fillArrays( pFrameBroker,
			     Wircor,
			     *arraySize );
}   
