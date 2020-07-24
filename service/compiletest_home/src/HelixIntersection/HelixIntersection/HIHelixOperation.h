#if !defined(HELIXINTERSECTION_HIHELIXOPERATION_H)
#define HELIXINTERSECTION_HIHELIXOPERATION_H
// -*- C++ -*-
//
// Package:     HelixIntersection
// Module:      HIHelixOperation
// 
// Description: Base class for the operation to be applied to a helix by
//              HIHelixIntersector after it determines that the helix
//              intersects the associated HIIntersectionSurface.
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Tue Sep 22 19:05:59 EDT 1998
// $Id: HIHelixOperation.h,v 1.3 1999/08/27 20:11:59 wsun Exp $
//
// Revision history
//
// $Log: HIHelixOperation.h,v $
// Revision 1.3  1999/08/27 20:11:59  wsun
// New signature for HIHelixOperation::perform().
//
// Revision 1.2  1999/04/27 22:04:01  wsun
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

class HIHelixOperation
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      typedef double Meters ;
      typedef double GeV ;

      // ---------- Constructors and destructor ----------------
      HIHelixOperation();
      virtual ~HIHelixOperation();

      // ---------- member functions ---------------------------

      virtual void setPrintDiagnostics( DABoolean flag )
      { m_printDiagnostics = flag ; }

      // ---------- const member functions ---------------------
      virtual DABoolean perform(
	 HIHelix& aHelixAfterOperation,
	 const HIHelix& aHelixBeforeOperation,
	 const HIHelix& aHelixAtLastIntersection,
	 HIIntersectionSurface& aCurrentSurface,
	 const HIIntersectionSurface& aLastSurface ) const
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
      HIHelixOperation( const HIHelixOperation& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HIHelixOperation& operator=( const HIHelixOperation& );
      // stop default

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
//# include "HelixIntersection/Template/HIHelixOperation.cc"
//#endif

#endif /* HELIXINTERSECTION_HIHELIXOPERATION_H */
