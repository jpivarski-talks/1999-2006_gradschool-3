// -*- C++ -*-
//
// Package:     <package>
// Module:      SiGeom
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Alexander Undrus
// Created:     Fri May 22 20:51:24 EDT 1998
// $Id: SiGeom.cc,v 1.12 2001/03/15 21:54:35 bkh Exp $
//
// Revision history
//
// $Log: SiGeom.cc,v $
// Revision 1.12  2001/03/15 21:54:35  bkh
// ExtraVolSpecs now lives in StartRun not base geometry
//
// Revision 1.11  2000/11/15 21:51:51  cdj
// changed report from INFO to DEBUG and removed unused parameters
//
// Revision 1.10  2000/03/28 15:38:08  pg
// Fixed SiAlignedGeometry proxy
//
// Revision 1.9  1999/07/25 00:58:55  undrus
//  added SiAlignedGeometryProxy.  SiGeom  produces both base and aligned geometry
//
// Revision 1.8  1999/07/03 22:35:24  cleo3
//  all references to ASiSensor (although already commented out) are eliminated
//
// Revision 1.7  1999/06/29 19:39:20  lkg
// Add Si3 Geant3 extra volume specs proxy
//
// Revision 1.6  1999/05/18 16:49:06  cleo3
//  remove registration of ASiSensorsProxy again
//
// Revision 1.5  1999/05/18 03:42:04  cleo3
//  register ASiSensorsProxy again , otherwise there unresolved symbols when loading ASiStorePro
//
// Revision 1.4  1999/05/17 20:33:47  cleo3
//  minor changes
//
// Revision 1.3  1999/02/08 18:06:31  cdj
// Uses new proxy registration system
//
// Revision 1.2  1998/09/29 18:49:48  pg
//   Added ASiSensorProxy.
//
// Revision 1.1.1.1  1998/08/18 07:42:04  cleo3
// imported SiGeom sources
//
//

#include "Experiment/Experiment.h"

// system include files
#if defined(AMBIGUOUS_STRING_FUNCTIONS_BUG)
#include <string>
#endif             
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "SiGeom/SiGeom.h"
#include "SiGeom/SiGeometryProxy.h"
#include "SiGeom/SiAlignedGeometryProxy.h"
#include "SiGeom/SiGeomEVSProxy.h"
#include "Experiment/report.h"
#include "SiGeom/SiGeomDescription.h"

#include "DataHandler/Frame.h"
#include "DataHandler/Record.h"
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"
#include "FrameAccess/extract.h"

#include "DataHandler/ProxyFactory.h"
#include "DataHandler/ProxyFactoryFunction.h"

// STL classes

//
// constants, enums and typedefs
//
static const char* const kFacilityString = "Producer.SiGeom" ;

//
// static data member definitions
//

//
// constructors and destructor
//
SiGeom::SiGeom( void )               // anal1
   : Producer( "SiGeom" )
/*   ,
   m_transx( "transx", this, 1000.),
   m_transy( "transy", this, 1000.),
   m_transz( "transz", this, 1000.),
   m_phix(    "phix", this, 1000.),
   m_phiy(    "phiy", this, 1000.),
   m_phiz(    "phiz", this, 1000.) */
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;
/*   m_transx.setHelpString(" translation along x \n ");
   m_transy.setHelpString(" translation along y \n ");
   m_transz.setHelpString(" translation along z \n ");
   m_phix.setHelpString(" phi about x \n ");
   m_phiy.setHelpString(" phi about y \n ");
   m_phiz.setHelpString(" phi about z \n ");
   */
   // ----- register the Proxies ------
   registerProxyFactory( Stream::kBaseGeometry,
			 new ProxyFactory<SiGeometryProxy>,
			 UsageTag(SiGeomDescription::kSecondaryID ) );

   registerProxyFactory( Stream::kStartRun,
			 new ProxyFactory<SiGeomEVSProxy>,
			 UsageTag(SiGeomDescription::kSecondaryID ) );

   registerProxyFactory( Stream::kSVAlignment,
			 new ProxyFactory<SiAlignedGeometryProxy>,
			 UsageTag(SiGeomDescription::kSecondaryID) );

//   registerProxyFactory( 
//      Stream::kSVAlignment, 
//      new ProxyFactoryFunction< SiGeom, 
//      SiAlignedGeometryProxy >( *this, 
//                                 &SiGeom::makeSiAlignedGeometryProxy ),
//      UsageTag( "Aligned Si" )    
//      );
}

#include "DataHandler/Template/ProxyFactory.cc"
template class ProxyFactory<SiGeometryProxy>;
template class ProxyFactory<SiGeomEVSProxy>;
template class ProxyFactory<SiAlignedGeometryProxy>;
//template class ProxyFactory<SiAlignedGeometryProxy>;
//#include "DataHandler/Template/ProxyFactoryFunction.cc"
//template class ProxyFactoryFunction< SiGeom,SiAlignedGeometryProxy >;

//ProxyBase* 
//SiGeom::makeSiAlignedGeometryProxy() const
//{
//   return new SiAlignedGeometryProxy( *this );
//}

// SiGeom::SiGeom( const SiGeom& iSiGeom )
// {
//    *this = iSiGeom;
// }

SiGeom::~SiGeom()                    // anal5
{
   report( DEBUG, kFacilityString ) << "here in dtor()" << endl;
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}

//
// assignment operators
//
// const SiGeom& SiGeom::operator=( const SiGeom& iSiGeom )
// {
//   if( this != &iSiGeom ) {
//      // do actual copying here, plus:
//      // "SuperClass"::operator=( iSiGeom );
//   }
//
//   return *this;
// }

//
// member functions
//

// ------------ methods for beginning/end "Interactive" ------------
// --------------------------- init method -------------------------
void
SiGeom::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
SiGeom::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}

// ---------------- standard place to book histograms ---------------
void
SiGeom::hist_book( TBHistoManager& )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   // book your histograms here

}


//
// const member functions
//

//
// static member functions
//




