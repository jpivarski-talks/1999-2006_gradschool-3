// -*- C++ -*-
//
// Package:     DRMCUtil
// Module:      DRMCIonsFB
// 
// Description: Frame Broker object for DR MC Constants - Ions
//              for Fortran access to these constants.  
//              Based on Mike Marsh's K3FFrameBroker object.

//
// Implementation:
//     <Notes on implementation>
//
// Author:      John J O'Neill
// Created:     Wed Jun 16 22:11:32 EDT 1999
// $Id: DRMCIonsFB.cc,v 1.3 1999/10/10 14:33:06 cdj Exp $
//
// Revision history
//
// $Log: DRMCIonsFB.cc,v $
// Revision 1.3  1999/10/10 14:33:06  cdj
// removed BDLclient dirctory from include
//
// Revision 1.2  1999/09/20 22:02:46  jjo
// Improve error checking
//
// Revision 1.1  1999/08/10 19:45:33  jjo
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

#include "DRMCUtil/DRMCIonsFB.h"
#include "DBDRMCIons.hh"

#include "FrameAccess/extract.h"
#include "FrameAccess/FAConstants.h"

#include <string>

// STL classes
//#include <string>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "DRMCUtil.DRMCIonsFB" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DRMCIonsFB.cc,v 1.3 1999/10/10 14:33:06 cdj Exp $";
static const char* const kTagString = "$Name:  $";


//
// static data member definitions
//
DRMCIonsFB* DRMCIonsFB::m_instance = 0;

//
// constructors and destructor
//
DRMCIonsFB::DRMCIonsFB()
{
}

// DRMCIonsFB::DRMCIonsFB( const DRMCIonsFB& )
// {
// }

DRMCIonsFB::~DRMCIonsFB()
{
}

//
// assignment operators
//
// const DRMCIonsFB& DRMCIonsFB::operator=( const DRMCIonsFB& )
// {
// }

//
// member functions
//


void DRMCIonsFB::setIons( const FAConstants< DBDRMCIons > aIons ) 
{
   //-> Save FAConstans container of DBDRMCIons constants
   m_ions = aIons;
   return;
}


//
// const member functions
//
//

const DBDRMCIons& DRMCIonsFB::getDBDRMCIons( unsigned int entry) const
{
   //-> Return a DBDRMCIons object for a given entry
   // Error checking valid entry # (unsigned int requires entry > 0)
   if ( entry > DRMCIonsFB::kNumberOfEntries  ) {
      report( EMERGENCY, kFacilityString )
	 << "Invalid constant entry = "
	 << entry
	 << " DRMCIonsFB::kNumberOfEntries = "
	 << DRMCIonsFB::kNumberOfEntries << endl;
      ::exit(-1);
   }

   return m_ions[entry];
}


//
// static member functions
//
DRMCIonsFB*
DRMCIonsFB::instance()
{
   //-> Creates instance of singleton object
   report( DEBUG, kFacilityString ) << "instance - begining of routine " << endl;

   if ( 0 == m_instance )
   {
      report( DEBUG, kFacilityString ) << "Creating singleton object" << endl;
      m_instance = new DRMCIonsFB;
   }

   return m_instance;
}

void
DRMCIonsFB::destroy()
{
   //-> Destroys instance of singleton object
   if ( 0 != m_instance )
   {
      delete m_instance;
      m_instance = 0;
   }
   return;
}


void DRMCIonsFB::fillArrays( DRMCIonsFB* pFrameBroker,
			     FInteger* N,
			     const FInteger& arraySize )
{
   //-> Function fills arrays of constants 
   // iterate over collection of FAConstants and fill arrays
   FAConstants<DBDRMCIons>::const_iterator itEnd   = m_ions.end();
   FAConstants<DBDRMCIons>::const_iterator itBegin = m_ions.begin();
   // Require arraySize = length of FAConstant container
   if ( arraySize != (itEnd-itBegin) ) {
      assert( arraySize == (itEnd - itBegin) );
      ::exit( -1 );
   }
   for( FAConstants<DBDRMCIons>::const_iterator itConstant = itBegin;
	itConstant != itEnd;
	++itConstant ) {
      // Need getIndex()-1 because constants start with index=1, not 0
      // Fortran access works normal - starts with 1
      N[(*itConstant).getIndex()-1] = (*itConstant).get_N();
   }
}


//
// F77 interface functions
//
extern "C" void
drmc_ions_( FInteger* N,
	    FInteger* arraySize )
{
   report( INFO, kFacilityString ) << "DRMC_IONS- F77 access to constants" << endl;

   // Get pointer to DRMC Ions Frame Broker object
   DRMCIonsFB* pFrameBroker = DRMCIonsFB::instance();

   // Check array size
   if( *arraySize != DRMCIonsFB::kNumberOfEntries ) {
      report( EMERGENCY, kFacilityString )
	 << "Input Array wrong size:"
	 << " arraySize = " << *arraySize
	 << " DRMCIonsFB::kNumberOfEntries = " << DRMCIonsFB::kNumberOfEntries
	 << endl;
					   
      assert( *arraySize == DRMCIonsFB::kNumberOfEntries );
      ::exit(-1);
   }

   // Fill arrays via fillArrays function
   pFrameBroker->fillArrays( pFrameBroker,
			     N,
			     *arraySize );
}   
