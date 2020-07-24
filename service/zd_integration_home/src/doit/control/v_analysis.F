*
* $Id: v_analysis.F,v 1.2 2003/04/07 17:30:31 cleo3 Exp $
*
* $Log: v_analysis.F,v $
* Revision 1.2  2003/04/07 17:30:31  cleo3
* now compiles under Linux
*
* Revision 1.1  2003/03/05 17:07:12  dpp
*      -> NEW ROUTINE
*      -> this is the analysis to find K shorts in doit output
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author : D Peterson
      SUBROUTINE V_ANALYSIS
C.......................................................................
C.
C. V_ANALYSIS - look for K shorts
C.
C.
C. COMMON    :
C. CALLS     :
C. CALLED    :
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.0
C. CREATED   : 23-Oct-2002
C.......................................................................

#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfconspa.inc"
#include "doit/duseq/controlpar.inc"
#include "doit/duseq/cdscrtcd.inc"
#include "doit/duseq/cdtrakcd.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/runev.inc"

c23456789 123456789 123456789 123456789 123456789 123456789 123456789 12 

      INTEGER ITRK
      INTEGER JTRK,ILIM,JLIM
      INTEGER AFTRTRKS/0/
      INTEGER IY

      REAL PXORG(MTRKCD),PYORG(MTRKCD),PZORG(MTRKCD)
      REAL EPORG(MTRKCD),EKORG(MTRKCD),PPERP(MTRKCD)
      REAL XCNTRCURV(MTRKCD),YCNTRCURV(MTRKCD),RADISCURV(MTRKCD)

      REAL PXSUM,PYSUM,PZSUM,PPSUM2,PPSUM,PSUM2,PSUM
      REAL PAIRMASSK0,PAIRMASSD0

      REAL LARGD0
      REAL CLOSEST,DIFFCLS,DIFFNOW
      REAL FISHSEAGULL

      REAL XDIF,YDIF,X2DIF,Y2DIF,R2DIF,LINCON,LINSLP
      REAL AQUAD,BQUAD,CQUAD,RADQUAD,SOLVQUAD(2)
      LOGICAL SLOPEisDXovrDy
      REAL XDECAY(2),YDECAY(2),RDECAY
      REAL RNORM
      INTEGER NDECAY,IDECAY,KDECAY
      REAL RTEST,RCOSTEST,RCOSTESTMIN,RDECAYCOS
      LOGICAL KEEPTHIS

      REAL RADCURVTRAN
      REAL PXTRANI,PYTRANI
      REAL PXTRANJ,PYTRANJ
      REAL PXSUMTRAN,PYSUMTRAN,PPSUMTRAN2,PSUMTRAN2,PAIRMASSK0TRAN


      INTEGER ICLSAV,JCLSAV
      INTEGER ISAV,NSAV,ITRKSAV(1000),JTRKSAV(1000)
      REAL MASSK0SAV(1000),MASSD0SAV(1000),LARGD0SAV(1000)
      REAL FISHSEAGULLSAV(1000),RDECAYCOSSAV(1000)
      REAL MASSK0TRANSAV(1000)


      REAL CFIX
      COMMON/FIXER/CFIX(250)

      INTEGER NCALL/0/
      INTEGER NCALL50

* ----------Executable code starts here---------------------
      PRINT 7701,NTRKCD
 7701 FORMAT(' V_ANALYSISdoitTPC:NTRKCD=',I3)
      NCALL=NCALL+1

c23456789 123456789 123456789 123456789 123456789 123456789 123456789 12 
      ILIM=NTRKCD-1
      IF(ILIM.GE.1)THEN
        
        DO 819 ITRK=1,NTRKCD
          PPERP(ITRK)=.225/ABS(CURCD(ITRK))
          PXORG(ITRK)=PPERP(ITRK)*COS(PHI0CD(ITRK))
          PYORG(ITRK)=PPERP(ITRK)*SIN(PHI0CD(ITRK))
          PZORG(ITRK)=PPERP(ITRK)*TANDCD(ITRK)
          EPORG(ITRK)=SQRT(AMPI2 +PPERP(ITRK)**2 *(1.+TANDCD(ITRK)**2))
          EKORG(ITRK)=SQRT(AMKA2 +PPERP(ITRK)**2 *(1.+TANDCD(ITRK)**2))
          XCNTRCURV(ITRK)=-(.5/CURCD(ITRK)+D0CD(ITRK))*SIN(PHI0CD(ITRK))
          YCNTRCURV(ITRK)=+(.5/CURCD(ITRK)+D0CD(ITRK))*COS(PHI0CD(ITRK))
          RADISCURV(ITRK)=ABS(.5/CURCD(ITRK))
 819    CONTINUE
        
        CLOSEST=0.
        DIFFCLS=9.
        ICLSAV=0
        JCLSAV=0
        NSAV=0
        DO 849 ITRK=1,ILIM
          JLIM=ITRK+1
          DO 839 JTRK=JLIM,NTRKCD
            PXSUM=PXORG(ITRK)+PXORG(JTRK)
            PYSUM=PYORG(ITRK)+PYORG(JTRK)
            PZSUM=PZORG(ITRK)+PZORG(JTRK)
            PPSUM2=PXSUM**2+PYSUM**2
            PPSUM=SQRT(PPSUM2)
            PSUM2=PPSUM2+PZSUM**2
            PSUM=SQRT(PSUM2)
            
            PAIRMASSK0=SQRT((EPORG(ITRK)+EPORG(JTRK))**2-PSUM2)
            
