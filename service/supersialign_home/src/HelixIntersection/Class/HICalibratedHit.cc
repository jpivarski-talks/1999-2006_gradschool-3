// -*- C++ -*-
//
// Package:     <HelixIntersection>
// Module:      HICalibratedHit
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Sun Jan 30 16:17:50 EST 2000
// $Id: HICalibratedHit.cc,v 1.9 2002/02/17 17:21:38 wsun Exp $
//
// Revision history
//
// $Log: HICalibratedHit.cc,v $
// Revision 1.9  2002/02/17 17:21:38  wsun
// Don't erase track corrections in clear().
//
// Revision 1.8  2000/10/11 05:53:15  wsun
// Corrected previous log comment.
//
// Revision 1.7  2000/10/11 05:25:52  wsun
// Added data members for corrected fitting weight.
//
// Revision 1.6  2000/10/08 23:38:22  wsun
// Added dca smoothing.
//
// Revision 1.5  2000/06/09 00:27:06  wsun
// Cleaned up code.
//
// Revision 1.4  2000/02/25 07:56:46  wsun
// Smoothing: no decorrelation of residuals before taking weighted average.
//
// Revision 1.3  2000/02/07 20:16:52  wsun
// Forgot return statements in calculateSmoothedResidualAndError().
//
// Revision 1.2  2000/02/05 21:39:05  wsun
// Moved sqrt from .h to .cc file to avoid cleo_math.h dependency.
//
// Revision 1.1  2000/02/03 07:13:10  wsun
// First submission.
//

#include "Experiment/Experiment.h"
#include "Experiment/cleo_math.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "HelixIntersection/HICalibratedHit.h"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "HelixIntersection.HICalibratedHit" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HICalibratedHit.cc,v 1.9 2002/02/17 17:21:38 wsun Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
HICalibratedHit::HICalibratedHit( DABoolean aUseInFit,
				  DABoolean aInfoOnTrack )
   : m_useInFit(               aUseInFit ),
     m_infoOnTrack(            aInfoOnTrack ),
     m_nResiduals(             0 ),
     m_smoothedResidual(       0. ),
     m_smoothedErrsq(          0. ),
     m_smoothedDistPred(       0. ),
     m_smoothedDistPredErrsq(  0. ),
     m_residualsSmoothed(      false ),
     m_fittingWeightCorrected( false ),
     m_correctedFittingWeight( 0. )
{
   m_residuals[0] = 0. ;
   m_residuals[1] = 0. ;
   m_errsqs[0] = 0. ;
   m_errsqs[1] = 0. ;

   m_distPreds[0] = 0. ;
   m_distPreds[1] = 0. ;
   m_distPredErrsqs[0] = 0. ;
   m_distPredErrsqs[1] = 0. ;
}

HICalibratedHit::HICalibratedHit( const HICalibratedHit& rhs )
   : m_useInFit(               rhs.m_useInFit ),
     m_infoOnTrack(            rhs.m_infoOnTrack ),
     m_nResiduals(             rhs.m_nResiduals ),
     m_smoothedResidual(       rhs.m_smoothedResidual ),
     m_smoothedErrsq(          rhs.m_smoothedErrsq ),
     m_smoothedDistPred(       rhs.m_smoothedDistPred ),
     m_smoothedDistPredErrsq(  rhs.m_smoothedDistPredErrsq ),
     m_residualsSmoothed(      rhs.m_residualsSmoothed ),
     m_fittingWeightCorrected( rhs.m_fittingWeightCorrected ),
     m_correctedFittingWeight( rhs.m_correctedFittingWeight )
{
   m_residuals[0]     = rhs.m_residuals[0] ;
   m_residuals[1]     = rhs.m_residuals[1] ;
   m_errsqs[0]        = rhs.m_errsqs[0] ;
   m_errsqs[1]        = rhs.m_errsqs[1] ;

   m_distPreds[0]      = rhs.m_distPreds[0] ;
   m_distPreds[1]      = rhs.m_distPreds[1] ;
   m_distPredErrsqs[0] = rhs.m_distPredErrsqs[0] ;
   m_distPredErrsqs[1] = rhs.m_distPredErrsqs[1] ;
}

HICalibratedHit::~HICalibratedHit()
{
}

//
// assignment operators
//
const HICalibratedHit& HICalibratedHit::operator=( const HICalibratedHit& rhs )
{
  if( this != &rhs )
  {
     m_useInFit                = rhs.m_useInFit ;
     m_infoOnTrack             = rhs.m_infoOnTrack ;
     m_residuals[0]            = rhs.m_residuals[0] ;
     m_residuals[1]            = rhs.m_residuals[1] ;
     m_errsqs[0]               = rhs.m_errsqs[0] ;
     m_errsqs[1]               = rhs.m_errsqs[1] ;
     m_distPreds[0]            = rhs.m_distPreds[0] ;
     m_distPreds[1]            = rhs.m_distPreds[1] ;
     m_distPredErrsqs[0]       = rhs.m_distPredErrsqs[0] ;
     m_distPredErrsqs[1]       = rhs.m_distPredErrsqs[1] ;
     m_nResiduals              = rhs.m_nResiduals ;
     m_smoothedResidual        = rhs.m_smoothedResidual ;
     m_smoothedErrsq           = rhs.m_smoothedErrsq ;
     m_smoothedDistPred        = rhs.m_smoothedDistPred ;
     m_smoothedDistPredErrsq   = rhs.m_smoothedDistPredErrsq ;
     m_residualsSmoothed       = rhs.m_residualsSmoothed ;
     m_fittingWeightCorrected  = rhs.m_fittingWeightCorrected ;
     m_correctedFittingWeight  = rhs.m_correctedFittingWeight ;
  }

  return *this;
}

