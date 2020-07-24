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
// $Id: device_constants.inc,v 1.2 2002/09/27 21:01:26 bkh Exp $
//
// Revision history
//
// $Log: device_constants.inc,v $
// Revision 1.2  2002/09/27 21:01:26  bkh
// Add ZD capability and compatibility with new TrackFinder interface
//
// Revision 1.1  2000/05/19 19:58:57  cdj
// C++ now knows what device the doit hit comes from
//
#endif

#include "cl3seq/cdgm3/devicecodes.inc"

#define kDoitDeviceDR DEVICE_DRFT
#define kDoitDeviceCathode DEVICE_DRCT
#define kDoitDeviceSVRphi DEVICE_SVX1
#define kDoitDeviceSVZ DEVICE_SVZ1
#define kDoitDeviceZD DEVICE_ZDA

#endif /* DOIT_DEVICE_CONSTANTS_H */





