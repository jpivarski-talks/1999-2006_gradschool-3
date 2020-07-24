
*
* $Id: cfdfit.F,v 1.9 2003/03/05 17:41:28 dpp Exp $
*
* $Log: cfdfit.F,v $
* Revision 1.9  2003/03/05 17:41:28  dpp
*      -> changed all variable names in cdscrtcd to have common root, comment
*
* Revision 1.8  2002/09/05 19:22:40  dpp
*      -> protect against weight=0
*      -> require STRFFT.GT.0 instead of .NE.0
*         now using STRFFT=-1 to designate orphaned hits
*
* Revision 1.7  2002/05/23 14:33:41  dpp
*     -> use new compiler switch to limit output
*
* Revision 1.6  2001/11/19 23:44:48  dpp
*      -> just remove some blanks that make it difficult to search
*
* Revision 1.5  2001/08/21 14:07:47  dpp
*      -> print more information for divergent fit
*      -> do not signal divergent fit for small expected change
*
* Revision 1.4  2001/03/06 20:35:14  dpp
*      -> diagnostics
*      -> return argument for divergence
*      -> check for divergence
*      -> return last good track parameters when it diverges
*
* Revision 1.3  2000/08/18 18:58:26  dpp
*      -> use XFIT instead of EvntHit_X, etc
*
* Revision 1.2  1999/09/12 22:35:13  dpp
* remove arguments, use CF directly; use NHITCF not NFIT in chisq,
* require STRFHT
*
* Revision 1.1.1.1  1998/02/06 19:11:42  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:27  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:00:49  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  5.54/07 20/05/91  09.57.19  by  Steve Schaffner
*CMZ :  5.51/00 10/08/89  15.34.59  by  Steve Schaffner
*CMZ :  5.50/00 25/02/88  19.50.58  by  R. Namjoshi
*-- Author :
      SUBROUTINE CFDFIT(DIVERGE)
C.......................................................................
C.
C. CFDFIT - Circle fit keeping GAM constant. Non-linear Newton
C.
C. COMMON    : ?
C. CALLS     : CDCFID CDKABG
C. CALLED    : ?
C. AUTHOR    : M. Ogg. Converted to Fortran-77 by R. Namjoshi
C.
C. VERSION   : 1.00
C. CREATED   : ?
C. LAST MOD  : 04-Jul-87
C.
C.
C. Modification Log.
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tftrakcd.inc"
#include "doit/duseq/cdscrtcd.inc"

C== Local variables

      INTEGER ITMAX,ITMAXPR
      REAL DCHI2,CHI2SV,CHI2_CALC_PREV,CHI2_DENOM,CHI2_SAVED
      PARAMETER( ITMAX=10, DCHI2=0.01 )
      REAL XI,CUR,PHI,D0,G1,G2,GG11,GG12,GG22,WGT,SIGTMP
      REAL THETAW,RESI,DFI1,DFI2,DET,DET1
      REAL HH11,HH12,HH22,DT1,DT2
      REAL DCHICF_EXPECT,RATIO_EXPECT,DCHICF_OBSERV,RATIO_OBSERV
      INTEGER ITRN,NGOOD,IFIT,IADR
      LOGICAL CONVERGE,DIVERGE,INVERT_OK,RESULT_SAVED

C ----------Executable code starts here---------------------------------
C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12

C-----------------------------------------------------------------------
C will return default chisq if not enough hits to even calculate chisq;
C will return last good result if there are enough hits
C-----------------------------------------------------------------------

      CONVERGE=.FALSE.
      RESULT_SAVED=.FALSE.

      IF(NFIT.GE.MNHTCF)THEN

C-----------------------------------------------------------------------
C enough entries in fit list; try to fit
C-----------------------------------------------------------------------

         XICF   = SQRT(1. + 4.*KAPCF*GAMCF)
         CALL CDCFID( CURCF,PHI0CF,D0CF,KAPCF,ALPCF,BETCF,GAMCF,XICF)
         CHI2SV = 1.E30

C-----------------------------------------------------------------------
C iterate fit
C-----------------------------------------------------------------------

         ITRN=0
 1       ITRN=ITRN+1
         
         CHICF=   0
         G1     = 0
         G2     = 0
         GG11   = 0
         GG12   = 0
         GG22   = 0

#if defined(CLEO_DPPDIAG)
         PRINT 1001,ITRN,
     1        CURCF,PHI0CF,D0CF
 1001    FORMAT(
     1        ' CFDFIT: ITRN=',I3,
     2        ' start iteration',
     3        ' cur=',F8.3,'  phi0=',F8.3,'  D0=',F8.3)
