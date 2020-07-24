#if !defined(IRGEOM_BEAMPIPEGEOMETRY_H)
#define IRGEOM_BEAMPIPEGEOMETRY_H
// -*- C++ -*-
//
// Package:     IRGeom
// Module:      BeamPipeGeometry
// 
// Description: Function class for creation of CLEO3 Beam pipe
//
// Usage:
//    <usage>
//
// Author:      Aren Jansen
// Created:     Tue Jun 15 16:06:37 EDT 1999
// $Id: BeamPipeGeometry.h,v 1.1.1.1 1999/07/22 14:38:40 lkg Exp $
//
// Revision history
//
// $Log: BeamPipeGeometry.h,v $
// Revision 1.1.1.1  1999/07/22 14:38:40  lkg
// First release
//

// system include files

// user include files
#include "DetectorGeometry/DGVolumeParentPlaced.h"
#include "DataHandler/Record.h"

// forward declarations

class BeamPipeGeometry
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
  
      // ---------- Constructors and destructor ----------------
      BeamPipeGeometry(const Record& iRecord);
      virtual ~BeamPipeGeometry();

      // ---------- member functions ---------------------------
      void operator()(DGVolumeParentPlaced* pIRMother);

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      BeamPipeGeometry( const BeamPipeGeometry& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const BeamPipeGeometry& operator=( const BeamPipeGeometry& ); // stop default

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
//# include "IRGeom/Template/BeamPipeGeometry.cc"
//#endif

#endif /* IRGEOM_BEAMPIPEGEOMETRY_H */



