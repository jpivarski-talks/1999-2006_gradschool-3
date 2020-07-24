#if !defined(SJP_HBOOK_H)
#define SJP_HBOOK_H

//
//  File:	sjp_hbook.h
//  Author: 	Simon Patton
//  Contents:	Declarations of the FORTRAN HBOOK functions.
//		This is currently filled as the routines are required.
//
// $Id: hbook_functions.h,v 1.6 2001/09/21 03:21:40 mccann Exp $
//
// $Log: hbook_functions.h,v $
// Revision 1.6  2001/09/21 03:21:40  mccann
// Finally implemented reset for histograms (1d,2d,prof)
//
// Revision 1.5  2000/06/19 18:18:21  bkh
// Implement new pure virtual functions from HistogramInterface
//
// Revision 1.4  2000/03/11 17:48:45  cdj
// added hff1_ and hff2_
//
// Revision 1.3  1999/12/23 23:34:50  cdj
// updated to new histogram interface (for arithmetic operations)
//
// Revision 1.2  1999/09/29 17:55:57  cdj
// added histogram directories
//
// Revision 1.1.1.1  1999/09/06 22:02:31  mkl
// imported HbookHistogram source
//
// Revision 1.1.1.1  1999/02/23 22:00:59  cdj
// imported HbookHistogram source
//
// Revision 1.6  1998/03/05 18:09:40  mkl
// eradicated Utility/fortran_types.h in favor of Experiment/fortran_types; name changes: Fort* become F* types
//
// Revision 1.5  1997/07/03 20:18:19  sjp
// New <Package>/<File>.h structure
//
// Revision 1.4  1997/03/31 14:12:34  sjp
// Fixed double declaration of arguement
//
// Revision 1.3  1997/02/17 19:46:53  sjp
// Added declarations needed for hbook2 and hbprof
//
// Revision 1.2  1996/12/31 16:46:41  sjp
// added signature of hbprof
//
// Revision 1.1  1996/04/11 15:42:54  sjp
// Include file declaring routines necessary for HBOOK
//
// Revision 1.2  1995/07/17 23:03:45  sjp
// Changed declaration of chararters use by FORTRAN from 'char' to 'FChar'
//
// Revision 1.1.1.1  1995/06/24  19:17:18  sjp
// Initial example of Ntuple code
//

#include "Experiment/fortran_types.h"

#ifdef __cplusplus
extern "C"
{
#endif
extern void hbookn_( const FInteger* pIdentifier, const FChar* pTitle,
		     const FInteger* pNumberItems , const FChar* pTopdir,
		     const FInteger* pChunkSize, const FChar* pLabels,
		     const FInteger aLenTitle, const FInteger aLenTopdir,
		     const FInteger aLenOneLabel ) ;
extern void hbook1_( const FInteger* pIdentifier, const FChar* pTitle,
		     const FInteger* pNumberBins , const FReal* pLowEdge,
		     const FReal* pHighEdge, const FReal* pWidth,
		     const FInteger aLenTitle ) ;
extern void hbook2_( const FInteger* pIdentifier, const FChar* pTitle,
		     const FInteger* pXNumberBins , const FReal* pXLowEdge,
		     const FReal* pXHighEdge, const FInteger* pYNumberBins ,
		     const FReal* pYLowEdge, const FReal* pYHighEdge,
		     const FReal* pWidth, const FInteger aLenTitle ) ;
extern void hbprof_( const FInteger* pIdentifier, const FChar* pTitle,
		     const FInteger* pNumberBins , const FReal* pLowEdgeX,
		     const FReal* pHighEdgeX, const FReal* pMinimumY,
		     const FReal* pMaximumY, const FChar* pChopt,
		     const FInteger aLenTitle, const FInteger aLenChopt) ;
extern void hfn_(    const FInteger* pIdentifier, const FReal* pEntry ) ;
extern void hf1_(    const FInteger* pIdentifier, const FReal* pEntry,
		     const FReal* pWeight ) ;
extern void hf2_(    const FInteger* pIdentifier, const FReal* pXEntry,
		     const FReal* pYEntry, const FReal* pWeight ) ;
extern void hff1_(   const FInteger* pIdentifier, const FInteger* pHistAddress,
		     const FReal* pEntry, const FReal* pWeight ) ;
extern void hff2_(   const FInteger* pIdentifier, const FInteger* pHistAddress,
		     const FReal* pXEntry, const FReal* pYEntry, 
		     const FReal* pWeight ) ;
extern void hfill_(  const FInteger* pIdentifier, const FReal* pXEntry,
		     const FReal* pYEntry, const FReal* pWeight ) ;
extern void hrend_(  const FChar* pTopdir, const FInteger aLenTopdir ) ;
extern void hropen_( const FInteger* pLun, const FChar* pTopdir,
		     const FChar* pFilename, const FChar* pChopt,
		     const FInteger* pLrec, FInteger* pIstat,
		     const FInteger aLenTopdir, const FInteger aLenFilenam,
		     const FInteger aLenChopt ) ;
extern void hrout_(  const FInteger* pIdentifier, FInteger* pIcycle,
		     const FChar* pChopt, const FInteger aLenChopt ) ;
extern void hcdir_(  const FChar* pTopdir, const FChar* pChopt,
		     const FInteger aLenTopdir, const FInteger aLenChopt ) ;
extern void hmdir_(  const FChar* pTopdir, const FChar* pChopt,
		     const FInteger aLenTopdir, const FInteger aLenChopt ) ;
extern FLogical hexist_( const FInteger* aIdentifier ) ;

// Arithmetic Operation function
extern void hopera_( const FInteger* pIdentifier1, const FChar* pChoper,
		     const FInteger* pIdentifier2, FInteger* pIdentifier3,
		     const FReal* pScale1, const FReal* pScale2, 
		     const FInteger aLenChoper );

extern void hscale_( const FInteger* pIdentifier1, const FReal* pScale );

extern void hcopy_( const FInteger* pIdentifier1, FInteger* pIdentifier2, 
		    const FChar* pTitle, const FInteger aLenTitle );
// Global Error Filling
extern void hpake_( const FInteger* pIdentifier, const FReal* pErrorArray ); 

// Channel value return functions
extern FReal hi_( const FInteger* pIdentifier, 
		  const FInteger* pChannelNumber );

extern FReal hij_( const FInteger* pIdentifier, 
		   const FInteger* pChannelNumber1,
		   const FInteger* pChannelNumber2 );

extern FReal hie_( const FInteger* pIdentifier, 
		   const FInteger* pChannelNumber );

extern FReal hsum_( const FInteger* pIdentifier );

extern FReal hmin_( const FInteger* pIdentifier );

extern FReal hmax_( const FInteger* pIdentifier );

/* my extra routines to hbook */
extern void hreset_( const FInteger* pID, const FChar* pChTitl );

extern void hbookinit_( void ) ;
#ifdef __cplusplus
}
#endif

#endif
