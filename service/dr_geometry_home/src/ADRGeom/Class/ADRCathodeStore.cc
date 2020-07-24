// -*- C++ -*-
//
// Package:     ADRGeom
// Module:      ADRCathodeStore
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Inga Karliner. 
// Revision history//                          

// 7/9/99 Inga Karliner 
//  Add 
//  padObjectClosest()        Input phi,z; Output: Success, padID, pad object 
//  panelObjectClosest()      Input phi,z; Output: Success, panel object
//  panelNumberClosest()      Input phi,z; Output: Success, panel number 
//  padInPanelNumberClosest() Input phi,z; Output: Success, pad in panel #
//  Note:  if Success=false the results are not to be used.    
//         (z out of range -otherwise it should not happen) 
// Revision 9/15/99 Inga Karliner
//  Initialize m_panels
// Revision 12/11/00 I. Karliner
//   Add pointer DGConstLabVolumePtr  m_EastConicalEndCap 
//   Include survey measurements in panel placement
//          (see ADRCathodeParams)
// 4/24/01 I.Karliner
//   Change over to DGConstLabVolumePtr m_spEastConicalEndCapPiece2        
//

#include "Experiment/Experiment.h"
#include "Experiment/units.h"
// system include filesf
#include "C++Std/iomanip.h" 
#include "C++Std/fstream.h"
#include <stdlib.h>   // for ::exit
#include <assert.h>
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "ADRGeom/ADRCathodeStore.h"
#include "ADRGeom/ADRCathodePanel.h"
#include "ADRGeom/ADRCathodePadInfo.h"
#include "ADRGeom/ADRCathodeParams.h"

#include "DetectorGeometry/DGLabVolume.h"
#include "DetectorGeometry/DGVolumeTubePhiSegment.h"
#include "DRGeom/DRGeometryParams.h"

#include "CLHEP/Geometry/Point3D.h"
#include "CLHEP/Geometry/Vector3D.h"

// STL classes
#include <vector>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "ADRGeom.ADRCathodeStore" ;

