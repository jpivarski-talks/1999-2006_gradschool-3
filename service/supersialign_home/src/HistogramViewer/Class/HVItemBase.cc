// -*- C++ -*-
//
// Package:     <HistogramViewer>
// Module:      HVItemBase
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Thu Jul 19 09:19:54 EDT 2001
// $Id: HVItemBase.cc,v 1.1.1.1 2001/07/30 14:11:33 cdj Exp $
//
// Revision history
//
// $Log: HVItemBase.cc,v $
// Revision 1.1.1.1  2001/07/30 14:11:33  cdj
// importing
//

#include "Experiment/Experiment.h"

// system include files
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

// user include files
//#include "Experiment/report.h"
#include "HistogramViewer/HVItemBase.h"


//
// constants, enums and typedefs
//

static const char* const kFacilityString = "HistogramViewer.HVItemBase" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HVItemBase.cc,v 1.1.1.1 2001/07/30 14:11:33 cdj Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
HVItemBase::HVItemBase(
   QListViewItem* iParent,
   const QString& iName,
   const QString& iType,
   unsigned int iIdentifier) :
   QListViewItem( iParent, iName, iType, QString::number(iIdentifier) )
{
}

HVItemBase::HVItemBase(
   QListView* iParent,
   const QString& iName,
   const QString& iType,
   unsigned int iIdentifier) :
   QListViewItem( iParent, iName, iType, QString::number(iIdentifier) )
{
}
// HVItemBase::HVItemBase( const HVItemBase& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HVItemBase::~HVItemBase()
{
}

//
// assignment operators
//
// const HVItemBase& HVItemBase::operator=( const HVItemBase& rhs )
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
HVItemBase::expanded()
{
}

void
HVItemBase::collapsed()
{
}

void
HVItemBase::updateInfo()
{
}

//
// const member functions
//

//
// static member functions
//
