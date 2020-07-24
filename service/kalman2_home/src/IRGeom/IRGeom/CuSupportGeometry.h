#if !defined(IRGEOM_CUSUPPORTGEOMETRY_H)
#define IRGEOM_CUSUPPORTGEOMETRY_H
// -*- C++ -*-
//
// Package:     IRGeom
// Module:      CuSupportGeometry
// 
// Description: Function class to create copper support pcones for the 
//              Silicon Detector
//
// Usage:
//    <usage>
//
// Author:      Aren Jansen
// Created:     Fri Jun 25 16:39:14 EDT 1999
// $Id: CuSupportGeometry.h,v 1.1.1.1 1999/07/22 14:38:41 lkg Exp $
//
// Revision history
//
// $Log: CuSupportGeometry.h,v $
// Revision 1.1.1.1  1999/07/22 14:38:41  lkg
// First release
//

// system include files

// user include files
#include "DetectorGeometry/DGVolumeParentPlaced.h"
#include "DataHandler/Record.h"

// forward declarations

class CuSupportGeometry
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      CuSupportGeometry(const Record& iRecord);
      virtual ~CuSupportGeometry();

      // ---------- member functions ---------------------------
      void operator()(DGVolumeParentPlaced* pIRMother);      
      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      CuSupportGeometry( const CuSupportGeometry& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const CuSupportGeometry& operator=( const CuSupportGeometry& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      const Record& m_Record;
      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "IRGeom/Template/CuSupportGeometry.cc"
//#endif

#endif /* IRGEOM_CUSUPPORTGEOMETRY_H */
