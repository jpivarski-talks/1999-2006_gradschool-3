#if !defined(DUMMYHISTOGRAM_DUMMYNUMBER_H)
#define DUMMYHISTOGRAM_DUMMYNUMBER_H
// -*- C++ -*-
//
// Package:     <DummyHistogram>
// Module:      DummyNumber
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Martin Lohner
// Created:     Thu Sep  2 16:53:30 EDT 1999
// $Id: DummyNumber.h,v 1.3 1999/10/07 18:53:57 hufnagel Exp $
//
// Revision history
//
// $Log: DummyNumber.h,v $
// Revision 1.3  1999/10/07 18:53:57  hufnagel
// added name method for Numbers
//
// Revision 1.2  1999/10/07 18:06:33  hufnagel
// Numbers now use float instead of double
//
// Revision 1.1  1999/09/06 21:47:29  mkl
// move to new histogramming interface: dummy implementation
//

// system include files

// user include files
#include "HistogramInterface/HINumber.h"

// forward declarations

class DummyNumber : public HINumber
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      DummyNumber( const string& iName,
		   float         iValue=0 );
      virtual ~DummyNumber();

      // ---------- member functions ---------------------------
      
      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      DummyNumber( const DummyNumber& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const DummyNumber& operator=( const DummyNumber& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "DummyHistogram/Template/DummyNumber.cc"
//#endif

#endif /* DUMMYHISTOGRAM_DUMMYNUMBER_H */
