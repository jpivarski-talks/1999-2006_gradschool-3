#if !defined(GEOMETRYFORFORTRAN_GEOMETRYFORFORTRAN_H)
#define GEOMETRYFORFORTRAN_GEOMETRYFORFORTRAN_H
// -*- C++ -*-
//
// Package:     <geometryForFortran>
// Module:      geometryForFortran
// 
// Description: Handles passing geometry for Fortran
//
// Usage:
//    <usage>
//
// Author:      Adam Lyon
// Created:     Tue May  4 09:42:57 EDT 1999
// $Id: geometryForFortran.h,v 1.8 2003/06/27 20:05:17 huanggs Exp $
//
// Revision history
//
// $Log: geometryForFortran.h,v $
// Revision 1.8  2003/06/27 20:05:17  huanggs
// change zdStore argument type
//
// Revision 1.7  2003/04/24 14:22:17  huanggs
// more arguments for fillFortranCDGeometry
//
// Revision 1.6  2003/02/04 15:10:39  huanggs
// zd added
//
// Revision 1.5  2000/03/29 20:50:02  pg
// Added fillFortranSVGeometry.cc to be less disruptive to other packages
//
// Revision 1.4  2000/03/28 16:02:54  pg
// Added routine gff_fillsigeom.F to make doit Si-aware
//
// Revision 1.3  1999/10/31 15:11:25  lyon
// improvements for cathodes
//
// Revision 1.2  1999/05/28 02:38:41  lyon
// Changed names of some routines
//
// Revision 1.1.1.1  1999/05/04 17:44:13  lyon
// Imported geometryForFortran sources
//
//


#include "ADRGeom/ADRSenseWireStore.h"
#include "ADRGeom/ADRCathodeStore.h"
#include "ASiStorePro/ASiStore.h"
#include "AZDGeom/AZDSenseWireStore.h"

void fillFortranCDGeometry(const ADRSenseWireStore& g,
			   const ADRCathodeStore& gc,
			   const AZDSenseWireStore* gzd,
			   DABoolean fill_SI,
			   DABoolean fill_ZD );

void fillFortranSVGeometry(const ASiStore& gsi);

#endif /* GEOMETRYFORFORTRAN_GEOMETRYFORFORTRAN_H */
