#if !defined(IRGEOM_BEAMPIPEGEOMETRY23_H)
#define IRGEOM_BEAMPIPEGEOMETRY23_H
// -*- C++ -*-
//
// Package:     IRGeom
// Module:      BeamPipeGeometry23
// 
// Description: Function class for creation of CLEO23 Beam pipe
//
// Usage:
//    <usage>
//
// Author:      Aren Jansen
// Created:     Tue Jun 22 11:49:27 EDT 1999
// $Id: BeamPipeGeometry23.h,v 1.1.1.1 1999/07/22 14:38:41 lkg Exp $
//
// Revision history
//
// $Log: BeamPipeGeometry23.h,v $
// Revision 1.1.1.1  1999/07/22 14:38:41  lkg
// First release
//

// system include files

// user include files
#include "DetectorGeometry/DGVolumeParentPlaced.h"
#include "DataHandler/Record.h"

// forward declarations

class BeamPipeGeometry23
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      BeamPipeGeometry23(const Record& iRecord);
      virtual ~BeamPipeGeometry23();

      // ---------- member functions ---------------------------
      void operator()(DGVolumeParentPlaced* pIRMother);

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      BeamPipeGeometry23( const BeamPipeGeometry23& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const BeamPipeGeometry23& operator=( const BeamPipeGeometry23& ); // stop default

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
//# include "IRGeom/Template/BeamPipeGeometry23.cc"
//#endif

#endif /* IRGEOM_BEAMPIPEGEOMETRY23_H */
