// -*- C++ -*-
//
// Package:     DRMCUtil
// Module:      DRMCEffFB
// 
// Description: Frame Broker object for DR MC Constants - Eff
//              for Fortran access to these constants.  
//              Based on Mike Marsh's K3FFrameBroker object.

//
// Implementation:
//     <Notes on implementation>
//
// Author:      John J O'Neill
// Created:     Wed Jun 16 22:11:32 EDT 1999
// $Id: DRMCEffFB.cc,v 1.3 1999/10/10 14:33:06 cdj Exp $
//
// Revision history
//
// $Log: DRMCEffFB.cc,v $
// Revision 1.3  1999/10/10 14:33:06  cdj
// removed BDLclient dirctory from include
//
// Revision 1.2  1999/09/20 22:02:45  jjo
// Improve error checking
//
// Revision 1.1  1999/08/10 19:45:32  jjo
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

#include "DRMCUtil/DRMCEffFB.h"
#include "DBDRMCEff.hh"

#include "FrameAccess/extract.h"
#include "FrameAccess/FAConstants.h"

#include <string>

// STL classes
//#include <string>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "DRMCUtil.DRMCEffFB" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DRMCEffFB.cc,v 1.3 1999/10/10 14:33:06 cdj Exp $";
static const char* const kTagString = "$Name:  $";


//
// static data member definitions
//
DRMCEffFB* DRMCEffFB::m_instance = 0;

//
// constructors and destructor
//
DRMCEffFB::DRMCEffFB()
{
}

// DRMCEffFB::DRMCEffFB( const DRMCEffFB& )
// {
// }

DRMCEffFB::~DRMCEffFB()
{
}

//
// assignment operators
//
// const DRMCEffFB& DRMCEffFB::operator=( const DRMCEffFB& )
// {
// }

//
// member functions
//


void DRMCEffFB::setEff( const FAConstants< DBDRMCEff > aEff ) 
{
   //-> Save FAConstans container of DBDRMCCoseff constants
   m_eff = aEff;
   return;
}


//
// const member functions
//
//

const DBDRMCEff& DRMCEffFB::getDBDRMCEff( unsigned int entry) const
{
   //-> Return a DBDRMCEff object for a given entry

   // Error checking valid entry # (unsigned int requires entry > 0)
   if ( entry > DRMCEffFB::kNumberOfEntries  ) {
      report( EMERGENCY, kFacilityString )
	 << "Invalid constant entry = " << entry
	 << " DRMCEffFB::kNumberOfEntries = "
	 << DRMCEffFB::kNumberOfEntries << endl;
      ::exit(-1);
   }

   return m_eff[entry];
}


//
// static member functions
//
DRMCEffFB*
DRMCEffFB::instance()
{
   //-> Creates instance of singleton object
   if ( 0 == m_instance )
   {
      report( DEBUG, kFacilityString ) << "Creating singleton object" << endl;
      m_instance = new DRMCEffFB;

   }

   return m_instance;
}

void
DRMCEffFB::destroy()
{
   //-> Destroys instance of singleton object
   if ( 0 != m_instance )
   {
      delete m_instance;
      m_instance = 0;
   }
   return;
}


void DRMCEffFB::fillArrays( DRMCEffFB* pFrameBroker,
			    FReal* A,
			    FReal* ParameterB,
			    FReal* ParameterC,
			    const FInteger& arraySize )
{
   //-> Function fills arrays of constants 
   //  iterate over collection of FAConstants and fill arrays
   FAConstants<DBDRMCEff>::const_iterator itEnd   = m_eff.end();
   FAConstants<DBDRMCEff>::const_iterator itBegin = m_eff.begin();
   // Require arraySize = length of FAConstant container
   if ( arraySize != (itEnd-itBegin) ) {
      assert( arraySize == (itEnd - itBegin) );
      ::exit( -1 );
   }
   for( FAConstants<DBDRMCEff>::const_iterator itConstant = itBegin;
	itConstant != itEnd;
	++itConstant ) {
      // Need getIndex()-1 because constants start with index=1, not 0
      // Fortran access works normal - starts with 1
      A[(*itConstant).getIndex()-1] = (*itConstant).get_A();
      ParameterB[(*itConstant).getIndex()-1] = (*itConstant).get_B();
      ParameterC[(*itConstant).getIndex()-1] = (*itConstant).get_C();
   }
}


//
// F77 interface functions
//
extern "C" void
drmc_eff_( FReal* A,
	   FReal* B,
	   FReal* C,
	   FInteger* arraySize )
{
   report( INFO, kFacilityString ) << "DRMC_EFF- F77 access to constants" << endl;

   // Get pointer to DRMC Eff Frame Broker object
   DRMCEffFB* pFrameBroker = DRMCEffFB::instance();

   // Check array size
   if( *arraySize != DRMCEffFB::kNumberOfEntries ) {
      report( EMERGENCY, kFacilityString )
	 << "Input Array wrong size:"
	 << " arraySize = " << *arraySize
	 << " DRMCEffFB::kNumberOfEntries = " << DRMCEffFB::kNumberOfEntries
	 << endl;
					   
      assert( *arraySize == DRMCEffFB::kNumberOfEntries );
      ::exit(-1);
   }

   // Fill arrays via fillArrays function
   pFrameBroker->fillArrays( pFrameBroker,
			     A,
			     B,
			     C,
			     *arraySize );
}   

