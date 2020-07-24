#if !defined(DRMCUTIL_DRMCFFFB_H)
#define DRMCUTIL_DRMCRESOLUTIONSFB_H
// -*- C++ -*-
//
// Package:     <DRMCUtil>
// Module:      DRMCResolutionsFB
// 
// Description: Frame Broker object for DR MC Constants - MCResolutions
//              for Fortran access to these constants.  
//              Based on Mike Marsh's K3FFrameBroker object.
//
// Usage:
//  This is a Singleton pattern (_Design_Patterns_, p.127).
//  The unique instance holds smart pointer to DBDRMCResolutions m_resolutions
//  which holds a FAConstants collection of DBDRMCResolutions objects
//
//  Need to initialize object with constants extracted from the Frame
//  via setXXX( XXX ) function.
//
//  The function drmc_resolutions_ provides Fortran access to these constants.
//  See this function to see how to use this object.
//
//  The Fortran function TestDRMCConstantsProc/Fortran/test_resolutions.F calls
//  drmc_resolutions_ and prints out the constants
//
//  The TestDRMCConstantsProc Suez processor initializes the DRMCxxxxFB
//  objects and then calls Fortran subroutines to test all of the constants
//
// Author:      John J O'Neill
// Created:     Wed Jun 30 23:20:59 EDT 1999
// $Id: DRMCResolutionsFB.h,v 1.5 2001/01/10 16:38:05 dubrovin Exp $
//
// Revision history
//
// $Log: DRMCResolutionsFB.h,v $
// Revision 1.5  2001/01/10 16:38:05  dubrovin
// Set kNumberOfEntries=940 in stead of 2000
//
// Revision 1.4  1999/10/26 19:03:44  jjo
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
#include "DBDRMCResolutions.hh"
#include "FrameAccess/FAConstants.h"

#include "ADRGeom/ADRSenseWireStore.h"


// forward declarations

class DRMCResolutionsFB
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      // Need to update # of entries in $C3_LIB/data/cleo2_90494.drmcresolutions
      // 10/21/99 - not sure where this # comes from. 20 bins / layer = 2000 for cleo2 ???
      // 01/08/01 D.M. - 20 bins / layer * 47 layers = 940 for cleo3
      enum {kNumberOfEntries=940};

      // Constructors and destructor

      // member functions

      // Initialize object with FAConstants collection of DBDRMCResolutions objects
      void setResolutions( const FAConstants< DBDRMCResolutions > aResolutions );

      // Return a DBDRMCResolutions object for a given entry
      const DBDRMCResolutions& getDBDRMCResolutions(unsigned int entry) const;
      
      // Fill Arrays with constants
      void fillArrays( DRMCResolutionsFB* pFrameBroker,
		       FReal* Sigma1,
		       FReal* Sigma2,
		       FReal* Offst1,
		       FReal* Offst2,
		       FReal* Frac,
		       const FInteger& arraySize );

      // static member functions
      static DRMCResolutionsFB* instance();
      static void destroy();

   protected:
      // protected member functions
      DRMCResolutionsFB();
      virtual ~DRMCResolutionsFB();

      // protected const member functions

   private:
      // Constructors and destructor
      DRMCResolutionsFB( const DRMCResolutionsFB& );  // stop default

      // assignment operator(s)
      const DRMCResolutionsFB& operator=( const DRMCResolutionsFB& );  // stop default

      // private member functions

      // private const member functions

      // data members
      // m_resolutions - save collection of DBDRMCResolutions constants
      FAConstants<DBDRMCResolutions> m_resolutions ;

      // static data members
      // m_instance - pointer to the instance of this singelton object
      static DRMCResolutionsFB* m_instance;
};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "DRMCUtil/Template/DRMCResolutionsFB.cc"
//#endif

#endif /* DRMCUTIL_DRMCRESOLUTIONSFB_H */
