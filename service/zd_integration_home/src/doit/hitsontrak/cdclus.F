*
* $Id: cdclus.F,v 1.4 2003/03/05 17:57:39 dpp Exp $
*
* $Log: cdclus.F,v $
* Revision 1.4  2003/03/05 17:57:39  dpp
*      -> remove unused variables, qcrlct,qpkmct,qminct,etc.
*      -> remove use of PWCD; PWCD can be used for  other derived values
*      -> F77
*      -> changed all variable names in cdscrtcd to have common root
*
* Revision 1.3  2001/11/19 23:54:40  dpp
*      -> delete unused include file
*
* Revision 1.2  2000/11/15 16:54:14  dpp
*      -> "CDHITS" changed to "ROADHITS" to make way for MC infringement
*
* Revision 1.1.1.1  1998/02/06 19:11:43  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:29  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:00:52  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  6.00/24 06/05/94  22.43.51  by  Anders Ryd
*CMZ :  5.54/14 07/01/92  12.37.20  by  Steve Schaffner
*CMZ :  5.54/03 16/05/91  14.31.50  by  Brian Geiser
*CMZ :  5.53/07 09/10/90  00.05.47  by  Steve Schaffner
*CMZ :  5.53/01 12/04/90  16.24.28  by  Steve Schaffner
*CMZ :  5.52/01 22/03/90  20.57.30  by  Steve Schaffner
*CMZ :  5.51/00 09/08/89  15.33.08  by  Steve Schaffner
*CMZ :  5.50/00 06/09/88  14.28.41  by  Dan Riley
*-- Author :    Dan Riley   06/09/88
      SUBROUTINE CDCLUS(ILYR, PHITRY, NHW, IHWADR)
C.......................................................................
C.
C. CDCLUS - Arrange cathode strips into clusters.
C.
C. COMMON    : ?
C. CALLS     : ?
C. CALLED    : ROADHITS
C. AUTHOR    : M. Ogg. Converted to Fortran-77 by D. Riley
C.                     Corrected               by Brian Geiser
C.                     Corrected and partially converted to FORTRAN IV
C.                                             by Dan Peterson
C. VERSION   : 1.00
C. CREATED   : ?
C. LAST MOD  : 05-Sep-88
C.
C. Modification Log.
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/cdscrtcd.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/lfctlcde.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/tfhitscd.inc"
#include "doit/duseq/tftrakcd.inc"

      INTEGER IHWADR(*),IPKADR(NHITDM)
      INTEGER JLYR,ILYR,NWIRC,JWIRC,NHWSV,NHW,NSRCH,ISRCH,IWIRC,JADR
      INTEGER IPEAK,IHW,IADR,ILAST,INEXT,IADLST,IADNXT,IDIFF
      REAL PHITRY,QMAX,RAWTIMCRLT
      REAL TDPEAK
      LOGICAL TIME_CORR

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
* ----------Executable code starts here---------------------------------

C find the associated anode layer for cathodes only

C JLYR:   the associated anode layer
C NWIRC:  number of wires in associated anode layer
C JWIRC:  predicted wire in associated layer
C         (too bad PHITRY is always passed =0)
C NSRCH:  search path for looking for correlated wire
C         (this has been =1 in the past)

      IF(
     1     (IDVCTF(ILYR).EQ.IVDCT).OR.
     2     (IDVCTF(ILYR).EQ.IDRCT)) THEN
        JLYR=  ILCRTF(ILYR)
        NWIRC= NWIRTF(JLYR)
        JWIRC= NINT((PHITRY - PHIFTF(JLYR))/CELLTF(JLYR))
      ELSE
        JLYR=-1
      ENDIF
      NSRCH  = 0
      
C NHWSV:  save the passed NHW, it will be zeroed to prepare for output
      NHWSV  = NHW
      NHW    = 0

C loop over the associated anode search path
C IWIRC:  tested anode wire number
C JADR:   wire address (address in map) of tested anode wire

      DO 695 ISRCH=-NSRCH,NSRCH
        IF(JLYR.GT.0)THEN
          IWIRC  = JWIRC + ISRCH
          IF (IWIRC.LT.0)     IWIRC = IWIRC + NWIRC
          IF (IWIRC.GE.NWIRC) IWIRC = IWIRC - NWIRC
          JADR   = IABS(EvntHit_MapDet(INDXTF(JLYR) + IWIRC))
        ELSE
          JADR=0
        ENDIF
        
