// -*- C++ -*-
//
// Package:     <HistogramViewer>
// Module:      HVCommunicate
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim McCann
// Created:     Tue May 14 15:11:23 EDT 2002
// $Id$
//
// Revision history
//
// $Log$

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
#include "HistogramViewer/HVCommunicate.h"


//
// constants, enums and typedefs
//

static const char* const kFacilityString = "HistogramViewer.HVCommunicate" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: skeleton.cc,v 1.6 2001/01/03 16:25:23 cdj Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

double HVCommunicate::pos_thresh = 0.;
double HVCommunicate::neg_thresh = 0.;

//
// constructors and destructor
//
HVCommunicate::HVCommunicate()
{
}

// HVCommunicate::HVCommunicate( const HVCommunicate& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HVCommunicate::~HVCommunicate()
{
}

//
// assignment operators
//
// const HVCommunicate& HVCommunicate::operator=( const HVCommunicate& rhs )
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
