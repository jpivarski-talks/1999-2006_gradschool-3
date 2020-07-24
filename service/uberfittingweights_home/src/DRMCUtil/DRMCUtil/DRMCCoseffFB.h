#if !defined(DRMCUTIL_DRMCCOSEFFFB_H)
#define DRMCUTIL_DRMCCOSEFFFB_H
// -*- C++ -*-
//
// Package:     <DRMCUtil>
// Module:      DRMCCoseffFB
// 
// Description: Frame Broker object for DR MC Constants - Coseff
//              for Fortran access to these constants.  
//              Based on Mike Marsh's K3FFrameBroker object.
//
// Usage:
//  This is a Singleton pattern (_Design_Patterns_, p.127).
//  The unique instance holds smart pointer to DBDRMCCoseff m_coseff
//  which holds a FAConstants collection of DBDRMCCoseff objects
//
//  Need to initialize object with constants extracted from the Frame
//  via setXXX( XXX ) function.
//
//  The function drmc_coseff_ provides Fortran access to these constants.
//  See this function to see how to use this object.
//
//  The Fortran function TestDRMCConstantsProc/Fortran/test_coseff.F calls
//  drmc_coseff_ and prints out the constants
//
//  The TestDRMCConstantsProc Suez processor initializes the DRMCxxxxFB
//  objects and then calls Fortran subroutines to test all of the constants
//
// Author:      John J O'Neill
// Created:     Wed Jun 30 23:20:59 EDT 1999
// $Id: DRMCCoseffFB.h,v 1.4 1999/10/26 19:03:35 jjo Exp $
//
// Revision history
//
// $Log: DRMCCoseffFB.h,v $
// Revision 1.4  1999/10/26 19:03:35  jjo
// Update to Cleo3 geometry.  Uses ADRGeom instead of hardwired numbers
//
// Revision 1.3  1999/10/10 14:33:16  cdj
// removed BDLclient dirctory from include
//
// Revision 1.2  1999/09/13 14:21:59  jjo
// Add fortran_types.h include which is now needed
//
// Revision 1.1  1999/08/10 19:45:45  jjo
// Add Fortran access for DR Monte Carlo constants
//
//

// system include files

// user include files
#include "Cleo/fortran_types.h"
#include "DBDRMCCoseff.hh"
#include "FrameAccess/FAConstants.h"

#include "ADRGeom/ADRSenseWireStore.h"

// forward declarations

class DRMCCoseffFB
{
      // friend classes and functions

   public:
      // constants, enums and typedefs 
      // Need to update # of entries in $C3_LIB/data/cleo2_90494.drmccoseff
      // 10/21/99 Changing to Cleo3 geometry - One constant per layer
      enum {kNumberOfEntries=ADRSenseWireStore::kNumberOfLayers};

      // Constructors and destructor

      // member functions

      // Initialize object with FAConstants collection of DBDRMCCoseff objects
      void setCoseff( const FAConstants< DBDRMCCoseff > aCoseff );

      // Return a DBDRMCCoseff object for a given entry
      const DBDRMCCoseff& getDBDRMCCoseff(unsigned int entry) const;
      
      // Fill Arrays with constants
      void fillArrays( DRMCCoseffFB* pFrameBroker,
		       FReal* Min,
		       FReal* Max,
		       FReal* Cheb1,
		       FReal* Cheb2,
		       const FInteger& arraySize );

      // static member functions
      static DRMCCoseffFB* instance();
      static void destroy();

   protected:
      // protected member functions
      DRMCCoseffFB();
      virtual ~DRMCCoseffFB();

      // protected const member functions

   private:
      // Constructors and destructor
      DRMCCoseffFB( const DRMCCoseffFB& );  // stop default

      // assignment operator(s)
      const DRMCCoseffFB& operator=( const DRMCCoseffFB& );  // stop default

      // private member functions

      // private const member functions

      // data members
      // m_coseff - holds collection of DBDRMCCoseff constants
      FAConstants<DBDRMCCoseff> m_coseff ;

      // static data members
      // m_instance - pointer to the instance of this singelton object
      static DRMCCoseffFB* m_instance;
};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "DRMCUtil/Template/DRMCCoseffFB.cc"
//#endif

#endif /* DRMCUTIL_DRMCCOSEFFFB_H */