C-----------------------------------------------------------------------
C start of searching for peak cathode pad in the phi sector
C-----------------------------------------------------------------------

220       IPEAK  = 0

C QMAX: pulse height of the maximum ph pad
C       (initialize to the minimum allowed)

          QMAX=CLS_Q_P_MIN(ILYR)

C-----------------------------------------------------------------------
C loop through cathode pads looking for a new maximum
C-----------------------------------------------------------------------

          DO 239 IHW=1,NHWSV
            IADR   = IHWADR(IHW)
            IF (IADR.GT.0) THEN
              IF (EvntHit_Charge(IADR).GT.QMAX) THEN

                IF (JADR.GT.0) THEN
                  RAWTIMCRLT = EvntHit_rawTIM(JADR)
                  TIME_CORR=
     1                 ABS(EvntHit_rawTIM(IADR)-RAWTIMCRLT)
     2                 .LT.CLS_TC_MAX(ILYR)
                ELSE
                  RAWTIMCRLT = EvntHit_rawTIM(IADR)
                  TIME_CORR=.TRUE.
                ENDIF

                IF (TIME_CORR) THEN
                  IPEAK  = IHW
                  QMAX   = EvntHit_Charge(IADR)
                  TDPEAK = EvntHit_rawTIM(IADR)-RAWTIMCRLT
                ENDIF

              ENDIF
            ENDIF
 239      CONTINUE
          
C stop processing the input section if there is no valid peak
          
          IF(IPEAK.GT.0)THEN

C-----------------------------------------------------------------------
C skip down to the first connected strip in the cluster
C-----------------------------------------------------------------------

C this assumed that the hit number (ie 1:NHW) is sequential
C   in z direction (ie EvntHit_laWire)

C ILAST:   last found hit
C INEXT:   tested hit, replaces ILAST if it is valid

            ILAST  = IPEAK
 240        CONTINUE

            IF(ILAST.GT.1)THEN
              INEXT  = ILAST - 1
              IADLST = IHWADR(ILAST)
              IADNXT = IHWADR(INEXT)

              IF(IADNXT.GT.0)THEN
                IDIFF=IABS(
     1               EvntHit_laWire(IADLST)-EvntHit_laWire(IADNXT))
                
                IF(IDIFF.EQ.1)THEN

                  IF(EvntHit_Charge(IADNXT).GT.CLS_Q_P_MIN(ILYR))THEN
                    ILAST=INEXT
                    GO TO 240
                    
C end condition (EvntHit_Charge(IADNXT).GT.CLS_Q_P_MIN(ILYR)); pulse height
                  ENDIF
C end condition (IDIFF.EQ.1); pad is sequential
                ENDIF
C end condition (IADNXT.GT.0); tested pad exists, not previously used
              ENDIF
C end condition (ILAST.GT.1); have not skipped down to bottom of list
            ENDIF

C-----------------------------------------------------------------------
C skip up to the last connected strip in the cluster
C-----------------------------------------------------------------------

            NHW=NHW+1
            IADLST=IHWADR(ILAST)
            IPKADR(NHW)=IADLST
            IHWADR(ILAST)=0
 270        CONTINUE
            IF(ILAST.LT.NHWSV) THEN
              INEXT=ILAST+1
              IADNXT=IHWADR(INEXT)
              EvntHit_NxtCls(IADLST)=0
              ILAST=NHWSV
              IF (IADNXT.GT.0) THEN
                IDIFF=IABS(
     1               EvntHit_laWire(IADLST)-EvntHit_laWire(IADNXT))
                IF (IDIFF.EQ.1)THEN
                  IF(EvntHit_Charge(IADNXT).GT.CLS_Q_P_MIN(ILYR))THEN
                    EvntHit_NxtCls(IADLST) = IADNXT
                    EvntHit_NxtCls(IADNXT) = 0
                    IHWADR(INEXT)  = 0
                    IADLST         = IADNXT
                    ILAST          = INEXT
                  ENDIF
                ENDIF
              ENDIF
              GOTO 270
            ENDIF

C-----------------------------------------------------------------------
C loop back to do the next peak cathode pad
C-----------------------------------------------------------------------
          GOTO 220

C end of condition that peak is found
        ENDIF
C end of loop over associated anode search path
695   CONTINUE

      IF (NHW.GT.0) CALL UCOPY( IPKADR, IHWADR, NHW )

      RETURN
      END







