// -*- C++ -*-
//
// Package:     <ADRGEOM>
// Module:      ADRWireLayerRealistic
// 
// Description:     See header file
//
// Implementation:
//
// Author:      Inga Karliner
// Created:     October 1999
//
// Revision history
//
// First submission
// 11-10-99 delete survey constants in layer. correct # of delets
// 02-06-01 transform the correction vector (WireSurveyConstants) to the lab

#include "Experiment/Experiment.h"

// system include files
#include <assert.h>
#include "C++Std/iomanip.h" 
#include "C++Std/fstream.h"

#if defined(AMBIGUOUS_STRING_FUNCTIONS_BUG)
#include <string>
#endif /* AMBIGUOUS_STRING_FUNCTIONS_BUG */

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "Experiment/units.h"
#include "ADRGeom/ADRWireLayerRealistic.h"
#include "DetectorGeometry/DGConstLabVolumePtr.h"
#include "DetectorGeometry/DGLabVolume.h"

// STL classes

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "ActiveElements.ADRWireLayerRealistic" ;

//
// static data member definitions
//

//
// constructors and destructor
//
ADRWireLayerRealistic::ADRWireLayerRealistic():
   AEWireLayerCylindrical(),
  m_PWireSurveyConstants(0)
{}

ADRWireLayerRealistic::ADRWireLayerRealistic(
		            double iRadiusEndCap,
			    double iPhiOfFirstWireInVolume1,
			    double iPhiOfFirstWireInVolume2,
			    unsigned int iNumberOfWires,
			    double iWireSagx,
			    double iWireSagy,
			    const DGConstLabVolumePtr& iVolume1,
			    const DGConstLabVolumePtr& iVolume2,
			    ADRWireSurveyConstants** pWireSurveyConstants,
			    double izOffset1,
			    double izOffset2):
  AEWireLayerCylindrical(
	      iRadiusEndCap,iPhiOfFirstWireInVolume1,iPhiOfFirstWireInVolume2,
	      iNumberOfWires, iWireSagx, iWireSagy, iVolume1, iVolume2,
	      izOffset1,      izOffset2),
  m_PWireSurveyConstants(pWireSurveyConstants) 
{
}



// ADRWireLayerRealistic::ADRWireLayerRealistic( const ADRWireLayerRealistic& )
// {
// }

ADRWireLayerRealistic::~ADRWireLayerRealistic()
{   
  for(  int ix=0; ix <  numberOfWires()  ; ix++)  
    {
      delete m_PWireSurveyConstants [ix];
    }  

}

//
// assignment operators
//
// const ADRWireLayerRealistic& ADRWireLayerRealistic::operator=( const ADRWireLayerRealistic& )
// {
// }

//
// member functions
//
      // member functions
