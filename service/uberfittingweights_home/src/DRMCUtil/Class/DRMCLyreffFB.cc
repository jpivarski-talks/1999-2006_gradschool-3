// -*- C++ -*-
//
// Package:     DRMCUtil
// Module:      DRMCLyreffFB
// 
// Description: Frame Broker object for DR MC Constants - Lyreff
//              for Fortran access to these constants.  
//              Based on Mike Marsh's K3FFrameBroker object.

//
// Implementation:
//     <Notes on implementation>
//
// Author:      John J O'Neill
// Created:     Wed Jun 16 22:11:32 EDT 1999
// $Id: DRMCLyreffFB.cc,v 1.3 1999/10/10 14:33:07 cdj Exp $
//
// Revision history
//
// $Log: DRMCLyreffFB.cc,v $
// Revision 1.3  1999/10/10 14:33:07  cdj
// removed BDLclient dirctory from include
//
// Revision 1.2  1999/09/20 22:02:47  jjo
// Improve error checking
//
// Revision 1.1  1999/08/10 19:45:34  jjo
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

#include "DRMCUtil/DRMCLyreffFB.h"
#include "DBDRMCLyreff.hh"

#include "FrameAccess/extract.h"
#include "FrameAccess/FAConstants.h"

#include <string>

// STL classes
//#include <string>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "DRMCUtil.DRMCLyreffFB" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DRMCLyreffFB.cc,v 1.3 1999/10/10 14:33:07 cdj Exp $";
static const char* const kTagString = "$Name:  $";


//
// static data member definitions
//
DRMCLyreffFB* DRMCLyreffFB::m_instance = 0;

//
// constructors and destructor
//
DRMCLyreffFB::DRMCLyreffFB()
{
}

// DRMCLyreffFB::DRMCLyreffFB( const DRMCLyreffFB& )
// {
// }

DRMCLyreffFB::~DRMCLyreffFB()
{
}

//
// assignment operators
//
// const DRMCLyreffFB& DRMCLyreffFB::operator=( const DRMCLyreffFB& )
// {
// }

//
// member functions
//


void DRMCLyreffFB::setLyreff( const FAConstants< DBDRMCLyreff > aLyreff ) 
{
   //-> Save FAConstans container of DBDRMCLyreff constants
   m_lyreff = aLyreff;
   return;
}


//
// const member functions
//
//

const DBDRMCLyreff& DRMCLyreffFB::getDBDRMCLyreff(unsigned int entry) const
{
   //-> Return a DBDRMCLyreff object for a given entry

   // Error checking valid entry # (unsigned int requires entry > 0)
   if ( entry > DRMCLyreffFB::kNumberOfEntries  ) {
      report( EMERGENCY, kFacilityString )
	 << "Invalid constant entry = "
	 << entry
	 << " DRMCLyreffFB::kNumberOfEntries = "
	 << DRMCLyreffFB::kNumberOfEntries << endl;
      ::exit(-1);
   }

   return m_lyreff[entry];
}


//
// static member functions
//
DRMCLyreffFB*
DRMCLyreffFB::instance()
{
   //-> Creates instance of singleton object
   if ( 0 == m_instance )
   {
      report( DEBUG, kFacilityString ) << "Creating singleton object" << endl;
      m_instance = new DRMCLyreffFB;

   }

   return m_instance;
}

void
DRMCLyreffFB::destroy()
{
   //-> Destroys instance of singleton object
   if ( 0 != m_instance )
   {
      delete m_instance;
      m_instance = 0;
   }
   return;
}


void DRMCLyreffFB::fillArrays( DRMCLyreffFB* pFrameBroker,
			       FReal* Slpn,
			       FReal* Slpp,
			       FReal* Dstn,
			       FReal* Dstp,
			       FReal* Norm,
			       FReal* Chb1,
			       FReal* Chb2,
			       FReal* Slpm,
			       FReal* Xr,  
			       const FInteger& arraySize )
{
   //-> Function fills arrays of constants 
   //  iterate over collection of FAConstants and fill arrays
   FAConstants<DBDRMCLyreff>::const_iterator itEnd   = m_lyreff.end();
   FAConstants<DBDRMCLyreff>::const_iterator itBegin = m_lyreff.begin();
   // Require arraySize = length of FAConstant container
   if ( arraySize != (itEnd-itBegin) ) {
      assert( arraySize == (itEnd - itBegin) );
      ::exit( -1 );
   }
   for( FAConstants<DBDRMCLyreff>::const_iterator itConstant = itBegin;
	itConstant != itEnd;
	++itConstant ) {
      // Need getIndex()-1 because constants start with index=1, not 0
      // Fortran access works normal - starts with 1
      Slpn[(*itConstant).getIndex()-1] = (*itConstant).get_Slpn();
      Slpp[(*itConstant).getIndex()-1] = (*itConstant).get_Slpp();
      Dstn[(*itConstant).getIndex()-1] = (*itConstant).get_Dstn();
      Dstp[(*itConstant).getIndex()-1] = (*itConstant).get_Dstp();
      Norm[(*itConstant).getIndex()-1] = (*itConstant).get_Norm();
      Chb1[(*itConstant).getIndex()-1] = (*itConstant).get_Chb1();
      Chb2[(*itConstant).getIndex()-1] = (*itConstant).get_Chb2();
      Slpm[(*itConstant).getIndex()-1] = (*itConstant).get_Slpm();
        Xr[(*itConstant).getIndex()-1] = (*itConstant).get_Xr();
   }
}


//
// F77 interface functions
//
extern "C" void
drmc_lyreff_( FReal* Slpn,
	      FReal* Slpp,
	      FReal* Dstn,
	      FReal* Dstp,
	      FReal* Norm,
	      FReal* Chb1,
	      FReal* Chb2,
	      FReal* Slpm,
	      FReal* Xr, 
	      FInteger* arraySize )
{
   report( INFO, kFacilityString ) << "DRMC_LYREFF- F77 access to constants" << endl;

   // Get pointer to DRMC Lyreff Frame Broker object
   DRMCLyreffFB* pFrameBroker = DRMCLyreffFB::instance();

   // Check array size
   if( *arraySize != DRMCLyreffFB::kNumberOfEntries ) {
      report( EMERGENCY, kFacilityString )
	 << "Input Array wrong size:"
	 << " arraySize = " << *arraySize
	 << " DRMCLyreffFB::kNumberOfEntries = " << DRMCLyreffFB::kNumberOfEntries
	 << endl;
					   
      assert( *arraySize == DRMCLyreffFB::kNumberOfEntries );
      ::exit(-1);

   }

   // Fill arrays via fillArrays function
   pFrameBroker->fillArrays( pFrameBroker,
			     Slpn,
			     Slpp,
			     Dstn,
			     Dstp,
			     Norm,
			     Chb1,
			     Chb2,
			     Slpm,
			     Xr,
			     *arraySize );
}   
