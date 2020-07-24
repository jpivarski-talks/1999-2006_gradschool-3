// -*- C++ -*-
//
// Package:     <geometryForFortran>
// Module:      fillFortranDRGeometry
// 
// Description: Fill the SV geometry.
//
// Implementation:
//     <Notes on implementation>
//
// Author:      
// Created:     Tue May  4 10:04:08 EDT 1999
// $Id: fillFortranSVGeometry.cc,v 1.3 2000/04/05 16:54:06 pg Exp $
//
//

#include "Experiment/Experiment.h"
#include "Experiment/types.h"
#include "Experiment/fortran_types.h"


// system include files
#include <stdlib.h>   // for ::exit
#include <assert.h>
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
#include "geometryForFortran/geometryForFortran.h"
#include "ASiStorePro/ASiLadder.h"
#include "ASiStorePro/ASiSensorAddress.h"
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

static const char* const kFacilityString = 
                      "geometryForFortran.fillFortranSVGeometry" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: fillFortranSVGeometry.cc,v 1.3 2000/04/05 16:54:06 pg Exp $";
static const char* const kTagString = "$Name:  $";

extern "C" {
   void gff_fillsigeom_(FInteger*, FReal*, FReal*, FReal*, FReal*, 
			FReal*, FReal*, FReal*, FReal*, FReal*, FReal*,
			FReal*,FReal*);
   void gff_fillsigeomglobal_();
			
}

//////////////////////////////////////////////////////////////////////
// Fill the appropriate Fortran common blocks with DR geometry information
//
void fillFortranSVGeometry(const ASiStore& gsi)
{
// First, fill all global parameters: radii, hybrid numbers, pitch etc.
   gff_fillsigeomglobal_();
// Now, deal with hybrid boundaries etc,
   HepVector3D unitX(1.,0.,0.);
   HepVector3D unitY(0.,1.,0.);
   // Silicon R-PHI
   for (int i = 1; i <= ASiSensorAddress::kAdNLadders; ++i)
   {  
      FInteger hybrid_w = i;
      const ASiLadder& ladder = gsi.ladder(i);
      float halfZ = ladder.halfLengthZ();
      float halfX = ladder.halfLengthX();

      float cosX = ladder.unitVectorZInSi().x();
      float cosY = ladder.unitVectorZInSi().y();

// Do the West side first, Positive Z
      
      float farZ_w = ladder.coorInSi( HepPoint3D(0.,0., halfZ)).z();
      float closeZ_w = ladder.coorInSi( HepPoint3D(0.,0.,0.)).z();
// Strip 0, sort of...       
      float x0_w = ladder.coorInSi( HepPoint3D(-halfX,0., 0.5*halfZ)).x();
      float y0_w = ladder.coorInSi( HepPoint3D(-halfX,0., 0.5*halfZ)).y();
      float phi0_w = ladder.coorInSi( HepPoint3D(-halfX,0., 0.5*halfZ)).phi();
      phi0_w = (phi0_w >= 0.) ? phi0_w : 2*M_PI + phi0_w ;  
      float r0_w = ladder.coorInSi( HepPoint3D(-halfX,0., 0.5*halfZ)).perp();

// Strip 510, sort of...       
      float x511_w = ladder.coorInSi( HepPoint3D(halfX,0., 0.5*halfZ)).x();
      float y511_w = ladder.coorInSi( HepPoint3D(halfX,0., 0.5*halfZ)).y();
      float phi511_w = ladder.coorInSi( HepPoint3D(halfX,0., 0.5*halfZ)).phi();
      phi511_w = (phi511_w >= 0.) ? phi511_w : 2*M_PI + phi511_w ;  
      float r511_w = ladder.coorInSi( HepPoint3D(halfX,0., 0.5*halfZ)).perp();
      
      gff_fillsigeom_(&hybrid_w, &x0_w, &x511_w, &y0_w, &y511_w, &r0_w, 
		      &r511_w, &phi0_w, &phi511_w, &farZ_w, &closeZ_w,
		      &cosX, &cosY);

// Now the East side , negative Z
      FInteger hybrid_e = i + ASiSensorAddress::kAdNLadders;
      
      float farZ_e = ladder.coorInSi( HepPoint3D(0.,0., -halfZ)).z();
      float closeZ_e = ladder.coorInSi( HepPoint3D(0.,0.,0.)).z();
// Strip 0, sort of...       
      float x0_e = ladder.coorInSi( HepPoint3D(-halfX,0., -0.5*halfZ)).x();
      float y0_e = ladder.coorInSi( HepPoint3D(-halfX,0., -0.5*halfZ)).y();
      float phi0_e = ladder.coorInSi( HepPoint3D(-halfX,0., -0.5*halfZ)).phi();
      phi0_e = (phi0_e >= 0.) ? phi0_e : 2*M_PI + phi0_e ;  
      float r0_e = ladder.coorInSi( HepPoint3D(-halfX,0., -0.5*halfZ)).perp();

// Strip 510, sort of...       
      float x511_e = ladder.coorInSi(HepPoint3D(halfX,0., -0.5*halfZ)).x();
      float y511_e = ladder.coorInSi(HepPoint3D(halfX,0., -0.5*halfZ)).y();
      float phi511_e = ladder.coorInSi(HepPoint3D(halfX,0., -0.5*halfZ)).phi();
      phi511_e = (phi511_e >= 0.) ? phi511_e : 2*M_PI + phi511_e ;  
      float r511_e = ladder.coorInSi(HepPoint3D(halfX,0., -0.5*halfZ)).perp();
      
      gff_fillsigeom_(&hybrid_e, &x0_e, &x511_e, &y0_e, &y511_e, &r0_e, 
		      &r511_e, &phi0_e, &phi511_e, &farZ_e, &closeZ_e,
		      &cosX, &cosY);
   }

}