void 
ADRWireLayerRealistic::setLayer(unsigned int iNumberOfWires,
				const DGConstLabVolumePtr& iVolume1,
				const DGConstLabVolumePtr& iVolume2,
				double sagx,
				double sagy, 
				int readoutEnd,
				double radiusEndCap,
				double angle1,
				double angle2,
				double zoffset1,
				double zoffset2,
				double rextentMin,
				double rextentPls,
				DABoolean axial,
				DABoolean stereo,
				DABoolean stereoU,
				DABoolean stereoV,
				ADRWireSurveyConstants** pWireSurveyConstants)
{ 
  m_PWireSurveyConstants = pWireSurveyConstants;
  
  setNumberOfWires(iNumberOfWires);
  setWireSagx( sagx);
  setWireSagy( sagy);
  setReadoutEnd(readoutEnd);
  setVolume1(  iVolume1);
  setVolume2(  iVolume2);
  setPhiOfFirstWireInVolume1(angle1);
  setPhiOfFirstWireInVolume2(angle2);
  setRadiusEndCap(radiusEndCap);
  setZOffset1(zoffset1);
  setZOffset2(zoffset2);
  setRadialExtentMinus(rextentMin);
  setRadialExtentPlus( rextentPls); 
  setAxial  ( axial); 
  setStereo ( stereo); 
  setStereoU ( stereoU); 
  setStereoV ( stereoV);
//---------------+
//make all wires |
//---------------+
  makeWiresInLayer();
  setRadiusNominal();
//-------------------------------------------------+
// radius, phi0 of wire 0 at z=0 |
//-------------------------------------------------+
  setRadius();
  setPhi0();
//-------------------------------------------------+
// phi0Nominal = phi0 of the 1st wire, without sag |
//-------------------------------------------------+
 setPhi0Nominal(); 
 setTanStereoAngle(); 
 setZMin();
 setZMax(); 

  //debug printout
 /* unsigned int ktWire=0;
    cout<<setw(10)<<ktWire
    <<setw(10)<<wireSurveyConstants(ktWire).dRWireNumber()
    <<setw(10)<<wireSurveyConstants(ktWire). correction_East_x_Microns()
     <<setw(10)<<wireSurveyConstants(ktWire). correction_East_y_Microns()
     <<setw(10)<<wireSurveyConstants(ktWire). correction_East_z_Microns()
     <<setw(10)<<wireSurveyConstants(ktWire). correction_West_z_Microns()
     <<setw(10)<<wireSurveyConstants(ktWire). correction_West_x_Microns()
     <<setw(10)<<wireSurveyConstants(ktWire). correction_West_y_Microns()
     <<setw(10)<<wireSurveyConstants(ktWire). correction_West_z_Microns()
     <<endl;
     */    
}
void 
ADRWireLayerRealistic::makeWiresInLayer() 
{
  //  cout<<" ADRWireLayerRealistic:::makeWiresInLayer() "<<endl;

//         +------------------------------+
//         | Setup array of wires         |
//         +------------------------------+
//         
  const unsigned int  NumberOfWires = numberOfWires();

  if(0 == m_wires)
    {
      m_wires= new AEWireInfo[NumberOfWires];
      for( unsigned int itWire(0); itWire < numberOfWires(); ++itWire ) 
	{  
	  m_wires[itWire]= wireMk(itWire);
	}
    }
  else
    {
      //      update
      for( unsigned int itWire(0); itWire < numberOfWires(); ++itWire )
	{
	  wireUpdate(itWire);
	}
      // 
    }
}

