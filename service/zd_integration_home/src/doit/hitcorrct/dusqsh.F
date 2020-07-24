*
* $Id: dusqsh.F,v 1.3 2003/03/05 17:48:56 dpp Exp $
*
* $Log: dusqsh.F,v $
* Revision 1.3  2003/03/05 17:48:56  dpp
*      -> changed all variable names in cdscrtcd to have common root,comment
*
* Revision 1.2  1998/07/09 00:39:57  lyon
* seq/cdgeom -> cl3seq/cdgm3
*
* Revision 1.1.1.1  1998/02/06 19:11:43  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:29  clib
* Developmental version of DUET.
*
* Revision 1.2  1996/03/26 17:05:50  zfiles
* A new version from bhb
*
* Revision 1.1.1.1  1994/10/08  01:00:52  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  6.00/34 29/09/94  20.36.50  by  CLEO II Librarian (UNIX)
*CMZ :  5.54/07 17/11/91  15.14.10  by  Steve Schaffner
*-- Author :    Steve Schaffner   14/11/91
      REAL FUNCTION DUSQSH(ICYL,IWIRE)
C.......................................................................
C.
C. DUSQSH - Correct alleged squash of the PT
C.
C. Inputs    : ICYL -- layer number, CD numbering
C.           : IWIRE
C.           
C. Outputs   : DUSQSH -- additive correction to EvntHit_PhiWir
C.
C. Called    : CDRWCP
C. Author    : Steve Schaffner  14/11/91  11.26.04
C.
C. Updates   : Bruce Behrens     1/18/96
C.           : converted to corrections stored in an array
C.           : rather than calculating a correction from 
C.           : a sine wave.  The array of corrections is
C.           : calculated from the mu-pair miss distances
C.           : by fitting a fourier series of sine and cosine
C.           : functions to the miss distances.  
C.           : I use 64 bins in the range from 0.007644 to 
C.           : 3.149237 radians.  The first bin corresponds
C.           : to MAG(1) below.
C.......................................................................
#if defined(CLEO_TYPECHEK)
      IMPLICIT NONE
#endif

#include "doit/duseq/tfindpar.inc"
#include "cl3seq/cdgm3/cdgeomcd.inc"
#include "doit/duseq/tfctlcde.inc"
      LOGICAL RCSINI
      CHARACTER*(*) CHRCSS, CHRCSR, CHRCST, CHRCSD, CHRCSA
      PARAMETER(CHRCSS='$RCSfile: dusqsh.F,v $')
      PARAMETER(CHRCSR='$Revision: 1.3 $')
      PARAMETER(CHRCST='$State: Exp $')
      PARAMETER(CHRCSD='$Date: 2003/03/05 17:48:56 $')
      PARAMETER(CHRCSA='$Author: dpp $')
      CHARACTER*(*) CRNAME
      PARAMETER(    CRNAME = 'DUSQSH' )

      INTEGER ICYL,IDEV,IWIRE
      INTEGER I
      REAL MAG(64), PHISQ
      DATA MAG /10.18, -18.32, -26.04, -47.63, -28.83,
     +          -21.19, -19.36,   5.29,   9.12,  -9.03,
     +          -28.93, -53.29, -12.97,   9.71,   9.41,
     +          -18.12, -68.49, -63.47, -37.92, -20.53,
     +          -21.53, -46.50, -41.18, -25.76, -42.33,
     +          -52.48, -43.70, -30.98, -45.00, -39.56,
     +          -28.97,  -0.06,   2.00,  -5.90, -13.71,
     +           -3.58,  25.17,  40.22,  19.17,  34.75,
     +           52.35,  49.42,  44.77,  18.93,  37.64,
     +           49.56,  50.90,  50.86,  24.65,  35.88,
     +           45.91,  42.59,  41.16,  40.94,  20.64,
     +           29.70,  11.04,  32.83,  13.27,   8.17,
     +           19.31,   8.10,  22.30,  -0.57/


* ----------------- Executable code starts here ------------
#if WRITE_RCS_INFO
      IF(RCSINI)THEN
        RCSINI = .FALSE.
        WRITE(6,*)'-------- CVSinfo --------'
        WRITE(6,*)CHRCSS
        WRITE(6,*)CHRCSR//' '//CHRCST
        WRITE(6,*)CHRCSD//' '//CHRCSA
      ENDIF
#endif

      DUSQSH = 0.0

      IDEV = IDVCCD(ICYL)
      IF ((IDEV.EQ.IPTLA).and.(.NOT.LMCRLO)) then
         I = 2*(IWIRE)+63+MOD(ICYL,2)
         IF (I.GE.129) I = I-128
         IF (I.GE.65) I = I-64
         IF ((I.LE.0).or.(I.GE.65)) then
            WRITE(6,*) "ERROR:  Index I out of range in Dusqsh"
            I = 64
         ENDIF
         DUSQSH = -MAG(I) * 1.0E-6 / RCD(ICYL)
      ENDIF

      RETURN
      END

