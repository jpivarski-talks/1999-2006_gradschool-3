*
* $Id: parxtr.F,v 1.6 2002/12/06 17:02:01 dpp Exp $
*
* $Log: parxtr.F,v $
* Revision 1.6  2002/12/06 17:02:01  dpp
*    -> abort fit if the calculate move in parameter space is grossly larger
*       than the derivative-calculating grid size to avoid nasty infinity
*
* Revision 1.5  2002/08/15 18:57:00  dpp
* will jump out of infinite loop cuase by NaN, and
* F77 changes
*
* Revision 1.4  2002/06/06 18:37:35  atems
*         ACCEPT changed to READ on all platforms in parxtr.F
*
* Revision 1.3  2002/06/05 03:04:07  atems
* minor fixes to compile under linux
*
* Revision 1.2  2000/08/18 19:00:16  dpp
*      -> error handling for INVERS
*
* Revision 1.1.1.1  1998/02/06 19:11:42  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:28  clib
* Developmental version of DUET.
*
* Revision 1.2  1996/06/24 21:18:50  dpp
* add argument to/and allow return with only error matrix without minimization
*
* Revision 1.1  1996/01/24 21:44:13  zfiles
* initial version: chi square minimization by parabolic extrapolation
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author : D. Peterson
      SUBROUTINE PARXTR(DUMP_LOCAL,DUMP_IN_CH2,MINIMIZE,ERR_RET)
C.......................................................................
C.
C. PARXTR    : PARabolic eXTRapolation of chi-squared
C.
C. COMMON    : /FVARI/ /FVARR/ /FVARC/
C. CALLS     : UCOPY, DEFCH2, INVMULT
C. CALLED    : CFFT4D
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.00
C. CREATED   : 18-Dec-95
C. LAST MOD  : 
C.
C.
C. Modification Log.
C. 1-29-96   : Added MINIMIZE: if FALSE, calculate error matrix only 
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

C***********************************************************************
C***********************************************************************

C--- minimizes a user defined chi-squared
C--- relative to user defined variables
C--- by means of a PARabolic eXTRapolation of chi-squared

C The user defined chi-squared routine must be of the form
C     SUBROUTINE DEFCH2(CH2,IERR,DUMP_IN_CH2,LEXTRA)
C where CH2          (real) is the returned value of chi-squared,
C       IERR         (integer) is returned zero if there are no errors
C                        encountered; otherwise minimization ends,
C       DUMP_IN_CH2  (logical) is a flag to type various diagnostics, and
C       LEXTRA       (logical) is an extra logical to be used by other
C                        calling routines. PARXTR always sets this FALSE.
C                        Other routines could use it to eg. to freeze
C                        an element list.

C The calling routine must provide the values of
C      the fitting variables,    VARNOW
C      and various bin sizes,    BINSZS,BNSZL1,BNSZL2
C along with the number of fitting variables and auxiliry variables
C                                NVARF,NVARX.
C If auxiliary variables are used, values must be provided in DEFCH2.

C***********************************************************************
C***********************************************************************

C-----------------------------------------------------------------------
C argument variables
C-----------------------------------------------------------------------

C --> variables must befined at CALL to subroutine
C DUMP_LOCAL ...type various diagnostics inside this routine
C DUMP_IN_CH2 ..type various diagnostics inside the chi-squared routine
C MINIMIZE......true: minimize here; false: return error matrix
C ERR_RET.......error rerurn=0 for no error

      LOGICAL DUMP_LOCAL,DUMP_IN_CH2,MINIMIZE
      INTEGER ERR_RET

C-----------------------------------------------------------------------
C COMMONs used in parabolic extrapolation of chi-squared
C-----------------------------------------------------------------------

C --> must be included and defined by the calling routine
C     and must be included in the user defined chi-squared
C MVAR  ..array size for the user defined fitting variables
C MVARX ..array size for auxiliary variables defined in the chi-squared

      INTEGER MVAR,MVARX
      PARAMETER(MVAR=6,MVARX=6)

C NVARF ..number of user defined variables
C NVARX ..number of auxiliary variables defined in the chi-squared
C CH2NOW..input and returned chi-squared; can be initiated at a known
C         value at the start point, or sufficiently high
C VARNOW..starting point and returned values of the user variables,
C         respect to which the user defined chi-squared will be minimized
C VARAXN..returned values auxiliary variables defined in the chi-squared
C BINSZS..starting bin size, granularity, for calculating derivatives
C BNSZL1..limit for starting the bin size for an iteration
C BNSZL2..limit of reduction of bin size when attempting to
C         look for convergence of the derivatives
C NMVARF..character string to identify the user variables in diagnostics
C NMVARX..character string to identify the auxiliary variables in diagnostics
C ERRMTX..error matrix; if no inversion 

      INTEGER     NVARF, NVARX
      REAL        CH2NOW
      REAL        VARNOW,VARAXN,BINSZS,BNSZL1,BNSZL2
      REAL*8 	  ERRMTX
      CHARACTER*4 NMVARF,NMVARX
      COMMON/FVARI/
     1      NVARF,     NVARX
      COMMON/FVARR/
     1      CH2NOW,
     2      VARNOW(MVAR), VARAXN(MVARX),
     3      BINSZS(MVAR), BNSZL1(MVAR), BNSZL2(MVAR)
      COMMON/FVARC/
     1      NMVARF(MVAR), NMVARX(MVARX)
      COMMON/FVARER/
     1      ERRMTX(MVAR,MVAR)


