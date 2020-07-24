#if !defined(PACKAGE_DUMMYNTUPLE_H)
#define PACKAGE_DUMMYNTUPLE_H
// -*- C++ -*-
//
// Package:     <package>
// Module:      DummyNtuple
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Bill McCloskey
// Created:     Sat Mar  6 22:30:52 EST 1999
// $Id: DummyNtuple.h,v 1.1.1.1 1999/05/12 00:10:12 billm Exp $
//
// Revision history
//
// $Log: DummyNtuple.h,v $
// Revision 1.1.1.1  1999/05/12 00:10:12  billm
// imported DummyHistogram sources
//

// system include files

// user include files
#include "HistogramInterface/HINtuple.h"

// forward declarations

class DummyNtuple : public HINtuple
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      DummyNtuple(
		   unsigned int  iId,
		   const string& iTitle,
		   unsigned int  iNumberOfVariables,
		   unsigned int  iChunkSize,
		   const char* const* iLabels ) ;
      virtual ~DummyNtuple();

      // ---------- member functions ---------------------------
      virtual void fill( const float* iEntry ) ; 

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------
      virtual void book ( const string& iTitle,
			  const string& iTopDir,
			  unsigned int  iNumberOfVariables,
			  unsigned int  iChunkSize,
			  const char* const* iLabels ) ;
      virtual void close() ;

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      DummyNtuple( const DummyNtuple& ); // stop default
      DummyNtuple(); // default

      // ---------- assignment operator(s) ---------------------
      const DummyNtuple& operator=( const DummyNtuple& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "package/Template/DummyNtuple.cc"
//#endif

#endif /* PACKAGE_DUMMYNTUPLE_H */