c opp sign
            IF(CURCD(ITRK)*CURCD(JTRK).LT.0.)THEN
              
c get the D0 mass
              IF(CURCD(ITRK).GT.0.)THEN
                PAIRMASSD0=(EPORG(ITRK)+EKORG(JTRK))**2 
                FISHSEAGULL=D0CD(ITRK)-D0CD(JTRK)
              ELSE
                PAIRMASSD0=(EKORG(ITRK)+EPORG(JTRK))**2 
                FISHSEAGULL=D0CD(JTRK)-D0CD(ITRK)
              ENDIF
              
              PAIRMASSD0=SQRT(PAIRMASSD0-PSUM2)
              
              DIFFNOW=ABS(PAIRMASSD0-1.865)
              IF(DIFFNOW.LT.DIFFCLS)THEN
                CLOSEST=PAIRMASSD0
                DIFFCLS=DIFFNOW
                ICLSAV=ITRK
                JCLSAV=JTRK
              ENDIF
              
c crossing point
              XDIF=XCNTRCURV(ITRK)-XCNTRCURV(JTRK)
              YDIF=YCNTRCURV(ITRK)-YCNTRCURV(JTRK)
              X2DIF=XCNTRCURV(ITRK)**2-XCNTRCURV(JTRK)**2
              Y2DIF=YCNTRCURV(ITRK)**2-YCNTRCURV(JTRK)**2
              R2DIF=RADISCURV(ITRK)**2-RADISCURV(JTRK)**2
              
              IF(ABS(XDIF).GT.ABS(YDIF))THEN
c line is for x=lincon+linslp*y
                SLOPEisDXovrDy=.TRUE.
                LINCON=-.5*(R2DIF-X2DIF-Y2DIF)/XDIF
                LINSLP=-YDIF/XDIF
                AQUAD=LINSLP**2+1.
                BQUAD=2.*
     2               (LINSLP*(LINCON-XCNTRCURV(ITRK))
     3               -YCNTRCURV(ITRK))
                CQUAD=(LINCON-XCNTRCURV(ITRK))**2
     2               +YCNTRCURV(ITRK)**2
     3               -RADISCURV(ITRK)**2
              ELSE
