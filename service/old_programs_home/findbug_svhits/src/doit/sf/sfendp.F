*
* $Id: sfendp.F,v 1.4 2000/11/15 17:00:18 dpp Exp $
*
* $Log: sfendp.F,v $
* Revision 1.4  2000/11/15 17:00:18  dpp
*      -> "CDHITS" changed to "ROADHITS" to make way for MC infringement
*
* Revision 1.3  2000/10/02 20:51:34  dpp
*      -> use stak number parameter
*      -> clear out use of STATCF(2)
*
* Revision 1.2  1999/08/03 21:58:18  dpp
* calls to stack routines
*
* Revision 1.1.1.1  1998/02/06 19:11:46  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:41  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:00:43  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  6.00/07 31/08/93  14.06.11  by  Rob Kutschke
*CMZ :  5.54/29 17/11/92  18.35.32  by  Dan Peterson
*CMZ :  5.54/16 08/10/92  12.53.00  by  Rob Kutschke
*CMZ :  5.54/15 22/09/92  16.25.10  by  Dan Peterson
*CMZ :  3.01/01 23/03/90  17.31.01  by  Dan Peterson
*CMZ :          15/09/88  15.12.03  by  Unknown
*-- Author :    Dan Peterson
      SUBROUTINE SFENDP
C.......................................................................
C.
C. SFENDP - find a Z projextion from the endplate exits; and hits
C.
C. COMMON    : ?
C. CALLS     : TFXYTR, func ARCFUN,  TFUSTK
C. CALLED    : DUFIND
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.00
C. CREATED   : 1-Aug-88
C. LAST MOD  : 12-Oct-88
C.
C.
C. Modification Log.
C. 10-Mar-92 allow NZFMIN(NTROAD)=1 for last hit defined roads
C. 16-Oct-91 more cuts to avoid stopping tracks
C. 11-Sept-91 code for status bits; STATCF
C. 23-Mar-90 Compatable with CLEOII
C. 12-Oct-88 require more hits for longer tracks
C. 06-Oct-88 Changed from TFPHTF to TFXYTF, & handle TFXYTF error
C. 15-Sept-88 now creates median drift distance in place of zero
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/lfctlcde.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/tfhitscd.inc"
#include "doit/duseq/tftrakcd.inc"
#include "doit/duseq/fast_trak.inc"
#include "doit/duseq/escappar.inc"
C
C..LLFIND.......set to .TRUE. if a solution is found
C
C LOCAL PARAMETERS
C..RCMXEP......MAXIMUM OF R*CURVATURE TO USE ENDPLATE; NO CURLERS
C..RCMNHT......MINIMUM OF R*CURV TO USE HITS OF LAST LAYER
C..HITCUT......ALLOWED Z-MISS FOR HITS ON NORMAL ANGLE PATH
C..WTFO........WEIGHT IN FIT FOR ORIGIN
C..WTFE........WEIGHT IN FIT FOR EXIT POINT
C..RADADD......MAXIMUM ALLOWED EXTRA RADIUS BEYOND EXIT LAYER, FOR ROAD
C..DIFMAX......DIFFERENCE IN Z VALUE OF HITS TO DEFINE AS IDENTICAL
C..DELADD......SEPARATION ADDED TO IDENTICAL HITS
C..MTROAD......MAXIMUM NUMBER OF TRIAL ROADS TO SEARCH FOR HITS
C..LRESC1......SMALLEST LAYER OF ESCAPE OF MEDIUM LENGTH TRACKS
C..LRESC2......SMALLEST LAYER OF ESCAPE OF LONG LENGTH TRACKS
C..NFIND1......MINIMUM Z LAYERS FOUND FOR MEDIUM LENGTH TRACKS
C..NFIND2......MINIMUM Z LAYERS FOUND FOR LONG LENGTH TRACKS
      REAL RCMXEP,RCMNHT,HITCUT
      REAL WTFO,WTFE
      REAL RADADD,DIFMAX,DELADD
      INTEGER MTROAD
      INTEGER LRESC1,LRESC2,NFIND1,NFIND2
      PARAMETER (RCMXEP=.88,RCMNHT=.82)
      PARAMETER (HITCUT=.05)
      PARAMETER (WTFO=.25,WTFE=.5)
      PARAMETER (RADADD=.05)
      PARAMETER (DIFMAX=1.E-9, DELADD=.07)
      PARAMETER (MTROAD=6)
