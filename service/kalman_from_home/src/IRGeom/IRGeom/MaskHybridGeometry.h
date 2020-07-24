#if !defined(IRGEOM_MASKHYBRIDGEOMETRY_H)
#define IRGEOM_MASKHYBRIDGEOMETRY_H
// -*- C++ -*-
//
// Package:     IRGeom
// Module:      MaskHybridGeometry
// 
// Description: Function class for creation of Hybrid (Phase 2-3) Tungsten Mask
//
// Usage:
//    <usage>
//
// Author:      Aren Jansen
// Created:     Wed Jun 16 12:22:59 EDT 1999
// $Id: MaskHybridGeometry.h,v 1.1.1.1 1999/07/22 14:38:40 lkg Exp $
//
// Revision history
//
// $Log: MaskHybridGeometry.h,v $
// Revision 1.1.1.1  1999/07/22 14:38:40  lkg
// First release
//

// system include files

// user include files
#include "DetectorGeometry/DGVolumeParentPlaced.h"
#include "DataHandler/Record.h"

// forward declarations

class MaskHybridGeometry
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
     
      // ---------- Constructors and destructor ----------------
      MaskHybridGeometry(const Record& iRecord);
      virtual ~MaskHybridGeometry();

      // ---------- member functions ---------------------------
      void operator()(DGVolumeParentPlaced* pIRMother);
      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      MaskHybridGeometry( const MaskHybridGeometry& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const MaskHybridGeometry& operator=( const MaskHybridGeometry& ); // stop default

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
//# include "IRGeom/Template/MaskHybridGeometry.cc"
//#endif

#endif /* IRGEOM_MASKHYBRIDGEOMETRY_H */