//
// member functions
//

void
HICalibratedHit::addResidualAndErrorSquared( const Meters& aResidual,
					     const Meters& aErrorSquared,
					     const Meters& aDistPred,
					     const Meters& aDistPredErrsq )
{
   if( m_nResiduals <= 2 )
   {
      m_residuals[ m_nResiduals ] = aResidual ;
      m_errsqs[ m_nResiduals ]    = aErrorSquared ;
      m_distPreds[ m_nResiduals ] = aDistPred ;
      m_distPredErrsqs[ m_nResiduals ] = aDistPredErrsq ;

      ++m_nResiduals ;
   }
}

void
HICalibratedHit::clear()
{
   m_residuals[0] = 0. ;
   m_residuals[1] = 0. ;
   m_errsqs[0]    = 0. ;
   m_errsqs[1]    = 0. ;
   m_distPreds[0] = 0. ;
   m_distPreds[1] = 0. ;
   m_distPredErrsqs[0] = 0. ;
   m_distPredErrsqs[1] = 0. ;
   m_nResiduals   = 0 ;
   m_smoothedResidual = 0. ;
   m_smoothedErrsq = 0. ;
   m_smoothedDistPred = 0. ;
   m_smoothedDistPredErrsq = 0. ;
   m_residualsSmoothed = false ;
//   m_fittingWeightCorrected = false ;
//   m_correctedFittingWeight = 0. ;
}

void
HICalibratedHit::calculateSmoothedResidualAndError()
{
   if( m_nResiduals < 1 || m_nResiduals > 2 )
   {
      m_smoothedResidual = 0. ;
      m_smoothedErrsq = 0. ;
      m_smoothedDistPred = 0. ;
      m_smoothedDistPredErrsq = 0. ;
      m_residualsSmoothed = false ;
      return ;
   }
   else if( m_nResiduals == 1 )
   {
      m_smoothedResidual = m_residuals[0] ;
      m_smoothedErrsq = m_errsqs[0] ;
      m_smoothedDistPred = m_distPreds[0] ;
      m_smoothedDistPredErrsq = m_distPredErrsqs[0] ;
      m_residualsSmoothed = true ;
      return ;
   }

   // Take naive weighted average (accounting for correlation due to the
   // measured distance) without rotating into the uncorrelated basis.

//    double covar2 = covar * covar ;
//    double errsqDiff = m_errsqs[1] - m_errsqs[0] ;

//    double residDiffScale = sqrt( errsqDiff * errsqDiff + 4. * covar2 ) ;
//    double eigMinusErrsq  = 0.5 * ( errsqDiff + residDiffScale ) ;

//    double denom = sqrt( covar2 + eigMinusErrsq * eigMinusErrsq ) ;
//    Meters inResidUncorr =
//       ( covar * m_residuals[0] + eigMinusErrsq * m_residuals[1] ) / denom ;
//    Meters outResidUncorr =
//       ( covar * m_residuals[1] - eigMinusErrsq * m_residuals[0] ) / denom ;

//    m_smoothedResidual =
//       ( inResidUncorr + outResidUncorr +
// 	residDiffScale * ( outResidUncorr - inResidUncorr ) / errsqSum ) / 2. ;

//    m_smoothedErrsq =
//       ( m_errsqs[0] * m_errsqs[1] - covar2 ) / errsqSum ;

   double covar  = measurementErrorSquared() ;
   double errsqSum = m_errsqs[0] + m_errsqs[1] ;
   double distPredErrsqSum = m_distPredErrsqs[0] + m_distPredErrsqs[1] ;

   m_smoothedResidual =
      ( m_errsqs[0] * m_residuals[1] + m_errsqs[1] * m_residuals[0] )/errsqSum;
   m_smoothedErrsq = m_errsqs[0] * m_errsqs[1] / ( errsqSum * errsqSum ) *
      ( m_errsqs[0] + 2. * covar + m_errsqs[1] ) ;

   // Predicted distances are uncorrelated because they have no overlapping
   // hits.

   m_smoothedDistPred = 
      ( m_distPredErrsqs[0] * m_distPreds[1] +
	m_distPredErrsqs[1] * m_distPreds[0] ) / distPredErrsqSum ;
   m_smoothedDistPredErrsq = 
      m_distPredErrsqs[0] * m_distPredErrsqs[1] / distPredErrsqSum ;

   m_residualsSmoothed = true ;
}


//
// const member functions
//

Meters
HICalibratedHit::smoothedResidualError() const
{
   return sqrt( m_smoothedErrsq ) ;
}

Meters
HICalibratedHit::smoothedDistPredError() const
{
   return sqrt( m_smoothedDistPredErrsq ) ;
}

//
// static member functions
//