//
// static data member definitions
//
//
// constructors and destructor
//
ADRCathodeStore::ADRCathodeStore( 		 
		           const DGConstLabVolumePtr& iCathodePanel0, 
			   const DGConstLabVolumePtr& iCathodePanel1,
			   const DGConstLabVolumePtr& iCathodePanel2,
			   const DGConstLabVolumePtr& iCathodePanel3,
			   const DGConstLabVolumePtr& iCathodePanel4,
			   const DGConstLabVolumePtr& iCathodePanel5,
			   const DGConstLabVolumePtr& iCathodePanel6,
			   const DGConstLabVolumePtr& iCathodePanel7,
			   double iCathodePanelRadius,
			   double iCathodePanelPhiMin,
			   double iCathodePanelPhiDelta,
			   double iCathodePanelHalfLengthZ,
			   const DGConstLabVolumePtr& iConicalEndPlateEastPiece2,
			   ADRCathodePanelConstants** pCathodePanelConstants 
			   ):
  m_panelRadius      (iCathodePanelRadius),
  m_panelPhiMin      (iCathodePanelPhiMin),
  m_panelPhiDelta    (iCathodePanelPhiDelta),
  m_panelHalfLengthZ (iCathodePanelHalfLengthZ),
  m_panels(0),
  m_EastConicalEndCapPiece2(iConicalEndPlateEastPiece2),
  m_PCathodePanelConstants(pCathodePanelConstants)  
{
  // constants
  int ix=0;

  report( DEBUG, kFacilityString )
     <<" ADRCathodeParams Initial Constants "<<endl
    << " Panel PhiMinEast PhiMaxEast ZOffEastPhiMin ZOffEastPhiMax  "<< endl;
  for(  ix=0; ix <ADRCathodeParams::kNumberOfPanels  ; ix++) 
    {
      report( DEBUG, kFacilityString )
	<<setw(2)<<ix<<"."	           
	<<setw(10)<<ADRCathodeParams::kPanelMeasuredPhiMinEast[ix]<<" "	           
	<<setw(10)<<ADRCathodeParams::kPanelMeasuredPhiMaxEast[ix]<<" "         
	<<setw(10)<<ADRCathodeParams::kPanelMeasuredZOffEastPhiMin[ix]<<" "      
	<<setw(10)<<ADRCathodeParams::kPanelMeasuredZOffEastPhiMax[ix]<<" "    
	<<setw(10)<<ADRCathodeParams::kADCathodeZShift[ix]<<endl;
    }  
  report( DEBUG, kFacilityString )
     <<" ADRCathodePanelConstants in ADRCathodeStore"<<endl
    << " Panel PhiMinEast PhiMaxEast ZOffEastPhiMin ZOffEastPhiMax  "<< endl;
  for(  ix=0; ix <ADRCathodeParams::kNumberOfPanels  ; ix++) 
    {
      report( DEBUG, kFacilityString )
	<<setw(2)<<panelConstants(ix).panelNumber()<<"."	           
	<<setw(10)<<panelConstants(ix).phiMinEast()<<" "	           
	<<setw(10)<<panelConstants(ix).phiMaxEast() <<" "         
	<<setw(10)<<panelConstants(ix).zOffEastPhiMin()<<" "      
	<<setw(10)<<panelConstants(ix).zOffEastPhiMax()<<" "    
	<<setw(10)<<panelConstants(ix).zShift()<<endl;
      // replace initial values in DRGeomParams with the constants from 
      // ADRGeomCathodePanels (DB) or test.adrgeomcathodepanels (ascii)
      ADRCathodeParams::kPanelMeasuredPhiMinEast[ix]     = panelConstants(ix).phiMinEast() ;
      ADRCathodeParams::kPanelMeasuredPhiMaxEast[ix]     = panelConstants(ix).phiMaxEast() ;
      ADRCathodeParams::kPanelMeasuredZOffEastPhiMin[ix] = k_in*panelConstants(ix).zOffEastPhiMin(); 
      ADRCathodeParams::kPanelMeasuredZOffEastPhiMax[ix] = k_in*panelConstants(ix).zOffEastPhiMax();
      ADRCathodeParams::kADCathodeZShift[ix]             = panelConstants(ix).zShift() ;
    }

  report( DEBUG, kFacilityString )
     <<" ADRCathodeParams after = constants "<<endl
    << " Panel PhiMinEast PhiMaxEast ZOffEastPhiMin ZOffEastPhiMax  "<< endl;
  for(  ix=0; ix <ADRCathodeParams::kNumberOfPanels  ; ix++) 
    {
      report( DEBUG, kFacilityString )
	<<setw(2)<<ix<<"."	           
	<<setw(10)<<ADRCathodeParams::kPanelMeasuredPhiMinEast[ix]<<" "	           
	<<setw(10)<<ADRCathodeParams::kPanelMeasuredPhiMaxEast[ix]<<" "         
	<<setw(10)<<ADRCathodeParams::kPanelMeasuredZOffEastPhiMin[ix]<<" "      
	<<setw(10)<<ADRCathodeParams::kPanelMeasuredZOffEastPhiMax[ix]<<" "    
	<<setw(10)<<ADRCathodeParams::kADCathodeZShift[ix]<<endl;
    }
  //----------------------------------------+
  // Find the reference surface in the DR3; |
  // compare nominal and aligned geometry   |
  //----------------------------------------+
  
DGConstLabVolumePtr volumeEast = m_EastConicalEndCapPiece2;
double offset65_0 = DRGeometryParams::kConicalOuterLittlePieceZ[0];
double zConical =   (volumeEast->localToLabNominal()*HepPoint3D(0., 0., .0)).z();
double Z65        = (volumeEast->localToLabNominal()*HepPoint3D(0., 0., offset65_0)).z();

HepPoint3D Nominal_X = (volumeEast->localToLabNominal()*HepVector3D(1., 0., .0));
HepPoint3D Aligned_X = (volumeEast->localToLabAligned()*HepVector3D(1., 0., .0));
double zConAligned   = (volumeEast->localToLabAligned()*HepPoint3D(0., 0., .0)).z();

/*
cout<<"------------------------------"<<endl<<endl
<<" CATHODE PLACEMENT            "<<endl
<<"------------------------------"<<endl
<<" zConical East Nominal at     "<< zConical<<endl
<<" zConical East Aligned at     "<< zConAligned<<endl
<<" offset to surface 65         "<<offset65_0<<endl
<<" Z at Surface 65 East         "<<Z65<<endl
<<endl
<<" East Conical x vector in Nominal Geometry "<< Nominal_X<<endl
<<" East Conical x vector in Aligned Geometry "<< Aligned_X<<endl
<<"------------------------"<<endl;
*/
double delz, delphi, ab, tilt_angle,zPhiMin,zPhiMax,phiMin,phiMax;
double zOffEast, phiEast;
double phiMinE, phiMaxE;
double zEdge1st;
double x00,y00, z00 ;
double localx00,localy00,localz00;
double phi00, phiShift;

/*cout<<"delxmin, delxmax,radius [m], panel half length[m]"<<endl
  <<ADRCathodeParams::kPanelMeasuredDelMin<<" "
  <<ADRCathodeParams::kPanelMeasuredDelMax<<" "
  <<m_panelRadius<<"  "
  <<m_panelHalfLengthZ<<endl
  <<"m_panelPhiMin m_panelPhiDelta"
  <<m_panelPhiMin<<" "<<m_panelPhiDelta<<endl;;
  */
// find edge and center of panel 0 using measured positions 
/*unsigned short iPanel=0;
  delz   = ADRCathodeParams::kPanelMeasuredZOffEastPhiMax[0]
  - ADRCathodeParams::kPanelMeasuredZOffEastPhiMin[0];
  
  delphi = ADRCathodeParams::kPanelMeasuredPhiMaxEast[0]
  - ADRCathodeParams::kPanelMeasuredPhiMinEast[0];
  
  ab = sqrt(delz*delz + delphi*delphi* m_panelRadius*m_panelRadius);
  
  tilt_angle = delz/ab;
  
  zPhiMin = - tilt_angle*ADRCathodeParams::kPanelMeasuredDelMin    
  + ADRCathodeParams::kPanelMeasuredZOffEastPhiMin[0];
  
  zPhiMax =   tilt_angle*ADRCathodeParams::kPanelMeasuredDelMax  
    + ADRCathodeParams::kPanelMeasuredZOffEastPhiMax[0];
  
  phiMin  = - ADRCathodeParams::kPanelMeasuredDelMin*delphi/ab
  + ADRCathodeParams::kPanelMeasuredPhiMinEast[0];

  phiMax  =   ADRCathodeParams::kPanelMeasuredDelMax*delphi/ab
  + ADRCathodeParams::kPanelMeasuredPhiMaxEast[0];

  zOffEast =  0.5*(zPhiMin + zPhiMax) - ADRCathodeParams::kADRCathodePitch;
  phiEast  =  0.5*(phiMin  +  phiMax);

  phiMinE  = phiEast -  m_panelPhiDelta/2.;
  phiMaxE =  phiEast +  m_panelPhiDelta/2.;
  
  zEdge1st =  Z65 + zOffEast;
  
  cout<<endl<<"panel 0            "<<endl
  <<"PhiMin PhiMax            "
  <<ADRCathodeParams::kPanelMeasuredPhiMinEast[0]
  <<ADRCathodeParams::kPanelMeasuredPhiMaxEast[0]<<endl
  <<"delz,delphi              "<<delz<<" "<<delphi<<endl
  <<"tilt                     "<<tilt_angle<<endl
  <<"ab,phicc(min),phicd(max) "<<ab<<" "<<phiMin<<" "<<phiMax<<endl
  <<"PhiCenter                "<<phiEast<<endl
  <<"ZEdge1st                 "<<zEdge1st<<endl;
  


  z00      =  zEdge1st + 95.5*ADRCathodeParams::kADRCathodePitch 
  +  0.5*ADRCathodeParams::kADRCathodePadLength;
  x00      =  m_panelRadius*cos(phiEast);
  y00      =  m_panelRadius*sin(phiEast);
  double xMinE    =  m_panelRadius*cos(phiMinE);
  double yMinE    =  m_panelRadius*sin(phiMinE);
  double xMaxE    =  m_panelRadius*cos(phiMaxE);
  double yMaxE    =  m_panelRadius*sin(phiMaxE); 
  
  cout <<endl
  <<"panel 0 center at ("<<x00  <<","<<y00   <<","<<z00<<")"<<endl
  <<"East Phi Min at   ("<<xMinE<<","<<yMinE <<","<<zEdge1st<<")"<<endl
  <<"East Phi Max at   ("<<xMaxE<<","<<yMaxE <<","<<zEdge1st<<")"<<endl;

     */ 
  //------------------------------------------------------------+
  // The data for DGConstLabVolumePtr&  m_panelSupport[]        |
  //------------------------------------------------------------+
  m_panelSupport[0]  =  iCathodePanel0; 
  m_panelSupport[1]  =  iCathodePanel1; 
  m_panelSupport[2]  =  iCathodePanel2; 
  m_panelSupport[3]  =  iCathodePanel3; 
  m_panelSupport[4]  =  iCathodePanel4; 
  m_panelSupport[5]  =  iCathodePanel5; 
  m_panelSupport[6]  =  iCathodePanel6; 
  m_panelSupport[7]  =  iCathodePanel7; 

  unsigned short kPanel;
  unsigned int iPadID = 0;
  unsigned int kPad;


  for (kPanel=0; kPanel< kNumberOfPanels; ++kPanel)
    {  
       for ( kPad = 0; kPad < kNumberOfPadsInPanel; ++kPad) 
	 {  
	   m_padToPanel[iPadID] = kPanel;	          
	   ++iPadID;
	 }  
    }
    
 
//         +-------------------------------------+
//         | Setup each panel                    |
//         | use measured phiMin,PhiMax points   |
//         | shift all by kADCathodeZShift       | 
//         +-------------------------------------+

  m_panels = new ADRCathodePanel[kNumberOfPanels];     

   unsigned int itPanel;
   unsigned int itPad;
   //   double delta = .02;
   for(  itPanel=0; itPanel < kNumberOfPanels; ++itPanel ) 
     { 
       
       ADRCathodePanel* pPanel = m_panels+itPanel;
       
       pPanel->setNumberOfPads( int( kNumberOfPadsInPanel ) );
       pPanel->setVolume      ( m_panelSupport[itPanel]);
       pPanel->setPanelRadius ( m_panelRadius);
       pPanel->setLocalPhiMin (  m_panelPhiMin);
       pPanel->setPhiDelta    (m_panelPhiDelta); 
       pPanel->setHalfLengthZ (m_panelHalfLengthZ);
//------------------------------------------------------------------+
// find edge and center of each panel using the measured positions  |
// of the East side of the 2nd pad at (measured) phimin, phimaxin.  |
// These points are kPanelMeasuredDelMin/Max from the phi edges of  |
// the panels. See also ADRCathodeParams                            |
//------------------------------------------------------------------+

       delz   = ADRCathodeParams::kPanelMeasuredZOffEastPhiMax[itPanel]
	 - ADRCathodeParams::kPanelMeasuredZOffEastPhiMin[itPanel];       
       delphi = ADRCathodeParams::kPanelMeasuredPhiMaxEast[itPanel]
	 - ADRCathodeParams::kPanelMeasuredPhiMinEast[itPanel];       
       ab = sqrt(delz*delz + delphi*delphi* m_panelRadius*m_panelRadius);       
       tilt_angle = delz/ab;       
       zPhiMin = - tilt_angle*ADRCathodeParams::kPanelMeasuredDelMin  
	 + ADRCathodeParams::kPanelMeasuredZOffEastPhiMin[itPanel];       
       zPhiMax =   tilt_angle*ADRCathodeParams::kPanelMeasuredDelMax  
	 + ADRCathodeParams::kPanelMeasuredZOffEastPhiMax[itPanel];       
       phiMin  = - ADRCathodeParams::kPanelMeasuredDelMin*delphi/ab
	 + ADRCathodeParams::kPanelMeasuredPhiMinEast[itPanel];       
       phiMax  =   ADRCathodeParams::kPanelMeasuredDelMax*delphi/ab
	 + ADRCathodeParams::kPanelMeasuredPhiMaxEast[itPanel];
       
       zOffEast =  0.5*(zPhiMin + zPhiMax) - ADRCathodeParams::kADRCathodePitch;
       phiEast  =  0.5*(phiMin  +  phiMax);
       phiMinE  = phiEast -  m_panelPhiDelta/2.;
       phiMaxE =  phiEast +  m_panelPhiDelta/2.;
       zEdge1st =  Z65 + zOffEast + ADRCathodeParams::kADCathodeZShift[itPanel];
       
       z00      =  zEdge1st + 95.5*ADRCathodeParams::kADRCathodePitch 
	 +  0.5*ADRCathodeParams::kADRCathodePadLength;
       x00      =  m_panelRadius*cos(phiEast);
       y00      =  m_panelRadius*sin(phiEast);

       HepPoint3D point0M = HepPoint3D(x00,y00,z00);
       HepPoint3D point00 = (pPanel->panelVolume())->localToLabNominal() *
	 HepPoint3D(pPanel->panelRadius(),0.,0.) ;

       phi00 = atan2(point00.y(),point00.x());
       phiShift = phiEast - phi00;

       HepVector3D Vector0M= point0M - point00;

       //Local Coordinates of the panel:
       //Offset of the center of the panel from (0,0,0) 
       localx00 = Vector0M.dot(
			       (pPanel->panelVolume())->localToLabNominal() * 
			       HepVector3D(1,0,0));
       localy00 = Vector0M.dot(
			       (pPanel->panelVolume())->localToLabNominal() * 
			       HepVector3D(0,1,0));
       localz00 = Vector0M.dot(
			       (pPanel->panelVolume())->localToLabNominal() * 
			       HepVector3D(0,0,1));
       HepVector3D offsetPanelCenter = HepVector3D(localx00,localy00,localz00);

       // nominal (materials aligned) center 
       HepPoint3D point0A = (pPanel->panelVolume())->localToLabAligned() *
	 HepPoint3D(pPanel->panelRadius(),0.,0.) ;
  
       // nominal corner East, PhiMin 
       HepPoint3D pointEMin0= (pPanel->panelVolume())->localToLabAligned() *
	 HepPoint3D(
		   pPanel->panelRadius()*cos(pPanel->localPhiMin()),
		   pPanel->panelRadius()*sin(pPanel->localPhiMin()),
		   -(pPanel->halfLengthZ()));

     // true center (measured and aligned)
       HepPoint3D point0   =
	 (pPanel->panelVolume())->localToLabAligned() * offsetPanelCenter
	 + point0A ;

       // true corner (measured and aligned) East, PhiMin
       HepPoint3D pointEMin= (pPanel->panelVolume())->localToLabAligned() *
	HepPoint3D(
		   pPanel->panelRadius()*cos(pPanel->localPhiMin() + phiShift),
		   pPanel->panelRadius()*sin(pPanel->localPhiMin() + phiShift),
		   -(pPanel->halfLengthZ()) + localz00); 
       HepVector3D EMinDif = pointEMin - pointEMin0;

       HepPoint3D pointEMax= (pPanel->panelVolume())->localToLabAligned() *
	HepPoint3D(
		   pPanel->panelRadius()*cos(pPanel->localPhiMin()
		     +pPanel->phiDelta()+ phiShift),
		   pPanel->panelRadius()*sin( pPanel->localPhiMin()
		     +pPanel->phiDelta()+ phiShift),
		   -(pPanel->halfLengthZ()) + localz00);
      
       HepPoint3D pointWMin= (pPanel->panelVolume())->localToLabAligned() *
	 HepPoint3D(
		    pPanel->panelRadius() *cos(pPanel->localPhiMin()+ phiShift),
		    pPanel->panelRadius() *sin(pPanel->localPhiMin()+ phiShift),
		    (pPanel->halfLengthZ()) + localz00);
       
       HepPoint3D pointWMax= (pPanel->panelVolume())->localToLabAligned() *
	 HepPoint3D(
		    pPanel->panelRadius()*cos(pPanel->localPhiMin()
		      +pPanel->phiDelta()+ phiShift),
		    pPanel->panelRadius()*sin( pPanel->localPhiMin()
		      +pPanel->phiDelta()+ phiShift),
		    (pPanel->halfLengthZ()) + localz00);
  //check

/*       cout<<"panel " <<itPanel<<" phiEast                " <<phiEast<<endl
	 <<"panel " <<itPanel<<" phiNominal             " <<phi00<<endl
	 <<"panel " <<itPanel<<" phiEast - phiNominal   " <<phiShift<<endl
	 <<"panel " <<itPanel<<" center measured        " <<point0M<<endl
	 <<"panel " <<itPanel<<" center nominal         " <<point00<<endl
	 <<"panel " <<itPanel<<" vector difference      " <<Vector0M<<endl
	 <<"panel " <<itPanel<<"  =? ->vol->Lab         " 
	 <<(pPanel->panelVolume())->localToLabAligned() * offsetPanelCenter<<endl
	 <<"panel " <<itPanel<<" =? center measured     "<<point0   <<endl
	 <<"panel " <<itPanel<<" EMin nominal           "<<pointEMin0<<endl
	 <<"panel " <<itPanel<<" EMin measured          "<<pointEMin<<endl
	 <<"panel " <<itPanel<<" EMin difference        "<<EMinDif<<endl
	 <<"panel " <<itPanel<<" R-panel                "<<m_panelRadius<<endl
	 <<"panel " <<itPanel<<" R-point0 (center,shftd "
	 <<sqrt(point0.x()*point0.x() + point0.y()*point0.y())<<endl
	 <<"panel " <<itPanel<<" R-EMin0                "
	 <<sqrt(pointEMin0.x()*pointEMin0.x() +pointEMin0.y()*pointEMin0.y())<<endl
	 <<"panel " <<itPanel<<" R-EMin                 "
	 <<sqrt(pointEMin.x()*pointEMin.x()   +pointEMin.y()*pointEMin.y())  
	 <<endl ;
	 */
       pPanel->setPoint0(point0);
       pPanel->setCornerEastPhiMin(pointEMin);
       pPanel->setCornerEastPhiMax(pointEMax);
       pPanel->setCornerWestPhiMin(pointWMin);
       pPanel->setCornerWestPhiMax(pointWMax);
       pPanel->setPanelPhiOffset(phiShift);
       pPanel->setPanelZOffset(localz00);
       pPanel->setPanelOffsetCenter(offsetPanelCenter);

       pPanel->makePadsInPanel();
     }
      
   DABoolean success; 
   // print out panelsno


}

