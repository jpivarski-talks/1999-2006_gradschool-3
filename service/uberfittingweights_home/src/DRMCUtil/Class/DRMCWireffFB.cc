// -*- C++ -*-
//
// Package:     DRMCUtil
// Module:      DRMCWireffFB
// 
// Description: Frame Broker object for DR MC Constants - Wireff
//              for Fortran access to these constants.  
//              Based on Mike Marsh's K3FFrameBroker object.

//
// Implementation:
//     <Notes on implementation>
//
// Author:      John J O'Neill
// Created:     Wed Jun 16 22:11:32 EDT 1999
// $Id: DRMCWireffFB.cc,v 1.3 1999/10/10 14:33:11 cdj Exp $
//
// Revision history
//
// $Log: DRMCWireffFB.cc,v $
// Revision 1.3  1999/10/10 14:33:11  cdj
// removed BDLclient dirctory from include
//
// Revision 1.2  1999/09/20 22:02:53  jjo
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

#include "DRMCUtil/DRMCWireffFB.h"
#include "DBDRMCWireff.hh"

#include "FrameAccess/extract.h"
#include "FrameAccess/FAConstants.h"

#include <string>

// STL classes
//#include <string>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "DRMCUtil.DRMCWireffFB" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DRMCWireffFB.cc,v 1.3 1999/10/10 14:33:11 cdj Exp $";
static const char* const kTagString = "$Name:  $";


//
// static data member definitions
//
DRMCWireffFB* DRMCWireffFB::m_instance = 0;


//
// constructors and destructor
//
DRMCWireffFB::DRMCWireffFB()
{
}


// DRMCWireffFB::DRMCWireffFB( const DRMCWireffFB& )
// {
// }


DRMCWireffFB::~DRMCWireffFB()
{
}


//
// assignment operators
//
// const DRMCWireffFB& DRMCWireffFB::operator=( const DRMCWireffFB& )
// {
// }


//
// member functions
//
void DRMCWireffFB::setWireff( const FAConstants< DBDRMCWireff > aWireff ) 
{
   //-> Save FAConstans container of DBDRMCWireff constants
   m_wireff = aWireff;
   return;
}


//
// const member functions
//
//
const DBDRMCWireff& DRMCWireffFB::getDBDRMCWireff( unsigned int entry) const
{
   //-> Return a DBDRMCWireff object for a given entry

   // Error checking valid entry # (unsigned int requires entry > 0)
   if ( entry > DRMCWireffFB::kNumberOfEntries  ) {
      report( EMERGENCY, kFacilityString )
	 << "Invalid constant entry = " << entry
	 << " DRMCWireffFB::kNumberOfEntries = "
	 << DRMCWireffFB::kNumberOfEntries << endl;
      ::exit(-1);
   }

   return m_wireff[entry];
}


//
// static member functions
//
DRMCWireffFB*
DRMCWireffFB::instance()
{
   //-> Creates instance of singleton object
   if ( 0 == m_instance )
   {
      report( DEBUG, kFacilityString ) << "Creating singleton object" << endl;
      m_instance = new DRMCWireffFB;
   }

   return m_instance;
}


void
DRMCWireffFB::destroy()
{
   //-> Destroys instance of singleton object
   if ( 0 != m_instance )
   {
      delete m_instance;
      m_instance = 0;
   }
   
   return;
}


void DRMCWireffFB::fillArrays( DRMCWireffFB* pFrameBroker,
			       FReal* Effi,
			       const FInteger& arraySize )
{
   //-> Function fills arrays of constants 
   //  iterate over collection of FAConstants and fill arrays
   FAConstants<DBDRMCWireff>::const_iterator itEnd   = m_wireff.end();
   FAConstants<DBDRMCWireff>::const_iterator itBegin = m_wireff.begin();
   // Require arraySize = length of FAConstant container
   if ( arraySize != (itEnd-itBegin) ) {
      assert( arraySize == (itEnd - itBegin) );
      ::exit( -1 );
   }
   for( FAConstants<DBDRMCWireff>::const_iterator itConstant = itBegin;
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
drmc_wireff_( FReal* Effi,
	      FInteger* arraySize )
{
   report( INFO, kFacilityString ) << "DRMC_WIREFF- F77 access to constants" << endl;

   // Get pointer to DRMC Wireff Frame Broker object
   DRMCWireffFB* pFrameBroker = DRMCWireffFB::instance();

   // Check array size
   if( *arraySize != DRMCWireffFB::kNumberOfEntries ) {
      report( EMERGENCY, kFacilityString )
	 << "Input Array wrong size:"
	 << " arraySize = " << *arraySize
	 << " DRMCWireffFB::kNumberOfEntries = " << DRMCWireffFB::kNumberOfEntries
	 << endl;
					   
      assert( *arraySize == DRMCWireffFB::kNumberOfEntries );
      ::exit(-1);
   }

   // Fill arrays via fillArrays function
   pFrameBroker->fillArrays( pFrameBroker,
			     Effi,
			     *arraySize );
}   
