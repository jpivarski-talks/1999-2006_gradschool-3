*
* $Id: tfindpar.inc,v 1.15 2001/09/12 22:16:11 dpp Exp $
*
* $Log: tfindpar.inc,v $
* Revision 1.15  2001/09/12 22:16:11  dpp
*      -> expand use of program indicator
*
* Revision 1.14  2000/12/19 23:29:31  dpp
*      -> increase NPARTF: counters of number of DR layers of types in fit
*
* Revision 1.13  2000/10/24 22:42:44  dpp
*      -> define program indicator values for different subroutines
*
* Revision 1.12  2000/10/02 21:16:56  dpp
*      -> add DoitModeNumber bit for calibration
*
* Revision 1.11  2000/08/18 18:32:39  dpp
*      -> adjust NPARTF added and removed stak variables
*      -> increase NTKPAR
*      -> remove NPARKL
*
* Revision 1.10  2000/03/01 23:01:39  dpp
*      -> even more parameters for unfolding NEXPT,   and changed one
*
* Revision 1.9  2000/02/18 21:17:51  dpp
*      -> more parameters for unfolding NEXPT as the doit running level flag
*
* Revision 1.8  2000/02/12 20:37:09  dpp
*      -> parameters for unfolding NEXPT as the doit running level flag
*
* Revision 1.7  1999/12/10 22:23:31  dpp
* replace NLMNVD
*
* Revision 1.6  1999/12/09 21:00:11  dpp
*  clean up some un-used junk
*
* Revision 1.5  1999/11/29 21:43:39  lyon
* Changes to include files to remove references to old Cleo II include areas
*
* Revision 1.4  1999/08/03 22:01:36  dpp
* dimension of stack from (5) to (2,50); stack order saturation
*
* Revision 1.3  1999/07/27 15:42:32  lyon
* Changed NHITDM from 150 to 300 - should help with lattice violations
*
* Revision 1.2  1998/07/09 00:38:05  lyon
* seq/cdgeom -> cl3seq/cdgm3
*
* Revision 1.1.1.1  1998/02/06 19:11:49  dpp
* DOIT first release.
*
*
*
#if !defined(CLEO_NOCOMDOC)
*== These PARAMETERS are general

* MNHTCF - Minimum number of hits in a CFIND track
* MDPLCF - Minimum number of adjacent hits for track to be duplicate
* MNHTLF - Minimum number of hits in a LFIND track

*??? cdkill.F only
* NLMNVD - some sort of minimum layers in the VD

* RMINVD - Min rad for a VD seed to curl

* WID_SAT_HITS_STK_ORD - width of the saturation in hits for STACK order
* MIN_HITS_SAT_STK_ORD - minimum number of hits to saturate the STACK order 

*== These PARAMETERS control array sizes. The size used is optional
*   and is a compromise between space/speed/efficiency.

* MHWCD  - Maximum number of hit wires in CD
* MTRKCD - Maximum number of tracks in /CDTRAK/
* NHITDM - Maximum number of hits used
* NSTKDM - Max number of tracks in da STACK
* NSTKTYP- Max number of types in da STACK

* MTRAK2 - maximum number of distinct tracks which can be
*            created internally within doit.  There are no external
*            limits on this.
* MTRKCD - maximum number of tracks internal to doit.
*            ( including multiple hypotheses ).
* MTRKDR - maximum number of tracks sent to the zebra banks.
*            ( including multiple hypotheses ).



*??? tflinkcd.inc only
* NLNKDM - Maximum number of links accepted
* NLMBDM - Maximum number of limbs  in the limb  list

*??? tfpsum.F only
* NCHNDM - Maximum number branch or root chains for a link
* NRBLDM - Max number of links in all root or branch chains in TFCLMB

*??? tftrkscd.inc only
* NTRKDM - Max number of tracks in the track list
* NLKTDM - Max number of links  in the track link  list



*== These PARAMETERS are dependent on the geometry and have to be
*   changed if the geometry is changed. (Use TFCHEK to calculate sizes)

*??? tfindpar.inc, cftypecd.inc, lftypecd.inc, tftypecd.inc only
* NFETCF - Number of first elementary trees : CFIND
* NTYPCF - Number of link types used in CFIND
* NETRCF - Number of elementary tree kinds in CFIND
* NTYPLF - Number of link types used in LFIND
* NETRLF - Number of elementary tree kinds in LFIND
* NTYPDM - = MAX0(NTYPCF,NTYPLF)
* NETRDM - = MAX0(NETRCF,NETRLF)
* NFETDM - = MAX0(NFETCF,NFETLF)
* NLYRDM - = MAX0(NLYRCF,NLYRLF)

*??? now a variable in tfgeomcd
* NFETLF - Number of first elelementary trees : LFIND

*== These PARAMETERS should not be changed at all

