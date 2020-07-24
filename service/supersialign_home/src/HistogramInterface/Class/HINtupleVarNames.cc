// -*- C++ -*-
//
// Package:     HistogramInterface
// Module:      HINtupleVarNames
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D. Jones
// Created:     Fri Jul 24 12:17:35 EDT 1998
// $Id: HINtupleVarNames.cc,v 1.1 1999/02/24 19:15:53 cdj Exp $
//
// Revision history
//
// $Log: HINtupleVarNames.cc,v $
// Revision 1.1  1999/02/24 19:15:53  cdj
// first submission
//
// Revision 1.2  1998/10/22 17:38:47  cdj
// name() now returns a const char* const *
//
// Revision 1.1  1998/07/24 18:31:54  cdj
// First submission
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */
#include <assert.h>
#include <string.h>

// user include files
#include "Experiment/report.h"
#include "HistogramInterface/HINtupleVarNames.h"

// STL classes

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "HistogramInterface.HINtupleVarNames" ;

//
// static data member definitions
//

//
// constructors and destructor
//
HINtupleVarNames::HINtupleVarNames( unsigned int iNumberOfVariables ) :
   m_nVars( iNumberOfVariables )
{
   m_varNames = new char*[ m_nVars ];

   if( 0 == m_varNames ){
      report( ERROR, kFacilityString ) << "Unable to allocate memory" << endl;
      assert(false);
      exit(-1);
   }

   //clear the array
   for( char** pIndex = m_varNames ; 
	pIndex != m_varNames + m_nVars; 
	++pIndex ){
      *pIndex = 0;
   }
}

// HINtupleVarNames::HINtupleVarNames( const HINtupleVarNames& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HINtupleVarNames::~HINtupleVarNames()
{
   //delete the entries
   for( char** pIndex = m_varNames ; 
	pIndex != m_varNames + m_nVars; 
	++pIndex ){
      delete [] *pIndex ;
   }

   delete [] m_varNames;
}

//
// assignment operators
//
// const HINtupleVarNames& HINtupleVarNames::operator=( const HINtupleVarNames& rhs )
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
HINtupleVarNames::addVar( unsigned int iIndex, const char* iName )
{
   //check to see if index is within our limit
   if( iIndex >= m_nVars ) {
      report( ERROR, kFacilityString ) 
	 <<"addVar( "<<iIndex<<" , " << iName << "):\n"
	 <<"index is larger than or equal to number of Variables="
	 << m_nVars <<endl;
      exit( -1 );
   }

   //check to see if index has already been used
   if( m_varNames[ iIndex ] != 0 ) {
      report( ERROR, kFacilityString ) 
	 <<"addVar( "<<iIndex<<" , " << iName << "):\n"
	 <<"index has already been assigned during call to "
	 <<"addVar( "<<iIndex<<" , " << m_varNames[ iIndex ]<< ")" << endl;

      exit( -1 );
   }

   // check to see if variable name has already been used
   for( char** pIndex = m_varNames ; 
	pIndex != m_varNames + m_nVars; 
	++pIndex ){
      if( 0 != *pIndex ) { 
	 if( 0 == strcmp( *pIndex, iName ) ) {
	    report( ERROR, kFacilityString ) 
	       <<"addVar( "<<iIndex<<" , " << iName << "):\n"
	       <<"name has already been assigned during call to "
	       <<"addVar( "<<pIndex - m_varNames <<" , " << iName << ")" 
	       << endl;
	    exit( -1 );
	 }
      }
   }
   
   //everything is fine
   char** pName = m_varNames + iIndex;
   *pName = new char[ strlen( iName ) + 1];

   if( 0 == *pName ){
      report( ERROR, kFacilityString ) << "Out of memory" << endl;
      assert(false);
      exit( -1 );
   }

   strcpy( *pName, iName );
}

//
// const member functions
//
const char* const*
HINtupleVarNames::names() const
{
   //See if all variables have been assigned
   for( char** pIndex = m_varNames ; 
	pIndex != m_varNames + m_nVars; 
	++pIndex ){
      if( 0 == *pIndex ) { 
	 report(ERROR, kFacilityString)
	    <<" NTuple has missing variable name for index: " 
	    << pIndex - m_varNames << endl;
	 exit( -1 );
      }
   }

   return (const char* const*) m_varNames;
}

//
// static member functions
//