c line is for y=lincon+linslp*x
                SLOPEisDXovrDy=.FALSE.
                LINCON=-.5*(R2DIF-X2DIF-Y2DIF)/YDIF
                LINSLP=-XDIF/YDIF
                AQUAD=LINSLP**2+1.
                BQUAD=2.*
     2               (LINSLP*(LINCON-YCNTRCURV(ITRK))
     3               -XCNTRCURV(ITRK))
                CQUAD=(LINCON-YCNTRCURV(ITRK))**2
     2               +XCNTRCURV(ITRK)**2
     3               -RADISCURV(ITRK)**2
              ENDIF
              RADQUAD=BQUAD**2-4.*AQUAD*CQUAD
              
              IF(RADQUAD.GT.0.)THEN
                NDECAY=2
                SOLVQUAD(1)=.5*(-BQUAD+SQRT(RADQUAD))/AQUAD
                SOLVQUAD(2)=.5*(-BQUAD-SQRT(RADQUAD))/AQUAD
                IF(SLOPEisDXovrDy)THEN
                  YDECAY(1)=SOLVQUAD(1)
                  XDECAY(1)=LINCON+LINSLP*YDECAY(1)
                  YDECAY(2)=SOLVQUAD(2)
                  XDECAY(2)=LINCON+LINSLP*YDECAY(2)
                ELSE
                  XDECAY(1)=SOLVQUAD(1)
                  YDECAY(1)=LINCON+LINSLP*XDECAY(1)
                  XDECAY(2)=SOLVQUAD(2)
                  YDECAY(2)=LINCON+LINSLP*XDECAY(2)
                ENDIF
              ELSE
                NDECAY=1
                RNORM=RADISCURV(ITRK)/
     2               (RADISCURV(ITRK)+RADISCURV(JTRK))
                XDECAY(1)=XCNTRCURV(ITRK)
     2               +RNORM*(XCNTRCURV(JTRK)-XCNTRCURV(ITRK))
                YDECAY(1)=YCNTRCURV(ITRK)
     2               +RNORM*(YCNTRCURV(JTRK)-YCNTRCURV(ITRK))
              ENDIF
              
              KDECAY=0
              RDECAY=0.
              
              RCOSTESTMIN=-.01

              IF(NDECAY.GE.1)THEN
                RTEST=SQRT(XDECAY(1)**2+YDECAY(1)**2)
                RCOSTEST=(PXSUM*XDECAY(1)+PYSUM*YDECAY(1))/PPSUM
                IF(RCOSTEST.GT.RCOSTESTMIN)THEN
                  KDECAY=1
                  RDECAY=RTEST
                  RDECAYCOS=RCOSTEST
                ENDIF
              ENDIF

              IF(NDECAY.GE.2)THEN
                RTEST=SQRT(XDECAY(2)**2+YDECAY(2)**2)
                RCOSTEST=(PXSUM*XDECAY(2)+PYSUM*YDECAY(2))/PPSUM
                IF(RCOSTEST.GT.RCOSTESTMIN)THEN
                  IF(KDECAY.EQ.0)THEN
                    KEEPTHIS=.TRUE.
                  ELSE
                    IF(RCOSTEST.LT.RDECAYCOS)THEN
                      KEEPTHIS=.TRUE.
                    ELSE
                      KEEPTHIS=.FALSE.
                    ENDIF
                  ENDIF
                  IF(KEEPTHIS)THEN
                    KDECAY=2
                    RDECAY=RTEST
                    RDECAYCOS=RCOSTEST
                  ENDIF
                ENDIF
              ENDIF
              
              PRINT 8002,
     1             ITRK,CURCD(ITRK),D0CD(ITRK),PHI0CD(ITRK),
     2             XCNTRCURV(ITRK),YCNTRCURV(ITRK),RADISCURV(ITRK),
     1             JTRK,CURCD(JTRK),D0CD(JTRK),PHI0CD(JTRK),
     2             XCNTRCURV(JTRK),YCNTRCURV(JTRK),RADISCURV(JTRK),
     3             NDECAY,XDECAY(1),YDECAY(1),XDECAY(2),YDECAY(2),
     4             KDECAY,RDECAY,RDECAYCOS
 8002         FORMAT(' '/
     1             ' track:',I3,' cu=',F6.3,' d=',F6.4,' f=',F6.4,
     1             ' center x,y=(',F8.3,',',F8.3,') r=',F8.3/
     2             ' track:',I3,' cu=',F6.3,' d=',F6.4,' f=',F6.4,
     2             ' center x,y=(',F8.3,',',F8.3,') r=',F8.3/
     3             ' Ndecay=',I1,
     3             '   decay at ',
     3             '(',F11.6,',',F11.6,')',
     3             ' or ',
     3             '(',F11.6,',',F11.6,')',
     4             ' KD=',I1,'  RDECAY=',F8.3,'  RDECAYCOS=',F8.3)


              IF(KDECAY.NE.0)THEN
                RADCURVTRAN=SIGN(SQRT(
     1               (XCNTRCURV(ITRK)-XDECAY(KDECAY))**2+
     2               (YCNTRCURV(ITRK)-YDECAY(KDECAY))**2),CURCD(ITRK))
                PXTRANI=
     1               -(YCNTRCURV(ITRK)-YDECAY(KDECAY))
     2               /RADCURVTRAN*PPERP(ITRK)
                PYTRANI=
     1               +(XCNTRCURV(ITRK)-XDECAY(KDECAY))
     2               /RADCURVTRAN*PPERP(ITRK)
                RADCURVTRAN=SIGN(SQRT(
     1               (XCNTRCURV(JTRK)-XDECAY(KDECAY))**2+
     2               (YCNTRCURV(JTRK)-YDECAY(KDECAY))**2),CURCD(JTRK))
                PXTRANJ=
     1               -(YCNTRCURV(JTRK)-YDECAY(KDECAY))
     2               /RADCURVTRAN*PPERP(JTRK)
                PYTRANJ=
     1               +(XCNTRCURV(JTRK)-XDECAY(KDECAY))
     2               /RADCURVTRAN*PPERP(JTRK)
                PXSUMTRAN=PXTRANI+PXTRANJ
                PYSUMTRAN=PYTRANI+PYTRANJ
                PPSUMTRAN2=PXSUMTRAN**2+PYSUMTRAN**2
                PSUMTRAN2=PPSUMTRAN2+PZSUM**2
                PAIRMASSK0TRAN=
     2               SQRT((EPORG(ITRK)+EPORG(JTRK))**2-PSUMTRAN2)
              ELSE
                PAIRMASSK0TRAN=PAIRMASSK0
              ENDIF