AEWireInfo
ADRWireLayerRealistic::wireMk( unsigned int iWireID ) const
{
  /*  if(iWireID==0) { cout<<" ADRWireLayerRealistic::wireMk"<<endl;} */
  assert( numberOfWires() > 0 );
  assert( iWireID < numberOfWires() );
  AEWireLayerCylindrical::EndPointsEW  iEastWest = wireCalc(iWireID );
  HepVector3D corrEast = volume1()->localToLabAligned() *
                        HepVector3D( k_um * wireSurveyConstants(iWireID). correction_East_x_Microns(),
				     k_um * wireSurveyConstants(iWireID). correction_East_y_Microns(),
				     k_um * wireSurveyConstants(iWireID). correction_East_z_Microns());
				    
  HepVector3D corrWest = volume2()->localToLabAligned() *
                        HepVector3D( k_um * wireSurveyConstants(iWireID). correction_West_x_Microns(),
				     k_um * wireSurveyConstants(iWireID). correction_West_y_Microns(),
				     k_um * wireSurveyConstants(iWireID). correction_West_z_Microns());
				    
  double Eastx =  iEastWest.first.x() + corrEast.x();
  double Easty =  iEastWest.first.y() + corrEast.y();
  double Eastz =  iEastWest.first.z() + corrEast.z();

  double Westx =  iEastWest.second.x() + corrWest.x();
  double Westy =  iEastWest.second.y() + corrWest.y();
  double Westz =  iEastWest.second.z() + corrWest.z();

  DABoolean do_axial = false;
  DABoolean do_stereo = false;
  char* dirty_which = getenv("DIRTY_WHICH");
  if ( dirty_which != NULL ) {
     if (*dirty_which == 's') {
	do_axial = false;
	do_stereo = true;
     }
     else if (*dirty_which == 'a') {
	do_axial = true;
	do_stereo = false;
     }
     else if (*dirty_which == 'b') {
	do_axial = true;
	do_stereo = true;
     }
     report(DEBUG, kFacilityString)
	<< "$env(DIRTY_WHICH) == \"" << dirty_which << "\" == (do_axial " << (do_axial ? "yes" : "no")
	<< ", do_stereo " << (do_stereo ? "yes" : "no") << ")" << endl;
  }

  char* dirty_stretch = getenv("DIRTY_STRETCH");
  if ( dirty_stretch != NULL ) {
     int microns(0);
     char* d(dirty_stretch);
     for (; *d >= '0'  &&  *d <= '9';  d++) {
	microns *= 10;
	microns += *d - '0';
     }
     report(DEBUG, kFacilityString)
	<< "$env(DIRTY_STRETCH) == \"" << dirty_stretch << "\" == " << microns << " microns." << endl;
     report(DEBUG, kFacilityString)
	<< "{Eastx, Easty} = {" << Eastx << ", " << Easty << "},"
	<< " {Westx, Westy} = {" << Westx << ", " << Westy << "}" << endl;

     double stretch(double(microns)*1e-6);
     if (Eastx == 0.) {
	if (Easty > 0.) Easty += stretch;
	else Easty -= stretch;
     }
     else {
	double east_yoverx(Easty/Eastx);
	double east_sign(Eastx/fabs(Eastx));
	double east_gamma(1./sqrt(1. + sqr(east_yoverx)));

	if ((do_axial  &&  sqrt(sqr(Eastx) + sqr(Easty)) < 0.356)  ||
	    (do_stereo  &&  sqrt(sqr(Eastx) + sqr(Easty)) > 0.356)   ) {
	   Eastx += stretch * east_sign * east_gamma;
	   Easty += stretch * east_sign * east_yoverx * east_gamma;
	}
     }

     if (Westx == 0.) {
	if (Westy > 0.) Westy += stretch;
	else Westy -= stretch;
     }
     else {
	double west_yoverx(Westy/Westx);
	double west_sign(Westx/fabs(Westx));
	double west_gamma(1./sqrt(1. + sqr(west_yoverx)));

	if ((do_axial  &&  sqrt(sqr(Eastx) + sqr(Easty)) < 0.356)  ||
	    (do_stereo  &&  sqrt(sqr(Eastx) + sqr(Easty)) > 0.356)   ) {
	   Westx += stretch * west_sign * west_gamma;
	   Westy += stretch * west_sign * west_yoverx * west_gamma;
	}
     }

     report(DEBUG, kFacilityString)
	<< "{Eastx, Easty} = {" << Eastx << ", " << Easty << "},"
	<< " {Westx, Westy} = {" << Westx << ", " << Westy << "}" << endl;
  }

  // code below uses lab values of corrections, replaced by the code above
  /*
  double Eastx =  iEastWest.first.x()
    + k_um * wireSurveyConstants(iWireID). correction_East_x_Microns();
  double Easty =  iEastWest.first.y()
    + k_um * wireSurveyConstants(iWireID). correction_East_y_Microns();
  double Eastz =  iEastWest.first.z()
    + k_um * wireSurveyConstants(iWireID). correction_East_z_Microns();
  double Westx =  iEastWest.second.x()
    + k_um * wireSurveyConstants(iWireID). correction_West_x_Microns();
  double Westy =  iEastWest.second.y()
    + k_um * wireSurveyConstants(iWireID). correction_West_y_Microns();
  double Westz =  iEastWest.second.z()
    + k_um * wireSurveyConstants(iWireID). correction_West_z_Microns();
    */
  HepPoint3D point1(Eastx,Easty, Eastz);
  HepPoint3D point2(Westx,Westy, Westz);
  
  return AEWireInfo( point1, point2, wireSagx(), wireSagy() );
}
  