C-----------------------------------------------------------------------
C local variables used in parabolic extrapolation of chi-squared
C-----------------------------------------------------------------------

C LEXTRA......extra logical defined for DEFCH2
              LOGICAL LEXTRA
C IERRF ......integer error flag used in various calls
              INTEGER IERRF 
C VAR0 .......general index
C VAR1 .......index used to count the matrix row
C VAR2 .......index used to count the matrix column
              INTEGER VAR0,VAR1,VAR2
C POINT1 .....index indicates point in variable space for calculated deriv
C POINT2 .....index indicates point in variable space for calculated deriv
              INTEGER POINT1,POINT2
C ITERAT......iteration number
              INTEGER ITERAT
C ITER_ALLOW .this main iteration is allowed
              LOGICAL ITER_ALLOW
C MAX_ITER....maximum iteration number
              INTEGER MAX_ITER
C PASS_NUM....indicates pass number in testing for convergence of derivatives
              INTEGER PASS_NUM
C PASS ALLOW .pass with granularity size allowed
              LOGICAL PASS_ALLOW
C STEP_NUM....step number, counted to jump out of infinite loop
              INTEGER STEP_NUM 
C MAX_STEP....maximum number of step allowed, to jump out of infinite loop
              INTEGER MAX_STEP
C STEP_ALLOW .another step in searching parameter space is allowed
              LOGICAL STEP_ALLOW
C ATTEMPT1....indicates the first attemp at a new point
              LOGICAL ATTEMPT1
C HAVDIR......inticates that the direction to walk in chi-squared is known
              LOGICAL HAVDIR
C TSTDIR......counts the number of signs of the walk direction 
C              that have been tested (and failed)
              INTEGER TSTDIR
C BINSZI......starting bin size, or granularity, for an iteration
C BINSZN......current bin size, or granularity, for an iteration
C BSZL1L......limit for starting the bin size for an iteration,local
C BSZL2L......limit of reduction of bin size when attempting to
C             look for convergence of the derivatives,local
              REAL BINSZI(MVAR),BINSZN(MVAR),BSZL1L(MVAR),BSZL2L(MVAR)
C CH2OLD......chi-squared from previous iteration
C CH2BST......best chi-squared found
              REAL CH2OLD,CH2BST
C CH2TST......chi-squared at the 4 points for determining the derivatives
              REAL CH2TST(0:1,0:1)
C CH2BCD......Best chi-squared found while Calculating Derivatives
              REAL CH2BCD
C DENOM1......denominator for the first order derivative
C DENOM2......denominator for the second order derivative
              REAL DENOM1,DENOM2
C DERIV_CONV..indicates that derivative is converged
              LOGICAL DERIV_CONV
C DIFF1.......difference, numerator for 1st derivative
C DIFF2.......difference, numerator for 2nd derivative
              REAL DIFF1,DIFF2
C DIRIV1......1st derivative, for current and previous pass
C DIRIV2......2nd derivative, for current and previous pass
              REAL DIRIV1(2),DIRIV2(2)
C DIFF1I......inverse of 1st derivative
C DIFF2I......inverse of 2nd derivative
              REAL DIFF1I,DIFF2I
C DRV2SM......abs value of sum of the derivative from this and earlier pass
C DRVASM......sum of abs value of the derivative from this and earlier pass
C DRV2DF......abs value of diff of the derivative from this and earlier pass
              REAL DRVASM,DRV2SM,DRV2DF
C MOVDIR......signed multiplier of the gradiant
              REAL MOVDIR
C GRADNT......gradiant; amount to change variable to reach apparent minimum
C GRADMI......gradiant: found from matrix inversion
              REAL GRADNT(MVAR),GRADMI(MVAR)
C GRAD_CAL_D..gradiant: found while calculating derivatives
              REAL GRAD_CAL_D(MVAR)
C L_GRAD_C_D..logical: use of the GRAD_CAL_D gradiant, 
              LOGICAL L_GRAD_C_D
C VSTART......values of user variables at the start of an iteration
C VARBST......current best found values of the user variables
              REAL VSTART(MVAR),VARBST(MVAR)
