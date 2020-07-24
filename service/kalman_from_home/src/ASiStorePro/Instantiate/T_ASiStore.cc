// -*- C++ -*-
//
// Package:     ASiStorePro
// Module:      T_ASiStore
// 
// Description: Instantiate Classes for ASiStore
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Alexander Undrus
// Created:     Sat Jan  9 13:40:27 EST 1999
// $Id: T_ASiStore.cc,v 1.3 1999/07/09 17:34:49 cleo3 Exp $
//
// Revision history
//
// $Log: T_ASiStore.cc,v $
// Revision 1.3  1999/07/09 17:34:49  cleo3
//  arguments returned by reference are declared const
//
// Revision 1.2  1999/03/24 02:51:34  cleo3
// added new ASiStoreConstants.h, added functions with STL vector of subelements, added functions returning various objects
//
// Revision 1.1.1.1  1999/01/23 02:28:03  undrus
// imported c3tr sources
//
//

#include "Experiment/Experiment.h"

// user include files
#include "ASiStorePro/ASiStore.h"

#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_ONE( ASiStore )
//Use the following if 'ASiStore' contains a ','
// also make sure there are no blank spaces within 'ASiStore'
/* INSTANTIATE_ONE_1_COMMA( ASiStore ) */
#include "FrameAccess/instantiate_one.h"
// STL classes
#include <vector>

typedef const ASiLayer* _vector_contents_;

#include "STLUtility/instantiate_vector.h"
