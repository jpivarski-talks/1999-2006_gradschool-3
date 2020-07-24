// -*- C++ -*-
//
// Package:     <DummyHistogram>
// Module:      DummyNtuple
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Bill McCloskey
// Created:     Sat Mar  6 23:06:06 EST 1999
// $Id: DummyNtuple.cc,v 1.1.1.1 1999/05/12 00:10:10 billm Exp $
//
// Revision history
//
// $Log: DummyNtuple.cc,v $
// Revision 1.1.1.1  1999/05/12 00:10:10  billm
// imported DummyHistogram sources
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "DummyHistogram/DummyNtuple.h"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "DummyHistogram.DummyNtuple" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DummyNtuple.cc,v 1.1.1.1 1999/05/12 00:10:10 billm Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
DummyNtuple::DummyNtuple( unsigned int  iId,
			  const string& iTitle,
			  unsigned int  iNumberOfVariables,
			  unsigned int  iChunkSize,
			  const char* const* iLabels ) :
  HINtuple( iId,
	    iTitle,
	    iNumberOfVariables,
	    iChunkSize,
	    iLabels )
{
}

// DummyNtuple::DummyNtuple( const DummyNtuple& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

DummyNtuple::~DummyNtuple()
{
}

//
// assignment operators
//
// const DummyNtuple& DummyNtuple::operator=( const DummyNtuple& rhs )
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
void DummyNtuple::fill( const float* iEntry )
{
}

//
// Book a row-wise Ntuple into memory on an RZ file.  Only single 
// precision floating point numbers can be strored, and no data
// compression is provided.
//
void DummyNtuple::book( const string& iTitle,
			const string& iTopDir,
			unsigned int  iNumberOfVariables,
			unsigned int  iChunkSize,
			const char* const* iLabels )
{
}

void DummyNtuple::close (void ) 
{
}

//
// const member functions
//

//
// static member functions
//
