#if !defined(DRMCUTIL_DRMCFFFB_H)
#define DRMCUTIL_DRMCIONSFB_H
// -*- C++ -*-
//
// Package:     <DRMCUtil>
// Module:      DRMCIonsFB
// 
// Description: Frame Broker object for DR MC Constants - MCIons
//              for Fortran access to these constants.  
//              Based on Mike Marsh's K3FFrameBroker object.
//
// Usage:
//  This is a Singleton pattern (_Design_Patterns_, p.127).
//  The unique instance holds smart pointer to DBDRMCIons m_ions
//  which holds a FAConstants collection of DBDRMCIons objects
//
//  Need to initialize object with constants extracted from the Frame
//  via setXXX( XXX ) function.
//
//  The function drmc_ions_ provides Fortran access to these constants.
//  See this function to see how to use this object.
//
//  The Fortran function TestDRMCConstantsProc/Fortran/test_ions.F calls
//  drmc_ions_ and prints out the constants
//
//  The TestDRMCConstantsProc Suez processor initializes the DRMCxxxxFB
//  objects and then calls Fortran subroutines to test all of the constants
//
// Author:      John J O'Neill
// Created:     Wed Jun 30 23:20:59 EDT 1999
// $Id: DRMCIonsFB.h,v 1.4 1999/10/26 19:03:38 jjo Exp $
//
// Revision history
//
// $Log: DRMCIonsFB.h,v $
// Revision 1.4  1999/10/26 19:03:38  jjo
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
#include "DBDRMCIons.hh"
#include "FrameAccess/FAConstants.h"

#include "ADRGeom/ADRSenseWireStore.h"


// forward declarations

class DRMCIonsFB
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      // Need to update # of entries in $C3_LIB/data/cleo2_90494.drmcions
      // 10/21/99 Changing to Cleo3 geometry - One constant per layer
      enum {kNumberOfEntries=ADRSenseWireStore::kNumberOfLayers};

      // Constructors and destructor

      // member functions

      // Initialize object with FAConstants collection of DBDRMCIons objects
      void setIons( const  FAConstants< DBDRMCIons > aIons );

      // Return a DBDRMCIons object for a given entry
      // TODO : Make this const correct !
      const DBDRMCIons& getDBDRMCIons(unsigned int entry) const;
      
      // Fill Arrays with constants
      void fillArrays( DRMCIonsFB* pFrameBroker,
		       FInteger* N,
		       const FInteger& arraySize );

      // static member functions
      static DRMCIonsFB* instance();
      static void destroy();

   protected:
      // protected member functions
      DRMCIonsFB();
      virtual ~DRMCIonsFB();

      // protected const member functions

   private:
      // Constructors and destructor
      DRMCIonsFB( const DRMCIonsFB& );  // stop default

      // assignment operator(s)
      const DRMCIonsFB& operator=( const DRMCIonsFB& );  // stop default

      // private member functions

      // private const member functions

      // data members
      // m_ions - save collection of DBDRMCIons constants
      FAConstants<DBDRMCIons> m_ions ;

      // static data members
      // m_instance - pointer to the instance of this singelton object
      static DRMCIonsFB* m_instance;
};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "DRMCUtil/Template/DRMCIonsFB.cc"
//#endif

#endif /* DRMCUTIL_DRMCIONSFB_H */
