// -*- C++ -*-
//
// Package:     DRMCUtil
// Module:      DRMCSpefudFB
// 
// Description: Frame Broker object for DR MC Constants - Spefud
//              for Fortran access to these constants.  
//              Based on Mike Marsh's K3FFrameBroker object.

//
// Implementation:
//     <Notes on implementation>
//
// Author:      John J O'Neill
// Created:     Wed Jun 16 22:11:32 EDT 1999
// $Id: DRMCSpefudFB.cc,v 1.3 1999/10/10 14:33:10 cdj Exp $
//
// Revision history
//
// $Log: DRMCSpefudFB.cc,v $
// Revision 1.3  1999/10/10 14:33:10  cdj
// removed BDLclient dirctory from include
//
// Revision 1.2  1999/09/20 22:02:51  jjo
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

#include "DRMCUtil/DRMCSpefudFB.h"
#include "DBDRMCSpefud.hh"

#include "FrameAccess/extract.h"
#include "FrameAccess/FAConstants.h"

#include <string>

// STL classes
//#include <string>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "DRMCUtil.DRMCSpefudFB" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DRMCSpefudFB.cc,v 1.3 1999/10/10 14:33:10 cdj Exp $";
static const char* const kTagString = "$Name:  $";


//
// static data member definitions
//
DRMCSpefudFB* DRMCSpefudFB::m_instance = 0;

//
// constructors and destructor
//
DRMCSpefudFB::DRMCSpefudFB()
{
}

// DRMCSpefudFB::DRMCSpefudFB( const DRMCSpefudFB& )
// {
// }

DRMCSpefudFB::~DRMCSpefudFB()
{
}

//
// assignment operators
//
// const DRMCSpefudFB& DRMCSpefudFB::operator=( const DRMCSpefudFB& )
// {
// }

//
// member functions
//


void DRMCSpefudFB::setSpefud( const FAConstants< DBDRMCSpefud > aSpefud ) 
{
   //-> Save FAConstans container of DBDRMCSpefud constants
   m_spefud = aSpefud;
   return;
}


//
// const member functions
//
//
const DBDRMCSpefud& DRMCSpefudFB::getDBDRMCSpefud( unsigned int entry) const
{
   //-> Return a DBDRMCSpefud object for a given entry number
   // Error checking valid entry # (unsigned int requires entry > 0)
   if ( entry > DRMCSpefudFB::kNumberOfEntries  ) {
      report( EMERGENCY, kFacilityString )
	 << "Invalid constant entry = "
	 << entry
	 << " DRMCSpefudFB::kNumberOfEntries = "
	 << DRMCSpefudFB::kNumberOfEntries << endl;
      ::exit(-1);
   }

   return m_spefud[entry];
}


//
// static member functions
//
DRMCSpefudFB*
DRMCSpefudFB::instance()
{
   //-> Creates instance of singleton object
   if ( 0 == m_instance )
   {
      report( DEBUG, kFacilityString ) << "Creating singleton object" << endl;
      m_instance = new DRMCSpefudFB;
   }

   return m_instance;
}

void
DRMCSpefudFB::destroy()
{
   //-> Destroys instance of singleton object
   if ( 0 != m_instance )
   {
      delete m_instance;
      m_instance = 0;
   }
   return;
}

void DRMCSpefudFB::fillArrays( DRMCSpefudFB* pFrameBroker,
			       FReal* Elefud,
			       FReal* Muofud,
			       FReal* Piofud,
			       FReal* Kaofud,
			       FReal* Profud,
			       const FInteger& arraySize )
{
   //-> Function fills arrays of constants 
   // iterate over collection of FAConstants and fill arrays
   FAConstants<DBDRMCSpefud>::const_iterator itEnd   = m_spefud.end();
   FAConstants<DBDRMCSpefud>::const_iterator itBegin = m_spefud.begin();
   // Require arraySize = length of FAConstant container
   if ( arraySize != (itEnd-itBegin) ) {
      assert( arraySize == (itEnd - itBegin) );
      ::exit( -1 );
   }
   for( FAConstants<DBDRMCSpefud>::const_iterator itConstant = itBegin;
	itConstant != itEnd;
	++itConstant ) {
      // Need getIndex()-1 because constants start with index=1, not 0
      // Fortran access works normal - starts with 1
      Elefud[(*itConstant).getIndex()-1] = (*itConstant).get_Elefud();
      Muofud[(*itConstant).getIndex()-1] = (*itConstant).get_Muofud();
      Piofud[(*itConstant).getIndex()-1] = (*itConstant).get_Piofud();
      Kaofud[(*itConstant).getIndex()-1] = (*itConstant).get_Kaofud();
      Profud[(*itConstant).getIndex()-1] = (*itConstant).get_Profud();
   }
}



//
// F77 interface functions
//
extern "C" void
drmc_spefud_( FReal* Elefud,
	      FReal* Muofud,
	      FReal* Piofud,
	      FReal* Kaofud,
	      FReal* Profud,
	      FInteger* arraySize )
{
   report( DEBUG, kFacilityString ) << "DRMC_SPEFUD- F77 access to constants" << endl;

   // Get pointer to DRMC Spefud Frame Broker object
   DRMCSpefudFB* pFrameBroker = DRMCSpefudFB::instance();

   // Check array size
   if( *arraySize != DRMCSpefudFB::kNumberOfEntries ) {
      report( EMERGENCY, kFacilityString )
	 << "Input Array wrong size:"
	 << " arraySize = " << *arraySize
	 << " DRMCSpefudFB::kNumberOfEntries = " << DRMCSpefudFB::kNumberOfEntries
	 << endl;
					   
      assert( *arraySize == DRMCSpefudFB::kNumberOfEntries );
      ::exit(-1);
   }

   // Fill arrays via fillArrays function
   pFrameBroker->fillArrays( pFrameBroker,
			     Elefud,
			     Muofud,
			     Piofud,
			     Kaofud,
			     Profud,
			     *arraySize );
}   
