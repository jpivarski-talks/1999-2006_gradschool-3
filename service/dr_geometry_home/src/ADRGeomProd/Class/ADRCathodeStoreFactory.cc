// -*- C++ -*-
//
// Package:     ADRGeomProd
// Module:      ADRCathodeStoreFactory
// 
// Description: makes ADRCathodeStore
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Adam Lyon and Inga Karliner
// Created:     Tue Nov 16 13:48:59 EST 1999
// $Id: ADRCathodeStoreFactory.cc,v 1.6 2003/01/27 19:43:02 cdj Exp $
//
// Revision history
//
// $Log: ADRCathodeStoreFactory.cc,v $
// Revision 1.6  2003/01/27 19:43:02  cdj
// get iomanip.h from C++Std package
//
// Revision 1.5  2002/11/21 21:01:41  cleo3
// changed fstream.h to C++Std/fstream.h
//
// Revision 1.4  2001/09/13 22:36:59  inga
// read panel constants from the frame
//
// Revision 1.3  2001/04/25 02:31:49  inga
// change reference to ConicalEndPiece2Name following DRGeom reorganization, fix typo
//
// Revision 1.2  2000/12/11 21:22:41  inga
// add pointer to the ConicalEndPlateEast
//
// Revision 1.1.1.1  1999/12/08 17:23:09  inga
// import ADRGeomProd sources
//
// Revision 1.1  1999/11/29 18:37:06  inga
// add ADRCathodeStoreFactory.cc
// 12/1/99  I.Karliner New library ADRGeomProd (was in ADRGeom) 
// 12/11/00 I. Karliner
//   Add pointer DGConstLabVolumePtr  p.spConicalEndPlateEast
// 4/24/01 I.Karliner
//   Build cathode using modified DRGeom
//   Change over to DGConstLabVolumePtr p.spEastConicalEndCapPiece2Name to build the Cathode
//   after DRGeom adjusted for the material between DR and CC 
//   + fix a typo 
// 9/13/01 use 8 cathode panels read constants from 

#include "Experiment/Experiment.h"

// system include files
#include <assert.h>
#include "C++Std/iomanip.h" 
#include "C++Std/fstream.h"

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files

#include "ADRGeomProd/ADRCathodeStoreFactory.h"
#include "ADRGeom/ADRCathodePanelConstants.h"
#include "ADRGeom/ADRCathodeParams.h"

#include "Experiment/report.h"
#include "DataHandler/Record.h"
#include "FrameAccess/extract.h"           
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"
#include "DataHandler/Frame.h"

#include "FrameAccess/FAConstants.h"
#include "DBADRGeomCathodePanels.hh"

#include "DRGeom/DRGeometryParams.h"
#include "DetectorGeometry/DGDetectorStore.h"
#include "DetectorGeometry/DGVolumePtr.h"
#include "DetectorGeometry/DGLabVolume.h"
#include "DetectorGeometry/DGConstLabVolumePtr.h"
#include "DetectorGeometry/DGVolume.h"
#include "DetectorGeometry/DGVolumeTubePhiSegment.h"
#include "DetectorGeometry/DGConstVolumePtr.h"
#include "DetectorGeometry/DGPath.h"
#include "DetectorGeometry/DGVolumePolyCone.h"
#include "DetectorGeometry/DGVolumeType.h"
#include "DetectorGeometry/DGZrRList.h"
#include "DetectorGeometry/DGVolumeTypeInfo.h"
#include "DetectorGeometry/DGVolumeParent.h"

// STL classes

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "ADRGeomProd.ADRCathodeStoreFactory" ;

//
// static data member definitions
//
//
// constructors and destructor
//

