*
* $Id: zfdupard.inc,v 1.1 1998/05/07 20:39:41 pg Exp $
*
* $Log: zfdupard.inc,v $
* Revision 1.1  1998/05/07 20:39:41  pg
*   Added ZFILES subdirectory with copy of the files from CLEO II
* libraries. Increased size of the common in usbank.inc from 3500000 to
* 4200000 to accomodate large number of channels in Si3. Other files are
* unchanged.
*
* Revision 1.1.1.1  1994/10/04 20:58:43  zfiles
* first version of seq in CVS
*
*
*  Documentation for ZFDUPAR
*
* zfdupard.inc
*
*
*
* There are 4 different sorts of track banks which can be created
* by different versions of duet.
* All of the 4 types have the same initial 37 words:
*    JKINCD ... JFKN6, + 21 words error matrix.
* There are two types of additions to this basic structure:
*  a) second kink
*  b) extra track quality variables, (chi2/dof etc ).
*
* The second kink information is present if bit 7 of NFTMSK is set.
* The extra track quality information is present if bit 2 of
*   KBITCD(ITRK,4) is set.
*
* The 4 types of track banks are the 4 combinations of these two
* addons being present or absent.
*
* Type
*  1) basic track bank.
*  2) (1) + 7 extra error matrix elements + JFKN7 + JRKN7
*  3) (1) + 5 extra words track quality information.
*  4) (2) + 5 extra words track quality information.
*
* The offsets to the extra chi2 words are defined for bank type (4).
* In the event that of a type (3) track then the offsets for the
* extra chi2 words should be reduced by IOFFK7.
*
*JHNAME -- offset for program name (bank TRKF)
*JNHTWR -- offset for number of hits that pass cuts (TRKF)
*JHTTRK -- offset for number of hits on all tracks (TRKF)
*JGDTRK  -- offset for number of good tracks (TRKF)
*JBADTR -- offset for number of bad tracks (TRKF)
*JNVTRK -- offset for number of tracks in primary vertex (VPTX)
*JXVDR  -- offset for x of vertex (VPTX)
*JYVDR  -- offset for y "    "      "
*JZVDR  -- offset for z "    "      "
*JDXVDR -- offset for error in x of vertex (VPTX)
*JDYVD  -- offset for   "    " y  "    "      "
*JDZVDR -- offset for   "    " z  "    "      "
*JKINCD -- offset for track type (TRAK)
*JMISCD -- offset for number of missed layers (TRAK)
*MKBIT  -- offset for 1st bit code (KBITDR) word (TRAK)
*JRESID -- offset for residuals (TRAK)
*JRKN6  -- offset for radius of kink (TRAK)
*JPMASK -- offset for parameter mask (TRAK)
*JCURCD -- offset for curvature (TRAK)
*JPHI0  -- offset for phi at PCA (TRAK)
*JD0CD  -- offset for impact parameter (TRAK)
*JCOTAN  -- offset for cotan of polar angle (TRAK)
*JZ0CD  -- offset for z at PCA (TRAK)
*JFKN6  -- offset for phi of kink at VD-DR(TRAK)
*JFKN7  -- offset for phi of kink in DR   (TRAK)
*JRKN7  -- offset for radius of kink in DR(TRAK)
*JERRM  -- offset for 1st element of error matrix (TRAK)
*JCH3CD -- offset for chi2/dof of track fit
*JCH3PT -- offset for chi2/layer in PT
*JCH3VD -- offset for chi2/layer in VD
*JCH3DR -- offset for chi2/layer in DR
*JCH3CT -- offset for chi2/layer in CT
*
*IOFFK7 -- negative offset for CHI3CD etc when second kink is absent.
