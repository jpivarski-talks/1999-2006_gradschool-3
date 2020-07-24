// -*- C++ -*-
//
// Package:     ADRGeom
// Module:      ADRCathodePadInfo
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:     Inga Karliner
// Created:    May 14, 1999
//
// Revision history
// First submission
//

#include "Experiment/Experiment.h"

// system include files
#include "C++Std/iomanip.h"
#include <assert.h> 
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "ADRGeom/ADRCathodePadInfo.h"

// STL classes

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "ADRGeom.ADRCathodePadInfo" ;
 
//
// static data member definitions
//

//
// constructors and destructor
//
ADRCathodePadInfo::ADRCathodePadInfo() :
  m_radius(0),
  m_padPhiMin(0),
  m_padPhiDelta(0),
  m_padHalfLengthZ(0)
{
}

ADRCathodePadInfo::ADRCathodePadInfo(
		    const HepPoint3D& iPoint0,
		    const HepPoint3D& iCornerEastPhiMin,
		    const HepPoint3D& iCornerEastPhiMax,
		    const HepPoint3D& iCornerWestPhiMin,
		    const HepPoint3D& iCornerWestPhiMax,
		    double iradius,
		    double iPhiMin,
		    double iPhiDelta,
		    double iHalfLengthZ) :
   m_point0(iPoint0),
   m_cornerEastPhiMin(iCornerEastPhiMin),
   m_cornerEastPhiMax(iCornerEastPhiMax),
   m_cornerWestPhiMin(iCornerWestPhiMin),
   m_cornerWestPhiMax(iCornerWestPhiMax),
   m_radius(iradius),
   m_padPhiMin(iPhiMin),
   m_padPhiDelta(iPhiDelta),
   m_padHalfLengthZ(iHalfLengthZ)
{
}

// ADRCathodePadInfo::ADRCathodePadInfo( const ADRCathodePadInfo& )
// {
// }

ADRCathodePadInfo::~ADRCathodePadInfo()
{
}

//
// assignment operators
//x
// const ADRCathodePadInfo& ADRCathodePadInfo::operator=( const ADRCathodePadInfo& )
// {
// }

//
// member functions
//
void 
ADRCathodePadInfo::setPoint0( const HepPoint3D& iPoint0 )
{
   m_point0 = iPoint0;
}

void 
ADRCathodePadInfo::setCornerEastPhiMin(const HepPoint3D& iCornerEastPhiMin)
{
     m_cornerEastPhiMin=iCornerEastPhiMin;
}

void 
ADRCathodePadInfo::setCornerEastPhiMax(const HepPoint3D& iCornerEastPhiMax)
{
     m_cornerEastPhiMax=iCornerEastPhiMax;
}
void 
ADRCathodePadInfo::setCornerWestPhiMin(const HepPoint3D& iCornerWestPhiMin)
{
     m_cornerWestPhiMin=iCornerWestPhiMin;
}

void 
ADRCathodePadInfo::setCornerWestPhiMax(const HepPoint3D& iCornerWestPhiMax)
{
     m_cornerWestPhiMax=iCornerWestPhiMax;
}
void 
ADRCathodePadInfo::setRadius(const double iradius)
{
  m_radius= iradius;
}
void 
ADRCathodePadInfo::setLocalPadPhiMin(const double iPhiMin)
{
  m_padPhiMin=iPhiMin;
}

void 
ADRCathodePadInfo::setPadPhiDelta(const double iPhiDelta)
{
  m_padPhiDelta=  iPhiDelta;
} 

void 
ADRCathodePadInfo::setPadHalfLengthZ(const double iHalfLengthZ)
{
  m_padHalfLengthZ= iHalfLengthZ;
}

double  
ADRCathodePadInfo::padGlobalPhiMin() const
{
  double phiMinEast = atan2(cornerEastPhiMin().y(),cornerEastPhiMin().x());
  double phiMinWest = atan2(cornerWestPhiMin().y(),cornerWestPhiMin().x());
  //project difference between two points on the phi unit vector:
  //(eastcorner - westcorner)*(phiunit(eastcorner))
  double difTimesPhi= -sin(phiMinEast)*(cos(phiMinEast)-cos(phiMinWest))
                      +cos(phiMinEast)*(sin(phiMinEast)-sin(phiMinWest));
  if(difTimesPhi< 0)
    { return phiMinEast;}
  else
    { return phiMinWest;}
}