ADRCathodeStoreFactory::ADRCathodeStoreFactory(
			    const DGDetectorStore& drDetectorStore,
			    const Frame& theFrame) :
  m_ADRCathodeStore(0)
{
  // Get the cathode panel constants

  ADRCathodePanelConstants** pCathodePanels = getCathodePanelConst(theFrame);
  // Fill the cathode pointers
  ADRCathodePointers p = fillCathodePointers(drDetectorStore);

  // Get the tube information
  const DGConstVolumeType< DGVolumeTubePhiSegment > 
    spTubePhiCathodePanel0 (getTube(p.spCathodePanel0()));

   if ( !spTubePhiCathodePanel0.isValid()  )
   {
     report( EMERGENCY, kFacilityString )
       << "DR3 BaseGeometry's one of" 
       << (getTube(p.spCathodePanel0()))->name() <<endl
       << " is not a DGVolumeTubePhiSegment" << endl;
     assert( spTubePhiCathodePanel0.isValid() );
   }    
   
   // Make the cathode store
   m_ADRCathodeStore = 
     new ADRCathodeStore(p.spCathodePanel0(),
			 p.spCathodePanel1(),
			 p.spCathodePanel2(),
			 p.spCathodePanel3(),
			 p.spCathodePanel4(),
			 p.spCathodePanel5(),
			 p.spCathodePanel6(),
			 p.spCathodePanel7(),
			 (*spTubePhiCathodePanel0).innerRadius(),
			 (*spTubePhiCathodePanel0).phiMin(),
			 (*spTubePhiCathodePanel0).deltaPhi(),
			 (*spTubePhiCathodePanel0).halfLengthZ(),
			 p.spConicalEndPlateEast(),
                         pCathodePanels
			 );
}


// ADRCathodeStoreFactory::ADRCathodeStoreFactory( const ADRCathodeStoreFactory& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

ADRCathodeStoreFactory::~ADRCathodeStoreFactory()
{
  delete m_ADRCathodeStore;
}

//
// assignment operators
//
// const ADRCathodeStoreFactory& 
//   ADRCathodeStoreFactory::operator=( const ADRCathodeStoreFactory& rhs )
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

void ADRCathodeStoreFactory::updateCathodeStore(
			      const DGDetectorStore& drDetectorStore,
			      const Frame& theFrame)
{
  // Get the cathode panel constants

  ADRCathodePanelConstants** pCathodePanels = getCathodePanelConst(theFrame);

  // Fill the cathode pointers
  ADRCathodePointers p = fillCathodePointers(drDetectorStore);

  const DGConstVolumeType< DGVolumeTubePhiSegment > 
    spTubePhiCathodePanel0 (getTube(p.spCathodePanel0()));

   if ( !spTubePhiCathodePanel0.isValid()  )
   {
     report( EMERGENCY, kFacilityString )
       << "DR3 BaseGeometry's one of" 
       << (getTube(p.spCathodePanel0()))->name() <<endl
       << " is not a DGVolumeTubePhiSegment" << endl;
     assert( spTubePhiCathodePanel0.isValid() );
   }
  m_ADRCathodeStore->setCathode(p.spCathodePanel0(),
				p.spCathodePanel1(),
				p.spCathodePanel2(),
				p.spCathodePanel3(),
				p.spCathodePanel4(),
				p.spCathodePanel5(),
				p.spCathodePanel6(),
				p.spCathodePanel7(),
				(*spTubePhiCathodePanel0).innerRadius(),
				(*spTubePhiCathodePanel0).phiMin(),
				(*spTubePhiCathodePanel0).deltaPhi(),
				(*spTubePhiCathodePanel0).halfLengthZ(),
				p.spConicalEndPlateEast(),
				pCathodePanels
				);
}


