// -*- C++ -*-
//
// Package:     ADRGeomProd
// Module:      ADRCathodeProxy
// 
// Description: Active DR3 geometry Cathode proxy
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Inga Karliner
//              (first version: Chris D. Jones DRWireExample)
// Revision history
//
// First submission
// 12/1/99  I.Karliner New library ADRGeomProd (was in ADRGeom)
// 12/10/99 I.Karliner Remove check if etracted data is valid
//                     The new extract() checks it. 

#include "Experiment/Experiment.h"

// system include files
#include <stdlib.h>   // for ::exit
#include <assert.h>

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"

#include "DataHandler/Record.h"
#include "FrameAccess/extract.h"           
#include "FrameAccess/FAItem.h"

#include "DetectorGeometry/DGDetectorStore.h"
#include "DRGeom/DRGeometryParams.h"
#include "ADRGeomProd/ADRCathodeProxy.h"


// STL classes

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "ADRGeomProd.ADRCathodeProxy" ;

//
// static data member definitions
//

//
// constructors and destructor
//

ADRCathodeProxy::ADRCathodeProxy()
  :m_ADRCathodeStoreFactory( 0 )
{
}


//ADRCathodeProxy::ADRCathodeProxy(const ADRCathodeProxy& iproxy)
//  :m_ADRCathodeStore( 0 )
//{
//   *this = iproxy;
//}

ADRCathodeProxy::~ADRCathodeProxy()
{
   delete m_ADRCathodeStoreFactory ;
}

//
// assignment operators
//
// const ADRCathodesProxy& ADRCathodeProxy::operator=
//                    ( const ADRCathodeProxy& iproxy )
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
ADRCathodeProxy::invalidateCache()
{
   //We change m_ADRCathodeStore instead of making a new one
   //delete m_ADRCathodeStore ;
   //m_ADRCathode = 0 ;
}

const ADRCathodeProxy::value_type*
ADRCathodeProxy::faultHandler( const Record& iRecord,
			      const IfdKey& iKey )
{

   //Extract the DR geometry
   // NOTE: this info should be in this Record
   FAItem< DGDetectorStore > spDRStore;
   extract( iRecord, spDRStore, DRGeometryParams::kSecondaryID );
   /*
     if( ! spDRStore.valid() )
     {
     report( WARNING, kFacilityString )
     << "ADRCathodeProxy Could not find DGDetectorStore in record "
     << iRecord.stream().value() 
     << endl;
     return 0;
     }
   */
   // Do we already have a cathode store factory?
   if ( 0 == m_ADRCathodeStoreFactory )
   {
     // Nope - make one
     m_ADRCathodeStoreFactory = new ADRCathodeStoreFactory(*spDRStore, iRecord.frame());
       //--------------------------------------+
       // is the cathode updated??seems that no |
       //--------------------------------------+ 
     if( 0 == m_ADRCathodeStoreFactory->cathodeStore() )
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
     // We've already made a factory - just tell it to update the store
     m_ADRCathodeStoreFactory->updateCathodeStore(*spDRStore, iRecord.frame());
   }

   // Return a pointer to the cathode store in the factory
   return ( m_ADRCathodeStoreFactory->cathodeStore() );
}

//
// const member functions
//

//
// static member functions
//











