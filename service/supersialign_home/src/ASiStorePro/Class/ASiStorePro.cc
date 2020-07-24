// -*- C++ -*-
//
// Package:     ASiStorePro
// Module:      ASiStorePro
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Alexander Undrus
// Created:     Sat Jan  9 12:06:13 EST 1999
// $Id: ASiStorePro.cc,v 1.12 2000/04/10 15:43:05 pg Exp $
//
// Revision history
//
// $Log: ASiStorePro.cc,v $
// Revision 1.12  2000/04/10 15:43:05  pg
// Chips numbered from 1 to 976. Active element can now be extracted from both BaseGeometry and SVAlignment streams
//
// Revision 1.11  2000/03/28 15:47:08  pg
// Changed all coordinate transformations to use aligned geometry.
//
// Revision 1.10  1999/07/03 22:27:29  cleo3
//  ASiSensorAddress is transferred from ASiSensor package, ASiStorePro becomes producer, all references to ASiSensor are eliminated
//
// Revision 1.9  1999/06/28 22:37:47  cleo3
//  print diagnostics if param printkey = 1
//
// Revision 1.8  1999/06/23 03:11:29  cleo3
//  added function transToSi
//
// Revision 1.7  1999/05/18 03:45:23  cleo3
// *** empty log message ***
//
// Revision 1.6  1999/05/04 02:57:28  cleo3
//  corrected bug in ASiStore::hybridInLadderForSensor
//
// Revision 1.5  1999/04/06 23:09:21  cleo3
//  added functions returning the width of sensor/ladder
//
// Revision 1.4  1999/03/24 02:51:31  cleo3
// added new ASiStoreConstants.h, added functions with STL vector of subelements, added functions returning various objects
//
// Revision 1.3  1999/03/15 18:59:41  undrus
// added functions halfLength*, ASiSensorInfo::strip(NStrip,NSide)
//
// Revision 1.1.1.1  1999/01/23 02:28:02  undrus
// imported c3tr sources
//
//

#include "Experiment/Experiment.h"

// system include files
#if defined(AMBIGUOUS_STRING_FUNCTIONS_BUG)
#include <string>
#endif             
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "ASiStorePro/ASiStorePro.h"
#include "ASiStorePro/ASiStoreProxy.h"

#include "Experiment/report.h"

#include "DataHandler/Frame.h"
#include "DataHandler/Record.h"
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"
#include "FrameAccess/extract.h"

#include "DataHandler/ProxyFactory.h"
#include "DataHandler/ProxyFactoryFunction.h"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
#include <string>
#include <vector>

//
// constants, enums and typedefs
//
static const char* const kFacilityString = "ASiStorePro.ASiStorePro" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
//static const char* const kIdString  = "$Id: ASiStorePro.cc,v 1.12 2000/04/10 15:43:05 pg Exp $";
//static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
ASiStorePro::ASiStorePro( void )               // anal1
   : Producer( "ASiStorePro" ),
     m_printkey( "printkey", this, 0 )
{
   report( INFO, kFacilityString ) << "here in ctor()" << endl;

   m_printkey.setHelpString(" allows verbose printout if 1 \n ");
// ----- register the Proxies ------

   registerProxyFactory( 
      Stream::kBaseGeometry,
      new ProxyFactoryFunction< ASiStorePro,ASiStoreProxy >
      (*this,&ASiStorePro::makeASiStoreProxy ), 
      UsageTag() );
   registerProxyFactory( 
      Stream::kSVAlignment,
      new ProxyFactoryFunction< ASiStorePro,ASiStoreProxy >
      (*this,&ASiStorePro::makeASiStoreProxy ), 
      UsageTag() );
}


#include "DataHandler/Template/ProxyFactoryFunction.cc"
template class ProxyFactoryFunction< ASiStorePro,ASiStoreProxy >;

ProxyBase* 
ASiStorePro::makeASiStoreProxy() const
{
   return new ASiStoreProxy( *this );
}

ASiStorePro::~ASiStorePro()                    // anal5
{
   report( INFO, kFacilityString ) << "here in dtor()" << endl;
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}

//
// assignment operators
//
// const ASiStorePro& ASiStorePro::operator=( const ASiStorePro& iASiStorePro )
// {
//   if( this != &iASiStorePro ) {
//      // do actual copying here, plus:
//      // "SuperClass"::operator=( iASiStorePro );
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
ASiStorePro::init( void )          // anal1 "Interactive"
{
   report( INFO, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
ASiStorePro::terminate( void )     // anal5 "Interactive"
{
   report( INFO, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}

//
// const member functions
//

//
// static member functions
//