ADRCathodePointers ADRCathodeStoreFactory::fillCathodePointers(
			      const DGDetectorStore& spDRStore) const
{

  // find the cathode panels:

  DGPath pathToCathodePanel0         =
    spDRStore.findPathTo(  DRGeometryParams::kCathodePanelName,
			    ADRCathodeStore::kPanel0);
  DGPath pathToCathodePanel1         =
    spDRStore.findPathTo(  DRGeometryParams::kCathodePanelName,
			    ADRCathodeStore::kPanel1);
  DGPath pathToCathodePanel2         =
    spDRStore.findPathTo(  DRGeometryParams::kCathodePanelName,
			    ADRCathodeStore::kPanel2);
  DGPath pathToCathodePanel3         =
    spDRStore.findPathTo(  DRGeometryParams::kCathodePanelName,
			    ADRCathodeStore::kPanel3);
  DGPath pathToCathodePanel4         =
    spDRStore.findPathTo(  DRGeometryParams::kCathodePanelName,
			     ADRCathodeStore::kPanel4);
  DGPath pathToCathodePanel5         =
    spDRStore.findPathTo(  DRGeometryParams::kCathodePanelName,
			    ADRCathodeStore::kPanel5);
  DGPath pathToCathodePanel6         =
    spDRStore.findPathTo(  DRGeometryParams::kCathodePanelName,
			    ADRCathodeStore::kPanel6);
  DGPath pathToCathodePanel7         =
    spDRStore.findPathTo(  DRGeometryParams::kCathodePanelName,
			    ADRCathodeStore::kPanel7);

  if( (0 == pathToCathodePanel0.numberOfNodes() ) ||
      (0 == pathToCathodePanel1.numberOfNodes() ) ||
      (0 == pathToCathodePanel2.numberOfNodes() ) ||
      (0 == pathToCathodePanel3.numberOfNodes() ) ||
      (0 == pathToCathodePanel4.numberOfNodes() ) ||
      (0 == pathToCathodePanel5.numberOfNodes() ) ||
      (0 == pathToCathodePanel6.numberOfNodes() ) ||
      (0 == pathToCathodePanel7.numberOfNodes() ) )
  {
    report( WARNING, kFacilityString )
      << "Could not find Cathode Panels  in DGDetectorStore DR"
      << endl;
    assert(false);
  }

  // find East Conical End Plate PieceName2  
  // update to reflect new DRGeom treatment of the DR bounding volume:

  /*  DGPath pathToEastConicalEndPlate   =

    spDRStore.findPathTo(
			     DRGeometryParams::kConicalEndplateName,
			     DRGeometryParams::kEastID);
  DGPath pathToEastConicalEndPlate1  = pathToEastConicalEndPlate;
  
  pathToEastConicalEndPlate1.addNodeToEnd(DGPath::Node(
			     DRGeometryParams::kConicalEndplatePiece1Name ,
			     DRGeometryParams::kDefaultVolumeID));
  if( (0 == pathToEastConicalEndPlate1.numberOfNodes() ) )
  {
    report( WARNING, kFacilityString )
      << "Could not find Conical EndPlate1 in DGDetectorStore DR"
      << endl;
    assert(false);
  }

  const DGConstLabVolumePtr spConicalEndPlateEast1 =
    spDRStore.labVolume( pathToEastConicalEndPlate1 );
    */
  // create const pointers to PolyCones
  /*
  const DGConstVolumeType< DGVolumePolyCone >
    spPolyConicalEndPlateEast1(spConicalEndPlateEast1->volume() ) ;
    */  
  //  if( !spPolyConicalEndPlateEast1.isValid() )
  //    {
  //   // The geometry has a different description than what
  //  // we expected.
  //  // This is a programming error.
  //    report( EMERGENCY, kFacilityString )
  //      << "DR3 BaseGeometry's "
  //      << (spConicalEndPlateEast1->volume())->name()
  //      << " is not a DGVolumePolyCone" << endl;
  //    assert(spPolyConicalEndPlateEast1.isValid()  );
  //      }

  DGPath path = spDRStore.findPathTo(
				     DRGeometryParams::kConicalEndplatePiece2Name ,
				     DRGeometryParams::kEastID );
  
  return ADRCathodePointers(spDRStore.labVolume( pathToCathodePanel0 ),
			    spDRStore.labVolume( pathToCathodePanel1 ),
			    spDRStore.labVolume( pathToCathodePanel2 ),
			    spDRStore.labVolume( pathToCathodePanel3 ),
			    spDRStore.labVolume( pathToCathodePanel4 ),
			    spDRStore.labVolume( pathToCathodePanel5 ),
			    spDRStore.labVolume( pathToCathodePanel6 ),
			    spDRStore.labVolume( pathToCathodePanel7 ),
			    //			    spConicalEndPlateEast1
			    spDRStore.labVolume( path ) 
			    );
} 


