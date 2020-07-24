#if !defined(HELIXINTERSECTION_HICOMPOSITECONDITION_H)
#define HELIXINTERSECTION_HICOMPOSITECONDITION_H
// -*- C++ -*-
//
// Package:     HelixIntersection
// Module:      HICompositeCondition
// 
// Description: Composite of HICondition.
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Tue Sep 22 22:25:57 EDT 1998
// $Id: HICompositeCondition.h,v 1.4 2001/03/27 05:27:23 wsun Exp $
//
// Revision history
//
// $Log: HICompositeCondition.h,v $
// Revision 1.4  2001/03/27 05:27:23  wsun
// Added deleteAndRemoveLastChild().
//
// Revision 1.3  1999/04/28 22:56:54  wsun
// Fixes for Solaris.
//
// Revision 1.2  1999/04/27 22:03:51  wsun
// Major reorganization and structural changes.
//
// Revision 1.1.1.1  1998/12/07 04:58:16  wsun
// First submission.
//

// system include files
// user include files
#include "HelixIntersection/HICondition.h"

// forward declarations
#include "STLUtility/fwd_vector.h"

class HIHelix ;
class HIIntersectionSurface ;

class HICompositeCondition : public HICondition
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      HICompositeCondition();
      virtual ~HICompositeCondition();

      // ---------- member functions ---------------------------
      virtual void setPrintDiagnostics( DABoolean flag ) ;
      virtual void addChild( HICondition* aCondition ) ;
      virtual void removeLastChild() ;
      virtual void deleteAndRemoveLastChild() ;
      virtual void deleteChildren() ;

      // ---------- const member functions ---------------------
      virtual const HICondition*  getChild( int iChild ) const ;

      virtual int numberOfChildren() const ;

      virtual DABoolean satisfied( const HIHelix& aHelix,
				   HIIntersectionSurface& ) const ;

      virtual DABoolean isComposite() const { return true ; }

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HICompositeCondition( const HICompositeCondition& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HICompositeCondition& operator=( const HICompositeCondition& );
      // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // Make the data member a pointer, which is assigned to a "new"ed
      // STL_VECTOR at each instantiation
      STL_VECTOR( HICondition* )* m_conditions ;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HelixIntersection/Template/HICompositeCondition.cc"
//#endif

#endif /* HELIXINTERSECTION_HICOMPOSITECONDITION_H */
