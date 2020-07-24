#if !defined(DRMCUTIL_DRMCFFFB_H)
#define DRMCUTIL_DRMCWIREFFFB_H
// -*- C++ -*-
//
// Package:     <DRMCUtil>
// Module:      DRMCWireffFB
// 
// Description: Frame Broker object for DR MC Constants - MCWireff
//              for Fortran access to these constants.  
//              Based on Mike Marsh's K3FFrameBroker object.
//
// Usage:
//  This is a Singleton pattern (_Design_Patterns_, p.127).
//  The unique instance holds smart pointer to DBDRMCWireff m_wireff
//  which holds a FAConstants collection of DBDRMCWireff objects
//
//  Need to initialize object with constants extracted from the Frame
//  via setXXX( XXX ) function.
//
//  The function drmc_wireff_ provides Fortran access to these constants
//  
//  The Fortran function TestDRMCConstantsProc/Fortran/test_wireff.F calls
//  drmc_wireff and prints out the constants.
//
//  The TestDRMCConstantsProc Suez processor initializes the DRMCxxxxFB
//  objects and then calls Fortran subroutines to test all of the constants
//
// Author:      John J O'Neill
// Created:     Wed Jun 30 23:20:59 EDT 1999
// $Id: DRMCWireffFB.h,v 1.4 1999/10/26 19:03:49 jjo Exp $
//
// Revision history
//
// $Log: DRMCWireffFB.h,v $
// Revision 1.4  1999/10/26 19:03:49  jjo
// Update to Cleo3 geometry.  Uses ADRGeom instead of hardwired numbers
//
// Revision 1.3  1999/10/10 14:33:22  cdj
// removed BDLclient dirctory from include
//
// Revision 1.2  1999/09/13 14:22:03  jjo
// Add fortran_types.h include which is now needed
//
// Revision 1.1  1999/08/10 19:45:50  jjo
// Add Fortran access for DR Monte Carlo constants
//
//

// system include files

// user include files
#include "Cleo/fortran_types.h"
#include "DBDRMCWireff.hh"
#include "FrameAccess/FAConstants.h"

#include "ADRGeom/ADRSenseWireStore.h"

// forward declarations

class DRMCWireffFB
{
      // friend classes and functions

   public:
      // constants, enums and typedefs 
      // Need to update # of entries in $C3_LIB/data/cleo2_90494.drmcwireff
      // 10/21/99 Change to Cleo3 geometry - One constant per wire
      enum {kNumberOfEntries=ADRSenseWireStore::kNumberOfWiresTotal};

   
      // Constructors and destructor

      // member functions

      // Initialize object with FAConstants collection of DBDRMCWireff objects
      void setWireff( const FAConstants< DBDRMCWireff > aWireff );

      // Return a DBDRMCWireff object for a given entry
      const DBDRMCWireff& getDBDRMCWireff(unsigned int entry) const;

      // Fill Arrays with constants
      void fillArrays( DRMCWireffFB* pFrameBroker,
		       FReal* Effi,
		       const FInteger& arraySize );

      // static member functions
      static DRMCWireffFB* instance();
      static void destroy();

   protected:
      // protected member functions
      DRMCWireffFB();
      virtual ~DRMCWireffFB();

      // protected const member functions

   private:
      // Constructors and destructor
      DRMCWireffFB( const DRMCWireffFB& );  // stop default

      // assignment operator(s)
      const DRMCWireffFB& operator=( const DRMCWireffFB& );  // stop default

      // private member functions

      // private const member functions

      // data members
      // m_wireff - save collection of DBDRMCWireff constants
      FAConstants<DBDRMCWireff> m_wireff ;

      // static data members
      // m_instance - pointer to the instance of this singelton object
      static DRMCWireffFB* m_instance;
};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "DRMCUtil/Template/DRMCWireffFB.cc"
//#endif

#endif /* DRMCUTIL_DRMCWIREFFFB_H */
