// -*- C++ -*-
//
// Package:     <HbookHistogram>
// Module:      test
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Wed Dec 22 13:15:47 EST 1999
// $Id: test.cc,v 1.3 2000/02/16 14:47:52 cdj Exp $
//
// Revision history
//
// $Log: test.cc,v $
// Revision 1.3  2000/02/16 14:47:52  cdj
// added tests for more complex math operations
//
// Revision 1.2  2000/02/15 16:31:39  cdj
// also test division by scalar
//
// Revision 1.1  1999/12/23 23:34:05  cdj
// first submission
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */
#include <iostream.h>

#include <functional>
#include <utility>

// user include files
//#include "Experiment/report.h"
#include "HistogramInterface/HistogramPackage.h"
#include "HbookHistogram/HbookManager.h"

//#include "CLHEP/Geometry/Vector3D.h"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "HbookHistogram.test" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: test.cc,v 1.3 2000/02/16 14:47:52 cdj Exp $";
static const char* const kTagString = "$Name:  $";


template <class Op>
inline
DABoolean
testOperation(HIHist1D* iResult,
	      HIHist1D* iFirst,
	      HIHist1D* iSecond,
	      Op op)
{
   const unsigned int kNChannels = iResult->nChannels();
   for( unsigned int bin = 1; bin <= kNChannels; ++bin ) {

      if( iResult->channelValue( bin ) != 
	  op( iFirst->channelValue(bin), 
	      iSecond->channelValue(bin) ) ) {
	 return false;
      }
   }

   return true;
}

//copied from <functional>
template <class T>
struct divide_check_0 : public binary_function<T, T, T>
{
      T operator() (const T& x, const T& y) const { 
	 if( 0 == y) { return 0; } 
	 return x / y; }
};


const float kScaleValue = 2.0;

template <class T>
struct scale : public binary_function<T, T, T>
{
      T operator() (const T& x, const T& ) const { 
	 return kScaleValue* x ; }
};


#define CHECK(_op_, _result_) if( testOperation( _result_, increase1D, decrease1D, _op_<float>() ) ) { cout << # _op_ " OK" << endl; } else { cout << # _op_ " broken" << endl;}


template <class Op>
inline
DABoolean
testError(HIHist1D* iResult,
	  HIHist1D* iFirst,
	  HIHist1D* iSecond,
	  Op op)
{
   DABoolean returnValue = true;
   const unsigned int kNChannels = iResult->nChannels();
   for( unsigned int bin = 1; bin <= kNChannels; ++bin ) {

      float calculated = op( make_pair(iFirst->channelValue(bin), 
				       iFirst->channelError(bin)), 
			     make_pair(iSecond->channelValue(bin), 
				       iSecond->channelError(bin) ) 
	 );
      if( iResult->channelError( bin ) != calculated ) {
	 float diff =  iResult->channelError( bin ) - calculated ;
	 float sum = diff + 2* calculated;
	 if( sum != 0.0 ) {
	    if ( fabs(diff/ sum) < 0.00001 ) { 
	       cout <<"round off fraction of " << fabs(diff/sum)
		    <<" for bin " << bin << endl;
	       continue; }
	 }
	 cout <<"bin: " << bin <<" values \n  " 
	      << iFirst->channelValue(bin) <<" " <<iFirst->channelError(bin)
	      << "\n  "
	      << iSecond->channelValue(bin) <<" " <<iSecond->channelError(bin)
	      <<"\n result error " << iResult->channelError(bin)
	      <<"\n  calculated error " 
	      << calculated
	      << endl;
	 returnValue = false;
      }
   }

   return returnValue;
}


template <class T>
struct plus_error : public binary_function<pair<T,T>, pair<T,T>, T>
{
      T operator() (const pair<T,T>& x, const pair<T,T>& y) const { 
	 return sqrt( x.second*x.second + y.second*y.second); }
};

template <class T>
struct minus_error : public binary_function<pair<T,T>, pair<T,T>, T>
{
      T operator() (const pair<T,T>& x, const pair<T,T>& y) const { 
	 return sqrt( x.second*x.second + y.second*y.second); }
};


template <class T>
struct multiplies_error : public binary_function<pair<T,T>, pair<T,T>, T>
{
      T operator() (const pair<T,T>& x, const pair<T,T>& y) const { 
	 return sqrt( x.second*x.second*(y.first*y.first) + 
		      y.second*y.second*(x.first*x.first) ); }
};

template <class T>
struct divide_error_check_0 : public binary_function<pair<T,T>, pair<T,T>, T>
{
      T operator() (const pair<T,T>& x, const pair<T,T>& y) const {
	 if( T(0) == y.first ) { return T(0); }
	 T divisor = y.first*y.first;
	 return sqrt( x.second*x.second/divisor + 
		      y.second*y.second*(x.first*x.first)/(divisor*divisor)); }
};

template <class T>
struct scale_error : public binary_function<pair<T,T>, pair<T,T>, T>
{
      T operator() (const pair<T,T>& x, const pair<T,T>& ) const { 
	 return kScaleValue* x.second ; }
};