C 23-MAR-90; REQUIRE 2 Z HITS FOR TRACKS ESCAPING AT (DR)LAYER 15
C 23-MAR-90; REQUIRE 3 Z HITS FOR TRACKS ESCAPING AT (DR)LAYER 23
      PARAMETER (LRESC1=28,NFIND1=2)
      PARAMETER (LRESC2=34,NFIND2=3)
C
C LOCAL VARIABLES
C..STATTM......TEMPORARY STATUS BITS
C..STATRD......TEMPORARY STATUS BITS FOR EACH ROAD
C..ILLYR.......LAYER COUNTER OF LAYERS FOUND BY ROADHITS
C..ILYR........PHYSICAL LAYER NUMBER
C..IER.........ERROR RETURNED FROM TFXYTR
C..TRYDR.......TRIAL NUMBER TO FIND A VALID RADIUS OF ESCAPE
C..RESCVD.......RADIUS OF ESCAPE FROM VD
C..RESCDR.......RADIUS OF ESCAPE FROM DR
C..RSQESC......RADUIUS SQUARED OF ESCAPE FROM DR
C..RKAPPA......ABS OF SGN OF ANGLE AT ESCAPE; TO IDENTIFY CURLERS
C..XESCAP......X   RETURN FROM TFXYTR
C..YESCAP......Y   RETURN FROM TFXYTR
C..FESCAP......PHI RETURN FROM TFXYTR
C..SESC........ARC LENGTH TO ESCAPE FOR 1:VD  AND   2:DR
      INTEGER STATTM,STATRD(MTROAD)
      INTEGER ILLYR,ILYR,IER,TRYDR
      REAL RESCVD,RESCDR,RSQESC,RKAPPA
      REAL XESCAP,YESCAP,FESCAP,SESC(2)
C..DET.........DETERMINENT OF LINE FIT, USED FOR INITIAL AND 2ND FIT
C..X0S,X1S,X2S,YX0S,YX1S...
C       .......MATRIX ELEMENTS FOR STRAIGHT LINE FIT
      REAL DET,X0S,X1S,X2S,YX0S,YX1S
