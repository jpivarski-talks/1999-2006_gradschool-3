*
* $Id: tfexer.F,v 1.5 2001/11/19 23:57:52 dpp Exp $
*
* $Log: tfexer.F,v $
* Revision 1.5  2001/11/19 23:57:52  dpp
*      -> add new control parameter include file
*      -> calculate extrapolation error for stereo layers used in LF fit
*
* Revision 1.4  2000/10/24 22:49:40  dpp
*      -> return error=0 for z extrapolation
*
* Revision 1.3  2000/08/18 18:49:53  dpp
*      -> remove call of LFEXER
*      -> use error matrix from sz_fitter for silicon-z as well as cathodes
*      -> remove call of TFINSV
*      -> remove PARXTR commons
*      -> obtain err matrix from ERR_MATRIX_ELEMENT
*      -> remove call of CFEXER
*      -> calculate contributions for error in extrapolating circle track
*      -> remove kabg arguments; use KAPCF etc
*      -> call CFFTEX when LCFEMC false; set LCFEMC
*      -> new arguments for CFFTEX
*      -> error handling for PARXTR,INVERS, argument for CFFTEX
*
* Revision 1.2  2000/07/10 15:07:39  dpp
* block argument of sqrt
*
* Revision 1.1.1.1  1998/02/06 19:11:44  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:30  clib
* Developmental version of DUET.
*
* Revision 1.1  1996/06/24 21:14:01  dpp
* interface to call CFFTEX and CFEXER
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author : A. Foland
      SUBROUTINE TFEXER(TFLYR,SDZ,ERR_RET)
C.......................................................................
C.
C. TFEXER: finds the error in the extrapolation of the current fit
C		to the layer TFLYR and returns it as SDZ
C.
C. COMMON    :
C. CALLS     : CFFTEX
C. CALLED    : TFXFIT
C. AUTHOR    : A. Foland
C.
C. VERSION   : 1.00
C. CREATED   : 24-Jan-96
C. LAST MOD  : 
C.
C.
C. Modification Log.
C. 1-29-96 ADF Added a fourth common for errmtx
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

C Argument declarations
C INPUTS  TFLYR:    the TF layer number
C RETURNS sdz:      the error (in either r-phi or in z) in extrapolation
C         ERR_RET   =0 IF NO ERROR

      INTEGER TFLYR
      REAL SDZ
      INTEGER ERR_RET



C local declarations
           
      REAL XTRY,YTRY,PHITRY
      INTEGER IER,IER_CFFTEX
      REAL ERR(6),ERR_CONT(6),ERR_CONT_TMP(6)
      REAL D_LOC,F_LOC,C_LOC,SD
      REAL SIN_TERM,COS_TERM,XI_TERM
      REAL dRESdC,dRESdF,dRESdD
      REAL S11,S12,S22
      INTEGER ITERM
      REAL RAD

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/controlpar.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/tftrakcd.inc"

C * ---------------Executable code starts here ---------------
C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
      
      ERR_RET=0
      RAD=RTF(TFLYR)

      IF(SAV_FIT_TYP.EQ.FITTYP_CF)THEN
         IF (IDVCTF(TFLYR).EQ.ISVX1) THEN

            IF(.NOT.LCFEMC)THEN
#if defined(CLEO_TFXFDIAG)
               PRINT 9001
 9001          FORMAT(' TFEXER: error matrix elements are stale:',
     2              ' call CFFTEX') 
#endif

               CALL CFFTEX(
     1              KAPCF,ALPCF,BETCF,GAMCF,PHI6CF,RKN6CF,
     2              D_LOC,F_LOC,C_LOC,IER_CFFTEX)

#if defined(CLEO_TFXFDIAG)
               PRINT 9003,IER_CFFTEX
 9003          FORMAT(' TFEXER: return from CFFTEX, ERR=',I4)
#endif
               IF(IER_CFFTEX.EQ.0)THEN
                  LCFEMC=.TRUE.
                  ERR_RET=0
               ELSE
                  ERR_RET=1
               ENDIF
            ELSE
               ERR_RET=0
#if defined(CLEO_TFXFDIAG)
               PRINT 9002
 9002          FORMAT(' TFEXER: error matrix elements are CURRENT:',
     2              ' do NOT call CFFTEX') 
#endif
            ENDIF


            IF(ERR_RET.GT.0)THEN
               SD=-1.
            ELSE

C this info should have been saved in tfhitscd
               CALL TFPHTR(ALPCF,BETCF,KAPCF,GAMCF,PHI6CF,RKN6CF,
     2              RSQTF(TFLYR),+1,
     3              XTRY,YTRY,PHITRY,IER)



