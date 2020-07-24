// -*- C++ -*-
//
// Package:     HelixIntersection
// Module:      HICompositeOperation
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Tue Sep 22 22:26:28 EDT 1998
// $Id: HICompositeOperation.cc,v 1.11 2002/01/23 22:17:38 llh14 Exp $
//
// Revision history
//
// $Log: HICompositeOperation.cc,v $
// Revision 1.11  2002/01/23 22:17:38  llh14
// Added HICorrectForBFieldOperation, and small modifications to HISingleWireCylinder to incorporate the quad fields
//
// Revision 1.10  2001/03/26 06:47:34  wsun
// Added deleteAndRemoveLastChild() and mods for backwards execution of children.
//
// Revision 1.9  1999/11/21 22:31:35  wsun
// Sped up calculations.
//
// Revision 1.8  1999/08/27 20:14:47  wsun
// New signature for HIHelixOperation::perform().
//
// Revision 1.7  1999/06/24 22:24:53  wsun
// Added use of HIMagField and new SiHits.
//
// Revision 1.6  1999/06/18 19:25:53  wsun
// More improvements to low momentum fitting.
//
// Revision 1.5  1999/06/02 23:05:19  wsun
// Added features for low-momentum fitting.
//
// Revision 1.4  1999/04/29 20:15:35  wsun
// Additional fixes for Solaris.
//
// Revision 1.3  1999/04/28 22:57:16  wsun
// Fixes for Solaris.
//
// Revision 1.2  1999/04/27 22:06:20  wsun
// Major reorganization and structural changes.
//
// Revision 1.1.1.1  1998/12/07 04:58:24  wsun
// First submission.
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "HelixIntersection/HIHelix.h"
#include "HelixIntersection/HIIntersectionSurface.h"
#include "HelixIntersection/HICompositeOperation.h"

// STL classes
#include <vector>

//
// constants, enums and typedefs
//

static const char* const kFacilityString =
"HelixIntersection.HICompositeOperation" ;

//
// static data member definitions
//

//
// constructors and destructor
//
HICompositeOperation::HICompositeOperation()
   : HIHelixOperation(),
     m_operations( new STL_VECTOR( HIHelixOperation* ) ),
     m_executeForward( true )
{
}

// HICompositeOperation::HICompositeOperation( const HICompositeOperation& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HICompositeOperation::~HICompositeOperation()
{
   deleteChildren() ;
   delete m_operations ;
}

//
// assignment operators
//
// const HICompositeOperation& HICompositeOperation::operator=( const HICompositeOperation& rhs )
// {
//   if( this != &rhs ) {
//      // do actual copying here, plus:
//      // "SuperClass"::operator=( rhs );
//   }
//
//   return *this;
// }

//
// member functions
//

void
HICompositeOperation::addChild( HIHelixOperation* anOperation )
{
   m_operations->push_back( anOperation ) ;
}

void
HICompositeOperation::removeLastChild()
{
   if( m_operations->size() > 0 )
   {
      m_operations->pop_back() ;
   }
}

void
HICompositeOperation::deleteAndRemoveLastChild()
{
   if( m_operations->size() > 0 )
   {
      delete *( m_operations->end() - 1 ) ;
      m_operations->pop_back() ;
   }
}

void
HICompositeOperation::deleteChildren()
{
   for( STL_VECTOR( HIHelixOperation* )::iterator iter =
	   m_operations->begin() ;
	iter != m_operations->end() ;
	++iter )
   {
      delete *iter ;
   }

   m_operations->clear() ;
}


void
HICompositeOperation::setPrintDiagnostics( DABoolean flag )
{
   for( STL_VECTOR( HIHelixOperation* )::iterator iter =
	   m_operations->begin() ;
	iter != m_operations->end() ;
	++iter )
   {
      ( *iter )->setPrintDiagnostics( flag ) ;
   }
   HIHelixOperation::setPrintDiagnostics( flag ) ;
}


//
// const member functions
//

const HIHelixOperation*
HICompositeOperation::getChild( int iChild ) const
{
   return ( *m_operations )[ iChild ] ;
}

DABoolean
HICompositeOperation::perform( HIHelix& aHelixAfterOperation,
			       const HIHelix& aHelixBeforeOperation,
			       const HIHelix& aHelixAtLastIntersection,
			       HIIntersectionSurface& aSurface,
			       const HIIntersectionSurface& aLastSurface )const
{
   if( m_executeForward )
   {
      for( STL_VECTOR( HIHelixOperation* )::const_iterator iter =
	      m_operations->begin() ;
	   iter != m_operations->end() ;
	   ++iter )
      {
	 if( !(*iter)->perform( aHelixAfterOperation,
				aHelixBeforeOperation,
				aHelixAtLastIntersection,
				aSurface,
				aLastSurface ) )
	 {
	    return false ;
	 }
      }
   }
   else
   {
      for( STL_VECTOR( HIHelixOperation* )::reverse_iterator iter =
	      m_operations->rbegin() ;
	   iter != m_operations->rend() ;
	   ++iter )
      {

	 if( !(*iter)->perform( aHelixAfterOperation,
				aHelixBeforeOperation,
				aHelixAtLastIntersection,
				aSurface,
				aLastSurface ) )
	 {
	    return false ;
	 }
      }
   }

   return true ;
}


int
HICompositeOperation::numberOfChildren() const
{
   if( m_operations->size() == 0 )
   {
      return 0 ;
   }

   STL_VECTOR( HIHelixOperation* )::const_iterator operItr =
      m_operations->begin() ;
   STL_VECTOR( HIHelixOperation* )::const_iterator endOper =
      m_operations->end() ;

   int nChildren = 0 ;
   for( ; operItr != endOper ; ++operItr )
   {
      if( ( *operItr )->isComposite() )
      {
	 HICompositeOperation* castOper =
	    ( HICompositeOperation* ) ( *operItr ) ;
	 nChildren += castOper->numberOfChildren() ;
      }
      else
      {
	 ++nChildren ;
      }
   }
   return nChildren ;
}

//
// static member functions
//
