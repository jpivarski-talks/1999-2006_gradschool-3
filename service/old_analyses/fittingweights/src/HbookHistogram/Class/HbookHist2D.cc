// -*- C++ -*-
//
// Package:     <package>
// Module:      HbookHist2D
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Marc Soussa
// Created:     Sat Dec  5 18:17:11 EST 1998
// $Id: HbookHist2D.cc,v 1.6 2001/09/21 03:21:35 mccann Exp $
//
// Revision history
//
// $Log: HbookHist2D.cc,v $
// Revision 1.6  2001/09/21 03:21:35  mccann
// Finally implemented reset for histograms (1d,2d,prof)
//
// Revision 1.5  2000/07/11 20:48:20  bkh
// Set directory before fortran HBOOK calls
//
// Revision 1.4  2000/06/19 18:18:11  bkh
// Implement new pure virtual functions from HistogramInterface
//
// Revision 1.3  1999/12/23 23:34:45  cdj
// updated to new histogram interface (for arithmetic operations)
//
// Revision 1.2  1999/09/29 17:55:46  cdj
// added histogram directories
//
// Revision 1.1.1.1  1999/09/06 22:02:30  mkl
// imported HbookHistogram source
//
// Revision 1.1.1.1  1999/02/23 22:00:58  cdj
// imported HbookHistogram source
//
#include <string.h>
#include "HbookHistogram/hbook_functions.h"
#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "HbookHistogram/HbookHist2D.h"

// STL classes

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "HbookHistogram.HbookHist2D" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HbookHist2D.cc,v 1.6 2001/09/21 03:21:35 mccann Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
HbookHist2D::HbookHist2D( HbookHistDir* iParent,
                          Count         iId,
			  const string& iTitle,
			  unsigned int  iNChannelsX,
			  float         iLowEdgeX,
			  float         iHighEdgeX,
			  unsigned int  iNChannelsY,
			  float         iLowEdgeY,
			  float         iHighEdgeY,
			  float         iBitsPerChannel) :
  HIHist2D( iId,
	    iTitle,
	    iNChannelsX,
	    iLowEdgeX,
	    iHighEdgeX,
	    iNChannelsY,
	    iLowEdgeY,
	    iHighEdgeY,
	    iBitsPerChannel ),
  HbookDirContents( iParent )
	    
{
  hbookinit_();

  FInteger ID = id() ;
  FInteger NX = nChannelsX() ;
  FInteger NY = nChannelsY() ;

  if ( ( hexist_( &ID ) & 0x1 ) )
    return ;

  hbook2_( &ID, 
	   iTitle.c_str(),
	   &NX,
	   &iLowEdgeX,
	   &iHighEdgeX,
	   &NY,
	   &iLowEdgeY,
	   &iHighEdgeY,
	   &iBitsPerChannel,
	   iTitle.size() );

  return;
}

// HbookHist2D::HbookHist2D( const HbookHist2D& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HbookHist2D::~HbookHist2D()
{
}
// Fill a 2D histogram
void HbookHist2D::fill( float iXEntry,
			float iYEntry,
			float iWeight )
{
  setDir();
  FInteger ID = id() ;
  hf2_( &ID,
	&iXEntry,
	&iYEntry,
	&iWeight );
}

// Scale a 2D histogram
void HbookHist2D::scale( const float iScale )
{
   add( *this, *this, 0., iScale );
}

// Add two 2D histograms
void HbookHist2D::add( const HIHist2D& rHist1,
		       const HIHist2D& rHist2,
		       const float iScale1,
		       const float iScale2 )
{
   const string kOper = "+";


   FInteger ID1 = rHist1.id();
   FInteger ID2 = rHist2.id();
   FInteger ID3 = id();

   // Call fortran arithmetic operation function
   hopera_( &ID1, kOper.c_str(), &ID2, &ID3, 
	    &iScale1, &iScale2, kOper.size() );
}

// Subtract two 2D histograms
void HbookHist2D::sub( const HIHist2D& rHist1,
		       const HIHist2D& rHist2,
		       const float iScale1,
		       const float iScale2 )
{
   const string kOper = "-";


   FInteger ID1 = rHist1.id();
   FInteger ID2 = rHist2.id();
   FInteger ID3 = id();


   // Call fortran arithmetic operation function
   hopera_( &ID1, kOper.c_str(), &ID2, &ID3, 
	    &iScale1, &iScale2, kOper.size() );
}

// Multiply two 2D histograms
void HbookHist2D::mult( const HIHist2D& rHist1,
			const HIHist2D& rHist2,
			const float iScale1,
			const float iScale2 )
{
   const string kOper = "*";


   FInteger ID1 = rHist1.id();
   FInteger ID2 = rHist2.id();
   FInteger ID3 = id();

   // Call fortran arithmetic operation function
   hopera_( &ID1, kOper.c_str(), &ID2, &ID3, 
	    &iScale1, &iScale2, kOper.size() );
}

// Divide two 2D histograms
void HbookHist2D::div( const HIHist2D& rHist1,
		       const HIHist2D& rHist2,
		       const float iScale1,
		       const float iScale2 )
{
   const string kOper = "/";


   FInteger ID1 = rHist1.id();
   FInteger ID2 = rHist2.id();
   FInteger ID3 = id();

   // Call fortran arithmetic operation function
   hopera_( &ID1, kOper.c_str(), &ID2, &ID3, 
	    &iScale1, &iScale2, kOper.size() );
}

