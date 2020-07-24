#if !defined(IRGEOM_STAINLESSTUBEGEOMETRY_H)
#define IRGEOM_STAINLESSTUBEGEOMETRY_H
// -*- C++ -*-
//
// Package:     IRGeom
// Module:      StainlessTubeGeometry
// 
// Description: Function class for creation of IRGeom Stainless Steel Tube
//
// Usage:
//    <usage>
//
// Author:      Aren Jansen
// Created:     Wed Jun 16 14:31:14 EDT 1999
// $Id: StainlessTubeGeometry.h,v 1.1.1.1 1999/07/22 14:38:41 lkg Exp $
//
// Revision history
//
// $Log: StainlessTubeGeometry.h,v $
// Revision 1.1.1.1  1999/07/22 14:38:41  lkg
// First release
//

// system include files

// user include files
#include "DetectorGeometry/DGVolumeParentPlaced.h"
#include "DataHandler/Record.h"

// forward declarations

class StainlessTubeGeometry
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      
      // ---------- Constructors and destructor ----------------
      StainlessTubeGeometry(const Record& iRecord);
      virtual ~StainlessTubeGeometry();

      // ---------- member functions ---------------------------
      void operator()(DGVolumeParentPlaced* pIRMother);
      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      StainlessTubeGeometry( const StainlessTubeGeometry& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const StainlessTubeGeometry& operator=( const StainlessTubeGeometry& ); // stop default

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
//# include "IRGeom/Template/StainlessTubeGeometry.cc"
//#endif

#endif /* IRGEOM_STAINLESSTUBEGEOMETRY_H */