C VARAXB......current values of the auxiliary values corresponding to the
C             best found values of the user variables
              REAL VARAXB(MVARX)
C NOTE1 ......character string for diagnostic purposes
C NOTE2 ......character string for diagnostic purposes
              CHARACTER*8 NOTE1,NOTE2
C MATSQR......square matrix used in fitting
C             corresponds to alpha of equation 11-13, page 224 of Bevington
C             sent to the matrix inverter to be substituted 
C             with the error matrix
C MATLIN......column matrix used in fitting
C             corresponds to beta of equation 11-13, page 224 of Bevington
              REAL MATSQR(MVAR,MVAR),MATLIN(MVAR)
C MAT8........REAL*8 version of MATSQR for inversion
              REAL*8 MAT8(MVAR,MVAR)
C DET8........determinant from INVERS
              REAL*8 DET8
C YN..........to accept input during diagnostics
              INTEGER YN
C KILLA.......to limit annoying questions
              INTEGER KILLA
              DATA KILLA /0/

C***********************************************************************
C executable code starts here
C***********************************************************************

      IF(DUMP_LOCAL)THEN
        PRINT 8850,CH2NOW
 8850   FORMAT(' PARXTR: enter with CH2NOW=',F13.6)
      ENDIF

C-----------------------------------------------------------------------
C initialize the first iterations, protect against negative values
C-----------------------------------------------------------------------

      ERR_RET=0
      LEXTRA=.FALSE.
      MAX_ITER=2
      MAX_STEP=1000
      
      CH2BST=CH2NOW
      CH2BCD=CH2NOW
      CALL UCOPY(VARNOW,VARBST,NVARF)
      
      DO 101 VAR1=1,NVARF
        BINSZI(VAR1)=ABS(BINSZS(VAR1))
        BSZL1L(VAR1)=ABS(BNSZL1(VAR1))
        BSZL2L(VAR1)=ABS(BNSZL2(VAR1))
 101  CONTINUE
      
      ITERAT=0
 111  ITERAT=ITERAT+1

C-----------------------------------------------------------------------
C test if another iteration is allowed
C-----------------------------------------------------------------------
      IF(ITERAT.EQ.1)THEN
        ITER_ALLOW=.TRUE.
      ELSE
        ITER_ALLOW=.TRUE.
        IF(ITERAT.GT.MAX_ITER)ITER_ALLOW=.FALSE.
        IF(ITERAT.GT.4)ITER_ALLOW=.FALSE.
        
        CALL UCOPY(BINSZN,BINSZI,NVARF)
        DO 121 VAR0=1,NVARF
          IF( BINSZN(VAR0).LT.BSZL1L(VAR0) )ITER_ALLOW=.FALSE.
 121    CONTINUE
        
        IF(CH2BST.GT.CH2OLD-0.00001)ITER_ALLOW=.FALSE.
      ENDIF
      
      IF(ITER_ALLOW)THEN

C-----------------------------------------------------------------------
C reset values for the next iteration
C-----------------------------------------------------------------------

        CH2OLD=CH2BST
        CALL UCOPY(VARBST,VSTART,NVARF)

C-----------------------------------------------------------------------
C double loop over the variables
C this loop specifies a location in the square matrix
C-----------------------------------------------------------------------

        VAR1=0
 611    VAR1=VAR1+1
        IF(VAR1.LE.NVARF)THEN
          
          IF(DUMP_LOCAL)THEN
            PRINT 8800,VAR1
 8800       FORMAT(' PARXTR: outer loop over variable number',I2)
#if defined(CLEO_DPPDIAG)
            IF(KILLA.NE.1)THEN 
              CALL DSF_RFV_INPUT(YN)
              IF(YN.EQ.'K')KILLA=1
            ENDIF
#endif
          ENDIF

C calculate only the triangle, use symmetry 
          VAR2=VAR1-1
C calculate the full square matrix
c          VAR2=0
 612      VAR2=VAR2+1
          IF(VAR2.LE.NVARF)THEN


C-----------------------------------------------------------------------
C test multiple values of the bin size to establish derivative
C-----------------------------------------------------------------------

            DERIV_CONV=.FALSE.
            PASS_NUM=0
 621        PASS_NUM=PASS_NUM+1
            PASS_ALLOW=.TRUE.

C-----------------------------------------------------------------------
C start the first pass using the initial bin size
C-----------------------------------------------------------------------
            IF(PASS_NUM.EQ.1)THEN
              CALL UCOPY(BINSZI,BINSZN,NVARF)
              
              IF(DUMP_LOCAL)THEN
                PRINT 8801,VAR1,VAR2,(NMVARF(VAR0),VAR0=1,NVARF)
 8801           FORMAT(' matrix point:',I2,I2,
     2               ' start to calculate derivatives respect to '
     3               10(A4,','))
                PRINT 8802,            (BINSZN(VAR0),VAR0=1,NVARF)
 8802           FORMAT(45X,'(granularity)  ',10F10.6)
              ENDIF
              
