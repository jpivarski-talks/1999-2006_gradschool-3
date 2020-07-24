*
* $Id: svtable.inc,v 1.1.1.1 1998/02/06 19:11:49 dpp Exp $
*
* $Log: svtable.inc,v $
* Revision 1.1.1.1  1998/02/06 19:11:49  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:25  clib
* Developmental version of DUET.
*
* Revision 1.1  1997/02/26 06:30:17  clib
* New release.  Hold SV geometry.
*
*
	DOUBLE PRECISION SVEDGEX, SVEDGEY
C These variables save the X and Y coordinates, respectively, of
C  the edges of the detector for each wafer.  Edge = 1 gives the
C  channel = 0 side, Edge = 2 gives the channel = NSTRIPS - 1
C  side.

	INTEGER NUM_EDGES
	PARAMETER ( NUM_EDGES = 2)

	COMMON /SVTABLE/SVEDGEX(SVMWFR , NUM_EDGES),
     .                  SVEDGEY(SVMWFR , NUM_EDGES)



