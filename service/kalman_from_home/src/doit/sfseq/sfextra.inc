*
* $Id: sfextra.inc,v 1.2 1998/07/09 01:17:30 lyon Exp $
*
*
* $Log: sfextra.inc,v $
* Revision 1.2  1998/07/09 01:17:30  lyon
* Added CVS tags (oops)
*
*

C*****
C*****  sfextra.inc  -- Adam L. Lyon -- 26 March 1998
C*****
C*****   Information not in regular cdscrtcd.inc and sfcom.inc common blocks
C*****   needed by SFDATA



* EXFHCD(KLYRCD)   - First hit number in the layer (indexed by layer)
*                    KLYRCD = max number of layers in the cd (see cdgeompa.inc)
*                    Equivalent to IZU(PZU(IPFHCD)+layerNum) [see sfdata.F]
*
* EXNHCD(MHWCD)    - Gives next hit number. If last hit in layer, then
*                    next hit is zero.
*                    MHWCD = Maximum number of hits allowed [see cdscrtcd.inc]
*                    Equivalent to IZU(PZU(IPNHCD)+hitNum)   [see sfdata.F]
*
* EXRADC(MHWCD)    - Raw ADC value
*                    Equivalent to IZU(PZU(IPADCD)+hitNum)   [see sfdata.F]

      INTEGER EXFHCD, EXNHCD, EXRADC

      COMMON /SFXTRAALL/
     &     EXFHCD(KLYRCD), EXNHCD(MHWCD), EXRADC(MHWCD)
