*
* $Id: testTPC.F,v 1.4 2003/05/12 16:07:04 dpp Exp $
*
* $Log: testTPC.F,v $
* Revision 1.4  2003/05/12 16:07:04  dpp
*      -> use CLEO_DOITTPC to dummy this routine in CLEO production
*
* Revision 1.3  2003/03/06 16:58:53  dpp
*      -> changed compile switch from CLEO_SUNOS to CLEO_SunOS
*
* Revision 1.2  2003/03/05 17:06:30  dpp
*      -> remove use of PWCD; PWCD can be used for  other derived values
*      -> changed all variable names in cdscrtcd to have common root
*      -> use EvntHit_OpnHit to indicate usable hits
*      -> use compile switch for the declaration of RAN
*
* Revision 1.1  2002/09/05 19:38:50  dpp
*      -> NEW ROUTINE
*      -> load in data for running on a TPC
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"

      SUBROUTINE TESTTPC
#if defined(CLEO_DOITTPC)
C.......................................................................
C.
C. TESTTPC - fill commons and run a test of a TPC detector
C.
C. COMMON    : 
C. CALLS     : 
C. CALLED    : 
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.
C. CREATED   : 11-Jun-2002
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
#include "doit/duseq/tfgeomcd.inc"
#include "cl3seq/cdgm3/cdgeomcd.inc"
#include "doit/duseq/cdscrtcd.inc"
#include "doit/sfseq/sfextra.inc"


      INTEGER DEVICE,ILDV,ILCD
      INTEGER NCELLSUM
      INTEGER NAXIAL,NANODE


      REAL PHIJET,PHIJETWID
      REAL D0JET,D0JETWID
      REAL Z0JET,Z0JETWID

      REAL THETADIPJET,THETADIPJETWID
      REAL DZJET,DZJETWID

      REAL TPCRES
      REAL PTRACKHOLD(20),PSUM,PPERP
      REAL CUTR,PHITR,D0TR
      REAL KAPTR,ALPTR,BETTR,GAMTR,XITR
      REAL Z0TR,DZTR,THETADIPTR
      REAL ARCFUN
      REAL XTRY,YTRY,PHITRY,ARCTRY,ZTRY
      REAL VEL
      INTEGER IER


      REAL PHIDIF,PHINOW,PHISMR
      INTEGER IADR
      INTEGER LASTHIT(KLYRCD)
      INTEGER ITRK,IDUM
      INTEGER WIRECALC
      INTEGER RANCNT
      REAL GSUM
      REAL RAN

C to compile in solaris, must be deleted in OSF
#if defined(CLEO_SunOS)
      REAL RAN(2)
#endif

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
* ----------Executable code starts here---------------------
C-----------------------------------------------------------------------
C change the doit control bits
C-----------------------------------------------------------------------
      NEXPT=32
C-----------------------------------------------------------------------
C changes to cdgeompa.inc
C-----------------------------------------------------------------------
C KWIRCD=160000  is used to build kws1cd , the dimension of EvntHit_MapDet
C KSSVX1=0       is used to build kws1cd , the dimension of EvntHit_MapDet
C KSSVZ1=0       is used to build kws1cd , the dimension of EvntHit_MapDet

C KCANCD=100     is used in tfgeompa to build KLYRTF
C KCSTCD=0       is used in tfgeompa to build KLYRTF
C KCATCD=0       is used in tfgeompa to build KLYRTF

C-----------------------------------------------------------------------
C list of cdgeompa.inc indicating used in doit
C-----------------------------------------------------------------------
* KDEVCD no.F
* MXDVCD     tfgmi1, ?bitcd
* MXDTCD no.F

* IDRFT      cdnewt, testTPC, xts_geom,  xts_trak,  xts_zhit, tf3fit, 
*            tfkink,  cdcorf,  cdcorh,  cfxyht,  lfxyht,  cfhita, 
*            scan_z_roads,  tfexer,  tfxfit,
*            doit_init_sigma,  doit_layer_order,  tfxfin,  
*            sfdefn,  sfinit    
* IDRCT      (+) dump_dufind, ctgmcd, tfxyht, cdhitc, cdfill, 
*            findhw, tfgmi1  
* ISVX1      (+) xts_tfxf, lfxyht, cdclus, roadhits, doit_cd_scatters,
*            tfgmi0,  
* ISVZ1      (+) boot_si_z_hits, roadhits_coord
* IIVDT      ( already listed )
* IVDET      (+) cdwtqd
* IVDCT      ( already listed )
* IPROP      ( already listed )
* IPTLA      (+) dusqsh 
* IPTLC      ( already listed )

* ANODCD     only testTPC
* CATHCD no
* SSTRCD no

