*
* $Id: cfexer.F,v 1.3 2002/06/17 16:14:40 atems Exp $
*
* $Log: cfexer.F,v $
* Revision 1.3  2002/06/17 16:14:40  atems
*         removed fortran comments from #include directives in cfexer.F
*
* Revision 1.2  1999/11/29 21:43:47  lyon
* Changes to include files to remove references to old Cleo II include areas
*
* Revision 1.1  1998/02/24 18:52:49  dpp
* not entered 2/9/98
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author : A. Foland
      SUBROUTINE CFEXER(C,PHI,D,WFN,ERRM,SDZ)
C.......................................................................
C.
C. CFEXER Given fit Cu D and Phi0 with errors, returns the expected
C.	   error in extrapolation to the wafer
C.
C. COMMON    :
C. CALLS     : TRUPCK,PLNVEC,XYPLAN
C. CALLED    : FTEXER
C. AUTHOR    : A. Foland
C.
C. VERSION   : 1.00
C. CREATED   : 24-Jan-96
C. LAST MOD  : 
C.
C.
C. Modification Log.
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

C== Argument declarations
C INPUTS
      REAL C,PHI,D,ERRM(6),SDZ
      INTEGER WFN
C	C:	the curvature of the fit
C	Phi:	the phi0 of the fit
C	D:	the D (impact paramter) of the fit
C	wfn:	the wafer number of interest
C	errm:	the packed error matrix of the fit
C RETURNS
C	sdz:	the error in r-phi (i.e. r*dphi)


C	I use the TFlayer number and see what wafer the fit intersects
C using z=0, cot=0 (doesn't matter since I want the plane in the end.)
C 
C 	The wafer number is used to define a plane in r-phi, so it can be
C any wafer in the appropriate layer and octant. What happens is this:
C	1. The subroutine planevectors gets two vectors defining
C		the r-phi plane
C	2. The subroutine xyplane extrapolates the track and error 
C		matrix to that plane, in plane local coordinates
C	3. The 11 element of the returned error matrix is the
C		square of the r*dphi (=d(d0) ) error!  Yay!

C SVD parameters
#include "cl3seq/sv3seq/svgeompa.inc"       
C SVD parameters
#include "cl3seq/sv3seq/svlaseq.inc"        
C Initial design positions and offsets
#include "cl3seq/sv3seq/svgseq.inc"         
C Survey offsets 
#include "cl3seq/sv3seq/svzfdb.inc"         
C   #include "align.inc"          ! Hit and track arrays

C     local declarations
C     ==================
      
      INTEGER IMATRIX
      REAL wafer_origin(3),uvec(3),vvec(3)
      REAL trkerrpacked(15),veta(5,5),z0,cot,u,dummy,uverr(2,2)



C ----------Executable code starts here---------------------
C From C. Jessop 24-Jan-96
C Modified A. Foland 24-Jan-96

C Find vector to wafer origin and unit vectors in the
C plane of the wafer uvec = rphi and vvec=rz
C ==========================================


         call plnvec(wfn,wafer_origin,uvec,vvec)

C Find intercept of track with plane in wafer local co-ordinates
C ==============================================================

C Get the track error matrix and unpack to 5 by 5
C ===============================================
         do imatrix=1,6
          trkerrpacked(imatrix)=errm(imatrix)
         enddo

C Set the z and cot parts of the matrix to 0.00001 (0 probably bad)
C =================================================================

	 do imatrix=7,15
	  trkerrpacked(imatrix)=0.0
	 enddo
	 trkerrpacked(10)=1.0E-20
	 trkerrpacked(15)=1.0E-20

         call trupck(trkerrpacked,veta,5)          

C Set z, cot=0 since I don't care about Z
C  =======================================
	 z0=0
	 cot=0

C Find intercept
C  ==============

         call xyplan(c,d,phi,z0,cot,
     &                veta,wafer_origin,uvec,vvec,u,dummy,uverr)

C Local co-ordinates of projection
C ================================


	 if (uverr(1,1).lt.0) then
	    uverr(1,1)=-uverr(1,1)
	 endif

C ADF The above is clearly not a fine thing, since it should never be <0
         sdz=sqrt(uverr(1,1))


      RETURN
      END













