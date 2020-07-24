// Author:      Guangshun Huang
// Created:     Thu Jan 9 2003
// $Id: mathfix.h,v 1.1.1.1 2003/06/26 19:02:49 huanggs Exp $
//
// Revision history
//
// $Log: mathfix.h,v $
// Revision 1.1.1.1  2003/06/26 19:02:49  huanggs
// imported ZDDriftFunctionProd source
//
// Revision 1.1.1.1  2003/02/03 20:11:58  huanggs
// imported source
//
// A cxx bug fix. See 
// /nfs/cleo3/dlib/include/Experiment/Experiment.h, deccxx_flags.h
// If I could get the Makefile to work,
// I'd just include this file (although it wouldn't work
// at Caltech unless I copied everything over...)
//      AJW, 9/16/97

//#include "Experiment/Experiment.h"
//#if defined(EXCEPTION_CLASH_WITH_MATH_H_BUG)
#define exception _math_bug_
#include <math.h>  // required for trigonometry and fabs
#undef exception
//#else
//#include <math.h>  // required for trigonometry and fabs
//#endif /* EXCEPTION_CLASH_WITH_MATH_H_BUG */

