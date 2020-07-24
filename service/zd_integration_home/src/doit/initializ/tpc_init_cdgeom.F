*
* $Id: tpc_init_cdgeom.F,v 1.1 2003/03/05 18:05:08 dpp Exp $
*
* $Log: tpc_init_cdgeom.F,v $
* Revision 1.1  2003/03/05 18:05:08  dpp
*      -> NEW ROUTINE
*      -> initialize cdgeom from LCD info amd other user supplied info
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author :
      SUBROUTINE TPC_INIT_CDGEOM
C.......................................................................
C.
C. TPC_INIT_CDGEOM - Initialize quantities for TPC trackfinding
C.
C. COMMON    : 
C. CALLS     : TPC_INIT_FROM_LCD  TPC_INIT_DET_RESPONSE
C. CALLED    :
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.00
C. CREATED   : 29-Oct-2002
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

#include "doit/duseq/tfconspa.inc"
#include "doit/duseq/tfindpar.inc"
C which includes 
C               #include "cl3seq/cdgm3/cdgeompa.inc"
C               #include "doit/duseq/tfgeompa.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/tfunitcd.inc"
#include "cl3seq/cdgm3/cdgeomcd.inc"

      real ret_bfield,ret_IR,ret_or,ret_haflen
      integer ret_nlay 

      real ret_nominal_cell_wid,ret_cell_z_width
      integer ret_ncell_devis 

      INTEGER ILCD,NCELLSUM,DEVICE,ILDV,NAXIAL,NANODE
      REAL NCELL_TEMP


c23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
* ----------Executable code starts here---------------------------------

      print 1000
 1000 FORMAT(' TPC_INIT_CDGEOM: enter')
      



      CALL TPC_INIT_FROM_LCD(
     2     ret_bfield,ret_IR,ret_or,ret_haflen,ret_nlay) 
      
      CALL TPC_INIT_DET_RESPONSE(
     2     ret_nominal_cell_wid,ret_cell_z_width,ret_ncell_devis)




C-----------------------------------------------------------------------
C fill CD geom
C-----------------------------------------------------------------------
C IDFRCD is used in XTS_GEOM
C IDTOCD is used in CDCORF, CDWTQD, DOIT_LAYER_ORDER, TFGMI0 , TFGMI1, TFXFIN
C IDTOCD is dimensioned (MXDVCD=16); there are 10 devices used in the 
C       cleo3 cdgeompa, so there is room for the TPC

      CALL VZERO(IDTOCD,MXDVCD)


C create the TPC
      
      IDTOCD(ITPC1)=1
      IDFRCD(IDTOCD(ITPC1))=ITPC1

      NLDVCD(ITPC1)=ret_nlay
      NDVLCD(IDTOCD(ITPC1))=NLDVCD(ITPC1)

      ILDVCD(1,ITPC1)=1
      ILDVCD(2,ITPC1)=ILDVCD(1,ITPC1)-1+NLDVCD(ITPC1)
      IDVLCD(1,IDTOCD(ITPC1))=ILDVCD(1,ITPC1)
      IDVLCD(2,IDTOCD(ITPC1))=ILDVCD(2,ITPC1)

C wait, must calculate number of cells in each layer
C 5mm wide cells
C along the way, calculate IDVCCD, IXSFCD, RCD, NWIRCD

      ILCD=0
      NCELLSUM=0

      DEVICE=IDFRCD(1)
      DO 19 ILDV=ILDVCD(1,DEVICE),ILDVCD(2,DEVICE)
        ILCD=ILCD+1
        IDVCCD(ILCD)=DEVICE
        IXSFCD(ILCD)=ILDV
        IXLRCD(ILDV,DEVICE)=ILCD
        RCD(ILCD)=ret_IR+(ret_or-ret_IR)*(ILDV-1)/(NLDVCD(DEVICE)-1)
        ncell_temp=(RCD(ILCD)*TWOPI/ret_nominal_cell_wid)
     2       *ret_ncell_devis + ret_ncell_devis - .5
        NWIRCD(ILCD)=ncell_temp/ret_ncell_devis
        INDVCD(ILCD)=NCELLSUM+1
        NCELLSUM=NCELLSUM+NWIRCD(ILCD)
 19   CONTINUE
        
      print 1002,NCELLSUM,KWIRCD
 1002 FORMAT(' TPC_INIT_CDGEOM: NCELLSUM=',I10,'  KWIRCD=',I10)

      IF(NCELLSUM.GT.KWIRCD)THEN
        print 1003
 1003   FORMAT(' TPC_INIT_CDGEOM: you must increase KWIRCD')

      ELSE
      
        NWDVCD(DEVICE)=NCELLSUM
        NDVWCD(IDTOCD(DEVICE))=NWDVCD(DEVICE)
        
        LCDVCD(DEVICE)=.FALSE.
        LDVCCD(IDTOCD(DEVICE))=LCDVCD(DEVICE)
        
        ITDVCD(DEVICE)=1
        IDVTCD(IDTOCD(DEVICE))=ITDVCD(DEVICE)
        
        ILDTCD(1,ANODCD)=ILDVCD(1,DEVICE)
        ILDTCD(2,ANODCD)=ILDVCD(2,DEVICE)
        
