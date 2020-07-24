// -*- C++ -*-
//
// Package:     <package>
// Module:      DRCAGuru
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Dan Cronin-Hennessy
// Created:     Wed Feb  2 14:57:58 EST 2000
// $Id: DRCAGuru.cc,v 1.2 2000/04/12 13:14:40 hennessy Exp $
//
// Revision history
//
// $Log: DRCAGuru.cc,v $
// Revision 1.2  2000/04/12 13:14:40  hennessy
// put in limits for CellId in cathodes (1 to 1536)
//
// Revision 1.1  2000/03/24 01:28:46  hennessy
// utility object for cathode simulation
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
//#include <math>

#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
//#include "ADRGeomProd/ADRCathodePadInfo.h"
#include "DRMCUtil/DRCAGuru.h"

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

static const char* const kFacilityString = "package.DRCAGuru" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DRCAGuru.cc,v 1.2 2000/04/12 13:14:40 hennessy Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
DRCAGuru::DRCAGuru()
{
}

// DRCAGuru::DRCAGuru( const DRCAGuru& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

DRCAGuru::~DRCAGuru()
{
}

//
// assignment operators
//
// const DRCAGuru& DRCAGuru::operator=( const DRCAGuru& rhs )
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
int DRCAGuru::whatIsClosestPadID(float effZposition, float drPhi)
{
   int iPanel=0;
   int iZ;
   int IDClosest=0;

   iPanel = drPhi/.7854;
   iZ=(effZposition+1.056)/.011;
   IDClosest= iPanel*192+iZ;
   if(IDClosest>1536){IDClosest=1536;}
   if(IDClosest<1){IDClosest=1;}

   return IDClosest;
}
int DRCAGuru::whatIsUpperLimitID(int closestPadID)
{
  int iTemp=0;
  int iToHi=0;
  int iPanel=0;
  int iInPanel=0;


  iPanel=closestPadID/192;
  iInPanel=closestPadID-iPanel*192;
  iToHi=192-iInPanel;
  if(iToHi>10){iToHi=10;}
  iTemp=closestPadID+iToHi;
  if(iTemp>1536){iTemp=1536;}
  if(iTemp<1){iTemp=1;}

// Take care of overun of panel.
 
   
     return iTemp;
}
int DRCAGuru::whatIsLowerLimitID(int closestPadID)
{

// Take care of overun of panel.
  int iTemp=0;
  int iToLo=0;
  int iPanel=0;
  int iInPanel=0;


  iPanel=closestPadID/192;
  iInPanel=closestPadID-iPanel*192;
  iToLo=iInPanel-1;
  if(iToLo>10){iToLo=10;}
  iTemp=closestPadID-iToLo;
  if(iTemp>1536){iTemp=1536;}
  if(iTemp<1){iTemp=1;}

 
     
     return iTemp;
}

float DRCAGuru::whatIsWidthOfQDist(int closestPadID)
{

  float a=.5;
  
  return a;
}


float DRCAGuru::whatIsZPosition(int PadID)
{
  int iTemp=0;
  int iInPanel=0;
  float zPos=0.;
  iInPanel=PadID-(PadID/192)*192;
  zPos=iInPanel;
  zPos=zPos*.011-1.056;
  
// Take care of overun of panel.
  
  return zPos;
}


float DRCAGuru::whatIsQDeposited(int PadID, int iCenter, float width, float charge, float dZ)
{
  float Q=0.;
  float xl=0;
  float xh=0;

// Integration limits
  xh=((float) PadID + .5 - (float) iCenter + dZ/.011)/(1.414*width);
  xl=((float) PadID - .5 - (float) iCenter + dZ/.011)/(1.414*width);

// Integral over pad 
  Q=(.5*(erf(xh)-erf(xl)))*charge ;
   
   
//  Q=charge*(erf(xp   
//  expon=-(Diff*Diff)/(2*width*width);
//  Norm=charge/(2.51*width);

   
//  Q=Norm*exp(expon);
  
  return Q;
}



//
// const member functions
//

//
// static member functions
//