C-----------------------------------------------------------------------
C later pass
C swap the results of the previous bin size into the second position
C reduce the bin size for the next iteration
C-----------------------------------------------------------------------
              
            ELSE
              DIRIV2(2)=DIRIV2(1)
              DIRIV1(2)=DIRIV1(1)
              
              DO 622 VAR0=1,NVARF
                BINSZN(VAR0)=BINSZN(VAR0)/2.
 622          CONTINUE
              
C-----------------------------------------------------------------------
C accept the last value when the bin size gets too small
C-----------------------------------------------------------------------
              
              DO 623 VAR0=1,NVARF
                IF( BINSZN(VAR0).LT.BSZL2L(VAR0))PASS_ALLOW=.FALSE.
 623          CONTINUE
            ENDIF

C-----------------------------------------------------------------------
C set the denominator of the derivatives for this bin size
C-----------------------------------------------------------------------
            IF(PASS_ALLOW)THEN
              DENOM1=1.
              DENOM2=1.
              DENOM1=DENOM1*BINSZN(VAR1)
              DENOM2=DENOM2*BINSZN(VAR1)
              DENOM2=DENOM2*BINSZN(VAR2)

C-----------------------------------------------------------------------
C get the current CH2 at the 4 points for a particular bin size
C    for the particular set of 2 variables (WHICH MAY BE THE SAME VARIABLE)
C this loop specifies the points in chisquared space used to calculate
C    the second derivative
C-----------------------------------------------------------------------

              IF(DUMP_LOCAL)THEN
                PRINT 8803,VAR1,VAR2,(NMVARF(VAR0),VAR0=1,NVARF)
                PRINT 8804,            (BINSZN(VAR0),VAR0=1,NVARF)
              ENDIF
 8803         FORMAT(' matrix point:',I2,I2,
     2             '      now calculate derivatives respect to '
     3             10(A4,','))
 8804         FORMAT(45X,'(granularity)  ',10F10.6)
              
              DO 649 POINT1=0,1
                DO 648 POINT2=0,1
                  
                  CALL UCOPY(VSTART,VARNOW,NVARF)
                  
                  IF(POINT1.EQ.1)VARNOW(VAR1)=VARNOW(VAR1)+BINSZN(VAR1)
                  IF(POINT2.EQ.1)VARNOW(VAR2)=VARNOW(VAR2)+BINSZN(VAR2)
                  
                  CALL DEFCH2(CH2TST(POINT1,POINT2),
     2                 IERRF,DUMP_IN_CH2,LEXTRA)

                  IF(IERRF.NE.0)THEN
                    ERR_RET=100+IERRF
                    GO TO 902
                  ENDIF
                  IF(DUMP_LOCAL)THEN
                    PRINT 8805,VAR1,VAR2,POINT1,POINT2,
     2                   (NMVARF(VAR0),VAR0=1,NVARF)
                    PRINT 8806,(VSTART(VAR0),VAR0=1,NVARF)
                    PRINT 8807,(VARNOW(VAR0),VAR0=1,NVARF)
                    PRINT 8808,CH2TST(POINT1,POINT2)
                  ENDIF
 8805             FORMAT(' '/' matrix point:',I2,I2,14X,
     2                 'calculated point',I2,I2/
     3                 ' values of ',10(A4,','))
 8806             FORMAT(' at the start...',10F10.6)
 8807             FORMAT('      and now-->',10F10.6)
 8808             FORMAT(48X,' chi-squared at point=',F13.6)
                  
                  IF(CH2TST(POINT1,POINT2).LT.CH2BCD)THEN
                    
                    IF(DUMP_LOCAL)THEN
                      PRINT 8834
                    ENDIF
 8834               FORMAT(48X,' this is a new best chi-squared') 
                    
                    CH2BCD=CH2TST(POINT1,POINT2)

                    DO 645 VAR0=1,NVARF
                      GRAD_CAL_D(VAR0)=VARNOW(VAR0)-VSTART(VAR0)
 645                CONTINUE
                  ENDIF
                    
                    
 648            CONTINUE
 649          CONTINUE