C..NTROAD......NUMBER OF TRIAL ROADS TO SEARCH FOR HITS
C..XINTER......R=0  Z-INTERCEPT  OF ROAD
C..XSLOPE......Z/S SLOPE OF ROAD
C..XARCES......ARC LENGTH TO ESCAPE POINT, INCLUDE POINT IN FIT, IF >.001
C..XZESCA......Z OF ESCAPE POINT, INCLUDE THIS POINT IN FIT
C..WTFEU.......WEIGHT OF ESCAPE POINT USED IN THE FIT, SOMETIMES WTFE
C..IROAD.......ROAD COUNTER
C..IROADS......ROAD SELECTED TO BE SAVED
C..NZFIND......NUMBER OF HITS FOUND IN ROAD
C..NZFMIN......NUMBER OF HITS NEEDED IN A ROAD, FOR A VALID FIT
C..NZFMAX......NUMBER OF HIS FOUND IN THE BEST FIT
C..MAXLYR......LOGICAL LAYER NUMBER WITH MAXIMUM ALLOWED RADIUS
C..IHIT........HIT COUNTER OF HITS IN LAYER
C..IHITM.......MINIMUM HIT NUMBER IN LOOP TO SEARCH FOR DUPLICATES
C..IHITS.......HIT NUMBER OF SAVED BEST HIT
C..USECUT......SLOPE DEPENDENT CUT TO USE ON HITS
C..RESMIN......RESIDUAL OF BEST HIT FOUND IN LAYER
C..RESCAL......RESIDUAL OF CURRENT HIT FOUND IN LAYER
C..IPLEND......SAVED LAYER NUMBERS OF HITS FROM "ENDPLATE" ROAD
C..IADEND......SAVED ADDRESSES OF HITS
C..IHTEND......SAVED IHIT OF HITS
C..SSSEND......SAVED ARC LENGTH "S" TO HIT
C..Z0FIND......Z0 FROM FIT TO ORIGIN,ESCAPE, AND HITS ON ROAD
C..DZFIND......DZ/DS FROM FIT
C..RESSUM......SUM; THE AVERAGE; RESIDUAL OF HITS IN ROAD
      REAL XINTER(MTROAD),XSLOPE(MTROAD),XARCES(MTROAD),XZESCA(MTROAD)
      REAL WTFEU(MTROAD)
      INTEGER NTROAD,IROAD,IROADS,NZFMAX
      INTEGER NZFIND(MTROAD),NZFMIN(MTROAD)
      INTEGER MAXLYR,IHIT,IHITM,IHITS
      REAL USECUT,RESMIN,RESCAL
      INTEGER IPLEND(MTROAD,KLYRLF),IADEND(MTROAD,KLYRLF)
      INTEGER IHTEND(MTROAD,KLYRLF)
      REAL SSSEND(MTROAD,KLYRLF)
      REAL Z0FIND(MTROAD),DZFIND(MTROAD),RESSUM(MTROAD)
C
C FUNCTIONS
      REAL ARCFUN
      EXTERNAL ARCFUN
C----------------------Executable code starts here---------------------
#if defined(CLEO_SFDIAG)
      PRINT 1001,NLYRLR,NLYRHT,ILRESC(1),ILRESC(2)
1001  FORMAT(' SFENDP: NLYRLR=',I2,' NLYRHT=',I2,
     1                ' ESC(VD)=',I2,'  ESC(DR)=',I2)
#endif
      IF(NLYRLR.LE.0)GO TO 199
      IF(ILRESC(2).EQ.0)GO TO 199
      LLFIND=.FALSE.
      IF(ILRESC(2).GE.MLYRCF-1)ILRESC(2)=MLYRCF-1
C              find the intersection of the DR exit
      RESCDR=(RENDTF(ILRESC(2))+RENDTF(ILRESC(2)+1) )/2.
      RKAPPA=ABS(KAPCF)*RESCDR
      TRYDR=1
      GO TO 7
5     IF(TRYDR.EQ.2)GO TO 41
      TRYDR=2
C              expect track to cross the last hit layer
      RESCDR=RENDTF(ILRESC(2))
      RKAPPA=ABS(KAPCF)*RESCDR
7     RSQESC=RESCDR**2
      CALL TFXYTR(ALPCF,BETCF,KAPCF,GAMCF,RSQESC,+1,
     1              XESCAP,YESCAP,FESCAP,IER)
      IF(IER.NE.0)GO TO 5
      SESC(2)=ARCFUN(KAPCF,ALPCF,BETCF,XESCAP,YESCAP,XICF)
C              check to see if the VD exit is available
C
C             zero out NTROAD, set to 2 in endplate  section,
C                               increment in the curler section
      NTROAD=0
C
C decide which kind of escape this really is
C KAPCF > RCMXEP/RADIUS indicates that track is near point of curling
      IF(RKAPPA.GT.RCMXEP)GO TO 43
C track is not at point of curling
      IF(ILRESC(1).NE.0)GO TO 21
C do not have ILRESC(1), an exit point from the VD
      IF(ILRESC(2).GT.RESCC6)GO TO 35
      IF(ILRESC(2).LT.RESCC1)GO TO 35
      GO TO 43
C have exits from both VD and DR, but do not use the VD exit if
C  it should not exist
21    CONTINUE
      IF(ILRESC(2).GT.RESCC1)GO TO 23
      GO TO 35
