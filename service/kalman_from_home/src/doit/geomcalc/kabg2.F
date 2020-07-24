*
* $Id: kabg2.F,v 1.5 2001/11/19 23:46:13 dpp Exp $
*
* $Log: kabg2.F,v $
* Revision 1.5  2001/11/19 23:46:13  dpp
*      -> add new control parameter include file
*
* Revision 1.4  2001/03/06 20:32:37  dpp
*      -> provide rails in iterative calculation of pre-energy-loss momentum
*         to remove possible infinite loop
*
* Revision 1.3  2000/10/02 21:10:18  dpp
*      -> add new variables
*       -> iterate to find correct initial curvature for energy gain
*
* Revision 1.2  2000/08/18 19:03:29  dpp
*      -> new arguments, new assignments for ACTION
*      -> energy loss option
*
* Revision 1.1.1.1  1998/02/06 19:11:43  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:28  clib
* Developmental version of DUET.
*
* Revision 1.2  1996/02/29 17:41:34  zfiles
* defined kink in curvature; kink type=1
*
* Revision 1.1  1996/01/24 21:54:00  zfiles
* initial version: return kabg after a kink
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author : D. Peterson
      SUBROUTINE KABG2(ACTION,FIX_FLOAT,RK,STRENGTH,
     2                 RECALC_XI,
     3                 KA, AL, BE, GA, XI, KN, 
     4                 KA2,AL2,BE2,GA2,XI2)
C.......................................................................
C.
C. CFFTM2    : calculates a,b,g,x after at kink in r-phi at RK 
C.              recalulates XI if not provided:RECALC_XI.ne.0
C. COMMON    : 
C. CALLS     :
C. CALLED    :
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.00
C. CREATED   : 21-Dec-95
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
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/controlpar.inc"
#include "seq/clutil/mesrep.inc"


      REAL    KA       ,AL       ,BE       ,GA       ,XI
      REAL    KN
      REAL    RK       ,SAV_RK
      REAL    STRENGTH ,SAV_STRENGTH
      INTEGER ACTION   ,SAV_ACTION
      INTEGER FIX_FLOAT,SAV_FIX_FLOAT
      INTEGER RECALC_XI
      REAL    CU       ,D0
      REAL    SPHI0    ,SPHI0S   ,CPHI0    ,DDDK     ,KKRR
      REAL    RKSAV2
      REAL    KA2      ,AL2      ,BE2      ,GA2      ,XI2
      REAL    XI2SQR
      INTEGER IER
      REAL    XXINT    ,YYINT    ,FDUM
      REAL    MASS_PION/.139/
      REAL    PART_MOM_BEF,PART_MOM2_BEF,PART_E_BEF
      REAL    PART_MOM_BEF_MAX,PART_MOM_BEF_MIN,PART_MOM_BEF_TEST
      REAL    STRENGTH_USE,REL_BETA ,E_LOSS   
      REAL    PART_MOM_AFT,PART_MOM2_AFT,PART_E_AFT
      INTEGER MODE
      REAL    PART_MOM_TARGET,DIFF
      REAL    NEW_KA
      LOGICAL PRINTIT
      SAVE

C ACTION.........sets the kink status
C SAV_ACTION.....kink status, specifies transformation
C FIX_FLOAT......input/output flag of Fix or Float
C SAV_FIX_FLOAT..internal saved value
C STRENGTH.......strength of the kink
C                usually number os radiation lengths
C SAV_STRENGTH...internal saved value


C RECALC_XI.....recalculate XI from kabg
C               if =0, return input values
C SAV_RK........value of kink radius set at time of enable from RK

C all this enable stuff is installed so that 
C GEOMCALC/TFPHTR which does not have access to /TFTRAK/ ie KNK6CF, and 
C FITTER/CFFTX2 which is called with deviations of KAPCF etc. can be
C called without affecting the kabg variables

C NOTE: calculation must not change AL2 before AL is used, etc
C so that calling routine call use same arguments for input and output

      DATA SAV_ACTION/0/
      DATA SAV_RK/99999./

