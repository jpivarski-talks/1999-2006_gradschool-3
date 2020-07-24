// -*- C++ -*-
//
// Package:     ADRGeomProd
// Module:      ADRSenseWireStoreFactory
// 
// Description: make and maintain ADRSenseWireStore
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Adam Lyon and Inga Karliner
//
// Revision history
//
// $Log: ADRSenseWireStoreFactory.cc,v $
// Revision 1.8  2003/01/27 19:43:02  cdj
// get iomanip.h from C++Std package
//
// Revision 1.7  2002/11/27 20:59:43  bkh
// Alter code to accommodate new endplate hierarchy
//
// Revision 1.6  2002/11/21 21:01:42  cleo3
// changed fstream.h to C++Std/fstream.h
//
// Revision 1.5  2001/05/05 14:54:30  slee
// Modified to fit the Conical Endplate name change in DRGeom
//
// Revision 1.4  2001/04/13 18:35:17  inga
// Remove cout statements from ADRSenseWireStoreFactory.cc
//
// Revision 1.3  2001/02/06 23:27:51  inga
//  read ADRGeomLayersEndsConstants & ADRGeomLayerConstants
//
// Revision 1.2  1999/12/10 19:56:55  inga
// let extract check if the data .valid
//
// Revision 1.1.1.1  1999/12/08 17:23:09  inga
// import ADRGeomProd sources
//
// Revision 1.2  1999/11/29 16:14:06  inga
// Fix typo in get_WestXMicron()
//
// Revision 1.1  1999/11/18 23:16:32  inga
// add ADRSenseWireStoreFactory
// 11/29/99 I.Karliner Fix typo in get_WestXMicron(),get_WestYMicron()
// 12/1/99 I.Karliner New library ADRGeomProd (was in ADRGeom
// 12/10/99 I.Karliner Remove check if etracted data is valid
//                     The new extract() checks it. 
// 2/06/01  I.Karliner  Read in ADRGeomLayer and ADRGeomLayersEnds
// 5/04/01  SangjoonLee DRGeom changed from 
//   DRGeometryParams::kConicalEndplateName, DRGeometryParams::kWestID)    
//   DRGeometryParams::kConicalEndplateName, DRGeometryParams::kEastID)    to
//   
//   DRGeometryParams::kConicalEndplateWestName, DRGeometryParams::kDefaultVolumeID);
//   DRGeometryParams::kConicalEndplateEastName, DRGeometryParams::kDefaultVolumeID);
//

#include "Experiment/Experiment.h"

// system include files
#include "C++Std/iomanip.h" 
#include "C++Std/fstream.h"
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
//#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "ADRGeomProd/ADRSenseWireStoreFactory.h"
#include "ADRGeomProd/ADRGeomProd.h"
#include "ADRGeom/ADRWireSurveyConstants.h"
#include "ADRGeom/ADRLayerConstants.h"
#include "ADRGeom/ADRGeomLayersEndsConstants.h"

#include "Experiment/report.h"

#include "DataHandler/Record.h"
#include "FrameAccess/extract.h"           
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"
#include "DataHandler/Frame.h"

#include "FrameAccess/FAConstants.h"
#include "DBADRGeomWireSurvey.hh"
#include "DBADRGeomLayer.hh"
#include "DBADRGeomLayersEnds.hh"


#include "ADRGeomProd/ADRSenseWireProxy.h"
#include "DRGeom/DRGeometryParams.h"

#include "DetectorGeometry/DGDetectorStore.h"
#include "DetectorGeometry/DGVolumePtr.h"
#include "DetectorGeometry/DGLabVolume.h"
#include "DetectorGeometry/DGConstLabVolumePtr.h"
#include "DetectorGeometry/DGVolume.h"
#include "DetectorGeometry/DGConstVolumePtr.h"
#include "DetectorGeometry/DGPath.h"
#include "DetectorGeometry/DGVolumePolyCone.h"
#include "DetectorGeometry/DGVolumeType.h"
#include "DetectorGeometry/DGVolumeParentPlaced.h"