* IFGEOM - Flags : Geometry arrays filled
* ICFDRG - Flags CFDRDG
*
* NP3DTF - Number of parameters in 3D fit
* NERMTF - Number of elements in error matrix
* NP3DKL - Number of parameters in Kalman Filter fit
* NERMKL - Number of elements in error matrix from Kalman Filter.
* NPARTF - Number of stack parameters in /TFTRAK/ (change with care --
*              TFTRAKCD, TFHITSCD both must change)
* NTKPAR - Number of track parameters in /TFTRAK/ (change with care --
*              TFTRAKCD, TFHITSCD both must change)
* NKAPAR - Number of (K,A,B,G,X) parameters in /TFTRAK/ (change with care --
*              TFTRAKCD, TFHITSCD both must change)
* MERCTL - Number of control errors
* NFSTAT - Dimension of track finding statistics
* IWRPTF - Packing factor for cyl & wire in TF
* ICFPAK - Packing factor for hits in the middle CFIND link
* ILFPAK - Packing factor for hits in the middle LFIND link
* CHIDEF - Default value of track chisquare
* ERRDEF - Default value for diagonal elements of the error matrix.
*          Set in the seedtrack routines.
* IERPAC - Packing coefficient for errors
* NERWRD - Number of words per error
* NERBDM - Depth of error reporting buffer
*
*
* Magic numbers:

*??? doit_init_sigma.F
* NRUN.GT.NRUNDM ==> PT changed to DME.
*                    Also, new cathode finding weights used.

*??? doitnl.F
* NRUN.GE.NRUNVE ==> Use VDEAST TDC's by default

*??? doitnl.F sftcut.F
* NRUN.GE.NRNVDS ==> run number when the VD electronics were swapped
*                    (ie East <=> West ) 

* RKN7MN - minimum allowed radius for second kink (7th fitting variable)
* RKN7MX - maximum allowed radius for second kink (7th fitting variable)
*          IF RKN7CD(ITRK) is outside this range, then it was not
*          a fitting variable.

* EXB_DOIT_FAST -  NEXPT bit value to designate DoitFast
* EXB_DOIT_FULL -  NEXPT bit value to designate full production DOIT  
* EXB_DOIT_NEW   - NEXPT bit value to designate running some new stuff
* EXB_SF_DATA_CUT- NEXPT bit value to designate using SF "data" time cuts
* EXB_SF_STARTUP - NEXPT bit value to designate using SF "STARTUP" cut set
* EXB_DOIT_NOCHI - NEXPT bit value to designate not using DUFIND CHICF cut
* EXB_CALIB      - NEXPT bit value to designate special calibration settings

* P_I_FAST_TRAK            - PROGRAM INDICATOR for FAST_TRAK
* P_I_F_T_AMB_GRP_TO_TRAK  - PROGRAM INDICATOR for FAST_TRAK_AMB_GRP_TO_TRAK 
* P_I_F_T_CLOSE_FIT_STORE  - PROGRAM INDICATOR for FAST_TRAK_CLOSE_FIT_STORE
* P_I_F_T_DEL_BAD_AND_ADD  - PROGRAM INDICATOR for FAST_TRAK_DEL_BAD_AND_ADD
* P_I_F_T_FIT_WIRE_POSITN  - PROGRAM INDICATOR for FAST_TRAK_FIT_WIRE_POSITN
* P_I_TFXFIT               - PROGRAM INDICATOR for TFXFIT
* P_I_TFXFIT_REDUCE        - PROGRAM INDICATOR for TFXFIT, but reduced drift
* P_I_REPLACE_Z_HITS       - PROGRAM INDICATOR for REPLACE_Z_HITS
* P_I_BOOT_SI_Z_HITS       - PROGRAM INDICATOR for BOOT_SI_Z_HITS

#endif