23    CONTINUE
      IF(ILRESC(2).GT.RESCC2)GO TO 25
      GO TO 43
25    CONTINUE
      IF(ILRESC(2).GT.RESCC3)GO TO 27
      IF(ILRESC(1).LT.RESCC4)GO TO 43
      IF(ILRESC(1).GT.RESCC5)GO TO 43
      GO TO 33
27    CONTINUE
      IF(ILRESC(2).GT.RESCC6)GO TO 29
      IF(ILRESC(1).LT.RESCC7)GO TO 43
      IF(ILRESC(1).GT.RESCC8)GO TO 43
      GO TO 33
29    CONTINUE
      IF(ILRESC(2).GT.RESCC9)GO TO 35
      GO TO 33
C have USABLE exits from both DR and VD, do a fit,
C initialize matrix with origin + the X,Y vale at DR exit
C
33    STATTM=1
      X0S=2.
      X1S=SESC(2)
      X2S=SESC(2)**2
      YX0S=ZENDTF(ILRESC(2))
      YX1S=ZENDTF(ILRESC(2))*SESC(2)
C              find the intersection of the VD exit
      RESCVD=(RENDTF(ILRESC(1))+RENDTF(ILRESC(1)+1) )/2.
      RSQESC=RESCVD**2
      CALL TFXYTR(ALPCF,BETCF,KAPCF,GAMCF,RSQESC,+1,
     1              XESCAP,YESCAP,FESCAP,IER)
      IF(IER.NE.0)GO TO 199
      SESC(1)=ARCFUN(KAPCF,ALPCF,BETCF,XESCAP,YESCAP,XICF)
C              add this to the matrix
      X0S=X0S+1.
      X1S=X1S+SESC(1)
      X2S=X2S+SESC(1)**2
      YX0S=YX0S+ZENDTF(ILRESC(1))
      YX1S=YX1S+ZENDTF(ILRESC(1))*SESC(1)
C              invert the matrix and find the Z track parameters
      DET=X0S*X2S-X1S**2
      IF(ABS(DET).LT. 1.E-10)GO TO 35
      XINTER(1)=-( X2S*YX0S-X1S*YX1S)/DET
      XSLOPE(1)=-(-X1S*YX0S+X0S*YX1S)/DET
#if defined(CLEO_SFDIAG)
      PRINT 1006,ILRESC(1),ILRESC(2),XINTER(1),XSLOPE(1)
1006  FORMAT(' SFENDP: 2 ESCAPE: ESC(VD)=',I2,'  ESC(DR)=',I2,
     1       '  INTC=',F9.3,'  SLOPE=',F8.2)
#endif
      GO TO 37
C              this section if there is only a DR exit or if matrix is singular
35    STATTM=2
      XINTER(1)=0.
      XSLOPE(1)=-ZENDTF(ILRESC(2))/SESC(2)
#if defined(CLEO_SFDIAG)
      PRINT 1007,ILRESC(1),ILRESC(2),XINTER(1),XSLOPE(1)
1007  FORMAT(' SFENDP: 1 ESCAPE: ESC(VD)=',I2,'  ESC(DR)=',I2,
     1       '  INTC=',F9.3,'  SLOPE=',F8.2)
#endif
C              common finish point, (for escape defined roads)
37    NTROAD=2
      XARCES(1)=SESC(2)
      XZESCA(1)=-ZENDTF(ILRESC(2))
      XINTER(2)=-XINTER(1)
      XSLOPE(2)=-XSLOPE(1)
      XARCES(2)=+XARCES(1)
      XZESCA(2)=-XZESCA(1)
      NZFMIN(1)=1
      NZFMIN(2)=1
      WTFEU(1)=WTFE
      WTFEU(2)=WTFE
      STATRD(1)=STATTM
      STATRD(2)=STATTM
C
      IF(STATTM.EQ.2)GO TO 43
