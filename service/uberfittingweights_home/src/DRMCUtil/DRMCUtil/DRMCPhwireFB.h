#if !defined(DRMCUTIL_DRMCFFFB_H)
#define DRMCUTIL_DRMCPHWIREFB_H
// -*- C++ -*-
//
// Package:     <DRMCUtil>
// Module:      DRMCPhwireFB
// 
// Description: Frame Broker object for DR MC Constants - MCPhwire
//              for Fortran access to these constants.  
//              Based on Mike Marsh's K3FFrameBroker object.
//
// Usage:
//  This is a Singleton pattern (_Design_Patterns_, p.127).
//  The unique instance holds smart pointer to DBDRMCPhwire m_phwire
//  which holds a FAConstants collection of DBDRMCPhwire objects
//
//  Need to initialize object with constants extracted from the Frame
//  via setXXX( XXX ) function.
//
//  The function drmc_phwire_ provides Fortran access to these constants.
//  See this function to see how to use this object.
//
//  The Fortran function TestDRMCConstantsProc/Fortran/test_phwire.F calls
//  drmc_phwire_ and prints out the constants
//
//  The TestDRMCConstantsProc Suez processor initializes the DRMCxxxxFB
//  objects and then calls Fortran subroutines to test all of the constants
//
// Author:      John J O'Neill
// Created:     Wed Jun 30 23:20:59 EDT 1999
// $Id: DRMCPhwireFB.h,v 1.4 1999/10/26 19:03:43 jjo Exp $
//
// Revision history
//
// $Log: DRMCPhwireFB.h,v $
// Revision 1.4  1999/10/26 19:03:43  jjo
// Update to Cleo3 geometry.  Uses ADRGeom instead of hardwired numbers
//
// Revision 1.3  1999/10/10 14:33:19  cdj
// removed BDLclient dirctory from include
//
// Revision 1.2  1999/09/13 14:22:01  jjo
// Add fortran_types.h include which is now needed
//
// Revision 1.1  1999/08/10 19:45:48  jjo
// Add Fortran access for DR Monte Carlo constants
//
//

// system include files

// user include files
#include "Cleo/fortran_types.h"
#include "DBDRMCPhwire.hh"
#include "FrameAccess/FAConstants.h"

#include "ADRGeom/ADRSenseWireStore.h"

// forward declarations

class DRMCPhwireFB
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      // Need to update # of entries in $C3_LIB/data/cleo2_90494.drmcphwire
      // 10/21/99 Changing to Cleo3 geometry - One constant per wire
      enum {kNumberOfEntries=ADRSenseWireStore::kNumberOfWiresTotal};

      // Constructors and destructor

      // member functions

      // Initialize object with FAConstants collection of DBDRMCPhwire objects
      void setPhwire( const FAConstants< DBDRMCPhwire > aPhwire );

      // Return a DBDRMCPhwire object for a given entry
      const DBDRMCPhwire& getDBDRMCPhwire(unsigned int entry) const;
      
      // Fill Arrays with constants
      void fillArrays( DRMCPhwireFB* pFrameBroker,
		       FReal* Wircor,
		       const FInteger& arraySize );

      // static member functions
      static DRMCPhwireFB* instance();
      static void destroy();

   protected:
      // protected member functions
      DRMCPhwireFB();
      virtual ~DRMCPhwireFB();

      // protected const member functions

   private:
      // Constructors and destructor
      DRMCPhwireFB( const DRMCPhwireFB& );  // stop default

      // assignment operator(s)
      const DRMCPhwireFB& operator=( const DRMCPhwireFB& );  // stop default

      // private member functions

      // private const member functions

      // data members
      // m_phwire - save collection of DBDRMCPhwire constants
      FAConstants<DBDRMCPhwire> m_phwire ;

      // static data members
      // m_instance - pointer to the instance of this singelton object
      static DRMCPhwireFB* m_instance;
};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "DRMCUtil/Template/DRMCPhwireFB.cc"
//#endif

#endif /* DRMCUTIL_DRMCPHWIREFB_H */
