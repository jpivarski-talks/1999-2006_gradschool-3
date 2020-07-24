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
// $Id: testKD.cc,v 1.2 2002/08/16 15:09:02 cleo3 Exp $
//
// Revision history
//
// $Log: testKD.cc,v $
// Revision 1.2  2002/08/16 15:09:02  cleo3
// replaced iostream.h with C++/iostream.h
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
unsigned int kMatrixSize = 7;
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
testErrorMatrix( const KTKinematicData& iData,
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
   KTKinematicData defaultCtor;

   DABoolean shouldBeNull;
   //test that default constructor uses proper matrix
   cout <<"default object"<< endl;
   testErrorMatrix( defaultCtor, shouldBeNull = true);

   cout <<"copy of default object"<<endl;
   KTKinematicData copyDefault( defaultCtor );
   testErrorMatrix( copyDefault, shouldBeNull = true);
   
   cout <<"copy null error matrix"<<endl;
   KTKinematicData copyNull( defaultCtor, true);
   testErrorMatrix( copyNull, shouldBeNull = true);

   cout <<"copy all but matrix"<<endl;
   KTKinematicData copyNotMatrix( defaultCtor, false);
   testErrorMatrix( copyNotMatrix, shouldBeNull = true);

   cout <<"set all but matrix"<<endl;
   KTKinematicData setAllButMatrix( HepVector3D(),
				    HepPoint3D(),
				    .140,
				    -1.0);
   testErrorMatrix( setAllButMatrix, shouldBeNull = true);

   HepSymMatrix identity(kMatrixSize,1);
   cout <<"set all "<<endl;
   KTKinematicData setAll( HepVector3D(),
			   HepPoint3D(),
			   .140,
			   -1.0,
			   identity);
   testErrorMatrix( setAll, shouldBeNull = false);

   cout <<"copy of set all"<<endl;
   KTKinematicData copySetAll( setAll );
   testErrorMatrix( setAll, shouldBeNull = false);
   
   cout <<"from default helix" <<endl;
   KTHelix defaultHelix;
   KTKinematicData fromDefaultHelix( defaultHelix,
				     .14,
				     HepVector3D(0,0,-14),
				     true);
   testErrorMatrix( fromDefaultHelix, shouldBeNull = true );

   cout <<"from helix" <<endl;
   KTHelix helix;
   helix.setErrorMatrix( HepSymMatrix(5,1) );
   KTKinematicData fromHelix( helix,
			      .14,
			      HepVector3D(0,0,-14),
			      true);
   testErrorMatrix( fromHelix, shouldBeNull = false );

   cout <<"operator= for default to default"<<endl;
   KTKinematicData defaultToDefault;
   defaultToDefault = defaultCtor;
   testErrorMatrix( defaultToDefault, true);

   cout <<"operator= for default to set all"<<endl;
   KTKinematicData defaultToSetAll;
   defaultToSetAll = setAll;
   testErrorMatrix( defaultToSetAll, false );

   cout <<"operator= for set all to default"<<endl;
   KTKinematicData setAllToDefault(setAll);
   setAllToDefault = defaultCtor;
   testErrorMatrix(setAllToDefault, true);

   cout <<"operator= for set all to helix"<<endl;
   KTKinematicData setAllToHelix(setAll);
   setAllToHelix = fromHelix;
   testErrorMatrix(setAllToHelix, false);

   return 0;
}
