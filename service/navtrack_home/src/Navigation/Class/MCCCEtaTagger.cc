// -*- C++ -*-
//
// Package:     <Navigation>
// Module:      MCCCEtaTagger
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Adam Lyon
// Created:     Thu Mar  7 22:36:05 EST 2002
// $Id: MCCCEtaTagger.cc,v 1.1 2003/06/04 20:16:43 tom Exp $
//
// Revision history
//
// $Log: MCCCEtaTagger.cc,v $
// Revision 1.1  2003/06/04 20:16:43  tom
// Move higher-level Eta, Pi0, and Ks tagging code to Navigation to
//   live with the NavEta, NavPi0, and NavKs objects, which are built
//   from the more basic tracks and showers. Also eliminates circular
//   linking dependency that resulted from earlier location of these files
//   in the tagging libraries.
//
// Revision 1.1  2003/02/03 19:53:11  bkh
// Restructure interface.
//
// Revision 1.2  2002/04/30 13:32:29  cleo3
// added missing report.h
//
// Revision 1.1  2002/03/14 23:35:38  lyon
// A Eta tagger
//

#include "Experiment/Experiment.h"
#include "Navigation/NavEtaToGG.h"
#include "MCCCTagger/Template/MCCCGGTagger.cc"

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
  
template class MCCCGGTagger< NavEtaToGG > ;
