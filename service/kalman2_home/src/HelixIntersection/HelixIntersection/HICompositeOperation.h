#if !defined(HELIXINTERSECTION_HICOMPOSITEOPERATION_H)
#define HELIXINTERSECTION_HICOMPOSITEOPERATION_H
// -*- C++ -*-
//
// Package:     HelixIntersection
// Module:      HICompositeOperation
// 
// Description: Composite of HIHelixOperation.
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Tue Sep 22 22:25:34 EDT 1998
// $Id: HICompositeOperation.h,v 1.5 2001/03/27 05:27:59 wsun Exp $
//
// Revision history
//
// $Log: HICompositeOperation.h,v $
// Revision 1.5  2001/03/27 05:27:59  wsun
// Added deleteAndRemoveLastChild() and flag for backward child execution.
//
// Revision 1.4  1999/08/27 20:11:57  wsun
// New signature for HIHelixOperation::perform().
//
// Revision 1.3  1999/04/28 22:56:55  wsun
// Fixes for Solaris.
//
// Revision 1.2  1999/04/27 22:03:53  wsun
// Major reorganization and structural changes.
//
// Revision 1.1.1.1  1998/12/07 04:58:16  wsun
// First submission.
//

// system include files
// user include files
#include "HelixIntersection/HIHelixOperation.h"

// forward declarations
#include "STLUtility/fwd_vector.h"

class HIHelix ;
class HIIntersectionSurface ;

class HICompositeOperation : public HIHelixOperation
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      HICompositeOperation();
      virtual ~HICompositeOperation();

      // ---------- member functions ---------------------------
      virtual void setPrintDiagnostics( DABoolean flag ) ;
      virtual void addChild( HIHelixOperation* anOperation ) ;
      virtual void removeLastChild() ;
      virtual void deleteAndRemoveLastChild() ;
      virtual void deleteChildren() ;

      void setExecuteForward() { m_executeForward = true ; }
      void setExecuteBackward() { m_executeForward = false ; }

      // ---------- const member functions ---------------------
      virtual const HIHelixOperation*  getChild( int iChild ) const ;

      virtual int numberOfChildren() const ;

      virtual DABoolean perform(
	 HIHelix& aHelixAfterOperation,
	 const HIHelix& aHelixBeforeOperation,
	 const HIHelix& aHelixAtLastIntersection,
	 HIIntersectionSurface& aCurrentSurface,
	 const HIIntersectionSurface& aLastSurface ) const ;

      virtual DABoolean isComposite() const { return true ; }

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HICompositeOperation( const HICompositeOperation& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HICompositeOperation& operator=( const HICompositeOperation& );
      // stop default

      // ---------- private member functions -------------------
      STL_VECTOR( HIHelixOperation* )* m_operations ;

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      DABoolean m_executeForward ;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HelixIntersection/Template/HICompositeOperation.cc"
//#endif

#endif /* HELIXINTERSECTION_HICOMPOSITEOPERATION_H */