C
41    IF(RKAPPA.LT.RCMNHT)GO TO 91
C
C        it is assumed that RCMNHT < RCMXEP
c
C        this section to load roads as defined by hits in
C          the outer Z layer
43    CONTINUE
C         determine radius of layer with maximum allowed radius
C         use NLYRLR to avoid noisy layers
      MAXLYR=0
      DO 55 ILLYR=1,NLYRLR
      IF(RENDTF(IPLRLR(ILLYR)).GT.RESCDR+RADADD)GO TO 55
      IF(MAXLYR.EQ.0)GO TO 53
      IF(RENDTF(IPLRLR(ILLYR)).LT.RENDTF(IPLRLR(MAXLYR)))GO TO 55
53    MAXLYR=ILLYR
55    CONTINUE
C         get the hits in  this layer
      IF(MAXLYR.LE.0)GO TO 71
      DO 69 IHIT=LHITLR(MAXLYR),MHITLR(MAXLYR)
      IF(IHIT.EQ.LHITLR(MAXLYR))GO TO 63
      IF( ABS(YHIT(IHIT)-YHIT(IHIT-1)) .LT. DIFMAX)GO TO 69
63    IF(NTROAD.GE.MTROAD)GO TO 71
      NTROAD=NTROAD+1
      XINTER(NTROAD)=0.
      XSLOPE(NTROAD)=YHIT(IHIT)/XHIT(IHIT)
      XARCES(NTROAD)=XHIT(IHIT)
      XZESCA(NTROAD)=YHIT(IHIT)
      WTFEU(NTROAD)=0.
      NZFMIN(NTROAD)=1
      STATRD(NTROAD)=4
#if defined(CLEO_SFDIAG)
      PRINT 1008,ILRESC(1),ILRESC(2),XINTER(NTROAD),XSLOPE(NTROAD),
     1          NTROAD,IPLRLR(MAXLYR)
1008  FORMAT(' SFENDP: LAST HIT: ESC(VD)=',I2,'  ESC(DR)=',I2,
     1       '  INTC=',F9.3,'  SLOPE=',F8.2,
     2       '  ROAD',I2,' PHYSLAYR',I3)
#endif
69    CONTINUE
71    CONTINUE
      GO TO 91
C             common finnish point for all roads
C              use XINTER and XSLOPE to select hits
C              use XARCES and XZESCA to load escape point into the fit
C              there are NLYRLR hit layers saved in IPLRLR, LHITLR, MHITLR
C              there are NHIT hits saved in IADRHT, XHIT, YHIT....
91    IF(NTROAD.LT.1)GO TO 199
C
C    BUT FIRST    fix up the ramped down layers
C           do this only on the stereo layers
      DO 97 ILLYR=1,NLYRHT
      IF(IPLRLR(ILLYR).LT.60)GO TO 97
      IF(IPLRLR(ILLYR).GT.70)GO TO 97
      IHITM=LHITLR(ILLYR)+1
      IF(IHITM.GT.MHITLR(ILLYR))GO TO 97
      DO 93 IHIT=IHITM,MHITLR(ILLYR)
      IF( ABS(XHIT(IHIT)-XHIT(IHIT-1)) .GT. DIFMAX)GO TO 93
      IF( ABS(YHIT(IHIT)-YHIT(IHIT-1)) .GT. DIFMAX)GO TO 93
      YHIT(IHIT)=YHIT(IHIT)-DELADD
      YHIT(IHIT-1)=YHIT(IHIT-1)+DELADD
93    CONTINUE
97    CONTINUE
C
C
C              loop over the PRESELECTED ROADS
      DO 139 IROAD=1,NTROAD
      USECUT=HITCUT*SQRT(1.+XSLOPE(IROAD)**2)
      NZFIND(IROAD)=0
