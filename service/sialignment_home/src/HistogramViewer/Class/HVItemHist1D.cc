// -*- C++ -*-
//
// Package:     <HistogramViewer>
// Module:      HVItemHist1D
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Wed Jul 18 14:54:33 EDT 2001
// $Id: HVItemHist1D.cc,v 1.1.1.1 2001/07/30 14:11:33 cdj Exp $
//
// Revision history
//
// $Log: HVItemHist1D.cc,v $
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
#include "HistogramViewer/HVItemHist1D.h"

#include "HistogramInterface/HIHist1D.h"
//
// constants, enums and typedefs
//

static const char* const kFacilityString = "HistogramViewer.HVItemHist1D" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HVItemHist1D.cc,v 1.1.1.1 2001/07/30 14:11:33 cdj Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
HVItemHist1D::HVItemHist1D(QListViewItem* iParent, const HIHist1D* iHist) :
   HVItemBase( iParent, iHist->title().c_str(), "1D", iHist->id() ),
   m_hist(iHist)
{
}

// HVItemHist1D::HVItemHist1D( const HVItemHist1D& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

//HVItemHist1D::~HVItemHist1D()
//{
//}

//
// assignment operators
//
// const HVItemHist1D& HVItemHist1D::operator=( const HVItemHist1D& rhs )
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
