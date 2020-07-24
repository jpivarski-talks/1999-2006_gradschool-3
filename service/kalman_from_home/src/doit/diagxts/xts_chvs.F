*
* $Id: xts_chvs.F,v 1.6 2000/05/26 16:50:44 dpp Exp $
*
* $Log: xts_chvs.F,v $
* Revision 1.6  2000/05/26 16:50:44  dpp
*      -> stereo wires now defined as (tan >.005)
*
* Revision 1.5  1999/01/29 15:34:37  dpp
* force update
*
* Revision 1.4  1998/07/21 15:02:38  lyon
* Protect against including cdscrtcd.inc twice
*
* Revision 1.3  1998/07/09 13:01:33  lyon
* Added include cdscrtcd.inc
*
* Revision 1.2  1998/07/09 01:19:35  lyon
* Retrofitted for sf_data_access.inc
*
* Revision 1.1.1.1  1998/02/06 19:11:41  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:23  clib
* Developmental version of DUET.
*
* Revision 1.3  1997/02/25 21:35:49  dpp
* use new XTS number system
*
* Revision 1.2  1996/02/14 20:31:41  zfiles
* removes call to SFLWF; uses SFLSFA etc.
*
* Revision 1.1.1.1  1994/10/08 01:00:59  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  6.00/18 22/03/94  14.42.22  by  Dan Peterson
*CMZ :  3.01/01 23/03/90  12.38.41  by  Dan Peterson
*-- Author : dpp
      SUBROUTINE XTS_CHVIEW_SIMPLE(ICHAIN,IBRITE,CALLER)
      IMPLICIT NONE
      SAVE
C this will include tfindpar, cdgeompa, cdgeomcd, cdraw1in,
C                   tfctlcde, usbank,
C                   sfpar, sfcom, and sfsvcom, and cdscrtcd (if nozfiles)
#define SF_DATA_INCLUDES
#include "doit/sf_data_access/sf_data_access.inc"
C----------

#include "doit/sfseq/sfxtsp.inc"
#include "doit/sfseq/sfxtsc.inc"

#ifndef CLEO_SFNOZFILES
#include "doit/duseq/cdscrtcd.inc"
#endif

#define SF_DATA_DECLARE
#include "doit/sf_data_access/sf_data_access.inc"

C IADRI..SF ADDRESS OF CHAIN ELEMENT
C IADRSF.SF ADDRESS OF CURRENT CELL, CHAIN ELEMENTS AND DOUBLES
C IADRCD.CD ADDRESS OF IADRSF
C IADRXT.XTS ADDRESS OF IADRSF
C IH.....HIT NUMBER IN ZFILES
C IWIR...WIRE NUMBER IN LAYER, SF OR CD
C ILYRSF.SF LAYER NUMBER
C ILYRCD.CD LAYER NUMBER

      INTEGER IBRITE,ICHAIN
      CHARACTER*(*) CALLER

      INTEGER IADRI, IADRSF,IADRCD,IADRXT
      INTEGER IH,    IWIR,  ILYRSF,ILYRCD
      INTEGER RAWTDC

#if defined(CLEO_XTSDEBUG)
      PRINT 1001,ICHAIN,IBRITE,CALLER
1001  FORMAT(' XTS_CHVIEW_SIMPLE: ENTER, ICHAIN=',I2,
     1       ' IBRITE=',I3,'  CALLED BY',A20)
#endif

      SHOWCH(ICHAIN)=IBRITE
      NSEND=0

C loop over hits in chain
      IADRI=LINCHN(ICHAIN)
105   IF(IADRI.LE.0)GO TO 511
      IADRSF=IADRI

C continue with loop
107   ILYRSF=SFLSFA(IADRSF)
      IWIR=SFWSFA(IADRSF)

      ILYRCD=CDLASF(ILYRSF)
      IADRCD=0
      IF(ILYRCD.GT.0)
     2IADRCD=IADRSF-INSFSF(ILYRSF)+INCDSF(ILYRSF)

      IADRXT=IADRSF-INSFSF(ILYRSF)+INXSSF(ILYRSF)

      NSEND=NSEND+1
      IDHIT(NSEND)=IADRXT


C kill VD EAST
      IF((ILYRCD.GE.LCANVD).AND.(ILYRCD.LE.MCANVD))THEN
         IF(IWIR.GE.NWIRCD(ILYRCD))GO TO 401
      ENDIF

C flag hits already used by DUET
      IF(IBRITE.EQ.0)THEN
         COLRHT(NSEND)=CLUSED
         IF(IADRCD.GT.0)THEN
            IF(IADRHW(IADRCD).LT.0)COLRHT(NSEND)=CLDUSE
         ENDIF
         GO TO 401
      ELSEIF(IBRITE.EQ.3)THEN
         COLRHT(NSEND)=CLCHAN
         IF(IADRCD.GT.0)THEN
            IF(IADRHW(IADRCD).LT.0)COLRHT(NSEND)=CLDUCH
         ENDIF
         GO TO 401
      ENDIF

C restore the default colors
C default starting color for good hits
      COLRHT(NSEND)=CLNORM

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
C different color for DR stereo AND alternate side hits
      IF(ILYRCD.LT.0)THEN
            IF(IWIR  .GE.SFSVCL(-ILYRCD))COLRHT(NSEND)=CLSTRO
      ELSE
            IF(ILYRCD.GE.LCANDR.AND.ILYRCD.LE.MCANDR)THEN
                IF(ABS(TANSCD(ILYRCD)).GT.0.005)COLRHT(NSEND)=CLSTRO
            ENDIF
      ENDIF


C diagnostic; "BLAK" for very high time
      IF(IADRCD.GT.0)THEN

      sf_data_input_sf_adr=IADRSF
#define SF_DATA_MAPV
#include "doit/sf_data_access/sf_data_access.inc"
      IH=sf_data_output_hit_adr

      sf_data_input_hit_adr=IH
#define SF_DATA_RTDC
#include "doit/sf_data_access/sf_data_access.inc"
      RAWTDC=sf_data_output_tdc_raw

C.........IF(RAWTDC.GT.2800)COLRHT(NSEND)=CLBLAK
      ENDIF

C hit quality in the SF arrays; these hits are all SF layers
C LABEL HITS THAT HAVE HIGH TIME, NOT USED FOR SSF
      IF(INUCHN(IADRSF).EQ.-1)COLRHT(NSEND)=CLHITM
C DULLER FOR HIGH (LIKE PEDISTAL) PH HITS
      IF(INUCHN(IADRSF).EQ.-2)COLRHT(NSEND)=CLHADC
C LABEL HITS USED BY DUET
      IF(INUCHN(IADRSF).EQ.-3)COLRHT(NSEND)=CLDUSE
      IF(INUCHN(IADRSF).EQ.-4)COLRHT(NSEND)=CLDADJ



401   IADRSF=DOUBPT(IADRSF)
      IF(IADRSF.NE.0)GO TO 107
      IF(IADRI.EQ.MINCHN(ICHAIN))GO TO 511
      IADRI=ISTFOR(IADRI)
      GO TO 105


511   IF(NSEND.LE.0)GO TO 519
      CALL XTS_FILL('XTS_CHVIEW_SIMPLE')
519   CONTINUE
      CALL UPDATE_C


#if defined(CLEO_XTSDEBUG)
      PRINT 1003
1003  FORMAT(' XTS_CHVIEW_SIMPLE: EXIT')
#endif

      RETURN
      END















