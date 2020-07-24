#if !defined(IRGEOM_REQMAGNETGEOMETRY_H)
#define IRGEOM_REQMAGNETGEOMETRY_H
// -*- C++ -*-
//
// Package:     IRGeom
// Module:      REQMagnetGeometry
// 
// Description: Function class for creation of IRGeom REQ Magnet
//
// Usage:
//    <usage>
//
// Author:      Aren Jansen
// Created:     Wed Jun 16 14:30:26 EDT 1999
// $Id: REQMagnetGeometry.h,v 1.1.1.1 1999/07/22 14:38:41 lkg Exp $
//
// Revision history
//
// $Log: REQMagnetGeometry.h,v $
// Revision 1.1.1.1  1999/07/22 14:38:41  lkg
// First release
//

// system include files

// user include files
#include "DetectorGeometry/DGVolumeParentPlaced.h"
#include "DataHandler/Record.h"

// forward declarations

class REQMagnetGeometry
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      REQMagnetGeometry(const Record& iRecord);
      virtual ~REQMagnetGeometry();

      // ---------- member functions ---------------------------
      void operator()(DGVolumeParentPlaced* pIRMother);
      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      REQMagnetGeometry( const REQMagnetGeometry& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const REQMagnetGeometry& operator=( const REQMagnetGeometry& ); // stop default

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
//# include "IRGeom/Template/REQMagnetGeometry.cc"
//#endif

#endif /* IRGEOM_REQMAGNETGEOMETRY_H */
