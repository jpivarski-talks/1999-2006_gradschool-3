#if !defined(HELIXINTERSECTION_HIZCONDITION_H)
#define HELIXINTERSECTION_HIZCONDITION_H
// -*- C++ -*-
//
// Package:     HelixIntersection
// Module:      HIZBoundaryCondition
// 
// Description: HICondition subclass for specifying z boundary conditions.
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Fri Sep 25 15:16:57 EDT 1998
// $Id: HIZBoundaryCondition.h,v 1.2 1999/04/27 22:04:13 wsun Exp $
//
// Revision history
//
// $Log: HIZBoundaryCondition.h,v $
// Revision 1.2  1999/04/27 22:04:13  wsun
// Major reorganization and structural changes.
//
// Revision 1.1.1.1  1998/12/07 04:58:21  wsun
// First submission.
//

// system include files

// user include files
#include "HelixIntersection/HICondition.h"

// forward declarations
class HIHelix ;

class HIZBoundaryCondition : public HICondition
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      HIZBoundaryCondition();
      HIZBoundaryCondition( Meters aZmin, Meters aZmax ) ;
      virtual ~HIZBoundaryCondition();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------
      Meters zmin() const { return m_zmin ; }
      Meters zmax() const { return m_zmax ; }

      // Returns true if the helix lies between the z boundaries.
      virtual DABoolean satisfied( const HIHelix& aHelix,
				   HIIntersectionSurface& ) const ;

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HIZBoundaryCondition( const HIZBoundaryCondition& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HIZBoundaryCondition& operator=( const HIZBoundaryCondition& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      Meters m_zmin ;
      Meters m_zmax ;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HelixIntersection/Template/HIZBoundaryCondition.cc"
//#endif

#endif /* HELIXINTERSECTION_HIZCONDITION_H */
