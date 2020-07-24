// -*- C++ -*-
//
// Package:     <Navigation>
// Module:      T_NavLambda
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Eric Eckhart
// Created:     Tue Mar 20 11:15:57 EST 2001
// $Id: T_NavLambda.cc,v 1.1 2001/09/07 21:51:34 eckhart Exp $
//
// Revision history
//
// $Log: T_NavLambda.cc,v $
// Revision 1.1  2001/09/07 21:51:34  eckhart
// Created File
//
// Revision 1.1  2001/03/26 20:13:51  urner
// added Nav Lambda
//

#include "Experiment/Experiment.h"

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

#include "Navigation/NavLambda.h"


#include "FrameAccess/FAPtrTable.h"

#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_MANY( NavLambda )
//Use the following if 'NavLambda' contains a ','
// also make sure there are no blank spaces within 'NavLambda'
/* INSTANTIATE_MANY_1_COMMA( NavLambda ) */
#include "FrameAccess/instantiate_many.h"


