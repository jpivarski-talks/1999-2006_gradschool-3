*
* $Id: parxtr.F,v 1.4 2002/06/06 18:37:35 atems Exp $
*
* $Log: parxtr.F,v $
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
      SUBROUTINE PARXTR(DUMP_LOCAL,DUMP_IN_CH2,LMNMZ,ERR_RET)
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
C. 1-29-96   : Added LMNMZ: calculating error matrix only if FALSE
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
C LMNMZ.........true: minimize here; false: return error matrix
C ERR_RET.......error rerurn=0 for no error

      LOGICAL DUMP_LOCAL,DUMP_IN_CH2,LMNMZ
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

C LEXTRA..extra logical defined for DEFCH2
C IERRF ..integer error flag used in various calls
C NEWLOW..indicates that a new low chi-squared has been found
C IVAR0 ..general index
C IVAR1 ..index used to count the matrix row
C IVAR2 ..index used to count the matrix column
C IPNT1 ..index used to indicate point in variable space for calculated deriv
C IPNT2 ..index used to indicate point in variable space for calculated deriv
C ITERAT..iteration number
C MITERA..maximum iteration number
C IPASS ..pass number in testing for convergence of derivatives
C HAVDIR..inticates that the direction to walk in chi-squared is known
C TSTDIR..countes the number of signs of the direction to walk that are tested
C BINSZI..starting bin size, or granularity, for an iteration
C BINSZN..current bin size, or granularity, for an iteration
C BSZL1L..limit for starting the bin size for an iteration,local
C BSZL2L..limit of reduction of bin size when attempting to
C         look for convergence of the derivatives,local
C CH2OLD..chi-squared from previous iteration
C CH2BST..best chi-squared found
C CH2TST..chi-squared at the 4 points for determining the derivatives
C CH2BCD..Best chi-squared found while Calculating Derivatives
C DENOM1..denominator for the first order derivative
C DENOM2..denominator for the second order derivative
C DRV2SM..absolute value of sum of the derivative from this and earlier pass
C DRVASM..sum of absolute value of the derivative from this and earlier pass
C DRV2DF..absolute value of diff of the derivative from this and earlier pass
C MOVDIR..signed multiplier of the gradiant
C GRADNT..gradiant; amount to change variable to reach apparent minimum
C GRADMI..gradiant: found from matrix inversion
C GRADCD..gradiant: found while calculating derivatives
C LGRDCD..logical: use of the GRADCD gradiant, 0:NOT YET, 1:IN USE, 2:WAS USED
C VSTART..values of user variables at the start of an iteration
C VARBST..current best found values of the user variables
C VARAXB..current values of the auxiliary values corresponding to the
C         best found values of the user variables
C NOTE1 ..character string for diagnostic purposes
C NOTE2 ..character string for diagnostic purposes
C MATSQR..square matrix used in fitting
C         corresponds to alpha of equation 11-13, page 224 of Bevington
C         sent to the matrix inverter to be substituted with the error matrix
C MATLIN..column matrix used in fitting
C         corresponds to beta of equation 11-13, page 224 of Bevington
C MAT8....REAL*8 version of MATSQR for inversion
C DET8....determinant from INVERS

      LOGICAL LEXTRA
      INTEGER IERRF
      INTEGER NEWLOW
      INTEGER IVAR0,IVAR1,IVAR2,IPNT1,IPNT2,ITERAT,MITERA,IPASS
      INTEGER HAVDIR,TSTDIR
      REAL BINSZI(MVAR),BINSZN(MVAR),BSZL1L(MVAR),BSZL2L(MVAR)
      REAL CH2OLD,CH2BST,CH2BCD,CH2TST(0:1,0:1)
      REAL DENOM1,DENOM2,DIFF1,DIFF2,DIFF1I,DIFF2I
      REAL DIRIV1(2),DIRIV2(2),DRVASM,DRV2SM,DRV2DF
      REAL MOVDIR
      REAL GRADNT(MVAR),GRADMI(MVAR),GRADCD(MVAR)
      LOGICAL LGRDCD
      REAL VSTART(MVAR),VARBST(MVAR),VARAXB(MVARX)
      CHARACTER*8 NOTE1,NOTE2
      REAL MATSQR(MVAR,MVAR),MATLIN(MVAR)
      REAL*8 MAT8(MVAR,MVAR)
      REAL*8 DET8
      INTEGER YN,KILLA

      DATA KILLA /0/

