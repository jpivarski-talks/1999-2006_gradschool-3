#if !defined(HISTOGRAMINTERFACE_TBNTUPLEVARNAMES_H)
#define HISTOGRAMINTERFACE_TBNTUPLEVARNAMES_H
// -*- C++ -*-
//
// Package:     HistogramInterface
// Module:      HINtupleVarNames
// 
// Description: Provides a safe way to construct names for NTuple variables
//
// Usage:
//    When the user is going to initialize an NTuple they can use this
//     class to define the names of the various variables.
//
//    We assume that they have created an enum with the different names and
//     the last entry in the enum gives the number of variables
//
//       //NTuple variables
//       enum { kX, kY, kZ, kNumVars };
//
//    Then when they are going to create the ntuple they us HINtupleVarNames
// 
//       //Set up variable names
//       HINtupleVarNames varNames( kNumVars );
//
//       varNames.addVar( kX, "x" );
//       varNames.addVar( kY, "y" );
//       varNames.addVar( kZ, "z" );
//
//       //Create the ntuple
//       m_ntuple = aHistManager.ntuple( 100, "Title", kNumVars,
//                                       50000, varNames.names() );
//
//     The class does the following consistency checks
//     1) the number of variables added equals the number of
//        variables specified in the constructor call
//     2) each variable has a unique index and name
//
//     If any of these consistency checks fail an error message will be
//      printed and the program will be terminated.
//
//
// Author:      Chris D. Jones
// Created:     Fri Jul 24 12:03:44 EDT 1998
// $Id: HINtupleVarNames.h,v 1.1 1999/02/24 19:15:54 cdj Exp $
//
// Revision history
//
// $Log: HINtupleVarNames.h,v $
// Revision 1.1  1999/02/24 19:15:54  cdj
// first submission
//
// Revision 1.2  1998/10/22 17:38:52  cdj
// name() now returns a const char* const *
//
// Revision 1.1  1998/07/24 18:32:06  cdj
// First submission
//

// system include files

// user include files

// forward declarations

class HINtupleVarNames
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      HINtupleVarNames( unsigned int iNumberOfVariables );
      ~HINtupleVarNames();

      // ---------- member functions ---------------------------
      void addVar( unsigned int iIndex, const char* iName );

      // ---------- const member functions ---------------------
      const char* const* names() const;
      unsigned int numberOfVariables() const { return m_nVars; }

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HINtupleVarNames();
      HINtupleVarNames( const HINtupleVarNames& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HINtupleVarNames& operator=( const HINtupleVarNames& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      unsigned int m_nVars;
      char** m_varNames;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HistogramInterface/Template/HINtupleVarNames.cc"
//#endif

#endif /* HISTOGRAMINTERFACE_TBNTUPLEVARNAMES_H */
