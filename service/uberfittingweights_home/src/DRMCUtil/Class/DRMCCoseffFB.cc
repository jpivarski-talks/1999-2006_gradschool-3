// -*- C++ -*-
//
// Package:     DRMCUtil
// Module:      DRMCCoseffFB
// 
// Description: Frame Broker object for DR MC Constants - Coseff
//              for Fortran access to these constants.  
//              Based on Mike Marsh's K3FFrameBroker object.

//
// Implementation:
//     <Notes on implementation>
//
// Author:      John J O'Neill
// Created:     Wed Jun 16 22:11:32 EDT 1999
// $Id: DRMCCoseffFB.cc,v 1.3 1999/10/10 14:33:05 cdj Exp $
//
// Revision history
//
// $Log: DRMCCoseffFB.cc,v $
// Revision 1.3  1999/10/10 14:33:05  cdj
// removed BDLclient dirctory from include
//
// Revision 1.2  1999/09/20 22:02:43  jjo
// Improve error checking
//
// Revision 1.1  1999/08/10 19:45:32  jjo
// Add Fortran access for DR Monte Carlo constants
//
// Revision 1.1.1.1  1999/06/21 15:50:16  jjo
// Import files into CVS
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <string>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "Experiment/fortran_types.h"

#include "DRMCUtil/DRMCCoseffFB.h"
#include "DBDRMCCoseff.hh"

#include "FrameAccess/extract.h"
#include "FrameAccess/FAConstants.h"

#include <string>

// STL classes
//#include <string>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "DRMCUtil.DRMCCoseffFB" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DRMCCoseffFB.cc,v 1.3 1999/10/10 14:33:05 cdj Exp $";
static const char* const kTagString = "$Name:  $";


//
// static data member definitions
//
DRMCCoseffFB* DRMCCoseffFB::m_instance = 0;

//
// constructors and destructor
//
DRMCCoseffFB::DRMCCoseffFB()
{
}

// DRMCCoseffFB::DRMCCoseffFB( const DRMCCoseffFB& )
// {
// }

DRMCCoseffFB::~DRMCCoseffFB()
{
}

//
// assignment operators
//
// const DRMCCoseffFB& DRMCCoseffFB::operator=( const DRMCCoseffFB& )
// {
// }

//
// member functions
//


void DRMCCoseffFB::setCoseff( const FAConstants< DBDRMCCoseff > aCoseff ) 
{
   //-> Save FAConstans container of DBDRMCCoseff constants
   m_coseff = aCoseff;
   return;
}


//
// const member functions
//
//

const DBDRMCCoseff& DRMCCoseffFB::getDBDRMCCoseff( unsigned int entry) const
{
   //-> Return a DBDRMCCoseff object for a given entry

   // Error checking valid entry # (unsigned int requires entry > 0)
   if ( entry > DRMCCoseffFB::kNumberOfEntries  ) {
      report( EMERGENCY, kFacilityString )
	 << "Invalid constant entry = "
	 << entry
	 << " DRMCCoseffFB::kNumberOfEntries = "
	 << DRMCCoseffFB::kNumberOfEntries << endl;
      ::exit(-1);
   }

   return m_coseff[entry];
}


//
// static member functions
//
DRMCCoseffFB*
DRMCCoseffFB::instance()
{
   //-> Creates instance of singleton object
   if ( 0 == m_instance )
   {
      report( DEBUG, kFacilityString ) << "Creating singleton object" << endl;
      m_instance = new DRMCCoseffFB;

   }

   return m_instance;
}

void
DRMCCoseffFB::destroy()
{
   //-> Destroys instance of singleton object
   if ( 0 != m_instance )
   {
      delete m_instance;
      m_instance = 0;
   }
   return;
}


void DRMCCoseffFB::fillArrays( DRMCCoseffFB* pFrameBroker,
			       FReal* Min,
			       FReal* Max,
			       FReal* Cheb1,
			       FReal* Cheb2,
			       const FInteger& arraySize )
{
   //-> Function fills arrays of constants 
   //  iterate over collection of FAConstants and fill arrays
   FAConstants<DBDRMCCoseff>::const_iterator itEnd   = m_coseff.end();
   FAConstants<DBDRMCCoseff>::const_iterator itBegin = m_coseff.begin();
   // Require arraySize = length of FAConstant container
   if ( arraySize != (itEnd-itBegin) ) {
      assert( arraySize == (itEnd - itBegin) );
      ::exit( -1 );
   }
   for( FAConstants<DBDRMCCoseff>::const_iterator itConstant = itBegin;
	itConstant != itEnd;
	++itConstant ) {
      // Need getIndex()-1 because constants start with index=1, not 0
      // Fortran access works normal - starts with 1
        Min[(*itConstant).getIndex()-1] = (*itConstant).get_Min();
        Max[(*itConstant).getIndex()-1] = (*itConstant).get_Max();
      Cheb1[(*itConstant).getIndex()-1] = (*itConstant).get_Cheb1();
      Cheb2[(*itConstant).getIndex()-1] = (*itConstant).get_Cheb2();
   }
}


//
// F77 interface functions
//
extern "C" void
drmc_coseff_( FReal* Min,
	      FReal* Max,
	      FReal* Cheb1,
	      FReal* Cheb2,
	      FInteger* arraySize )
{
   report( INFO, kFacilityString ) << "DRMC_COSEFF- F77 access to constants" << endl;

   // Get pointer to DRMC Coseff Frame Broker object
   DRMCCoseffFB* pFrameBroker = DRMCCoseffFB::instance();

   // Check array size
   if( *arraySize != DRMCCoseffFB::kNumberOfEntries ) {
      report( EMERGENCY, kFacilityString )
	 << "Input Array wrong size:"
	 << " arraySize = " << *arraySize
	 << " DRMCCoseffFB::kNumberOfEntries = " << DRMCCoseffFB::kNumberOfEntries
	 << endl;
					   
      assert( *arraySize == DRMCCoseffFB::kNumberOfEntries );
      ::exit(-1);
   }

   // Fill arrays via fillArrays function
   pFrameBroker->fillArrays( pFrameBroker,
			     Min,
			     Max,
			     Cheb1,
			     Cheb2,
			     *arraySize );
}   
