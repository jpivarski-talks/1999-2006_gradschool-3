// -*- C++ -*-
//
// Package:     <package>
// Module:      HbookHistProf
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Marc Soussa
// Created:     Sat Dec  5 18:17:20 EST 1998
// $Id: HbookHistProf.cc,v 1.7 2001/09/21 03:21:36 mccann Exp $
//
// Revision history
//
// $Log: HbookHistProf.cc,v $
// Revision 1.7  2001/09/21 03:21:36  mccann
// Finally implemented reset for histograms (1d,2d,prof)
//
// Revision 1.6  2001/08/03 17:29:17  bkh
// Implement value and error fuctionality
//
// Revision 1.5  2000/07/11 20:48:20  bkh
// Set directory before fortran HBOOK calls
//
// Revision 1.4  2000/06/19 18:18:12  bkh
// Implement new pure virtual functions from HistogramInterface
//
// Revision 1.3  1999/12/23 23:34:46  cdj
// updated to new histogram interface (for arithmetic operations)
//
// Revision 1.2  1999/09/29 17:55:47  cdj
// added histogram directories
//
// Revision 1.1.1.1  1999/09/06 22:02:30  mkl
// imported HbookHistogram source
//
// Revision 1.1.1.1  1999/02/23 22:00:59  cdj
// imported HbookHistogram source
//
#include <string.h>
#include "Experiment/Experiment.h"
#include "HbookHistogram/hbook_functions.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "HbookHistogram/HbookHistProf.h"

// STL classes

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "HbookHistogram.HbookHistProf" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HbookHistProf.cc,v 1.7 2001/09/21 03:21:36 mccann Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
HbookHistProf::HbookHistProf( HbookHistDir* iParent,
                              Count          iId,
			      const string&  iTitle,
			      unsigned int   iNChannels,
			      float          iLowEdgeX,
			      float          iHighEdgeX,
			      float          iLowEdgeY,
			      float          iHighEdgeY,
			      HIHistProf::DisplayErrorsAs iDisplayErrorsAs ) :
  HIHistProf(
	      iId,
	      iTitle,
	      iNChannels,
	      iLowEdgeX,
	      iHighEdgeX,
	      iLowEdgeY,
	      iHighEdgeY,
	      iDisplayErrorsAs ),
  HbookDirContents( iParent )
{
  hbookinit_();
  FInteger ID = id() ;
  FInteger NX = nChannels() ;

  if ( ( hexist_( &ID ) & 0x1 ) )
    return;

  FChar* ERROR ;

  switch( iDisplayErrorsAs ) {
    case HIHistProf::kSpread : ERROR = "S" ; 
      break ;

    case HIHistProf::kErrorOnMean : ERROR = " " ;
      break ;
  }

  hbprof_( &ID,
	   iTitle.c_str(),
	   &NX,
	   &iLowEdgeX,
	   &iHighEdgeX,
	   &iLowEdgeY,
	   &iHighEdgeY,
	   ERROR,
	   iTitle.size(),
	   strlen( ERROR ) );

  return;
}

// HbookHistProf::HbookHistProf( const HbookHistProf& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HbookHistProf::~HbookHistProf()
{
}

// Fill a profile plot
void HbookHistProf::fill ( float iXEntry,
			   float iYEntry )
{
  setDir();
  static FReal iWeight = 1.0;

  FInteger ID = id();
  hfill_( &ID,
	  &iXEntry,
	  &iYEntry,
	  &iWeight );
}


float 
HbookHistProf::sumWithinLimits() const 
{
   const_cast< HbookHistProf* >( this ) ->setDir();

   FInteger ID ( id() ) ;
   return hsum_( &ID ) ;
}

float 
HbookHistProf::maxValue()        const
{
   const_cast< HbookHistProf* >( this ) ->setDir();

   FInteger ID ( id() ) ;
   return hmax_( &ID ) ;
}

float 
HbookHistProf::minValue()        const
{
   const_cast< HbookHistProf* >( this ) ->setDir();

   FInteger ID ( id() ) ;
   return hmin_( &ID ) ;
}

float
HbookHistProf::channelValue( unsigned int channel ) const
{
   const_cast< HbookHistProf* >( this ) ->setDir();
   FInteger Channel = channel;
   FInteger ID = id();

   return hi_( &ID, &Channel );
}

float
HbookHistProf::channelError( unsigned int iChannel ) const
{
   const_cast< HbookHistProf* >( this ) ->setDir();
   FInteger ID = id();
   FInteger channel = iChannel;

   return hie_( &ID, &channel );
}


/*  Leave this stuff out until requested
// Scale a profile plot
void HbookHistProf::scale( const float iScale )
{
   add( *this, *this, 0., iScale );
}

// Add two profile plots
void 
HbookHistProf::add ( const HIHistProf& rProf1,
		     const HIHistProf& rProf2,
		     const float iScale1,
		     const float iScale2 )
{
   const string kOper = "+";

   FInteger ID1 = rProf1.id();
   FInteger ID2 = rProf2.id();
   FInteger ID3 = id();

   // Call fortran arithmetic operation function
   hopera_( &ID1, kOper.c_str(), &ID2, &ID3, 
	    &iScale1, &iScale2, kOper.size() ); 
}


// Subtract two profile plots
void 
HbookHistProf::sub ( const HIHistProf& rProf1,
		     const HIHistProf& rProf2,
		     const float iScale1,
		     const float iScale2 )
{
   const string kOper = "-";


   FInteger ID1 = rProf1.id();
   FInteger ID2 = rProf2.id();
   FInteger ID3 = id();


   // Call fortran arithmetic operation function
   hopera_( &ID1, kOper.c_str(), &ID2, &ID3, 
	    &iScale1, &iScale2, kOper.size() );
}


// Multiply two profile plots
void 
HbookHistProf::mult ( const HIHistProf& rProf1,
		      const HIHistProf& rProf2,
		      const float iScale1,
		      const float iScale2 )
{
   const string kOper = "*";


   FInteger ID1 = rProf1.id();
   FInteger ID2 = rProf2.id();
   FInteger ID3 = id();


   // Call fortran arithmetic operation function
   hopera_( &ID1, kOper.c_str(), &ID2, &ID3, 
	    &iScale1, &iScale2, kOper.size() );
}


// Divide two profile plots
void 
HbookHistProf::div ( const HIHistProf& rProf1,
		     const HIHistProf& rProf2,
		     const float iScale1,
		     const float iScale2 )
{
   const string kOper = "/";

   FInteger ID1 = rProf1.id();
   FInteger ID2 = rProf2.id();
   FInteger ID3 = id();


   // Call fortran arithmetic operation function
   hopera_( &ID1, kOper.c_str(), &ID2, &ID3, 
	    &iScale1, &iScale2, kOper.size() );
}

// Copy a profile plot
void HbookHistProf::copy( const HIHistProf& rOriginal )
{
//  USING FORTRAN COPYROUTINE-MORE DANGEROUS!
//   const string kTitle = title();

//   FInteger ID1 = rOriginal.id();
//  FInteger ID2 = id();

//   hcopy_( &ID1, &ID2, kTitle.c_str(), kTitle.size() );


   add( *this, rOriginal, 0., 1.0 );
}
*/

//
// assignment operators
//
// const HbookHistProf& HbookHistProf::operator=( const HbookHistProf& rhs )
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
void HbookHistProf::reset()
{
   FInteger ID = id();
   FChar* CHTITL = "";
   hreset_( &ID, CHTITL );
}
//
// const member functions
//
/*
float 
HbookHistProf::getValue( unsigned int channel ) const
{
   return 0;
}
*/ 
//
// static member functions
//