#if defined(CLEO_CFDFITDIAG)
         PRINT 1002
 1002    FORMAT(
     4        ' IFIT     RESI   THETAW    SIGMA       CHI2',
     5        '        GG11        GG12        GG22')
#endif
#endif
         
C-----------------------------------------------------------------------
C Loop thru hits, constructing HESSIAN matrix  ("whatever" dpp)
C-----------------------------------------------------------------------

         NHITCF= 0
         DO 20 IFIT = 1, NFIT
            IADR   = IABS(IADFIT(IFIT))
            IF((IADFIT(IFIT).NE.0).AND.(STRFFT(IFIT).GT.0))THEN
               NHITCF= NHITCF + 1

               CALL RESIDUALS('FIT',0,IFIT,RESI)
               PULFIT(IFIT) = RESI

               THETAW = ALPCF*YFIT(IFIT) - BETCF*XFIT(IFIT)

               WGT    = WGTFIT(IFIT)
               CHICF= CHICF + RESI**2*WGT
               
C-----------------------------------------------------------------------
C Derivatives
C-----------------------------------------------------------------------

               DFI1   = XFIT(IFIT)**2 + YFIT(IFIT)**2
               DFI2   = THETAW
               G1     = G1   - 2.*WGT*RESI*DFI1
               G2     = G2   - 2.*WGT*RESI*DFI2
               GG11   = GG11 + 2.*WGT*DFI1*DFI1
               GG12   = GG12 + 2.*WGT*DFI1*DFI2
               GG22   = GG22 + 2.*WGT*DFI2*DFI2

#if defined(CLEO_DPPDIAG)
#if defined(CLEO_CFDFITDIAG)
               IF(WGT.GT.0.)THEN
                 SIGTMP=1./SQRT(WGT)
               ELSE
                 SIGTMP=99.99999
               ENDIF
               PRINT 1003,IFIT,RESI,THETAW,SIGTMP,CHICF,GG11,GG12,GG22
 1003          FORMAT(I5,F9.6,F9.6,F9.6,F11.2,3E12.4)
#endif
#endif

            ENDIF
 20      CONTINUE

C-----------------------------------------------------------------------
C continue only if there are enough useable hits 
C prepare for next decision which is based on INVERT_OK
C-----------------------------------------------------------------------

         IF(NHITCF.GE.MNHTCF)THEN

C-----------------------------------------------------------------------
C save chisq from input track parameters
C check if fit is diverging
C-----------------------------------------------------------------------

            DCHICF_OBSERV=CHICF-CHI2_CALC_PREV
            IF(
     1           (ITRN.EQ.1).OR.
     2           (DCHICF_OBSERV.LT.0.))THEN
               CHI2_SAVED=CHICF
               CALL UCOPY(KAPCF, KAPTMP, NTKPAR)
               RESULT_SAVED=.TRUE.
               DIVERGE=.FALSE.
               INVERT_OK=.TRUE.

            ELSE
               CHI2_DENOM=CHI2_CALC_PREV
               IF(CHI2_DENOM.LT.1.)CHI2_DENOM=1.
               RATIO_EXPECT=ABS(DCHICF_EXPECT)/CHI2_DENOM
               RATIO_OBSERV=ABS(DCHICF_OBSERV)/CHI2_DENOM

               IF(RATIO_EXPECT.LT.2.E-6)THEN
                  DIVERGE=.FALSE.
                  INVERT_OK=.FALSE.

#if defined(CLEO_DPPDIAG)
#if defined(CLEO_CFDFITDIAG)
                  PRINT 1010,ITRN,CHI2_CALC_PREV,
     1                 DCHICF_EXPECT,DCHICF_OBSERV
 1010             FORMAT(
     1                 ' CFDFIT: ITRN=',I3,
     2                 ' SMALL EXPECTED CHANGE',
     3                 ' prev CHI2=',E13.6,
     4                 ' expect chi2 change=',E13.6,
     5                 ' observed change=',E13.6)
#endif
#endif

               ELSEIF(RATIO_OBSERV.LT.2.E-6)THEN
                  DIVERGE=.FALSE.
                  INVERT_OK=.FALSE.

#if defined(CLEO_DPPDIAG)
#if defined(CLEO_CFDFITDIAG)
                  PRINT 1011,ITRN,CHI2_CALC_PREV,
     1                 DCHICF_EXPECT,DCHICF_OBSERV
 1011             FORMAT(
     1                 ' CFDFIT: ITRN=',I3,
     2                 ' SMALL DIVERGENCE',
     3                 ' prev CHI2=',E13.6,
     4                 ' expect chi2 change=',E13.6,
     5                 ' observed change=',E13.6)