C***********************************************************************
C executable code starts here
C***********************************************************************
      ERR_RET=0
      LEXTRA=.FALSE.
      MITERA=2
      ITERAT=0
 100  ITERAT=ITERAT+1

C-----------------------------------------------------------------------
C initialize the first iterations, protect against negative values
C-----------------------------------------------------------------------
      IF(ITERAT.EQ.1)THEN
         CH2BST=CH2NOW
         CH2BCD=CH2NOW
         CALL UCOPY(VARNOW,VARBST,NVARF)

         DO 101 IVAR1=1,NVARF
            BINSZI(IVAR1)=ABS(BINSZS(IVAR1))
            BSZL1L(IVAR1)=ABS(BNSZL1(IVAR1))
 101        BSZL2L(IVAR1)=ABS(BNSZL2(IVAR1))

C-----------------------------------------------------------------------
C test if another iteration is allowed
C-----------------------------------------------------------------------
      ELSE
         IF(ITERAT.GT.MITERA)                   GO TO 901
         IF(ITERAT.GT.4)                        GO TO 901
         CALL UCOPY(BINSZN,BINSZI,NVARF)
         DO 102 IVAR0=1,NVARF
 102        IF( BINSZN(IVAR0).LT.BSZL1L(IVAR0) )GO TO 901
         IF(CH2BST.GT.CH2OLD-0.00001)           GO TO 901
      ENDIF

C-----------------------------------------------------------------------
C reset values for the next iteration
C-----------------------------------------------------------------------
      CH2OLD=CH2BST
      CALL UCOPY(VARBST,VSTART,NVARF)

C-----------------------------------------------------------------------
C double loop over the variables
C this loop specifies a location in the square matrix
C-----------------------------------------------------------------------
      IVAR1=0
 601  IVAR1=IVAR1+1
      IF(IVAR1.GT.NVARF)GO TO 689

                IF(DUMP_LOCAL)THEN
                PRINT 8800,IVAR1
#if defined(CLEO_DPPDIAG)
                IF(KILLA.NE.1)THEN 
                   CALL DSF_RFV_INPUT(YN)
                   IF(YN.EQ.'K')KILLA=1
                ENDIF
#endif
                ENDIF
 8800 FORMAT(' PARXTR: outer loop over variable number',I2)

c      IVAR2=IVAR1-1
      IVAR2=0
 611  IVAR2=IVAR2+1
      IF(IVAR2.GT.NVARF)GO TO 679

C-----------------------------------------------------------------------
C start at the initial bin size
C-----------------------------------------------------------------------

      IPASS=1
      CALL UCOPY(BINSZI,BINSZN,NVARF)
      
                IF(DUMP_LOCAL)THEN
                PRINT 8801,IVAR1,IVAR2,(NMVARF(IVAR0),IVAR0=1,NVARF)
                PRINT 8802,            (BINSZN(IVAR0),IVAR0=1,NVARF)
                ENDIF
 8801 FORMAT(' matrix point:',I2,I2,
     2       ' start to calculate derivatives respect to '
     3       10(A4,','))
 8802 FORMAT(45X,'(granularity)  ',10F10.6)

      GO TO 631

C-----------------------------------------------------------------------
C swap the results of the previous bin size into the second position
C reduce the bin size for the next iteration
C-----------------------------------------------------------------------

 621  IPASS=2
      DIRIV2(2)=DIRIV2(1)
      DIRIV1(2)=DIRIV1(1)

      DO 622 IVAR0=1,NVARF
 622     BINSZN(IVAR0)=BINSZN(IVAR0)/2.

C-----------------------------------------------------------------------
C accept the last value when the bin size gets too small
C-----------------------------------------------------------------------

      DO 623 IVAR0=1,NVARF
 623     IF( BINSZN(IVAR0).LT.BSZL2L(IVAR0) )GO TO 671

C-----------------------------------------------------------------------
C set the denominator of the derivatives for this bin size
C-----------------------------------------------------------------------

 631  DENOM1=1.
      DENOM2=1.
      DENOM1=DENOM1*BINSZN(IVAR1)
      DENOM2=DENOM2*BINSZN(IVAR1)
      DENOM2=DENOM2*BINSZN(IVAR2)

