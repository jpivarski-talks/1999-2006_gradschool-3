// -*- C++ -*-
//
// Package:     ZDMCUtil
// Module:      ZDMCResolutionsFB
// 
// Description: Frame Broker object for ZD MC Constants - Resolutions
//              for Fortran access to these constants.  
//              Based on Mike Marsh's K3FFrameBroker object.
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Guangshun Huang
// Created:     Tue Dec 17, 2002
//
// Revision history
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <string>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "Experiment/fortran_types.h"

#include "ZDMCUtil/ZDMCResolutionsFB.h"
#include "DBZDMCResolutions.hh"

#include "FrameAccess/extract.h"
#include "FrameAccess/FAConstants.h"

#include <string>

// STL classes
//#include <string>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "ZDMCUtil.ZDMCResolutionsFB" ;

//
// static data member definitions
//
ZDMCResolutionsFB* ZDMCResolutionsFB::m_instance = 0;

//
// constructors and destructor
//
ZDMCResolutionsFB::ZDMCResolutionsFB()
{
}

// ZDMCResolutionsFB::ZDMCResolutionsFB( const ZDMCResolutionsFB& )
// {
// }

ZDMCResolutionsFB::~ZDMCResolutionsFB()
{
}

//
// assignment operators
//
// const ZDMCResolutionsFB& ZDMCResolutionsFB::operator=( const ZDMCResolutionsFB& )
// {
// }

//
// member functions
//


void ZDMCResolutionsFB::setResolutions( const FAConstants< DBZDMCResolutions > aResolutions ) 
{
   //-> Save FAConstans container of DBZDMCResolutions constants
   m_resolutions = aResolutions;
   return;
}


//
// const member functions
//
//
const DBZDMCResolutions& ZDMCResolutionsFB::getDBZDMCResolutions( unsigned int entry) const
{
   //-> Return a DBZDMCResolutions object for a given entry

   // Error checking valid entry # (unsigned int requires entry > 0)
   if ( entry > ZDMCResolutionsFB::kNumberOfEntries  ) {
      report( EMERGENCY, kFacilityString )
	 << "Invalid constant entry = "
	 << entry
	 << " ZDMCResolutionsFB::kNumberOfEntries = "
	 << ZDMCResolutionsFB::kNumberOfEntries << endl;
      ::exit(-1);
   }

   return m_resolutions[entry];
}


//
// static member functions
//
ZDMCResolutionsFB*
ZDMCResolutionsFB::instance()
{
   //-> Creates instance of singleton object

   if ( 0 == m_instance )
   {
      report( DEBUG, kFacilityString ) << "Creating singleton object" << endl;
      m_instance = new ZDMCResolutionsFB;
   }

   return m_instance;
}

void
ZDMCResolutionsFB::destroy()
{
   //-> Destroys instance of singleton object
   if ( 0 != m_instance )
   {
      delete m_instance;
      m_instance = 0;
   }
   return;
}


void ZDMCResolutionsFB::fillArrays( ZDMCResolutionsFB* pFrameBroker,
			       FReal* Sigma1,
			       FReal* Sigma2,
			       FReal* Offst1,
			       FReal* Offst2,
			       FReal* Frac,
			       const FInteger& arraySize )
{
   //-> Function fills arrays of constants 
   //  iterate over collection of FAConstants and fill arrays
   FAConstants<DBZDMCResolutions>::const_iterator itEnd   = m_resolutions.end();
   FAConstants<DBZDMCResolutions>::const_iterator itBegin = m_resolutions.begin();

   // Require arraySize = length of FAConstant container
   if ( arraySize != (itEnd-itBegin) ) {
      assert( arraySize == (itEnd - itBegin) );
      ::exit( -1 );
   }

   for( FAConstants<DBZDMCResolutions>::const_iterator itConstant = itBegin;
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
zdmc_resolutions_( FReal* Sigma1,
		   FReal* Sigma2,
		   FReal* Offst1,
		   FReal* Offst2,
		   FReal* Frac,  
		   FInteger* arraySize )
{
   
   report( INFO, kFacilityString ) << "ZDMC_RESOLUTIONS-F77 access to constants" << endl;

   // Get pointer to ZDMC Resolutions Frame Broker object
   ZDMCResolutionsFB* pFrameBroker = ZDMCResolutionsFB::instance();

   // Check array size
   // 1) Is array passed in big enough ?
   if( *arraySize != ZDMCResolutionsFB::kNumberOfEntries ) {
      report( EMERGENCY, kFacilityString )
	 << "Input Array wrong size:"
	 << " arraySize = " << *arraySize
	 << " ZDMCResolutionsFB::kNumberOfEntries = " << ZDMCResolutionsFB::kNumberOfEntries
	 << endl;
					   
      assert( *arraySize == ZDMCResolutionsFB::kNumberOfEntries );
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
