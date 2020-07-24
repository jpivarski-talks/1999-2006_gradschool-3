// -*- C++ -*-
//
// Package:     HelixIntersection
// Module:      HICompositeCondition
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Tue Sep 22 22:26:12 EDT 1998
// $Id: HICompositeCondition.cc,v 1.7 2001/03/26 06:46:55 wsun Exp $
//
// Revision history
//
// $Log: HICompositeCondition.cc,v $
// Revision 1.7  2001/03/26 06:46:55  wsun
// Added deleteAndRemoveLastChild().
//
// Revision 1.6  1999/06/18 19:25:52  wsun
// More improvements to low momentum fitting.
//
// Revision 1.5  1999/06/02 23:05:18  wsun
// Added features for low-momentum fitting.
//
// Revision 1.4  1999/04/29 20:15:34  wsun
// Additional fixes for Solaris.
//
// Revision 1.3  1999/04/28 22:57:15  wsun
// Fixes for Solaris.
//
// Revision 1.2  1999/04/27 22:06:19  wsun
// Major reorganization and structural changes.
//
// Revision 1.1.1.1  1998/12/07 04:58:23  wsun
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
#include "HelixIntersection/HICompositeCondition.h"

// STL classes
#include <vector>

//
// constants, enums and typedefs
//

static const char* const kFacilityString =
"HelixIntersection.HICompositeCondition" ;

//
// static data member definitions
//

//
// constructors and destructor
//
HICompositeCondition::HICompositeCondition()
   : HICondition(),
     m_conditions( new STL_VECTOR( HICondition* ) )
{
}

// HICompositeCondition::HICompositeCondition( const HICompositeCondition& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HICompositeCondition::~HICompositeCondition()
{
   deleteChildren() ;
   delete m_conditions ;
}

//
// assignment operators
//
// const HICompositeCondition& HICompositeCondition::operator=( const HICompositeCondition& rhs )
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
HICompositeCondition::addChild( HICondition* aCondition )
{
   m_conditions->push_back( aCondition ) ;
}

void
HICompositeCondition::removeLastChild()
{
   if( m_conditions->size() > 0 )
   {
      m_conditions->pop_back() ;
   }
}

void
HICompositeCondition::deleteAndRemoveLastChild()
{
   if( m_conditions->size() > 0 )
   {
      delete *( m_conditions->end() - 1 ) ;
      m_conditions->pop_back() ;
   }
}

void
HICompositeCondition::deleteChildren()
{
   for( STL_VECTOR( HICondition* )::iterator iter = m_conditions->begin() ;
	iter != m_conditions->end() ;
	++iter )
   {
      delete *iter ;
   }

   m_conditions->clear() ;
}


void
HICompositeCondition::setPrintDiagnostics( DABoolean flag )
{
   for( STL_VECTOR( HICondition* )::iterator iter = m_conditions->begin() ;
	iter != m_conditions->end() ;
	++iter )
   {
      ( *iter )->setPrintDiagnostics( flag ) ;
   }
   HICondition::setPrintDiagnostics( flag ) ;
}

//
// const member functions
//

const HICondition* 
HICompositeCondition::getChild( int iChild ) const
{
   return ( *m_conditions )[ iChild ] ;
}

DABoolean
HICompositeCondition::satisfied( const HIHelix& aHelix,
				 HIIntersectionSurface& aSurface ) const
{
   for( STL_VECTOR( HICondition* )::const_iterator iter =
	   m_conditions->begin() ;
	iter != m_conditions->end() ;
	++iter )
   {
      if( !(*iter)->satisfied( aHelix, aSurface ) )
      {
	 return false ;
      }
   }
   return true ;
}


int
HICompositeCondition::numberOfChildren() const
{
   STL_VECTOR( HICondition* )::const_iterator condItr = m_conditions->begin() ;
   STL_VECTOR( HICondition* )::const_iterator endCond = m_conditions->end() ;

   int nChildren = 0 ;
   for( ; condItr != endCond ; ++condItr )
   { 
      if( ( *condItr )->isComposite() )
      {
	 HICompositeCondition* castCond =
	    ( HICompositeCondition* ) ( *condItr ) ;
	 nChildren += castCond->numberOfChildren() ;
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