C-----------------------------------------------------------------------
C calculate the first and second order derivatives
C-----------------------------------------------------------------------

              IF(DUMP_LOCAL)THEN
                PRINT 8809,DENOM1,DENOM2,PASS_NUM
 8809           FORMAT(' will calculate derivatives: DENOM1/2=',2E11.3,
     2               ', pass number=',I2)
              ENDIF
              
              DIFF1      =  CH2TST(1,0)-CH2TST(0,0)
              DIRIV1(1)  =  DIFF1/DENOM1
              DIFF2      =  CH2TST(1,1)-CH2TST(0,1)-DIFF1
              DIRIV2(1)  =  DIFF2/DENOM2
              
              IF(DUMP_LOCAL)THEN
                PRINT 8810,DIFF1,DENOM1,DIRIV1(1),
     2               DIFF2,DENOM2,DIRIV2(1)
 8810           FORMAT('  first derivative DIF=',F13.6,
     2               ' DENOM=',E11.3,' DIRIV=',E11.3/
     3               ' second derivative DIF=',F13.6,
     2               ' DENOM=',E11.3,' DIRIV=',E11.3)
              ENDIF
              
C-----------------------------------------------------------------------
C calculate the optimum bin size from the first derivative
C-----------------------------------------------------------------------

              IF(VAR1.EQ.VAR2)THEN
                IF(DIRIV1(1).NE.0.)THEN
                  DIFF1I=1./DIRIV1(1)
                  
                  IF(DUMP_LOCAL)THEN
                    PRINT 8811,DIFF1I
 8811               FORMAT(' first derivative implies ',
     2                   'GRANULARITY=',F13.6)
                  ENDIF
                  
                ELSE
                  DIFF1I=BINSZN(VAR1)
                  
                  IF(DUMP_LOCAL)THEN
                    PRINT 8812,DIFF1I,VAR1
 8812               FORMAT(' first derivative =0,     ',
     2                   'GRANULARITY=',F13.6,
     3                   ' <<<<< THIS IS A PROBLEM >>>>  VAR1=',I2)
#if defined(CLEO_DPPDIAG)
                      CALL DSF_RFV_INPUT(YN)
#else
                    READ 8819,YN
#endif
 8819               FORMAT(A1)            
                  ENDIF
                  
                ENDIF
              ENDIF
              
C-----------------------------------------------------------------------
C look for convergence of the second derivative
C-----------------------------------------------------------------------

              IF(PASS_NUM.NE.1)THEN
                DRVASM =.5*( ABS( DIRIV2(1) )  +  ABS( DIRIV2(2) ) )
                
                IF(DUMP_LOCAL)THEN
                  PRINT 8813,DRVASM
 8813             FORMAT(' REQUIRE DRVASM=',E11.3,' .GE. 1.0e-9')
                ENDIF
                
                IF(  DRVASM         .LT. 1.E-9)THEN
                  IF(DUMP_LOCAL)THEN
                    PRINT 8816
 8816               FORMAT(' ABORT EVENT')
                  ENDIF
                  ERR_RET=200
                  GO TO 902
                ENDIF
                
                DRV2SM =.5*( ABS( DIRIV2(1)  +  DIRIV2(2) ) )
                
                IF(DUMP_LOCAL)THEN
                  PRINT 8814,DRV2SM,DRVASM
 8814             FORMAT(' REQUIRE DRV2SM/DRVASM=',E11.3,'/',E11.3,
     2                 ' .GE. 1.0e-5')
                ENDIF
                
                IF( (DRV2SM/DRVASM) .LT. 1.E-5)THEN
                  IF(DUMP_LOCAL)THEN
                    PRINT 8816
                  ENDIF
                  ERR_RET=300
                  GO TO 902
                ENDIF
                
                DRV2DF =     ABS( DIRIV2(1)  -  DIRIV2(2) )
                
                IF(DUMP_LOCAL)THEN
                  PRINT 8815,DRV2DF,DRV2SM
                ENDIF
 8815           FORMAT(' REQUIRE DRV2DF/DRV2SM=',E11.3,'/',E11.3,
     2               ' .LT. 0.3')
                
                IF( (DRV2DF/DRV2SM) .GT.  0.3)THEN
                  IF(DUMP_LOCAL)THEN
                    PRINT 8817
 8817               FORMAT(' LOOP BACK AND DECREASE BIN SIZE')
                  ENDIF
                ELSE
                  DERIV_CONV=.TRUE.
                ENDIF
              ENDIF

C-----------------------------------------------------------------------
C loop back to continue calculation of derivative
C-----------------------------------------------------------------------

              IF(.NOT.DERIV_CONV)THEN
                GO TO 621
              ENDIF

C-----------------------------------------------------------------------
C end of condition that pass is allowed
C-----------------------------------------------------------------------

            ENDIF
            
C-----------------------------------------------------------------------
C calculate the matrix element for this point
C-----------------------------------------------------------------------

            IF(DUMP_LOCAL)THEN
              PRINT 8818,VAR1,VAR2
 8818         FORMAT(' save matrix elements for matrix point:',I2,I2 )
            ENDIF
            
            IF(DUMP_LOCAL)THEN
              PRINT 8836
 8836         FORMAT(' WAITING FOR ACKNOWLEDGE')
