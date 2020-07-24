#if !defined(DUMMYHISTOGRAM_DUMMYNUMBERWITHERRORS_H)
#define DUMMYHISTOGRAM_DUMMYNUMBERWITHERRORS_H
// -*- C++ -*-
//
// Package:     <DummyHistogram>
// Module:      DummyNumberWithErrors
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Martin Lohner
// Created:     Thu Sep  2 16:53:35 EDT 1999
// $Id: DummyNumberWithErrors.h,v 1.3 1999/10/07 18:53:58 hufnagel Exp $
//
// Revision history
//
// $Log: DummyNumberWithErrors.h,v $
// Revision 1.3  1999/10/07 18:53:58  hufnagel
// added name method for Numbers
//
// Revision 1.2  1999/10/07 18:06:34  hufnagel
// Numbers now use float instead of double
//
// Revision 1.1  1999/09/06 21:47:29  mkl
// move to new histogramming interface: dummy implementation
//

// system include files
#include <string>

// user include files
#include "HistogramInterface/HINumberWithErrors.h"

// forward declarations

class DummyNumberWithErrors : public HINumberWithErrors
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      DummyNumberWithErrors( const string& iName,
			     float         iValue,
			     float         iPositiveError,
			     float         iNegativeError );
      virtual ~DummyNumberWithErrors();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      DummyNumberWithErrors( const DummyNumberWithErrors& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const DummyNumberWithErrors& operator=( const DummyNumberWithErrors& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "DummyHistogram/Template/DummyNumberWithErrors.cc"
//#endif

#endif /* DUMMYHISTOGRAM_DUMMYNUMBERWITHERRORS_H */
