*
* $Id: sfxtsp.inc,v 1.6 2003/03/05 18:37:43 dpp Exp $
*
* $Log: sfxtsp.inc,v $
* Revision 1.6  2003/03/05 18:37:43  dpp
*      -> increase MCD from 160000 to 560000
*      -> increase MLAY_CD from 100 to 150
*      -> increase MLAY_SF from 100 to 150
*      -> increase M_XTS_HITS from 300 to 1000
*
* Revision 1.5  2002/09/05 19:20:26  dpp
*      -> change MCD  for TPC , protected by compile switc
*
* Revision 1.4  2000/05/04 21:30:50  dpp
* remove reference to parameter in tfindpar.inc
*
* Revision 1.3  2000/04/26 14:28:31  dpp
* sfxtsp     -> add xts_rfview entry numbers
*            -> add parameters for saved {residual,radius} location
* sfxtsc     -> add saved {residual,radius} location of displayed hits
*
* Revision 1.2  2000/02/12 20:34:58  dpp
*      -> increase maximum number of borders
*
* Revision 1.1.1.1  1998/02/06 19:11:50  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:42  clib
* Developmental version of DUET.
*
* Revision 1.3  1997/02/25 22:09:24  dpp
* add separate layer number parameters for CD and SF
*
* Revision 1.2  1995/04/20 22:29:08  zfiles
* Dan's change. See TITLE.doit
*
* Revision 1.1.1.1  1994/10/08  01:01:12  zfiles
* first version of doit in CVS
*
*
* PARAMETERS FOR SFIND XTSUB ROUTINES
*
* sfxtsp.inc
*
#if !defined(CLEO_NOCOMDOC)
* SFXTSP DOCUMENTATION
C MSEND...........number cells sent at once to the display
C MCD.............number of cells in display
C MAXCHO..........number of elements in choise structure
C MIDHLD..........number held
C MBOUNDS.........number of boundry circles
C MBNDSEG.........number of straight line boundry segments 
C MZOOM...........number of levels stored in zoom
C MLAY_CD.........number of CD layers
C MLAY_SF.........number of SF layers
C M_XTS_HITS......number of hits in XTS, this should be the same as NHITDM
#endif

      INTEGER MSEND
      PARAMETER (MSEND=112)
      INTEGER MCD
#if defined(CLEO_DOITTPC)
      PARAMETER (MCD=560000)
#else
      PARAMETER (MCD=14224)
#endif
      INTEGER MAXCHO
      PARAMETER (MAXCHO=100)
      INTEGER MIDHLD
      PARAMETER (MIDHLD=3000)
      INTEGER MBOUNDS
      PARAMETER (MBOUNDS=20)
      INTEGER MBNDSEG
      PARAMETER (MBNDSEG=100)
      INTEGER MZOOM
      PARAMETER (MZOOM=1000)
      INTEGER MLAY_CD,MLAY_SF
      PARAMETER (MLAY_CD=150,MLAY_SF=150)


      INTEGER M_XTS_HITS
#if defined(CLEO_DOITTPC)
      PARAMETER (M_XTS_HITS=3000)
#else
      PARAMETER (M_XTS_HITS=300)
#endif

      INTEGER ENTRFV_RFVIEW,  ENTRFV_SCREEN,  ENTRFV_SCREEN_D
      INTEGER ENTRFV_INPUT,   ENTRFV_GETINT,  ENTRFV_ZHIT
      INTEGER ENTRFV_TFXFIT,  ENTRFV_LOOP1,   ENTRFV_LOOP2

      PARAMETER (ENTRFV_RFVIEW=1,    ENTRFV_INPUT=2     )
      PARAMETER (ENTRFV_SCREEN=5,    ENTRFV_SCREEN_D=6  )
      PARAMETER (ENTRFV_GETINT=3,    ENTRFV_ZHIT=4      )
      PARAMETER (ENTRFV_TFXFIT=7)
      PARAMETER (ENTRFV_LOOP1=8,     ENTRFV_LOOP2=9     )