const DGConstVolumePtr&  
ADRCathodeStoreFactory::getTube(
			    const DGConstLabVolumePtr& spCathodePanel0) const
{
  // create const pointers to TubePhiSegment
  //
  // All panels (phi segments) are made the same, use panel0. 
  // If the panels are different, each has to be found 
  //
  // first convert DG Volume PlacedParent to DGVolume Parent

  const DGConstVolumeType< DGVolumeParent > 
    spCathodePanel0ParentVolume ( spCathodePanel0->volume()) ;
	
   if( !spCathodePanel0ParentVolume.isValid() ) 
   {
     // The geometry has a different description than what
     // we expected.
     // This is a programming error. 
     report( EMERGENCY, kFacilityString )
       << "getTube: DR3 BaseGeometry's  " 
       << (spCathodePanel0->volume())->name()<<endl
       << " is not a DGVolumeParent" << endl;
     assert(spCathodePanel0ParentVolume.isValid());

   }
   
   // Construct bounding volumes
   return spCathodePanel0ParentVolume->boundingVolume(); 
}
  
ADRCathodePanelConstants** ADRCathodeStoreFactory::getCathodePanelConst(
						     const Frame& theFrame) const
{
    report( DEBUG, kFacilityString )
      <<"Factory reading in the ADRCathodePanelConstants"<<endl;
  
  // Load in the constants
  FAConstants< DBADRGeomCathodePanels> iADRCathodePanelConst;
  extract( theFrame.record( Stream::kBaseGeometry), 
           iADRCathodePanelConst);
  
  // Did we get them?  
  if(! iADRCathodePanelConst.valid())    
    {
      report( WARNING, kFacilityString )
	<< "Could not find DBADRGeomCathodePanels in record " << endl; 
      assert(iADRCathodePanelConst.valid());
    }

ADRCathodePanelConstants** pCathodePanels =
  new ADRCathodePanelConstants*[ADRCathodeParams::kNumberOfPanels];   
    
  // Parse and make a local copy to pass to ADRCathodeStore 
  // (which is responsible for deleting these things).  
  report( DEBUG, kFacilityString )
    << "     ADRGeomCathodePanel Constants" <<endl
    << " Panel PhiMinEast PhiMaxEast ZOffEastPhiMin ZOffEastPhiMax  "<< endl; 
  for(  int ix=0; ix <ADRCathodeParams::kNumberOfPanels  ; ix++)  
    {
      ADRCathodePanelConstants* pCP = new ADRCathodePanelConstants(			           
         iADRCathodePanelConst[ix].get_PanelNumber(),		           
         iADRCathodePanelConst[ix].get_PanelMeasuredPhiMinEast(),		           
         iADRCathodePanelConst[ix].get_PanelMeasuredPhiMaxEast(),	           
         iADRCathodePanelConst[ix].get_PanelMeasuredZOffEastPhiMin(),			         
         iADRCathodePanelConst[ix].get_PanelMeasuredZOffEastPhiMax(),           
         iADRCathodePanelConst[ix].get_PanelCatodeZShift());
      report( DEBUG, kFacilityString )
	<<setw(2)<<iADRCathodePanelConst[ix].get_PanelNumber()<<"."	           
         <<setw(10)<<iADRCathodePanelConst[ix].get_PanelMeasuredPhiMinEast()<<" "	           
         <<setw(10)<<iADRCathodePanelConst[ix].get_PanelMeasuredPhiMaxEast() <<" "         
         <<setw(10)<<iADRCathodePanelConst[ix].get_PanelMeasuredZOffEastPhiMin()<<" "      
         <<setw(10)<<iADRCathodePanelConst[ix].get_PanelMeasuredZOffEastPhiMax()<<" "    
	<<setw(10)<<iADRCathodePanelConst[ix].get_PanelCatodeZShift()<<endl;
      report( DEBUG, kFacilityString )
	<<setw(2)<<pCP->panelNumber()<<"."	           
         <<setw(10)<<pCP->phiMinEast()<<" "	           
         <<setw(10)<<pCP->phiMaxEast() <<" "         
         <<setw(10)<<pCP->zOffEastPhiMin()<<" "      
         <<setw(10)<<pCP->zOffEastPhiMax()<<" "    
	<<setw(10)<<pCP->zShift()<<endl;
      pCathodePanels[ix] = pCP; 
    }
  return  pCathodePanels;
    }
//
// const member functions
//

//
// static member functions
//

