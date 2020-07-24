#if !defined(CDOFFCAL_NAIVEDRIFTFUNCTION_H)
#define CDOFFCAL_NAIVEDRIFTFUNCTION_H
// -*- C++ -*-
//
// Package:     <CDOffCal>
// Module:      NaiveDriftFunction
// 
// Author:      Martin Lohner, Ken McLean
// Created:     Wed Mar  4 14:16:47 EST 1998
// $Id: NaiveDriftFunction.h,v 1.5 2000/10/31 23:09:20 mclean Exp $
//
// Revision history
//
// $Log: NaiveDriftFunction.h,v $
// Revision 1.5  2000/10/31 23:09:20  mclean
// *** empty log message ***
//
// Revision 1.4  2000/02/02 15:36:17  cdj
// now dependent on DFDataHolder instead of DriftFunctionProxy
//
// Revision 1.3  1999/12/07 23:29:41  mclean
// Many changes:
//    (1) Parameter data is stored locally to the Producer and accessible
//        via member functions.
//    (2) Frame data is stored locally to the Proxy and accessible via
//        member functions (i.e. Geometry, Constants)
//    (3) Includes have been streamlined to speed up makedepend
//    (4) New parameters have been instituted for
//                 GlobalT0     (Global T0 shift)
//                 TDCQuantum   (TDC slope, 250. MC, -500. data)
//                 Mintime      (in normal time sense, earliest allowed hit)
//                 Maxtime      (in normal time sense, latest allowed hit)
//    (5) The following masks are overlayed in the "goodness"
//        parameter returned to makeCalibratedHit:
//                Flag_Bad         =  1;
//                Flag_SingleTDC   =  2;
//                Flag_TestChannel =  4;
//                Flag_NoCharge    =  8;
//                Flag_NoTime      = 16;
//                Flag_NotInWindow = 32;
//    (6) Class HitQuality has its own file and additional functionality
//        to set_ and test_ all the above qualities.
//    (7) Provision for access to all current DR constant entities
//        has been made for in the Proxy.
//    (8) There is an additional Class inheriting from DriftFunction:
//        DataDriftFunction. This will use real Frame-based constants
//        eventually.
//
// Revision 1.2  1999/11/26 22:11:45  mclean
// Moved constants from parent DriftFunction Class to derived class
// they are actually used in.
//
// Revision 1.1  1999/06/23 22:16:55  lyon
// Specific drift functions
//
#endif /* CDOFFCAL_NAIVEDRIFTFUNCTION_H */
