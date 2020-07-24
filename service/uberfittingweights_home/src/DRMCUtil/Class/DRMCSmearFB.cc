// -*- C++ -*-
//
// Package:     DRMCUtil
// Module:      DRMCSmearFB
// 
// Description: Frame Broker object for DR MC Constants - Smear
//              for Fortran access to these constants.  
//              Based on Mike Marsh's K3FFrameBroker object.

//
// Implementation:
//     <Notes on implementation>
//
// Author:      John J O'Neill
// Created:     Wed Jun 16 22:11:32 EDT 1999
// $Id: DRMCSmearFB.cc,v 1.3 1999/10/10 14:33:09 cdj Exp $
//
// Revision history
//
// $Log: DRMCSmearFB.cc,v $
// Revision 1.3  1999/10/10 14:33:09  cdj
// removed BDLclient dirctory from include
//
// Revision 1.2  1999/09/20 22:02:50  jjo
// Improve error checking
//
// Revision 1.1  1999/08/10 19:45:36  jjo
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

#include "DRMCUtil/DRMCSmearFB.h"
#include "DBDRMCSmear.hh"

#include "FrameAccess/extract.h"
#include "FrameAccess/FAConstants.h"

#include <string>

// STL classes
//#include <string>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "DRMCUtil.DRMCSmearFB" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DRMCSmearFB.cc,v 1.3 1999/10/10 14:33:09 cdj Exp $";
static const char* const kTagString = "$Name:  $";


//
// static data member definitions
//
DRMCSmearFB* DRMCSmearFB::m_instance = 0;

//
// constructors and destructor
//
DRMCSmearFB::DRMCSmearFB()
{
}

// DRMCSmearFB::DRMCSmearFB( const DRMCSmearFB& )
// {
// }

DRMCSmearFB::~DRMCSmearFB()
{
}

//
// assignment operators
//
// const DRMCSmearFB& DRMCSmearFB::operator=( const DRMCSmearFB& )
// {
// }

//
// member functions
//


void DRMCSmearFB::setSmear( const FAConstants< DBDRMCSmear > aSmear ) 
{
   //-> Save FAConstans container of DBDRMCSmear constants
   m_smear = aSmear;
   return;
}


//
// const member functions
//
//
const DBDRMCSmear& DRMCSmearFB::getDBDRMCSmear( unsigned int entry) const
{
   //-> Return a DBDRMCSmear object for a given entry
   // Error checking valid entry # (unsigned int requires entry > 0)
   if ( entry > DRMCSmearFB::kNumberOfEntries  ) {
      report( EMERGENCY, kFacilityString )
	 << "Invalid constant entry = "
	 << entry
	 << " DRMCSmearFB::kNumberOfEntries = "
	 << DRMCSmearFB::kNumberOfEntries << endl;
      ::exit(-1);
   }

   return m_smear[entry];
}


//
// static member functions
//
DRMCSmearFB*
DRMCSmearFB::instance()
{
   //-> Creates instance of singleton object
   if ( 0 == m_instance )
   {
      report( DEBUG, kFacilityString ) << "Creating singleton object" << endl;
      m_instance = new DRMCSmearFB;
   }

   return m_instance;
}

void
DRMCSmearFB::destroy()
{
   //-> Destroys instance of singleton object
   if ( 0 != m_instance )
   {
      delete m_instance;
      m_instance = 0;
   }
   return;
}


void DRMCSmearFB::fillArrays( DRMCSmearFB* pFrameBroker,
			      FReal* Dz,
			      const FInteger& arraySize )
{
   //-> Function fills arrays of constants 
   //  iterate over collection of FAConstants and fill arrays
   FAConstants<DBDRMCSmear>::const_iterator itEnd   = m_smear.end();
   FAConstants<DBDRMCSmear>::const_iterator itBegin = m_smear.begin();
   // Require arraySize = length of FAConstant container
   if ( arraySize != (itEnd-itBegin) ) {
      assert( arraySize == (itEnd - itBegin) );
      ::exit( -1 );
   }
   for( FAConstants<DBDRMCSmear>::const_iterator itConstant = itBegin;
	itConstant != itEnd;
	++itConstant ) {
      // Need getIndex()-1 because constants start with index=1, not 0
      // Fortran access works normal - starts with 1
      Dz[(*itConstant).getIndex()-1] = (*itConstant).get_Dz();
   }
}



//
// F77 interface functions
//
extern "C" void
drmc_smear_( FReal* Dz,
	     FInteger* arraySize )
{
   report( INFO, kFacilityString ) << "DRMC_SMEAR- F77 access to constants" << endl;

   // Get pointer to DRMC Smear Frame Broker object
   DRMCSmearFB* pFrameBroker = DRMCSmearFB::instance();

   // Check array size
   if( *arraySize != DRMCSmearFB::kNumberOfEntries ) {
      report( EMERGENCY, kFacilityString )
	 << "Input Array wrong size:"
	 << " arraySize = " << *arraySize
	 << " DRMCSmearFB::kNumberOfEntries = " << DRMCSmearFB::kNumberOfEntries
	 << endl;
					   
      assert( *arraySize == DRMCSmearFB::kNumberOfEntries );
      ::exit(-1);      
   }

   // Fill arrays via fillArrays function
   pFrameBroker->fillArrays( pFrameBroker,
			     Dz,
			     *arraySize );
}   
