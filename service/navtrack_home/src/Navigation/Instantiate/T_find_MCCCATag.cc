// -*- C++ -*-
//
// Package:     <package>
// Module:      T_find_MCCCATag
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Adam Lyon
// Created:     Tue Dec 18 17:25:35 EST 2001
// $Id: T_find_MCCCATag.cc,v 1.1 2003/06/04 20:16:45 tom Exp $
//
// Revision history
//
// $Log: T_find_MCCCATag.cc,v $
// Revision 1.1  2003/06/04 20:16:45  tom
// Move higher-level Eta, Pi0, and Ks tagging code to Navigation to
//   live with the NavEta, NavPi0, and NavKs objects, which are built
//   from the more basic tracks and showers. Also eliminates circular
//   linking dependency that resulted from earlier location of these files
//   in the tagging libraries.
//
// Revision 1.2  2003/02/19 15:34:05  tom
// Put instantiations into files with corresponding names (had been swapped)
//
// Revision 1.1  2003/02/03 19:53:25  bkh
// Restructure interface.
//
// Revision 1.1.1.1  2001/12/19 22:39:27  lyon
// Imported MCCCTagger sources
//

#include "Experiment/Experiment.h"

#include "MCCCTagger/MCCCATag.h"


// system include files
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

// user include files


typedef const MCCCATag* _find_iterator_ ;
typedef MCCCATag  _find_type_ ;

#include "STLUtility/instantiate_find.h"
