// -*- C++ -*-
//
// Package:     <RunStatisticsSourceModule>
// Module:      RSProxyDeliverer
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Wed Jan 26 13:56:06 EST 2000
// $Id: RSProxyDeliverer.cc,v 1.13 2002/09/11 15:13:05 cdj Exp $
//
// Revision history
//
// $Log: RSProxyDeliverer.cc,v $
// Revision 1.13  2002/09/11 15:13:05  cdj
// improved safe conversion of Crossing Angle from milliradians to radians
//
// Revision 1.12  2002/09/11 14:58:51  cdj
// fixed bug when converting crossing angle to radians when angle > 2
//
// Revision 1.11  2002/05/30 20:16:56  tom
// Added conversion routine to change crossing angle in milliradians
//   to radians
//
// Revision 1.10  2002/02/15 17:53:47  hubert
// Use methods instead of static variables for type codes to avoid dependency on static constructor run order
//
// Revision 1.9  2002/01/24 18:56:35  lyon
// Added code to handle CesrQuadrupoleCurrents
//
// Revision 1.8  2001/02/26 00:52:47  cdj
// moved most data from BeginRun to StartRun
//
// Revision 1.7  2000/11/08 03:02:08  lkg
// tweaked various scale factors to match new online definitions of some CESR parameters
//
// Revision 1.6  2000/11/07 03:03:21  lkg
// updated to match changes in the CESR online component
//
// Revision 1.5  2000/11/02 20:51:34  lkg
// Add access to the SolenoidHallProbe field reading
//
// Revision 1.4  2000/06/28 21:54:22  cdj
// now serves CesrAtmosphericPressure
//
// Revision 1.3  2000/05/08 19:52:07  cdj
// now compiles on OSF with MICO
//
// Revision 1.2  2000/02/29 22:41:06  cdj
// added final Cesr objects
//
// Revision 1.1.1.1  2000/01/29 20:47:37  cdj
// imported RunStatisticsSourceModule source
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "RunStatisticsSourceModule/RSProxyDeliverer.h"
#include "RunStatisticsSourceModule/infoXandStream.h"
#include "RunStatisticsSourceModule/RSProxyInterfaceFactory.h"
#include "RunStatisticsSourceModule/RSProxyInterface.h"
#include "RunStatisticsSourceModule/RSProxySingleValue.h"
#include "RunStatisticsSourceModule/RSProxyDualValue.h"
#include "RunStatisticsSourceModule/RSProxyDualFactory.h"

#include "Experiment/units.h"
#include "CesrCleoComm/CesrBeamEnergy.h"
#include "CesrCleoComm/CesrFillNumber.h"
#include "CesrCleoComm/CesrOutdoorTemp.h"
#include "CesrCleoComm/CesrAtmosphericPressure.h"
#include "CesrCleoComm/CesrCrossingAngle.h"
#include "CesrCleoComm/CesrTodaysLumi.h"
#include "CesrCleoComm/CesrBendFields.h"
#include "CesrCleoComm/CesrTimestamps.h"
#include "CesrCleoComm/CesrTotalBeamCurrents.h"
#include "CesrCleoComm/CesrFilledRfBuckets.h"
#include "CesrCleoComm/CesrBucketCurrents.h"
#include "CesrCleoComm/CesrQuadrupoleCurrents.h"

#include "MagneticFieldReadings/SolenoidHallProbe.h"

#include "CLEORB.hxx" //defines CLEORB_CLIENT_HH
#include CLEORB_CLIENT_HH(InfoX)

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "RunStatisticsSourceModule.RSProxyDeliverer" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: RSProxyDeliverer.cc,v 1.13 2002/09/11 15:13:05 cdj Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//

//Classes that hold Corba Type information

class RSCORBALong {
   public:
      typedef CORBA::Long Type;

      static CORBA::TypeCode_var typeCode()
    { return( CORBA::TypeCode_ptr(CORBA::_tc_long) ); }
};

class RSCORBALongList {
   public:
      typedef InfoX::LongList Type;

      static CORBA::TypeCode_var typeCode()
    { return( CORBA::TypeCode_ptr(InfoX::_tc_LongList) ); }
};

