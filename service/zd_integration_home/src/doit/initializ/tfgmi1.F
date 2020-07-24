*
* $Id: tfgmi1.F,v 1.6 2003/03/05 18:06:51 dpp Exp $
*
* $Log: tfgmi1.F,v $
* Revision 1.6  2003/03/05 18:06:51  dpp
*      -> changed all variable names in cdscrtcd to have common root, comment
*      -> remove obsolete comments
*
* Revision 1.5  2002/09/05 19:11:56  dpp
*      -> diagnostics
*      -> protection against null devices
*
* Revision 1.4  2000/08/18 18:30:15  dpp
*      -> expanded diagnostics
*
* Revision 1.3  2000/04/26 15:06:57  dpp
*      -> fill in silicon layer radius from svgeometry.inc
*
* Revision 1.2  1998/07/09 00:51:46  lyon
* seq/cdgeom -> cl3seq/cdgm3
*
* Revision 1.1.1.1  1998/02/06 19:11:44  dpp
* DOIT first release.
*
* Revision 1.2  1995/09/25 19:44:41  zfiles
* print statements
*
* Revision 1.1.1.1  1994/10/08  01:00:55  zfiles
* first version of duet in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  5.54/14 16/07/92  16.28.03  by  Dan Peterson
*CMZ :  5.54/03 15/05/91  16.26.15  by  Steve Schaffner
*CMZ :  5.53/05 12/07/90  14.36.00  by  Steve Schaffner
*CMZ :  5.53/03 16/05/90  17.29.55  by  Steve Schaffner
*CMZ :  5.53/01 12/04/90  17.18.48  by  Steve Schaffner
*CMZ :  5.52/00 11/01/90  22.40.19  by  Steve Schaffner
*CMZ :  5.51/03 27/10/89  11.32.10  by  Steve Schaffner
*CMZ :  5.51/01 06/09/89  18.03.10  by  Steve Schaffner
*CMZ :  5.51/00 11/08/89  14.22.54  by  Steve Schaffner
*CMZ :          12/06/89  15.47.10  by  Steve Schaffner
*Correct for fact that DIFSCD is now loaded with 999.99 for cathodes
*-- Author :
      SUBROUTINE TFGMI1
C.......................................................................
C.
C. TFGMI1 - Job start Duet geometry initialization.
C.
C. COMMON    : CDGEOC CDGEOI CDGEOL CDGEOR TFGEOI TFGEOL TFGEOR
C. CALLS     : DOIT_LAYER_ORDER VZERO
C. CALLED    : Various places
C. AUTHOR    : R. Namjoshi
C.
C. VERSION   : 1.00
C. CREATED   : 02-Sep-86
C. LAST MOD  : 23-Apr-87
C.
C. Modification Log.
C.
C. 10-Apr-87 : Switch sign of BSAGTF since sign of BSAGCD changed. When
C.             and why BSAGCD was changed is not known. R. Namjoshi
C.
C. 23-Apr-87 : Correct ZEEFTF and ANGSTF even though not used.
C.
C. 16-May-89 : Convert to new geometry regime (add copying of NLYRCD etc,
C.             remove call to CDGMIN )
C. 12-Jun-89 : NLYRCD etc. moved to TFGMI0.
C. 12-Jul-90 : Correct for cathode effective radius here.
C. 16-Oct-91 : Update LQDVTF depending on result of LUSEQD.
C.......................................................................
C
C    ******************************************************************
C    *                                                                *
C    *    This is the stage 1 version, copy from CD commons to        *
C    *    TF commons in Duet non-radial order.                        *
C    *                                                                *
C    ******************************************************************
C
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "cl3seq/cdgm3/cdgeomcd.inc"
#include "doit/duseq/cfctlcde.inc"
#include "doit/duseq/lfctlcde.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/svgeometry.inc"


C--> Local variables
      INTEGER IDEV,IDLYR,ILCD,ILTF,JLYR,ILLF,I,NSCAT
      INTEGER NWIRTOT
      REAL RRMAX,RRDIF
      CHARACTER*4 LABEL
* ----------Executable code starts here---------------------

C--> fill the TF layer order

      CALL DOIT_LAYER_ORDER

C--> set the scatter positions; they must not be in z-files

      CALL DOIT_CD_SCATTERS

C--> Copy from CD to TF arrays

      CALL VZERO( ILCRTF, KLYRTF )
      NWIRTOT=0

