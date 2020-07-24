*
* $Id: tfctlcde.inc,v 1.4 2000/10/02 21:15:20 dpp Exp $
*
* $Log: tfctlcde.inc,v $
* Revision 1.4  2000/10/02 21:15:20  dpp
*      -> add PROGRAM_INDICATOR
*      -> move Z0BIAS to tfctlcde
*
* Revision 1.3  2000/04/26 14:14:33  dpp
*      -> add D0_CONSTRAIN  and Z0_CONSTRAIN
*
* Revision 1.2  2000/02/12 20:36:41  dpp
*      -> comment on NEXPT
*
* Revision 1.1.1.1  1998/02/06 19:11:49  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:25  clib
* Developmental version of DUET.
*
* Revision 1.7  1996/07/08 21:47:37  dpp
* switch to allow a single layer to be deleted from trackfinding
*
* Revision 1.6  1996/04/24 21:55:44  zfiles
* Add LWHITS
*
* Revision 1.5  1996/04/15 17:06:13  dpp
* comment
*
* Revision 1.4  1996/02/06 00:18:55  zfiles
* add LTSTSD to switch call of DUTEST in DUCFTR
* add LEXTFX to switch on use of TFXFIT in CFEXFN
*
* Revision 1.3  1996/01/30 15:16:38  zfiles
* add LKNPRC   and   incorporate documentation
*
* Revision 1.2  1995/11/28 18:44:15  zfiles
* Added LSVDUF,LTKLOG
*
* Revision 1.1.1.1  1994/10/08  01:01:08  zfiles
* first version of doit in CVS
*
*
*CMZ :  6.00/28 29/05/94  21.19.10  by  Anders Ryd
*CMZ :  6.00/24 09/05/94  00.56.27  by  Anders Ryd
*CMZ :  6.00/14 21/02/94  19.22.58  by  Dan Peterson
*CMZ :  5.54/15 29/09/92  14.11.54  by  Rob Kutschke
*CMZ :  5.54/14 20/07/92  10.09.02  by  Dan Peterson
*CMZ :  5.54/00 18/10/90  00.35.43  by  Steve Schaffner
*CMZ :  5.53/07 08/10/90  12.03.40  by  Steve Schaffner
*CMZ :  5.53/06 30/07/90  18.27.36  by  Steve Schaffner
*CMZ :  5.53/00 02/04/90  16.02.52  by  Steve Schaffner
*CMZ :  5.52/01 15/03/90  15.56.30  by  Steve Schaffner
*CMZ :  5.52/00 08/02/90  12.44.13  by  Steve Schaffner
*-- Author :
*
* tfctlcde.inc
*
#if !defined(CLEO_NOCOMDOC)
* ----------------------------------------------------------
*  /TFCTL / Controls the TFIND programs
* ----------------------------------------------------------
* NEXPT  - Experiment number of current run
*          now used for the DOIT running condition word
* NRUN   - Run    number of current run
* NEVENT - Event  number of current event
* NRECRD - Record number of current event
* NTRACK - Track  number of current track

* PROGRAM_INDICATOR - integer specifying which subroutine is adding hits

