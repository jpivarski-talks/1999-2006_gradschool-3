// -*- C++ -*-
//
// Package:     SiAlignedGeom
// Module:      SiAlignedGeometryProxy
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
//

#include "Experiment/Experiment.h"

// system include files
#include <stdlib.h>   // for ::exit
#include <assert.h>

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <map>
#include <set>
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "Experiment/units.h"

#include "DataHandler/Frame.h"
#include "DataHandler/Record.h"
#include "FrameAccess/extract.h"
//#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"

#include "SiGeom/SiGeom.h"
#include "SiGeom/SiAlignedGeometryProxy.h"
#include "SiGeom/SiGeomDescription.h"

#include "DetectorGeometry/DGVolume.h"
#include "DetectorGeometry/DGVolumePtr.h"
#include "DetectorGeometry/DGPlacedVolume.h"
#include "DetectorGeometry/DGPlacedVolumePtr.h"
#include "DetectorGeometry/DGConstLabVolumePtr.h"
#include "DetectorGeometry/DGVolumeParentPlaced.h"
#include "DetectorGeometry/DGVolumeType.h"
#include "DetectorGeometry/DGPath.h"
#include "DetectorGeometry/DGLabVolume.h"

#include "DetectorGeometry/DGDetectorStore.h"
#include "ToolBox/Triplet.h"

#include "CLHEP/Geometry/Point3D.h"
// Stuff for alignment constants access
#include "FrameAccess/FAConstants.h"
#include "DBSVAlignmentGlobal.hh"
#include "DBSVAlignmentLayer.hh"
#include "DBSVAlignmentLadder.hh"
#include "DBSVAlignmentSensor.hh"
//#include <vector>
// STL classes

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "SiGeom.SiAlignedGeometryProxy" ;

//
// static data member definitions
//

//
// constructors and destructor
//

//SiAlignedGeometryProxy::SiAlignedGeometryProxy()
SiAlignedGeometryProxy::SiAlignedGeometryProxy()
   : m_baseGeometrySyncValue( SyncValue::kBeginningOfTime )
{
}


SiAlignedGeometryProxy::~SiAlignedGeometryProxy()
{
}

//
// assignment operators
//
// const SiAlignedGeometryProxy& SiAlignedGeometryProxy::operator=( const SiAlignedGeometryProxy& iproxy )
// {
//   if( this != &iproxy ) {
//      // do actual copying here, plus:
//      // "SuperClass"::operator=( iproxy );
//   }
//
//   return *this;
// }

//
// member functions
//
void
SiAlignedGeometryProxy::invalidateCache()
{
  // We change m_detectorStore instead of making a new one
}

