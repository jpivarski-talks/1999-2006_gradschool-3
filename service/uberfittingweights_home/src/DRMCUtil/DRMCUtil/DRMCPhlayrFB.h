#if !defined(DRMCUTIL_DRMCFFFB_H)
#define DRMCUTIL_DRMCPHLAYRFB_H
// -*- C++ -*-
//
// Package:     <DRMCUtil>
// Module:      DRMCPhlayrFB
// 
// Description: Frame Broker object for DR MC Constants - MCPhlayr
//              for Fortran access to these constants.  
//              Based on Mike Marsh's K3FFrameBroker object.
//
// Usage:
//  This is a Singleton pattern (_Design_Patterns_, p.127).
//  The unique instance holds smart pointer to DBDRMCPhlayr m_phlayr
//  which holds a FAConstants collection of DBDRMCPhlayr objects
//
//  Need to initialize object with constants extracted from the Frame
//  via setXXX( XXX ) function.
//
//  The function drmc_phlayr_ provides Fortran access to these constants.
//  See this function to see how to use this object.
//
//  The Fortran function TestDRMCConstantsProc/Fortran/test_phlayr.F calls
//  drmc_phlayr_ and prints out the constants
//
//  The TestDRMCConstantsProc Suez processor initializes the DRMCxxxxFB
//  objects and then calls Fortran subroutines to test all of the constants
//
// Author:      John J O'Neill
// Created:     Wed Jun 30 23:20:59 EDT 1999
// $Id: DRMCPhlayrFB.h,v 1.4 1999/10/26 19:03:42 jjo Exp $
//
// Revision history
//
// $Log: DRMCPhlayrFB.h,v $
// Revision 1.4  1999/10/26 19:03:42  jjo
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
#include "DBDRMCPhlayr.hh"
#include "FrameAccess/FAConstants.h"

#include "ADRGeom/ADRSenseWireStore.h"


// forward declarations

class DRMCPhlayrFB
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      // Need to update # of entries in $C3_LIB/data/cleo2_90494.drmcphlayr
      // 10/21/99 Changing to Cleo3 geometry - kNPhLayer_PerLayer constant per layer
      enum {kNPhLayer_PerLayer=20};
      enum {kNumberOfEntries=kNPhLayer_PerLayer * ADRSenseWireStore::kNumberOfLayers};

      // Constructors and destructor

      // member functions

      // Initialize object with FAConstants collection of DBDRMCPhlayr objects
      void setPhlayr( const FAConstants< DBDRMCPhlayr > aPhlayr );

      // Return a DBDRMCPhlayr object for a given entry
      // TODO : Make this const correct !
      const DBDRMCPhlayr& getDBDRMCPhlayr(unsigned int entry) const;
      
      // Fill Arrays with constants
      void fillArrays( DRMCPhlayrFB* pFrameBroker,
		       FReal* ,
		       const FInteger& arraySize );

      // static member functions
      static DRMCPhlayrFB* instance();
      static void destroy();

   protected:
      // protected member functions
      DRMCPhlayrFB();
      virtual ~DRMCPhlayrFB();

      // protected const member functions

   private:
      // Constructors and destructor
      DRMCPhlayrFB( const DRMCPhlayrFB& );  // stop default

      // assignment operator(s)
      const DRMCPhlayrFB& operator=( const DRMCPhlayrFB& );  // stop default

      // private member functions

      // private const member functions

      // data members
      // m_phlayr - save collection of DBDRMCPhlayr constants
      FAConstants<DBDRMCPhlayr> m_phlayr ;

      // static data members
      // m_instance - pointer to the instance of this singelton object
      static DRMCPhlayrFB* m_instance;
};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "DRMCUtil/Template/DRMCPhlayrFB.cc"
//#endif

#endif /* DRMCUTIL_DRMCPHLAYRFB_H */