//Need explicit decleration of TypeCode_ptr for MICO since
//  under MICO InfoX::_tc_LongList is a TypeCodeConst
//CORBA::TypeCode_var
//RSCORBALongList::kTypeCode = CORBA::TypeCode_ptr(InfoX::_tc_LongList);

class RSCORBAFloat {
   public:
      typedef CORBA::Float Type;

      static CORBA::TypeCode_var typeCode()
    { return( CORBA::TypeCode_ptr(CORBA::_tc_float) ); }
};

class RSCORBAFloatList {
   public:
      typedef InfoX::FloatList Type;

      static CORBA::TypeCode_var typeCode()
    { return( CORBA::TypeCode_ptr(InfoX::_tc_FloatList) ); }
};

//Need explicit decleration of TypeCode_ptr for MICO since
//  under MICO InfoX::_tc_FloatList is a TypeCodeConst
//CORBA::TypeCode_var
//RSCORBAFloatList::kTypeCode = CORBA::TypeCode_ptr(InfoX::_tc_FloatList);

//Converts used to convert from the value stored in RunStatistics
//  to the value held by the various objects
class KeV2GeV {
   public:
      static float convert( CORBA::Float fValue ) {
	 return (fValue * k_keV) ;
      }
};

class MilliGauss2KGauss {
   public:
      static float convert( CORBA::Float fValue ) {
	 return (fValue * 1.0E-3 * k_Gauss ) ;
      }
};

class Micro2Unit {
   public:
      static float convert( CORBA::Float fValue ) {
	 return (fValue * 1.0E-6 ) ;
      }
};

class SafeMilliradians2Radians {
   public:
      static float convert( CORBA::Float fValue ) {
	 //CESR can not run with a crossing angle in the
	 // hundreds of milli radians
         if ( fabs( fValue ) > 0.1 ) {
            fValue /= 1000.0 ;
         }
        
         return ( fValue ) ;
      }
};

template< class VectorT, class Converter >
class LongList2Vector {
   public:
      static STL_VECTOR(VectorT) convert( const InfoX::LongList& iValue ) {
	 int length = iValue.length();
	 STL_VECTOR(VectorT) returnValue;
	 returnValue.reserve(length );

	 for( int index = 0; index < length ; ++index ) {
	    returnValue.push_back( 
	       Converter::convert( iValue[ index ] ) );
	 }
	 return returnValue;
      }
};

template< class VectorT, class Converter >
class FloatList2Vector {
   public:
      static STL_VECTOR(VectorT) convert( const InfoX::FloatList& iValue ) {
	 int length = iValue.length();
	 STL_VECTOR(VectorT) returnValue;
	 returnValue.reserve(length );

	 for( int index = 0; index < length ; ++index ) {
	    returnValue.push_back( 
	       Converter::convert( iValue[ index ] ) );
	 }
	 return returnValue;
      }
};

typedef RSDefaultConverter< CORBA::Long, RSCORBALong > LongConverter;
typedef RSDefaultConverter< CORBA::Float, RSCORBAFloat > FloatConverter;