C              initialize matrix with origin and DR exit point
      X0S= WTFO+WTFEU(IROAD)
      X1S=      WTFEU(IROAD)*XARCES(IROAD)
      X2S=      WTFEU(IROAD)*XARCES(IROAD)**2
      YX0S=     WTFEU(IROAD)                 *XZESCA(IROAD)
      YX1S=     WTFEU(IROAD)*XARCES(IROAD)   *XZESCA(IROAD)
      RESSUM(IROAD)=0.
C              loop over layers
C NOTE! loop over NLYRHT layers, not NLYRLR
      DO 129 ILLYR=1,NLYRHT
C cut on radius of layers added to road, similar to raod definition
      IF(RENDTF(IPLRLR(ILLYR)).GT.RESCDR+(  2.5 )*RADADD)GO TO 129
      RESMIN=9999.
C              loop over hits in the layer
      DO 119 IHIT=LHITLR(ILLYR),MHITLR(ILLYR)
      RESCAL=ABS( XSLOPE(IROAD)*XHIT(IHIT)
     1             -YHIT(IHIT)
     2             +XINTER(IROAD) )
      IF(RESCAL.GT.RESMIN)GO TO 119
      IHITS=IHIT
      RESMIN=RESCAL
119   CONTINUE
C              end of loop over hits in the layer
#if defined(CLEO_SFDIAG)
      PRINT 1004,IROAD,ILLYR,IPLRLR(ILLYR),
     2         LHITLR(ILLYR),MHITLR(ILLYR),RESMIN,IHITS,USECUT
1004  FORMAT(' SFENDP:R=',I1,' LL=',I2,' IPL=',I2,' LH=',I3,
     1       ' MH=',I3,' RESMIN=',F4.2,' HS=',I2,' CUT=',F5.3)
#endif
      IF(RESMIN.GT.USECUT)GO TO 129
      NZFIND(IROAD)=NZFIND(IROAD)+1
      X0S= X0S +1.
      X1S= X1S +   XHIT(IHITS)
      X2S= X2S +   XHIT(IHITS)**2
      YX0S=YX0S+                  YHIT(IHITS)
      YX1S=YX1S+   XHIT(IHITS)   *YHIT(IHITS)
      RESSUM(IROAD)=RESSUM(IROAD)+RESMIN
      IPLEND(IROAD,NZFIND(IROAD))=IPLRLR(ILLYR)
      IADEND(IROAD,NZFIND(IROAD))=IADRHT(IHITS)
      IHTEND(IROAD,NZFIND(IROAD))=IHITS
      SSSEND(IROAD,NZFIND(IROAD))=SLYRLR(ILLYR)
129   CONTINUE
C              end of loop over layers
C              finish the fit if hits were found
C              using origin and DR exit means that only one hit is nessesary
      IF(NZFIND(IROAD).LT.NZFMIN(IROAD))GO TO 139
      DET=X0S*X2S-X1S**2
      Z0FIND(IROAD)=( X2S*YX0S-X1S*YX1S)/DET
      DZFIND(IROAD)=(-X1S*YX0S+X0S*YX1S)/DET
      RESSUM(IROAD)=RESSUM(IROAD)/NZFIND(IROAD)
139   CONTINUE
C              end of loop over PRESELECTED ROADS
C              eliminate solution with fewer hits
#if defined(CLEO_SFDIAG)
      PRINT 1002,(IROAD,NZFIND(IROAD),RESSUM(IROAD),Z0FIND(IROAD),
     1           DZFIND(IROAD),IROAD=1,NTROAD)
1002  FORMAT(
     1' SFENDP:R',I1,' HITS=',I2,' RES=',F8.4,' Z0=',F9.6,' DZ=',F9.6)
#endif
C  select road to be saved
      IROADS=0
      NZFMAX=1
      DO 149 IROAD=1,NTROAD
      IF(NZFIND(IROAD).LT.NZFMIN(IROAD))GO TO 148
      IF(NZFIND(IROAD).LT.NZFMAX)GO TO 148
      IF(IROADS.EQ.0)GO TO 147
      IF(NZFIND(IROAD).GT.NZFMAX)GO TO 147