// STL classes
//#include <vector>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "ADRGeomProd.ADRSenseWireStoreFactory" ;

//
// static data member definitions
//

//
// constructors and destructor
//

ADRSenseWireStoreFactory::ADRSenseWireStoreFactory(
				      const DGDetectorStore& drDetectorStore,
				      const Frame& theFrame) :
 m_ADRSenseWireStore(0)
{
  // Get the wire survey constants
  ADRWireSurveyConstants** pWireSurveyConstants = 
    getWireSurveyConstants(theFrame);

  // Get the layer constants
  ADRLayerConstants** pGeomLayer =  getGeomLayer(theFrame);
       
  // Get Dan Peterson's ends of layers
  ADRGeomLayersEndsConstants** pGeomLayerEnds =  getGeomLayerEnds(theFrame);

  // Fill the End Cap Pointers
  ADREndCapPointers p =  fillEndCapPointers( drDetectorStore);

  // Make the senseWireStore 
  // cout <<"ADRSenseWireFactory makes SenseWireStore"<<endl;
  m_ADRSenseWireStore =
    new ADRSenseWireStore(p.spCake1aEast(), p.spCake1aWest(), 
			  p.spCake2aEast(), p.spCake2aWest(),
			  p.spCake3aEast(), p.spCake3aWest(), 
			  p.spCake4aEast(), p.spCake4aWest(),
			  p.spCake5aEast(), p.spCake5aWest(), 
			  p.spCake6aEast(), p.spCake6aWest(),
			  p.spCake7aEast(), p.spCake7aWest(), 
			  p.spCake8aEast(), p.spCake8aWest(),
			  p.spConicalEndPlateEast1(),
			  p.spConicalEndPlateWest1(),
			  pWireSurveyConstants,
			  pGeomLayer,
			  pGeomLayerEnds);
			  
  // Note that the responsiblilty for deleting pWireSurveyConstants 
  // and stuff therein is handed off to ADRSenseWireStore.
}

// ADRSenseWireStoreFactory::ADRSenseWireStoreFactory( const ADRSenseWireStoreFactory& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

ADRSenseWireStoreFactory::~ADRSenseWireStoreFactory()
{
delete  m_ADRSenseWireStore;
}

//
// assignment operators
//
// const ADRSenseWireStoreFactory& ADRSenseWireStoreFactory::operator=( const ADRSenseWireStoreFactory& rhs )
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

void ADRSenseWireStoreFactory::updateSenseWireStore(			    
  const DGDetectorStore& drDetectorStore)
{
  // Fill the End Cap Pointers
  ADREndCapPointers p = fillEndCapPointers(drDetectorStore);

  //  cout<<"ADRSenseWireFactoryStore updates ADRSenseWireStore"<<endl;
  // Update the ADRSenseWireStore
  m_ADRSenseWireStore->updateSenseWireStore
    (p.spCake1aEast(), p.spCake2aEast(), p.spCake3aEast(),
     p.spCake4aEast(), p.spCake5aEast(), p.spCake6aEast(),
     p.spCake7aEast(), p.spCake8aEast(), 
     p.spConicalEndPlateEast1(),
     p.spCake1aWest(), p.spCake2aWest(), p.spCake3aWest(),
     p.spCake4aWest(), p.spCake5aWest(), p.spCake6aWest(),
     p.spCake7aWest(), p.spCake8aWest(), 
     p.spConicalEndPlateWest1());
}


//
// const member functions
//

