// -*- C++ -*-
//
// Package:     <HistogramViewer>
// Module:      HVItemDir
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Wed Jul 18 14:45:49 EDT 2001
// $Id: HVItemDir.cc,v 1.3 2001/09/21 03:24:17 mccann Exp $
//
// Revision history
//
// $Log: HVItemDir.cc,v $
// Revision 1.3  2001/09/21 03:24:17  mccann
// Implemented profile histograms
//
// Revision 1.2  2001/09/19 16:00:59  cdj
// generalized to allow more than 1D histograms
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
#include "HistogramViewer/HVItemDir.h"
#include "HistogramViewer/HVItemHist1D.h"
#include "HistogramViewer/HVItemHist2D.h"
#include "HistogramViewer/HVItemHistProf.h"

#include "HistogramInterface/HIHistDir.h"

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "HistogramViewer.HVItemDir" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HVItemDir.cc,v 1.3 2001/09/21 03:24:17 mccann Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
HVItemDir::HVItemDir( QListViewItem* iParent, const HIHistDir* iDir ) :
   HVItemBase(iParent, QString( iDir->name().c_str()), QString("Directory"), 0 ),
   m_dir(iDir)
{
   makeChildren();

}

HVItemDir::HVItemDir( QListView* iParent, const HIHistDir* iDir ) :
   HVItemBase(iParent, QString( iDir->name().c_str()), QString("Directory"), 0 ),
   m_dir(iDir)
{
   makeChildren();
}

// HVItemDir::HVItemDir( const HVItemDir& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

//HVItemDir::~HVItemDir()
//{
//}

//
// assignment operators
//
// const HVItemDir& HVItemDir::operator=( const HVItemDir& rhs )
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
HVItemDir::expanded()
{
}

void
HVItemDir::collapsed()
{
}

void
HVItemDir::updateInfo()
{
}

void
HVItemDir::makeChildren()
{
   //make all Hist1D's
   for( HIHistDir::const_h1d_iterator itHist = m_dir->h1d_begin();
	itHist != m_dir->h1d_end();
	++itHist ) {
      insertItem( new HVItemHist1D( this, *itHist ) );
   }

   //make all Hist2D's
   for( HIHistDir::const_h2d_iterator itHist2 = m_dir->h2d_begin();
	itHist2 != m_dir->h2d_end();
	++itHist2 ) {
      insertItem( new HVItemHist2D( this, *itHist2 ) );
   }

   //make all HistProf's
   for( HIHistDir::const_prof_iterator itHistP = m_dir->prof_begin();
	itHistP != m_dir->prof_end();
	++itHistP ) {
      insertItem( new HVItemHistProf( this, *itHistP ) );
   }

   //make all directories
   for( HIHistDir::const_dir_iterator itDir = m_dir->dir_begin();
	itDir != m_dir->dir_end();
	++itDir ) {
      insertItem( new HVItemDir( this, *itDir ) );
   }
}

//
// const member functions
//

//
// static member functions
//
