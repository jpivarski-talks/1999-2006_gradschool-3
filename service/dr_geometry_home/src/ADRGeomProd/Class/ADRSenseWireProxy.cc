// -*- C++ -*-
//
// Package:     ADRGeomProd
// Module:      ADRSenseWireProxy
// 
// Description: Active DR3 geometry Sense Wire proxy
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Inga Karliner
//              (first version: Chris D. Jones DRWireExample)
// Revision history
// Revision 06/28/99 Inga Karliner
//    comment out the dump to PolyConeBigPiece.dump
// Revision 08/31/99 Inga Karliner
//     remove polycone Rlist following the change in DRGeom.
// Revision 09/15/99 Inga Karliner
//     use updateSenseWireStore() for updates
//          09/27/99 cleanup
// Revision 10/25/99 Inga Karliner
//     add extra DRGeom volumes in path to cake1a, etc 
// 9/27/99 add constant read-in from BaseGeometry stream
// 11/17/99 move most of proxy action to ADRSenseWireStoreFactory
// 12/1/99 I.Karliner New library ADRGeomProd (was in ADRGeom)
// 12/10/99 I.Karliner Remove check if etracted data is valid
//                     The new extract() checks it.
// 
// First submission
//
//

#include "Experiment/Experiment.h"

// system include files
#include "C++Std/iomanip.h" 
#include "C++Std/fstream.h"
#include <stdlib.h>   // for ::exit
#include <assert.h>

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "ADRGeomProd/ADRGeomProd.h"
#include "ADRGeom/ADRWireSurveyConstants.h"
#include "ADRGeom/ADREndCapPointers.h"
#include "ADRGeomProd/ADRSenseWireStoreFactory.h"

#include "Experiment/report.h"

#include "DataHandler/Record.h"
#include "FrameAccess/extract.h"           
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"
#include "DataHandler/Frame.h"

#include "FrameAccess/FAConstants.h"
#include "DBADRGeomWireSurvey.hh"

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

// STL classes
#include <vector>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "ADRGeomProd.ADRSenseWireProxy" ;

//
// static data member definitions
//

//
// constructors and destructor
//

ADRSenseWireProxy::ADRSenseWireProxy()
  :m_ADRSenseWireStoreFactory( 0 )
{
}


//ADRSenseWireProxy::ADRSenseWireProxy(const ADRSenseWireProxy& iproxy)
//  :m_ADRSenseWireStore( 0 )
//{
//   *this = iproxy;
//}

ADRSenseWireProxy::~ADRSenseWireProxy()
{
   delete m_ADRSenseWireStoreFactory ;
}

//
// assignment operators
//
// const ADRSenseWiresProxy& ADRSenseWireProxy::operator=
//                    ( const ADRSenseWireProxy& iproxy )
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
ADRSenseWireProxy::invalidateCache()
{
   //We change m_ADRSenseWireStore instead of making a new one
   //delete m_ADRSenseWireStore ;
   //m_ADRSenseWire = 0 ;
}

const ADRSenseWireProxy::value_type*
ADRSenseWireProxy::faultHandler( const Record& iRecord,
			      const IfdKey& iKey )
{

   //Extract the DR geometry
   // NOTE: this info should be in this Record
   FAItem< DGDetectorStore > spDRStore;
   extract( iRecord, spDRStore, DRGeometryParams::kSecondaryID );
 
   /*  
   if( ! spDRStore.valid() ){
      report( WARNING, kFacilityString )
	 << "ADRSenseWireProxy Could not find DGDetectorStore in record "
	 << iRecord.stream().value() 
	 << endl;
      return 0;
   }
   */
   // Check to see if we have created an m_ADRSenseWireStore yet
   
   if( 0 == m_ADRSenseWireStoreFactory )
   {
 
     m_ADRSenseWireStoreFactory
       = new ADRSenseWireStoreFactory(*spDRStore, iRecord.frame());    
     /*     ADRSenseWireStore* mADRSenseWireStore 
	    = m_ADRSenseWireStoreFactory->senseWireStore();
       */
      //--------------------------------------+
      // are the wires updated??seems that no |
      //--------------------------------------+ 
     //      if( 0 == mADRSenseWireStore ) 
      if( 0 == m_ADRSenseWireStoreFactory->senseWireStore() )
      {
	 report( EMERGENCY, kFacilityString )
	    << "out of memory"
	    << endl;
	 assert( false );
	 exit(1);
      }
   } 
   else 
   {
       //Update the values
     m_ADRSenseWireStoreFactory->updateSenseWireStore(*spDRStore);
						 

   }
   return ( m_ADRSenseWireStoreFactory->senseWireStore() );
	    //return ( m_ADRSenseWireStore ) ;
}

//
// const member functions
//

//
// static member functions
//