double  
ADRCathodePadInfo::padGlobalPhiMax() const
{
  double phiMaxEast = atan2(cornerEastPhiMax().y(),cornerEastPhiMax().x());
  double phiMaxWest = atan2(cornerWestPhiMax().y(),cornerWestPhiMax().x());
  //project difference between two points on the phi unit vector:
  //(eastcorner - westcorner)*(phiunit(eastcorner))
  double difTimesPhi= -sin(phiMaxEast)*(cos(phiMaxEast)-cos(phiMaxWest))
            +cos(phiMaxEast)*(sin(phiMaxEast)-sin(phiMaxWest));
  if(difTimesPhi> 0)
    { return phiMaxEast;}
  else
    { return phiMaxWest;}
}

double  
ADRCathodePadInfo::padGlobalZMax() const
{
  double zMax;
  if(cornerWestPhiMax().z() > cornerWestPhiMin().z())
    {
      zMax =cornerWestPhiMax().z();
    }
  else
    {
      zMax =cornerWestPhiMin().z();
    }
  return zMax;
}
double  
ADRCathodePadInfo::padGlobalZMin() const
{
  double zMin;
  if(cornerEastPhiMax().z() < cornerEastPhiMin().z())
    {
      zMin =cornerEastPhiMax().z();
    }
  else
    {
      zMin =cornerEastPhiMin().z();
    }
  return zMin;
}

