#if !defined(ZDMCUTIL_ZDMCFFFB_H)
#define ZDMCUTIL_ZDMCRESOLUTIONSFB_H
// -*- C++ -*-
//
// Package:     <ZDMCUtil>
// Module:      ZDMCResolutionsFB
// 
// Description: Frame Broker object for ZD MC Constants - MCResolutions
//              for Fortran access to these constants.  
//              Based on Mike Marsh's K3FFrameBroker object.
//
// Usage:
//  This is a Singleton pattern (_Design_Patterns_, p.127).
//  The unique instance holds smart pointer to DBZDMCResolutions m_resolutions
//  which holds a FAConstants collection of DBZDMCResolutions objects
//
//  Need to initialize object with constants extracted from the Frame
//  via setXXX( XXX ) function.
//
//  The function zdmc_resolutions_ provides Fortran access to these constants.
//  See this function to see how to use this object.
//
//  The Fortran function TestZDMCConstantsProc/Fortran/test_resolutions.F calls
//  zdmc_resolutions_ and prints out the constants
//
//  The TestZDMCConstantsProc Suez processor initializes the ZDMCxxxxFB
//  objects and then calls Fortran subroutines to test all of the constants
//
// Author:      Guangshun Huang
// Created:     Tue Dec 17, 2002
//
// Revision history
//

// system include files

// user include files
#include "Cleo/fortran_types.h"
#include "DBZDMCResolutions.hh"
#include "FrameAccess/FAConstants.h"

#include "AZDGeom/AZDSenseWireStore.h"


// forward declarations

class ZDMCResolutionsFB
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      // Need to update # of entries in $C3_LIB/data/cleo2_90494.zdmcresolutions
      // 10/21/99 - not sure where this # comes from. 20 bins / layer = 2000 for cleo2 ???
      // 01/08/01 D.M. - 20 bins / layer * 47 layers = 940 for cleo3
      // 03/28/03 hgs: this will be 120 for the ZD
      enum {kNumberOfEntries=120};

      // Constructors and destructor

      // member functions

      // Initialize object with FAConstants collection of DBZDMCResolutions objects
      void setResolutions( const FAConstants< DBZDMCResolutions > aResolutions );

      // Return a DBZDMCResolutions object for a given entry
      const DBZDMCResolutions& getDBZDMCResolutions(unsigned int entry) const;
      
      // Fill Arrays with constants
      void fillArrays( ZDMCResolutionsFB* pFrameBroker,
		       FReal* Sigma1,
		       FReal* Sigma2,
		       FReal* Offst1,
		       FReal* Offst2,
		       FReal* Frac,
		       const FInteger& arraySize );

      // static member functions
      static ZDMCResolutionsFB* instance();
      static void destroy();

   protected:
      // protected member functions
      ZDMCResolutionsFB();
      virtual ~ZDMCResolutionsFB();

      // protected const member functions

   private:
      // Constructors and destructor
      ZDMCResolutionsFB( const ZDMCResolutionsFB& );  // stop default

      // assignment operator(s)
      const ZDMCResolutionsFB& operator=( const ZDMCResolutionsFB& );  // stop default

      // private member functions

      // private const member functions

      // data members
      // m_resolutions - save collection of DBZDMCResolutions constants
      FAConstants<DBZDMCResolutions> m_resolutions ;

      // static data members
      // m_instance - pointer to the instance of this singelton object
      static ZDMCResolutionsFB* m_instance;
};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "ZDMCUtil/Template/ZDMCResolutionsFB.cc"
//#endif

#endif /* ZDMCUTIL_ZDMCRESOLUTIONSFB_H */
