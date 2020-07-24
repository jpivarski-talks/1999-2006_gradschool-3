*
* $Id: xts_chvs.F,v 1.8 2003/03/05 17:25:33 dpp Exp $
*
* $Log: xts_chvs.F,v $
* Revision 1.8  2003/03/05 17:25:33  dpp
*      -> new assignments for negative INUCHN
*      -> protect saturation of NSEND
*      -> remove zfiles stuff and use of SFNOZFILES switch
*      -> changed all variable names in cdscrtcd to have common root
*      -> use EvntHit_OpnHit to indicate usable hits
*
* Revision 1.7  2002/09/05 19:07:02  dpp
*      -> change name of local chain number; "ICHAIN" is now in common
*      -> does not use negative DOUBPT
*      -> diagnostics
*      -> F77
*
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
      SUBROUTINE XTS_CHVIEW_SIMPLE(LOCAL_CHAIN,IBRITE,CALLER)
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

#define SF_DATA_DECLARE
#include "doit/sf_data_access/sf_data_access.inc"


C arguments

      INTEGER IBRITE,LOCAL_CHAIN
      CHARACTER*(*) CALLER

C local variables

C SF_CellID_loop..SF ADDRESS OF CHAIN ELEMENT
C SF_CellID_cur...SF ADDRESS OF CURRENT CELL, CHAIN ELEMENTS AND DOUBLES
C CD_CellID.......CD ADDRESS OF SF_CellID_cur
C XTS_CellID......XTS ADDRESS OF SF_CellID_cur
      INTEGER SF_CellID_loop,SF_CellID_cur,CD_CellID,XTS_CellID
C IH..............HIT NUMBER IN ZFILES
C IWIR............WIRE NUMBER IN LAYER, SF OR CD
C LYRSF...........SF LAYER NUMBER
C LYRCD...........CD LAYER NUMBER
      INTEGER IH,    IWIR,  LYRSF,LYRCD
C HAS_OPEN_HIT....has an open, usable, hit in the time window
C HAS_USED_HIT....has a hit in the time window that is already used 
C HAS_SET_COLOR...has set the color with a hit on a chain
      LOGICAL HAS_OPEN_HIT,HAS_USED_HIT,HAS_SET_COLOR

      INTEGER RAWTDC
      REAL TIMER,ZPRED,ZCALC

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
C***********************************************************************
C executable code starts here
C***********************************************************************

#if defined(CLEO_XTSDEBUG)
      PRINT 1001,LOCAL_CHAIN,IBRITE,CALLER
 1001 FORMAT(' XTS_CHVIEW_SIMPLE: ENTER, CHAIN=',I2,
     1     ' IBRITE=',I3,'  CALLED BY',A20)
#endif
      
      SHOWCH(LOCAL_CHAIN)=IBRITE
      NSEND=0
      
C loop over hits in chain
      SF_CellID_loop=LINCHN(LOCAL_CHAIN)
 105  IF(SF_CellID_loop.GT.0)THEN
        SF_CellID_cur=SF_CellID_loop
        
C continue with loop
 107    LYRSF=SFLSFA(SF_CellID_cur)
        IWIR=SFWSFA(SF_CellID_cur)
        
        LYRCD=CDLASF(LYRSF)
        CD_CellID=0
        IF(LYRCD.GT.0)
     2       CD_CellID=SF_CellID_cur-INSFSF(LYRSF)+INCDSF(LYRSF)
        
        XTS_CellID=SF_CellID_cur-INSFSF(LYRSF)+INXSSF(LYRSF)
        
        IF(NSEND.EQ.MSEND)THEN
          CALL XTS_FILL('XTS_CHVIEW_SIMPLE')
          NSEND=0
        ENDIF
        
        NSEND=NSEND+1
        IDHIT(NSEND)=XTS_CellID
        
#if defined(CLEO_XTSDEBUG)
        PRINT 1004,CD_CellID,XTS_CellID,NSEND
 1004   FORMAT(' XTS_CHVIEW_SIMPLE: PROCESSING CD HIT:',
     1       '  CD_CellID=',I6,'  XTS_CellID=',I6,'  NSEND=',I4)
#endif
        
        
C kill VD EAST
        IF(
     1       (LYRCD.LT.LCANVD).OR.
     2       (LYRCD.GT.MCANVD).OR.
     3       (IWIR.LT.NWIRCD(LYRCD)))THEN
          
C-----------------------------------------------------------------------
C flag the current track
C flag hits already used by DOIT
C-----------------------------------------------------------------------
          HAS_SET_COLOR=.FALSE.
          
          IF(
     1         (IBRITE.EQ.0).OR.
     2         (IBRITE.EQ.3))THEN
            
            HAS_OPEN_HIT=.FALSE.
            HAS_USED_HIT=.FALSE.
            ZPRED=RCD(LYRCD)*SFZSLCNTR
            
#define SF_DATA_USE_ADDRESSED_MULTIHIT
#include "doit/sf_data_access/sf_data_access.inc"
            
            sf_data_input_sf_adr=SF_CellID_cur
#define SF_DATA_MAPV
#include "doit/sf_data_access/sf_data_access.inc"
            IH=sf_data_output_hit_adr

 201        CONTINUE
            IF(IH.NE.0)THEN
              
              sf_data_input_hit_adr=IH
