#if !defined(ZDMCUTIL_ZDMCFFFB_H)
#define ZDMCUTIL_ZDMCWIREFFFB_H
// -*- C++ -*-
//
// Package:     <ZDMCUtil>
// Module:      ZDMCWireffFB
// 
// Description: Frame Broker object for ZD MC Constants - MCWireff
//              for Fortran access to these constants.  
//              Based on Mike Marsh's K3FFrameBroker object.
//
// Usage:
//  This is a Singleton pattern (_Design_Patterns_, p.127).
//  The unique instance holds smart pointer to DBZDMCWireff m_wireff
//  which holds a FAConstants collection of DBZDMCWireff objects
//
//  Need to initialize object with constants extracted from the Frame
//  via setXXX( XXX ) function.
//
//  The function zdmc_wireff_ provides Fortran access to these constants
//  
//  The Fortran function TestZDMCConstantsProc/Fortran/test_wireff.F calls
//  zdmc_wireff and prints out the constants.
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
#include "DBZDMCWireff.hh"
#include "FrameAccess/FAConstants.h"

#include "AZDGeom/AZDSenseWireStore.h"

// forward declarations

class ZDMCWireffFB
{
      // friend classes and functions

   public:
      // constants, enums and typedefs 
      // Need to update # of entries in $C3_LIB/data/cleo2_90494.zdmcwireff
      // 10/21/99 Change to Cleo3 geometry - One constant per wire
      enum {kNumberOfEntries=AZDSenseWireStore::kNumberOfSenseWires};

   
      // Constructors and destructor

      // member functions

      // Initialize object with FAConstants collection of DBZDMCWireff objects
      void setWireff( const FAConstants< DBZDMCWireff > aWireff );

      // Return a DBZDMCWireff object for a given entry
      const DBZDMCWireff& getDBZDMCWireff(unsigned int entry) const;

      // Fill Arrays with constants
      void fillArrays( ZDMCWireffFB* pFrameBroker,
		       FReal* Effi,
		       const FInteger& arraySize );

      // static member functions
      static ZDMCWireffFB* instance();
      static void destroy();

   protected:
      // protected member functions
      ZDMCWireffFB();
      virtual ~ZDMCWireffFB();

      // protected const member functions

   private:
      // Constructors and destructor
      ZDMCWireffFB( const ZDMCWireffFB& );  // stop default

      // assignment operator(s)
      const ZDMCWireffFB& operator=( const ZDMCWireffFB& );  // stop default

      // private member functions

      // private const member functions

      // data members
      // m_wireff - save collection of DBZDMCWireff constants
      FAConstants<DBZDMCWireff> m_wireff ;

      // static data members
      // m_instance - pointer to the instance of this singelton object
      static ZDMCWireffFB* m_instance;
};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "ZDMCUtil/Template/ZDMCWireffFB.cc"
//#endif

#endif /* ZDMCUTIL_ZDMCWIREFFFB_H */
