*
* $Id: doit_cd_scatters.F,v 1.3 2000/08/18 18:30:49 dpp Exp $
*
* $Log: doit_cd_scatters.F,v $
* Revision 1.3  2000/08/18 18:30:49  dpp
*      -> fix zeroing of unused scatters
*      -> add new TF scatters directly;
*         would like to remove the old CD scatters
*
* Revision 1.2  1998/07/09 00:49:02  lyon
* seq/cdgeom -> cl3seq/cdgm3
*
* Revision 1.1.1.1  1998/02/06 19:11:44  dpp
* DOIT first release.
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"

*-- Author :

      SUBROUTINE DOIT_CD_SCATTERS
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif

C. 10/04/89 by  Steve Schaffner
C. put in scattering media information here (pending developments in CDGEOMCD)

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "cl3seq/cdgm3/cdgeomcd.inc"
#include "doit/duseq/tfctlcde.inc"

      INTEGER I

C Scattering media information
C     RSCACD,XSCACD - Radius & thickness of scatterers

C (beam pipe = 1)

      RSCACD(1) = 0.035
C Changed beam pipe radius to something smaller than SVX dimension
C      THIS IS UGGLY BUT FOR TODAY... NUIMBERS WRONG ANYWAY... (A. Ryd)
      IF (IDTOCD(ISVX1).GT.0) RSCACD(1)=0.015
      XSCACD(1) = 0.0043

C (DR3 axial/stereo interface =2)
      RSCACD(2) = 0.370
      XSCACD(2) = 0.2

C RSCACD(3) is set to =0 so that MSCATF is not incremented for it in TFGMI1
      RSCACD(3) = 0.00
      XSCACD(3) = 0.02

      IF(NSCATF.GE.4)THEN
         DO 71 I=4,NSCATF
            RSCACD(I) = 0.0
            XSCACD(I) = 0.0
 71      CONTINUE
      ENDIF

C XSCCCD,RSCCCD - Radius and inverse rad.len. of continuous scatterers
      XSCCCD(1) = 0.0094
      XSCCCD(2) = 0.0069
      RSCCCD(1) = 0.045
      RSCCCD(2) = 0.166

      IF(NSCCTF.GE.3)THEN
         DO 73 I=3,NSCCTF
            RSCCCD(I) = 0.0
            XSCCCD(I) = 0.0
 73      CONTINUE
      ENDIF

C fill in the new TF scattering layer descriptions directly 

C 1: all material up to the 4th layer of silicon
C    at a radius between the 4th layer of silicon and the DR inner tube
C    Count the beam pipe and the carbon as more layers of silicon
C    Total is about 2% rl
      
      ACTIV_SCAT(1)=.TRUE.
      RAD_SCAT(1)=   .11
      R_LEN_SCAT(1)= 6.* .000300/.0936

C 2: 19% rl at the DR3 axial/stereo interface,
C    make kink with no stiffness

      ACTIV_SCAT(2)=.TRUE.
      RAD_SCAT(2)=   .37
      R_LEN_SCAT(2)= .19

      IF(M_SCAT.GE.3)THEN
         DO 91 I=3,M_SCAT
            ACTIV_SCAT(I)=.FALSE.
            RAD_SCAT(I)=   9.
            R_LEN_SCAT(I)= 0.
 91      CONTINUE
      ENDIF

      RETURN
      END