C NWDTCD is not used
        
        NLDTCD(ANODCD)=NLDVCD(DEVICE)
        
C IDBQCD is not used
        
        NAXLCD=NLDVCD(DEVICE)
        NSTRCD=0
        NLYRCD=NLDVCD(DEVICE)
        NQDVCD=0
        MDEVCD=1
        MWIRCD=NCELLSUM
        
        
C ILBDCD is not used
C CDFIRP is not used
C CDLASP is not used
        
C DVNMCD is not used
C DTNMCD is not used
C CLTYCD is not used

C new loop over layers, now can loop on CD layers
        NCELLSUM=0
        NAXIAL=0
        NANODE=0
        DO 29 ILCD=1,NLYRCD
          INDXCD(ILCD)=NCELLSUM+1
          NCELLSUM=NCELLSUM+NWIRCD(ILCD)
          NWRZCD(ILCD)=1
          
          ICELCD(ILCD)=ICLTPC
          NAXIAL=NAXIAL+1
          NANODE=NANODE+1
          IAXLCD(NAXIAL)=ILCD
          ICANCD(NANODE)=ILCD
          
          ATOCCD(ILCD)=0
          CTOACD(ILCD)=0
          
C ILBSCD is not used
C NFLDCD is not used
          
          RSQCD(ILCD)=RCD(ILCD)*RCD(ILCD)
          REFFCD(ILCD)=0.
          TANSCD(ILCD)=0.
          RENDCD(ILCD)=RCD(ILCD)
          DIFSCD(ILCD)=0.
          ANGSCD(ILCD)=0.
          PHIFCD(ILCD)=0.
          CELLCD(ILCD)=TWOPI/NWIRCD(ILCD)
          ZEEFCD(ILCD)=0.
          CELZCD(ILCD)=ret_cell_z_width
          ZENDCD(ILCD)=ret_haflen
          XX0CD(1,ILCD)=0.
          XX0CD(2,ILCD)=0.
          XX0CD(3,ILCD)=0.
          UU0CD(1,ILCD)=0.
          UU0CD(2,ILCD)=0.
          UU0CD(3,ILCD)=0.
          BSAGCD(ILCD)=0.
          WENDCD(ILCD)=1.
          CELRCD(1,ILCD)=(ret_or-ret_IR)/ret_nlay
          CELRCD(2,ILCD)=CELRCD(1,ILCD)
          RDLFCD(ILCD)=0.
          WIDFCD(ILCD)=0.

          PRINT 2001,ILCD,INDXCD(ILCD),RCD(ILCD),NWIRCD(ILCD)
 2001     FORMAT(' TPC_INIT_CDGEOM:',I4,
     2         '  INDX=',I7,
     3         '  R=',F8.4,
     4         '  NWIRE=',I6)

 29     CONTINUE
        
        
        XXBMCD(1)=0.
        XXBMCD(2)=0.
        XXBMCD(3)=0.
        ERBMCD(1)=1.
        ERBMCD(2)=1.
        ERBMCD(3)=1.
        
C XXDVCD, UUDVCD, RTDVCD not used
        
        NSCTCD=0
      ENDIF 
        

      RETURN
      END







