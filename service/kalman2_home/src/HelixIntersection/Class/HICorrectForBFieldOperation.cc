// -*- C++ -*-
//
// Package:     <HelixIntersection>
// Module:      HICorrectForBFieldOperation
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
// This class was created in order to handle the non-uniformities introduced
// by the superconducting quadrupole magnets.  We treat effects of the 
// non-uniform B-field as a perturbation which is added on after MS and dedx
// operations
//
//
// Author:      Lauren Hsu
// Created:     Thu Dec 13 16:58:29 EST 2001
// $Id: HICorrectForBFieldOperation.cc,v 1.4 2002/03/09 20:48:15 llh14 Exp $
//
// Revision history
//
// $Log: HICorrectForBFieldOperation.cc,v $
// Revision 1.4  2002/03/09 20:48:15  llh14
// Added if statement to bypass B field calc if the non-uniform field is very small
//
// Revision 1.3  2002/02/18 23:00:21  wsun
// Apply corrections to aHelixAfterOperation, not aHelixBeforeOperation.
//
// Revision 1.2  2002/02/07 19:47:05  llh14
// Removed a debugging comment from HICorrectForBfieldOperation
//
// Revision 1.1  2002/01/23 22:17:39  llh14
// Added HICorrectForBFieldOperation, and small modifications to HISingleWireCylinder to incorporate the quad fields
//

#include "Experiment/Experiment.h"

// system include files
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
#include <string>
#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

// user include files
#include "Experiment/report.h"

#include "HelixIntersection/HICorrectForBFieldOperation.h"
#include "HelixIntersection/HIHelix.h"
#include "HelixIntersection/HIIntersectionSurface.h"
#include "HelixIntersection/HIMagField.h"

#include "CalibratedData/CalibratedHit.h"

#include "CLHEP/Vector/ThreeVector.h"
//
// constants, enums and typedefs
//

static const char* const kFacilityString = "package.HICorrectForBFieldOperation" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HICorrectForBFieldOperation.cc,v 1.4 2002/03/09 20:48:15 llh14 Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
HICorrectForBFieldOperation::HICorrectForBFieldOperation(
                             DABoolean aApplyBFieldCorrection) 
   : m_applyBFieldCorrection( aApplyBFieldCorrection )
{

}

// HICorrectForBFieldOperation::HICorrectForBFieldOperation( const HICorrectForBFieldOperation& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HICorrectForBFieldOperation::~HICorrectForBFieldOperation()
{
}