* ----------Executable code starts here---------------------
C set the ACTION status

      IF    (ACTION.EQ.KABG2_CLEAR)THEN
         PRINTIT=.FALSE.
         SAV_ACTION=KABG2_CLEAR
         SAV_RK=99999.
         SAV_FIX_FLOAT=KABG2_FIX
         SAV_STRENGTH=0.
      ELSEIF(ACTION.EQ.KABG2_PHI)THEN
         PRINTIT=.TRUE.
         SAV_ACTION=KABG2_PHI
         SAV_RK=RK
         SAV_FIX_FLOAT=FIX_FLOAT
         SAV_STRENGTH=STRENGTH
      ELSEIF(ACTION.EQ.KABG2_CURV)THEN
         PRINTIT=.TRUE.
         SAV_ACTION=KABG2_CURV
         SAV_RK=RK
         SAV_FIX_FLOAT=FIX_FLOAT
         SAV_STRENGTH=STRENGTH
      ELSEIF(ACTION.EQ.KABG2_ELOSS)THEN
         PRINTIT=.TRUE.
         SAV_ACTION=KABG2_ELOSS
         SAV_RK=RK
         SAV_FIX_FLOAT=FIX_FLOAT
         SAV_STRENGTH=STRENGTH

      ELSEIF(ACTION.EQ.KABG2_CALC)THEN
         PRINTIT=.FALSE.

C calculate XI if required
         
         IF(RECALC_XI.NE.KABG2_XI_RECALC)XI=SQRT(AL**2+BE**2)

C test for no operation

         IF(
     1        (SAV_ACTION.EQ.0).OR.
     1        (SAV_ACTION.EQ.KABG2_CLEAR))THEN
            KA2=KA
            AL2=AL
            BE2=BE
            GA2=GA
            XI2=XI
            KN=0.

C energy loss
C calculate the particle enery as if it were a pion
C ignore Z for now
C use min dE/dX for silicon for radiation length of silicon
C increase as 1/beta squared

C if strength <0, this is energy gain and must iterate to find correct
C   energy which would lose energy to come back to the input energy

            ELSEIF(SAV_ACTION.EQ.KABG2_ELOSS)THEN
               RKSAV2=SAV_RK**2
               CALL TFXYTR(AL,BE,KA,GA,RKSAV2,1,
     2              XXINT,YYINT,FDUM,IER)

               IF(IER.NE.0)THEN
                  KA2=KA
                  AL2=AL
                  BE2=BE
                  GA2=GA
                  XI2=XI
                  KN=0
               ELSE
c23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
                  PART_MOM_BEF=.225/ABS(KA)
                  STRENGTH_USE=SAV_STRENGTH
                  IF(SAV_STRENGTH.LT.0.)THEN
                     PART_MOM_TARGET=PART_MOM_BEF
                     MODE=1
#if defined(CLEO_DPPDIAG)
                     PRINT 3001,PART_MOM_BEF,STRENGTH_USE
 3001                FORMAT(' KABG2:energyGAIN:',
     1                    ' P(before)=',F9.6,' strength=',F11.6,
     2                    ' calculate energy gain')
#endif
                  ENDIF

 111              PART_MOM2_BEF=PART_MOM_BEF**2
                  PART_E_BEF=SQRT(PART_MOM_BEF**2+MASS_PION**2)
                  REL_BETA=PART_MOM_BEF/PART_E_BEF
                  E_LOSS=.036*STRENGTH_USE/REL_BETA**2
                  PART_E_AFT=PART_E_BEF-E_LOSS
                  PART_MOM2_AFT=PART_E_AFT**2-MASS_PION**2
                  IF(PART_MOM2_AFT.LT.0.)PART_MOM2_AFT=0.
                  PART_MOM_AFT=SQRT(PART_MOM2_AFT)
                  IF(PART_MOM_AFT.LT..0025)PART_MOM_AFT=.0025

                  IF(SAV_STRENGTH.LT.0.)THEN
                     IF(MODE.EQ.1)THEN
