// -*- C++ -*-
//
// Package:     < ADRGeom >
// Module:      ADRCathodePanel
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Inga Karliner
// Created:     May 9, 1999
//
// Revision history 
// Revision 9/15/99 Inga Karliner
//  Initialize m_pads
// Revision 12/11/00 I.Karliner 
//  Include survey measurements in panel placement (see ADRCathodeParams)
//
// First submission
//
#include "Experiment/Experiment.h"

// system include files
#include "C++Std/iomanip.h" 
#include "C++Std/fstream.h"
#include <assert.h>
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "DetectorGeometry/DGConstLabVolumePtr.h"
#include "DetectorGeometry/DGLabVolume.h"
#include "ADRGeom/ADRCathodePanel.h"
#include "ADRGeom/ADRCathodeParams.h"
#include "Experiment/units.h"
#include <math.h> 

// STL classes

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "ADRGeom.ADRCathodePanel" ;

//
// static data member definitions
//

//
// constructors and destructor
//
//   
// 
ADRCathodePanel::ADRCathodePanel() :
  m_numberOfPads(0),
  m_panelRadius(0),
  m_panelPhiMin(0),
  m_panelPhiDelta(0),
  m_panelHalfLengthZ(0),
  m_pads(0)
{
}

ADRCathodePanel::ADRCathodePanel( 
                    unsigned int iNumberOfPads,
		    const DGConstLabVolumePtr& iCathodePanel,
		    const HepPoint3D& iPoint0,
		    const HepPoint3D& iCornerEastPhiMin,
		    const HepPoint3D& iCornerEastPhiMax,
		    const HepPoint3D& iCornerWestPhiMin,
		    const HepPoint3D& iCornerWestPhiMax,
		    double iPanelRadius,
		    double iPhiMin,
		    double iPhiDelta,
		    double iHalfLengthZ,
		    double iZOffset,
		    double iPhiOffset,
		    const HepPoint3D& iCenterOffset) :
   m_numberOfPads( iNumberOfPads),
   m_volume( iCathodePanel),
   m_point0(iPoint0),
   m_cornerEastPhiMin(iCornerEastPhiMin),
   m_cornerEastPhiMax(iCornerEastPhiMax),
   m_cornerWestPhiMin(iCornerWestPhiMin),
   m_cornerWestPhiMax(iCornerWestPhiMin),
   m_panelRadius( iPanelRadius),
   m_panelPhiMin( iPhiMin),
   m_panelPhiDelta( iPhiDelta),
   m_panelHalfLengthZ( iHalfLengthZ),
   m_panelZOffset(iZOffset),
   m_panelPhiOffset(iPhiOffset),
   m_panelOffsetCenter(iCenterOffset),
   m_pads(0)
{
}

// ADRCathodePanel::ADRCathodePanel( const ADRCathodePanel& )
// {
// }

ADRCathodePanel::~ADRCathodePanel()
{
    delete [] m_pads;
}

//
// assignment operators
//
// const ADRCathodePanel& ADRCathodePanel::operator=( const ADRCathodePanel& )
// {
// }

//
// member functions
//
void
ADRCathodePanel::setNumberOfPads( unsigned int iNumberOfPads )
{
   m_numberOfPads = iNumberOfPads;
}
void
ADRCathodePanel::setVolume( const DGConstLabVolumePtr& iVolume )
{
   m_volume = iVolume;
}
void ADRCathodePanel::setPanelRadius(const double iRadius)
{
  m_panelRadius = iRadius ;
}
void ADRCathodePanel::setLocalPhiMin(const double iPhiMin)
{
  m_panelPhiMin= iPhiMin;
}
void ADRCathodePanel::setPhiDelta(const double iPhiDelta)
{
  m_panelPhiDelta= iPhiDelta; 
}
void ADRCathodePanel::setHalfLengthZ(const double iHalfLengthZ)
{
  m_panelHalfLengthZ = iHalfLengthZ;
}
void ADRCathodePanel::setPanelZOffset(const double iZOffset)
{
 m_panelZOffset = iZOffset;
}
void ADRCathodePanel::setPanelPhiOffset(const double iPhiOffset)
{
 m_panelPhiOffset = iPhiOffset;
}
void ADRCathodePanel::setPanelOffsetCenter(const HepPoint3D&  iOffsetCenter)
{
 m_panelOffsetCenter = iOffsetCenter;
}
void 
ADRCathodePanel::setPoint0(const HepPoint3D& iPoint) 
{
  m_point0 = iPoint;
}
void 
ADRCathodePanel::setCornerEastPhiMin(const HepPoint3D& iCornerEastPhiMin)
{ 
  m_cornerEastPhiMin= iCornerEastPhiMin;
}
void 
ADRCathodePanel::setCornerEastPhiMax(const HepPoint3D& iCornerEastPhiMax)
{ 
  m_cornerEastPhiMax=iCornerEastPhiMax;
}
void 
ADRCathodePanel::setCornerWestPhiMin(const HepPoint3D& iCornerWestPhiMin)
{
  m_cornerWestPhiMin= iCornerWestPhiMin;
}
void 
ADRCathodePanel::setCornerWestPhiMax(const HepPoint3D& iCornerWestPhiMax)
{
  m_cornerWestPhiMax= iCornerWestPhiMax;
}

const ADRCathodePadInfo& 
ADRCathodePanel::ADRCathodePad( unsigned int iPadID ) const
{
assert (numberOfPads() > 0 );
assert (iPadID <numberOfPads());
return m_pads[iPadID - kFirstPad];
}

