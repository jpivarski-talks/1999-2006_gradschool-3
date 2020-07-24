#if !defined(DRMCUTIL_DRMCFFFB_H)
#define DRMCUTIL_DRMCEFFFB_H
// -*- C++ -*-
//
// Package:     <DRMCUtil>
// Module:      DRMCEffFB
// 
// Description: Frame Broker object for DR MC Constants - MCeff
//              for Fortran access to these constants.  
//              Based on Mike Marsh's K3FFrameBroker object.
//
// Usage:
//  This is a Singleton pattern (_Design_Patterns_, p.127).
//  The unique instance holds smart pointer to DBDRMCEff m_eff
//  which holds a FAConstants collection of DBDRMCEff objects
//
//  Need to initialize object with constants extracted from the Frame
//  via setXXX( XXX ) function.
//
//  The function drmc_eff_ provides Fortran access to these constants.
//  See this function to see how to use this object.
//
//  The Fortran function TestDRMCConstantsProc/Fortran/test_eff.F calls
//  drmc_eff_ and prints out the constants
//
//  The TestDRMCConstantsProc Suez processor initializes the DRMCxxxxFB
//  objects and then calls Fortran subroutines to test all of the constants
//
//
// Author:      John J O'Neill
// Created:     Wed Jun 30 23:20:59 EDT 1999
// $Id: DRMCEffFB.h,v 1.4 1999/10/26 19:03:37 jjo Exp $
//
// Revision history
//
// $Log: DRMCEffFB.h,v $
// Revision 1.4  1999/10/26 19:03:37  jjo
// Update to Cleo3 geometry.  Uses ADRGeom instead of hardwired numbers
//
// Revision 1.3  1999/10/10 14:33:17  cdj
// removed BDLclient dirctory from include
//
// Revision 1.2  1999/09/13 14:22:00  jjo
// Add fortran_types.h include which is now needed
//
// Revision 1.1  1999/08/10 19:45:46  jjo
// Add Fortran access for DR Monte Carlo constants
//
//

// system include files

// user include files
#include "Cleo/fortran_types.h"
#include "DBDRMCEff.hh"
#include "FrameAccess/FAConstants.h"

#include "ADRGeom/ADRSenseWireStore.h"


// forward declarations

class DRMCEffFB
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      // Need to update # of entries in $C3_LIB/data/cleo2_90494.drmceff
      // 10/21/99 Changing to Cleo3 geometry - One constant per cathode layer
      enum {kNumberOfEntries=1};

      // Constructors and destructor

      // member functions

      // Initialize object with FAConstants collection of DBDRMCEff objects
      void setEff( const FAConstants< DBDRMCEff > aEff );

      // Return a DBDRMCEff object for a given entry
      // TODO : Make this const correct !
      const DBDRMCEff& getDBDRMCEff(unsigned int entry) const;
      
      // Fill Arrays with constants
      void fillArrays( DRMCEffFB* pFrameBroker,
		       FReal* A,
		       FReal* B,
		       FReal* C,
		       const FInteger& arraySize );

      // static member functions
      static DRMCEffFB* instance();
      static void destroy();

   protected:
      // protected member functions
      DRMCEffFB();
      virtual ~DRMCEffFB();

      // protected const member functions

   private:
      // Constructors and destructor
      DRMCEffFB( const DRMCEffFB& );  // stop default

      // assignment operator(s)
      const DRMCEffFB& operator=( const DRMCEffFB& );  // stop default

      // private member functions

      // private const member functions

      // data members
      // m_eff - save collection of DBDRMCEff constants
      FAConstants<DBDRMCEff> m_eff ;

      // static data members
      // m_instance - pointer to the instance of this singelton object
      static DRMCEffFB* m_instance;
};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "DRMCUtil/Template/DRMCEffFB.cc"
//#endif

#endif /* DRMCUTIL_DRMCEFFFB_H */