RSProxyDeliverer::RSProxyDeliverer() :
   ProxyDeliverer("RunStatistics")
{

   registerProxyFactory( Stream::kStartRun,
			 new RSProxyInterfaceFactory<
			          RSProxySingleValue<CesrBeamEnergy, 
			                             RSCORBAFloat,
			                             FloatConverter > >
			 ("energy",
								 0 ),
			 UsageTag("") );

   registerProxyFactory( Stream::kStartRun,
			 new RSProxyInterfaceFactory<
			          RSProxySingleValue<CesrFillNumber, 
			                             RSCORBALong,
			                             LongConverter > >
			      ("cesr_fill_number",
			       0),
			 UsageTag("") );
   registerProxyFactory( Stream::kBeginRun,
			 new RSProxyInterfaceFactory<
			          RSProxySingleValue<CesrOutdoorTemp, 
			                             RSCORBAFloat,
			                             FloatConverter > >
			      ("outside_temp",
			       0),
			 UsageTag("") );
   registerProxyFactory( Stream::kStartRun,
			 new RSProxyInterfaceFactory<
			          RSProxySingleValue<CesrOutdoorTemp, 
			                             RSCORBAFloat,
			                             FloatConverter > >
			      ("outside_temp",
			       0),
			 UsageTag("") );
   registerProxyFactory( Stream::kStartRun,
			 new RSProxyInterfaceFactory<
			          RSProxySingleValue<CesrCrossingAngle, 
			                             RSCORBAFloat,
			                             SafeMilliradians2Radians > >
			      ("crossing_angle",
			       0),
			 UsageTag("") );
   registerProxyFactory( Stream::kBeginRun,
			 new RSProxyInterfaceFactory<
			          RSProxySingleValue<CesrTodaysLumi, 
			                             RSCORBAFloat,
			                             FloatConverter > >
			      ("luminosity_today",
			       0),
			 UsageTag("") );
   registerProxyFactory( Stream::kStartRun,
			 new RSProxyInterfaceFactory<
			          RSProxySingleValue<CesrTodaysLumi, 
			                             RSCORBAFloat,
			                             FloatConverter > >
			      ("luminosity_today",
			       0),
			 UsageTag("") );
   registerProxyFactory( Stream::kBeginRun,
			 new RSProxyInterfaceFactory<
			          RSProxySingleValue<CesrAtmosphericPressure, 
			                             RSCORBAFloat,
			                             FloatConverter > >
			      ("barometric_pressure",
			       0),
			 UsageTag("") );
   registerProxyFactory( Stream::kStartRun,
			 new RSProxyInterfaceFactory<
			          RSProxySingleValue<CesrAtmosphericPressure, 
			                             RSCORBAFloat,
			                             FloatConverter > >
			      ("barometric_pressure",
			       0),
			 UsageTag("") );

   registerProxyFactory( Stream::kStartRun,
			 new RSProxyDualFactory<
			          RSProxyDualValue<CesrBendFields, 
			                           RSCORBAFloat,
			                           FloatConverter>  >
			      ("sen1",
			       "sen2",
			       CesrBendFields(0,0) ),
			 UsageTag("") );

   registerProxyFactory( Stream::kStartRun,
			 new RSProxyDualFactory<
			          RSProxyDualValue<CesrTimestamps, 
			                           RSCORBALong,
			                           LongConverter >  >
			      ("energy_timestamp",
			       "cesr_timestamp",
			       CesrTimestamps(0,0) ),
			 UsageTag("") );

   registerProxyFactory( Stream::kStartRun,
			 new RSProxyDualFactory<
			          RSProxyDualValue<CesrTotalBeamCurrents, 
			                           RSCORBAFloat,
			                           FloatConverter >  >
			      ("electron_cur_tot",
			       "positron_cur_tot",
			       CesrTotalBeamCurrents(0,0) ),
			 UsageTag("") );

   registerProxyFactory( Stream::kStartRun,
			 new RSProxyInterfaceFactory<
			   RSProxySingleValue<CesrFilledRfBuckets, 
			                      RSCORBALongList,
			                      LongList2Vector<UInt32,
			                                      LongConverter> >  >
			      ("bunch_structure",
			       CesrFilledRfBuckets(STL_VECTOR(UInt32)(0) ) ),
			 UsageTag("") );

   registerProxyFactory( Stream::kStartRun,
			 new RSProxyDualFactory<
			   RSProxyDualValue<CesrBucketCurrents, 
			                    RSCORBALongList,
			                    LongList2Vector<float,
			                                    Micro2Unit> >  >
			      ("electron_bunches",
			       "positron_bunches",
			       CesrBucketCurrents(STL_VECTOR(float)(0),
						  STL_VECTOR(float)(0) ) ),
			 UsageTag("") );
   registerProxyFactory( Stream::kStartRun,
			 new RSProxyInterfaceFactory<
			          RSProxySingleValue<SolenoidHallProbe, 
			                             RSCORBAFloat,
			                             FloatConverter > >
			      ("BField",
			       0),
			 UsageTag("") );

   registerProxyFactory( Stream::kStartRun,
			 new RSProxyInterfaceFactory<
			   RSProxySingleValue<CesrQuadrupoleCurrents, 
			                    RSCORBAFloatList,
			                    FloatList2Vector<float,
			                                    FloatConverter> >  >
			      ("scir_current",
			       CesrQuadrupoleCurrents(STL_VECTOR(float)(12, 0.0) )),
			 UsageTag("") );

}

// RSProxyDeliverer::RSProxyDeliverer( const RSProxyDeliverer& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

