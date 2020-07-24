// -*- C++ -*-
//
// Package:     DRMCUtil
// Module:      DRMCResolutionsFB
// 
// Description: Frame Broker object for DR MC Constants - Resolutions
//              for Fortran access to these constants.  
//              Based on Mike Marsh's K3FFrameBroker object.

//
// Implementation:
//     <Notes on implementation>
//
// Author:      John J O'Neill
// Created:     Wed Jun 16 22:11:32 EDT 1999
// $Id: DRMCResolutionsFB.cc,v 1.4 1999/10/15 18:31:34 jjo Exp $
//
// Revision history
//
// $Log: DRMCResolutionsFB.cc,v $
// Revision 1.4  1999/10/15 18:31:34  jjo
// Remove debugging printout messages
//
// Revision 1.3  1999/10/10 14:33:09  cdj
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

#include "DRMCUtil/DRMCResolutionsFB.h"
#include "DBDRMCResolutions.hh"

#include "FrameAccess/extract.h"
#include "FrameAccess/FAConstants.h"

#include <string>

// STL classes
//#include <string>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "DRMCUtil.DRMCResolutionsFB" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DRMCResolutionsFB.cc,v 1.4 1999/10/15 18:31:34 jjo Exp $";
static const char* const kTagString = "$Name:  $";


//
// static data member definitions
//
DRMCResolutionsFB* DRMCResolutionsFB::m_instance = 0;

//
// constructors and destructor
//
DRMCResolutionsFB::DRMCResolutionsFB()
{
}

// DRMCResolutionsFB::DRMCResolutionsFB( const DRMCResolutionsFB& )
// {
// }

DRMCResolutionsFB::~DRMCResolutionsFB()
{
}

//
// assignment operators
//
// const DRMCResolutionsFB& DRMCResolutionsFB::operator=( const DRMCResolutionsFB& )
// {
// }

//
// member functions
//


void DRMCResolutionsFB::setResolutions( const FAConstants< DBDRMCResolutions > aResolutions ) 
{
   //-> Save FAConstans container of DBDRMCResolutions constants
   m_resolutions = aResolutions;
   return;
}


//
// const member functions
//
//
const DBDRMCResolutions& DRMCResolutionsFB::getDBDRMCResolutions( unsigned int entry) const
{
   //-> Return a DBDRMCResolutions object for a given entry

   // Error checking valid entry # (unsigned int requires entry > 0)
   if ( entry > DRMCResolutionsFB::kNumberOfEntries  ) {
      report( EMERGENCY, kFacilityString )
	 << "Invalid constant entry = "
	 << entry
	 << " DRMCResolutionsFB::kNumberOfEntries = "
	 << DRMCResolutionsFB::kNumberOfEntries << endl;
      ::exit(-1);
   }

   return m_resolutions[entry];
}


//
// static member functions
//
DRMCResolutionsFB*
DRMCResolutionsFB::instance()
{
   //-> Creates instance of singleton object

   if ( 0 == m_instance )
   {
      report( DEBUG, kFacilityString ) << "Creating singleton object" << endl;
      m_instance = new DRMCResolutionsFB;
   }

   return m_instance;
}

void
DRMCResolutionsFB::destroy()
{
   //-> Destroys instance of singleton object
   if ( 0 != m_instance )
   {
      delete m_instance;
      m_instance = 0;
   }
   return;
}


void DRMCResolutionsFB::fillArrays( DRMCResolutionsFB* pFrameBroker,
			       FReal* Sigma1,
			       FReal* Sigma2,
			       FReal* Offst1,
			       FReal* Offst2,
			       FReal* Frac,
			       const FInteger& arraySize )
{
   //-> Function fills arrays of constants 
   //  iterate over collection of FAConstants and fill arrays
   FAConstants<DBDRMCResolutions>::const_iterator itEnd   = m_resolutions.end();
   FAConstants<DBDRMCResolutions>::const_iterator itBegin = m_resolutions.begin();

   // Require arraySize = length of FAConstant container
   if ( arraySize != (itEnd-itBegin) ) {
      assert( arraySize == (itEnd - itBegin) );
      ::exit( -1 );
   }

   for( FAConstants<DBDRMCResolutions>::const_iterator itConstant = itBegin;
	itConstant != itEnd;
	++itConstant ) {
      // Need getIndex()-1 because constants start with index=1, not 0
      // Fortran access works normal - starts with 1
      Sigma1[(*itConstant).getIndex()-1] = (*itConstant).get_Sigma1();
      Sigma2[(*itConstant).getIndex()-1] = (*itConstant).get_Sigma2();
      Offst1[(*itConstant).getIndex()-1] = (*itConstant).get_Offst1();
      Offst2[(*itConstant).getIndex()-1] = (*itConstant).get_Offst2();
        Frac[(*itConstant).getIndex()-1] = (*itConstant).get_Frac();
   }

}


//
// F77 interface functions
//
extern "C" void
drmc_resolutions_( FReal* Sigma1,
		   FReal* Sigma2,
		   FReal* Offst1,
		   FReal* Offst2,
		   FReal* Frac,  
		   FInteger* arraySize )
{
   
   report( INFO, kFacilityString ) << "DRMC_RESOLUTIONS-F77 access to constants" << endl;

   // Get pointer to DRMC Resolutions Frame Broker object
   DRMCResolutionsFB* pFrameBroker = DRMCResolutionsFB::instance();

   // Check array size
   // 1) Is array passed in big enough ?
   if( *arraySize != DRMCResolutionsFB::kNumberOfEntries ) {
      report( EMERGENCY, kFacilityString )
	 << "Input Array wrong size:"
	 << " arraySize = " << *arraySize
	 << " DRMCResolutionsFB::kNumberOfEntries = " << DRMCResolutionsFB::kNumberOfEntries
	 << endl;
					   
      assert( *arraySize == DRMCResolutionsFB::kNumberOfEntries );
      ::exit(-1);
   }

   // Fill arrays via fillArrays function
   pFrameBroker->fillArrays( pFrameBroker,
			     Sigma1,
			     Sigma2,
			     Offst1,
			     Offst2,
			     Frac,
			     *arraySize );
   
}
