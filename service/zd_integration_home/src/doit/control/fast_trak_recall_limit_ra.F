* $Id: fast_trak_recall_limit_ra.F,v 1.4 2002/09/24 22:54:34 dpp Exp $
*
* $Log: fast_trak_recall_limit_ra.F,v $
* Revision 1.4  2002/09/24 22:54:34  dpp
*      -> new control constant names for existing do contraint control
*
* Revision 1.3  2002/09/24 00:28:21  dpp
*      -> set the "test" and "solution" bits
*          in the CS representation, before fast_trak to try
*          to get them to appear during FAST_TRAK
*
* Revision 1.2  2002/09/05 19:36:48  dpp
*      -> add _check_hits_layers argument
*         to indicate that D0 is probably established
*
* Revision 1.1  2002/05/23 14:18:27  dpp
*      -> NEW ROUTINE
*      -> recall FAST_TRAK with limited radius to search for a
*         better representation before a kink
*
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"

      SUBROUTINE FAST_TRAK_RECALL_LIMIT_RA(WHAT,EXIT_STATUS)
C.......................................................................
C.
C. FAST_TRAK_RECALL_LIMIT_RA - loop to recall FAST_TRAK with 
C.                             limited radius
C.
C. COMMON    : 
C. CALLS     :
C. CALLED    : 
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.00
C. CREATED   : 06-03-2002
C. LAST MOD  : 
C.
C.......................................................................

#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/controlpar.inc"
#include "doit/duseq/statusbits.inc"
#include "doit/duseq/fast_trak.inc"

C-----------------------------------------------------------------------
C argument
C-----------------------------------------------------------------------
C WHAT............distinguish between the calls
                       INTEGER WHAT
C EXIT_STATUS.....=.TRUE. if recall was successful and exiting with CF hits
                       LOGICAL EXIT_STATUS 
C-----------------------------------------------------------------------
C indices
C-----------------------------------------------------------------------

C STAK_LOC.............stack location index returned from TFPOPF
                       INTEGER STAK_LOC
C INDX_RAD_LIM.........index to lop over the values of radius limit
                       INTEGER INDX_RAD_LIM 

C-----------------------------------------------------------------------
C temp values
C-----------------------------------------------------------------------

C KAP.............track curvature
                       REAL KAP
C D0..............track r-phi impact parameter
                       REAL D0
C RAD_CURL........curl over radius
                       REAL RAD_CURL
C TFXFIT_ERR......error returned from TFXFIT
                       INTEGER TFXFIT_ERR
C D_S_ERR.........eror returned form DEL_STRAGLER
                       INTEGER D_S_ERR
C PASS_D0_CRIT....logical, =.TRUE. if D0 criteria is passed 
C                 argument from FAST_TRAK_CHECK_HIT_LAYRS
                       LOGICAL PASS_D0_CRIT 
C PASS_Z_CRIT.....logical, =.TRUE. if Z criteria is passed 
C                 argument from FAST_TRAK_CHECK_HIT_LAYRS
                       LOGICAL PASS_Z_CRIT 
C MAX_ITER........maximum number of iterations
                       INTEGER MAX_ITER
C CHISQ...........returned chisq value from CF fit
                       REAL CHISQ
C NFIT............number of hits on track
                       INTEGER NFIT
C NFITS...........NFIT saved
                       INTEGER NFITS
C NLFDRA..........number of axial hits on track
                       INTEGER NLFDRA

C-----------------------------------------------------------------------
C  Executable code starts HERE
C-----------------------------------------------------------------------

      MAX_ITER=6
      EXIT_STATUS=.FALSE.
      CALL FIT_ACCESS(FIT_ACCESS_NLFDRA,0,NLFDRA)

C-----------------------------------------------------------------------

#if defined(CLEO_FTDIAG)
      PRINT 1003
      PRINT 1011,NLFDRA
 1003 FORMAT(
     1' --------------------------------------------------------------')
 1011 FORMAT(' FAST_TRAK_RECALL_LIMIT_RA, enter with NLFDRA=',I3)
#endif

C-----------------------------------------------------------------------
C FAST_TRAK requires use of STAK_PRIMARY and STAK_SECONDARY
C Save the original representation in STAK_TMP1.
C Save the "test" bit in the original representation
C /TFTRAK/ still contains the original representation with the test bit set
C-----------------------------------------------------------------------

      CALL STATUSBITS(kSTfillcf,kstFTlimitrTEST,1)
      CALL TFSTKC(STAK_TMP1)
      CALL TFPUSH(STAK_TMP1,STAK_LOC)