* IFIND  - = ICFND  if we are searching for circular tracks
*          = ILFND  if we are searching for linear   tracks
*          = ICFDRG if we are dredging
* NHTLNK - # of hits  in a link
* NHTMIN - # of hits  required for a track with minimum # of drift hits
* NLKMIN - # of links required for a track with minimum # of drift hits
* MLRLHT - Maximum layer for the last hit in a track with NLKMIN links
* NLKREQ - # of links required for a track with NHITS
* BMAGTF - Magnetic field in kgauss
* ENRGTF - Beam energy in GeV
* CHICUT - Chi2 cut for ?FTCUT
* CH2SCL - Chi2 scale factor for given # of degrees of freedom
* DCH2TF - Delta(chi2) cut in TF3FIT
* NDELTF - Max # of hits to delete in TF3FIT
* MP3DTF - # of parameters in helix fit (depends on material)
* MERMTF - # of matrix els in helix fit (depends on material)
* IMNVTF - = 0  => use     diagonal cov matrix in TF3FIT
*          = 1  => use non-diagonal cov matrix in TF3FIT
*          = 2  => use diagonal cov matrix but post-fit correction
* LKN6TF - .TRUE. if a helix is fitted with kink at VD-DR,adds 1 to  MP3DTF
* LKN7TF - .TRUE. if a helix is fitted with kink in mid DR,adds 1 to  MP3DTF
* T0BHTF - T0 correction on BHABHA events (dirty, dirty)
* XSCMIN - Minimum material before 6 parameters are used in fit
* CHIFUZ - Width of fuzzy cut in TFSORT (for applying z=0 biased chisq)
* PHMNCD - Raw data min. pulse height cuts for each CD device
* TMINCD - Raw data min. timing  cuts for each CD device
* TMAXCD - Raw data max. timing  cuts for each CD device
*
* LTRIED - = .TRUE. => TFIND has searched for tracks with given # hits
*
* LDEBUG - debug flag
* LPRNTN - .TRUE. => N'th print flag on
* LPRNT1 - CFINIT or LFINIT - turns on TFPRTP
* LPRNT2 - CFINIT or LFINIT - turns on TFPRET
* LPRNT3 - CFINIT or LFINIT - prints DIFCCF or DIFCLF
* LPRNT4 - TFGETR print enabled
* LPRNT5 - CDPRI2 print enabled for this run
* LPRERR - prints error buffer
* LMESFF - prints FFREAD values via message logger
* LPROUT - prints routine messages (permits turning off all output)
* LOPEN  - open lineprinter output file
* LPLOT  - enable event plotting
* LPLOT2 - enable candidate track plotting
* LGRAPH - flags that GKS/HIGZ has been initialized
* LXTSUB - enable XTSUBS track plotting
* LCOMPR - enable Compress histogram output (incompatible with LWHIST)
* LWHIST - enable Hbook histogram output
* LVERT  - use DUVFND to find primary vertex (.T. by default, .F. only for
*                       backward compatability)
* LNAME  - enable reading of namelist file
*
* NEVDU3 - # of events with a call to DUET3
* NEVDRH - # of events with no error in get DIN 20 (DR hits)
* NEVGDR - # of events with no error in get DIN 22 (ANLCDR tracks)
* NEVUSE - # of events with hits and SOLO tracks that can be used
*
* NDMUSE - Maximum # index used for array associated with given error #
* NERCTL - # of tracks or events with given error #
* LERCTL - This track  or event  had  given error #
*
* NCLOST - # of CFIND tracks lost with given error #
* NLLOST - # of LFIND tracks lost with given error #
*
* NCHNTR - Maximum # root or branch chains actually used in TFCLMB
* NRBLTR - Maximum # root or branch links  actually used in TFCLMB
*
* NTTRYD - # of tracks of given type tried
* NTBADL - # of tracks of given type lost with bad layers
* NTLOST - # of tracks of given type lost in finding
* NTFIND - # of tracks of given type found
* NTRTIN - # of tracks of given KINDR given  to  DUET3 as seeds
* NTRT3F - # of tracks of given KINDR given  to  TF3FIT
* NTRTCD - # of tracks of given KINDR stored in /CDTRAK/
* NTRTDR - # of tracks of given KINDR stored in  ANLCDR
* LYRKIL - PICK ONE LAYER THAT YOU WOULD LIKE TO DELETE FROM TRACKFINDING
*
* FNDNAM - Name of ?FIND in characters for printing
*
* LUSEDR - .TRUE.  => Use DR data
* LUSEIZ - .TRUE.  => Use IZ data
* LUSEVD - .TRUE.  => Use VD data
* LUSEIV - .TRUE.  => Use IV data
* LUSEPT - .TRUE.  => Use PT data
* LUSECT - .TRUE.  => Use cathode data
* LUSEQD - .TRUE.  => LUSQD1 .AND. inside good qdiv run range
* LGPADR - .TRUE.  => Write wire addresses etc. on output stream
* LMCRLO - .TRUE.  => This is CLEO   monte carlo
* LDME   - .TRUE.  => DME in PT
* LDUEMC - .TRUE.  => This is DUETMC monte carlo
* LUSE(MXDVCD) -- Temporary array for GTCDRW
* LUSQD1 - .TRUE.  => Use charge division data if inside good run range
* LAB1TF - .TRUE.  => Enable discrete energy loss.
* LKALMN - .TRUE.  => Use Kalman filter instead of TF3FIT.
* LVTEST - .TRUE.  => Check version number of DUET against run number.
*                   & check if TFBN process was called for multi-bunch runing
* LSLEAC - .TRUE.  => Enable Stereo Layer Entrance Angle Correction.
* LUSVDE - .TRUE.  => Logical: USe VD East instead of west
* LSVDUF - .TRUE.  => Use SVD leftovers as seeds in DUFIND.
* LTKLOG - .TRUE.  => Call TRKLOG in CDSTOR.
* LKNPRC - .TRUE.  => allow a KiNk in Pattern ReCognition for SFIND seeds
* LTSTSD - .TRUE.  => call DUTEST to test validity of seed track result
* LEXTFX - .TRUE.  => call TFXFIT from CFEXFN (CFEXTN and CFFLIP)
* LWHITS - .TRUE.  => Write out the THID,THPL,THS1,THS2,THDD banks.
* D0_CONSTRAIN.........=.TRUE. if D0 is to constrained to =0
* Z0BIAS...............value of Z0 origin sigma used in the fit