#define SF_DATA_TMPS
#include "doit/sf_data_access/sf_data_access.inc"
              TIMER=sf_data_output_r_tim_ps
              
              ZCALC=TIMER*SFZSLVEL
              
              IF(ABS(ZCALC-ZPRED).LE.SFZSLWID)THEN
                IF(EvntHit_OpnHit(IH).GE.0)THEN
                  HAS_OPEN_HIT=.TRUE.
                ELSE
                  HAS_USED_HIT=.TRUE.
                ENDIF
              ENDIF
              
              IF(L_SF_DOIT_TPC)THEN
                sf_data_input_hit_adr=IH
#define SF_DATA_NEXTHIT_MULTIHIT
#include "doit/sf_data_access/sf_data_access.inc"
                IH=sf_data_output_next_mult
                GO TO 201
              ENDIF
            ENDIF
              
#define SF_DATA_USE_MULTIHIT_RESTORE
#include "doit/sf_data_access/sf_data_access.inc"
            
            IF(HAS_OPEN_HIT)THEN
              IF(IBRITE.EQ.3)THEN
                COLRHT(NSEND)=CLCHAN
              ELSEIF(IBRITE.EQ.0)THEN
                COLRHT(NSEND)=CLUSED
              ELSE
                COLRHT(NSEND)=CLUSED
              ENDIF
              HAS_SET_COLOR=.TRUE.
              
            ELSEIF((HAS_USED_HIT).AND.(.NOT.HAS_SET_COLOR))THEN
              IF(CD_CellID.GT.0)THEN
                IF(IBRITE.EQ.3)THEN
                  COLRHT(NSEND)=CLDUCH
                ELSEIF(IBRITE.EQ.0)THEN
                  COLRHT(NSEND)=CLDUSE
                ELSE
                  COLRHT(NSEND)=CLDUSE
                ENDIF
                HAS_SET_COLOR=.TRUE.
              ENDIF
            ENDIF
          ENDIF
            
C-----------------------------------------------------------------------
C restore the default colors
C default starting color for good hits
C-----------------------------------------------------------------------
          IF(.NOT.HAS_SET_COLOR)THEN
            COLRHT(NSEND)=CLNORM
            
C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
C different color for DR stereo AND alternate side hits
            IF(LYRCD.LT.0)THEN
              IF(IWIR  .GE.SFSVCL(-LYRCD))COLRHT(NSEND)=CLSTRO
            ELSE
              IF(LYRCD.GE.LCANDR.AND.LYRCD.LE.MCANDR)THEN
                IF(ABS(TANSCD(LYRCD)).GT.0.005)COLRHT(NSEND)=CLSTRO
              ENDIF
            ENDIF
            
            
C diagnostic; "BLAK" for very high time
            IF(CD_CellID.GT.0)THEN
              
              sf_data_input_sf_adr=SF_CellID_cur
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
C LABEL HITS THAT ARE NOT THE PRIMARY HIT OF A CLUSTER
            IF(INUCHN(SF_CellID_cur).EQ.-1)COLRHT(NSEND)=CLCLUS
C LABEL HITS THAT HAVE HIGH TIME, NOT USED FOR SSF
            IF(INUCHN(SF_CellID_cur).EQ.-2)COLRHT(NSEND)=CLHITM
            IF(INUCHN(SF_CellID_cur).EQ.-3)COLRHT(NSEND)=CLHITM
            IF(INUCHN(SF_CellID_cur).EQ.-4)COLRHT(NSEND)=CLHITM
            IF(INUCHN(SF_CellID_cur).EQ.-5)COLRHT(NSEND)=CLHITM
C DULLER FOR HIGH (LIKE PEDISTAL) PH HITS
            IF(INUCHN(SF_CellID_cur).EQ.-6)COLRHT(NSEND)=CLHADC
C LABEL HITS already USED BY DOIT
            IF(INUCHN(SF_CellID_cur).EQ.-7)COLRHT(NSEND)=CLDUSE
            IF(INUCHN(SF_CellID_cur).EQ.-8)COLRHT(NSEND)=CLDADJ
C LABEL HITS THAT ARE BELOW THE CLUSTERING THRESHOLD, SAME AS PH CUT
            IF(INUCHN(SF_CellID_cur).EQ.-9)COLRHT(NSEND)=CLHADC
            
          ENDIF
        ENDIF
        
C check for double hits and loop back for the next primary hit
        
        SF_CellID_cur=DOUBPT(SF_CellID_cur)
        IF(SF_CellID_cur.GT.0)GO TO 107
        IF(SF_CellID_loop.NE.MINCHN(LOCAL_CHAIN))THEN
          SF_CellID_loop=ISTFOR(SF_CellID_loop)
          GO TO 105
        ENDIF
      ENDIF
      
      IF(NSEND.GT.0)THEN
        CALL XTS_FILL('XTS_CHVIEW_SIMPLE')
      ENDIF
      CALL UPDATE_C
      
      
#if defined(CLEO_XTSDEBUG)
      PRINT 1003
 1003 FORMAT(' XTS_CHVIEW_SIMPLE: EXIT')
#endif
      
      RETURN
      END
      