// Copy a 2D histogram
void HbookHist2D::copy( const HIHist2D& rOriginal )
{
/* COMMENTED OUT THE USE OF THE FORTRAN COPY ROUTINE - DANGEROUS!
   const string kTitle = title();

   FInteger ID1 = rOriginal.id();
   FInteger ID2 = id();

   hcopy_( &ID1, &ID2, kTitle.c_str(), kTitle.size() );
*/

   // trickier, but somewhat safer
   add( *this, rOriginal, 0., 1.0 );
}

//
// assignment operators
//
// const HbookHist2D& HbookHist2D::operator=( const HbookHist2D& rhs )
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
void HbookHist2D::reset()
{
   FInteger ID = id();
   FChar* CHTITL = "";
   hreset_( &ID, CHTITL );
}

void
HbookHist2D::fillChannelValue( unsigned int iChannel, float iWeight )
{
   int channelY = iChannel/nChannelsX() + 1;
   int channelX = iChannel % nChannelsY();
   
   fill( lowEdgeX() + ( (((float)channelX) - 0.5) / ((float)nChannelsX()) * 
			(highEdgeX()-lowEdgeX())      ),
	 lowEdgeY() + ( (((float)channelY) - 0.5) / ((float)nChannelsY()) * 
			(highEdgeY()-lowEdgeY())      ),
	 iWeight );
}

//
// const member functions
//
float  
HbookHist2D::channelValue( unsigned int channel ) const
{
   const_cast< HbookHist2D* >( this ) ->setDir();

   FInteger ChannelY = (channel - 1)/nChannelsX() + 1;
   FInteger ChannelX = (channel -1) % nChannelsX() + 1;
   FInteger ID = id();

   return hij_( &ID, &ChannelX, &ChannelY );
}

float 
HbookHist2D::sumWithinLimits() const 
{
   const_cast< HbookHist2D* >( this ) ->setDir();

   FInteger ID ( id() ) ;
   return hsum_( &ID ) ;
}

float 
HbookHist2D::maxValue()        const
{
   const_cast< HbookHist2D* >( this ) ->setDir();

   FInteger ID ( id() ) ;
   return hmax_( &ID ) ;
}

float 
HbookHist2D::minValue()        const
{
   const_cast< HbookHist2D* >( this ) ->setDir();

   FInteger ID ( id() ) ;
   return hmin_( &ID ) ;
}


float 
HbookHist2D::hBookChannelValue( unsigned int iX, unsigned int iY ) const 
{
   FInteger ID ( id() ) ;
   FInteger I  ( iX )   ;
   FInteger J  ( iY )   ;

   return hij_( &ID, &I, &J ) ;
}


float 
HbookHist2D::underXoverY()    const
{
   const_cast< HbookHist2D* >( this ) ->setDir();

   return hBookChannelValue( 0, nChannelsY()+1 ) ;
}

float 
HbookHist2D::insideXoverY()   const
{
   const_cast< HbookHist2D* >( this ) ->setDir();

   float sum ( 0 ) ;
   const unsigned int NX ( nChannelsX() ) ;
   const unsigned int NY1 ( nChannelsY()+1 ) ;
   for( unsigned int i ( 0 ) ; i != NX; ++i )
   {
      sum += hBookChannelValue( i+1, NY1 ) ;
   }
   return sum ;
}

float 
HbookHist2D::overXoverY()     const
{
   const_cast< HbookHist2D* >( this ) ->setDir();

   return hBookChannelValue( nChannelsX()+1, nChannelsY()+1 ) ;
}

float 
HbookHist2D::underXinsideY()  const
{
   const_cast< HbookHist2D* >( this ) ->setDir();

   float sum ( 0 ) ;
   const unsigned int NY ( nChannelsY() ) ;
   for( unsigned int i ( 0 ) ; i != NY; ++i )
   {
      sum += hBookChannelValue( 0, i+1 ) ;
   }
   return sum ;
}

float 
HbookHist2D::overXinsideY()   const
{
   const_cast< HbookHist2D* >( this ) ->setDir();

   float sum ( 0 ) ;
   const unsigned int NX1 ( nChannelsX()+1 ) ;
   const unsigned int NY  ( nChannelsY()   ) ;
   for( unsigned int i ( 0 ) ; i != NY; ++i )
   {
      sum += hBookChannelValue( NX1, i+1 ) ;
   }
   return sum ;
}

float 
HbookHist2D::underXunderY()   const
{
   const_cast< HbookHist2D* >( this ) ->setDir();

   return hBookChannelValue( 0, 0 ) ;
}

float 
HbookHist2D::insideXunderY()  const
{
   const_cast< HbookHist2D* >( this ) ->setDir();

   float sum ( 0 ) ;
   const unsigned int NX ( nChannelsX() ) ;
   for( unsigned int i ( 0 ) ; i != NX; ++i )
   {
      sum += hBookChannelValue( i+1, 0 ) ;
   }
   return sum ;
}

float 
HbookHist2D::overXunderY()    const
{
   const_cast< HbookHist2D* >( this ) ->setDir();

   return hBookChannelValue( nChannelsX()+1, 0 ) ;
}

//
// static member functions
//