ADRWireSurveyConstants** ADRSenseWireStoreFactory::getWireSurveyConstants(
					    const Frame& theFrame) const
{
  //  cout<<"Factory reading in the constants"<<endl;
  // Load in the constants
  FAConstants< DBADRGeomWireSurvey> iADRGeomWireSurveyConstants;
  extract( theFrame.record( Stream::kBaseGeometry), 
	         iADRGeomWireSurveyConstants); 
  /*
  // Did we get them?
  if(! iADRGeomWireSurveyConstants.valid())
  {
    report( WARNING, kFacilityString )
      << "Could not find DBADRGeomWireSurvey in record " << endl;
    assert(iADRGeomWireSurveyConstants.valid());
  }
  */
  ADRWireSurveyConstants** pWireSurveyConstants = 
    new ADRWireSurveyConstants*[ADRSenseWireStore::kNumberOfWiresTotal];    

  
  // Parse and make a local copy to pass to ADRSenseWireStore 
  // (which is responsible for deleting these things).
  for(  int ix=0; ix <ADRSenseWireStore::kNumberOfWiresTotal  ; ix++)  
  {    
    ADRWireSurveyConstants* pWC = new ADRWireSurveyConstants(
		iADRGeomWireSurveyConstants[ix].get_DRWireNumber(),
		iADRGeomWireSurveyConstants[ix].get_EastXMicron(),
		iADRGeomWireSurveyConstants[ix].get_EastYMicron(),
		iADRGeomWireSurveyConstants[ix].get_EastZMicron(),
		iADRGeomWireSurveyConstants[ix].get_WestXMicron(),
		iADRGeomWireSurveyConstants[ix].get_WestYMicron(),
		iADRGeomWireSurveyConstants[ix].get_WestZMicron());
    pWireSurveyConstants[ix] =  pWC;
  }



  return pWireSurveyConstants;
}

