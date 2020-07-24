#if !defined(DRMCUTIL_DRMCFFFB_H)
#define DRMCUTIL_DRMCSMEARFB_H
// -*- C++ -*-
//
// Package:     <DRMCUtil>
// Module:      DRMCSmearFB
// 
// Description: Frame Broker object for DR MC Constants - MCSmear
//              for Fortran access to these constants.  
//              Based on Mike Marsh's K3FFrameBroker object.
//
// Usage:
//  This is a Singleton pattern (_Design_Patterns_, p.127).
//  The unique instance holds smart pointer to DBDRMCSmear m_smear
//  which holds a FAConstants collection of DBDRMCSmear objects
//
//  Need to initialize object with constants extracted from the Frame
//  via setXXX( XXX ) function.
//
//  The function drmc_smear_ provides Fortran access to these constants.
//  See this function to see how to use this object.
//
//  The Fortran function TestDRMCConstantsProc/Fortran/test_smear.F calls
//  drmc_smear_ and prints out the constants
//
//  The TestDRMCConstantsProc Suez processor initializes the DRMCxxxxFB
//  objects and then calls Fortran subroutines to test all of the constants
//
// Author:      John J O'Neill
// Created:     Wed Jun 30 23:20:59 EDT 1999
// $Id: DRMCSmearFB.h,v 1.4 1999/10/26 19:03:45 jjo Exp $
//
// Revision history
//
// $Log: DRMCSmearFB.h,v $
// Revision 1.4  1999/10/26 19:03:45  jjo
// Update to Cleo3 geometry.  Uses ADRGeom instead of hardwired numbers
//
// Revision 1.3  1999/10/10 14:33:20  cdj
// removed BDLclient dirctory from include
//
// Revision 1.2  1999/09/13 14:22:02  jjo
// Add fortran_types.h include which is now needed
//
// Revision 1.1  1999/08/10 19:45:48  jjo
// Add Fortran access for DR Monte Carlo constants
//
//

// system include files

// user include files
#include "Cleo/fortran_types.h"
#include "DBDRMCSmear.hh"
#include "FrameAccess/FAConstants.h"

#include "ADRGeom/ADRSenseWireStore.h"


// forward declarations

class DRMCSmearFB
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      // Need to update # of entries in $C3_LIB/data/cleo2_90494.drmcsmear
      // 10/21/99 Changing to Cleo3 geometry - One constant per cathode layer
      enum {kNumberOfEntries=1};

      // Constructors and destructor

      // member functions

      // Initialize object with FAConstants collection of DBDRMCSmear objects
      void setSmear( const FAConstants< DBDRMCSmear > aSmear );

      // Return a DBDRMCSmear object for a given entry
      const DBDRMCSmear& getDBDRMCSmear(unsigned int entry) const;
      
      // Fill Arrays with constants
      void fillArrays( DRMCSmearFB* pFrameBroker,
		       FReal* Dz,
		       const FInteger& arraySize );

      // static member functions
      static DRMCSmearFB* instance();
      static void destroy();

   protected:
      // protected member functions
      DRMCSmearFB();
      virtual ~DRMCSmearFB();

      // protected const member functions

   private:
      // Constructors and destructor
      DRMCSmearFB( const DRMCSmearFB& );  // stop default

      // assignment operator(s)
      const DRMCSmearFB& operator=( const DRMCSmearFB& );  // stop default

      // private member functions

      // private const member functions

      // data members
      // m_smear - save collection of DBDRMCSmear constants
      FAConstants<DBDRMCSmear> m_smear ;

      // static data members
      // m_instance - pointer to the instance of this singelton object
      static DRMCSmearFB* m_instance;
};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "DRMCUtil/Template/DRMCSmearFB.cc"
//#endif

#endif /* DRMCUTIL_DRMCSMEARFB_H */