// ADRCathodeStore::ADRCathodeStore(const ADRCathodeStore& ) 
// {
// }


ADRCathodeStore::~ADRCathodeStore()
{
   delete [] m_panels; 
   for(  int ix=0; ix < (kNumberOfPanels-1) ; ix++)  
     {	
       delete m_PCathodePanelConstants [ix];
     }   
}

//
// assignment operators
//
// const ADRCathodeStore& ADRCathodeStore::operator=
//                                 ( const ADRCathodeStore& )
// {
// }

//
// member functions
//

void ADRCathodeStore::setCathode (  		 
		             const DGConstLabVolumePtr& iCathodePanel0, 
			     const DGConstLabVolumePtr& iCathodePanel1,
			     const DGConstLabVolumePtr& iCathodePanel2,
			     const DGConstLabVolumePtr& iCathodePanel3,
			     const DGConstLabVolumePtr& iCathodePanel4,
			     const DGConstLabVolumePtr& iCathodePanel5,
			     const DGConstLabVolumePtr& iCathodePanel6,
			     const DGConstLabVolumePtr& iCathodePanel7,
			     double iCathodePanelRadius,
			     double iCathodePanelPhiMin,
			     double iCathodePanelPhiDelta,
			     double iCathodePanelHalfLengthZ,
			     const DGConstLabVolumePtr& iConicalEndPlateEastPiece2,
			     ADRCathodePanelConstants** pCathodePanelConstants)
{   
  m_panelSupport[0]  =  iCathodePanel0; 
  m_panelSupport[1]  =  iCathodePanel1; 
  m_panelSupport[2]  =  iCathodePanel2; 
  m_panelSupport[3]  =  iCathodePanel3; 
  m_panelSupport[4]  =  iCathodePanel4; 
  m_panelSupport[5]  =  iCathodePanel5; 
  m_panelSupport[6]  =  iCathodePanel6; 
  m_panelSupport[7]  =  iCathodePanel7;
  m_panelRadius      =  iCathodePanelRadius;
  m_panelPhiMin      =  iCathodePanelPhiMin;
  m_panelPhiDelta    =  iCathodePanelPhiDelta;
  m_panelHalfLengthZ =  iCathodePanelHalfLengthZ;
  m_EastConicalEndCapPiece2 = iConicalEndPlateEastPiece2 ;
  m_PCathodePanelConstants = pCathodePanelConstants;
  unsigned int itPanel;
  for( itPanel=0 ; itPanel < kNumberOfPanels ; ++ itPanel )
    { 
    m_panels[itPanel].setVolume( m_panelSupport[itPanel] );
    }

}