#if defined(CLEO_DPPDIAG)
                        PRINT 3002,PART_MOM_BEF,PART_MOM_AFT
 3002                   FORMAT(' KABG2:energyGAIN:',
     1                       ' P(before)=',F9.6,' P(after)=',F9.6,
     2                       ' calculate energy LOSS from',
     3                       ' the increased energy')
#endif
                        PART_MOM_BEF=PART_MOM_AFT
                        PART_MOM_BEF_MAX=PART_MOM_BEF
                        PART_MOM_BEF_MIN=PART_MOM_TARGET
                        STRENGTH_USE=-SAV_STRENGTH
#if defined(CLEO_DPPDIAG)
                        PRINT 3003,PART_MOM_BEF_MAX,PART_MOM_BEF_MIN
 3003                   FORMAT('            mode 1',
     1                       ' P(before)MAX=',F9.6,
     2                       ' P(before)MIN=',F9.6)
#endif
                        MODE=2
                        GO TO 111


                     ELSE
                        DIFF=PART_MOM_AFT-PART_MOM_TARGET

#if defined(CLEO_DPPDIAG)
                        PRINT 3004,PART_MOM_BEF,PART_MOM_AFT,
     2                       PART_MOM_TARGET,DIFF,
     3                       PART_MOM_BEF_MAX,PART_MOM_BEF_MIN
 3004                   FORMAT(' '/' KABG2:energyGAIN:',
     1                       ' P(before)=',F9.6,' P(after)=',F9.6,
     2                       ' target=',F9.6,' DIFF=',F9.6/
     3                       '            mode 2',
     4                       ' P(before)MAX=',F9.6,
     5                       ' P(before)MIN=',F9.6)
#endif

                        IF((DIFF/PART_MOM_TARGET).GT. 0.005)THEN
                           IF(PART_MOM_BEF.LT.PART_MOM_BEF_MAX)
     2                          PART_MOM_BEF_MAX=PART_MOM_BEF
                           PART_MOM_BEF_TEST=PART_MOM_BEF-DIFF/2.
#if defined(CLEO_DPPDIAG)
                        PRINT 3005,PART_MOM_BEF_TEST,
     2                          PART_MOM_BEF_MIN
 3005                   FORMAT(' KABG2:energyGAIN:',
     1                       ' P(before)_TEST=',F9.6,
     2                       ' P(before)MIN=',F9.6)
#endif
                           IF(PART_MOM_BEF_TEST.LE.PART_MOM_BEF_MIN)
     2                          PART_MOM_BEF_TEST=
     3                          (PART_MOM_BEF+PART_MOM_BEF_MIN)/2.
                           PART_MOM_BEF=PART_MOM_BEF_TEST

#if defined(CLEO_DPPDIAG)
                           PRINT 3006,PART_MOM_BEF
 3006                      FORMAT(' KABG2:energyGAIN:',
     1                          ' recalculate energy loss from',
     2                          ' new starting energy',
     3                          ' P(before)=',F9.6)
#endif
                           GO TO 111

                        ELSEIF((DIFF/PART_MOM_TARGET).LT.-0.005)THEN
                           IF(PART_MOM_BEF.GT.PART_MOM_BEF_MIN)
     2                          PART_MOM_BEF_MIN=PART_MOM_BEF
                           PART_MOM_BEF_TEST=PART_MOM_BEF-DIFF/2.
#if defined(CLEO_DPPDIAG)
                        PRINT 3007,PART_MOM_BEF_TEST,
     2                          PART_MOM_BEF_MAX
 3007                   FORMAT(' KABG2:energyGAIN:',
     1                       ' P(before)_TEST=',F9.6,
     2                       ' P(before)MAX=',F9.6)
