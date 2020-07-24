*
* $Id: tfgeompa.inc,v 1.9 2001/11/19 23:18:48 dpp Exp $
*
* $Log: tfgeompa.inc,v $
* Revision 1.9  2001/11/19 23:18:48  dpp
*      -> move control constants to controlpar.inc
*
* Revision 1.8  2001/09/14 21:19:02  dpp
*      -> add directive to remove fit entry
*
* Revision 1.7  2001/09/12 22:09:20  dpp
*      -> add directive to restore hits in fit to full weight
*      -> expand use of program indicator
*
* Revision 1.6  2000/10/24 22:43:20  dpp
*      -> add HIT_TO_FIT directive for changing program indicator
*
* Revision 1.5  2000/08/18 18:34:10  dpp
*     -> add number of new DOIT scatters
*
* Revision 1.4  1999/12/09 21:03:03  dpp
* add parameters for directives to hit_to_fit
*  add directive to deweight hit
*
* Revision 1.3  1999/11/17 21:42:28  dpp
* add USE_ACCESS to control WIRES_2_HITS counter
*
* Revision 1.2  1999/03/10 15:52:22  lkg
* Remove spurious "X" introduced in front of one of the comment lines
*
* Revision 1.1.1.1  1998/02/06 19:11:49  dpp
* DOIT first release.
*
*
* PARAMETERS Defining Duet geometry.
*
#if !defined(CLEO_NOCOMDOC)

*V2.00 KCANTF and KCATTF replace NCANTF and NCATTF
*Because of this, NLYRTF, NLR2TF and NGYMTF became KLYRTF, KLR2TF and KGYMTF
*And NLYRCF, NLYRLF became KLYRCF and KLYRLF

* KCANTF - Max. Number of TFIND anode   layers
* KCATTF - Max. Number of TFIND cathode layers
* KLYRTF - Max. Number of TFIND total   layers
*
* KLR2TF - Twice the max. total number of layers
*
* NSCATF - Maximum number of discreet   scattering planes
* NSCCTF - Maximum number of continuous scattering media

* M_SCAT - Maximum number of new DOIT scatters

* NLVLTYP - number of level type, ie pat-rec, TFXFIT; for hit finding controls
* NHITTYP - number of hit type, ie WIRE, HIT;  for hit finding controls
* NFITTYP - number of fit type, ie CF, LF;  for hit finding controls
*

#endif

      INTEGER    KCANTF         , KCATTF
      PARAMETER( KCANTF = KCANCD, KCATTF = KCATCD )
      INTEGER    KLYRTF                , KLR2TF
      PARAMETER( KLYRTF = KCANTF+KCATTF, KLR2TF = 2*(KLYRTF+KQDVCD) )
      INTEGER    KGYMTF
      PARAMETER( KGYMTF =(KLR2TF*(KLR2TF+1))/2 )
      INTEGER    KLYRCF         ,   KLYRLF
      PARAMETER( KLYRCF = KCAXCD,   KLYRLF = KCSTCD+KCATCD)


      INTEGER    NSCATF         , NSCCTF
      PARAMETER( NSCATF = 4, NSCCTF = 4 )
      INTEGER    M_SCAT
      PARAMETER( M_SCAT = 10 )

      INTEGER    NLVLTYP    , NHITTYP    , NFITTYP
      PARAMETER( NLVLTYP=  2, NHITTYP=  2, NFITTYP=  2 )