* ----------------------------------------------------------
#endif

      INTEGER          NEXPT,  NRUN  , NEVENT, NRECRD, NTRACK,
     +                 PROGRAM_INDICATOR,
     +                 IFIND ,
     +                 NHTLNK, NHTMIN, NLKMIN, MLRLHT,
     +                         NHTCUT, NLKCUT,
     +                 NDELTF, MP3DTF, MERMTF, IMNVTF,
     +                 NEVDU3, NEVDRH, NEVGDR, NEVUSE,
     +                 NDMUSE        , NERCTL        ,
     +                 NCLOST        , NLLOST        ,
     +                 NCHNTR, NRBLTR, NTRSTK,
     +                 NTTRYD        , NTBADL        ,
     +                 NTLOST        , NTFIND        ,
     +  NTRTIN      , NTRT3F      , NTRTCD      , NTRTDR,
     +  LYRKIL

      COMMON /TFCTLI/  NEXPT,  NRUN  , NEVENT, NRECRD, NTRACK,
     +                 PROGRAM_INDICATOR,
     +                 IFIND ,
     +                 NHTLNK, NHTMIN, NLKMIN, MLRLHT,
     +                         NHTCUT, NLKCUT,
     +                 NDELTF, MP3DTF, MERMTF, IMNVTF,
     +                 NEVDU3, NEVDRH, NEVGDR, NEVUSE,
     +                 NDMUSE(MERCTL), NERCTL(MERCTL),
     +                 NCLOST(MERCTL), NLLOST(MERCTL),
     +                 NCHNTR, NRBLTR, NTRSTK,
     +                 NTTRYD(NFSTAT), NTBADL(NFSTAT),
     +                 NTLOST(NFSTAT), NTFIND(NFSTAT),
     +  NTRTIN(-2:2), NTRT3F(-2:2), NTRTCD(-2:2), NTRTDR(-2:2),
     +                 LYRKIL

      LOGICAL   LUSEPT,LUSEIZ,LUSEVD,LUSEDR,LUSECT,LUSEQD,LTRIED,LERCTL,
     +   LUSSV1,LDEBUG,LPRNT1,LPRNT2,LPRNT3,LPRNT4,LPRNT5,LPRNT6,LPRNT7,
     +   LTYPE ,LPRINT,LPRINH,LPLOT ,LPTRKS,LPLNKS,LPETRS,LPHITS,LPLHTS,
     +   LPWLST,LPSTAK,LP3DT ,
     +   LWSUM ,LWTIME,LWHIST,LCOMPR,LVERT, LNAME, LXTSUB,
     +   LGPADR,LMCRLO,LDME  ,LSV1  ,LPRERR,LDIVID,LPROUT,LOPEN,
     +   LKN6TF,LKN7TF,
     +   LDUEMC,LUSEIV,LCFHST,LLFHST,LANHST,LPLOT2,LGRAPH,LUSE,
     +   LUSQD1,LAB1TF,LKALMN,LVTEST,LSLEAC,LUSVDE,LSVDUF,LTKLOG,
     +   LKNPRC,LTSTSD,LEXTFX,LWHITS
      LOGICAL D0_CONSTRAIN


      COMMON /TFCTLL/  LUSEIZ,LUSEVD,LUSEDR,LUSECT,LUSEQD,
     +                 LTRIED(KLYRTF),
     +                 LERCTL(MERCTL),
     +   LDEBUG,LPRNT1,LPRNT2,LPRNT3,LPRNT4,LPRNT5,LPRNT6,LPRNT7,
     +   LTYPE ,LPRINT,LPRINH,LPLOT ,LPTRKS,LPLNKS,LPETRS,LPHITS,LPLHTS,
     +   LPWLST,LPSTAK,LP3DT ,LUSE(MXDVCD),
     +   LWSUM ,LWTIME,LWHIST,LCOMPR,LVERT, LNAME, LXTSUB,
     +   LGPADR,LMCRLO,LDME  ,LSV1  ,LPRERR,LDIVID,LPROUT,LOPEN,
     +   LKN6TF,LKN7TF,
     +   LDUEMC,LUSEPT,LUSEIV,LCFHST,LLFHST,LANHST,LPLOT2,LGRAPH,
     +   LUSQD1,LAB1TF,LKALMN,LVTEST,LSLEAC,LUSVDE,LSVDUF,LTKLOG,
     +   LKNPRC,LTSTSD,LEXTFX,LWHITS
      COMMON /TFCTLL2/ 
     1                      D0_CONSTRAIN


      REAL             BMAGTF, ENRGTF, CHICUT,
     +                 CH2SCL        , DCH2TF, T0BHTF, XSCMIN, CHIFUZ,
     +                 PHMNCD        , TMINCD        , TMAXCD        ,
     +                 PHMNIZ, PHMNIV, PHMNVD, PHMNDR, PHMNVC, PHMNDC,
     +                 PHMNPT, PHMNPC,
     +                 TMINIZ, TMINIV, TMINVD, TMINDR, TMINVC, TMINDC,
     +                 TMAXIZ, TMAXIV, TMAXVD, TMAXDR, TMAXVC, TMAXDC,
     +                 TMINPT, TMAXPT,
     +                 CPHIT,  SPHIT,
     +                 Z0BIAS

      COMMON /TFCTLR/  BMAGTF, ENRGTF, CHICUT,
     +                 CH2SCL(KLYRTF), DCH2TF, T0BHTF, XSCMIN, CHIFUZ,
     +                 PHMNCD(MXDVCD), TMINCD(MXDVCD), TMAXCD(MXDVCD),
     +                 CPHIT(MTRKDR),  SPHIT(MTRKDR) ,
     +                 Z0BIAS

      EQUIVALENCE (PHMNIZ, PHMNCD(IPROP)), (PHMNIV, PHMNCD(IIVDT))
      EQUIVALENCE (PHMNVD, PHMNCD(IVDET)), (PHMNDR, PHMNCD(IDRFT))
      EQUIVALENCE (PHMNVC, PHMNCD(IVDCT)), (PHMNDC, PHMNCD(IDRCT))
      EQUIVALENCE (PHMNPT, PHMNCD(IPTLA)), (PHMNPC, PHMNCD(IPTLC))
      EQUIVALENCE (TMINIZ, TMINCD(IPROP)), (TMINIV, TMINCD(IIVDT))
      EQUIVALENCE (TMINPT, TMINCD(IPTLA)), (TMAXPT, TMAXCD(IPTLA))
      EQUIVALENCE (TMINVD, TMINCD(IVDET)), (TMINDR, TMINCD(IDRFT))
      EQUIVALENCE (TMINVC, TMINCD(IVDCT)), (TMINDC, TMINCD(IDRCT))
      EQUIVALENCE (TMAXIZ, TMAXCD(IPROP)), (TMAXIV, TMAXCD(IIVDT))
      EQUIVALENCE (TMAXVD, TMAXCD(IVDET)), (TMAXDR, TMAXCD(IDRFT))
      EQUIVALENCE (TMAXVC, TMAXCD(IVDCT)), (TMAXDC, TMAXCD(IDRCT))

      CHARACTER*8     FNDNAM
      COMMON /TFCTLC/ FNDNAM(4)