* KWIRCD                                   no.F used to build KWS1CD

* NCANDR     testTPC, tfgmi2,  duhbok
* NCATDR     testTPC, 
* NCAXDR     testTPC, doit_init_sigma, doit_layer_order, tfgmi0
* NCSTDR     testTPC,  doit_layer_order, tfgmi0
* NSSVX1               doit_layer_order, tfgmi0
* NCANIZ     tfgmi0
* NCANIV     tfgmi0
* NCANVD     tfgmi0
* NCATVD     doit_layer_order
* NCANPT     tfgmi0
* NCATPT no

* KWIRDX - Number of DR axial   wires      no.F  used to build KWIRDR
* KWIRDS - Number of DR stereo  wires      no.F  used to build KWIRDR
* KWIRDR - Number of DR         wires      no.F   utilseq/cdcalin.inc
*                                            but cdcalin.inc is used only in
*                                                 cdcorf and cdcorh
* KSTPDR - Number of DR cathode strips     no.F  no.inc
* KSSVX1 - Number of SVX1 strips (R-Phi)   no.F  no.inc
* KSSVZ1 - Number of SVZ1 strips Z         no.F  no.inc
* KWS1CD                                   cdscrtvd
* KWR2VD                                   no.F  no.inc

* KCAXCD - max pos num   axial anode   layers    tfgeompa.inc, build KCANCD
* KCSTCD - max pos num  stereo anode   layers    tfgeompa.inc, build KCANCD
* KCANCD - max pos num         anode   layers    utilseq/cdcalin.inc,
*                                                              build KLYRCD
* KCATCD - max pos num         cathode layers    tfgeompa.inc, build KLYRCD
* KLYRCD - max pos num                 layers    testTPC, sfover, random_noise
*                                                sfextra.inc, sffix.inc, 
*                                                utilseq/cdcalin.inc
* KQDVCD - max pos num charge division layers    tfgeompa.inc
* KSTXCD - max pos num axial   strip   layers   no
* KSTZCD - max pos num Z       strip   layers   no
* KSTRCD - max pos num          strip  layers   no

* none of these cell codes below are used anywhere

* ICLTUB - Tubular cell
* ICLSQ1 - Square  cell DR1
* ICLSQ2 - Square  cell DR2
* ICLSQZ - Square  cell DR2 stereo
* ICLHEX - Hex     cell VD
* ICLS1A - Square  cell DR1 cathodes above (outer cathodes)
* ICLS1B - Square  cell DR1 cathodes below (inner cathodes)
* ICLS2A - Square  cell DR2 cathodes above
* ICLS2B - Square  cell DR2 cathodes below
* ICLHXA - Hex     cell VD  cathodes above
* ICLHXB - Hex     cell VD  cathodes below
* ICLHXB - Hex     cell VD  cathodes below
* ICLSTR - Cathode strip.
* ICLSST - Silicon strip
*
* KCBDCD                                                                 no
* KCINCD                                                                 no
* KCRUCD                                                                 no
* NSCACD - Max. number of discreet   scatterers (including beam pipe)    no
* NSCCDD - Max. number of continuous scatterers                          no
* KSCTCD - Max. number of scatterers                                     no
* KSCTVA - Max. number of variables in scattering data base              no 

* KPARVA - max number of variables in device parameter list              no
* KPADVA - max number of variables in PT anode                           no
* KPCDVA - max number of variables in PT cathode                         no
* KVADVA - max number of variables in VD anode                           no
* KVCDVA - max number of variables in VD cathode                         no 
* KDADVA - max number of variables in DR anode                           no
* KDCDVA - max number of variables in DR cathode                         no
* KSIDVA - max number of variables in silicon                            no
* KSIDPL - max number of planes per layer in silicon                     no


C-----------------------------------------------------------------------
C fill CD geom
C-----------------------------------------------------------------------
C IDFRCD is used in XTS_GEOM
C IDTOCD is used in CDCORF, CDWTQD, DOIT_LAYER_ORDER, TFGMI0 , TFGMI1, TFXFIN
C IDTOCD is dimensioned (MXDVCD=16); there are 10 devices used in the 
C       cleo3 cdgeompa, so there is room for the TPC

      CALL VZERO(IDTOCD,MXDVCD)
      IDTOCD(ITPC1)=1
      IDFRCD(IDTOCD(ITPC1))=ITPC1

      NLDVCD(ITPC1)=100
      NDVLCD(IDTOCD(ITPC1))=NLDVCD(ITPC1)

      ILDVCD(1,ITPC1)=1
      ILDVCD(2,ITPC1)=100
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
        IDVCCD(ILDV)=DEVICE
        IXSFCD(ILCD)=ILDV
        IXLRCD(ILDV,DEVICE)=ILCD
        RCD(ILCD)=.5+(ILDV-1)*.015
        NWIRCD(ILCD)=RCD(ILCD)*2.*pi/.005 + .5
        INDVCD(ILCD)=NCELLSUM+1
        NCELLSUM=NCELLSUM+NWIRCD(ILCD)
 19     CONTINUE

      NWDVCD(DEVICE)=NCELLSUM
      NDVWCD(IDTOCD(DEVICE))=NWDVCD(DEVICE)

      LCDVCD(DEVICE)=.FALSE.
      LDVCCD(IDTOCD(DEVICE))=LCDVCD(DEVICE)

      ITDVCD(DEVICE)=1
      IDVTCD(IDTOCD(DEVICE))=ITDVCD(DEVICE)

      ILDTCD(1,ANODCD)=1
      ILDTCD(2,ANODCD)=100

