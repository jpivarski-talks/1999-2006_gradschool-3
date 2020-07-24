*
* $Id: cdclus.F,v 1.2 2000/11/15 16:54:14 dpp Exp $
*
* $Log: cdclus.F,v $
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
      SUBROUTINE CDCLUS(ILYR, PHITRY, QMIN, QMAX, TCMAX, NHW, IHWADR)
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
C
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfctlpar.inc"
#include "doit/duseq/tftime.inc"
#include "doit/duseq/cdscrtcd.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/lfctlcde.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/tfhitscd.inc"
#include "doit/duseq/tftrakcd.inc"

      INTEGER IHWADR(*),IPKADR(NHITDM)
      INTEGER JLYR,ILYR,NWIRC,JWIRC,NHWSV,NHW,NSRCH,ISRCH,IWIRC,JADR
      INTEGER IPEAK,IHW,IADR,ILAST,INEXT,IADLST,IADNXT,IDIFF
      REAL PHITRY,QMAX,TDWCDC,TCMAX,QMIN
      REAL TDPEAK

* ----------Executable code starts here---------------------
#include "doit/duseq/duexed.inc"

C initialization   /find time correlation
C JLYR:   the associated anode layer
C NWIRC:  number of wires in associated anode layer
C JWIRC:  predicted wire in associated layer
C         (too bad PHITRY is always passed =0)
C NSRCH:  search path for looking for correlated wire
C         (this has been =1 in the past)

      IF (IDVCTF(ILYR).NE.ISVX1.AND.
     +    IDVCTF(ILYR).NE.ISVZ1) THEN

        JLYR   = ILCRTF(ILYR)
        NWIRC  = NWIRTF(JLYR)
        JWIRC  = NINT((PHITRY - PHIFTF(JLYR))/CELLTF(JLYR))

       ENDIF

      NSRCH  = 0

C NHWSV:  save the passed NHW, it will be zeroed to prepare for output
      NHWSV  = NHW
      NHW    = 0

C loop over the associated anode search path
C IWIRC:  tested anode wire number
C JADR:   wire address (address in map) of tested anode wire
      DO 695 ISRCH=-NSRCH,NSRCH
      IF (IDVCTF(ILYR).NE.ISVX1.AND.
     +    IDVCTF(ILYR).NE.ISVZ1) THEN
          IWIRC  = JWIRC + ISRCH
          IF (IWIRC.LT.0)     IWIRC = IWIRC + NWIRC
          IF (IWIRC.GE.NWIRC) IWIRC = IWIRC - NWIRC
          JADR   = IABS(IADRHW(INDXTF(JLYR) + IWIRC))
        ELSE
CRYD     Seriously uggly......
           JADR=IHWADR(1)
        ENDIF

C start of searching for peak cathode pad in the phi sector
220       IPEAK  = 0
C QMAX:   pulse height of the maximum ph pad
C         (initialize to the minimum allowed)
              IF (IDVCTF(ILYR).EQ.IVDCT) THEN
                  QMAX = QPKMCT
              ELSE
                  QMAX = QPKMDC
              ENDIF

              IF (IDVCTF(ILYR).EQ.ISVX1.OR.
     +          IDVCTF(ILYR).EQ.ISVZ1) THEN
                   QMAX=0.01
              ENDIF


C loop through cathode pads
              DO 239 IHW=1,NHWSV
                  IADR   = IHWADR(IHW)
                  IF (IADR.GT.0) THEN
                      IF (PWCD(IADR).GT.QMAX) THEN
                          IF (JADR.GT.0) THEN
                              TDWCDC = TDWCD(JADR)
                          ELSE
                              TDWCDC = TDWCD(IADR)
                          ENDIF
                          IF (ABS(TDWCD(IADR)-TDWCDC).LT.TCMAX) THEN
                              IPEAK  = IHW
                              QMAX   = PWCD(IADR)
                              TDPEAK = TDWCD(IADR)-TDWCDC
                          ENDIF
                      ENDIF
                  ENDIF
239           CONTINUE

              IF (IDVCTF(ILYR).EQ.IVDCT) THEN
                 CALL HFILL(3000+IXSFTF(ILYR),0.001*TDPEAK,0.,1.)
              ELSE
                 CALL HFILL(3500+IXSFTF(ILYR),0.001*TDPEAK,0.,1.)
              ENDIF

C stop processing the phi sector if there is no valid peak
              IF(IPEAK.LE.0)GO TO 685

C skip down to the first connected strip in the cluster
C this assumed that the hit number (ie 1:NHW) is sequential
C   in z direction (ie IWIRCD)

C ILAST:   last found hit
C INEXT:   tested hit, replaces ILAST if it is valid
                  ILAST  = IPEAK
240               IF (ILAST.LE.1)GO TO 243
                      INEXT  = ILAST - 1
                      IADLST = IHWADR(ILAST)
                      IADNXT = IHWADR(INEXT)
                      IF (IADNXT.LE.0)GO TO 243
                          IDIFF  = IABS(IWIRCD(IADLST)-IWIRCD(IADNXT))
                          IF (IDIFF.NE.1)GO TO 243
                          IF (PWCD(IADNXT).LE.QMIN)GO TO 243
                              ILAST  = INEXT
                              GO TO 240
243               CONTINUE
C                        ^---branch point when ILAST is at beginning of list
C                            or when the tested pad is nonexistant
C                               (including previously used)
C                            or when the pad is nonsequential
C                            or when the pad is low pulse height
C                            in this version, ILAST was never corrupted to =0
C
C skip up to the last connected strip in the cluster
                  NHW           = NHW + 1
                  IADLST        = IHWADR(ILAST)
                  IPKADR(NHW)   = IADLST
                  IHWADR(ILAST) = 0
270               IF (ILAST.LT.NHWSV) THEN
                      INEXT  = ILAST + 1
                      IADNXT = IHWADR(INEXT)
                      INXHCD(IADLST) = 0
                      ILAST  = NHWSV
                      IF (IADNXT.GT.0) THEN
                          IDIFF  = IABS(IWIRCD(IADLST)-IWIRCD(IADNXT))
                          IF (IDIFF.EQ.1.AND.PWCD(IADNXT).GT.QMIN) THEN
                              INXHCD(IADLST) = IADNXT
                              INXHCD(IADNXT) = 0
                              IHWADR(INEXT)  = 0
                              IADLST         = IADNXT
                              ILAST          = INEXT
                          ENDIF
                      ENDIF
                      GOTO 270
                  ENDIF
C loop back to do the next peak cathode pad
          GOTO 220
685   CONTINUE
C            ^---branch point when no peak is found
695   CONTINUE
C            ^---end of loop over associated anode search path

      IF (NHW.GT.0) CALL UCOPY( IPKADR, IHWADR, NHW )
      RETURN
      END
* <><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
#include "doit/duseq/dudeckd.inc"
