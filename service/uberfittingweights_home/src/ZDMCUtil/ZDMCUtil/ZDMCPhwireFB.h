#if !defined(ZDMCUTIL_ZDMCFFFB_H)
#define ZDMCUTIL_ZDMCPHWIREFB_H
// -*- C++ -*-
//
// Package:     <ZDMCUtil>
// Module:      ZDMCPhwireFB
// 
// Description: Frame Broker object for ZD MC Constants - MCPhwire
//              for Fortran access to these constants.  
//              Based on Mike Marsh's K3FFrameBroker object.
//
// Usage:
//  This is a Singleton pattern (_Design_Patterns_, p.127).
//  The unique instance holds smart pointer to DBZDMCPhwire m_phwire
//  which holds a FAConstants collection of DBZDMCPhwire objects
//
//  Need to initialize object with constants extracted from the Frame
//  via setXXX( XXX ) function.
//
//  The function zdmc_phwire_ provides Fortran access to these constants.
//  See this function to see how to use this object.
//
//  The Fortran function TestZDMCConstantsProc/Fortran/test_phwire.F calls
//  zdmc_phwire_ and prints out the constants
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
#include "DBZDMCPhwire.hh"
#include "FrameAccess/FAConstants.h"

#include "AZDGeom/AZDSenseWireStore.h"

// forward declarations

class ZDMCPhwireFB
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      // Need to update # of entries in $C3_LIB/data/cleo2_90494.zdmcphwire
      // 10/21/99 Changing to Cleo3 geometry - One constant per wire
      enum {kNumberOfEntries=AZDSenseWireStore::kNumberOfSenseWires};

      // Constructors and destructor

      // member functions

      // Initialize object with FAConstants collection of DBZDMCPhwire objects
      void setPhwire( const FAConstants< DBZDMCPhwire > aPhwire );

      // Return a DBZDMCPhwire object for a given entry
      const DBZDMCPhwire& getDBZDMCPhwire(unsigned int entry) const;
      
      // Fill Arrays with constants
      void fillArrays( ZDMCPhwireFB* pFrameBroker,
		       FReal* Wircor,
		       const FInteger& arraySize );

      // static member functions
      static ZDMCPhwireFB* instance();
      static void destroy();

   protected:
      // protected member functions
      ZDMCPhwireFB();
      virtual ~ZDMCPhwireFB();

      // protected const member functions

   private:
      // Constructors and destructor
      ZDMCPhwireFB( const ZDMCPhwireFB& );  // stop default

      // assignment operator(s)
      const ZDMCPhwireFB& operator=( const ZDMCPhwireFB& );  // stop default

      // private member functions

      // private const member functions

      // data members
      // m_phwire - save collection of DBZDMCPhwire constants
      FAConstants<DBZDMCPhwire> m_phwire ;

      // static data members
      // m_instance - pointer to the instance of this singelton object
      static ZDMCPhwireFB* m_instance;
};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "ZDMCUtil/Template/ZDMCPhwireFB.cc"
//#endif

#endif /* ZDMCUTIL_ZDMCPHWIREFB_H */