//
// const member functions
//
const ADRCathodePanel&
ADRCathodeStore::panel( unsigned int iPanelID ) const
{
  assert( kNumberOfPanels >= iPanelID );
  return m_panels[iPanelID-kFirstPanel];
}


// NOTE: padID            counts from 1 
//       pan, panel COUNT from 0

unsigned int 
ADRCathodeStore::padID(const unsigned short panel,
		       const unsigned short pad) const  
{ 
  assert ( panel < ADRCathodeStore::kNumberOfPanels );  
  assert ( pad  < ADRCathodeStore::kNumberOfPadsInPanel ) ;
  unsigned int ipadID=panel*ADRCathodeStore::kNumberOfPadsInPanel 
    + pad  +  ADRCathodeStore::kFirstPadID ;
  return ipadID;
}

unsigned short 
ADRCathodeStore::panelOfPadID(const UInt32 PadId) const 
{
  assert ( (PadId -ADRCathodeStore::kFirstPadID) 
	    < ADRCathodeStore::kNumberOfPadsTotal);
  return m_padToPanel[PadId-ADRCathodeStore::kFirstPadID];
}

unsigned short
ADRCathodeStore::padInPanelOfPadID(const UInt32 PadId) const 
{
  assert ( (PadId  - ADRCathodeStore::kFirstPadID)
	      < ADRCathodeStore::kNumberOfPadsTotal);
  return  (int(PadId) - ADRCathodeStore::kFirstPadID
	   - m_padToPanel[PadId-ADRCathodeStore::kFirstPadID]
	   *(ADRCathodeStore::kNumberOfPadsInPanel));
}

