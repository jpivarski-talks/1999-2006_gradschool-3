*
* $Id: statusbits.inc,v 1.2 2002/05/23 14:28:39 dpp Exp $
*
* $Log: statusbits.inc,v $
* Revision 1.2  2002/05/23 14:28:39  dpp
*      -> new status bits to indicate use of limited radius solution
*
* Revision 1.1  2001/11/19 23:19:32  dpp
*      -> NEW
*      -> parameters to control filling status bits
*
*
* PARAMETERS FOR STATUS BITS 
*
* statusbits.inc
*
#if !defined(CLEO_NOCOMDOC)

C-----------------------------------------------------------------------
C documentation copied from index/statcf.txt
C-----------------------------------------------------------------------

C One should use these bits to select tracks which are of appropriate
C quality for each particular analysis.

C  n : JBIT(STAT,n)            STAT(1)    loaded by                   value
  
C   1  SF seed indicator                  DUFIND                      0-1
C   2  alternate seed indicator                                       0-1
C   3  another alternate seed indicator                               0-1
C   4  pass number (through SF)           DUFIND                      0-7
C   5  |
C   6  -
C   7  merge type(1:normal, 2:superchain) SFNEXT                      1-2
C   8  -
C   9  merge indicator                    SFNEXT                      0-1
C  10  chain number(or super chain numbr) SFNEXT                      0-255
C  11  |
C  12  |
C  13  |
C  14  |
C  15  |
C  16  |
C  17  -
C  18  number of hits in first wire fit   FAST_TRAK_FIT_WIRE_POSITN   0-63
C  19  |
C  20  |
C  21  |
C  22  |
C  23  -
C  24  remove D0 constraint in wire fit   FAST_TRAK_FIT_WIRE_POSITN   0-1
C  25  try adding kink in wire fit        FAST_TRAK_FIT_WIRE_POSITN   0-1
C  26  continue with kink in wire fit     FAST_TRAK_FIT_WIRE_POSITN   0-1
C  27  continue kink after adding close   FAST_TRAK_CLOSE_FIT_STORE   0-1
C  28  try adding kink in hit fit         FAST_TRAK_CLOSE_FIT_STORE   0-1
C  29  continue with kink in hit fit      FAST_TRAK_CLOSE_FIT_STORE   0-1
C  30  continue kink after del bad & add  FAST_TRAK_DEL_BAD_AND_ADD   0-1
C  31  
C  32 ...............................sign bit ; don't use !

C n : JBIT(STAT,n)            STAT(2)    loaded by                   value

C....first FAST_TRAK iteration
C   1  D0_CONSTAIN                        FAST_TRAK                   0-1
C   2  Z0_CONSTAIN                        FAST_TRAK                   0-1
C   3  from alternate ambiguity           FAST_TRAK etc               0-1
C   4  alternate ambiguity uses NULL      FAST_TRAK etc               0-1
C   5  number of hits from loc amb        FAST_TRAK etc               0-63
C   6  |
C   7  |
C   8  |
C   9  |
C  10  -
C....second FAST_TRAK iteration
C  11  D0_CONSTAIN                        FAST_TRAK                   0-1
C  12  Z0_CONSTAIN                        FAST_TRAK                   0-1
C  13  from alternate ambiguity           FAST_TRAK etc               0-1
C  14  alternate ambiguity uses NULL      FAST_TRAK etc               0-1
C  15  number of hits from loc amb        FAST_TRAK etc               0-63
C  16  |
C  17  |
C  18  |
C  19  |
C  20  -
C  21  1st call of TFXFIT CF              DUFIND                      0-1
C  22  replace Z hits after 1st call      DUFIND                      0-1
C  23  need recall of FAST_TRAK           FAST_TRAK_RECALL...         0-1
C  24  1ST recall of FAST_TRAK            FAST_TRAK_RECALL...         0-1
C  25  2ND recall of FAST_TRAK            FAST_TRAK_RECALL...         0-1
C  26  new representation from recall     FAST_TRAK_RECALL...         0-1
C  27  2nd call of TFXFIT CF              DUFIND                      0-1
C  28  replace Z hits after 2nd call      DUFIND                      0-1
C  29  remove Z0 bias after CF calls      DUFIND                      0-1
C  30
C  31
C  32 ...............................sign bit ; don't use !
C                  ( sign bit used by diagxts/xts_trak for control?? )

