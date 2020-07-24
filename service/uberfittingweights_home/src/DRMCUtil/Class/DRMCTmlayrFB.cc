// -*- C++ -*-
//
// Package:     DRMCUtil
// Module:      DRMCTmlayrFB
// 
// Description: Frame Broker object for DR MC Constants - Tmlayr
//              for Fortran access to these constants.  
//              Based on Mike Marsh's K3FFrameBroker object.

//
// Implementation:
//     <Notes on implementation>
//
// Author:      John J O'Neill
// Created:     Wed Jun 16 22:11:32 EDT 1999
// $Id: DRMCTmlayrFB.cc,v 1.3 1999/10/10 14:33:10 cdj Exp $
//
// Revision history
//
// $Log: DRMCTmlayrFB.cc,v $
// Revision 1.3  1999/10/10 14:33:10  cdj
// removed BDLclient dirctory from include
//
// Revision 1.2  1999/09/20 22:02:52  jjo
// Improve error checking
//
// Revision 1.1  1999/08/10 19:45:37  jjo
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

#include "DRMCUtil/DRMCTmlayrFB.h"
#include "DBDRMCTmlayr.hh"

#include "FrameAccess/extract.h"
#include "FrameAccess/FAConstants.h"

#include <string>

// STL classes
//#include <string>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "DRMCUtil.DRMCTmlayrFB" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DRMCTmlayrFB.cc,v 1.3 1999/10/10 14:33:10 cdj Exp $";
static const char* const kTagString = "$Name:  $";


//
// static data member definitions
//
DRMCTmlayrFB* DRMCTmlayrFB::m_instance = 0;

//
// constructors and destructor
//
DRMCTmlayrFB::DRMCTmlayrFB()
{
}

// DRMCTmlayrFB::DRMCTmlayrFB( const DRMCTmlayrFB& )
// {
// }

DRMCTmlayrFB::~DRMCTmlayrFB()
{
}

//
// assignment operators
//
// const DRMCTmlayrFB& DRMCTmlayrFB::operator=( const DRMCTmlayrFB& )
// {
// }

//
// member functions
//


void DRMCTmlayrFB::setTmlayr( const FAConstants< DBDRMCTmlayr > aTmlayr )
{
   //-> Save FAConstans container of DBDRMCTmlayr constants
   m_tmlayr = aTmlayr;
   return;
}


//
// const member functions
//
//
const DBDRMCTmlayr& DRMCTmlayrFB::getDBDRMCTmlayr( unsigned int entry ) const
{
   //-> Return a DBDRMCTmlayr object for a given entry number
   // Error checking valid entry # (unsigned int requires entry > 0)
   if ( entry > DRMCTmlayrFB::kNumberOfEntries  ) {
      report( EMERGENCY, kFacilityString )
	 << "Invalid constant entry = "
	 << entry
	 << " DRMCTmlayrFB::kNumberOfEntries = "
	 << DRMCTmlayrFB::kNumberOfEntries << endl;
      ::exit(-1);
   }

   return m_tmlayr[entry];
}


//
// static member functions
//
DRMCTmlayrFB*
DRMCTmlayrFB::instance()
{
   //-> Creates instance of singleton object
   if ( 0 == m_instance )
   {
      report( DEBUG, kFacilityString ) << "Creating singleton object" << endl;
      m_instance = new DRMCTmlayrFB;
   }

   return m_instance;
}

void
DRMCTmlayrFB::destroy()
{
   //-> Destroys instance of singleton object
   if ( 0 != m_instance )
   {
      delete m_instance;
      m_instance = 0;
   }
   
   return;
}


void DRMCTmlayrFB::fillArrays( DRMCTmlayrFB* pFrameBroker,
			       FReal* Shift,
			       const FInteger& arraySize )
{
   //-> Function fills arrays of constants 
   // iterate over collection of FAConstants and fill arrays
   FAConstants<DBDRMCTmlayr>::const_iterator itEnd   = m_tmlayr.end();
   FAConstants<DBDRMCTmlayr>::const_iterator itBegin = m_tmlayr.begin();
   // Require arraySize = length of FAConstant container
   if ( arraySize != (itEnd-itBegin) ) {
      assert( arraySize == (itEnd - itBegin) );
      ::exit( -1 );
   }
   for( FAConstants<DBDRMCTmlayr>::const_iterator itConstant = itBegin;
	itConstant != itEnd;
	++itConstant ) {
      // Need getIndex()-1 because constants start with index=1, not 0
      // Fortran access works normal - starts with 1
      Shift[(*itConstant).getIndex()-1] = (*itConstant).get_Shift();
   }
}


//
// F77 interface functions
//
extern "C" void
drmc_tmlayr_( FReal* Shift,
	      FInteger* arraySize )
{
   report( DEBUG, kFacilityString ) << "DRMC_TMLAYR- F77 access to constants" << endl;

   // Get pointer to DRMC Tmlayr Frame Broker object
   DRMCTmlayrFB* pFrameBroker = DRMCTmlayrFB::instance();

   // Check array size
   if( *arraySize != DRMCTmlayrFB::kNumberOfEntries ) {
      report( EMERGENCY, kFacilityString )
	 << "Input Array wrong size:"
	 << " arraySize = " << *arraySize
	 << " DRMCTmlayrFB::kNumberOfEntries = " << DRMCTmlayrFB::kNumberOfEntries
	 << endl;
					   
      assert( *arraySize == DRMCTmlayrFB::kNumberOfEntries );
      ::exit(-1);
   }

   // Fill arrays via fillArrays function
   pFrameBroker->fillArrays( pFrameBroker,
			     Shift,
			     *arraySize );
}   