//private; minimize distance (phiCath, zCath) to the panel center:
unsigned int  
ADRCathodeStore::panelNumberClosestPerfect(
      DABoolean& success, double phiCath, double zCath) const
{
  success = true ; 
  int iPanel0 =0;
  int itPanel;
  const ADRCathodePanel& pPanel0 = panel(iPanel0);
  double RadCath =  pPanel0.panelRadius();
  //make a point at r=RadCath, PhiCath, zCath
  double xCath = RadCath*cos(phiCath);
  double yCath = RadCath*sin(phiCath);
  HepVector3D position= HepPoint3D(xCath,yCath,zCath);
  int iPhiPanel =-1;
  double minDistPanel = 999999. ;
  double dist;
  //minimize distance of (phiCath, zCath) to the panel center:
  for(  itPanel=0; itPanel < ADRCathodeStore::kNumberOfPanels; ++itPanel )
    { 
      const ADRCathodePanel& ipPanel = panel(itPanel);
      HepVector3D panelCenter = ipPanel.point0(); 
      HepVector3D distVector = panelCenter - position;
      dist = distVector.mag();
      if(dist <= minDistPanel)
	{
	  minDistPanel = dist;
	  iPhiPanel    = itPanel;
	}
    }
  unsigned int iPanel = iPhiPanel;
  if (  ( zCath < panel(iPhiPanel).zMin())
	| ( zCath > panel(iPhiPanel).zMax()) )
    { success = false; }
  return iPanel;
}

