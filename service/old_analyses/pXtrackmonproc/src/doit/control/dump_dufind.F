*
* $Id: dump_dufind.F,v 1.1 2001/11/19 22:59:36 dpp Exp $
*
* $Log: dump_dufind.F,v $
* Revision 1.1  2001/11/19 22:59:36  dpp
*      -> NEW ROUTINE
*      -> screen dump of hits on tracks
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author : D Peterson
      SUBROUTINE DUMP_DUFIND
C.......................................................................
C.
C. DUMP_DUFIND - dump hits on tracks
C.
C.
C. COMMON    :
C. CALLS     :
C. CALLED    :
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.0
C. CREATED   : 16-11-2001
C.......................................................................

#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/controlpar.inc"
#include "doit/duseq/cdscrtcd.inc"
#include "doit/duseq/cdtrakcd.inc"
#include "doit/duseq/tfgeomcd.inc"


      INTEGER ITRK
      INTEGER IY
      INTEGER IFIT
      INTEGER TRK_HAS_CATH,CATH_USED(1536)
      INTEGER IWIRE,IWADR,IWADR_USED(KWS1CD)
      INTEGER AFTRTRKS/0/

* ----------Executable code starts here---------------------
      CALL VZERO(CATH_USED,1536)
      CALL VZERO(IWADR_USED,KWS1CD)
      PRINT 7701,NTRKCD
 7701 FORMAT(' DOIT:DUFIND:NTRKCD=',I3)
      IF(NTRKCD.GE.1)THEN
        DO 749 ITRK=1,NTRKCD
          IF(NFITCD(ITRK).GE.1)THEN
            TRK_HAS_CATH=0
            DO 729 IFIT=1,NFITCD(ITRK)
              IF(IDVCTF(IABS(IPLRCD(IFIT,ITRK))).EQ.IDRCT)THEN
                TRK_HAS_CATH=TRK_HAS_CATH+1
                IF(CATH_USED(IADFCD(IFIT,ITRK)).EQ.0)THEN
                  CATH_USED(IADFCD(IFIT,ITRK))=ITRK
                  IF(TRK_HAS_CATH.EQ.1)THEN
                    PRINT 7711,IADFCD(IFIT,ITRK),ITRK
 7711               FORMAT(' DOIT:DUFIND:Cathode hit',I5,
     2                   ' belongs to track',I3)
                  ELSE
                    PRINT 7712,IADFCD(IFIT,ITRK),ITRK
 7712               FORMAT(' DOIT:DUFIND:Cathode hit',I5,
     2                   ' belongs to track',I3,
     3                   ' (hopefully!)')
c23456789 123456789 123456789 123456789 123456789 123456789 123456789 12 
                  ENDIF
                ELSE
                  
                  IF(TRK_HAS_CATH.EQ.1)THEN
                    PRINT 7713,IADFCD(IFIT,ITRK),ITRK,
     2                   CATH_USED(IADFCD(IFIT,ITRK))
 7713               FORMAT(' DOIT:DUFIND:Cathode hit',I5,
     2                   ' belongs to track',I3,
     3                   '             (and to track',I3,')')
                  ELSE
                    PRINT 7714,IADFCD(IFIT,ITRK),ITRK,
     2                   CATH_USED(IADFCD(IFIT,ITRK))
 7714               FORMAT(' DOIT:DUFIND:Cathode hit',I5,
     2                   ' belongs to track',I3,
     3                   ' (hopefully!)(and to track',I3,')')
                  ENDIF
                ENDIF
              ELSEIF(
     1               IDVCTF(IABS(IPLRCD(IFIT,ITRK))).EQ.
     2               IDRFT)THEN
                IWIRE=IWIRCD(IABS(IADFCD(IFIT,ITRK)))
                IWADR=INDXTF(IABS(IPLRCD(IFIT,ITRK)))+IWIRE
                IF(IWADR_USED(IWADR).EQ.0)THEN
                  IWADR_USED(IWADR)=ITRK
                  PRINT 7721,IPLRCD(IFIT,ITRK),IWIRE,ITRK
 7721             FORMAT(' DOIT:DUFIND:Anode L',I3,' w',I4,
     2                 ' belongs to track',I3)
                  
c23456789 123456789 123456789 123456789 123456789 123456789 123456789 12 
                ELSE
                  PRINT 7723,IPLRCD(IFIT,ITRK),IWIRE,ITRK,
     2                 IWADR_USED(IWADR)
 7723             FORMAT(' DOIT:DUFIND:Anode L',I3,' w',I4,
     2                 ' belongs to track',I3,
     3                 '             (and to track',I3,')')
                ENDIF
              ENDIF
 729        CONTINUE
          ELSE
            PRINT 7002,ITRK
 7002       FORMAT(' DOIT:DUFIND:',
     2           'pretty weird, no hits in track',I3)
          ENDIF
 749    CONTINUE
      ENDIF
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

