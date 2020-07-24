// -*- C++ -*-
//
// Package:     DRMCUtil
// Module:      DRMCCathnameFB
// 
// Description: Frame Broker object for DR MC Constants - Cathname
//              for Fortran access to these constants.  
//              Based on Mike Marsh's K3FFrameBroker object.
//
// Implementation:
//     <Notes on implementation>
//
// Author:      John J O'Neill
// Created:     Wed Jun 16 22:11:32 EDT 1999
// $Id: DRMCCathnameFB.cc,v 1.3 1999/10/10 14:33:04 cdj Exp $
//
// Revision history
//
// $Log: DRMCCathnameFB.cc,v $
// Revision 1.3  1999/10/10 14:33:04  cdj
// removed BDLclient dirctory from include
//
// Revision 1.2  1999/08/10 19:45:31  jjo
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
#include "DRMCUtil/DRMCCathnameFB.h"
#include "DBDRMCCathname.hh"
#include "FrameAccess/FAConstants.h"

#include <string>

// STL classes
//#include <string>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "DRMCUtil.DRMCCathnameFB" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DRMCCathnameFB.cc,v 1.3 1999/10/10 14:33:04 cdj Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//
DRMCCathnameFB* DRMCCathnameFB::m_instance = 0;

//
// constructors and destructor
//
DRMCCathnameFB::DRMCCathnameFB()
{
}

// DRMCCathnameFB::DRMCCathnameFB( const DRMCCathnameFB& )
// {
// }

DRMCCathnameFB::~DRMCCathnameFB()
{
}

//
// assignment operators
//
// const DRMCCathnameFB& DRMCCathnameFB::operator=( const DRMCCathnameFB& )
// {
// }

//
// member functions
//

void DRMCCathnameFB::setCathname( DBDRMCCathname aCathodeName ) 
{
   //-> Save cathode name
   // We only have 1 entry so use DBDRMCCathname
   // instead of FAConstants<DBDRMCCathname>
   m_cathodeName = aCathodeName;
   return;
}


//
// const member functions
//
const char*
DRMCCathnameFB::cathname() const
{
   return m_cathodeName.get_Name();
}

//
// static member functions
//
DRMCCathnameFB*
DRMCCathnameFB::instance()
{
   //-> Creates instance of singleton object
   if ( 0 == m_instance )
   {
      report( DEBUG, kFacilityString ) << "Creating singleton object" << endl;
      m_instance = new DRMCCathnameFB;
   }

   return m_instance;
}

void
DRMCCathnameFB::destroy()
{
   //-> Destroys instance of singleton object
   if ( 0 != m_instance )
   {
      delete m_instance;
      m_instance = 0;
   }
   return;
}


//
// F77 interface functions
//
extern "C" void drmc_cathname_( FCharacter* cathname,
				FInteger cathnameLength )  
{
   // Function returns cathname string to Fortran routine

   // Get pointer to DRMC Cathname Frame Broker object
   DRMCCathnameFB* pFrameBroker = DRMCCathnameFB::instance();
   
   // Set cathode string with value from Frame Broker object
   string cathodeString ( pFrameBroker->cathname() );

   // Fortran  strings - wants a pointer to the string 
   // and after the Fortran argument list there is a
   // "hidden" length of the string that are also passed.
   //
   // copy string returned from FrameBroker into cathname
   strcpy (cathname, cathodeString.c_str());

   if ( cathodeString.length() > cathnameLength )
   {
      report( EMERGENCY, kFacilityString )
	 << "Fatal Error" << endl 
	 << "String length of cathodeString  > cathnameLength " << endl
	 << "Which will (did ?) cause a memory overwrite" << endl
	 << "The only honourable thing I can do is abort !" << endl;
      assert ( false );
      ::exit( 1 );
   }

   // If cathodeString.length() < cathnameLength then need to 'pad' fortran
   // character string with blanks else fortran gets messed up.  I haven't
   // written a routine to do this yet
   if ( cathodeString.length() < cathnameLength )
   {
      report( EMERGENCY, kFacilityString )
	 << "Fatal Error" << endl 
	 << "String length of cathodeString  < cathnameLength " << endl
	 << "F77 string will (did ?) contain extra chacaters which is wrong" << endl
	 << "The only honourable thing I can do is abort !" << endl;
      assert ( false );
      ::exit( 1 );
   }
}   