#if defined(CLEO_DPPDIAG)
              IF(KILLA.NE.1)THEN 
                CALL DSF_RFV_INPUT(YN)
                IF(YN.EQ.'K')KILLA=1
              ENDIF
#else
              READ 8819,YN
#endif
            ENDIF

            MATSQR(VAR1,VAR2)= .5 *DIRIV2(1)

            IF(VAR1.EQ.VAR2)THEN
              MATLIN(VAR1)      =-.5 *DIRIV1(1)
            ELSE
              MATSQR(VAR2,VAR1)=MATSQR(VAR1,VAR2)
            ENDIF
            
C-----------------------------------------------------------------------
C end of double loop over the variables
C-----------------------------------------------------------------------
            
            GO TO 612
          ENDIF
          GO TO 611
        ENDIF
        
C-----------------------------------------------------------------------
C print out the matrix
C-----------------------------------------------------------------------
        
        IF(DUMP_LOCAL)THEN
          PRINT 8831,
     2         ((MATSQR(VAR1,VAR2),VAR2=1,4),
     3         VAR1,
     4         MATLIN(VAR1),VAR1=1,4),
     5         MVAR,NVARF
          PRINT 8837
 8837     FORMAT(' WAITING FOR ACKNOWLEDGE')
#if defined(CLEO_DPPDIAG)
          IF(KILLA.NE.1)THEN 
            CALL DSF_RFV_INPUT(YN)
            IF(YN.EQ.'K')KILLA=1
          ENDIF
#else
          READ 8819,YN
#endif
        ENDIF

C-----------------------------------------------------------------------
C invert matrix and stuff in error matrix and EXIT (if MINIMIZE=.FALSE.)
C-----------------------------------------------------------------------

C ADF 1-29-96 Add error-matrix escape route; convert MATSQR to real*8
        IF(.NOT.MINIMIZE)THEN

          DO 702 VAR0=1,MVAR
            DO 701 VAR1=1,MVAR
              MAT8(VAR0,VAR1)=MATSQR(VAR0,VAR1)
 701        CONTINUE
 702      CONTINUE

          CALL INVERS(MAT8,ERRMTX,MVAR,NVARF,DET8,IERRF)
          
          IF(IERRF.EQ.0)THEN
            IF(DUMP_LOCAL)THEN
              PRINT 8842,DET8
 8842         FORMAT(' PARXTR: return from INVERS, DET=',E11.3)
            ENDIF

          ELSE
            ERR_RET=400+IERRF
            IF(DUMP_LOCAL)THEN
              PRINT 8841,IERRF
 8841         FORMAT(' PARXTR: INVERS RETURNED ERROR=',I3)
            ENDIF
          ENDIF
      
      

C-----------------------------------------------------------------------
C invert matrix and multiply  (if MINIMIZE=.TRUE.)
C-----------------------------------------------------------------------
        ELSE
          CALL INVMLT(MATSQR,MATLIN,GRADMI,MVAR,NVARF,IERRF)
          
          IF(DUMP_LOCAL)THEN
            PRINT 8832,IERRF
          ENDIF

          IF(IERRF.NE.0)THEN
            ERR_RET=500+IERRF
            GO TO 902
          ENDIF

          IF(DUMP_LOCAL)THEN
            PRINT 8833,
     2           ((MATSQR(VAR1,VAR2),VAR2=1,4),
     3           MATLIN(VAR1),
     4           GRADMI(VAR1),VAR1=1,4)
          ENDIF

 8831     FORMAT(' ready for matrix inversion..............'/
     2         '{',9X,30X,     '}   {', 5X,  '}   {',10X,  '}'/
     3         '{',E9.2,3E10.2,'}   { gr',I2,'}   {',E10.3,'}'/
     2         '{',9X,30X,     '}   {', 5X,  '}   {',10X,  '}'/
     3         '{',E9.2,3E10.2,'}   { gr',I2,'}   {',E10.3,'}'/
     4         '{',9X,30X,     '} X {', 5X,  '} = {',10X,  '}'/
     3         '{',E9.2,3E10.2,'}   { gr',I2,'}   {',E10.3,'}'/
     2         '{',9X,30X,     '}   {', 5X,  '}   {',10X,  '}'/
     3         '{',E9.2,3E10.2,'}   { gr',I2,'}   {',E10.3,'}'/
     1         '{',9X,30X,     '}   {', 5X,  '}   {',10X,  '}'/
     9         '   MVAR,NVARF=',2I10)
 8832     FORMAT(' back from matrix inversion, IERRF=',I4)
 8833     FORMAT(' back from matrix inversion..............'/
     2         '{',9X,30X,     '}   {',10X,  '}   {',10X,  '}'/
     3         '{',E9.2,3E10.2,'}   {',E10.3,'}   {',F10.6,'}'/
     2         '{',9X,30X,     '}   {',10X,  '}   {',10X,  '}'/
     3         '{',E9.2,3E10.2,'}   {',E10.3,'}   {',F10.6,'}'/
     4         '{',9X,30X,     '} X {',10X,  '} = {',10X,  '}'/
     3         '{',E9.2,3E10.2,'}   {',E10.3,'}   {',F10.6,'}'/
     2         '{',9X,30X,     '}   {',10X,  '}   {',10X,  '}'/
     3         '{',E9.2,3E10.2,'}   {',E10.3,'}   {',F10.6,'}'/
     1         '{',9X,30X,     '}   {',10X,  '}   {',10X,  '}')
          