C              equal (nonzero) number of hits
C              favor solution from endplate; STATRD=1,2 endplate; =4 last
      IF(STATRD(IROAD).EQ.4)GO TO 143
C              new solution is endplate
      IF(STATRD(IROADS).EQ.4)GO TO 147
      GO TO 145
C              new solution is not endplate
143   IF(STATRD(IROADS).EQ.4)GO TO 145
      GO TO 148
C              eliminate solution with higher errors
145   IF(RESSUM(IROAD).GE.RESSUM(IROADS))GO TO 148
C               save a new solution
147   IROADS=IROAD
      NZFMAX=NZFIND(IROAD)
148   CONTINUE
#if defined(CLEO_SFDIAG)
      PRINT 1005,IROAD,NZFIND(IROAD),RESSUM(IROAD),IROADS,NZFMAX,
     1          NZFMIN(IROAD)
1005  FORMAT(' SFENDP:R=',I1,' NZF=',I2,' RES=',F5.2,' RS=',I1,
     1       ' NZFMAX=',I2,' NZFMIN=',I2)
#endif
149   CONTINUE
150   CONTINUE
      IF(IROADS.LE.0)GO TO 199
#if defined(CLEO_SFDIAG)
      PRINT 1003,IROADS
1003  FORMAT(' SFENDP: CHOSE ROAD=',I2)
#endif
C   end of road selection
C   but it may still not be good
      IF(ILRESC(2).LT.LRESC1)GO TO 153
      IF(ILRESC(2).LT.LRESC2)GO TO 151
C              long track, ILRESC(2).GE.LRESC2
      IF(NZFIND(IROADS).LT.NFIND2)GO TO 199
      GO TO 153
C              medium track, ILRESC(2).GE.LRESC1, but ILRESC(2).LT.LRESC2
151   IF(NZFIND(IROADS).LT.NFIND1)GO TO 199
C              short track or passed criterion above
153   LLFIND=.TRUE.
C              load up the TFTRAK values with solution IROADS
      TANDLF=DZFIND(IROADS)
      Y0LF=Z0FIND(IROADS)
      NFIT=NZFIND(IROADS)
      PHI6CF=0.
      PHI7CF=0.
      RKN7CF=9.98
C
C      CALL SBYT(STATRD(IROADS),STATCF(2),28,3)
C
      DO 155 ILLYR=1,NFIT
      ILYR         =IPLEND(IROADS,ILLYR)
      IPLFIT(ILLYR)=ILYR
      IADFIT(ILLYR)=IADEND(IROADS,ILLYR)
      IHITS        =IHTEND(IROADS,ILLYR)
      PULFIT(ILLYR)=YHIT(IHITS)   -(Y0LF+XHIT(IHITS)*TANDLF)
      IF(IDVCTF(ILYR).EQ.IDRFT)PULFIT(ILLYR)=PULFIT(ILLYR)*TANSTF(ILYR)
155   SFIT(ILLYR)=SSSEND(IROADS,ILLYR)
      NHITLF=NFIT
      NHITTF=NHITCF+NHITLF
C              this is a FAKE
      CHILF=0.
      CHITF=CHICF
C              this is really a FAKE, order: cu,fi,da,ct,z0,phi6
      CALL VZERO(ERMFIT,NERMTF)
      ERMFIT( 1)=.0001
      ERMFIT( 3)=.0025
      ERMFIT( 6)=.000025
      ERMFIT(10)=.0001
      ERMFIT(15)=.0004
      IF(NERMTF.GE.21)ERMFIT(21)=.00000001
      IF(NERMTF.GE.28)ERMFIT(28)=.00000001
      CALL TFUSTK(SECONDARY_STAK,NHITCF)
      GOTO 200
199   CONTINUE
      TANDLF=0.
      Y0LF  =0.
      PHI6CF=0.
      PHI7CF=0.
      RKN7CF=9.93
200   CONTINUE
      RETURN
      END
