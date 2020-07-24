// -*- C++ -*-
//
// Package:     DRMCUtil
// Module:      DRMCPhlayrFB
// 
// Description: Frame Broker object for DR MC Constants - Phlayr
//              for Fortran access to these constants.  
//              Based on Mike Marsh's K3FFrameBroker object.

//
// Implementation:
//     <Notes on implementation>
//
// Author:      John J O'Neill
// Created:     Wed Jun 16 22:11:32 EDT 1999
// $Id: DRMCPhlayrFB.cc,v 1.3 1999/10/10 14:33:07 cdj Exp $
//
// Revision history
//
// $Log: DRMCPhlayrFB.cc,v $
// Revision 1.3  1999/10/10 14:33:07  cdj
// removed BDLclient dirctory from include
//
// Revision 1.2  1999/09/20 22:02:48  jjo
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

#include "DRMCUtil/DRMCPhlayrFB.h"
#include "DBDRMCPhlayr.hh"

#include "FrameAccess/extract.h"
#include "FrameAccess/FAConstants.h"

#include <string>

// STL classes
//#include <string>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "DRMCUtil.DRMCPhlayrFB" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DRMCPhlayrFB.cc,v 1.3 1999/10/10 14:33:07 cdj Exp $";
static const char* const kTagString = "$Name:  $";


//
// static data member definitions
//
DRMCPhlayrFB* DRMCPhlayrFB::m_instance = 0;

//
// constructors and destructor
//
DRMCPhlayrFB::DRMCPhlayrFB()
{
}

// DRMCPhlayrFB::DRMCPhlayrFB( const DRMCPhlayrFB& )
// {
// }

DRMCPhlayrFB::~DRMCPhlayrFB()
{
}

//
// assignment operators
//
// const DRMCPhlayrFB& DRMCPhlayrFB::operator=( const DRMCPhlayrFB& )
// {
// }

//
// member functions
//


void DRMCPhlayrFB::setPhlayr( const FAConstants< DBDRMCPhlayr > aPhlayr ) 
{
   //-> Save FAConstans container of DBDRMCPhlayr constants
   m_phlayr = aPhlayr;
   return;
}


//
// const member functions
//
//
const DBDRMCPhlayr& DRMCPhlayrFB::getDBDRMCPhlayr( unsigned int entry) const
{
   //-> Return a DBDRMCPhlayr object for a given entry

   // Error checking valid entry # (unsigned int requires entry > 0)
   if ( entry > DRMCPhlayrFB::kNumberOfEntries  ) {
      report( EMERGENCY, kFacilityString )
	 << "Invalid constant entry = "
	 << entry
	 << " DRMCPhlayrFB::kNumberOfEntries = "
	 << DRMCPhlayrFB::kNumberOfEntries << endl;
      ::exit(-1);
   }

   return m_phlayr[entry];
}


//
// static member functions
//
DRMCPhlayrFB*
DRMCPhlayrFB::instance()
{
   //-> Creates instance of singleton object
   report( DEBUG, kFacilityString ) << "instance - begining of routine " << endl;

   if ( 0 == m_instance )
   {
      report( DEBUG, kFacilityString ) << "Creating singleton object" << endl;
      m_instance = new DRMCPhlayrFB;

   }

   return m_instance;
}

void
DRMCPhlayrFB::destroy()
{
   //-> Destroys instance of singleton object
   if ( 0 != m_instance )
   {
      delete m_instance;
      m_instance = 0;
   }
   return;
}


void DRMCPhlayrFB::fillArrays( DRMCPhlayrFB* pFrameBroker,
			       FReal* Lyrcor,
			       const FInteger& arraySize )
{
   //-> Function fills arrays of constants 
   //  iterate over collection of FAConstants and fill arrays
   FAConstants<DBDRMCPhlayr>::const_iterator itEnd   = m_phlayr.end();
   FAConstants<DBDRMCPhlayr>::const_iterator itBegin = m_phlayr.begin();
   // Require arraySize = length of FAConstant container
   if ( arraySize != (itEnd-itBegin) ) {
      assert( arraySize == (itEnd - itBegin) );
      ::exit( -1 );
   }
   for( FAConstants<DBDRMCPhlayr>::const_iterator itConstant = itBegin;
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
drmc_phlayr_( FReal* Lyrcor,
	      FInteger* arraySize )
{
   report( INFO, kFacilityString ) << "DRMC_PHLAYR-F77 access to constants" << endl;

   // Get pointer to DRMC Phlayr Frame Broker object
   DRMCPhlayrFB* pFrameBroker = DRMCPhlayrFB::instance();

   // Check array size
   if( *arraySize != DRMCPhlayrFB::kNumberOfEntries ) {
      report( EMERGENCY, kFacilityString )
	 << "Input Array wrong size:"
	 << " arraySize = " << *arraySize
	 << " DRMCPhlayrFB::kNumberOfEntries = " << DRMCPhlayrFB::kNumberOfEntries
	 << endl;
					   
      assert( *arraySize == DRMCPhlayrFB::kNumberOfEntries );
      ::exit(-1);      
   }

   // Fill arrays via fillArrays function
   pFrameBroker->fillArrays( pFrameBroker,
			     Lyrcor,
			     *arraySize );
}   
