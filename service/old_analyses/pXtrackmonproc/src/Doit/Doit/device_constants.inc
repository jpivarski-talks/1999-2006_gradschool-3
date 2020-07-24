#if !defined(DOIT_DEVICE_CONSTANTS_H)
#define DOIT_DEVICE_CONSTANTS_H
#if 0
// -*- C++ -*-
//
// Package:     <Doit>
// Module:      device_constants
// 
/**\class device_constants device_constants.h Doit/device_constants.h

 Description: constants to be shared between C++ and Fortran that denote
               what device a hit comes from

 Usage:
    <usage>

*/
//
// Author:      Chris D Jones
// Created:     Fri May 19 15:25:05 EDT 2000
// $Id: device_constants.inc,v 1.1 2000/05/19 19:58:57 cdj Exp $
//
// Revision history
//
// $Log: device_constants.inc,v $
// Revision 1.1  2000/05/19 19:58:57  cdj
// C++ now knows what device the doit hit comes from
//
#endif

#define kDoitDeviceDR 1
#define kDoitDeviceCathode 2
#define kDoitDeviceSVRphi 3
#define kDoitDeviceSVZ 4

#endif /* DOIT_DEVICE_CONSTANTS_H */
