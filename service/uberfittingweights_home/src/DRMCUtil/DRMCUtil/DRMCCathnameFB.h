#if !defined(DRMCUTIL_DRMCCATHNAMEFB_H)
#define DRMCUTIL_DRMCCATHNAMEFB_H
// -*- C++ -*-
//
// Package:     <DRMCUtil>
// Module:      DRMCCathnameFB
// 
// Description: Frame Broker object for DR MC Constants - Cathname
//              for Fortran access to these constants.  
//              Based on Mike Marsh's K3FFrameBroker object.
//
// Usage:
//  This is a Singleton pattern (_Design_Patterns_, p.127).
//  The unique instance holds smart pointer to DBDRMCCathname m_cathodeName
//  which holds a FAConstants collection of DBDRMCWireff objects
//
//  Need to initialize object with constants extracted from the Frame
//  via setXXX( XXX ) function.
//
//  The function drmc_cathname_ provides Fortran access to these constants.
//  See this function to see how to use this object.
//
//  The Fortran function TestDRMCConstantsProc/Fortran/test_cathname.F calls
//  drmc_cathname_ and prints out the constants
//
//  The TestDRMCConstantsProc Suez processor initializes the DRMCxxxxFB
//  objects and then calls Fortran subroutines to test all of the constants
//
// Author:      John J O'Neill
// Created:     Wed Jun 16 22:11:30 EDT 1999
// $Id: DRMCCathnameFB.h,v 1.3 1999/10/10 14:33:16 cdj Exp $
//
// Revision history
//
// $Log: DRMCCathnameFB.h,v $
// Revision 1.3  1999/10/10 14:33:16  cdj
// removed BDLclient dirctory from include
//
// Revision 1.2  1999/08/10 19:45:45  jjo
// Add Fortran access for DR Monte Carlo constants
//
// Revision 1.1.1.1  1999/06/21 15:50:16  jjo
// Import files into CVS
//

// system include files

// user include files
#include "DBDRMCCathname.hh"
#include "FrameAccess/FAConstants.h"

// forward declarations

class DRMCCathnameFB
{
      // friend classes and functions

   public:
      // constants, enums and typedefs

      // Constructors and destructor

      // member functions
      // Pass a DBDRMCCathname to initialize this object
      // this constant only has 1 entry so try DBDRMCCathname object instead of FAConstants
      void setCathname( DBDRMCCathname aCathodeName );
      
      // const member functions
      // This returns the constant 'Cathname'
      const char* cathname() const;
      
      // static member functions
      static DRMCCathnameFB* instance();
      static void destroy();

   protected:
      // protected member functions
      DRMCCathnameFB();
      virtual ~DRMCCathnameFB();

      // protected const member functions

   private:
      // Constructors and destructor
      DRMCCathnameFB( const DRMCCathnameFB& );  // stop default

      // assignment operator(s)
      const DRMCCathnameFB& operator=( const DRMCCathnameFB& );  // stop default

      // private member functions

      // private const member functions

      // data members
      // FAConstants< DBDRMCCathname > m_cathodeName ; 
      // this constant only has 1 entry so use DBDRMCCathname object instead of FAConstants      
      DBDRMCCathname m_cathodeName ;
      
      // static data members
      static DRMCCathnameFB* m_instance;
};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "DRMCUtil/Template/DRMCCathnameFB.cc"
//#endif

#endif /* DRMCUTIL_DRMCCATHNAMEFB_H */
