#if !defined(IRGEOM_MASKINNERGEOMETRY_H)
#define IRGEOM_MASKINNERGEOMETRY_H
// -*- C++ -*-
//
// Package:     IRGeom
// Module:      MaskInnerGeometry
// 
// Description: Function class for creation of IRGeom Inner Tungsten Mask
//
// Usage:
//    <usage>
//
// Author:      Aren Jansen
// Created:     Wed Jun 16 12:22:36 EDT 1999
// $Id: MaskInnerGeometry.h,v 1.1.1.1 1999/07/22 14:38:40 lkg Exp $
//
// Revision history
//
// $Log: MaskInnerGeometry.h,v $
// Revision 1.1.1.1  1999/07/22 14:38:40  lkg
// First release
//

// system include files

// user include files
#include "DetectorGeometry/DGVolumeParentPlaced.h"
#include "DataHandler/Record.h"

// forward declarations

class MaskInnerGeometry
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      MaskInnerGeometry(const Record& iRecord);
      virtual ~MaskInnerGeometry();

      // ---------- member functions ---------------------------
      void operator()(DGVolumeParentPlaced* pIRMother);

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      MaskInnerGeometry( const MaskInnerGeometry& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const MaskInnerGeometry& operator=( const MaskInnerGeometry& ); // stop default

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
//# include "IRGeom/Template/MaskInnerGeometry.cc"
//#endif

#endif /* IRGEOM_MASKINNERGEOMETRY_H */