C res=r**2  C  - (1+2AC)r SIN(f- f0 ) + A(1+AC)

               SIN_TERM=SIN(PHITRY-F_LOC)
               COS_TERM=COS(PHITRY-F_LOC)
               XI_TERM=1.+2.*D_LOC*C_LOC

               dRESdC=RAD**2 + 2.*D_LOC*RAD*SIN_TERM + D_LOC**2
               dRESdF=XI_TERM * RAD * COS_TERM
               dRESdD=2.*C_LOC*RAD*SIN_TERM + XI_TERM
            
               CALL ERR_MATRIX_ELEMENT(2,P3DCU,P3DCU,ERR(1))
               CALL ERR_MATRIX_ELEMENT(2,P3DFI,P3DCU,ERR(2))
               CALL ERR_MATRIX_ELEMENT(2,P3DFI,P3DFI,ERR(3))
               CALL ERR_MATRIX_ELEMENT(2,P3DDA,P3DCU,ERR(4))
               CALL ERR_MATRIX_ELEMENT(2,P3DDA,P3DFI,ERR(5))
               CALL ERR_MATRIX_ELEMENT(2,P3DDA,P3DDA,ERR(6))

               ERR_CONT(1)=     dRESdC*ERR(1)*dRESdC
               ERR_CONT(2)=     dRESdF*ERR(2)*dRESdC
               ERR_CONT(3)=     dRESdF*ERR(3)*dRESdF
               ERR_CONT(4)= 2.* dRESdD*ERR(4)*dRESdC
               ERR_CONT(5)= 2.* dRESdD*ERR(5)*dRESdF
               ERR_CONT(6)= 2.* dRESdD*ERR(6)*dRESdD



#if defined(CLEO_TFXFDIAG)
C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
               PRINT 9011,C_LOC,F_LOC,D_LOC,
     2              ERR(1),ERR(3),ERR(6),
     3              ERR(2),ERR(4),ERR(5)
 9011          FORMAT(/' ',
     1              ' TFEXER: RETURN FROM CFFTEX,        ',
     2              '    C=',F15.12,'    F=',F15.12,'    D=',F15.12/
     3              '       diagonal err matrix elements ',
     4              '  d2C=',F15.12,'  d2F=',F15.12,'  d2D=',F15.12/
     5              '   off diagonal err matrix elements ',
     6              ' dFdC=',F15.12,' dDdC=',F15.12,' dDdF=',F15.12)

               DO 11 ITERM=1,6
                  ERR_CONT_TMP(ITERM)=SIGN(
     2                 SQRT(ABS(ERR_CONT(ITERM))),ERR_CONT(ITERM))
 11            CONTINUE
            
               PRINT 9013,dRESdC,dRESdF,dRESdD,
     2              ERR_CONT_TMP(1),ERR_CONT_TMP(3),ERR_CONT_TMP(6),
     3              ERR_CONT_TMP(2),ERR_CONT_TMP(4),ERR_CONT_TMP(5)
            
 9013          FORMAT(/' ',
     1              ' TFEXER: dRES/dTERM and sqrt(cont)  ',
     2              '  "C"=',F15.12,'  "F"=',F15.12,'  "D"=',F15.12/
     3              '     diagonal signed sqrt(abs(cont))',
     4              '  d2C=',F15.12,'  d2F=',F15.12,'  d2D=',F15.12/
     5              ' off diagonal signed sqrt(abs(cont))',
     4              ' dFdC=',F15.12,' dDdC=',F15.12,' dDdF=',F15.12)
            
            
               PRINT 9014,dRESdC,dRESdF,dRESdD,
     2              ERR_CONT(1),ERR_CONT(3),ERR_CONT(6),
     3              ERR_CONT(2),ERR_CONT(4),ERR_CONT(5)
            
 9014          FORMAT(/' ',
     1              ' TFEXER: dRES/dTERM and contribution',
     2              '  "C"=',F15.12,'  "F"=',F15.12,'  "D"=',F15.12/
     3              '              diagonal contribution ',
     4              '  d2C=',F15.12,'  d2F=',F15.12,'  d2D=',F15.12/
     3              '          off diagonal contribution ',
     4              ' dFdC=',F15.12,' dDdC=',F15.12,' dDdF=',F15.12)
#endif
               SD=0.
               DO 12 ITERM=1,6
                  SD=SD+ERR_CONT(ITERM)
 12            CONTINUE
               
            ENDIF
         ELSE
            SD=0.
         ENDIF
         
      ELSEIF(SAV_FIT_TYP.EQ.FITTYP_LF)THEN
         IF(
     1        (IDVCTF(TFLYR).EQ.ISVZ1).OR.
     2        (IDVCTF(TFLYR).EQ.IVDCT).OR.
     3        (IDVCTF(TFLYR).EQ.IDRCT).OR.
     3        (IDVCTF(TFLYR).EQ.IDRFT)
     5        )THEN
         
            CALL ERR_MATRIX_ELEMENT(2,P3DZ0,P3DZ0,S11)
            CALL ERR_MATRIX_ELEMENT(2,P3DCT,P3DCT,S22)
            CALL ERR_MATRIX_ELEMENT(2,P3DCT,P3DZ0,S12)
            
#if defined(CLEO_TFXFDIAG)
            PRINT 9021,TFLYR,RAD,S11,S22,S12
 9021       FORMAT(' TFEXER: lyr,rad,S11,S22,S12=',
     2           I4,F8.4,2X,3F14.6)
#endif

            SD=S11 + RAD**2 * S22 + 2.*RAD * S12
         ELSE
            SD=0.
         ENDIF
      ENDIF


      IF(SD.LE.0.)THEN
         SDZ=0.
      ELSE
         SDZ=SQRT(SD)
      ENDIF

#if defined(CLEO_TFXFDIAG)
      PRINT 9031,TFLYR,RAD,SD,SDZ
 9031 FORMAT(' TFEXER: lyr',I4,'  rad',F8.4,
     2     '  SD',F18.12,'  EXTRAPOLATION ERROR=',F10.6)
#endif

      RETURN
      END




