#if !defined(CDOFFCAL_HEPRDRIFTFUNCTION_H)
#define CDOFFCAL_HEPRDRIFTFUNCTION_H
// -*- C++ -*-
//
// Package:     <CDOffCal>
// Module:      HePrDriftFunction
// 
// Author:      Martin Lohner, Ken McLean
// Created:     Wed Mar  4 14:16:47 EST 1998
// $Id: HePrDriftFunction.h,v 1.6 2000/10/31 23:09:20 mclean Exp $
//
// Revision history
//
// $Log: HePrDriftFunction.h,v $
// Revision 1.6  2000/10/31 23:09:20  mclean
// *** empty log message ***
//
// Revision 1.5  2000/02/02 15:36:17  cdj
// now dependent on DFDataHolder instead of DriftFunctionProxy
//
// Revision 1.4  1999/12/15 19:41:47  mclean
// Some bug fixes, made get-parameter functions in CDOffCalProducer
// inline functions, added channel t0s to DriftFunction.h,
// added DebugLevel parameter to CDOffCalProducer.
//
// Revision 1.3  1999/12/07 23:29:39  mclean
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
// Revision 1.2  1999/11/26 22:11:44  mclean
// Moved constants from parent DriftFunction Class to derived class
// they are actually used in.
//
// Revision 1.1  1999/06/23 22:16:54  lyon
// Specific drift functions
//
// Revision 1.21  1999/03/05 20:57:37  pg
// Switched to using STL_XXX macros (was crashing on Solaris). Got rid of delete calls in DriftFunction.cc (same reason)
//
// Revision 1.20  1998/12/15 21:03:30  mclean
// First working HePr DF code (hardwired, tcl parameters not enabled, not for
//                             general use)
//
// Revision 1.19  1998/12/10 21:43:28  mclean
// Updates for HePr drift functions (old analytic DFs are still default)
//
// Revision 1.18  1998/11/27 20:23:51  mclean
// Introduce STL definition conventions.
//
// Revision 1.17  1998/11/26 14:04:50  mclean
// Explicitly set CPP preprocessor flag to disable "HePr" drift functions
// (these can not be default until we modify cg3/cdoffcal, I will install
// a tcl toggle by that time)
//
// Revision 1.16  1998/11/25 23:07:14  lyon
// Use STL macros
//
// Revision 1.15  1998/11/19 19:16:15  mclean
// Preliminary changes for HEPR drift functions...dummied out in this
// file with C-preprocessor statements.
//
// Revision 1.12  1998/10/07 21:28:46  mclean
// Modified wire number to remove non-sense wires :)
//
// Revision 1.11  1998/09/21 00:57:04  lyon
// Changed location of Tseries
//
// Revision 1.10  1998/09/10 15:12:19  lyon
// Added getCalibratedDRSeries
//
// Revision 1.24  1999/11/26 22:11:43  mclean
// Moved constants from parent DriftFunction Class to derived class
// they are actually used in.
//
// Revision 1.22  1999/06/23 22:17:15  lyon
// DriftFunction is now a base class
//
#endif /* CDOFFCAL_HEPRDRIFTFUNCTION_H */