#endif
#endif

               ELSE
                  DIVERGE=.TRUE.
                  INVERT_OK=.FALSE.
                  
#if defined(CLEO_DPPDIAG)
                  PRINT 1004,ITRN,CHI2_CALC_PREV,
     1                 DCHICF_EXPECT,RATIO_EXPECT,
     2                 DCHICF_OBSERV,RATIO_OBSERV
 1004             FORMAT(
     1                 ' CFDFIT: ITRN=',I3,
     2                 ' CATCH DIVERGENCE',
     3                 ' prev CHI2=',E13.6,
     4                 ' expect chi2 change=',E13.6,' (',F5.3,')',
     5                 ' observed change=',E13.6,' (',F5.3,')')
#endif

               ENDIF
            ENDIF
            CHI2_CALC_PREV=CHICF

         ELSE
            DIVERGE=.FALSE.
            INVERT_OK=.FALSE.
         ENDIF

C-----------------------------------------------------------------------
C Invert matrix and transfer to new point in parameter space
C-----------------------------------------------------------------------

         IF(INVERT_OK)THEN

            DET    = (GG11*GG22 - GG12**2)

#if defined(CLEO_DPPDIAG)
#if defined(CLEO_CFDFITDIAG)
            PRINT 1005,DET
 1005       FORMAT(' CFDFIT: DET=',E18.6)
#endif
#endif

            DET1   = 1./DET
            HH11   =  DET1*GG22
            HH12   = -DET1*GG12
            HH22   =  DET1*GG11
         
            DT1    = -(HH11*G1 + HH12*G2)
            DT2    = -(HH12*G1 + HH22*G2)
            CURCF  = CURCF + DT1
            PHI0CF = PHI0CF + DT2
            DCHICF_EXPECT=
     1           (G1*DT1 + G2*DT2)
     2           + 0.5*(GG11*DT1**2 
     3           + 2.*GG12*DT1*DT2
     4           + GG22*DT2**2)
            CHICF= CHICF + DCHICF_EXPECT
            CALL CDKABG( CURCF,PHI0CF,D0CF,KAPCF,ALPCF,BETCF,GAMCF,XICF)

#if defined(CLEO_DPPDIAG)
            PRINT 1007,ITRN,
     1           CURCF,PHI0CF,DCHICF_EXPECT,
     2           KAPCF,ALPCF,BETCF,GAMCF,XICF
 1007       FORMAT(
     1           ' CFDFIT: ITRN=',I3,
     2           ' new point ',
     3           ' CUR=',F7.3,' PHI0=',F7.3,
     4           ' expect chi2 change=',F11.2,
     5           ' k=',F10.6,' a=',F10.6,' b=',F10.6,' g=',F10.6,
     6           ' x=',F10.6)
#endif
            
            IF(
     1           (ITRN.GT.1).AND.
     2           (ABS(CHICF-CHI2SV).LT.DCHI2))THEN
               CHICF= CHICF/FLOAT(NHITCF-2)
               CONVERGE=.TRUE.
            ENDIF
            CHI2SV= CHICF

         ENDIF
C-----------------------------------------------------------------------
C above is end of matrix inversion
C-----------------------------------------------------------------------
         
#if defined(CLEO_DPPDIAG)
         ITMAXPR=ITMAX
         PRINT 1009,ITRN,ITMAXPR,CONVERGE,DIVERGE,INVERT_OK
 1009    FORMAT(
     1        ' CFDFIT: ITRN=',I3,' (MAX)=',I3,
     2        '  CONVERGE=',L1,'  DIVERGE=',L1,
     3        ' INVERT_OK=',L1)
#endif

         IF(
     1        (ITRN.LT.ITMAX).AND.
     2        (.NOT.CONVERGE).AND.
     3        (INVERT_OK))GO TO 1

      ENDIF
C-----------------------------------------------------------------------
C above is end of test for enough entries in fit list
C-----------------------------------------------------------------------

C-----------------------------------------------------------------------
C if it did not converge, try to return to last good information
C-----------------------------------------------------------------------

      IF(.NOT.CONVERGE)THEN
         IF(RESULT_SAVED)THEN
            CHICF=CHI2_SAVED/FLOAT(NHITCF-2)
            CALL UCOPY(KAPTMP, KAPCF, NTKPAR)
         ELSE
            CHICF=CHIDEF
         ENDIF
      ENDIF

      RETURN
      END