C-----------------------------------------------------------------------
C get the current CH2 at the 4 points for a particular bin size
C    for the particular set of 2 variables (WHICH MAY BE THE SAME VARIABLE)
C this loop specifies the points in chisquared space used to calculate
C    the second derivative
C-----------------------------------------------------------------------

                IF(DUMP_LOCAL)THEN
                PRINT 8803,IVAR1,IVAR2,(NMVARF(IVAR0),IVAR0=1,NVARF)
                PRINT 8804,            (BINSZN(IVAR0),IVAR0=1,NVARF)
                ENDIF
 8803 FORMAT(' matrix point:',I2,I2,
     2       '      now calculate derivatives respect to '
     3        10(A4,','))
 8804 FORMAT(45X,'(granularity)  ',10F10.6)

      DO 649 IPNT1=0,1
      DO 648 IPNT2=0,1

      CALL UCOPY(VSTART,VARNOW,NVARF)

      IF(IPNT1.EQ.1)VARNOW(IVAR1)=VARNOW(IVAR1) + BINSZN(IVAR1)
      IF(IPNT2.EQ.1)VARNOW(IVAR2)=VARNOW(IVAR2) + BINSZN(IVAR2)

      CALL DEFCH2(CH2TST(IPNT1,IPNT2),IERRF,DUMP_IN_CH2,LEXTRA)
      IF(IERRF.NE.0)THEN
         ERR_RET=100+IERRF
         GO TO 902
      ENDIF
                IF(DUMP_LOCAL)THEN
                PRINT 8805,IVAR1,IVAR2,IPNT1,IPNT2,
     2                                   (NMVARF(IVAR0),IVAR0=1,NVARF)
                PRINT 8806,              (VSTART(IVAR0),IVAR0=1,NVARF)
                PRINT 8807,              (VARNOW(IVAR0),IVAR0=1,NVARF)
                PRINT 8808,CH2TST(IPNT1,IPNT2)
                ENDIF
 8805 FORMAT(' '/' matrix point:',I2,I2,14X,'calculated point',I2,I2/
     2           ' values of ',10(A4,','))
 8806 FORMAT(' at the start...',10F10.6)
 8807 FORMAT('      and now-->',10F10.6)
 8808 FORMAT(48X,' chi-squared at point=',F13.6)

      IF(CH2TST(IPNT1,IPNT2).LT.CH2BCD)THEN

                IF(DUMP_LOCAL)THEN
                PRINT 8834
                ENDIF
 8834 FORMAT(48X,' this is a new best chi-squared') 

         CH2BCD=CH2TST(IPNT1,IPNT2)
         DO 645 IVAR0=1,NVARF
 645        GRADCD(IVAR0)=VARNOW(IVAR0)-VSTART(IVAR0)
      ENDIF


 648  CONTINUE
 649  CONTINUE

C-----------------------------------------------------------------------
C calculate the first and second order derivatives
C-----------------------------------------------------------------------

                IF(DUMP_LOCAL)THEN
                PRINT 8809,DENOM1,DENOM2,IPASS
                ENDIF
 8809 FORMAT(' will calculate derivatives: DENOM1/2=',2E11.3,
     2       ', ipass=',I2)

      DIFF1      =  CH2TST(1,0)-CH2TST(0,0)
      DIRIV1(1)  =  DIFF1/DENOM1
      DIFF2      =  CH2TST(1,1)-CH2TST(0,1)-DIFF1
      DIRIV2(1)  =  DIFF2/DENOM2

                IF(DUMP_LOCAL)THEN
                PRINT 8810,DIFF1,DENOM1,DIRIV1(1),
     2                  DIFF2,DENOM2,DIRIV2(1)
                ENDIF
 8810 FORMAT('  first derivative DIF=',F13.6,
     2       ' DENOM=',E11.3,' DIRIV=',E11.3/
     3       ' second derivative DIF=',F13.6,
     2       ' DENOM=',E11.3,' DIRIV=',E11.3)