ADREndCapPointers ADRSenseWireStoreFactory::fillEndCapPointers(
			       const DGDetectorStore& detectorStore) const
{

  //Get the Conical Endplate = Endcaps for stereo layers 17 - 47 
  //West Conical Endplate 
  DGPath pathToWestConicalEndPlate   = 
    detectorStore.findPathTo(
       DRGeometryParams::kConicalEndplateWestName,
       DRGeometryParams::kDefaultVolumeID); 
  DGPath pathToWestConicalEndPlate1  = pathToWestConicalEndPlate;

  pathToWestConicalEndPlate1.addNodeToEnd(DGPath::Node(
     DRGeometryParams::kConicalEndplatePiece1Name,
     DRGeometryParams::kDefaultVolumeID));

  //East Conical Endplate 
  DGPath pathToEastConicalEndPlate   = 
     detectorStore.findPathTo(
	DRGeometryParams::kConicalEndplateEastName,
	DRGeometryParams::kDefaultVolumeID);
  DGPath pathToEastConicalEndPlate1  = pathToEastConicalEndPlate;
  
  pathToEastConicalEndPlate1.addNodeToEnd(DGPath::Node(
     DRGeometryParams::kConicalEndplatePiece1Name,
     DRGeometryParams::kDefaultVolumeID));

  if( (0 == pathToEastConicalEndPlate1.numberOfNodes() ) || 
      (0 == pathToWestConicalEndPlate1.numberOfNodes() ) )
  {
    report( WARNING, kFacilityString )
      << "Could not find Conical EndPlate1 in DGDetectorStore DR"
      << endl;
    assert(false);
  }

  const DGConstLabVolumePtr spConicalEndPlateEast1 = 
    detectorStore.labVolume( pathToEastConicalEndPlate1 );
  
  const DGConstLabVolumePtr spConicalEndPlateWest1 =
    detectorStore.labVolume( pathToWestConicalEndPlate1 );

  // create const pointers to PolyCones
  const DGConstVolumeType< DGVolumeParentPlaced >
     parEndPlateWest1( spConicalEndPlateWest1->volume() ) ;
  
  const DGConstVolumeType< DGVolumeParentPlaced >
     parEndPlateEast1( spConicalEndPlateEast1->volume() ) ;

  const DGConstVolumeType< DGVolumePolyCone >
    spPolyConicalEndPlateWest1( parEndPlateWest1->boundingVolume() ) ;
  
  const DGConstVolumeType< DGVolumePolyCone >
    spPolyConicalEndPlateEast1( parEndPlateEast1->boundingVolume() ) ;
  
  if( !spPolyConicalEndPlateEast1.isValid() ||
      !spPolyConicalEndPlateWest1.isValid() )
  {
    // The geometry has a different description than what
    // we expected.
    // This is a programming error.
    report( EMERGENCY, kFacilityString )
      << "DR3 BaseGeometry's " 
      << (spConicalEndPlateWest1->volume())->name() << " or "
      << (spConicalEndPlateEast1->volume())->name()
      << " is not a DGVolumePolyCone" << endl;
    assert(spPolyConicalEndPlateEast1.isValid() &&
	   spPolyConicalEndPlateWest1.isValid() );
  }
  
  //Get the CakeXa = Endcaps for axial layers, X = 1 to 8
				     
  //West Wedding Cake 
  DGPath pathToWestCake   = 
    detectorStore.findPathTo(
			     DRGeometryParams::kWeddingCakeName,
			     DRGeometryParams::kWestID);
  
  DGPath pathToWestCake1a = pathToWestCake;
  DGPath pathToWestCake2a = pathToWestCake;
  DGPath pathToWestCake3a = pathToWestCake;
  DGPath pathToWestCake4a = pathToWestCake;
  DGPath pathToWestCake5a = pathToWestCake;
  DGPath pathToWestCake6a = pathToWestCake;
  DGPath pathToWestCake7a = pathToWestCake;
  DGPath pathToWestCake8a = pathToWestCake;

  pathToWestCake1a.addNodeToEnd( DGPath::Node(
					 DRGeometryParams::kCake1Name,
				         DRGeometryParams::kDefaultVolumeID));
  pathToWestCake2a.addNodeToEnd( DGPath::Node(
				         DRGeometryParams::kCake2Name,
					 DRGeometryParams::kDefaultVolumeID));
  pathToWestCake3a.addNodeToEnd( DGPath::Node(
					 DRGeometryParams::kCake3Name,
					 DRGeometryParams::kDefaultVolumeID));
  pathToWestCake4a.addNodeToEnd( DGPath::Node(
					 DRGeometryParams::kCake4Name,
					 DRGeometryParams::kDefaultVolumeID));
  pathToWestCake5a.addNodeToEnd( DGPath::Node(
					 DRGeometryParams::kCake5Name,
					 DRGeometryParams::kDefaultVolumeID));
  pathToWestCake6a.addNodeToEnd( DGPath::Node(
					 DRGeometryParams::kCake6Name,
					 DRGeometryParams::kDefaultVolumeID));
  pathToWestCake7a.addNodeToEnd( DGPath::Node(
					 DRGeometryParams::kCake7Name,
					 DRGeometryParams::kDefaultVolumeID));
  pathToWestCake8a.addNodeToEnd( DGPath::Node(
					 DRGeometryParams::kCake8Name,
					 DRGeometryParams::kDefaultVolumeID));
  
  pathToWestCake1a.addNodeToEnd( DGPath::Node(
					 DRGeometryParams::kCake1aName,
					 DRGeometryParams::kDefaultVolumeID));
  pathToWestCake2a.addNodeToEnd( DGPath::Node(
					 DRGeometryParams::kCake2aName,
					 DRGeometryParams::kDefaultVolumeID));
  pathToWestCake3a.addNodeToEnd( DGPath::Node(
					 DRGeometryParams::kCake3aName,
					 DRGeometryParams::kDefaultVolumeID));
  pathToWestCake4a.addNodeToEnd( DGPath::Node(
					 DRGeometryParams::kCake4aName,
					 DRGeometryParams::kDefaultVolumeID));
  pathToWestCake5a.addNodeToEnd( DGPath::Node(
					 DRGeometryParams::kCake5aName,
					 DRGeometryParams::kDefaultVolumeID));
  pathToWestCake6a.addNodeToEnd( DGPath::Node(
					 DRGeometryParams::kCake6aName,
					 DRGeometryParams::kDefaultVolumeID));
  pathToWestCake7a.addNodeToEnd( DGPath::Node(
					 DRGeometryParams::kCake7aName,
					 DRGeometryParams::kDefaultVolumeID));
  pathToWestCake8a.addNodeToEnd( DGPath::Node(
					 DRGeometryParams::kCake8aName,
					 DRGeometryParams::kDefaultVolumeID));
  //East Wedding Cake 
  DGPath pathToEastCake   = detectorStore.findPathTo(
					   DRGeometryParams::kWeddingCakeName,
					   DRGeometryParams::kEastID);

  DGPath pathToEastCake1a = pathToEastCake;
  DGPath pathToEastCake2a = pathToEastCake;
  DGPath pathToEastCake3a = pathToEastCake;
  DGPath pathToEastCake4a = pathToEastCake;
  DGPath pathToEastCake5a = pathToEastCake;
  DGPath pathToEastCake6a = pathToEastCake;
  DGPath pathToEastCake7a = pathToEastCake;
  DGPath pathToEastCake8a = pathToEastCake;
  
  pathToEastCake1a.addNodeToEnd( DGPath::Node(
					 DRGeometryParams::kCake1Name,
					 DRGeometryParams::kDefaultVolumeID));
  pathToEastCake2a.addNodeToEnd( DGPath::Node(
					 DRGeometryParams::kCake2Name,
					 DRGeometryParams::kDefaultVolumeID));
  pathToEastCake3a.addNodeToEnd( DGPath::Node(
					 DRGeometryParams::kCake3Name,
					 DRGeometryParams::kDefaultVolumeID));
  pathToEastCake4a.addNodeToEnd( DGPath::Node(
					 DRGeometryParams::kCake4Name,
					 DRGeometryParams::kDefaultVolumeID));
  pathToEastCake5a.addNodeToEnd( DGPath::Node(
					 DRGeometryParams::kCake5Name,
					 DRGeometryParams::kDefaultVolumeID));
  pathToEastCake6a.addNodeToEnd( DGPath::Node(
					 DRGeometryParams::kCake6Name,
					 DRGeometryParams::kDefaultVolumeID));
  pathToEastCake7a.addNodeToEnd( DGPath::Node(
					 DRGeometryParams::kCake7Name,
					 DRGeometryParams::kDefaultVolumeID));
  pathToEastCake8a.addNodeToEnd( DGPath::Node(
					 DRGeometryParams::kCake8Name,
					 DRGeometryParams::kDefaultVolumeID));
  
  pathToEastCake1a.addNodeToEnd( DGPath::Node(
					 DRGeometryParams::kCake1aName,
					 DRGeometryParams::kDefaultVolumeID));
  pathToEastCake2a.addNodeToEnd( DGPath::Node(
					 DRGeometryParams::kCake2aName,
					 DRGeometryParams::kDefaultVolumeID));
  pathToEastCake3a.addNodeToEnd( DGPath::Node(
					 DRGeometryParams::kCake3aName,
					 DRGeometryParams::kDefaultVolumeID));
  pathToEastCake4a.addNodeToEnd( DGPath::Node(
					 DRGeometryParams::kCake4aName,
					 DRGeometryParams::kDefaultVolumeID));
  pathToEastCake5a.addNodeToEnd( DGPath::Node(
					 DRGeometryParams::kCake5aName,
					 DRGeometryParams::kDefaultVolumeID));
  pathToEastCake6a.addNodeToEnd( DGPath::Node(
					 DRGeometryParams::kCake6aName,
					 DRGeometryParams::kDefaultVolumeID));
  pathToEastCake7a.addNodeToEnd( DGPath::Node(
					 DRGeometryParams::kCake7aName,
					 DRGeometryParams::kDefaultVolumeID));
  pathToEastCake8a.addNodeToEnd( DGPath::Node(
					 DRGeometryParams::kCake8aName,
					 DRGeometryParams::kDefaultVolumeID));
  

  if( (0 == pathToEastCake1a.numberOfNodes() ) || 
      (0 == pathToWestCake1a.numberOfNodes() ) ||
      (0 == pathToEastCake2a.numberOfNodes() ) || 
      (0 == pathToWestCake2a.numberOfNodes() ) ||
      (0 == pathToEastCake3a.numberOfNodes() ) || 
      (0 == pathToWestCake3a.numberOfNodes() ) ||
      (0 == pathToEastCake4a.numberOfNodes() ) || 
      (0 == pathToWestCake4a.numberOfNodes() ) ||
      (0 == pathToEastCake5a.numberOfNodes() ) || 
      (0 == pathToWestCake5a.numberOfNodes() ) ||
      (0 == pathToEastCake6a.numberOfNodes() ) || 
      (0 == pathToWestCake6a.numberOfNodes() ) ||
      (0 == pathToEastCake7a.numberOfNodes() ) || 
      (0 == pathToWestCake7a.numberOfNodes() ) ||
      (0 == pathToEastCake8a.numberOfNodes() ) || 
      (0 == pathToWestCake8a.numberOfNodes() ) )
  {
    report( WARNING, kFacilityString )
      << "Could not find WeddingCake slices in DGDetectorStore DR"
      << endl;
    assert(false);
  }
  //  cout<<"ADRSenseWireStoryFactory making ADREndCapPointers"<<endl;
  return ADREndCapPointers(
			   detectorStore.labVolume( pathToEastCake1a ),
			   detectorStore.labVolume( pathToEastCake2a ),
			   detectorStore.labVolume( pathToEastCake3a ),
			   detectorStore.labVolume( pathToEastCake4a ),
			   detectorStore.labVolume( pathToEastCake5a ),
			   detectorStore.labVolume( pathToEastCake6a ),
			   detectorStore.labVolume( pathToEastCake7a ),
			   detectorStore.labVolume( pathToEastCake8a ),
			   detectorStore.labVolume( pathToWestCake1a ),
			   detectorStore.labVolume( pathToWestCake2a ),
			   detectorStore.labVolume( pathToWestCake3a ),
			   detectorStore.labVolume( pathToWestCake4a ),
			   detectorStore.labVolume( pathToWestCake5a ),
			   detectorStore.labVolume( pathToWestCake6a ),
			   detectorStore.labVolume( pathToWestCake7a ),
			   detectorStore.labVolume( pathToWestCake8a ),
			   spConicalEndPlateEast1,
			   spConicalEndPlateWest1 
			   );
}