#endif
                           IF(PART_MOM_BEF_TEST.GE.PART_MOM_BEF_MAX)
     2                          PART_MOM_BEF_TEST=
     3                          (PART_MOM_BEF+PART_MOM_BEF_MAX)/2.
                           PART_MOM_BEF=PART_MOM_BEF_TEST

#if defined(CLEO_DPPDIAG)
                           PRINT 3006,PART_MOM_BEF
#endif
                           GO TO 111

                        ELSE
                           PART_MOM_AFT=PART_MOM_BEF
#if defined(CLEO_DPPDIAG)
                           PRINT 3009,PART_MOM_AFT
 3009                      FORMAT(' KABG2:energyGAIN:',
     1                          ' target reached;',
     2                          ' momentum after GAIN=',F9.6)
#endif
                        ENDIF
                     ENDIF
                  ENDIF
                  
                  NEW_KA=.225/PART_MOM_AFT
                  IF(KA.LT.0.)NEW_KA=-NEW_KA
                  KN=NEW_KA-KA

                  KA2=   KA +           KN
                  AL2=   AL -2.*XXINT*  KN
                  BE2=   BE -2.*YYINT*  KN
                  XI2SQR=AL2**2+BE2**2
                  XI2=SQRT(XI2SQR)
                  GA2=(XI2SQR-1.)/(4.*KA2)
               ENDIF

C kink in curvature
C kink is additive to allow kink at kappa=0
C it was found, 2/16/96, that this does not converge well for
C curvature defined only in the VD

            ELSEIF(SAV_ACTION.EQ.KABG2_CURV)THEN
               RKSAV2=SAV_RK**2
               CALL TFXYTR(AL,BE,KA,GA,RKSAV2,1,
     2              XXINT,YYINT,FDUM,IER)
               IF(IER.NE.0)THEN
                  KA2=KA
                  AL2=AL
                  BE2=BE
                  GA2=GA
                  XI2=XI
                  KN=0
               ELSE
                  KA2=   KA +           KN
                  AL2=   AL -2.*XXINT*  KN
                  BE2=   BE -2.*YYINT*  KN
                  XI2SQR=AL2**2+BE2**2
                  XI2=SQRT(XI2SQR)
                  GA2=(XI2SQR-1.)/(4.*KA2)
               ENDIF
C kink in phi

            ELSEIF(SAV_ACTION.EQ.KABG2_PHI)THEN
               RKSAV2=SAV_RK**2
               SPHI0 = AL/XI
               CPHI0 =-BE/XI
               D0 = 2.*GA/(1.+XI)
               CU = KA
               KKRR=KA**2*RKSAV2
               IF(KKRR.LT.1.)THEN
                  DDDK=-SAV_RK*SQRT(1.-KKRR)
               ELSE
                  DDDK=0.
               ENDIF
               
               SPHI0S = SPHI0
               SPHI0  = SPHI0 + KN*CPHI0
               CPHI0  = CPHI0 - KN*SPHI0S
               D0     = D0    + KN*DDDK
               
               XI2   = 1. + 2.*D0*CU
               KA2   = CU
               AL2   = XI2*SPHI0
               BE2   =-XI2*CPHI0
               GA2   = D0*(1.+D0*CU)
            ELSE
               WRITE(CHMESS,2001)ACTION
               CALL MESLOG('KABG2',1,MSERRO)
            ENDIF 

      ELSE
         WRITE(CHMESS,2002)ACTION
         CALL MESLOG('KABG2',1,MSERRO)
      ENDIF

 2001 FORMAT(' undefined SAV_ACTION value=',I10)
 2002 FORMAT(' undefined ACTION value=',I10)


#if defined(CLEO_DPPDIAG)
      IF(PRINTIT)THEN
         PRINT 1001,SAV_ACTION,SAV_FIX_FLOAT,SAV_RK,SAV_STRENGTH
 1001    FORMAT(' KABG2: action=',I2,' fix/float=',I2,
     2        '  radius=',F9.3,' strenth=',F9.5)
      ENDIF
#endif

      RETURN
      END
