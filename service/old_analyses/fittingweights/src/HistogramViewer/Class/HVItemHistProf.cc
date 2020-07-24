// -*- C++ -*-
//
// Package:     <HistogramViewer>
// Module:      HVItemHistProf
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Wed Jul 18 14:54:33 EDT 2001
// $Id: HVItemHistProf.cc,v 1.1 2001/09/25 19:38:41 mccann Exp $
//
// Revision history
//
// $Log: HVItemHistProf.cc,v $
// Revision 1.1  2001/09/25 19:38:41  mccann
// forgot to put HVItemHistProf in the repository
//
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
#include "HistogramViewer/HVItemHistProf.h"

#include "HistogramInterface/HIHistProf.h"
//
// constants, enums and typedefs
//

static const char* const kFacilityString = "HistogramViewer.HVItemHistProf" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HVItemHistProf.cc,v 1.1 2001/09/25 19:38:41 mccann Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
HVItemHistProf::HVItemHistProf(QListViewItem* iParent, const HIHistProf* iHist) :
   HVItemBase( iParent, iHist->title().c_str(), "Prof", iHist->id() ),
   m_hist(iHist)
{
}

// HVItemHistProf::HVItemHistProf( const HVItemHistProf& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

//HVItemHistProf::~HVItemHistProf()
//{
//}

//
// assignment operators
//
// const HVItemHistProf& HVItemHistProf::operator=( const HVItemHistProf& rhs )
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

//
// const member functions
//

//
// static member functions
//
