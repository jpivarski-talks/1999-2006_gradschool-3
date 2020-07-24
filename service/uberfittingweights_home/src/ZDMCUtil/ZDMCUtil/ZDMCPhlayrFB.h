#if !defined(ZDMCUTIL_ZDMCFFFB_H)
#define ZDMCUTIL_ZDMCPHLAYRFB_H
// -*- C++ -*-
//
// Package:     <ZDMCUtil>
// Module:      ZDMCPhlayrFB
// 
// Description: Frame Broker object for ZD MC Constants - MCPhlayr
//              for Fortran access to these constants.  
//              Based on Mike Marsh's K3FFrameBroker object.
//
// Usage:
//  This is a Singleton pattern (_Design_Patterns_, p.127).
//  The unique instance holds smart pointer to DBZDMCPhlayr m_phlayr
//  which holds a FAConstants collection of DBZDMCPhlayr objects
//
//  Need to initialize object with constants extracted from the Frame
//  via setXXX( XXX ) function.
//
//  The function zdmc_phlayr_ provides Fortran access to these constants.
//  See this function to see how to use this object.
//
//  The Fortran function TestZDMCConstantsProc/Fortran/test_phlayr.F calls
//  zdmc_phlayr_ and prints out the constants
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
#include "DBZDMCPhlayr.hh"
#include "FrameAccess/FAConstants.h"

#include "AZDGeom/AZDSenseWireStore.h"


// forward declarations

class ZDMCPhlayrFB
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      // Need to update # of entries in $C3_LIB/data/cleo2_90494.zdmcphlayr
      // 10/21/99 Changing to Cleo3 geometry - kNPhLayer_PerLayer constant per layer
      enum {kNPhLayer_PerLayer=20};
      enum {kNumberOfEntries=kNPhLayer_PerLayer * AZDSenseWireStore::kNumberOfSenseLayers};

      // Constructors and destructor

      // member functions

      // Initialize object with FAConstants collection of DBZDMCPhlayr objects
      void setPhlayr( const FAConstants< DBZDMCPhlayr > aPhlayr );

      // Return a DBZDMCPhlayr object for a given entry
      // TODO : Make this const correct !
      const DBZDMCPhlayr& getDBZDMCPhlayr(unsigned int entry) const;
      
      // Fill Arrays with constants
      void fillArrays( ZDMCPhlayrFB* pFrameBroker,
		       FReal* ,
		       const FInteger& arraySize );

      // static member functions
      static ZDMCPhlayrFB* instance();
      static void destroy();

   protected:
      // protected member functions
      ZDMCPhlayrFB();
      virtual ~ZDMCPhlayrFB();

      // protected const member functions

   private:
      // Constructors and destructor
      ZDMCPhlayrFB( const ZDMCPhlayrFB& );  // stop default

      // assignment operator(s)
      const ZDMCPhlayrFB& operator=( const ZDMCPhlayrFB& );  // stop default

      // private member functions

      // private const member functions

      // data members
      // m_phlayr - save collection of DBZDMCPhlayr constants
      FAConstants<DBZDMCPhlayr> m_phlayr ;

      // static data members
      // m_instance - pointer to the instance of this singelton object
      static ZDMCPhlayrFB* m_instance;
};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "ZDMCUtil/Template/ZDMCPhlayrFB.cc"
//#endif

#endif /* ZDMCUTIL_ZDMCPHLAYRFB_H */