ADRCathodePadInfo 
ADRCathodePanel::ADRCathodePadMk( unsigned int iPadID ) const
{
assert (numberOfPads() > 0 );
assert (iPadID <numberOfPads());
//pad 0 = Eastmost pad. 
// first pad# = 0. Panel center is half way between pad 95 and 96 
// + zOffset()
// Local coordinates:
  double zpadoffset0 = ((int(iPadID) - (int(numberOfPads())-1.)/2))
    * ADRCathodeParams::kADRCathodePitch; 
  double zpadoffset= zpadoffset0  + zOffset();
  double zhalf   = ADRCathodeParams::kADRCathodePadLength/2;
 
  /*  cout<< endl<< "Making pad "<<setw(5)<<iPadID<<endl      
      << "zoffset="<<setw(10)<< zoffset<<endl
      << "zhalf=  "<<setw(10)<< zhalf  <<endl
      <<" radius= "<<panelRadius()<<endl
      <<" PhiMin  "<<localPhiMin()<<endl
      <<" PhiDelta"<<phiDelta()<<endl;
      */	
  HepPoint3D PadCenter0 = panelVolume()->localToLabAligned() *
             HepPoint3D(panelRadius(),0.,zpadoffset0) ; 
  HepPoint3D PadCenter = PadCenter0 + 
             panelVolume()->localToLabAligned() * m_panelOffsetCenter ;
/*  if(iPadID==0)     
    {
    cout<<"pad "<<iPadID<<" Pad Center Nominal" << PadCenter0<<endl
    <<"pad "<<iPadID<<" Pad Center Measurd" << PadCenter <<endl;
    }
    */
  HepPoint3D EPhiMin = panelVolume()->localToLabAligned() *
	 HepPoint3D(
	     panelRadius()*cos(localPhiMin()+phiOffset()),
	     panelRadius()*sin(localPhiMin()+phiOffset()),
	     zpadoffset - zhalf);
  HepPoint3D EPhiMax = panelVolume()->localToLabAligned() *
	 HepPoint3D(
	     panelRadius()*cos(localPhiMin()+phiDelta()+phiOffset()),
	     panelRadius()*sin(localPhiMin()+phiDelta()+phiOffset()),
	     zpadoffset - zhalf);
  HepPoint3D WPhiMin = panelVolume()->localToLabAligned() *
	 HepPoint3D(
	     panelRadius()*cos(localPhiMin()+phiOffset()),
	     panelRadius()*sin(localPhiMin()+phiOffset()),
	     zpadoffset + zhalf );
  HepPoint3D WPhiMax = panelVolume()->localToLabAligned() *
	 HepPoint3D(
	     panelRadius()*cos(localPhiMin()+phiDelta()+phiOffset()),
	     panelRadius()*sin(localPhiMin()+phiDelta()+phiOffset()),
	     zpadoffset + zhalf);
  return ADRCathodePadInfo(
			 PadCenter,
			 EPhiMin,
			 EPhiMax,
			 WPhiMin,		 
			 WPhiMax,
			 panelRadius(),
			 localPhiMin(),
			 phiDelta(),
			 zhalf);
}

void 
ADRCathodePanel::makePadsInPanel()
{
//         +---------------------------+
//         | Setup array of pads       |
//         +---------------------------+
  const unsigned int NumberOfPads = numberOfPads();
  m_pads = new ADRCathodePadInfo[NumberOfPads];
  for ( unsigned int itPad(0); itPad < numberOfPads(); ++itPad ) 
    {  
      m_pads[itPad]= ADRCathodePadMk(itPad); 
    }
}


double  
ADRCathodePanel::phiMin() const
{
  double phiMinEast = atan2(cornerEastPhiMin().y(),cornerEastPhiMin().x());
  double phiMinWest = atan2(cornerWestPhiMin().y(),cornerWestPhiMin().x());
  //project difference between two points on phi unit vector:
  //(eastcorner - westcorner)*(phiunit(eastcorner))
  double difTimesPhi= -sin(phiMinEast)*(cos(phiMinEast)-cos(phiMinWest))
                      +cos(phiMinEast)*(sin(phiMinEast)-sin(phiMinWest));
  if(difTimesPhi< 0)
    { return phiMinEast;}
  else
    { return phiMinWest;}
}

double  
ADRCathodePanel::phiMax() const
{
  double phiMaxEast = atan2(cornerEastPhiMax().y(),cornerEastPhiMax().x());
  double phiMaxWest = atan2(cornerWestPhiMax().y(),cornerWestPhiMax().x());
  //project difference between two points on phi unit vector:
  //(eastcorner - westcorner)*(phiunit(eastcorner))
  double difTimesPhi= -sin(phiMaxEast)*(cos(phiMaxEast)-cos(phiMaxWest))
                      +cos(phiMaxEast)*(sin(phiMaxEast)-sin(phiMaxWest));
  if(difTimesPhi> 0)
    { return phiMaxEast;}
  else
    { return phiMaxWest;}
}


double  
ADRCathodePanel::zMax() const
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
ADRCathodePanel::zMin() const
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

double   
ADRCathodePanel::phiMinAtZ(  DABoolean& success, double z) const
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
  if (phiMin() == phi1)
    {
      return (phi1 + (z-z1)*(deltaphi)/(z2-z1));      
    }
  else
    { 
      return (phi2 + (z-z2)*(deltaphi)/(z1-z2));
    }
}
double   
ADRCathodePanel::phiMaxAtZ(  DABoolean& success,double z) const
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
  if (phiMax() == phi1)
    { 
      return (phi2 + (z-z2)*(deltaphi)/(z1-z2));      
    }
  else
    { 
      return (phi1 + (z-z1)*(deltaphi)/(z2-z1));
    }
}
double   
ADRCathodePanel::zMinAtPhi(  DABoolean& success, double phi) const
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
double   
ADRCathodePanel::zMaxAtPhi( DABoolean& success, double phi) const
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
// const member functions
//

//
// static member functions
//









