// -*- C++ -*-
//
// Package:     <Navigation>
// Module:      T_NavKs
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      David Urner
// Created:     Tue Mar 20 11:15:57 EST 2001
// $Id: T_NavKs.cc,v 1.2 2003/09/02 01:53:10 cdj Exp $
//
// Revision history
//
// $Log: T_NavKs.cc,v $
// Revision 1.2  2003/09/02 01:53:10  cdj
// can now store NavKs and NavShowers using templated storage helper
//
// Revision 1.1  2001/03/26 20:13:51  urner
// added Nav Ks
//

#include "Experiment/Experiment.h"

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

#include "Navigation/NavKs.h"


#include "FrameAccess/FAPtrTable.h"

#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_MANY( NavKs )
//Use the following if 'NavKs' contains a ','
// also make sure there are no blank spaces within 'NavKs'
/* INSTANTIATE_MANY_1_COMMA( NavKs ) */
#include "FrameAccess/instantiate_many.h"

#include "Navigation/Storage/ManyNavKsStorageHelper.h"
SM_REGISTER_STORAGE(ManyNavKsStorageHelper)
