// -*- C++ -*-
//
// Package:     <package>
// Module:      HbookHist1D
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Marc Soussa
// Created:     Sat Dec  5 17:57:39 EST 1998
// $Id: HbookHist1D.cc,v 1.7 2001/09/21 03:21:35 mccann Exp $
//
// Revision history
//
// $Log: HbookHist1D.cc,v $
// Revision 1.7  2001/09/21 03:21:35  mccann
// Finally implemented reset for histograms (1d,2d,prof)
//
// Revision 1.6  2000/07/11 20:48:19  bkh
// Set directory before fortran HBOOK calls
//
// Revision 1.5  2000/06/19 18:18:11  bkh
// Implement new pure virtual functions from HistogramInterface
//
// Revision 1.4  2000/03/11 17:52:10  cdj
// now call hff1_ instead of hf1_
//
// Revision 1.3  1999/12/23 23:34:45  cdj
// updated to new histogram interface (for arithmetic operations)
//
// Revision 1.2  1999/09/29 17:55:45  cdj
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
#include "Experiment/report.h"
#include "HbookHistogram/HbookHist1D.h"

// STL classes

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "HbookHistogram.HbookHist1D" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HbookHist1D.cc,v 1.7 2001/09/21 03:21:35 mccann Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//

HbookHist1D::HbookHist1D( HbookHistDir* iParent,
			  Count         iId,
			  const string& iTitle,
			  unsigned int iNChannels,
			  float iLowEdge,
			  float iHighEdge,
			  float iBitsPerChannel ) :
  HIHist1D( iId, 
	    iTitle,
	    iNChannels,
	    iLowEdge,
	    iHighEdge,
	    iBitsPerChannel ),
  HbookDirContents( iParent ),
  m_histAddress(0)
{
  hbookinit_();
  
  FInteger ID = id() ;
  FInteger NX = nChannels() ;

  if ( ( hexist_( &ID ) & 0x1 ) ) 
    return;
  
  hbook1_( &ID,
           iTitle.c_str(),
           &NX,
	   &iLowEdge,
	   &iHighEdge,
	   &iBitsPerChannel,
	   iTitle.size() );


  return;
}

// HbookHist1D::HbookHist1D( const HbookHist1D& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HbookHist1D::~HbookHist1D()
{
}
//

// Fill a 1D histogram
void 
HbookHist1D::fill( float iEntry,
		   float iWeight )
{
   setDir();
   FInteger ID = id();
   FReal entry = iEntry;
   hff1_( &ID,
	  &m_histAddress,
	  &entry,
	  &iWeight );
}

// Scale a 1D histogram
void HbookHist1D::scale( const float iScale )
{
   add( *this, *this, 0., iScale );
}

// Add two 1D histograms
void HbookHist1D::add( const HIHist1D& rHist1,
		       const HIHist1D& rHist2,
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

// Subtract two 1D histograms
void HbookHist1D::sub( const HIHist1D& rHist1,
		       const HIHist1D& rHist2,
		       const float iScale1,
		       const float iScale2 )
{
   const string kOper = "-";


   FInteger ID1 = rHist1.id();
   FInteger ID2 = rHist2.id();
   FInteger ID3 = id();


   // Call fortran arithmetic operation function
   hopera_( &ID1, kOper.c_str(), &ID2, &ID3, &iScale1, &iScale2, kOper.size() );
}

// Multiply two 1D histograms
void HbookHist1D::mult( const HIHist1D& rHist1,
			const HIHist1D& rHist2,
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

// Divide two 1D histograms
void HbookHist1D::div( const HIHist1D& rHist1,
		       const HIHist1D& rHist2,
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

// Copy a 1D histogram
void HbookHist1D::copy( const HIHist1D& rOriginal )
{

/* COMMENTED OUT FORTRAN COPYROUTINE-MORE DANGEROUS!
   const string kTitle = title();

   FInteger ID1 = rOriginal.id();
   FInteger ID2 = id();

   hcopy_( &ID1, &ID2, kTitle.c_str(), kTitle.size() );
*/

   add( *this, rOriginal, 0., 1.0 );
}

// assignment operators
//
// const HbookHist1D& HbookHist1D::operator=( const HbookHist1D& rhs )
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
void HbookHist1D::reset()
{
   FInteger ID = id();
   FChar* CHTITL = "";
   hreset_( &ID, CHTITL );
}

void
HbookHist1D::fillChannelValue( unsigned int iChannel, float iWeight )
{
   fill( lowEdge() + ( (((float)iChannel) - 0.5) / ((float)nChannels()) * 
		       (highEdge()-lowEdge())      ),
	 iWeight );
}

void
HbookHist1D::setErrors( const float* iErrorArray )
{
   FInteger ID = id();
   const FReal* errorArray = iErrorArray;

   hpake_( &ID, errorArray );
}

//
// const member functions
//
float
HbookHist1D::channelValue( unsigned int channel ) const
{
   const_cast< HbookHist1D* >( this ) ->setDir();
   FInteger Channel = channel;
   FInteger ID = id();

   return hi_( &ID, &Channel );
}

float
HbookHist1D::channelError( unsigned int iChannel ) const
{
   const_cast< HbookHist1D* >( this ) ->setDir();
   FInteger ID = id();
   FInteger channel = iChannel;

   return hie_( &ID, &channel );
}


float 
HbookHist1D::sumWithinLimits() const 
{
   const_cast< HbookHist1D* >( this ) ->setDir();
   FInteger ID ( id() ) ;
   return hsum_( &ID ) ;
}

float 
HbookHist1D::maxValue()        const
{
   const_cast< HbookHist1D* >( this ) ->setDir();
   FInteger ID ( id() ) ;
   return hmax_( &ID ) ;
}

float 
HbookHist1D::minValue()        const
{
   const_cast< HbookHist1D* >( this ) ->setDir();
   FInteger ID ( id() ) ;
   return hmin_( &ID ) ;
}
 
float 
HbookHist1D::underflow() const
{
   const_cast< HbookHist1D* >( this ) ->setDir();
   FInteger ID ( id() ) ;
   FInteger under ( 0 ) ;
   return hi_( &ID, &under ) ;
}

float 
HbookHist1D::overflow()  const 
{
   const_cast< HbookHist1D* >( this ) ->setDir();
   FInteger ID ( id() ) ;
   FInteger over ( nChannels()+1 ) ;
   return hi_( &ID, &over ) ;
}

//
// static member functions
//