const ADRCathodePanel& 
ADRCathodeStore::panelObjectClosestPerfect(
              DABoolean& success, double phiCath, double zCath) const
{
  success = true ;
  DABoolean success1; 
  unsigned int iPanel = panelNumberClosestPerfect(success1, phiCath, zCath) ; 
  if(!success1) { success = false; }
  return panel(iPanel); 
}


const ADRCathodePadInfo& 
ADRCathodeStore::padObjectClosest(DABoolean& success, unsigned int& padId, 
				  double phiCath, double zCath) const
{
  success = true ;        
  padId = 0;              //default to non-existent id 
  unsigned int itPad = 0; //default to pad 0 ( must return a pad object)
  DABoolean success1; 
  unsigned int itPanel;
  unsigned int iPanel = panelNumberClosestPerfect(success1, phiCath, zCath) ;
  if(!success1) 
    { 
      success = false; 
      return panel(iPanel).ADRCathodePad(itPad);
    }
  //find pad
  int iPhiZPad= -1; 
  double dist;
  double minDistPad = 999999.;  
  const ADRCathodePanel& pPanel = panel(iPanel);
  double RadCath =  pPanel.panelRadius();
  //make a point at rcathode, PhiCath, zCath
  double xCath = RadCath*cos(phiCath);
  double yCath = RadCath*sin(phiCath);
  HepVector3D position= HepPoint3D(xCath,yCath,zCath);
  for (itPad = 0; itPad < ADRCathodeStore::kNumberOfPadsInPanel; itPad++)
    {
      const ADRCathodePadInfo& pad = pPanel.ADRCathodePad(itPad);
      HepVector3D padCenter = pad.point0(); 
      HepVector3D distVector = padCenter - position;
      dist = distVector.mag();
      if(dist <= minDistPad)
	{
	  minDistPad = dist;
	  iPhiZPad    = itPad;
	}
    }

  //check same pad for panel +/-1 (Last, Next)
  int iPanelL;
  int iPanelN;
  if(iPanel > 0 )
    { iPanelL= iPanel - 1;}
  else
    { iPanelL=  ADRCathodeStore::kNumberOfPanels -1;}
  
  if(iPanel <  ADRCathodeStore::kNumberOfPanels -1) 
    { iPanelN= iPanel + 1;}
  else
    { iPanelN=  0 ; }
	 
  itPanel = iPanelL; 
  const ADRCathodePanel& ipPanelL = panel(itPanel);  
  const ADRCathodePadInfo& padL   = ipPanelL.ADRCathodePad(iPhiZPad);
  HepVector3D padCenter = padL.point0(); 
  HepVector3D distVector = padCenter - position;
  dist = distVector.mag();
  if(dist <= minDistPad)
    {
      minDistPad = dist;
      iPanel = itPanel;
    }
  	 
  itPanel = iPanelN; 
  const ADRCathodePanel& ipPanelN = panel(itPanel);
  const ADRCathodePadInfo& padN   = ipPanelN.ADRCathodePad(iPhiZPad);
  padCenter = padN.point0(); 
  distVector = padCenter - position;
  dist = distVector.mag();
  if(dist <= minDistPad)
    {
      minDistPad = dist;
      iPanel = itPanel;
    }
  // replace panel if necessary
  const ADRCathodePanel& finalPhiPanel = panel(iPanel);
  padId = padID(iPanel,iPhiZPad);
  return finalPhiPanel.ADRCathodePad(iPhiZPad);
}
  