C n : JBIT(STAT,n)            STAT(3)    loaded by                   value

C   1  test TFXFIT-LF, normal             DUFIND                      0-1
C   2  test TFXFIT-LF with Z0 constraint  DUFIND                      0-1
C   3  use  TFXFIT-LF with Z0 constraint  DUFIND                      0-1
C   4  boot silicon z hits for curlers    BOOT_SI_Z_HITS              0-1
C   5  limited radius solution, tested    DUFIND                      0-1
C   6  limited radius,    found solution  DUFIND                      0-1
C   7  limited radius solution, selected  DUFIND                      0-1
C   8
C   9
C  10
C  11  track was formed from two curlers  CDJOIN                      0-1
C  12  CDJOIN track died in TF3FIT        CDJOIN                      0-1
C  13  entered TF3FIT                     TF3FIT                      0-1 
C  14  1st kink at VD-DR interfac allowed TF3FIT                      0-1
C  15  1st kink at VD-DR interface killed TF3FIT                      0-1
C  16  started loop over iterations       TF3FIT                      0-1
C  17  2nd kink in DR, activated          TF3FIT                      0-1
C  18  2nd kink in DR, killed             TF3FIT                      0-1
C  19  final fit did   not   converge     TF3FIT                      0-1
C  20  final fit converged, this is good  TF3FIT                      0-1
C  21  number of hits deleted from fit    TF3FIT                      0-7
C  22  |
C  23  -
C  24  number of iterations for convergen TF3FIT                      0-15
C  25  | 
C  26  |
C  27  -
C  28  error number, 0=no error           TF3FIT                      0-15
C  29  |
C  30  |
C  31  -
C  32 ...............................sign bit ; don't use !
#endif
C-----------------------------------------------------------------------
	INTEGER   mstatus
	PARAMETER(mstatus=54)
	INTEGER   kSTfillcs,      kSTfillcf,       kSTfillcd
	PARAMETER(kSTfillcs=    1,kSTfillcf=     2,kSTfillcd=    3)
	INTEGER   kSTcstocf,      kSTcftocd 
        PARAMETER(kSTcstocf=    4,kSTcftocd=     5)
	INTEGER   kSTfetchcs,     kSTfetchcf,      kSTfetchcd
	PARAMETER(kSTfetchcs=   6,kSTfetchcf=    7,kSTfetchcd=    8)

	INTEGER   kstSFseedind
	PARAMETER(kstSFseedind=      1)
	INTEGER   kstALT1seedind
	PARAMETER(kstALT1seedind=    2)
	INTEGER   kstALT2seedind
	PARAMETER(kstALT2seedind=    3)
	INTEGER   kstSFpassnum
	PARAMETER(kstSFpassnum=      4)
	INTEGER   kstSFmergetype
	PARAMETER(kstSFmergetype=    5)
	INTEGER   kstSFmergeind
	PARAMETER(kstSFmergeind=     6)
	INTEGER   kstSFchainnum
	PARAMETER(kstSFchainnum=     7)
	INTEGER   kstFTnhitwirfit
	PARAMETER(kstFTnhitwirfit=   8)
	INTEGER   kstFTrmD0conwf
	PARAMETER(kstFTrmD0conwf=    9)
	INTEGER   kstFTtaddKNKwf
	PARAMETER(kstFTtaddKNKwf=   10)
	INTEGER   kstFTcwKNKwf
	PARAMETER(kstFTcwKNKwf=     11)
	INTEGER   kstFTcwKNKaddc
	PARAMETER(kstFTcwKNKaddc=   12)
	INTEGER   kstFTtaddKNKhf
	PARAMETER(kstFTtaddKNKhf=   13)
	INTEGER   kstFTcwKNKhf
	PARAMETER(kstFTcwKNKhf=     14)
	INTEGER   kstFTcwKNKdbaa
	PARAMETER(kstFTcwKNKdbaa=   15)