ADRLayerConstants** ADRSenseWireStoreFactory::getGeomLayer(
					    const Frame& theFrame) const
{
  //    cout<<"Factory reading in the ADRGeomLayer constants"<<endl;
  // Load in the constants
  FAConstants< DBADRGeomLayer> iADRGeomLayerConstants;
  extract( theFrame.record( Stream::kBaseGeometry), 
	         iADRGeomLayerConstants); 
  
  // Did we get them?  
  if(! iADRGeomLayerConstants.valid())
  {
    report( WARNING, kFacilityString )
     << "Could not find DBADRGeomLayer in record " << endl;    
    assert(iADRGeomLayerConstants.valid());
  }
  
  ADRLayerConstants** pLayerConstants = 
    new ADRLayerConstants*[ADRSenseWireStore::kNumberOfLayers];    

  
  // Parse and make a local copy to pass to ADRSenseWireStore 
  // (which is responsible for deleting these things).
  
    report( DEBUG, kFacilityString )
      << "     ADRGeomLayer Constants: 1.Input iADRGeomLayerConstants 2. ADRLayerConstants  " <<endl
      << "Lyr Nwr REnd RECcm PEDeg   PWDeg Twst Cake Zstp ZBow ZBsh ZPin PnWrRad Sgx Sgy  RFMn RFMx"<< endl;    


  for(  int ix=0; ix <ADRSenseWireStore::kNumberOfLayers  ; ix++)  
  {     
      //Note: in the constants, DRLayerNumber = 0,46 = index in the const file
    ADRLayerConstants* pGL = new ADRLayerConstants(
		iADRGeomLayerConstants[ix].get_DRLayerNumber(),
		iADRGeomLayerConstants[ix].get_NumberOfWires(),
		iADRGeomLayerConstants[ix].get_ReadoutEnd(),
		iADRGeomLayerConstants[ix].get_RadiusEndCapCM(),
		iADRGeomLayerConstants[ix].get_Phi0EastInDegrees(),
		iADRGeomLayerConstants[ix].get_Phi0WestInDegrees(),
		iADRGeomLayerConstants[ix].get_TwistCakeInDegrees(),
		iADRGeomLayerConstants[ix].get_CakeNumber_Axial(),
		iADRGeomLayerConstants[ix].get_ZStepPolycone_StereoLayer(),
		iADRGeomLayerConstants[ix].get_ZBowingInch(),
		iADRGeomLayerConstants[ix].get_ZBushing(),
		iADRGeomLayerConstants[ix].get_ZPin(),
		iADRGeomLayerConstants[ix].get_Pin_WireRadiusInch(),
		iADRGeomLayerConstants[ix].get_SagXMicron(),
		iADRGeomLayerConstants[ix].get_SagYMicron(),
		iADRGeomLayerConstants[ix].get_FieldRadiusLessEndCapCM(),
		iADRGeomLayerConstants[ix].get_FieldRadiusMoreEndCapCM());
   report( DEBUG, kFacilityString )
     <<setw(2)<<iADRGeomLayerConstants[ix].get_DRLayerNumber()<<" "
     <<setw(3)<<iADRGeomLayerConstants[ix].get_NumberOfWires()<<" "
     <<setw(3)<<iADRGeomLayerConstants[ix].get_ReadoutEnd()<<" "
     <<setw(5)<<iADRGeomLayerConstants[ix].get_RadiusEndCapCM()<<" "
     <<setw(7)<<iADRGeomLayerConstants[ix].get_Phi0EastInDegrees()<<" "
     <<setw(7)<<iADRGeomLayerConstants[ix].get_Phi0WestInDegrees()<<" "
     <<setw(5)<<iADRGeomLayerConstants[ix].get_TwistCakeInDegrees()<<" "
     <<setw(2)<<iADRGeomLayerConstants[ix].get_CakeNumber_Axial()<<" "
     <<setw(3)<<iADRGeomLayerConstants[ix].get_ZStepPolycone_StereoLayer()<<" "
     <<setw(2)<<iADRGeomLayerConstants[ix].get_ZBowingInch()<<" "
     <<setw(2)<<iADRGeomLayerConstants[ix].get_ZBushing()<<" "
     <<setw(2)<<iADRGeomLayerConstants[ix].get_ZPin()<<" "
     <<setw(7)<<iADRGeomLayerConstants[ix].get_Pin_WireRadiusInch()<<" "
     <<setw(2)<<iADRGeomLayerConstants[ix].get_SagXMicron()<<" "
     <<setw(9)<<iADRGeomLayerConstants[ix].get_SagYMicron()<<" "
     <<setw(5)<<iADRGeomLayerConstants[ix].get_FieldRadiusLessEndCapCM()<<" "
     <<setw(2)<<iADRGeomLayerConstants[ix].get_FieldRadiusMoreEndCapCM()<<endl;
   report( DEBUG, kFacilityString )
     <<setw(2)<<pGL->dRLayerNumber()<<" "
     <<setw(3)<<pGL->numberOfWires()<<" "
     <<setw(3)<<pGL->readoutEnd()<<" "
     <<setw(5)<<pGL->radiusEndCapCM()<<" "
     <<setw(7)<<pGL->phi0EastInDegrees()<<" "
     <<setw(7)<<pGL->phi0WestInDegrees()<<" "
     <<setw(5)<<pGL->twistCakeInDegrees()<<" "
     <<setw(2)<<pGL->cakeNumber_Axial()<<" "
     <<setw(3)<<pGL->zStepPolycone_StereoLayer()<<" "
     <<setw(2)<<pGL->zBowingInch()<<" "
     <<setw(2)<<pGL->zBushing()<<" "
     <<setw(2)<<pGL->zPin()<<" "
     <<setw(7)<<pGL->pin_WireRadiusInch()<<" "
     <<setw(2)<<pGL->sagXMicron()<<" "
     <<setw(9)<<pGL->sagYMicron()<<" "
     <<setw(5)<<pGL->fieldRadiusLessEndCapCM()<<" "
     <<setw(2)<<pGL->fieldRadiusMoreEndCapCM()<<endl;

    pLayerConstants[ix] =  pGL;
  }
  return pLayerConstants;
}