c23456789 123456789 123456789 123456789 123456789 123456789 123456789 12 
              
            ELSE
              PAIRMASSD0=-1.
              RDECAYCOS=0.
              NDECAY=0
            ENDIF
            
            LARGD0=ABS(D0CD(ITRK))
            IF(ABS(D0CD(JTRK)).GT.LARGD0)LARGD0=ABS(D0CD(JTRK))
            
            IF(NSAV.LT.1000)THEN
              NSAV=NSAV+1
              ITRKSAV(NSAV)=ITRK
              JTRKSAV(NSAV)=JTRK
              MASSK0SAV(NSAV)=PAIRMASSK0
              MASSD0SAV(NSAV)=PAIRMASSD0
              LARGD0SAV(NSAV)=LARGD0
              FISHSEAGULLSAV(NSAV)=FISHSEAGULL
              RDECAYCOSSAV(NSAV)=RDECAYCOS
              MASSK0TRANSAV(NSAV)=PAIRMASSK0TRAN
            ENDIF
            
 839      CONTINUE
 849    CONTINUE
        CFIX(14)=1.*NTRKCD+.1
        CFIX(15)=DIFFCLS
        CALL MAKE
        
        IF(NSAV.GT.1)THEN
          DO 879 ISAV=1,NSAV
            ITRK=ITRKSAV(ISAV)
            JTRK=JTRKSAV(ISAV)
            PAIRMASSK0=MASSK0SAV(ISAV)
            PAIRMASSD0=MASSD0SAV(ISAV)
            LARGD0=LARGD0SAV(ISAV)
            FISHSEAGULL=FISHSEAGULLSAV(ISAV)
            RDECAYCOS=RDECAYCOSSAV(ISAV)
            PAIRMASSK0TRAN=MASSK0TRANSAV(ISAV)
            
            PRINT 8001,ITRK,JTRK,PAIRMASSK0,RDECAYCOS
 8001       FORMAT(' dump_dufind:',2I3,
     2           '  mass=',F7.3,' rDECAYcos=',F7.3)
            
            CFIX(1)=PAIRMASSK0
            
            IF(CURCD(ITRK)*CURCD(JTRK).LT.0.)THEN
              CFIX(2)=0.
              CFIX(7)=PAIRMASSD0
            ELSEIF(CURCD(ITRK).GT.0.)THEN
              CFIX(2)=2.
            ELSE
              CFIX(2)=-2.
            ENDIF
            
            CFIX(3)=LARGD0
            CFIX(4)=1.*NTRKCD+.1
            CFIX(5)=DIFFCLS
            CFIX(8)=FISHSEAGULL
            CFIX(9)=RDECAYCOS
            CFIX(10)=PAIRMASSK0TRAN

            IF(
     1           (ITRK.EQ.ICLSAV).OR.
     2           (ITRK.EQ.JCLSAV).OR.
     1           (JTRK.EQ.ICLSAV).OR.
     2           (JTRK.EQ.JCLSAV))THEN
              CFIX(6)= 1.1
            ELSE
              CFIX(6)=-1.1
            ENDIF
            
            CALL MAKE
 879      CONTINUE
        ENDIF
      ENDIF
        
        
      IF(CURRENT_EVENT.EQ.497)CALL HISTO(2,1)
      NCALL50=NCALL/50
      IF(50*NCALL50.EQ.NCALL)CALL HISTO(2,1)

#if defined(CLEO_SFDIAG)
      IF(AFTRTRKS.EQ.0)THEN
        CALL DSF_RFV_INPUT(IY)
        IF(IY.EQ.'K')THEN
          AFTRTRKS=-1
        ENDIF
      ENDIF
#endif
      RETURN
      END