void
ADRWireLayerRealistic::wireUpdate( unsigned int iWireID ) 
{ 
  /*  if(iWireID==0) {      cout<<" ADRWireLayerRealistic::wireUpdate"<<endl;} */
  assert( numberOfWires() > 0 );
  assert( iWireID < numberOfWires() );  
  AEWireLayerCylindrical::EndPointsEW  iEastWest = wireCalc(iWireID );
  AEWireInfo& Wire = wireNC(iWireID);

  HepVector3D corrEast = volume1()->localToLabAligned() *
                        HepVector3D( k_um * wireSurveyConstants(iWireID). correction_East_x_Microns(),
				     k_um * wireSurveyConstants(iWireID). correction_East_y_Microns(),
				     k_um * wireSurveyConstants(iWireID). correction_East_z_Microns());
				    
  HepVector3D corrWest = volume2()->localToLabAligned() *
                        HepVector3D( k_um * wireSurveyConstants(iWireID). correction_West_x_Microns(),
				     k_um * wireSurveyConstants(iWireID). correction_West_y_Microns(),
				     k_um * wireSurveyConstants(iWireID). correction_West_z_Microns());
				    
  double Eastx =  iEastWest.first.x() + corrEast.x();
  double Easty =  iEastWest.first.y() + corrEast.y();
  double Eastz =  iEastWest.first.z() + corrEast.z();

  double Westx =  iEastWest.second.x() + corrWest.x();
  double Westy =  iEastWest.second.y() + corrWest.y();
  double Westz =  iEastWest.second.z() + corrWest.z();

  DABoolean do_axial = false;
  DABoolean do_stereo = false;
  char* dirty_which = getenv("DIRTY_WHICH");
  if ( dirty_which != NULL ) {
     if (*dirty_which == 's') {
	do_axial = false;
	do_stereo = true;
     }
     else if (*dirty_which == 'a') {
	do_axial = true;
	do_stereo = false;
     }
     else if (*dirty_which == 'b') {
	do_axial = true;
	do_stereo = true;
     }
     report(DEBUG, kFacilityString)
	<< "$env(DIRTY_WHICH) == \"" << dirty_which << "\" == (do_axial " << (do_axial ? "yes" : "no")
	<< ", do_stereo " << (do_stereo ? "yes" : "no") << ")" << endl;
  }

  char* dirty_stretch = getenv("DIRTY_STRETCH");
  if ( dirty_stretch != NULL ) {
     int microns(0);
     char* d(dirty_stretch);
     for (; *d >= '0'  &&  *d <= '9';  d++) {
	microns *= 10;
	microns += *d - '0';
     }
     report(DEBUG, kFacilityString)
	<< "$env(DIRTY_STRETCH) == \"" << dirty_stretch << "\" == " << microns << " microns." << endl;
     report(DEBUG, kFacilityString)
	<< "{Eastx, Easty} = {" << Eastx << ", " << Easty << "},"
	<< " {Westx, Westy} = {" << Westx << ", " << Westy << "}" << endl;

     double stretch(double(microns)*1e-6);
     if (Eastx == 0.) {
	if (Easty > 0.) Easty += stretch;
	else Easty -= stretch;
     }
     else {
	double east_yoverx(Easty/Eastx);
	double east_sign(Eastx/fabs(Eastx));
	double east_gamma(1./sqrt(1. + sqr(east_yoverx)));

	if ((do_axial  &&  sqrt(sqr(Eastx) + sqr(Easty)) < 0.356)  ||
	    (do_stereo  &&  sqrt(sqr(Eastx) + sqr(Easty)) > 0.356)   ) {
	   Eastx += stretch * east_sign * east_gamma;
	   Easty += stretch * east_sign * east_yoverx * east_gamma;
	}
     }

     if (Westx == 0.) {
	if (Westy > 0.) Westy += stretch;
	else Westy -= stretch;
     }
     else {
	double west_yoverx(Westy/Westx);
	double west_sign(Westx/fabs(Westx));
	double west_gamma(1./sqrt(1. + sqr(west_yoverx)));

	if ((do_axial  &&  sqrt(sqr(Eastx) + sqr(Easty)) < 0.356)  ||
	    (do_stereo  &&  sqrt(sqr(Eastx) + sqr(Easty)) > 0.356)   ) {
	   Westx += stretch * west_sign * west_gamma;
	   Westy += stretch * west_sign * west_yoverx * west_gamma;
	}
     }

     report(DEBUG, kFacilityString)
	<< "{Eastx, Easty} = {" << Eastx << ", " << Easty << "},"
	<< " {Westx, Westy} = {" << Westx << ", " << Westy << "}" << endl;
  }

  // code below uses lab values of corrections, replaced by the code above
/*
  double Eastx =  iEastWest.first.x() 
    + k_um * wireSurveyConstants(iWireID). correction_East_x_Microns();
  double Easty =  iEastWest.first.y()
    + k_um * wireSurveyConstants(iWireID). correction_East_y_Microns();
  double Eastz =  iEastWest.first.z()
    + k_um * wireSurveyConstants(iWireID). correction_East_z_Microns();
  double Westx =  iEastWest.second.x()
    + k_um * wireSurveyConstants(iWireID). correction_West_x_Microns();
  double Westy =  iEastWest.second.y()
    + k_um * wireSurveyConstants(iWireID). correction_West_y_Microns();
  double Westz =  iEastWest.second.z()
    + k_um * wireSurveyConstants(iWireID). correction_West_z_Microns();
    */
  HepPoint3D point1(Eastx,Easty, Eastz);
  HepPoint3D point2(Westx,Westy, Westz);
  
  Wire.setPoint1(point1);
  Wire.setPoint2(point2);
}
//
  // const member functions
  //
  const ADRWireSurveyConstants&
    ADRWireLayerRealistic::wireSurveyConstants( unsigned int iwire ) const
    {
      assert(  numberOfWires() > iwire  );
      return *m_PWireSurveyConstants[iwire];
    }
  
  //
  // static member functions
  //
  
  







