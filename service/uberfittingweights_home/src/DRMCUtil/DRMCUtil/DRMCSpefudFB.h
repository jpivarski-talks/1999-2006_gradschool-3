#if !defined(DRMCUTIL_DRMCFFFB_H)
#define DRMCUTIL_DRMCSPEFUDFB_H
// -*- C++ -*-
//
// Package:     <DRMCUtil>
// Module:      DRMCSpefudFB
// 
// Description: Frame Broker object for DR MC Constants - MCSpefud
//              for Fortran access to these constants.  
//              Based on Mike Marsh's K3FFrameBroker object.
//
// Usage:
//  This is a Singleton pattern (_Design_Patterns_, p.127).
//  The unique instance holds smart pointer to DBDRMCSpefud m_spefud
//  which holds a FAConstants collection of DBDRMCSpefud objects
//
//  Need to initialize object with constants extracted from the Frame
//  via setXXX( XXX ) function.
//
//  The function drmc_spefud_ provides Fortran access to these constants.
//  See this function to see how to use this object.
//
//  The Fortran function TestDRMCConstantsProc/Fortran/test_spefud.F calls
//  drmc_spefud_ and prints out the constants
//
//  The TestDRMCConstantsProc Suez processor initializes the DRMCxxxxFB
//  objects and then calls Fortran subroutines to test all of the constants
//
// Author:      John J O'Neill
// Created:     Wed Jun 30 23:20:59 EDT 1999
// $Id: DRMCSpefudFB.h,v 1.4 1999/10/26 19:03:47 jjo Exp $
//
// Revision history
//
// $Log: DRMCSpefudFB.h,v $
// Revision 1.4  1999/10/26 19:03:47  jjo
// Update to Cleo3 geometry.  Uses ADRGeom instead of hardwired numbers
//
// Revision 1.3  1999/10/10 14:33:21  cdj
// removed BDLclient dirctory from include
//
// Revision 1.2  1999/09/13 14:22:02  jjo
// Add fortran_types.h include which is now needed
//
// Revision 1.1  1999/08/10 19:45:49  jjo
// Add Fortran access for DR Monte Carlo constants
//
//

// system include files

// user include files
#include "Cleo/fortran_types.h"
#include "DBDRMCSpefud.hh"
#include "FrameAccess/FAConstants.h"

#include "ADRGeom/ADRSenseWireStore.h"


// forward declarations

class DRMCSpefudFB
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      // Need to update # of entries in $C3_LIB/data/cleo2_90494.drmcspefud
      // 10/21/99 Changing to Cleo3 geometry - One constant per layer
      enum {kNumberOfEntries=ADRSenseWireStore::kNumberOfLayers};

      // Constructors and destructor

      // member functions

      // Initialize object with FAConstants collection of DBDRMCSpefud objects
      void setSpefud( const FAConstants< DBDRMCSpefud > aSpefud );

      // Return a DBDRMCSpefud object for a given entry
      const DBDRMCSpefud& getDBDRMCSpefud(unsigned int entry) const;
      
      // Fill Arrays with constants
      void fillArrays( DRMCSpefudFB* pFrameBroker,
		       FReal* Elefud,
		       FReal* Muofud,
		       FReal* Piofud,
		       FReal* Kaofud,
		       FReal* Profud,
		       const FInteger& arraySize );

      // static member functions
      static DRMCSpefudFB* instance();
      static void destroy();

   protected:
      // protected member functions
      DRMCSpefudFB();
      virtual ~DRMCSpefudFB();

      // protected const member functions

   private:
      // Constructors and destructor
      DRMCSpefudFB( const DRMCSpefudFB& );  // stop default

      // assignment operator(s)
      const DRMCSpefudFB& operator=( const DRMCSpefudFB& );  // stop default

      // private member functions

      // private const member functions

      // data members
      // m_spefud - hold collection of DBDRMCSpefud constants
      FAConstants<DBDRMCSpefud> m_spefud ;

      // static data members
      // m_instance - pointer to the instance of this singelton object
      static DRMCSpefudFB* m_instance;
};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "DRMCUtil/Template/DRMCSpefudFB.cc"
//#endif

#endif /* DRMCUTIL_DRMCSPEFUDFB_H */
