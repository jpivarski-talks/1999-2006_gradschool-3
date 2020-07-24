*
* $Id: tfindpar.inc,v 1.19 2003/03/05 17:34:45 dpp Exp $
*
* $Log: tfindpar.inc,v $
* Revision 1.19  2003/03/05 17:34:45  dpp
*      -> increase qdivad for TPC
*      -> changed all variable names in cdscrtcd to have common root
*
* Revision 1.18  2002/09/05 19:16:36  dpp
*      -> remove some obsolete parameters and comments
*      -> increase NSTKDM and NSTKTYP for new "wire fit" and "loc amb" stacks
*      -> for TPC, protected by compile switch CLEO_DOITTPC
*          -> increase KLYRDM and NHITDM for TPC
*
* Revision 1.17  2002/05/23 14:26:34  dpp
*      -> increase NSTKDM to be the global size
*      -> NSTKTYP is retained, and increased, for the sub-stak pointers
*      -> stak is now structured as sub-staks in a global stak
*          rather than having the stak be multidimensioned
*      -> increase size of parameter list in TFTRAK
*
* Revision 1.16  2001/11/19 23:16:55  dpp
*      -> remove unneeded parameters
*      -> move program indicators to controlpar.inc
*      -> move EXB bit masks to controlpar.inc
*      -> incorporate remnants of trkpar.inc
*
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
* Revision 1.3  1995/08/24 13:27:02  zfiles
* Changed MXTRK1 to 25 (from 40)
*
*
#if !defined(CLEO_NOCOMDOC)
*== these parameters come from the old trkpar.inc

C NTKCOP - Number of different copies of each track which are kept.
C          Currently 6, 5 different mass hypotheses reported at the
C          origin and the pion hypothesis reported at the last hit
C          on the track.
C MTRAK1 - Maximum number of tracks.
C MXTRAK - Dimension of arrays holding tracking information.

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

* EvntHitMax  - Maximum number of hit wires in CD, defined in cdscrtcd.inc
* MTRKCD - Maximum number of tracks in /CDTRAK/
* NHITDM - Maximum number of hits used
* NSTKDM - Max number of tracks in da STACK
* NSTKTYP- Max number of substaks (does not have to be the number defined)

* MTRAK2 - maximum number of distinct tracks which can be
*            created internally within doit.  There are no external
*            limits on this.
* MTRKCD - maximum number of tracks internal to doit.
*            ( including multiple hypotheses ).
* MTRKDR - maximum number of tracks sent to the zebra banks.
*            ( including multiple hypotheses ).


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

C QDIVAD  - Flag for charge division hits:
C           Layer number is set to the real layer number + QDIVAD.
C QDIVA1  - Similar flag but for use in the THID zebra bank.
C           For QDIV hits, the CDLA is encoded as CDLA+QDIVA1.

#endif

#include "cl3seq/cdgm3/cdgeompa.inc"
#include "doit/duseq/tfgeompa.inc"

      INTEGER NTKCOP
      PARAMETER ( NTKCOP = 6 )
      INTEGER MTRAK1 
      PARAMETER ( MTRAK1 = 25 )
      INTEGER MXTRAK
      PARAMETER ( MXTRAK = MTRAK1*NTKCOP)

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
 
      INTEGER    NHITDM
#if defined(CLEO_DOITTPC)
      PARAMETER( NHITDM = 3000)
#else
      PARAMETER( NHITDM = 300)
#endif

      INTEGER    NSTKDM      , NSTKTYP     ,MBITDR
      PARAMETER( NSTKDM = 106, NSTKTYP = 6 ,MBITDR = 5  )

      INTEGER    KFETCF
      PARAMETER( KFETCF = KLYRCF*(KLYRCF+1)/2 )

      INTEGER    KFETLF
      PARAMETER( KFETLF = KLYRLF*(KLYRLF+1)/2 )

      INTEGER    KFETDM
      PARAMETER( KFETDM = KFETCF )

      INTEGER    KLYRDM
#if defined(CLEO_DOITTPC)
      PARAMETER( KLYRDM = 200 )
#else
      PARAMETER( KLYRDM = 70 )
#endif
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
      PARAMETER( NPARTF =   50, MERCTL =   19, NFSTAT =    8    )
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

      INTEGER QDIVAD
      PARAMETER ( QDIVAD = 1000 )
      INTEGER QDIVA1
      PARAMETER ( QDIVA1 = 100000 )