C-----------------------------------------------------------------------
C calculate the optimum bin size from the first derivative
C-----------------------------------------------------------------------

      IF(IVAR1.EQ.IVAR2)THEN
         IF(DIRIV1(1).NE.0.)THEN
            DIFF1I=1./DIRIV1(1)

               IF(DUMP_LOCAL)THEN
                  PRINT 8811,DIFF1I
               ENDIF
 8811 FORMAT(' first derivative implies GRANULARITY=',F13.6)

         ELSE
            DIFF1I=BINSZN(IVAR1)

               IF(DUMP_LOCAL)THEN
                  PRINT 8812,DIFF1I,IVAR1
                  READ 8819,YN
               ENDIF
 8812 FORMAT(' first derivative =0,     GRANULARITY=',F13.6,
     2       ' <<<<< THIS IS A PROBLEM >>>>  IVAR1=',I2)
 8819 FORMAT(A1)            

         ENDIF
      ENDIF

C-----------------------------------------------------------------------
C look for convergence of the second derivative
C-----------------------------------------------------------------------

      IF(IPASS.EQ.1)GO TO 621
      DRVASM =.5*( ABS( DIRIV2(1) )  +  ABS( DIRIV2(2) ) )

                IF(DUMP_LOCAL)THEN
                PRINT 8813,DRVASM
                ENDIF
 8813 FORMAT(' REQUIRE DRVASM=',E11.3,' .GE. 1.0e-9')

      IF(  DRVASM         .LT. 1.E-9)THEN
         IF(DUMP_LOCAL)THEN
            PRINT 8816
         ENDIF
 8816    FORMAT(' ABORT EVENT')
         ERR_RET=200
         GO TO 902
      ENDIF
      
      DRV2SM =.5*( ABS( DIRIV2(1)  +  DIRIV2(2) ) )

                IF(DUMP_LOCAL)THEN
                PRINT 8814,DRV2SM,DRVASM
                ENDIF
 8814 FORMAT(' REQUIRE DRV2SM/DRVASM=',E11.3,'/',E11.3,' .GE. 1.0e-5')

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
 8815 FORMAT(' REQUIRE DRV2DF/DRV2SM=',E11.3,'/',E11.3,' .LT. 0.3')

      IF( (DRV2DF/DRV2SM) .GT.  0.3)THEN
                IF(DUMP_LOCAL)THEN
                PRINT 8817
                ENDIF
 8817 FORMAT(' LOOP BACK AND DECREASE BIN SIZE')
         GO TO 621
      ENDIF

 671  CONTINUE
                IF(DUMP_LOCAL)THEN
                PRINT 8818,IVAR1,IVAR2
                ENDIF
 8818 FORMAT(' save matrix elements for matrix point:',I2,I2 )

      MATSQR(IVAR1,IVAR2)= .5 *DIRIV2(1)
      IF(IVAR1.EQ.IVAR2)THEN
         MATLIN(IVAR1)      =-.5 *DIRIV1(1)
      ELSE
         MATSQR(IVAR2,IVAR1)=MATSQR(IVAR1,IVAR2)
      ENDIF

      GO TO 611
 679  CONTINUE
      GO TO 601
 689  CONTINUE

                IF(DUMP_LOCAL)THEN
                   PRINT 8831,
     2                  ((MATSQR(IVAR1,IVAR2),IVAR2=1,4),
     3                  IVAR1,
     4                  MATLIN(IVAR1),IVAR1=1,4),
     5                  MVAR,NVARF
                ENDIF

C-----------------------------------------------------------------------
C invert matrix and stuff in error matrix and EXIT (if LMNMZ=.FALSE.)
C-----------------------------------------------------------------------

C ADF 1-29-96 Add error-matrix escape route; convert MATSQR to real*8
      IF (LMNMZ) GO TO 705

      DO 701 IVAR0=1,MVAR
      DO 701 IVAR1=1,MVAR
 701     MAT8(IVAR0,IVAR1)=MATSQR(IVAR0,IVAR1)
      CALL INVERS(MAT8,ERRMTX,MVAR,NVARF,DET8,IERRF)
      IF(IERRF.EQ.0)THEN
         IF(DUMP_LOCAL)THEN
            PRINT 8842,DET8
 8842       FORMAT(' PARXTR: return from INVERS, DET=',E11.3)
         ENDIF
      ELSE
         ERR_RET=400+IERRF
         IF(DUMP_LOCAL)THEN
            PRINT 8841,IERRF
 8841       FORMAT(' PARXTR: INVERS RETURNED ERROR=',I3)
         ENDIF
      ENDIF
      GO TO 903