DABoolean
HICorrectForBFieldOperation::perform( HIHelix& aHelixAfterOperation,
				      const HIHelix& aHelixBeforeOperation,
				      const HIHelix& aHelixAtLastIntersection,
				      HIIntersectionSurface& aCurrentSurface,
				      const HIIntersectionSurface& aLastSurface ) const
{
   //~~~~~~ First we apply the corrections to the track paramters ~~~~~~~~~~~

   //The distance traversed
   double arcLength = aHelixBeforeOperation.lastArcLength();
   
   //If distance traversed is 0 then do nothing 
   //this is so for the other operations
   if (arcLength == 0)
   {
      return false;
   }   
   
   //We want the value of the B field in the middle of the swim interval
   HepPoint3D positionBeforeSwim = aHelixAtLastIntersection.position();
   HepPoint3D positionAfterSwim = aHelixBeforeOperation.position();
   HepPoint3D avgPosition = (1/2.0)*(positionBeforeSwim + positionAfterSwim);

   Hep3Vector deltaB = HIMagField::instance()->deltab( avgPosition );
   Hep3Vector defaultB = HIMagField::instance()->bfield( avgPosition );
   
   //Add this if statement to speed up the code!
   //Should we be hardcoding this parameter?
   double deltaBOBCut = 0.001;
   if( deltaB.mag()/defaultB.mag() < deltaBOBCut )
   { 
      return true;
   }

   //We need to find the change in position:
   Hep3Vector deltaX = positionAfterSwim - positionBeforeSwim;
      
   //The relevant track parameters
   double k = aHelixBeforeOperation.curvature();
   double phi0 = aHelixBeforeOperation.phi0();
   double t = aHelixBeforeOperation.cotTheta();
   double charge = k/abs(k);
   //CBX says A = 0.2248425 GeV/mc = cB/2 for a 1.5 T field
   double a = (0.2248425*defaultB.mag())/15.0;

   //Change in momentum from perturbative B field
   Hep3Vector deltaP = 0.0299792458*charge*(deltaX.cross(deltaB));

   //The corrections
   double deltaPhi0 = (abs(k)/a)*(deltaP.y()*cos(phi0) - deltaP.x()*sin(phi0));
   double deltaK = -(abs(k)*k/a)*(deltaP.x()*cos(phi0) + deltaP.y()*sin(phi0));
   double deltat = deltaP.z()*(abs(k)/a) + t*deltaK/k;

   //Changing the parameters
//    aHelixAfterOperation.setCurvature( (k + deltaK));
//    aHelixAfterOperation.setPhi0( (phi0 + deltaPhi0));
//    aHelixAfterOperation.setCotTheta( (t + deltat) );
   aHelixAfterOperation.setCurvature(
      (aHelixAfterOperation.curvature() + deltaK));
   aHelixAfterOperation.setPhi0(
      (aHelixAfterOperation.phi0() + deltaPhi0));
   aHelixAfterOperation.setCotTheta(
      (aHelixAfterOperation.cotTheta() + deltat) );

   //~~~~~~~~~~~ Now Apply Corrections to the error Matrices ~~~~~~~~~~~~~~~   
   HepSymMatrix errorMatrix = aHelixAfterOperation.errorMatrix();
   HepSymMatrix* otherErr = aHelixAfterOperation.otherErrorMatrix();
   
   //Construct the transport matrix
   HepMatrix transportMatrix( HIHelix::kNTrackParameters, HIHelix::kNTrackParameters, 1);

   //Corrections due to mag field:
   transportMatrix( HIHelix::kCurvature, HIHelix::kCurvature) += (2/k)*deltaK;
   transportMatrix( HIHelix::kCurvature, HIHelix::kPhi0) += -k*deltaPhi0;

   transportMatrix( HIHelix::kPhi0, HIHelix::kCurvature) += deltaPhi0/k;
   transportMatrix( HIHelix::kPhi0, HIHelix::kPhi0) += deltaK/k;

   transportMatrix( HIHelix::kCotTheta, HIHelix::kCurvature) += deltat/k;
   transportMatrix( HIHelix::kCotTheta, HIHelix::kPhi0) += -t*deltaPhi0;
   transportMatrix( HIHelix::kCotTheta, HIHelix::kCotTheta) += deltaK/k;

   HepMatrix errorMatrix_Corr = transportMatrix*errorMatrix*(transportMatrix.T());
   HepMatrix otherErr_Corr;
   
   if( otherErr != 0)
   {
      otherErr_Corr = transportMatrix*(*otherErr)*(transportMatrix.T());
   }

   //We have to do this because we have to force the matrix to be symmetric
   //and doing both the error matrix and the otherErr at the same time
   Count row = 1;
   Count col = 1;
   HepSymMatrix copyOfErrorMatrix(HIHelix::kNTrackParameters, 0);  
   HepSymMatrix copyOfOtherMatrix(HIHelix::kNTrackParameters, 0);  

   for( ; row <= HIHelix::kNTrackParameters; row++)
   {
      for( ; col <= row; col++)
      {
	 copyOfErrorMatrix(row, col) = errorMatrix_Corr(row, col);
	 if( otherErr != 0)
	 { copyOfOtherMatrix(row, col) = otherErr_Corr(row, col); }
      }

      col = 1;
   }
   
   //Change the values of the "otherErrMatrix"
   aHelixAfterOperation.setErrorMatrix(copyOfErrorMatrix);

   if( otherErr != 0)
   {
      *otherErr = copyOfOtherMatrix;
   }

   return true;

}

//
// assignment operators
//
// const HICorrectForBFieldOperation& HICorrectForBFieldOperation::operator=( const HICorrectForBFieldOperation& rhs )
// {
//   if( this != &rhs ) {
//      // do actual copying here, plus:
//      // "SuperClass"::operator=( rhs );
//   }
//
//   return *this;
// }

//
// member functions
//

//
// const member functions
//

//
// static member functions
//







