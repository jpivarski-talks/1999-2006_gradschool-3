// -*- C++ -*-
//
// Package:     TrackRoot
// Module:      T_TrackMany
// 
// Description: Instantiate Classes for TRHelixElectronFit
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Michael Marsh
// Created:     Wed Jun 24 14:06:59 EDT 1998
// $Id: T_TRHelixElectronFitField.cc,v 1.3 2000/01/05 19:08:03 cdj Exp $
//
// Revision history
//
// $Log: T_TRHelixElectronFitField.cc,v $
// Revision 1.3  2000/01/05 19:08:03  cdj
// added storage helpers
//
// Revision 1.2  1998/09/30 17:06:18  cdj
// use new instantiate macro
//
// Revision 1.1.1.1  1998/07/20 21:16:11  marsh
// New library for base-level track objects
//
// Revision 1.1.1.1  1998/07/13 23:27:57  marsh
// New library for track objects
//
//

#include "TrackRoot/Storage/instantiate_helix_field.h" 

INSTANTIATE_HELIX_FIELD( DBCandidate::kElectron, Electron )

#include "FrameAccess/instantiate_many.h" 

#include "StorageManagement/instantiate_faptrtablehelper.h" 

