#if !defined(HELIXINTERSECTION_HICONDITION_H)
#define HELIXINTERSECTION_HICONDITION_H
// -*- C++ -*-
//
// Package:     HelixIntersection
// Module:      HICondition
// 
// Description: Base class for the condition used by HIHelixIntersector to
//              determine whether a given HIHelix intersects the associated
//              HIIntersectionSurface.
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Tue Sep 22 19:04:57 EDT 1998
// $Id: HICondition.h,v 1.2 1999/04/27 22:03:55 wsun Exp $
//
// Revision history
//
// $Log: HICondition.h,v $
// Revision 1.2  1999/04/27 22:03:55  wsun
// Major reorganization and structural changes.
//
// Revision 1.1.1.1  1998/12/07 04:58:16  wsun
// First submission.
//

// system include files

// user include files

// forward declarations
class HIHelix ;
class HIIntersectionSurface ;

class HICondition
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      typedef double Meters ;
      typedef double GeV ;

      // ---------- Constructors and destructor ----------------
      HICondition();
      virtual ~HICondition();

      // ---------- member functions ---------------------------

      virtual void setPrintDiagnostics( DABoolean flag )
      { m_printDiagnostics = flag ; }

      // ---------- const member functions ---------------------
      virtual DABoolean satisfied( const HIHelix& aHelix,
				   HIIntersectionSurface& ) const
      { return true ; }

      DABoolean printDiagnostics() const
      { return m_printDiagnostics ; }

      virtual DABoolean isComposite() const { return false ; }

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HICondition( const HICondition& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HICondition& operator=( const HICondition& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      DABoolean m_printDiagnostics ;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HelixIntersection/Template/HICondition.cc"
//#endif

#endif /* HELIXINTERSECTION_HICONDITION_H */