double ADRCathodePadInfo::padPhiMinAtZ(  DABoolean& success,double z) const
{
  double phi = 0;
  success  = false;
  double z1 = cornerEastPhiMin().z();
  double z2 = cornerWestPhiMin().z();
  // z in range? 
  if( (z>=z1) && (z<=z2))
    { 
      success = true;
    }
  else
    {
      return phi;
    }
  double phi1 = atan2(cornerEastPhiMin().y(),cornerEastPhiMin().x()); 
  double phi2 = atan2(cornerWestPhiMin().y(),cornerWestPhiMin().x());
  HepPoint3D EastPoint =  
    HepPoint3D(cornerEastPhiMin().x(),cornerEastPhiMin().y(),0);
  HepPoint3D WestPoint =  
    HepPoint3D(cornerWestPhiMin().x(),cornerWestPhiMin().y(),0); 
  double deltaphi = acos(
			 (cornerEastPhiMin().y()*cornerWestPhiMin().y()
			+cornerEastPhiMin().x()*cornerWestPhiMin().x())
                         /EastPoint.distance()/WestPoint.distance()) ;
  if (padGlobalPhiMin() == phi1)
    {
      return (phi1 + (z-z1)*(deltaphi)/(z2-z1));      
    }
  else
    { 
      return (phi2 + (z-z2)*(deltaphi)/(z1-z2));
    }
}
double ADRCathodePadInfo::padPhiMaxAtZ(  DABoolean& success,double z) const
{
  double phi = 0;
  success  = false;
  double z1 = cornerEastPhiMax().z();
  double z2 = cornerWestPhiMax().z();
  // z in range? 
  if( (z>=z1) && (z<=z2))
    { 
      success = true;
    }
  else
    {
      return phi;
    }
  double phi1 = atan2(cornerEastPhiMax().y(),cornerEastPhiMax().x()); 
  double phi2 = atan2(cornerWestPhiMax().y(),cornerWestPhiMax().x());
  HepPoint3D EastPoint =  
    HepPoint3D(cornerEastPhiMax().x(),cornerEastPhiMax().y(),0);
  HepPoint3D WestPoint =  
    HepPoint3D(cornerWestPhiMax().x(),cornerWestPhiMax().y(),0);
  double deltaphi = acos(
			 (cornerEastPhiMax().y()*cornerWestPhiMax().y()
			  +cornerEastPhiMax().x()*cornerWestPhiMax().x())
			 /EastPoint.distance()/WestPoint.distance());
  if (padGlobalPhiMax() == phi1)
    { 
      return (phi2 + (z-z2)*(deltaphi)/(z1-z2));      
    }
  else
    { 
      return (phi1 + (z-z1)*(deltaphi)/(z2-z1));
    }
}
double ADRCathodePadInfo::padZMinAtPhi(  DABoolean& success,double phi) const
{ 
  double z = -999.;
  success  = false;
  double z1 = cornerEastPhiMin().z();
  double z2 = cornerEastPhiMax().z();
  double phiMinEast = atan2(cornerEastPhiMin().y(),cornerEastPhiMin().x());
  double phiMaxEast = atan2(cornerEastPhiMax().y(),cornerEastPhiMax().x());

  //phi in range?
  double difTimesPhi= -sin(phiMinEast)*(-cos(phiMinEast)+cos(phi))
                      +cos(phiMinEast)*(-sin(phiMinEast)+sin(phi));
  if(difTimesPhi<0)
    {//phi smaller than phimin
      return z;
    } 
  difTimesPhi= -sin(phi)*(-cos(phi)+cos(phiMaxEast))
               +cos(phi)*(-sin(phi)+sin(phiMaxEast));
  if(difTimesPhi<0)
    {//phi larger than phimax
      return z;
    } 
  HepPoint3D MinPoint = 
    HepPoint3D(cornerEastPhiMin().x(),cornerEastPhiMin().y(),0);
  HepPoint3D MaxPoint = 
    HepPoint3D(cornerEastPhiMax().x(),cornerEastPhiMax().y(),0); 
  double deltaphi = acos(
			 (cornerEastPhiMax().y()*cornerEastPhiMin().y()
			  +cornerEastPhiMax().x()*cornerEastPhiMin().x())
			 /MinPoint.distance()/MaxPoint.distance());
    
  if(phi<phiMinEast) {phi = phi + 2*M_PI;}
  return ( z1 + (phi - phiMinEast)*(z2-z1)/deltaphi) ;
   
}
double ADRCathodePadInfo::padZMaxAtPhi(  DABoolean& success,double phi) const
{
  double z = -999.;
  success  = false;
  double z1 = cornerWestPhiMin().z();
  double z2 = cornerWestPhiMax().z();
  double phiMinWest = atan2(cornerWestPhiMin().y(),cornerWestPhiMin().x());
  double phiMaxWest = atan2(cornerWestPhiMax().y(),cornerWestPhiMax().x());

  //phi in range?
  double difTimesPhi= -sin(phiMinWest)*(-cos(phiMinWest)+cos(phi))
                      +cos(phiMinWest)*(-sin(phiMinWest)+sin(phi));
  if(difTimesPhi<0)
    {
      return z;
    } 
  difTimesPhi= -sin(phi)*(-cos(phi)+cos(phiMaxWest))
               +cos(phi)*(-sin(phi)+sin(phiMaxWest));
  if(difTimesPhi<0)
    {
      return z;
    } 

  HepPoint3D MinPoint = 
    HepPoint3D(cornerWestPhiMin().x(),cornerWestPhiMin().y(),0);
  HepPoint3D MaxPoint = 
    HepPoint3D(cornerWestPhiMax().x(),cornerWestPhiMax().y(),0);
  double deltaphi = acos(
			 (cornerWestPhiMax().y()*cornerWestPhiMin().y()
			  +cornerWestPhiMax().x()*cornerWestPhiMin().x())
			 /MinPoint.distance()/MaxPoint.distance());
  if(phi<phiMinWest) {phi = phi + 2*M_PI;}
  return ( z1 + (phi - phiMinWest)*(z2-z1)/deltaphi) ;

}


/*double 
  ADRCathodePadInfo::pointDistance(const HepPoint3D& iPoint) const
  {  
  //  Return negative distance if z outside pad range. 
  //  Distance from point (xP,yP,zP), to pad  
  return distance;
  }
  */

//
// const member functions
//

//
// static member functions
//