C-----------------------------------------------------------------------
C have a new gradiant;  test for crazy gradiant
C-----------------------------------------------------------------------

          IF(DUMP_LOCAL)THEN
            PRINT 8829
          ENDIF
 8829     FORMAT(' Required (new gradiant)/(graularity)',
     2         ' less than 1.E5')

          DO 750 VAR1=1,NVARF
            IF( ABS(GRADMI(VAR1)/BINSZI(VAR1)) .GT. 1.E+5)THEN
              ERR_RET=550
              GO TO 902
            ENDIF
 750      CONTINUE

          L_GRAD_C_D=.FALSE.
          CALL UCOPY(GRADMI,GRADNT,NVARF)
          
 803      CONTINUE

          IF(DUMP_LOCAL)THEN
            PRINT 8820,CH2BST,(NMVARF(VAR0),VAR0=1,NVARF)
            PRINT 8821,       (VARBST(VAR0),VAR0=1,NVARF)
            PRINT 8822,       (GRADNT(VAR0),VAR0=1,NVARF)
 8820       FORMAT(' CH2BST=',F13.6,' new gradiant',10(A4,','))
 8821       FORMAT(40X,'    (point)       =',10F10.6)
 8822       FORMAT(40X,'        gradiant is',10F10.6)
          ENDIF

          ATTEMPT1=.TRUE.
          HAVDIR=.TRUE.
          TSTDIR=0
          MOVDIR=1.
          STEP_ALLOW=.TRUE.

C-----------------------------------------------------------------------
C walk in the direction of the gradiant
C-----------------------------------------------------------------------
          
          STEP_NUM=0
 811      CONTINUE
          STEP_NUM=STEP_NUM+1
          IF(STEP_NUM.LE.MAX_STEP)THEN

            IF(STEP_ALLOW)THEN

              DO 825 VAR0=1,NVARF
                VARNOW(VAR0)=VARBST(VAR0) +MOVDIR* GRADNT(VAR0)
 825          CONTINUE
              
C-----------------------------------------------------------------------
C calculate CHI SQUARED
C-----------------------------------------------------------------------
              
              CALL DEFCH2(CH2NOW,IERRF,DUMP_IN_CH2,LEXTRA)
              
              IF(IERRF.NE.0)THEN
                ERR_RET=600+IERRF
                GO TO 902
              ENDIF
              
              IF(DUMP_LOCAL)THEN
                IF(CH2NOW.LT.CH2BST)THEN
                  NOTE1=' -----> '
                  NOTE2='new low '
                ELSE
                  NOTE1='        '
                  NOTE2=' tested '
                ENDIF
                PRINT 8823,NOTE1,CH2NOW,NOTE2,
     2               (NMVARF(VAR0),VAR0=1,NVARF)
 8823           FORMAT(A8,F13.6,5X,A8,
     2               'CHI2 at ',10(A4,','))
                PRINT 8824,(VARNOW(VAR0),VAR0=1,NVARF)
 8824           FORMAT(40X,
     2               '(current point)   =',10F10.6)
                PRINT 8825,ATTEMPT1,HAVDIR,TSTDIR,MOVDIR
 8825           FORMAT(72X,
     2               '   1st=',L1,'  hd=',L1,'  td=',I1,'  mov=',F6.3 )
              ENDIF
              
C-----------------------------------------------------------------------
C test for new best values
C-----------------------------------------------------------------------
              
              IF(CH2NOW.LT.CH2BST)THEN
                CH2BST=CH2NOW
                CALL UCOPY(VARNOW,VARBST,NVARF)
                IF(NVARX.GT.0)CALL UCOPY(VARAXN,VARAXB,NVARX)
                
                HAVDIR=.TRUE.
                ATTEMPT1=.FALSE.
                TSTDIR=0
                
C-----------------------------------------------------------------------
C no new minimum found, 
C direction was not established and have not yet tested both directions
C  must test the other direction
C  if no new minimum found in either direction, reduce the bin size, repeat
C-----------------------------------------------------------------------
                
              ELSEIF(
     1               (.NOT.HAVDIR).AND.
     2               (TSTDIR.LT.2))THEN
                TSTDIR=TSTDIR+1
                MOVDIR=-MOVDIR
                