C-----------------------------------------------------------------------
C invert matrix and multiply  (if LMNMZ=.TRUE.)
C-----------------------------------------------------------------------

 705  CALL INVMLT(MATSQR,MATLIN,GRADMI,MVAR,NVARF,IERRF)
                IF(DUMP_LOCAL)THEN
                PRINT 8832,IERRF
                ENDIF
      IF(IERRF.NE.0)THEN
         ERR_RET=500+IERRF
         GO TO 902
      ENDIF
                IF(DUMP_LOCAL)THEN
                   PRINT 8833,
     2                  ((MATSQR(IVAR1,IVAR2),IVAR2=1,4),
     3                  MATLIN(IVAR1),
     4                  GRADMI(IVAR1),IVAR1=1,4)
                ENDIF

8831  FORMAT(' ready for matrix inversion..............'/
     2'{',9X,30X,     '}   {', 5X,  '}   {',10X,  '}'/
     3'{',E9.2,3E10.2,'}   { gr',I2,'}   {',E10.3,'}'/
     2'{',9X,30X,     '}   {', 5X,  '}   {',10X,  '}'/
     3'{',E9.2,3E10.2,'}   { gr',I2,'}   {',E10.3,'}'/
     4'{',9X,30X,     '} X {', 5X,  '} = {',10X,  '}'/
     3'{',E9.2,3E10.2,'}   { gr',I2,'}   {',E10.3,'}'/
     2'{',9X,30X,     '}   {', 5X,  '}   {',10X,  '}'/
     3'{',E9.2,3E10.2,'}   { gr',I2,'}   {',E10.3,'}'/
     1'{',9X,30X,     '}   {', 5X,  '}   {',10X,  '}'/
     9'   MVAR,NVARF=',2I10)
8832  FORMAT(' back from matrix inversion, IERRF=',I4)
8833  FORMAT(' back from matrix inversion..............'/
     2'{',9X,30X,     '}   {',10X,  '}   {',10X,  '}'/
     3'{',E9.2,3E10.2,'}   {',E10.3,'}   {',F10.6,'}'/
     2'{',9X,30X,     '}   {',10X,  '}   {',10X,  '}'/
     3'{',E9.2,3E10.2,'}   {',E10.3,'}   {',F10.6,'}'/
     4'{',9X,30X,     '} X {',10X,  '} = {',10X,  '}'/
     3'{',E9.2,3E10.2,'}   {',E10.3,'}   {',F10.6,'}'/
     2'{',9X,30X,     '}   {',10X,  '}   {',10X,  '}'/
     3'{',E9.2,3E10.2,'}   {',E10.3,'}   {',F10.6,'}'/
     1'{',9X,30X,     '}   {',10X,  '}   {',10X,  '}')


C-----------------------------------------------------------------------
C walk in the direction of the gradiant
C-----------------------------------------------------------------------

      LGRDCD=.FALSE.
      CALL UCOPY(GRADMI,GRADNT,NVARF)

 803  CONTINUE
                IF(DUMP_LOCAL)THEN
                PRINT 8820,CH2BST,(NMVARF(IVAR0),IVAR0=1,NVARF)
                PRINT 8821,       (VARBST(IVAR0),IVAR0=1,NVARF)
                PRINT 8822,       (GRADNT(IVAR0),IVAR0=1,NVARF)
                ENDIF
8820  FORMAT(' CH2BST=',F13.6,' new gradiant',10(A4,','))
8821  FORMAT(40X,'    (point)       =',10F10.6)
8822  FORMAT(40X,'        gradiant is',10F10.6)

      HAVDIR=2
      TSTDIR=0
      MOVDIR=1.
      GO TO 822

C-----------------------------------------------------------------------
C test if a new minimum was found
C-----------------------------------------------------------------------

 811  IF(NEWLOW.EQ.1)GO TO 821
      IF(HAVDIR.EQ.0)GO TO 813

C-----------------------------------------------------------------------
C no new minimum found, direction had been established
C  in this case, reduce the grid size,
C  if this is the first attempt then continue in original direction,
C  on later attempts, test both directions
C-----------------------------------------------------------------------

 812  IF(ABS(MOVDIR).LE. .07 )GO TO 814
      MOVDIR=.3 *MOVDIR
      IF(HAVDIR.EQ.2)GO TO 821