C-----------------------------------------------------------------------
C the representation coming from DUFIND
C properly preceded by a CF fit from FAST_TRAK
C because the representation had not been updated after the Z fit
C also, the new representations from FAST_TRAK end with a CF fit 
C-----------------------------------------------------------------------
      
C-----------------------------------------------------------------------

      IF(NLFDRA.GE.3)THEN

C-----------------------------------------------------------------------
#if defined(CLEO_FTDIAG)
        PRINT 1003
        PRINT 1012
 1012   FORMAT(' FAST_TRAK_RECALL_LIMIT_RA',
     2       '  start loop over radius limit')
#if defined(CLEO_XTSUBS)
        CALL XTS_TRAK_INTERRUPT_VALUE('1')
        CALL XTS_TRAK_INTERRUPT_THRESH('1')
        CALL XTS_TRAK('CF',0,'FAST_TRAK_RECALL_LIMIT_RA')
#endif
#endif
C-----------------------------------------------------------------------

        CALL FIT_ACCESS(FIT_ACCESS_KAPCF, 0,KAP)
        CALL FIT_ACCESS(FIT_ACCESS_D0CF,  0,D0)
        IF(ABS(KAP).GT. 0.1)THEN
          RAD_CURL=ABS(D0+1./KAP)
        ELSE
          RAD_CURL=9.99
        ENDIF

C-----------------------------------------------------------------------
C Save the "test" and "solution" bits in the CS representation
C it will be transfered into the working solution in FAST_TRAK
C-----------------------------------------------------------------------

        CALL STATUSBITS(kSTfillcs,kstFTlimitrTEST,1)
        CALL STATUSBITS(kSTfillcs,kstFTlimitrSOL,1)

C-----------------------------------------------------------------------
C start the loop over radius limit
C-----------------------------------------------------------------------

        RADIUS_LIMIT=.804+.014
        INDX_RAD_LIM=0
 423    INDX_RAD_LIM=INDX_RAD_LIM+1
        IF(INDX_RAD_LIM.LE.MAX_ITER)THEN
          RADIUS_LIMIT=RADIUS_LIMIT-.056
          IF(RADIUS_LIMIT.LT.(RAD_CURL-.028))THEN
            
#if defined(CLEO_FTDIAG)
            PRINT 1003
            PRINT 1014,INDX_RAD_LIM,RADIUS_LIMIT
 1014       FORMAT(' FAST_TRAK_RECALL_LIMIT_RA',
     2           ' iteration=',I2,'  radius limit=',F9.3)
#if defined(CLEO_XTSUBS)
            CALL XTS_TRAK_INTERRUPT_VALUE('1')
#endif
#endif

            CALL FAST_TRAK
            CALL FIT_ACCESS(FIT_ACCESS_NFIT,  0,NFIT)
            CALL FIT_ACCESS(FIT_ACCESS_CHICF, 0,CHISQ)

            IF(
     1           (CHISQ.LT.10.).AND.
     2           (NFIT.GE.16))
     3           EXIT_STATUS=.TRUE.
              

#if defined(CLEO_FTDIAG)
            PRINT 1003
            PRINT 1015,INDX_RAD_LIM,RADIUS_LIMIT,CHISQ,NFIT,EXIT_STATUS
 1015       FORMAT(' FAST_TRAK_RECALL_LIMIT_RA',
     2           ' iteration=',I2,'  radius limit=',F9.3,
     3           '  chisq=',F9.3,'   NFIT=',I3,
     3           '  exit status=',L1)
#if defined(CLEO_XTSUBS)
            CALL XTS_TRAK_INTERRUPT_VALUE('3')
            CALL XTS_TRAK('CF',0,'FAST_TRAK_RECALL_LIMIT_RA')
#endif
#endif

          ENDIF
          IF(.NOT.EXIT_STATUS)GO TO 423
        ENDIF

C-----------------------------------------------------------------------
C at end, retore the radius limit values
C restore the status bits in the CS representation
C-----------------------------------------------------------------------
         
        RADIUS_LIMIT=9.99
        CALL FIT_ACCESS(FIT_ACCESS_RPRLIM_SET,0,RADIUS_LIMIT)
         
        CALL STATUSBITS(kSTfillcs,kstFTlimitrTEST,0)
        CALL STATUSBITS(kSTfillcs,kstFTlimitrSOL,0)

