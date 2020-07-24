#if !defined(DRMCUTIL_DRMCFFFB_H)
#define DRMCUTIL_DRMCLYREFFFB_H
// -*- C++ -*-
//
// Package:     <DRMCUtil>
// Module:      DRMCLyreffFB
// 
// Description: Frame Broker object for DR MC Constants - MCLyreff
//              for Fortran access to these constants.  
//              Based on Mike Marsh's K3FFrameBroker object.
//
// Usage:
//  This is a Singleton pattern (_Design_Patterns_, p.127).
//  The unique instance holds smart pointer to DBDRMCLyreff m_lyreff
//  which holds a FAConstants collection of DBDRMCLyreff objects
//
//  Need to initialize object with constants extracted from the Frame
//  via setXXX( XXX ) function.
//
//  The function drmc_lyreff_ provides Fortran access to these constants.
//  See this function to see how to use this object.
//
//  The Fortran function TestDRMCConstantsProc/Fortran/test_lyreff.F calls
//  drmc_lyreff_ and prints out the constants
//
//  The TestDRMCConstantsProc Suez processor initializes the DRMCxxxxFB
//  objects and then calls Fortran subroutines to test all of the constants
//
// Author:      John J O'Neill
// Created:     Wed Jun 30 23:20:59 EDT 1999
// $Id: DRMCLyreffFB.h,v 1.4 1999/10/26 19:03:40 jjo Exp $
//
// Revision history
//
// $Log: DRMCLyreffFB.h,v $
// Revision 1.4  1999/10/26 19:03:40  jjo
// Update to Cleo3 geometry.  Uses ADRGeom instead of hardwired numbers
//
// Revision 1.3  1999/10/10 14:33:18  cdj
// removed BDLclient dirctory from include
//
// Revision 1.2  1999/09/13 14:22:01  jjo
// Add fortran_types.h include which is now needed
//
// Revision 1.1  1999/08/10 19:45:47  jjo
// Add Fortran access for DR Monte Carlo constants
//
//

// system include files

// user include files
#include "Cleo/fortran_types.h"
#include "DBDRMCLyreff.hh"
#include "FrameAccess/FAConstants.h"

#include "ADRGeom/ADRSenseWireStore.h"


// forward declarations

class DRMCLyreffFB
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      // Need to update # of entries in $C3_LIB/data/cleo2_90494.drmclyreff
      // 10/21/99 Changing to Cleo3 geometry - One constant per layer
      enum {kNumberOfEntries=ADRSenseWireStore::kNumberOfLayers};


      // Constructors and destructor

      // member functions

      // Initialize object with FAConstants collection of DBDRMCLyreff objects
      void setLyreff( const FAConstants< DBDRMCLyreff > aLyreff );

      // Return a DBDRMCLyreff object for a given entry
      // TODO : Make this const correct !
      const DBDRMCLyreff& getDBDRMCLyreff(unsigned int entry) const;
      
      // Fill Arrays with constants
      void fillArrays( DRMCLyreffFB* pFrameBroker,
		       FReal* Slpn,
		       FReal* Slpp,
		       FReal* Dstn,
		       FReal* Dstp,
		       FReal* Norm,
		       FReal* Chb1,
		       FReal* Chb2,
		       FReal* Slpm,
		       FReal* Xr,  
		       const FInteger& arraySize );

      // static member functions
      static DRMCLyreffFB* instance();
      static void destroy();

   protected:
      // protected member functions
      DRMCLyreffFB();
      virtual ~DRMCLyreffFB();

      // protected const member functions

   private:
      // Constructors and destructor
      DRMCLyreffFB( const DRMCLyreffFB& );  // stop default

      // assignment operator(s)
      const DRMCLyreffFB& operator=( const DRMCLyreffFB& );  // stop default

      // private member functions

      // private const member functions

      // data members
      // m_lyreff - save collection of DBDRMCLyreff constants
      FAConstants<DBDRMCLyreff> m_lyreff ;

      // static data members
      // m_instance - pointer to the instance of this singelton object
      static DRMCLyreffFB* m_instance;
};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "DRMCUtil/Template/DRMCLyreffFB.cc"
//#endif

#endif /* DRMCUTIL_DRMCLYREFFFB_H */