#define CHECK_ERROR(_op_, _result_) if( testError( _result_, increase1D, decrease1D, _op_<float>() ) ) { cout << # _op_ " OK" << endl; } else { cout << # _op_ " broken" << endl;}



int main()
{
   HbookManager manager("test.hbook", 17, 1024);

   const unsigned int kNChannels = 10;
   const float kMin = 0.;
   const float kMax = 10.;

   HIHist1D* increase1D = manager.histogram( 1,
					     "increase1D",
					     kNChannels,
					     kMin,
					     kMax );

   HIHist1D* decrease1D = manager.histogram( 2,
					     "decrease1D",
					     kNChannels,
					     kMin,
					     kMax );
   
   HIHist1D* flat1D = manager.histogram( 3,
					 "flat",
					 kNChannels,
					 kMin,
					 kMax );


   HIHist1D* plus1D = manager.histogram( 11,
					 "plus1D",
					 kNChannels,
					 kMin,
					 kMax );

   HIHist1D* minus1D = manager.histogram( 12,
					 "minus1D",
					  kNChannels,
					  kMin,
					  kMax );

   HIHist1D* mult1D = manager.histogram( 13,
					 "mult1D",
					 kNChannels,
					 kMin,
					 kMax );

   HIHist1D* div1D = manager.histogram( 14,
					"div1D",
					kNChannels,
					kMin,
					kMax );


   HIHist1D* scale1D = manager.histogram( 15,
					 "scale1D",
					 kNChannels,
					 kMin,
					 kMax );

   HIHist1D* div_scale1D = manager.histogram( 16,
					      "scale1D",
					      kNChannels,
					      kMin,
					      kMax );

   HIHist1D* complexMul1D = manager.histogram(17,
					      "complexMul",
					      kNChannels,
					      kMin,
					      kMax );
   HIHist1D* complexDiv1D = manager.histogram(18,
					      "complexDiv",
					      kNChannels,
					      kMin,
					      kMax );
   HIHist1D* complexSub1D = manager.histogram(19,
					      "complexSub",
					      kNChannels,
					      kMin,
					      kMax );
   HIHist1D* complexAdd1D = manager.histogram(20,
					      "complexAdd",
					      kNChannels,
					      kMin,
					      kMax );


   //fill histograms
   float x = kMin + (kMax-kMin)/kNChannels/2.0;
   for( unsigned int bin = 1;
	bin < kNChannels;
	++bin ) {


      for( unsigned int nEntries = 1;
	   nEntries < bin;
	   ++nEntries ) {
	 increase1D->fill( x );
	 decrease1D->fill( kMax - x );
      }
      
      flat1D->fill(x);

      x += (kMax-kMin)/kNChannels;
   }


   //put out line
   cout <<"------------------------------"<<endl;
   cout <<"1D Histogram arithmetics" <<endl;
   //do operations
   *plus1D = *increase1D + *decrease1D;
   CHECK(plus, plus1D)
   CHECK_ERROR(plus_error, plus1D)

   *minus1D = *increase1D - *decrease1D;
   CHECK(minus, minus1D)
   CHECK_ERROR(minus_error, minus1D)

   *mult1D = *increase1D * *decrease1D;
   CHECK(multiplies, mult1D)
   CHECK_ERROR(multiplies_error, mult1D)

   *div1D = *increase1D / *decrease1D;
   CHECK(divide_check_0, div1D)
   CHECK_ERROR(divide_error_check_0, div1D)

   *scale1D = kScaleValue * *increase1D;
   CHECK(scale, scale1D)
   CHECK_ERROR(scale_error, scale1D)

   const float div_scale = 1.0/kScaleValue;
   *div_scale1D = *increase1D / div_scale;
   CHECK(scale, div_scale1D)
   CHECK_ERROR(scale_error, div_scale1D)


#define CHECK_COMPLEX(_op_, _result_) if( testOperation( _result_, plus1D, plus1D, _op_<float>() ) ) { cout << "complex "# _op_ " OK" << endl; } else { cout << "complex " # _op_ " broken" << endl;}

   *complexMul1D = (*increase1D + *decrease1D) * (*increase1D + *decrease1D);
   CHECK_COMPLEX(multiplies, complexMul1D)
   *complexDiv1D = (*increase1D + *decrease1D) / (*increase1D + *decrease1D);
   CHECK_COMPLEX(divide_check_0, complexDiv1D)
   *complexSub1D = (*increase1D + *decrease1D) - (*increase1D + *decrease1D);
   CHECK_COMPLEX(minus, complexSub1D)
   *complexAdd1D = (*increase1D + *decrease1D) + (*increase1D + *decrease1D);
   CHECK_COMPLEX(plus, complexAdd1D)

   cout << "finished" <<endl;

   //HepVector3D temp1( 0, 1., 0);
   //HepVector3D temp2( 1., 0., 1.);

   //HepVector3D temp3 = temp1 + temp2;

   return 0;
}