C-----------------------------------------------------------------------
C no new minimum found, direction was not established
C  must test both directions
C  if no new minimum found in either direction, reduce the bin size, repeat
C-----------------------------------------------------------------------

      HAVDIR=0
      TSTDIR=0
 813  IF(TSTDIR.EQ.2)GO TO 812
      TSTDIR=TSTDIR+1
      MOVDIR=-MOVDIR
      GO TO 822

C-----------------------------------------------------------------------
C search in the direction of the gradiant is exhausted, get a new gradiant
C-----------------------------------------------------------------------

 814  CONTINUE
      IF(.NOT.LGRDCD)THEN
         IF(CH2BCD.LT.CH2BST)THEN
                IF(DUMP_LOCAL)THEN
                PRINT 8835,CH2BCD,CH2BST,CH2OLD
 8835 FORMAT('  overide with gradiant from calculating derivatives'/
     2       ' CH2BCD=',F13.6,'  CH2BST=',F13.6,'  CH2OLD=',F13.6)
                ENDIF
            LGRDCD=.TRUE.
            CH2BST=CH2OLD
            CALL UCOPY(GRADCD,GRADNT,NVARF)
            CALL UCOPY(VSTART,VARBST,NVARF)
            MITERA=MITERA+1
            GO TO 803
         ENDIF
      ENDIF
                IF(DUMP_LOCAL)THEN
#if defined(CLEO_DPPDIAG)
                IF(KILLA.NE.1)THEN 
                   CALL DSF_RFV_INPUT(YN)
                   IF(YN.EQ.'K')KILLA=1
                ENDIF
#endif
                ENDIF

      GO TO 100

C-----------------------------------------------------------------------
C set variables to best of this iteration
C-----------------------------------------------------------------------

 821  HAVDIR=1
      TSTDIR=0

 822  NEWLOW=0
      DO 825 IVAR0=1,NVARF
 825     VARNOW(IVAR0)=VARBST(IVAR0) +MOVDIR* GRADNT(IVAR0)

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
     2                              (NMVARF(IVAR0),IVAR0=1,NVARF)
                PRINT 8824,         (VARNOW(IVAR0),IVAR0=1,NVARF)
                PRINT 8825,HAVDIR,TSTDIR,MOVDIR
                ENDIF
8823  FORMAT(A8,F13.6,5X,A8,'CHI2 at ',10(A4,','))
8824  FORMAT(40X,'(current point)   =',10F10.6)
8825  FORMAT(72X,'   hd=',I1,'  td=',I1,'  mov=',F6.3 )

C-----------------------------------------------------------------------
C find the best values of variables and loop back
C-----------------------------------------------------------------------


      IF(CH2NOW.GE.CH2BST)GO TO 811
      NEWLOW=1
      CH2BST=CH2NOW

      CALL UCOPY(VARNOW,VARBST,NVARF)

      IF(NVARX.GT.0)
     1CALL UCOPY(VARAXN,VARAXB,NVARX)

      GO TO 811

C-----------------------------------------------------------------------
C end of loop over iterations
C-----------------------------------------------------------------------

 901  CH2NOW = CH2BST
      CALL UCOPY(VARBST,VARNOW,NVARF)
      IF(NVARX.GT.0)
     1CALL UCOPY(VARAXB,VARAXN,NVARX)

                IF(DUMP_LOCAL)THEN
                NOTE1=' DONE   '
                NOTE2='        '
                PRINT 8823,NOTE1,CH2NOW,NOTE2,
     2                              (NMVARF(IVAR0),IVAR0=1,NVARF)
                PRINT 8824,         (VARNOW(IVAR0),IVAR0=1,NVARF)

                NOTE1='        '
                PRINT 8826,         (NMVARX(IVAR0),IVAR0=1,NVARX)
                PRINT 8827,         (VARAXN(IVAR0),IVAR0=1,NVARX)
                PRINT 8828,         (VARAXN(IVAR0),IVAR0=1,NVARX)
#if defined(CLEO_DPPDIAG)
                IF(KILLA.NE.1)THEN 
                   CALL DSF_RFV_INPUT(YN)
                   IF(YN.EQ.'K')KILLA=1
                ENDIF
#endif
                ENDIF
8826  FORMAT(22X,'auxiliary variables ',10(A4,','))
8827  FORMAT(40X,'(current point)   =',10F10.6)
8828  FORMAT(40X,'(current point)   =',10E10.3)

      GO TO 903

 902  CONTINUE
      CH2NOW=9999.

 903  RETURN
      END