C-----------------------------------------------------------------------
C no new minimum found, 
C direction had been established, or both directions exhausted
C  in this case, reduce the grid size,
C  if this is the first attempt then continue in original direction,
C  on later attempts, test both directions
C-----------------------------------------------------------------------
                
              ELSEIF(ABS(MOVDIR).GT. .07 )THEN
                MOVDIR=.3 *MOVDIR
                
                IF(ATTEMPT1)THEN
                  HAVDIR=.TRUE.
                  ATTEMPT1=.FALSE.
                  TSTDIR=0
                  
                ELSE
                  HAVDIR=.FALSE.
                  ATTEMPT1=.FALSE.
                  TSTDIR=1
                  MOVDIR=-MOVDIR
                ENDIF
                
              ELSE
                STEP_ALLOW=.FALSE.
                
              ENDIF
              GO TO 811
              
C-----------------------------------------------------------------------
C another step along the current gradiant is not allowed,
C search in the direction of the gradiant is exhausted,
C get a new gradiant
C-----------------------------------------------------------------------
              
            ELSE
              IF(
     1             (.NOT.L_GRAD_C_D).AND.
     2             (CH2BCD.LT.CH2BST))THEN
                
                IF(DUMP_LOCAL)THEN
                  PRINT 8835,CH2BCD,CH2BST,CH2OLD
 8835             FORMAT(' overide with gradiant',
     2                 ' from calculating derivatives'/
     3                 ' CH2BCD=',F13.6,'  CH2BST=',F13.6,
     4                 '  CH2OLD=',F13.6)
                ENDIF
                
                L_GRAD_C_D=.TRUE.
                CH2BST=CH2OLD
                CALL UCOPY(GRAD_CAL_D,GRADNT,NVARF)
                CALL UCOPY(VSTART,VARBST,NVARF)
                MAX_ITER=MAX_ITER+1
                GO TO 803
              ENDIF
              
              IF(DUMP_LOCAL)THEN
                PRINT 8838
 8838           FORMAT(' WAITING FOR ACKNOWLEDGE')
#if defined(CLEO_DPPDIAG)
                IF(KILLA.NE.1)THEN 
                  CALL DSF_RFV_INPUT(YN)
                  IF(YN.EQ.'K')KILLA=1
                ENDIF
#else
                READ 8819,YN
#endif
              ENDIF

              GO TO 111
              
C-----------------------------------------------------------------------
C end of condition that step is allowed, or look for a new gradiant   
C-----------------------------------------------------------------------
            ENDIF
            
C-----------------------------------------------------------------------
C end of condition that number of steps is still within limit  
C-----------------------------------------------------------------------
          ENDIF

C-----------------------------------------------------------------------
C end of MINIMIZE condition   
C-----------------------------------------------------------------------
        ENDIF

C-----------------------------------------------------------------------
C end of condition that iteration is allowed
C-----------------------------------------------------------------------
      ENDIF
      
C-----------------------------------------------------------------------
C finish up after minimizing
C-----------------------------------------------------------------------

      IF(MINIMIZE)THEN
        CH2NOW = CH2BST
        CALL UCOPY(VARBST,VARNOW,NVARF)
        IF(NVARX.GT.0)
     1       CALL UCOPY(VARAXB,VARAXN,NVARX)
        
        IF(DUMP_LOCAL)THEN
          NOTE1=' DONE   '
          NOTE2='        '
          PRINT 8823,NOTE1,CH2NOW,NOTE2,
     2         (NMVARF(VAR0),VAR0=1,NVARF)
          PRINT 8824,         (VARNOW(VAR0),VAR0=1,NVARF)
          
          NOTE1='        '
          PRINT 8826,         (NMVARX(VAR0),VAR0=1,NVARX)
 8826     FORMAT(22X,'auxiliary variables ',10(A4,','))
          PRINT 8827,         (VARAXN(VAR0),VAR0=1,NVARX)
 8827     FORMAT(40X,'(current point)   =',10F10.6)
          PRINT 8828,         (VARAXN(VAR0),VAR0=1,NVARX)
 8828     FORMAT(40X,'(current point)   =',10E10.3)

          PRINT 8839
 8839     FORMAT(' WAITING FOR ACKNOWLEDGE')
#if defined(CLEO_DPPDIAG)
          IF(KILLA.NE.1)THEN 
            CALL DSF_RFV_INPUT(YN)
            IF(YN.EQ.'K')KILLA=1
          ENDIF
#else
          READ 8819,YN
#endif
        ENDIF
      ENDIF

      RETURN

 902  CONTINUE
      CH2NOW=9999.
      RETURN
      END