C-----------------------------------------------------------------------
	INTEGER   kstFT1D0con
	PARAMETER(kstFT1D0con=      16)
	INTEGER   kstFT1Z0con
	PARAMETER(kstFT1Z0con=      17)
	INTEGER   kstFT1altamb
	PARAMETER(kstFT1altamb=     18)
	INTEGER   kstFT1altambNUL
	PARAMETER(kstFT1altambNUL=  19)
	INTEGER   kstFT1nhitlamb
	PARAMETER(kstFT1nhitlamb=   20)
	INTEGER   kstFT2D0con
	PARAMETER(kstFT2D0con=      21)
	INTEGER   kstFT2Z0con
	PARAMETER(kstFT2Z0con=      22)
	INTEGER   kstFT2altamb
	PARAMETER(kstFT2altamb=     23)
	INTEGER   kstFT2altambNUL
	PARAMETER(kstFT2altambNUL=  24)
	INTEGER   kstFT2nhitlamb
	PARAMETER(kstFT2nhitlamb=   25)
	INTEGER   kstDUtfxfitCF1
	PARAMETER(kstDUtfxfitCF1=   26)
	INTEGER   kstDUrepZhaCF1
	PARAMETER(kstDUrepZhaCF1=   27)
	INTEGER   kstFTRneedrec
	PARAMETER(kstFTRneedrec=    28)
	INTEGER   kstFTRrecFT1
	PARAMETER(kstFTRrecFT1=     29)
	INTEGER   kstFTRrecFT2
	PARAMETER(kstFTRrecFT2=     30)
	INTEGER   kstFTRnewrep
	PARAMETER(kstFTRnewrep=     31)
	INTEGER   kstDUtfxfitCF2
	PARAMETER(kstDUtfxfitCF2=   32)
	INTEGER   kstDUrepZhaCF2
	PARAMETER(kstDUrepZhaCF2=   33)
	INTEGER   kstDUrZ0biasaCF
	PARAMETER(kstDUrZ0biasaCF=  34)
C-----------------------------------------------------------------------
	INTEGER   kstDUtfxfitLFnt
	PARAMETER(kstDUtfxfitLFnt=  35)
	INTEGER   kstDUtfxfitLFzt
	PARAMETER(kstDUtfxfitLFzt=  36)
	INTEGER   kstDUtfxfitLFzu
	PARAMETER(kstDUtfxfitLFzu=  37)
	INTEGER   kstBSZHboot
	PARAMETER(kstBSZHboot=      38)

	INTEGER   kstFTlimitrTEST
	PARAMETER(kstFTlimitrTEST=  39)
	INTEGER   kstFTlimitrSOL
	PARAMETER(kstFTlimitrSOL=   40)
	INTEGER   kstFTlimitrSEL
	PARAMETER(kstFTlimitrSEL=   41)

	INTEGER   kstCDJjoin
	PARAMETER(kstCDJjoin=       42)
	INTEGER   kstCDJdied
	PARAMETER(kstCDJdied=       43)
	INTEGER   kstTF3ent
	PARAMETER(kstTF3ent=        44)
	INTEGER   kstTF3knk1allow
	PARAMETER(kstTF3knk1allow=  45)
	INTEGER   kstTF3knk1kill
	PARAMETER(kstTF3knk1kill=   46)
	INTEGER   kstTF3stLOOPit
	PARAMETER(kstTF3stLOOPit =  47)
	INTEGER   kstTF3knk2activ
	PARAMETER(kstTF3knk2activ=  48)
	INTEGER   kstTF3knk2kill
	PARAMETER(kstTF3knk2kill=   49)
	INTEGER   kstTF3notCONV
	PARAMETER(kstTF3notCONV=    50)
	INTEGER   kstTF3yesCONV
	PARAMETER(kstTF3yesCONV=    51)
	INTEGER   kstTF3hitsdel
	PARAMETER(kstTF3hitsdel=    52)
	INTEGER   kstTF3iter
	PARAMETER(kstTF3iter=       53)
	INTEGER   kstTF3err
	PARAMETER(kstTF3err=        54)
C-----------------------------------------------------------------------












