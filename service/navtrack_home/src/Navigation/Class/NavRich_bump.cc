// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavRich
// 
// Description:
//
// Implementation:
//
// Author:      Tomasz Skwarnicki
// Created:     Tue Jul 20 10:46:38 EDT 2000
// $Id: NavRich_bump.cc,v 1.1 2000/07/27 01:31:41 ts Exp $
//
// Revision history
//
// $Log: NavRich_bump.cc,v $
// Revision 1.1  2000/07/27 01:31:41  ts
// NavRich added
//
//

#include "Experiment/Experiment.h"

// system include files
#include <assert.h>
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
//#include <set>
#include <map>
#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
//#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"

#include "Navigation/NavRich.h"
#include "Navigation/NavRichServer.h"

#include "RichPhotonsProd/RichPhoton.h"
#include "RichClustering/RichBump.h"
#include "C3Rich/RichHypotheses.h"


// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
//#include <set>
#include <map>
#include <algorithm>
//#include <utility>


NavRich::RichBumpTable NavRich::richBumpPtrs(   RichHypothesisIDType iHyp, const float minStdDev, const float maxStdDev ) const
{
  RichBumpTable pTable;
  if( iHyp < RichHypotheses::kHypMax ){
  RichPhotonTable photons = richPhotonPtrs(iHyp,minStdDev,maxStdDev);
  RichPhotonTable::const_iterator tableEnd( photons.end() );
  for( RichPhotonTable::const_iterator item( photons.begin() ) ; item != tableEnd ; ++item ) {
               pTable.push_back( (const RichBump*)(&*m_server.richBump( (*item)->bumpID() )) );
  } }
  return pTable;
}


//
// static member functions
//





