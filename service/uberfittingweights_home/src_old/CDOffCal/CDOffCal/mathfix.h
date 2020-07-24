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