const SiAlignedGeometryProxy::value_type*
SiAlignedGeometryProxy::faultHandler( const Record& iRecord,
				      const DataKey& iKey )
{
// see if global alignment is defined by external parameters
//  float transx_ext = m_producer.transx();
//  float transy_ext = m_producer.transy();
//  float transz_ext = m_producer.transz();
//  float phix_ext = m_producer.phix();
//  float phiy_ext = m_producer.phiy();
//  float phiz_ext = m_producer.phiz();

// Get Si global alignment constants.
  FAConstants<DBSVAlignmentGlobal> globalAlignment;
  extract( iRecord.frame().record( Stream::kSVAlignment ), globalAlignment);

   if( !globalAlignment.valid() ) {
      report (ERROR, kFacilityString ) << "No SVAlignmentGlobal constants"
				       << endl;
   }

// Get parameters: translations and rotations
// Goofy get_XXXX() names are imposed by BDL compiler.
// You have to look at the first element, even though there is only one
// for this type of constants.
   const double transx = globalAlignment[1].get_deltaX();
   const double transy = globalAlignment[1].get_deltaY();
   const double transz = globalAlignment[1].get_deltaZ();
// .. and rotations
   const double phix = globalAlignment[1].get_phiX();
   const double phiy = globalAlignment[1].get_phiY();
   const double phiz = globalAlignment[1].get_phiZ();

// if external alignment parameter != 1000 then its value supercedes 
// corresponding BDL constant

//  if (transx_ext != 1000.) transx=transx_ext;
//  if (transy_ext != 1000.) transy=transy_ext;
//  if (transz_ext != 1000.) transz=transz_ext;
//  if (phix_ext != 1000.) phix=phix_ext;
//  if (phiy_ext != 1000.) phiy=phiy_ext;
//  if (phiz_ext != 1000.) phiz=phiz_ext;

// The same thing applies to all other constants:
// Get Si layer alignment constants (4 layers)
   FAConstants<DBSVAlignmentLayer>                          layerAlignment;
   extract( iRecord.frame().record( Stream::kSVAlignment ), layerAlignment);

   if( !layerAlignment.valid() )  {
      report (ERROR, kFacilityString ) << "No SVAlignmentLayer constants"
				       << endl;
   }
// Get Si ladder alignment constants (61 ladders)
// NUMBER OF CONSTANT CORRESPONDS TO NUMBER OF LADDER in DGDETECTORSTORE !!!
//              
   FAConstants<DBSVAlignmentLadder>                         ladderAlignment;
   extract( iRecord.frame().record( Stream::kSVAlignment ), ladderAlignment);

   if( !ladderAlignment.valid() )  {
      report (ERROR, kFacilityString ) << "No SVAlignmentLadder constants"
				       << endl;
   }
// Get Si sensor alignment constants (all 447 of them)
// NUMBER OF CONSTANT CORRESPONDS TO NUMBER OF SENSOR in DGDETECTORSTORE !!!
// ABSOLUTELY DIFFERENT FROM THE NUMBER IN ASISTOREPRO !!! 
//             
   FAConstants<DBSVAlignmentSensor>                         sensorAlignment;
   extract( iRecord.frame().record( Stream::kSVAlignment ), sensorAlignment);

   if( !sensorAlignment.valid() )  {
      report (ERROR, kFacilityString ) << "No SVAlignmentSensor constants"
				       << endl;
   }

   report(DEBUG, kFacilityString) << "Parameters of global translation " 
				 << transx << " - " <<  transy 
				 << " - " << transz << endl;
   report(DEBUG, kFacilityString) << "Parameters of global rotation " 
				 << phix << " - " <<  phiy 
				 << " - " << phiz << endl;

   HepTransform3D globalTransform ( HepTranslateZ3D(transz) * 
				    HepTranslateY3D(transy) * 
				    HepTranslateX3D(transx) *
				    HepRotateX3D( phix )    * 
				    HepRotateY3D( phiy )    * 
				    HepRotateZ3D( phiz )      ) ;

   const double kyLadderOffset       = SiGeomDescription::kyLadderOffset ;


  // see if the base geometry has changed
  if( iRecord.frame().contains( Stream::kBaseGeometry ) )  {
     const Record& baseGeometry =
	iRecord.frame().record( Stream::kBaseGeometry );
     
     if( baseGeometry.syncValue() != m_baseGeometrySyncValue ) {
	// need to remove the old root in the DGDetectorStore
	// do this by replacing root with a null
	m_detectorStore.setRoot( DGVolumePtr() );
	m_baseGeometrySyncValue = baseGeometry.syncValue();
     }

     if( m_detectorStore.root().isNull() ) {
	// We need to create a new detector hierarchy for the Si
	// to do this we will
	// 1) get the Si base geometry description
	// 2) use a clone of the base geometry 'root' as our new root
	// 3) clone any PlacedVolume held by our bew root which can
	//    have its alignment changed
	// 4) assign the new PlacedVolumes to member data so we can
	//    easily change them each time the alignment changes
	
	// get the Si base geometry
	FAItem< DGDetectorStore > spSiBaseGeometry;
	
	extract( baseGeometry , spSiBaseGeometry ,
                  SiGeomDescription::kSecondaryID );
	
	if( !spSiBaseGeometry.valid() )  {
	   report( EMERGENCY, kFacilityString )
	      << "no Si BaseGeometry description found" << endl;
	   return 0;
	}

         // Now we need to make our own copy of the various pieces
	DGVolumePtr newRoot = spSiBaseGeometry->root()->clone();

         // Before we forget, hand this to our detector store
	m_detectorStore.setRoot( newRoot );

         // We need to make our own copy of the PlacedVolume used to hold
         // the Si mother volume
	const DGConstPlacedVolumePtr spMotherVolume =
	   newRoot->getChild(SiGeomDescription::kPlacedSiName, 1 );

	if( spMotherVolume.isNull() )  {
	   // The base geometry has a different description than what
	   // we expected
	   report( EMERGENCY, kFacilityString )
	      << "no " << SiGeomDescription::kPlacedSiName
	      << " found in Si BaseGeometry" << endl;
	   ::exit( -1 );
	   return 0;
	}

	DGPlacedVolumePtr pMotherVolume  = spMotherVolume->clone();

	pMotherVolume -> setAlignedRelativeToNominal(globalTransform);
	
	// We now need to replace our copy of the mother volume with the one
	// being held in newRoot.  To do this we must first convert
	// newRoot into its actual type.

	DGVolumeType< DGVolumeParentPlaced > spRootParentVolume( newRoot );
	
	if( !spRootParentVolume.isValid() )  {
	   // The base geometry has a different description than what
	   // we expected.
	   // This is a programming error.
	   report( EMERGENCY, kFacilityString )
	      << "Si BaseGeometry's " << newRoot->name()
	      << " is not a DGVolumeParentPlaced" << endl;
	   ::exit( -1 );
	   return 0;
	}

	if( !(*spRootParentVolume).replaceChild(pMotherVolume) )  {
	   report( EMERGENCY, kFacilityString )
	      << "unable to replace "
	      << pMotherVolume->volume()->name()
	      << " in " << newRoot->name() << endl;
	   ::exit( -1 );
	   return 0;
	}
	
	// Now we have to convert the mother volume to its real type so that
	// we can add the aligned sub-pieces
	DGVolumePtr spClonedMotherVolumePtr(pMotherVolume->volume()->clone());

	pMotherVolume->setVolume( spClonedMotherVolumePtr );

	DGVolumeType< DGVolumeParentPlaced >
	   spParentMotherVolume( spClonedMotherVolumePtr );

	if( !spParentMotherVolume.isValid() )  {
	   report( EMERGENCY, kFacilityString )
	      << "Si BaseGeometry's "
	      << spClonedMotherVolumePtr->name()
	      << " is not a DGVolumeParentPlaced" << endl;
	   ::exit( -1 );
	   return 0;
	}

	// Ladders of layer 0

	int counter_of_ladders(0);
	int counter_of_sensors(0);

	HepTransform3D layerTransform[4];
           
	//Adding this to properly initialize the constants for the clamshells 
	{for( UInt32 iLayer_or_Clam ( 0 ) ; 
	     iLayer_or_Clam < SiGeomDescription::kNLayers; ++iLayer_or_Clam )
	{
	   FAConstants<DBSVAlignmentLayer>::const_reference layAl
	      ( layerAlignment[iLayer_or_Clam+1] ) ;

	   layerTransform[iLayer_or_Clam] = 
	      HepTranslateZ3D(layAl.get_deltaZ()) *    
	      HepTranslateY3D(layAl.get_deltaY()) *    
	      HepTranslateX3D(layAl.get_deltaX()) *
	      HepRotateX3D(layAl.get_phiX()) * 
	      HepRotateY3D(layAl.get_phiY()) * 
	      HepRotateZ3D(layAl.get_phiZ());
	}} // extra {} for solaris compiler not to complain about loop variable

	//Now we are looping over layers and clamshells to change the geometry
	for( UInt32 iLayer_or_Clam ( 0 ) ; 
	     iLayer_or_Clam < SiGeomDescription::kNLayers; ++iLayer_or_Clam ) 
	{   
	   const UInt32 NPhi ( SiGeomDescription::NPhiStep[ iLayer_or_Clam ]);

	   DGConstPlacedVolumePtr spLayer ( (*spParentMotherVolume).getChild(
	      SiGeomDescription::kLayerName[iLayer_or_Clam], 1) ) ;
	   	   
	   DGPlacedVolumePtr pLayer ( spLayer->clone() ) ;

// Do nothing == Identity for L1 and 2 -> will apply clamshell corrections
// at the ladder level 

	   const HepTransform3D layerT ( iLayer_or_Clam > 1  ?
					 layerTransform[iLayer_or_Clam] :
					 HepTransform3D::Identity         ) ;

	   pLayer->setAlignedRelativeToNominal(layerT);
	   
	   if( !(*spParentMotherVolume).replaceChild(pLayer) ) {
	      report( EMERGENCY, kFacilityString )
		 << "unable to replace layer"
		 << pLayer->volume()->name()
		 << " in " << pMotherVolume->volume()->name() << endl;
	      ::exit( -1 );
	      return 0;
	   }

	   DGVolumePtr spClonedLayerPtr( pLayer->volume()->clone());

	   pLayer->setVolume(spClonedLayerPtr);
	   
	   DGVolumeType<DGVolumeParentPlaced> spParentLayer(spClonedLayerPtr);
	   
	   for(int iLadder(0); iLadder < NPhi; iLadder++) 
	   {
	      ++counter_of_ladders;
	      DGConstPlacedVolumePtr spLadder ( (*spParentLayer).getChild(
		 SiGeomDescription::kLadderName[counter_of_ladders-1],1) ) ;
	      if( spLadder.isNull() ) {
		 // The base geometry has a different description than what
		 // we expected
		 report( EMERGENCY, kFacilityString )
		    << "no " << SiGeomDescription::kLadderName[iLayer_or_Clam]
		    << " found in Si BaseGeometry" << endl;
		 ::exit( -1 );
		 return 0;
	      }

	      FAConstants<DBSVAlignmentLadder>::const_reference ladAl
		 ( ladderAlignment[counter_of_ladders] ) ;

	      // these manipulations for x and z account for the
	      // fact that the ladder parent volumes are traps
	      // which no longer center on the ladder center
	      // but are offset in the y dimension

	      const double sLyr ( 3 == iLayer_or_Clam ? -1 : 1 ) ;

	      const HepPoint3D senCen ( 0, kyLadderOffset , 0 ) ;
	      const HepPoint3D xAxis  ( 1, kyLadderOffset , 0 ) ;
	      const HepPoint3D zAxis  ( 0, kyLadderOffset , 1 ) ;

	      const HepRotate3D  phiX ( sLyr*ladAl.get_phiX() ,
					                      senCen, xAxis ) ;
	      const HepRotateY3D phiY ( sLyr*ladAl.get_phiY()  ) ;
	      const HepRotate3D  phiZ (      ladAl.get_phiZ() ,
					                      senCen, zAxis ) ;

	      const HepTransform3D ladderT = 
		 HepTranslateZ3D(     ladAl.get_deltaZ())* 
		 HepTranslateY3D(sLyr*ladAl.get_deltaY())* 
		 HepTranslateX3D(sLyr*ladAl.get_deltaX())* 
		 phiX * 
		 phiY * 
		 phiZ ;
	      
	      DGPlacedVolumePtr pLadder ( spLadder->clone() ) ;
	    
	      //Code for changing Clamshell geometry at the ladder level.
	      // I'd rather not use ASiStore function here.   
	      const HepTransform3D ladderTransform 
		 ( iLayer_or_Clam < 2   ?
		   (pLadder->parentToLocalNominal())* 
		   layerTransform[clamshell(counter_of_ladders)-1]*
		   (pLadder->localNominalToParent())
		   *ladderT :
		   ladderT ) ;

	      pLadder->setAlignedRelativeToNominal(ladderTransform);
//	 cout <<  SiGeomDescription::kLadderName[iLayer_or_Clam]  << iLadder << endl;
	      if( !(*spParentLayer).replaceChild(pLadder) )  {
		 report( EMERGENCY, kFacilityString )
		    << "unable to replace ladder"
		    << pLadder->volume()->name()
		    << " in " << pLayer->volume()->name() << endl;
		 ::exit( -1 );
		 return 0;
	      }

	      DGVolumePtr spClonedLadderPtr( pLadder->volume()->clone() );

	      pLadder->setVolume(spClonedLadderPtr);
	      
	      DGVolumeType< DGVolumeParentPlaced > spParentLadder(spClonedLadderPtr); 
	      
	      for( UInt32 iSensor(0); 
		  iSensor < SiGeomDescription::kNZStep[iLayer_or_Clam];
		  ++iSensor) 
	      {
		 ++counter_of_sensors;
		 DGConstPlacedVolumePtr spSensor ( (*spParentLadder).getChild(
		    SiGeomDescription::kSensorName[counter_of_ladders-1], 
		    counter_of_sensors) ) ;  

		 if( spSensor.isNull() ) {
		 // The base geometry has a different description than what
		 // we expected
		    report( EMERGENCY, kFacilityString )
		       << "no " 
		       << SiGeomDescription::kSensorName[counter_of_ladders-1]
		       << " found in Si BaseGeometry" << endl;
		    ::exit( -1 );
		    return 0;
		 }

		 FAConstants<DBSVAlignmentSensor>::const_reference senAl
		    ( sensorAlignment[counter_of_sensors] ) ;

		 const HepTransform3D sensorTransform = 
		    HepTranslateZ3D(senAl.get_deltaZ())* 
		    HepTranslateY3D(senAl.get_deltaY())* 
		    HepTranslateX3D(senAl.get_deltaX())* 
		    HepRotateX3D(senAl.get_phiX())* 
		    HepRotateY3D(senAl.get_phiY())* 
		    HepRotateZ3D(senAl.get_phiZ());
		 
		 DGPlacedVolumePtr pSensor ( spSensor->clone() ) ;

//	       HepTransform3D trrr = pSensor -> localNominalToParent();
//	       HepTransform3D trevers = trrr^-1;
//	       HepTransform3D trevers1 = trevers*sensorTransform;
//	       HepTransform3D trrr1 = trevers1^-1; 
//	       pSensor -> setLocalNominalToParent(trrr1);

		 pSensor -> setAlignedRelativeToNominal(sensorTransform);
	       
//	   cout  << "attempt to replace sensor " << iSensor << " -- "
//	      << pSensor->volume()->name()
//	      << " in " << m_siLadder01[iLadder] ->volume()->name() << endl;
	       
		 if( !(*spParentLadder).replaceChild(pSensor) ) {
		    report( EMERGENCY, kFacilityString )
		       << "unable to replace sensor "
		       << pSensor->volume()->name()
		       << " in " << pLadder->volume()->name() << endl;
		    ::exit( -1 );
		 }
		  
	      }  // for(int iSensor
	      
	   }  // for(int iLadder  
	}
     }
     else
     {
	report( EMERGENCY, kFacilityString )
	   << "no BaseGeometry record found" << endl;
	if( m_detectorStore.root().isNull() )
	   // can't continue
	   return 0;
     }
  }
  return ( &m_detectorStore ) ;
}

SmallCount 
SiAlignedGeometryProxy::clamshell( SmallCount ladder) const
{
   SmallCount map[61] = {1,1,2,2,2,2,1,
			 1,1,1,2,2,2,2,2,1,1};
   return map[ladder-1];
}


//
// const member functions
//

//
// static member functions
//