ADRGeomLayersEndsConstants** ADRSenseWireStoreFactory::getGeomLayerEnds(
						     const Frame& theFrame) const
{
  //  cout<<"Factory reading in the ADRGeomLayersEnds constants"<<endl;

  // Load in the constants
  FAConstants< DBADRGeomLayersEnds> iADRGeomLayersEndsConstants;
  extract( theFrame.record( Stream::kBaseGeometry), 
	   iADRGeomLayersEndsConstants); 
  
  // Did we get them?  
  if(! iADRGeomLayersEndsConstants.valid())
    {
      report( WARNING, kFacilityString )
	<< "Could not find DBADRGeomLayersEnds in record " << endl;    
      assert(iADRGeomLayersEndsConstants.valid());
    }
	
  ADRGeomLayersEndsConstants** pGeomLayersEnds = 
    new ADRGeomLayersEndsConstants*[ADRSenseWireStore::kNumberOfLayers];    
    
  // Parse and make a local copy to pass to ADRSenseWireStore 
  // (which is responsible for deleting these things). 
 
  report( DEBUG, kFacilityString )
    << "     ADRGeomLayersEnds Constants" <<endl
    << " Lyr RECmm    XWmm      YWmm    ZWmm     XEmm       YEmm "<< endl; 
  for(  int ix=0; ix <ADRSenseWireStore::kNumberOfLayers  ; ix++)  
    {    
      //Note: in the constants, DRLayerNumber = 1,47 = index in the const file
      ADRGeomLayersEndsConstants* pGLE = new ADRGeomLayersEndsConstants(
		       iADRGeomLayersEndsConstants[ix+1].get_DRLayerNumber(),
		       iADRGeomLayersEndsConstants[ix+1].get_Radius_MM() ,
		       iADRGeomLayersEndsConstants[ix+1].get_X_WestMM(),
		       iADRGeomLayersEndsConstants[ix+1].get_Y_WestMM(),
		       iADRGeomLayersEndsConstants[ix+1].get_Z_WestMM(),
		       iADRGeomLayersEndsConstants[ix+1].get_X_EastMM(),
		       iADRGeomLayersEndsConstants[ix+1].get_Y_EastMM()) ;
      report( DEBUG, kFacilityString )
	<<setw(2)<<iADRGeomLayersEndsConstants[ix+1].get_DRLayerNumber()<<"."
	<<setw(8)<<iADRGeomLayersEndsConstants[ix+1].get_Radius_MM()<<" " 	
	<<setw(8)<<iADRGeomLayersEndsConstants[ix+1].get_X_WestMM()<<" "	
	<<setw(8)<<iADRGeomLayersEndsConstants[ix+1].get_Y_WestMM()<<" "
	<<setw(8)<<iADRGeomLayersEndsConstants[ix+1].get_Z_WestMM()<<" "
	<<setw(8)<<iADRGeomLayersEndsConstants[ix+1].get_X_EastMM()<<" "
	<<setw(8)<<iADRGeomLayersEndsConstants[ix+1].get_Y_EastMM()<<endl;
      
      report( DEBUG, kFacilityString )
	<<setw(2)<<pGLE->dRLayerNumber()<<"."
	<<setw(8)<<pGLE->radius0MM()<<" " 	
	<<setw(8)<<pGLE->x0_WestMM()<<" "	
	<<setw(8)<<pGLE->y0_WestMM()<<" "
	<<setw(8)<<pGLE->z0_WestMM()<<" "
	<<setw(8)<<pGLE->x0_EastMM()<<" "
	<<setw(8)<<pGLE->y0_EastMM()<<endl;
      
      pGeomLayersEnds[ix] =  pGLE;
      
    }
  return pGeomLayersEnds;
}