C--> loop over devices, then layers in device

      DO 10 IDEV = 1, MXDVCD

#if defined (CLEO_DPPDIAG)
      PRINT 3001,IDEV,DEVNAM(IDEV),
     2       NLDVCD(IDEV),IDTOCD(IDEV),
     3       ITDVCD(IDEV)
3001  FORMAT(' TFGMI1:dev',I3,' testing(cdgm name) ',A4,
     2     ' has',I3,' layers, IDTOCD=',I3,
     3     ' (cd type) ',I3)
#endif

      IF(
     1     (NLDVCD(IDEV).GT.0).AND.
     2     (IDTOCD(IDEV).GT.0))THEN

          DO 20 IDLYR  = 1,NLDVCD(IDEV)
              ILCD         = IXLRCD(IDLYR,IDEV)
              ILTF         = IXLRTF(IDLYR,IDEV)
              IDVCTF(ILTF) = IDVCCD(ILCD)

#if defined (CLEO_DPPDIAG)
              IF(
     2             (IDVCTF(ILTF).EQ.ISVX1).OR.
     3             (IDVCTF(ILTF).EQ.ISVZ1))THEN
                LABEL='SVX '
              ELSEIF(
     2             (IDVCTF(ILTF).EQ.IDRCT).OR.
     3             (IDVCTF(ILTF).EQ.IVDCT).OR.
     3             (IDVCTF(ILTF).EQ.IPTLC))THEN
                 LABEL='cath'
              ELSE
                 LABEL='WIRE'
              ENDIF

              PRINT 3011,IDLYR,
     2             ILCD,ILTF,LABEL,
     3             IDVCCD(ILCD),ICELCD(ILCD)
 3011         FORMAT(' TFGMI1: layer (local)',I4,
     2             ' (CD)',I4,' (TF)',I4,' local type=',A4,
     3             ' device code',I3,' cell type',I3)
#endif

              IXSFTF(ILTF) = IDLYR
              ILCDTF(ILCD) = ILTF
              ILTFCD(ILTF) = ILCD

C special pointers for silicon
              IF(    (IDVCTF(ILTF).EQ.ISVX1))THEN
                 IL_TF_SV_F(ILTF)=IDLYR
                 IL_TF_SV_Z(ILTF)=0
                 IL_SV_F_TF(IDLYR)=ILTF
                 IL_SV_Z_TF(IDLYR)=0
              ELSEIF((IDVCTF(ILTF).EQ.ISVZ1))THEN
                 IL_TF_SV_F(ILTF)=0
                 IL_TF_SV_Z(ILTF)=IDLYR
                 IL_SV_F_TF(IDLYR)=0
                 IL_SV_Z_TF(IDLYR)=ILTF
              ELSE
                 IL_TF_SV_F(ILTF)=0
                 IL_TF_SV_Z(ILTF)=0
                 IL_SV_F_TF(IDLYR)=0
                 IL_SV_Z_TF(IDLYR)=0
              ENDIF

              INDVTF(ILTF) = INDVCD(ILCD)
              INDXTF(ILTF) = INDXCD(ILCD)
              NWIRTF(ILTF) = NWIRCD(ILCD)
              NWRZTF(ILTF) = NWRZCD(ILCD)

              NWIRTOT=NWIRTOT+NWIRCD(ILCD)

#if defined (CLEO_DPPDIAG)
                 PRINT 3012,
     2             INDVCD(ILCD),INDXCD(ILCD),
     3             NWIRCD(ILCD),NWRZCD(ILCD),NWIRTOT
 3012            FORMAT('         ',
     2                ' INDVCD,INDXCD=',2I8, 
     2                ' NWIRCD,NWRZCD=',2I6,
     2                ' total cells=',I7) 
#endif


C silicon: radius is average radius for projection and locating hits in road
C          from the silicon data base access common block

              IF(
     2             (IDVCTF(ILTF).EQ.ISVX1).OR.
     3             (IDVCTF(ILTF).EQ.ISVZ1))THEN
                 RTF(ILTF)=SV_RADIUS(IDLYR)
                 RSQTF(ILTF) = RTF(ILTF)**2
              ELSE

                 IF( CTOACD(ILCD) .EQ. 0 ) THEN
                    RTF(ILTF)   = RCD(ILCD)
                    RSQTF(ILTF) = RSQCD(ILCD)
                 ELSE
                    RTF(ILTF)   = RCD(CTOACD(ILCD))+REFFCD(ILCD)
                    RSQTF(ILTF) = RTF(ILTF)**2
                 ENDIF
              ENDIF

              ZENDTF(ILTF) = ZENDCD(ILCD)
              ZEEFTF(ILTF) = ZEEFCD(ILCD)
              RENDTF(ILTF) = RENDCD(ILCD)