C-----------------------------------------------------------------------
C if good solution found,
C set the SELECT bit which will be saved if the track passes all the way 
C use TFXFIT to fill in other hits
C use FAST_TRAK_CHECK_HIT_LAYRS to identify the stragler radius
C delete the straglers , requiring a minimum returned hits
C NOTE: first call to TFXFIT uses full radius range
C NOTE: FAST_TRAK_CHECK_HIT_LAYRS recalulates RSTRAG
C NOTE: DEL_STRAGLER sets RPRLIM to RSATRG for future calls to TFXFIT
C-----------------------------------------------------------------------

        IF(EXIT_STATUS)THEN
          CALL STATUSBITS(kSTfillcf,kstFTlimitrSEL,1)
          
#if defined(CLEO_FTDIAG)
          PRINT 1003
          PRINT 1021
 1021     FORMAT(' FAST_TRAK_RECALL_LIMIT_RA',
     2         ' will process solution with TFXFIT') 
          PRINT 1003
#endif
          CALL ROADHITS(LVLTYP_TFXFIT,HITTYP_HIT,FITTYP_CF)
          CALL TFXFIT(TFXF_NORMAL_STAND,TFXFIT_ERR)

#if defined(CLEO_FTDIAG)
          PRINT 1003
          PRINT 1022
 1022     FORMAT(' FAST_TRAK_RECALL_LIMIT_RA',
     2         ' will CHECK_HIT_LAYERS and DEL_STRAGLER') 
#endif
          CALL FAST_TRAK_CHECK_HIT_LAYRS(
     1         FT_D0CON_OFF,FT_SET_Z0BIAS,
     2         PASS_D0_CRIT,PASS_Z_CRIT)
          
          CALL FIT_ACCESS(FIT_ACCESS_NFIT,  0,NFITS)
          CALL DEL_STRAGLER(16,D_S_ERR)

#if defined(CLEO_FTDIAG)
          PRINT 1003
          PRINT 1023,D_S_ERR
 1023     FORMAT(' FAST_TRAK_RECALL_LIMIT_RA',
     2         ' after DEL_STRAGLER, err=',I3) 
#endif
          
          IF(D_S_ERR.EQ.0)THEN
            CALL FIT_ACCESS(FIT_ACCESS_NFIT,  0,NFIT)
            
#if defined(CLEO_FTDIAG)
            PRINT 1003
            PRINT 1025,NFIT,NFITS
 1025       FORMAT(' FAST_TRAK_RECALL_LIMIT_RA',
     2           ' after successful DEL_STRAGLER, NFIT=',I3,
     3           '  saved=',I3) 
#endif

            IF(NFIT.LT.NFITS)THEN
              
#if defined(CLEO_FTDIAG)
              PRINT 1003
              PRINT 1027
 1027         FORMAT(' FAST_TRAK_RECALL_LIMIT_RA',
     2             ' refit r-phi and Z',
     3             '  because straglers were deleted') 
              PRINT 1003
#endif

              CALL ROADHITS(LVLTYP_TFXFIT,HITTYP_HIT,FITTYP_CF)
              CALL FIT_WHATEVER
              CALL ROADHITS(LVLTYP_TFXFIT,HITTYP_HIT,FITTYP_LF)
              CALL FIT_WHATEVER
              CALL ROADHITS(LVLTYP_TFXFIT,HITTYP_HIT,FITTYP_CF)
            ENDIF

          ELSE
            EXIT_STATUS=.FALSE.
          ENDIF
        ENDIF

C store all results in STAK_PRIMARY
        CALL TFSTKC(STAK_PRIMARY)

C save the new represention, if there is one, in the first location  
        IF(EXIT_STATUS)CALL TFPUSH(STAK_PRIMARY,STAK_LOC)

C restore the original input representation to /TFTRAK/ 
C set bit in the original representation indicating that a solution was found 
C save the original input representation after in new representation 
        CALL TFPOPF(STAK_TMP1,STAK_PICK_FIRST,0,STAK_LOC)
        IF(EXIT_STATUS)CALL STATUSBITS(kSTfillcf,kstFTlimitrSOL,1)
        CALL TFPUSH(STAK_PRIMARY,STAK_LOC)

C-----------------------------------------------------------------------
      ENDIF

#if defined(CLEO_FTDIAG)
      PRINT 1003
      PRINT 1031,EXIT_STATUS
 1031 FORMAT(' FAST_TRAK_RECALL_LIMIT_RA',
     2     ' EXITING with EXIT_STATUS=',L1) 
      PRINT 1003
#endif

C-----------------------------------------------------------------------
      RETURN
      END