RSProxyDeliverer::~RSProxyDeliverer()
{
   StreamToFactoryMap::iterator itFactoryEnd = m_streamToFactoryMap.end();
   for( StreamToFactoryMap::iterator itFactory = m_streamToFactoryMap.begin();
	itFactory != itFactoryEnd;
	++itFactory ) {
      delete (*itFactory).second.first;
   }
}

//
// assignment operators
//
// const RSProxyDeliverer& RSProxyDeliverer::operator=( const RSProxyDeliverer& rhs )
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
void 
RSProxyDeliverer::use( const DBRunStatistics::RunStatistics* iRS,
		       const SyncValue& iSyncValue )
{
   Stream::Type stream = infoX2StreamType( iRS->rsType );

   m_streamToRSMap[stream] = iRS;

   report(DEBUG, kFacilityString ) << "using stream " << stream 
				  <<" " <<iSyncValue << " (just before reset)" << endl;

   report(DEBUG, kFacilityString) << "this is " << this << endl;
   reset( stream, iSyncValue );
}

void
RSProxyDeliverer::registerProxyFactory( 
   const Stream::Type& iStream,
   RSProxyInterfaceFactoryBase* iFactory,
   const UsageTag& iTag )
{
   registerStream( iStream );
   m_streamToFactoryMap.insert( StreamToFactoryMap::value_type(
      iStream, pair<RSProxyInterfaceFactoryBase*,UsageTag>(
	              iFactory, iTag) ) ) ;
}


void 
RSProxyDeliverer::disable( const Stream::Type& aStream ) 
{
   report(DEBUG, kFacilityString)
      << "enter RSProxyDeliverer::disable(aStream == " << aStream << ")" << endl;

   pair<StreamToProxyMap::iterator,
        StreamToProxyMap::iterator > range = m_streamToProxyMap.equal_range(
	   aStream );

   for( StreamToProxyMap::iterator itProxy = range.first;
	itProxy != range.second;
	++itProxy ) {
      report(DEBUG, kFacilityString) << "   disable one" << endl;
      (*itProxy).second->disable();
   }

   report(DEBUG, kFacilityString)
      << "exit RSProxyDeliverer::disable" << endl;

}

void 
RSProxyDeliverer::enable( const Stream::Type& aStream )
{
   report(DEBUG, kFacilityString)
      << "enter RSProxyDeliverer::enable(aStream == " << aStream << ")" << endl;

   pair<StreamToProxyMap::iterator,
        StreamToProxyMap::iterator > range = m_streamToProxyMap.equal_range(
	   aStream );

   for( StreamToProxyMap::iterator itProxy = range.first;
	itProxy != range.second;
	++itProxy ) {
      report(DEBUG, kFacilityString) << "   enable one" << endl;
      (*itProxy).second->enable();
   }

   report(DEBUG, kFacilityString)
      << "exit RSProxyDeliverer::enable" << endl;
}

void 
RSProxyDeliverer::resetCache( const Stream::Type& aStream )
{
   //do nothing
}


void 
RSProxyDeliverer::registerProxies( const Stream::Type& aStream ,
				   KeyedProxies& aProxyList ) 
{
   //Find the appropriate address for the RunStatistics info
   //  if the address isn't yet available, make it
   StreamToRSMap::iterator itRS = m_streamToRSMap.find( aStream );
   if( itRS == m_streamToRSMap.end() ) {
      m_streamToRSMap[aStream ];
      itRS = m_streamToRSMap.find( aStream );
   }

   //Use the factories to create the proxies
   pair<StreamToFactoryMap::iterator,
      StreamToFactoryMap::iterator > range = 
      m_streamToFactoryMap.equal_range( aStream );

   for( StreamToFactoryMap::iterator itFactory = range.first;
	itFactory != range.second;
	++itFactory ) {
      RSProxyInterface* interface = (*itFactory).second.first->make( &(*itRS) );
      
      aProxyList.push_back( 
	 KeyedProxy( DataKey( (*itFactory).second.first->typeTag(),
			      (*itFactory).second.second,
			      ProductionTag() ),
		     interface->proxyBase() ) 
	 );

      m_streamToProxyMap.insert( StreamToProxyMap::value_type( aStream,
							       interface ) );
   }
}
//
// const member functions
//

//
// static member functions
//