#if defined (CLEO_DPPDIAG)
                 PRINT 3013,
     2             RTF(ILTF),
     3             ZENDCD(ILCD),ZEEFCD(ILCD),
     4             TANSCD(ILCD),RENDCD(ILCD),
     5             CTOACD(ILCD),REFFCD(ILCD)
 3013            FORMAT('         ',
     2                ' RTF=',F9.6,
     3                ' ZENDCD,ZEEFCD=',F9.6,F10.6,
     4                ' TANSCD,RENDCD=',F10.6,F10.6,
     5                ' CTOACD=',I2,' REFFCD=',F9.6) 
#endif

              CELLTF(ILTF) = CELLCD(ILCD)
              CELZTF(ILTF) = CELZCD(ILCD)

#if defined (CLEO_DPPDIAG)
                 PRINT 3014,CELLCD(ILCD),CELZCD(ILCD)
 3014            FORMAT('         ',' CELLCD,CELZCD=',2F11.6) 
#endif


              PHIFTF(ILTF) = PHIFCD(ILCD)
              DIFSTF(ILTF) = DIFSCD(ILCD)
              IF (DIFSTF(ILTF).GT.900.) DIFSTF(ILTF) = 0.0
              ANGSTF(ILTF) = ANGSCD(ILCD)

              LQDVTF(ILTF) = LQDVCD(ILCD).AND.LUSEQD
              WENDTF(ILTF) = WENDCD(ILCD)
              TANSTF(ILTF) = TANSCD(ILCD)

C              BSAGTF(ILTF) =-BSAGCD

C fill ILCRTF for cathode devices

              IF( IDEV.EQ.IVDCT .OR. IDEV.EQ.IDRCT ) THEN
                  RRMAX = 1.0E9
                  DO 30 JLYR = 1, NLYRTF
                      IF( JLYR.NE.ILTF ) THEN
                          RRDIF = ABS(RENDTF(JLYR)-RENDTF(ILTF))
                          IF( RRDIF.LT.RRMAX ) THEN
                              RRMAX = RRDIF
                              ILCRTF(ILTF) = JLYR
                          ENDIF
                      ENDIF
30                CONTINUE
              ENDIF

C end of loop over layers in device
20        CONTINUE

#if defined (CLEO_DPPDIAG)
      PRINT 3003,IDEV
3003  FORMAT(' TFGMI1: end of device ',I8)
#endif
      ENDIF

C end of loop over devices
10    CONTINUE

C fill ILCRTF for silicon devices

      DO 35 ILTF=1,NLYRTF
         IF(IDVCTF(ILTF).EQ.ISVX1)THEN
            DO 33 JLYR=1,NLYRTF
               IF(IDVCTF(JLYR).EQ.ISVZ1)THEN
                  IF(IL_TF_SV_F(ILTF).EQ.IL_TF_SV_Z(JLYR))THEN
                     ILCRTF(ILTF) = JLYR
                     ILCRTF(JLYR) = ILTF
                  ENDIF
               ENDIF
 33         CONTINUE
         ENDIF
 35   CONTINUE

C--> Copy scattering media information

      IKN6TF = 2
      IKN7TF = 3
      NSCAT  = 0
      LSCATF = 1
      CALL UCOPY(RSCACD,RSCATF,NSCATF)
      CALL UCOPY(XSCACD,XSCATF,NSCATF)
      DO 45 I = LSCATF, NSCATF
          IF (RSCACD(I).GT.0.0 .AND. XSCACD(I).GT.0.0) NSCAT=NSCAT+1
          
45    CONTINUE
      MSCATF = LSCATF + NSCAT - 1

      LSCCTF = 1
      MSCCTF = 0
      DO 50 I = LSCCTF, NSCCTF
          RSCCTF(I) = RSCCCD(I)
          XSCCTF(I) = XSCCCD(I)
          IF (RSCCCD(I).GT.0. .AND. XSCCCD(I).GT.0.)
     +      MSCCTF = MSCCTF + 1
50    CONTINUE

C--> Flag geometry initialization complete

      IFLGCF = IFGEOM
      IFLGLF = IFGEOM

      RETURN
      END





