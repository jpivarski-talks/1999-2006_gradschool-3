// -*- C++ -*-
//
// Package:     <ConstantsDelivery>
// Module:      DBConstantsSelectionTag
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Rich Baker
// Created:     Sat Nov  7 13:17:39 EST 1998
// $Id: DBConstantsSelectionTag.cc,v 1.4 1999/04/29 18:45:36 cdj Exp $
//
// Revision history
//
// $Log: DBConstantsSelectionTag.cc,v $
// Revision 1.4  1999/04/29 18:45:36  cdj
// updated to use new constants implementation
//
// Revision 1.3  1998/12/03 19:34:16  baker
// Minor changes to support interface to ConstantsModule
//
// Revision 1.2  1998/11/24 20:12:45  baker
// Change normal constructor for ConstantsSourceBinder
// to allow specification of the desired Federated
// Database.
// Added a Constructor for DBConstantsRecordInfo
// that takes a single string variable.
//
// Revision 1.1  1998/11/07 19:12:47  baker
// Added Data Source ID information to Controller
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "ConstantsDelivery/DBConstantsSelectionTag.h"

// STL classes

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "ConstantsDelivery.DBConstantsSelectionTag" ;

//
// static data member definitions
//

//
// constructors and destructor
//
DBConstantsSelectionTag::DBConstantsSelectionTag(
   const STL_VECTOR(string)& iSelTags,
   const DBVersion::SelectType iType) :
   m_selTags( iSelTags ),
   m_type( iType )
{
}
// 11/24/98 Rich Baker
// Add new constructor that takes only a single string instead
// of requiring a vector of strings
DBConstantsSelectionTag::DBConstantsSelectionTag(
   const string& iSelTag,
   const DBVersion::SelectType iType) :
   m_type( iType )
{
   m_selTags.push_back( iSelTag );
}

DBConstantsSelectionTag::DBConstantsSelectionTag( 
   const DBConstantsSelectionTag& rhs ) :
   m_selTags( rhs.selectionTags() ),
   m_type( rhs.type() )
{
}

DBConstantsSelectionTag::~DBConstantsSelectionTag()
{
}

//
// assignment operators
//
const DBConstantsSelectionTag& DBConstantsSelectionTag::operator=( 
   const DBConstantsSelectionTag& rhs )
{
   if( this != &rhs ) {   
     m_selTags = rhs.selectionTags();
     m_type = rhs.type();
   }
   return *this;
}

//
// member functions
//

//
// const member functions
//
const STL_VECTOR( string )& 
DBConstantsSelectionTag::selectionTags() const
{
  return m_selTags;
}

DBVersion::SelectType 
DBConstantsSelectionTag::type() const
{
   return m_type;
}

string DBConstantsSelectionTag::toString() const
{
   string tagList;
   for ( int i = 0; i < m_selTags.size(); i++ ) {
      tagList += m_selTags[i];
      if ( m_selTags.size() > i+1 ) tagList += " ";
   }
   return tagList;
}

//
// static member functions
//
