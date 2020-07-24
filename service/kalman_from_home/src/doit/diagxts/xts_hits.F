*
* $Id: xts_hits.F,v 1.6 2000/05/26 16:51:37 dpp Exp $
*
* $Log: xts_hits.F,v $
* Revision 1.6  2000/05/26 16:51:37  dpp
*      -> stereo wires now defined as (tan >.005)
*
* Revision 1.5  1999/10/08 13:41:35  lyon
* Fixed SunOS directives and fixed CALLs to functions
*
* Revision 1.4  1999/01/29 15:34:39  dpp
* force update
*
* Revision 1.3  1999/01/11 02:47:37  lyon
* Added CLEO_SUNOS for the COLOR variable
*
* Revision 1.2  1998/07/09 01:20:14  lyon
* Retrofitted for sf_data_access.inc
*
* Revision 1.1.1.1  1998/02/06 19:11:41  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:23  clib
* Developmental version of DUET.
*
* Revision 1.3  1997/02/25 21:38:25  dpp
* also display SF layers that are not CD layers
*
* Revision 1.2  1995/04/20 22:38:56  zfiles
* See the comments for 6.00/39 in TITLE.doit
*
* Revision 1.1.1.1  1994/10/08  01:00:59  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  6.00/18 22/03/94  14.43.24  by  Dan Peterson
*CMZ :  3.01/01 23/03/90  12.38.40  by  Dan Peterson
*-- Author :dpp
      SUBROUTINE XTS_HITS(NINFO,CALLER)
      IMPLICIT NONE
      SAVE

C this will include tfindpar, cdgeompa, cdgeomcd, cdraw1in,
C                   tfctlcde, usbank,
C                   sfpar, sfcom, and sfsvcom
#define SF_DATA_INCLUDES
#include "doit/sf_data_access/sf_data_access.inc"
C----------

#include "doit/sfseq/sfxtsp.inc"
#include "doit/sfseq/sfxtsc.inc"

#define SF_DATA_DECLARE
#include "doit/sf_data_access/sf_data_access.inc"

      INTEGER ADRXT,ADRCD,ADRSF,LYRCD,LYRSF
      INTEGER MORELA,IH,IGO,IWIR,IWIRWE
      INTEGER NINFO
      INTEGER RAWTDC
      CHARACTER*(*)CALLER

#if defined(CLEO_ULTRIX)
      INTEGER COLOR
#else
C*****Alpha and SunOS
      INTEGER*8 COLOR
#endif




#if defined(CLEO_XTSDEBUG)
      PRINT 1001,NINFO,CALLER
1001  FORMAT(' XTS_HITS: ENTER: INFO#=',I3,'  CALLED FROM ',A20)
#endif

C fill in cells with colors
      NSEND=0

C loop over CD layers.....
      IGO=1
      LYRCD=LCANCD-1
105   IF(LYRCD.EQ.MCANCD)GO TO 129
      LYRCD=LYRCD+1

#if defined(CLEO_XTSDEBUG)
      PRINT 1003,LYRCD
1003  FORMAT(' XTS_HITS: PROCESSING LYRCD=',I10)
#endif

C default starting color for good hits
      COLOR=CLNORM

C different color for DR stereo
      IF(LYRCD.GE.LCANDR.AND.LYRCD.LE.MCANDR)THEN
          IF(ABS(TANSCD(LYRCD)).GT.0.005)COLOR=CLSTRO
      ENDIF

C loop over hits in the layer

      sf_data_input_layr_cd=LYRCD
#define SF_DATA_1STH_CD
#include "doit/sf_data_access/sf_data_access.inc"
      IH=sf_data_output_hit_adr

121   IF(IH.EQ.0)GO TO 127
      IF(NSEND.EQ.MSEND)GO TO 301
123   CONTINUE

      sf_data_input_hit_adr=IH
#define SF_DATA_WIRN
#include "doit/sf_data_access/sf_data_access.inc"
      IWIR=sf_data_output_wire

C kill VD east
      IF(IWIR.GE.NWIRCD(LYRCD))GO TO 125

C create a new hit
      ADRCD=INDXCD(LYRCD)+IWIR
      ADRXT=INXSCD(LYRCD)+IWIR

      NSEND=NSEND+1
      IDHIT(NSEND)=ADRXT
      COLRHT(NSEND)=COLOR

C alternate hit colors