unsigned int
ADRCathodeStore::panelNumberClosest(
	      DABoolean& success, double phiCath, double zCath) const
{
  success = true ; 
  DABoolean success1; 
  unsigned int panelDefault = 0;  //default panel
  unsigned int ipadId = 0;        
  const ADRCathodePadInfo& padObject =
  padObjectClosest(success1,  ipadId, phiCath, zCath);
  success = success1;
  if(success1)
    { 
      return panelOfPadID(ipadId);
    }
  else 
  {
    return panelDefault;
  }
}
   
unsigned int
ADRCathodeStore::padInPanelNumberClosest(
	      DABoolean& success, double phiCath, double zCath) const
{
  success = true ;
  DABoolean success1; 
  unsigned int padInPanelDefault = 0;  //default pad 
  unsigned int ipadId=0;
  const ADRCathodePadInfo& padObject =
  padObjectClosest(success1,  ipadId, phiCath, zCath);
  success = success1;
  if(success1)
    { 
      return padInPanelOfPadID(ipadId);
    }
  else 
  {
    return padInPanelDefault;
  }
}

const ADRCathodePanel& 
ADRCathodeStore::panelObjectClosest(
             DABoolean& success, double phi, double z ) const
{
  success = true; 
  DABoolean success1;
  unsigned int panelNumber = panelNumberClosest( success1, phi, z);
  success = success1;
  return panel(panelNumber);
}

const ADRCathodePanelConstants& 
ADRCathodeStore::panelConstants(unsigned int iPanelID) const
{
  assert( kNumberOfPanels >= iPanelID );
  return *m_PCathodePanelConstants[iPanelID-kFirstPanel];
}

 
  //
  // static member functions
  //










