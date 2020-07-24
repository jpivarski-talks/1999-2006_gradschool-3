// -*- C++ -*-
//
// Package:     <package>
// Module:      test
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Sat Jan  6 14:37:07 EST 2001
// $Id: testHelix.cc,v 1.2 2002/08/16 15:09:00 cleo3 Exp $
//
// Revision history
//
// $Log: testHelix.cc,v $
// Revision 1.2  2002/08/16 15:09:00  cleo3
// replaced iostream.h with C++/iostream.h
//
// Revision 1.1  2001/02/01 01:34:58  cdj
// properly handle null error matricies
//
// Revision 1.1  2001/01/06 20:45:22  cdj
// added test code
//

#include "Experiment/Experiment.h"

// system include files
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>
#include "C++Std/iostream.h"
#include <assert.h>

// user include files
//#include "Experiment/report.h"
#include "KinematicTrajectory/KTKinematicData.h"
#include "KinematicTrajectory/KTHelix.h"


//
// constants, enums and static const char* const kFacilityString = "package.test" ;
unsigned int kMatrixSize = 5;
DABoolean
compareSymMatrix(const HepSymMatrix& iLHS,
		 const HepSymMatrix& iRHS )
{
   if( &iLHS == &iRHS ) {return true;}

   DABoolean returnValue = true;

   for(unsigned int column = 1; column <= kMatrixSize; ++column) {
      for(unsigned int row = column; row <= kMatrixSize; ++row) {
	 if(iLHS.fast(row,column) != iRHS.fast(row,column)) {
	    returnValue = false;
	    goto endOfLoop;
	 }
      }
   }
  endOfLoop:  return returnValue;
}

void
testErrorMatrix( const KTHelix& iData,
		 DABoolean iShouldBeNull ) 
{
   assert(iData.hasNullErrorMatrix() == iShouldBeNull );
   cout <<"hasNullErrorMatrix "
	<<(iData.hasNullErrorMatrix() ? "true": "false" )<< endl;

   cout << iData.errorMatrix() << endl;

   HepSymMatrix nullMatrix(kMatrixSize,0);
   if( iShouldBeNull ) {
      assert( compareSymMatrix(iData.errorMatrix(), nullMatrix) );
   } else {
      assert( !compareSymMatrix(iData.errorMatrix(), nullMatrix) );
   }
}

int
main()
{
   KTHelix defaultCtor;

   DABoolean shouldBeNull;
   //test that default constructor uses proper matrix
   cout <<"default object"<< endl;
   testErrorMatrix( defaultCtor, shouldBeNull = true);

   cout <<"copy of default object"<<endl;
   KTHelix copyDefault( defaultCtor );
   testErrorMatrix( copyDefault, shouldBeNull = true);
   
   cout <<"copy null error matrix"<<endl;
   KTHelix copyNull( defaultCtor, true);
   testErrorMatrix( copyNull, shouldBeNull = true);

   cout <<"copy all but matrix"<<endl;
   KTHelix copyNotMatrix( defaultCtor, false);
   testErrorMatrix( copyNotMatrix, shouldBeNull = true);

   cout <<"set all but matrix"<<endl;
   KTHelix setAllButMatrix( 1.0,
			    0.,
			    0.01,
			    0.5,
			    0.001,
			    HepPoint3D() );
   testErrorMatrix( setAllButMatrix, shouldBeNull = true);

   HepSymMatrix identity(kMatrixSize,1);
   cout <<"set all "<<endl;
   KTHelix setAll( 1.0,
		   0.,
		   0.01,
		   .5,
		   0.001,
		   identity,
		   HepPoint3D());
   testErrorMatrix( setAll, shouldBeNull = false);

   cout <<"copy of set all"<<endl;
   KTHelix copySetAll( setAll );
   testErrorMatrix( setAll, shouldBeNull = false);
   
   cout <<"from default KinematicData no matrix" <<endl;
   KTKinematicData defaultKD(HepVector3D(1.0,0,0),
			     HepPoint3D(),
			     .140,
			     -1.0);
   KTHelix fromDefaultKD( defaultKD,
			  .14,
			  HepPoint3D(0,0,0) );
   testErrorMatrix( fromDefaultKD, shouldBeNull = true );

   cout <<"from kinematicData wiht matrix" <<endl;
   KTKinematicData kd(defaultKD);
   kd.setErrorMatrix( HepSymMatrix(7,1) );
   KTHelix fromKD( kd,
		   .14,
		   HepPoint3D(0,0,0) );
   testErrorMatrix( fromKD, shouldBeNull = false );

   cout <<"operator= for default to default"<<endl;
   KTHelix defaultToDefault;
   defaultToDefault = defaultCtor;
   testErrorMatrix( defaultToDefault, true);

   cout <<"operator= for default to set all"<<endl;
   KTHelix defaultToSetAll;
   defaultToSetAll = setAll;
   testErrorMatrix( defaultToSetAll, false );

   cout <<"operator= for set all to default"<<endl;
   KTHelix setAllToDefault(setAll);
   setAllToDefault = defaultCtor;
   testErrorMatrix(setAllToDefault, true);

   cout <<"operator= for set all to KinematicData"<<endl;
   KTHelix setAllToKD(setAll);
   setAllToKD = fromKD;
   testErrorMatrix(setAllToKD, false);

   return 0;
}