C-"BLAK" for very high time

      sf_data_input_hit_adr=IH
#define SF_DATA_RTDC
#include "doit/sf_data_access/sf_data_access.inc"
      RAWTDC=sf_data_output_tdc_raw

c temp     IF(RAWTDC.GT.2800)COLRHT(NSEND)=CLBLAK

C-hit quality in the SF arrays; skip if not SF layer
      LYRSF=SFLACD(LYRCD)
      IF(LYRSF.EQ.0)GO TO 124
      ADRSF=ADRCD-INCDSF(LYRSF)+INSFSF(LYRSF)

C-label hits that have high time, not used for SSF
      IF(INUCHN(ADRSF).EQ.-1)COLRHT(NSEND)=CLHITM

C-label high pH hits (like pedistal)
      IF(INUCHN(ADRSF).EQ.-2)COLRHT(NSEND)=CLHADC

C-label hits used by DUET
      IF(INUCHN(ADRSF).EQ.-3)COLRHT(NSEND)=CLDUSE
      IF(INUCHN(ADRSF).EQ.-4)COLRHT(NSEND)=CLDADJ
124   CONTINUE

C get next hit
125   CONTINUE

      sf_data_input_hit_adr=IH
#define SF_DATA_NXTH
#include "doit/sf_data_access/sf_data_access.inc"
      IH=sf_data_output_hit_adr

      GO TO 121
C end of loop over hits in layer
127   CONTINUE
      GO TO 105
C end of loop over layer
129   CONTINUE


C whole other loop for non-CD SF layers.....
      IGO=2
      LYRSF=0
205   IF(LYRSF.EQ.NLAYSF)GO TO 229
      LYRSF=LYRSF+1
      LYRCD=CDLASF(LYRSF)
      IF(LYRCD.GT.0)GO TO 227

#if defined(CLEO_XTSDEBUG)
      PRINT 1005,LYRSF
1005  FORMAT(' XTS_HITS: PROCESSING LYRSF=',I10)
#endif


C loop over hits in the layer
#if defined(CLEO_XTSDEBUG)
      PRINT 1006,LYRCD,SFSV1S(-LYRCD)
1006  FORMAT(' XTS_HITS:    SFSV1S(LYRCD=',I6,')=',I6)
#endif
      IH=SFSV1S(-LYRCD)
221   IF(IH.EQ.0)GO TO 227
      IF(NSEND.EQ.MSEND)GO TO 301
223   CONTINUE
      IWIR  =IH-SFSVIN(-LYRCD,1)
      IWIRWE=IH-SFSVIN(-LYRCD,SFSVWE)

C kill the non displayed side
c      IF(IWIRWE.LT.0             )GO TO  225
c      IF(IWIRWE.GE.SFSVCL(-LYRCD))GO TO  225

C default starting color for good hits
      COLOR=CLNORM
C alternate hit colors
      IF(IWIR  .GE.SFSVCL(-LYRCD))COLOR=CLSTRO

C create a new hit
      ADRSF=INSFSF(LYRSF)+IWIR
      ADRXT=INXSSF(LYRSF)+IWIR

      NSEND=NSEND+1
      IDHIT(NSEND)=ADRXT
      COLRHT(NSEND)=COLOR


C get next hit
225   CONTINUE
#if defined(CLEO_XTSDEBUG)
      PRINT 1007,LYRCD,IH,SFSVNX(IH)
1007  FORMAT(' XTS_HITS: LYRCD=',I3,' SFSVNX(',I6,')=',I6)
#endif
      IH=SFSVNX(IH)
      GO TO 221
C end of loop over hits in layer
227   CONTINUE
      GO TO 205
C end of loop over layer
229   CONTINUE



      IGO=0
301   IF(NSEND.EQ.0)GO TO 319
      IF(NINFO.NE.0)GO TO 305
         CALL XTS_FILL('XTS_HITS')
         GO TO 307
305      CALL XTS_INFO(NINFO,'XTS_HITS')
307   NSEND=0
      IF(IGO.EQ.1)GO TO 123
      IF(IGO.EQ.2)GO TO 223
319   CALL UPDATE_C



#if defined(CLEO_XTSDEBUG)
      PRINT 1002,NINFO
1002  FORMAT(' XTS_HITS: EXIT: INFO#=',I2,'  UPDATE_C CALLED')
#endif

      RETURN
      END