C NWDTCD is not used

      NLDTCD(ANODCD)=100

C IDBQCD is not used

      NAXLCD=100
      NSTRCD=0
      NLYRCD=100
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
        CELLCD(ILCD)=2.*pi/NWIRCD(ILCD)
        ZEEFCD(ILCD)=0.
        CELZCD(ILCD)=5.0
        ZENDCD(ILCD)=2.5
        XX0CD(1,ILCD)=0.
        XX0CD(2,ILCD)=0.
        XX0CD(3,ILCD)=0.
        UU0CD(1,ILCD)=0.
        UU0CD(2,ILCD)=0.
        UU0CD(3,ILCD)=0.
        BSAGCD(ILCD)=0.
        WENDCD(ILCD)=1.
        CELRCD(1,ILCD)=.015
        CELRCD(2,ILCD)=.015
        RDLFCD(ILCD)=0.
        WIDFCD(ILCD)=0.
 29   CONTINUE


      XXBMCD(1)=0.
      XXBMCD(2)=0.
      XXBMCD(3)=0.
      ERBMCD(1)=1.
      ERBMCD(2)=1.
      ERBMCD(3)=1.

C XXDVCD, UUDVCD, RTDVCD not used

      NSCTCD=0

C-----------------------------------------------------------------------
C changes to sfpar, sfxtsp
C-----------------------------------------------------------------------
C mhitsf=160000
C mlaysf=100
C mcd=160000


C-----------------------------------------------------------------------
C make a track
C-----------------------------------------------------------------------


      EvntHit_Num=0
      call vzero(EvntHit_1stLyr,klyrcd)
      call vzero(EvntHit_1stDev,mxdvcd)
      EvntHit_1stDev(ITPC1)=1

      PHIJET=.5
      PHIJETWID=.31
c   *1.414
      D0JET=0.
      D0JETWID=.02
      Z0JET=.05
      Z0JETWID=0.

C      DZJET=.5
C      DZJETWID=.3

      THETADIPJET=.5
      THETADIPJETWID=.31
c *1.414
      
      TPCRES=.0005

      PSUM=0.
      DO 219 ITRK=1,20
 201    CONTINUE
        PTRACKHOLD(ITRK)=RAN(IDUM)
        IF(PTRACKHOLD(ITRK).LT.0.01)GO TO 201
        PSUM=PSUM+PTRACKHOLD(ITRK)
 219  CONTINUE
      DO 229 ITRK=1,20
        PTRACKHOLD(ITRK)=PTRACKHOLD(ITRK)/PSUM*200.
 229  CONTINUE


      DO 259 ITRK=1,20
c      DO 259 ITRK=1,1
        
        PHITR=PHIJET+(RAN(IDUM)-.5)*PHIJETWID
        D0TR= D0JET+ (RAN(IDUM)-.5)*D0JETWID
        Z0TR= Z0JET+ (RAN(IDUM)-.5)*Z0JETWID

C        DZTR= DZJET+ (RAN(IDUM)-.5)*DZJETWID
        THETADIPTR= THETADIPJET+ (RAN(IDUM)-.5)*THETADIPJETWID

        IF(THETADIPTR.GT. PI)THEN
          THETADIPTR= 2.*PI-THETADIPTR
          PHITR=PHITR+PI
        ENDIF
        IF(THETADIPTR.LT.-PI)THEN
          THETADIPTR=-2.*PI+THETADIPTR
          PHITR=PHITR+PI
        ENDIF

        DZTR=TAN(THETADIPTR)

        IF(PHITR.LT.0.)   PHITR=PHITR+2.*PI
        IF(PHITR.GE.2.*PI)PHITR=PHITR-2.*PI

