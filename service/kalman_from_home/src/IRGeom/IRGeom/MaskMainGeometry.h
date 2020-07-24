#if !defined(IRGEOM_MASKMAINGEOMETRY_H)
#define IRGEOM_MASKMAINGEOMETRY_H
// -*- C++ -*-
//
// Package:     IRGeom
// Module:      MaskMainGeometry
// 
// Description: Function class for creation of Main Tungsten Mask
//
// Usage:
//    <usage>
//
// Author:      Aren Jansen
// Created:     Wed Jun 16 12:22:49 EDT 1999
// $Id: MaskMainGeometry.h,v 1.1.1.1 1999/07/22 14:38:40 lkg Exp $
//
// Revision history
//
// $Log: MaskMainGeometry.h,v $
// Revision 1.1.1.1  1999/07/22 14:38:40  lkg
// First release
//

// system include files

// user include files
#include "DetectorGeometry/DGVolumeParentPlaced.h"
#include "DataHandler/Record.h"

// forward declarations

class MaskMainGeometry
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      MaskMainGeometry(const Record& iRecord);
      virtual ~MaskMainGeometry();

      // ---------- member functions ---------------------------
      void operator()(DGVolumeParentPlaced* pIRMother);
      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      MaskMainGeometry( const MaskMainGeometry& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const MaskMainGeometry& operator=( const MaskMainGeometry& ); // stop default

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
//# include "package/Template/MaskMainGeometry.cc"
//#endif

#endif /* PACKAGE_MASKMAINGEOMETRY_H */