#include "cl3seq/cdgm3/cdgeompa.inc"
#include "doit/duseq/tfgeompa.inc"
#include "doit/duseq/trkpar.inc"

      INTEGER    MNHTCF     , MDPLCF
      PARAMETER( MNHTCF =  4, MDPLCF =  2 )
      INTEGER    MNHTLF
      PARAMETER( MNHTLF =  3 )

      INTEGER    NLMNVD
      PARAMETER( NLMNVD = 0.72*NCANVD )
      REAL       RMINVD
      PARAMETER( RMINVD = 0.47   )

      INTEGER WID_SAT_HITS_STK_ORD
      PARAMETER (WID_SAT_HITS_STK_ORD =5)
      INTEGER MIN_HITS_SAT_STK_ORD
      PARAMETER (MIN_HITS_SAT_STK_ORD =20)

      INTEGER    MTRKCD,       MTRKDR,       MTRAK2
      PARAMETER( MTRAK2 = 40                              )
      PARAMETER( MTRKCD = MTRAK2*NTKCOP                   )
      PARAMETER( MTRKDR = MXTRAK                          )
 
      INTEGER    NHITDM      , NLNKDM      , NLMBDM
      PARAMETER( NHITDM = 300, NLNKDM =1000, NLMBDM =2500 )
      INTEGER    NCHNDM     , NRBLDM      , NTRKDM      , NLKTDM
      PARAMETER( NCHNDM =250, NRBLDM =1000, NTRKDM =1000, NLKTDM =5000 )
      INTEGER    NSTKDM      , NSTKTYP ,  MBITDR
      PARAMETER( NSTKDM =  50, NSTKTYP=2, MBITDR = 5  )

      INTEGER    KFETCF
      PARAMETER( KFETCF = KLYRCF*(KLYRCF+1)/2 )
      INTEGER    NTYPCF       , NETRCF
      PARAMETER( NTYPCF =  291, NETRCF =  598 )

      INTEGER    KFETLF
      PARAMETER( KFETLF = KLYRLF*(KLYRLF+1)/2 )
      INTEGER    NTYPLF       , NETRLF
      PARAMETER( NTYPLF =  108, NETRLF =  405 )

      INTEGER    NTYPDM         , NETRDM         , KFETDM
      PARAMETER( NTYPDM = NTYPCF, NETRDM = NETRCF, KFETDM = KFETCF )
      INTEGER    KLYRDM
      PARAMETER( KLYRDM = 70 )

      INTEGER    IFGEOM
      PARAMETER( IFGEOM = 1                         )
      INTEGER    ICFDRG
      PARAMETER( ICFDRG = 4 )

      INTEGER    NP3DTF       , NERMTF
      PARAMETER( NP3DTF =    7, NERMTF = NP3DTF*(NP3DTF+1)/2    )

      INTEGER    P3DCU,  P3DFI,  P3DDA,  P3DCT,  P3DZ0,  P3DK6,  P3DK7
      PARAMETER (P3DCU  =    1, P3DFI  =    2, P3DDA  =    3    )
      PARAMETER (P3DCT  =    4, P3DZ0  =    5, P3DK6  =    6    )
      PARAMETER (P3DK7  =    7                                  )

      INTEGER    NPARTF       , MERCTL       , NFSTAT
      INTEGER    NTKPAR       , NKAPAR
      PARAMETER( NPARTF =   48, MERCTL =   19, NFSTAT =    8    )
      PARAMETER( NTKPAR =   20, NKAPAR =    5)

      INTEGER    NP3DKL       , NERMKL
      PARAMETER( NP3DKL =    5, NERMKL = NP3DKL*(NP3DKL+1)/2    )

      INTEGER    IWRPTF
      PARAMETER( IWRPTF = 65536)

      INTEGER    ICFPAK       , ILFPAK
      PARAMETER( ICFPAK =  256, ILFPAK =    0                   )

      REAL       CHIDEF
      PARAMETER( CHIDEF = -1.0                                  )

      REAL       ERRDEF
      PARAMETER( ERRDEF = 1.E-6 )

      INTEGER    IERPAC       , NERWRD       , NERBDM
      PARAMETER( IERPAC =  100, NERWRD =    6, NERBDM =  100    )


      INTEGER NRUNDM,NRUNVE,NRNVDS
      PARAMETER ( NRUNDM = 46000 )
      PARAMETER ( NRUNVE = 58837 )
      PARAMETER ( NRNVDS = 81206 )

      REAL RKN7MN, RKN7MX
      PARAMETER ( RKN7MN = 0.3249 )
      PARAMETER ( RKN7MX = 0.8001 )

      INTEGER EXB_DOIT_FAST,   EXB_DOIT_FULL,    EXB_DOIT_NEW
      INTEGER EXB_SF_DATA_CUT, EXB_SF_STARTUP,   EXB_DOIT_NOCHI
      INTEGER EXB_CALIB

      PARAMETER (EXB_DOIT_FAST=   1)
      PARAMETER (EXB_DOIT_FULL=  32)

      PARAMETER (EXB_DOIT_NEW=   64)
      PARAMETER (EXB_SF_DATA_CUT= 2)
      PARAMETER (EXB_SF_STARTUP=  4)
      PARAMETER (EXB_DOIT_NOCHI=  8)
      PARAMETER (EXB_CALIB=     256)

      INTEGER P_I_FAST_TRAK
      INTEGER P_I_F_T_AMB_GRP_TO_TRAK, P_I_F_T_CLOSE_FIT_STORE
      INTEGER P_I_F_T_DEL_BAD_AND_ADD, P_I_F_T_FIT_WIRE_POSITN
      INTEGER P_I_TFXFIT,              P_I_TFXFIT_REDUCE
      INTEGER P_I_REPLACE_Z_HITS,      P_I_BOOT_SI_Z_HITS


      PARAMETER (P_I_FAST_TRAK                =10)
      PARAMETER (P_I_F_T_AMB_GRP_TO_TRAK      =11)
      PARAMETER (P_I_F_T_CLOSE_FIT_STORE      =12)
      PARAMETER (P_I_F_T_DEL_BAD_AND_ADD      =13)
      PARAMETER (P_I_F_T_FIT_WIRE_POSITN      =14)
      PARAMETER (P_I_TFXFIT                   =21)
      PARAMETER (P_I_TFXFIT_REDUCE            =22)
      PARAMETER (P_I_REPLACE_Z_HITS           =31)
      PARAMETER (P_I_BOOT_SI_Z_HITS           =41)

* ----------------------------------------------------------







