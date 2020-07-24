*
* $Id: doitkd.cdf,v 1.2 1998/02/24 18:36:01 dpp Exp $
*
* $Log: doitkd.cdf,v $
* Revision 1.2  1998/02/24 18:36:01  dpp
* eliminated obsoletes
*
* Revision 1.1.1.1  1998/02/06 19:11:42  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:24  clib
* Developmental version of DUET.
*
*
* Basic KUIP command definition file for the DOIT processor
*
>Name DOITKD

>Menu DOIT
>Guidance
DOIT processor commands

>Command DOITBR
>Guidance
DOIT processor begin run processing
>Action DOITBR

>Command DOITEV
>Guidance
DOIT processor event processing
>Action DOITEV

>Command DOITER
>Guidance
DOIT processor end run processing
>Action DOITER

>Command DOITEJ
>Guidance
DOIT processor end job processing
>Action DOITEJ

>Command DOITVR
>Guidance
DOIT processor version information
>Action DOITVR

>Command DOVERT
>Parameters
ION 'Use primary vertex finder? (1 = finder, 0 = simple average)' I
>Guidance
Decide whether to use DUVFND to find primary event vertex; ION.EQ.0 => use
simple average instead (available for compatibility with 90-91 compress),.
>Action DUVERT

>Command DONAME
>Parameters
ION 'Read in namelist file? (1 = yes, 0 = no)' I
>Guidance
Turns on reading of DOIT namelist file (DOIT.NML), which permits cuts and
parameters to be reset without recompiling.
>Action DUNAME

>Command DOUSQD
>Parameters
ION 'Turn on/off charge division in DOIT (1 = on, 0 = off)' I
>Guidance
Turns on use of VD charge division in DOIT.
>Action DUUSQD

>Menu DOITPL
>Guidance
DOIT plotting commands

>Command DOXTPL
>Parameters
ION 'On or off? (1 = on, 0 = off)' I
>Guidance
Turn DOIT XTSUBS plotting on and off; ION.NE.0 => turn plotting on, ION.EQ.0 =>
turn plotting off.
>Action DUXTS

>Command DOWHOT
>Parameters
ION 'On or off? (1 = on, 0 = off)' I
>Guidance
Turn on writting out of hits on tracks.
>Action DUWHOT

>Command DOKLMN
>Parameters
ION 'On or off? (1 = on, 0 = off)' I
>Guidance
Turn on kalman filter.
>Action DUKLMN

>Command DOBNFL
>Parameters
ION 'On or off? (1 = on, 0 = off)' I
>Guidance
Turn on DOIT bunch finder 
>Action DUBNFL