c        CUTR=(.01+RAN(IDUM)*.49)
        PPERP=PTRACKHOLD(ITRK)*COS(THETADIPTR)
        CUTR=.15/PPERP * 3.
        IF(RAN(IDUM).LT.0.5)CUTR=-1.*CUTR
        

        
        CALL CDKABG(
     1       CUTR,PHITR,D0TR,
     2       KAPTR,ALPTR,BETTR,GAMTR,XITR)
        DO 249 ILCD=1,100
          CALL TFPHTR(
     1         ALPTR,BETTR,KAPTR,GAMTR,0.,9.,
     2         RSQCD(ILCD),+1,
     3         XTRY,YTRY,PHITRY,IER)
          PRINT 1101,ILCD,RCD(ILCD),PHITRY
 1101     FORMAT(' TESTTPC: making hit, layer',I4,
     2         ' rad=',F7.3,' phi=',f7.3)
          
          IF(IER.EQ.0)THEN
            
            ARCTRY=ARCFUN(KAPTR,ALPTR,BETTR,XTRY,YTRY,XITR)
            ZTRY=Z0TR+ARCTRY*DZTR
            
            PHIDIF=PHITRY-PHIFCD(ILCD)
            IF(PHIDIF.LT.0.)   PHIDIF=PHIDIF+2.*PI
            IF(PHIDIF.GE.2.*PI)PHIDIF=PHIDIF-2.*PI
            WIRECALC=PHIDIF/CELLCD(ILCD)
            IADR=INDXCD(ILCD)+WIRECALC
            
            IF(EvntHit_MapDet(IADR).EQ.0)THEN
              IF(EvntHit_Num.LT.EvntHitMax)THEN

                EvntHit_Num=EvntHit_Num+1
                EvntHit_OpnHit(EvntHit_Num)=7
                EvntHit_CDlayr(EvntHit_Num)=ILCD
                EvntHit_laWire(EvntHit_Num)=WIRECALC
                EvntHit_MapDet(IADR)=EvntHit_Num
                
                print 1102,
     1               INDXCD(ILCD),EvntHit_laWire(EvntHit_Num),
     2               IADR,EvntHit_MapDet(IADR)
 1102           format(' TESTTPC:',
     1               ' indxcd=',I6,'  laWire=',I6,
     2               ' iadr=',I6,'  map=',I6) 

                PHINOW=PHIFCD(ILCD)
     2               +CELLCD(ILCD)*EvntHit_laWire(EvntHit_Num)
                IF(PHINOW.LT.0.)   PHINOW=PHINOW+2.*PI
                IF(PHINOW.GE.2.*PI)PHINOW=PHINOW-2.*PI

                GSUM=0.
                DO 231 RANCNT=1,12
                  GSUM=GSUM+RAN(IDUM)-.5
 231            CONTINUE
                PHISMR=PHITRY++TPCRES*GSUM
                IF(PHISMR.LT.0.)   PHISMR=PHISMR+2.*PI
                IF(PHISMR.GE.2.*PI)PHISMR=PHISMR-2.*PI
                
                EvntHit_X(EvntHit_Num)=COS(PHINOW)*RCD(ILCD)
                EvntHit_Y(EvntHit_Num)=SIN(PHINOW)*RCD(ILCD)
                EvntHit_Z(EvntHit_Num)=ZTRY

                EvntHit_PhiWir(EvntHit_Num)=PHISMR

                EvntHit_Drift(EvntHit_Num)=0.
                
                EvntHit_NxtCls(EvntHit_Num)=0
                EvntHit_NHitDev(EvntHit_Num)=EvntHit_Num

                
                IF(EvntHit_1stLyr(ilcd).eq.0)then
                  EvntHit_1stLyr(ilcd)=EvntHit_Num
                ELSE
                  EvntHit_NxtLyr(LASTHIT(ILCD))=EvntHit_Num
                ENDIF
                LASTHIT(ILCD)=EvntHit_Num
                EvntHit_NxtLyr(EvntHit_Num)=0
                
C velocity in meters/ps            
                VEL=50. *1.E-9
                EvntHit_rawTIM(EvntHit_Num)=ZTRY/VEL
                EvntHit_corTIM(EvntHit_Num)=EvntHit_rawTIM(EvntHit_Num)
                EvntHit_rawTDC(EvntHit_Num)=ZTRY/2.5*1024
                
                EvntHit_Charge(EvntHit_Num)=100.
                EvntHit_rawADC(EvntHit_Num)=300

C EvntHit_corPhi, pwcd is not filled here
              ENDIF
            ENDIF


            
          ENDIF
 249    CONTINUE
 259  CONTINUE
C-----------------------------------------------------------------------

C end of compliation flag CLEO_DOITTPC
#endif

      RETURN
      END
