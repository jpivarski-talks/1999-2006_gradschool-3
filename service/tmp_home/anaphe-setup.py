#!/usr/bin/env python
# 
# Author: Andreas Pfeiffer, CERN/IT (andreas.pfeiffer@cern.ch)
#
# Creation date: 13-jan-2002
#
# $Revision: 1.29 $
#
# Setup script for Anaphe s/w.
#
# This script initialises the configuration for Anaphe s/w then
# checks whether the packages for a given version of Anaphe are
# already installed and installs the missing ones from the CERN
# repository (this requires network access).
#
# As this will use publicly accessible pages, only the license-free
# part of the s/w is downloaded for now.
#
# example for usage:
#
#  anaphe-setup.py --top ~/tmp/anaphe --vers 5.0.6 --aidaDir ~/tmp/AIDA --aidaVers 3.0.0 --verb 3 
#
# will install the license free version of version 5.0.6 of the s/w into 
# the ~/tmp/anaphe directory. In addition it will install the AIDA files
# for AIDA 3.0.0 ("AIDA 3.0") into ~/tmp/AIDA (if not yet there).
#
# --------------------------------------------------------------------------------
#
# Version 1.29,ap, 18-aug-2003 : fixing SuSE 8.x
#
# Version 1.28,ap, 18-aug-2003 : adding installation for libshift.so, updated Anaphe version to 5.0.6
#
# Version 1.27,ap, 02-dec-2002 : no diff to 1.26 ?
#
# Version 1.26,ap, 02-dec-2002 : fixed bug for redhat72/gcc-2.95.3
#
# Version 1.25,ap, 28-nov-2002 : make sure the names of the scripts in ~/bin/ and share/scripts
#                                are different !
#
# Version 1.24,ap, 28-nov-2002 : re-mapped suse 8.0 to redhat72 (reported by Giovanni Santin)
#                                added check for PYTHONPATH for csh script
#
# Version 1.23,ap, 27-nov-2002 : updated setting of ANAPHE_REL_DIR in ~/bin/setupAnaphe.csh
#                                sourcing of shared setupAnaphe.csh instead of sharedstart.csh
#                                linking sharedstart scripts to ~/bin/setupAnaphe scripts
#
# Version 1.22,ap, 25-nov-2002 : fixed wrong file name (setupAnapheCommon.csh -> setupAnaphe.csh)
#
# Version 1.21,ap, 21-nov-2002 : added creation of setupAnaphe.csh in ~/bin, check and
#                                create ~/bin if not existing  ...
#
# Version 1.20,ap, 12-nov-2002 : added SuSE 8.0 (gcc 2.95.3) as redhat72, aida-config now
#                                calls aida-config.py
#
# Version 1.19,ap, 28-oct-2002 : fixed bug in removing "python" (line 746)
#
# Version 1.18,ap, 28-oct-2002 : added creation of setupAnaphe in ~/bin 
#
# Version 1.17,ap, 22-oct-2002 : corrected path for AIDA_DIR in ~/bin/aida-config generation
#                                (bug reported by Alex Howard)
#
# Version 1.16,ap, 18-oct-2002 : fixed bug in creating the links in python/ (around line 735)
#                                changed from isInstalled to isRequested, otherwise key error
#                                for Lizard on first round ...
#
# Version 1.15,ap, 17-oct-2002 : check for existence of "OS"
#
# Version 1.14,ap, 16-oct-2002 : more fixes for Anaphe version 5 and AIDA 3.0.0; 
#                                removed download of PubDom (done with release)
#                                check for correct AIDA version (3.x for 5.x)
#
# Version 1.13,ap, 16-oct-2002 : updates for Anaphe version 5 and AIDA 3.0.0; force install 
#                                of AIDA if aida-config is existing (bug reported by Alex Howard)
#
# Version 1.12,ap, 04-sep-2002 : homogenized lizard and aida-config scripts, bash through env
#
# Version 1.11,ap, 04-sep-2002 : more fixes after debugging on pcgeant02 in Genova
#                                dealing with 'sh' in finishInstall (OS=...; export OS)
#                                adding PLATF to lizard scripts
#
# Version 1.10,ap, 04-sep-2002 : fixed various typos which prevented 1.9 from running
#
# Version 1.9, ap, 04-sep-2002 : added try clause around compiler checking in findCompSolaris
#
# Version 1.8, ap, 04-sep-2002 : fixed typo (specdir instead of specDir) in
#                                install/checking links (thanks to Michael Dressel)
#
# Version 1.7, ap, 02-aug-2002 : fixed bug preventing proper running
#
# Version 1.6, ap, 30-jul-2002 : separated out checking of space; corrected treatment
#                                of AFS file systems for space checking.
#                                added flags aidaDir and aidaVersion to download and 
#                                install AIDA files. added aida-config conveniency file 
#                                in ~/bin.  Commandline args now override env-vars.
#
# Version 1.5, ap, 25-feb-2002 : fixed missing decl. of _comp (bug reported by Alex 
#                                S. Howard) and added full host name for pyton 1.5
#
# Version 1.4, ap, 04-feb-2002 : fixed path expansion (use of abspath() instead of
#                                normpath(), fixed format of printout for fetching
#
# Version 1.3, ap, 04-feb-2002 : renumbered to be consistent with cvs versions
#
# Version 1.2, ap, 02-feb-2002 : expand directory names from user
#                                added checks for size in dirs (static numbers)
#                                more work on the bin/lizard script (thanks to
#                                Daniel Strul <Daniel.Strul@iphe.unil.ch> for
#                                reporting !)
#
# Version 1.1, ap, 31-jan-2002 : added octet-stream for type from urlfetch,
#                                added '-sec' to version for egcs-1.1.2 
#                                (reported by Giovanni.Santin@cern.ch, thanks!),
#                                more fixes for the final installation, tested
#                                on pcgeant02 INFN Genova (disk usage: 42 MB in
#                                tmpDir, 150 MB in topDir) ...
#
# Version 1.0, ap, 31-jan-2002 : fixed bug when using tmpDir (was not properly set)
#                                (reported by Giovanni.Santin@cern.ch, thanks!),
#                                removed download for Python2.0 (comes with PubDom)
#
# Version 0.1, ap, 13-jan-2002 : initial version
#
# --------------------------------------------------------------------------------
#
# toDo:
#
#  - check if fs is afs, then do space check differently !
#
#  - get sizes from server for checks on free space
#  - check for libs + includes or .done file to see what needs
#    to be installed
#  - check if transfer is ok (size of file)
#  - remove tgz files after successful installation
#
#  - anaphe-config script into ~/bin/
#  - use of config-file (also for other scripts (and lizard))
#
#  - if user is privileged: prompt for running scripts (security,
#    the script could be faked during transmission)
#  - checksum for scripts ?
#  - use scp to get licensed s/w ???
#
# --------------------------------------------------------------------------------
#

import os,sys,string
import getopt
import urllib

# ================================================================================

def guessPlatform() :
    opSys = None
    comp  = None

    # simple guesser ...
    if ( os.uname()[0] == "SunOS" ) :
        opSys = "sun4x_58"  # for now ...
        chOutErr = os.popen("CC -V", 'r')
        compString = string.join(chOutErr.readlines())[:-1]  # skip \n
        chOutErr.close()
        #print "got: '" + compString + "'"
        comp = "CC-" + compString.split()[7]

    elif ( os.uname()[0] == "Linux" ) :
        issue = open("/etc/issue", 'r')
        lines = string.join(issue.readlines())
        issue.close()
        if ( string.find(lines, "Red Hat") != -1 and
             string.find(lines, "7.3") != -1 ) : opSys = "redhat73"
        elif ( string.find(lines, "Red Hat") != -1 and
             string.find(lines, "7.2") != -1 ) : opSys = "redhat72"
        elif ( string.find(lines, "Red Hat") != -1 and
             string.find(lines, "6.1") != -1 ) : opSys = "redhat61"
        elif ( string.find(lines, "Red Hat") != -1 and
             string.find(lines, "9.0") != -1 ) : opSys = "redhat73"
        elif ( string.find(lines, "SuSE") != -1 and
             string.find(lines, "7.2") != -1 ) : opSys = "redhat61"
        elif ( string.find(lines, "SuSE") != -1 and
             string.find(lines, "8.0") != -1 ) : opSys = "redhat73"
        elif ( string.find(lines, "SuSE") != -1 and
             string.find(lines, "8.1") != -1 ) : opSys = "redhat73"
        elif ( string.find(lines, "SuSE") != -1 and
             string.find(lines, "8.2") != -1 ) : opSys = "redhat73"
        chOutErr = os.popen("g++ --version", 'r')
        compString = string.join(chOutErr.readlines())[:-1]  # skip \n
        chOutErr.close()
        #print "got: '" + compString + "'"
        if ( string.find(compString, "egcs-2.91.66")  != -1 ) : comp = "egcs_1.1.2"
        if ( string.find(compString, "2.95.2")        != -1 ) : comp = "gcc-2.95.2"
        if ( string.find(compString, "2.95.3")        != -1 ) : comp = "gcc-2.95.2"
        if ( string.find(compString, "g++ (GCC) 3.2") != -1 ) : comp = "gcc-3.2"

    return (opSys, comp)

# ================================================================================

def replaceLine(file, pat, sub) :

    import re
    
    ## print "replacing '" + pat + "' by '" + sub + "' in ", file

    inFile = open(file)
    lines = inFile.readlines()
    inFile.close()

    outFile = open(file,"w")
    for line in lines :
        newLine = re.sub(pat, sub, line)
        outFile.write(newLine)

    outFile.close()
    return

# ================================================================================

class SetupParams :
    "keep some common parameters"
    
    def __init__ (self) :
        self.versSel  = None
        self.tDirSel  = None
        self.platSel  = None

        self.tmpDirSel = "/tmp/"
        self.aidaDirSel  = None
        self.aidaVersSel = None
        self.addQt       = None
        self.prLvlSel = 3

        return

    def topDir(self)  : return self.tDirSel

    def version(self) : return self.versSel

    def platf(self)   : return self.platSel

    def tempDir(self) : return self.tmpDirSel

    def aidaDir(self) : return self.aidaDirSel
    def aidaVers(self) : return self.aidaVersSel

    def printLevel(self) : return self.prLvlSel
    
# ================================================================================

class Base :
    "Base class for the Checker and Installer "

    # --------------------------------------------------------------------------------

    def __init__ (self) :
        self.verbosity = -1

        self.startDir = os.getcwd()
        
        self._os    = None
        self._platf = None
        self._top   = None
        self._aidaDir  = None
        self._aidaVers = None
        self._vers  = None

        return
    
    def setPar(self, params) :
        self.setup = params
        self.setVerbose(params.printLevel())

        return

    # --------------------------------------------------------------------------------

    def verbose (self) :
        return int(self.verbosity)

    # --------------------------------------------------------------------------------

    def setVerbose (self, val) :
        self.verbosity = val
        return
    
    # --------------------------------------------------------------------------------

    def envOk(self) :
        "check whether all the needed variables are set"
        if ( not self._os    or
             not self._platf or
             not self._top   or
             not self._aidaDir   or
             not self._aidaVers  or
             not self._vers  ) :
            return (1==0)
        else :
            return (1==1)
        
# ================================================================================

class Checker(Base) :
    "Checks the environment variables and 'guesses' values if needed."
    
    def __init__ (self, params) :
        apply(Base.__init__, (self,) )
        self.setPar(params)
        
        return

    # --------------------------------------------------------------------------------

    def findEnvVar(self, varRequested) :
        "check for environment variable 'varRequested' and return it (None if not found)"
        _var = None
        try :
            _var = os.environ[varRequested]
        except KeyError:
            pass
        return _var

    # --------------------------------------------------------------------------------

    def findCompLinux(self) :
        "finds the compiler version for Linux using 'g++ --version' "
        _cv = os.popen("g++ --version").readlines()
        _comp = string.strip(_cv[0])

        if ( _comp == "egcs-2.91.66" ) : _comp = "egcs_1.1.2"
        elif ( _comp == "2.95.2"     ) : _comp = "gcc-2.95.2"
        else :
            if (self.verbose() > 1) :
                print "WARNING: Checker> unknown compiler version :", _comp,
                print " " + str(_cv) + " "
                print "                  selecting default compiler version gcc-2.95.2 "
            _comp = "gcc-2.95.2" # set 2.95.2 as default or (better ?): raise VersionError

        return _comp

    # --------------------------------------------------------------------------------

    def findCompSolaris(self) :
        """
        finds the compiler version for Linux using 'CC -V', as
        this is written to stderr, we need to capture this in a
        temporary file ...
        """

        tmpFileName = "/tmp/_compVers.tmp"
        os.system("/opt/SUNWspro/bin/CC -V 2>" + tmpFileName)
        _cvFile = open(tmpFileName)
        _cvRaw = _cvFile.readlines()
        _cvFile.close()
        os.unlink(tmpFileName)
        _cv = string.split( str(_cvRaw) )

        # toDo: check for other versions and their behaviour ([7] ?)
        try:
            _comp = "CC-" + _cv[7]
        except:
            print "ERROR: Checker> can't determine compiler from : '" + str(_cv) + "'"
            raise
        
        return _comp

    # --------------------------------------------------------------------------------

    def fromArgs (self) :

        if (self.verbose() > 3) : print "checking args from commandline "

        # override with user selected args:
        if ( self.setup.platf()    != None ) : self._platf    = self.setup.platf()
        if ( self.setup.topDir()   != None ) : self._top      = self.setup.topDir()
        if ( self.setup.version()  != None ) : self._vers     = self.setup.version()
        if ( self.setup.aidaDir()  != None ) : self._aidaDir  = self.setup.aidaDir()
        if ( self.setup.aidaVers() != None ) : self._aidaVers = self.setup.aidaVers()
        
        return
    
    # --------------------------------------------------------------------------------

    def chkEnvVars (self) :
        "check for the existence of OS, PLATF, the top-level directory, and the Anaphe version"

        self._unam = string.split( os.popen("uname -a").readlines()[0] )

        # check for "OS"
        self._os = self.findEnvVar("OS")
        if ( not self._os ) :
            self._os =  os.uname()[0]

        # check for "PLATF", if not passed on commandline
 	_comp = None
	if (self.setup.platf() != None ) :
	   self._platf = self.setup.platf()
	else :
           self._platf = self.findEnvVar("PLATF")
           (opSys, comp) = guessPlatform()
           if ( opSys == None or comp == None ) :
               print "FATAL: Checker> unknown OS/compiler : ", opSys, comp
               sys.exit(-1)
           self._platf = opSys + "/" + comp

        # check for "LHCXXTOP"
        self._top = self.findEnvVar("LHCXXTOP")

        # check for "LHCXXVERSION"
        self._vers = self.findEnvVar("LHCXXVERSION")

	# check for AIDA_DIR in environment
	self._aidaDir = self.findEnvVar("AIDA_DIR")

	# check for AIDA_VERSION in environment
	self._aidaVers = self.findEnvVar("AIDA_VERSION")

	# overwrite from command-line arguments
        self.fromArgs()

        if ( not self.envOk() ) :
	   print "FATAL: not all parameters set correctly, aborting. [", 
	   print self._os, self._platf, self._top, self._vers, self._aidaDir, self._aidaVers, "]"
	   sys.exit(-1)
    
        if ( not self.envOk() ) :
            if (self.verbose() > 3) : print "env NOT ok after checking args !?!??!?!? "

	# the following only if platf was not a command line arg !
        if ( self.setup.platf() != None and _comp == "egcs_1.1.2" ) :
            if ( string.find(self._vers, "-sec") == -1 ) :
                print "adding '-sec' to version number for egcs compiler "
                self._vers = self._vers + "-sec"

        return [self._os, self._platf, self._top, self._vers, self._aidaDir, self._aidaVers]
    
# ================================================================================

class Installer(Base) :

    def __init__ (self, params) :
        apply(Base.__init__, (self,) )
        self.setPar(params)

        self.newPackages = {}

        self.items = []
        self.itemVersions = {}

        self.itemsRequested = []
        self.itemRequestedVersions = {}

        self.tarFiles = []

        self.tmpDir = self.setup.tempDir()

        self._host = "pcitapiww.cern.ch"

        # so far, only the license free version is supported
        self._licFree = 1

        #-toDo: get these (in MB) from server ...
        self.tmpSpaceMin = 80
        self.topSpaceMin = 180

        return

    # --------------------------------------------------------------------------------

    def select(self, _item, _ver) :
        if (self.verbose() > 3) : print _item, "selected for installation"
        self.items.append(_item)
        self.itemVersions[_item] = _ver
        return

    # --------------------------------------------------------------------------------

    def isRequested(self, _item, _ver) :
        self.itemsRequested.append(_item)
        self.itemRequestedVersions[_item] = _ver
        return

    # --------------------------------------------------------------------------------

    def setEnvVars(self, varList) :
        self._os       = varList [0]
        self._platf    = varList [1]
        self._top      = varList [2]
        self._vers     = varList [3]
        self._aidaDir  = varList [4]
        self._aidaVers = varList [5]
        self._aidaAlreadyThere = None

        self._aidaDir  = self._aidaDir + self._aidaVers
        if (not os.path.exists( self._aidaDir )) :
            ret = os.system(" mkdir -p " + self._aidaDir)
            if (ret != 0) :
                print "FATAL: cannot create _aidaDir:", self._aidaDir, " aborting."
                sys.exit(-1);
        else :
            if ( len(os.listdir(self._aidaDir)) > 0 ) :
                self._aidaAlreadyThere = 1

        self.tmpDir = self.setup.tempDir()

        if (not os.path.exists( self.tmpDir )) :
            ret = os.system(" mkdir -p " + self.tmpDir)
            if (ret != 0) :
                print "FATAL: cannot create tmpDir:", self.tmpDir, " aborting."
                sys.exit(-1);
        
        if (not os.path.exists( self._top )) :
            ret = os.system(" mkdir -p " + self._top)
            if (ret != 0) :
                print "FATAL: cannot create topDir:", self._top, " aborting."
                sys.exit(-1);

        self.checkTmpSpace()
        self.checkTopSpace()

        return
    
    # --------------------------------------------------------------------------------

    # f_bsize, f_frsize, f_blocks, f_bfree, f_bavail, f_files, f_ffree, f_favail, f_flag, f_namemax.

    def getSpace(self, dir) :
        statList = os.statvfs(dir)
	freeSpace = 0
	if ( statList[2] == statList[3] and
	     statList[2] == statList[4] and
	     statList[2] == statList[5] and
	     statList[2] == statList[6] and
	     statList[2] == statList[7] and
	     statList[2] == 9000000 ) :
	   freeSpace = self.getSpaceAFS(dir)
        else :
           freeSpace = ( statList[0] / (1024.*1024.) ) * statList[3]

        return freeSpace
    
    # --------------------------------------------------------------------------------

    def getSpaceAFS(self, dir) :
	# f_bsize, f_frsize, f_blocks, f_bfree, f_bavail, f_files, f_ffree, f_favail, f_flag, f_namemax.
	# (1024, 1024, 9000000, 9000000, 9000000, 9000000, 9000000, 9000000, 0, 256)
        statList = os.popen("fs lq " + dir).readlines()[1]
	(name, quota, used, relUsed, relPart) = string.split(statList)
        freeSpace = ( float(quota) - float(used) ) / 1024.
        return freeSpace
    
    # --------------------------------------------------------------------------------

    # f_bsize, f_frsize, f_blocks, f_bfree, f_bavail, f_files, f_ffree, f_favail, f_flag, f_namemax.

    def checkTmpSpace (self) :
        "check the free space in tmpDir"

        freeSpace = self.getSpace ( self.tmpDir )
        if ( self.verbose() > 2 ) : print "space available in ", self.tmpDir, " : ", freeSpace, "MB"
        if ( freeSpace < self.tmpSpaceMin ) :
            print "FATAL: not enough space in ", self.tmpDir,
            print " need at least: ", self.tmpSpaceMin,
            print "MB available: ", freeSpace, "MB"
            print "       please select a larger directory with --tmpDir <yourTmpDir>."

        return
    # --------------------------------------------------------------------------------

    def checkTopSpace (self) :
        "check the free space in topDir"

        freeSpace = self.getSpace(self._top)
        if ( self.verbose() > 2 ) : print "space available in ", self._top, " : ", freeSpace, "MB"
        if ( freeSpace < self.topSpaceMin ) :
            print "FATAL: not enough space in ", self._top,
            print " need at least: ", self.topSpaceMin,
            print "MB available: ", freeSpace, "MB"
            print "       please select a different install directory."

        return

    # --------------------------------------------------------------------------------

    def getPackageList(self) :

        versURL = "http://" + self._host + "/anaphe/download/Versions-"
        versURL = versURL + self._vers + "-" + string.replace(self._platf,"/","-") + ".txt"

        versHandle = urllib.urlopen(versURL)
        versions = versHandle.readlines()
        versHandle.close()
        if ( self.verbose() > 6 ) : print versions

        pkgList = {}
        if ( string.find( str(versions), "404 Not Found" ) != -1 ) :
            if ( self.verbose() > 3 ) : print versions
            print "\nFATAL: Installer> could not find Versions file for version ", self._vers, "/ platform", self._platf
            print
            sys.exit(-1)

        for item in versions :
            if (string.find( item, ":") == -1) : continue
            [pkg, ver] = string.split( item, ":")
            pkg = string.strip(pkg)
            ver = string.strip(ver)
            pkgList[pkg] = ver

        if ( self.verbose() > 6 ) : print pkgList

        return pkgList
    
    # --------------------------------------------------------------------------------

    def updateVersion(self) :

        specDir = self._top + "/specific/" + self._platf

        # if not existing, try to (recursively) create the dir
        if ( not os.path.exists(specDir) ) : os.makedirs(specDir)

        os.chdir(specDir)

        self.initVersDir()

        self.newPackages = self.getPackageList()

        if ( os.path.exists(self._vers) ) :
            print "WARNING: Installer> Version directory already existing."
        else :
            if ( self.verbose() > 1 ) :
                print "INFO: Installer> will create directories for Version", self._vers
            #self.initVersDir()

        # (pseudo-)packages to be ignored
        ignoreList = [ "AIDA_Examples", "AIDA_Examples_HepExp", "AIDA_Examples_HBook" ]
        
        # list of public domain packages (in PublicDomain)
        pubDomList = [ "expat", "SWIG", "Python" ]

        # list of packages where licenses are required explicitely 
        licenseList = [ "Objectivity", "Nag_C", "ObjectSpace", "OpenInventor" ]

        if ( self.verbose() > 0 ) : print "checking packages "
        
        for pkg in self.newPackages.keys() :

            if ( pkg == "ObjectSpace" ) : continue
            if ( pkg == "Qt" and self.setup.addQt == None ) : continue
            if ( pkg in ignoreList    ) : continue
            if ( pkg in pubDomList    ) : continue
            if ( pkg in licenseList   ) : continue

            ver = self.newPackages[pkg]
            self.isRequested(pkg,ver)
            if ( not os.path.exists(pkg) ) :
                if ( self.verbose() > 2 ) : print "checking '" + pkg + "'"
                else : print ".",
                self.select(pkg, ver)
            elif ( not os.path.exists(pkg + "/" + ver) ) :
                if ( self.verbose() > 2 ) : print "checking '" + pkg + "/" + ver + "'"
                else : print ".",
                self.select(pkg, ver)
            else :
                if ( self.verbose() > 3 ) :
                    print "INFO: Installer> version", ver, "of package", pkg, "already existing."

        if ( self.verbose() < 3) : print " done."

        # return to starting directory
        os.chdir(self.startDir)
        
        return

    # --------------------------------------------------------------------------------
    def getPackage(self, pkg, vers) :

        if ( self.verbose() > 2 ) : print " ... ",
        else : print ".",
        sys.stdout.flush()
        
        tarFileName = pkg + "-" + vers + "-" + string.replace(self._platf,"/","-") + ".tgz"
        tarFileURL = "http://" + self._host + "/anaphe/export/" + self._platf + "/byPackage/"
        tarFileURL = tarFileURL + tarFileName

        if (not self.tmpDir) : 
            self.tmpDir = "/tmp/Anaphe/download/"
            
        if ( self.verbose() > 2 ) : print pkg, vers,

        fileName = self.tmpDir + tarFileName

        if ( os.path.exists(fileName) ) :
            if ( self.verbose() > 2 ) : print "file already fetched "
            self.tarFiles.append(tarFileName)
            return
        
        (fileNameRet, info) = urllib.urlretrieve (tarFileURL, fileName)

        if (info.gettype() == "text/html") :
            tmpFile = open(fileNameRet)
            tmpTxt = tmpFile.readlines()
            tmpFile.close()
            if ( string.find( str(tmpTxt), "404 Not Found" ) != -1 ) :
                print "\nFATAL: Installer> could not find tar file :", tarFileName
                print
                os.remove(fileName)
                sys.exit(-1)
        elif (info.gettype() != "application/x-tar" and
              info.gettype() != "application/octet-stream" ) :
            print "\nERROR: Installer.getPackage> got unknown content-type: ", info.gettype()
            print "      ", info
            sys.exit(-2)

        if ( self.verbose() > 5 ) : print "urlretrieve got: ", fileNameRet, info

        self.tarFiles.append(tarFileName)
        
        if ( self.verbose() > 2) : print " fetched."
        return

    # --------------------------------------------------------------------------------

    def install(self) :

        for item in self.items :
            self.getPackage(item, self.itemVersions[item])
        
        print "\ninstalling packages \n"

        # specific part
        specDir = self._top + "/specific/" + self._platf
        os.chdir(specDir)

        tarLogFile = self.tmpDir + "/tarLog"
        if ( os.path.exists(tarLogFile) ) : os.unlink(tarLogFile)
        for file in self.tarFiles :
            if ( self.verbose() > 2 ) : print " ... ", file,
            else : print ".",
            sys.stdout.flush()
            
            ret = os.system("tar zxf " + self.tmpDir + file + " 2>>" + tarLogFile )
            if ( ret == 512 ) :  # this catches "Member name contains `..'
	      pass
            elif ( ret != 0 ) :  # this catches ^C (ret == 2)
                print "error during installation, aborting. (return code:", ret, " log info:", tarLogFile, ")"
                sys.exit(-1)

            if ( self.verbose() > 2 ) :
                print "done."
                sys.stdout.flush()
            
        # PublicDomain packages ... (from 5.0.0 onward, they are in a specific package)
        if ( int(string.split(self._vers, ".")[0]) < 5 ) :
           self.getPackage("PubDom", self._vers)
           fullScriptName = "PubDom-" + self._vers + "-" + string.replace(self._platf,"/","-")
           # ... untar it ...
           ret = os.system("tar zxf " + self.tmpDir + fullScriptName + ".tgz" + " 2>>" + tarLogFile )
           print "done."
        
        if ( self.verbose() < 3) : print " done."

        print "\nfetching scripts to finalise installation"
        # shared scripts:
        self.getPackage("LHCXX-share", self._vers)
        sharDir = self._top + "/share/"
        if ( not os.path.exists(sharDir) ) : os.makedirs(sharDir)
        os.chdir(sharDir)
        os.system("tar zxf " + self.tmpDir + "LHCXX-share-" + self._vers + "-" + string.replace(self._platf,"/","-") + ".tgz")
        
        # finish installation ... get the script
        self.getPackage("finishInstall", "")
        fullScriptName = "finishInstall--" + string.replace(self._platf,"/","-")
        # ... untar it ...
        os.system("tar zxf " + self.tmpDir + fullScriptName + ".tgz")
        # ... and call the script with the arguments
        #-toDo: the export OS part should go into the script
        os.system("OS=Linux ; export OS ; ./" + fullScriptName + " " + self._vers + " " + self._top + " " + self._platf )

	# check whether these links are there:
	if ( self.verbose() < 3) : print " checking python links"
	os.chdir( specDir + "/" + self._vers )
        if ( os.path.exists("python") ) :
           os.system("rm -rf python/ ")
        os.mkdir("python")
	os.chdir( "python" )
	for link in ["bin", "help", "lib", "src", "startUpFiles"] :
	   if ( os.path.exists(link) ) :
              os.system(" rm -f " + link)
           print "linking " + specDir + "/Lizard/" + self.itemRequestedVersions["Lizard"] + "/python/" + link
           os.symlink( specDir + "/Lizard/" + self.itemRequestedVersions["Lizard"] + "/python/" + link, link)

        os.chdir("../bin")
        for link in os.listdir(".") :
            if ( string.find(link, "python") ) :
               os.remove(link)
        os.system("ln -s ../python/bin/* .")

        #-ap: toCheckAndDo
        ## # check where bash is located here and replace /usr/local/bin/bash in scripts:
        ## bashLocation = os.system("which bash")  !!! doesn't work, returns 0
        ## scriptFileName = ???
        ## replaceLine(scriptFileName, "/usr/local/bin/bash", bashLocation)

	# change these lines from the sharedstart (we are non-licensed anyways, it corrupts the output of the aida-config script)
	scriptFileName = self._top + "/share/LHCXX/" + self._vers + "/install/sharedstart.csh"
 	replaceLine(scriptFileName, 'echo "WARNING:  Objectivity bin or lib directory not accessible"', 'echo -n ""; # echo "WARNING:  Objectivity bin or lib directory not accessible"')
	scriptFileName = self._top + "/share/LHCXX/" + self._vers + "/install/sharedstart.sh"
 	replaceLine(scriptFileName, 'echo "WARNING:  Objectivity bin or lib directory not accessible"', 'echo -n ""; # echo "WARNING:  Objectivity bin or lib directory not accessible"')

        # fix this ... :-(
	scriptFileName = self._top + "/share/LHCXX/" + self._vers + "/scripts/setupAnaphe"
 	replaceLine(scriptFileName, 'echo "rh61"', '# echo "rh61"')
        replaceLine(scriptFileName, 'PLATF="redhat61/gcc-2.95.2"   # for SuSE 7.2 ... :-\)',
                                    'PLATF=$PLATF"gcc-2.95.2"  ')
        
        # now go to shared install/ directory, and link the sharedstart scripts to
        # point to the ../scripts/setupAnaphe scripts ...
        os.chdir(self._top + "/share/LHCXX/" + self._vers + "/install")
        os.rename("sharedstart.csh", "sharedstart.csh-old")
        os.rename("sharedstart.sh", "sharedstart.sh-old")
        if ( os.path.exists( "sharedstart.sh" ) ) : os.unlink("sharedstart.sh")
        os.symlink(os.environ["HOME"]+"/bin/setupAnaphe", "sharedstart.sh")
        if ( os.path.exists( "sharedstart.csh" ) ) : os.unlink("sharedstart.csh")
        os.symlink(os.environ["HOME"]+"/bin/setupAnaphe.csh", "sharedstart.csh")

        # rename scripts in ../scripts/ to a different name than in ~/bin !
        os.chdir(self._top + "/share/LHCXX/" + self._vers + "/scripts")
        if ( os.path.exists( "setup-Anaphe" ) ) : os.unlink("setup-Anaphe")
        os.rename("setupAnaphe", "setup-Anaphe")
        if ( os.path.exists( "setup-Anaphe.csh" ) ) : os.unlink("setup-Anaphe.csh")
        os.rename("setupAnaphe.csh", "setup-Anaphe.csh")

        # return to starting directory
        os.chdir(self.startDir)

        self.writeConfig()

        return

    # --------------------------------------------------------------------------------

    def writeConfig(self) :

        print "writing config-file"

        homeDir = os.environ["HOME"]
        fileName = homeDir + "/.anaphe/config"
        if ( not os.path.exists( homeDir + "/.anaphe" ) ) :
            os.mkdir ( homeDir + "/.anaphe" )
            
        file = open(fileName,"w")
        file.write ("[Anaphe]\n")
        file.write ("version = "     + str( self._vers    ) + "\n")
        file.write ("platf = "       + str( self._platf   ) + "\n")
        file.write ("licenseFree = " + str( self._licFree ) + "\n")
        file.write ("topDir = "      + str( self._top     ) + "\n")
        file.write ("\n")
        file.write ("[AIDA]\n")
        file.write ("dir = "     + str( self._aidaDir) + "\n")
        file.write ("version = "     + str( self._aidaVers) + "\n")
        file.write ("\n")
        file.close()


    # --------------------------------------------------------------------------------

    def writeConfigNotFor152(self) :
        # the ConfigParser of Python 1.5.2 is not usable for this ...
        import ConfigParser

        print "writing config-file"

        cfp = ConfigParser.ConfigParser()
        cfp.add_section("Anaphe")
        cfp.set("Anaphe", "version", self._vers)
        cfp.set("Anaphe", "platf"  , self._platf)
        cfp.set("Anaphe", "topDir" , self._top)
        cfp.set("Anaphe", "licenseFree" , self._licFree)

        homeDir = os.environ["HOME"]
        fileName = homeDir + "/.anaphe/config"
        file = open(fileName,"w")
        cfp.write(file)
        file.close()

        return

    # --------------------------------------------------------------------------------

    def scripts(self) :

        homeDir = os.environ["HOME"]
        binDir = homeDir + "/bin"
        if ( not os.path.exists(binDir) ) :
            print "cannot install scripts, directory", binDir, "not found."
            return

        startDir = os.getcwd()

        if ( not os.path.exists(homeDir + "/bin") ) :
            os.mkdir(homeDir + "/bin")

        os.chdir(homeDir + "/bin" )

	self.lizScript()
	self.aidaConfScript()
	self.setupScript()
	self.setupScriptCsh()

        os.chdir(startDir)

	return

    # --------------------------------------------------------------------------------

    def commonScript(self, fileHandle) :

        fileHandle.write("#!/usr/bin/env bash\n\n")
        fileHandle.write("# This file has been created automatically, DO NOT EDIT !\n\n")
        fileHandle.write("export OS=Linux\n\n")
        fileHandle.write("export PLATF=" + self._platf + "\n\n")
        if ( self._aidaVers == "2.2.1" ) :
            fileHandle.write("export AIDA_DIR="+self._aidaDir+"/cpp"+" \n\n")
        else :
            fileHandle.write("export AIDA_DIR="+self._aidaDir+"/AIDA/3.0/src/cpp"+" \n\n")
        fileHandle.write("export ANAPHETOP="+self._top+" \n\n")
        fileHandle.write("source "+ self._top + "share/LHCXX/" + self._vers + "/scripts/setup-Anaphe\n")
        fileHandle.write("ANAPHESPECDIR=$ANAPHETOP/specific/$PLATF\n\n")
        fileHandle.write("export PYTHONPATH=$ANAPHESPECDIR/PublicDomainPackages/"+self.itemRequestedVersions["PublicDomainPackages"]+"/lib:$PYTHONPATH \n\n")

        fileHandle.write("export PATH=$ANAPHESPECDIR/PublicDomainPackages/"+self.itemRequestedVersions["PublicDomainPackages"]+"/grace/bin:$PATH \n\n")
        fileHandle.write("export PATH=$ANAPHESPECDIR/PublicDomainPackages/"+self.itemRequestedVersions["PublicDomainPackages"]+"/bin:$PATH \n\n")
        fileHandle.write("export GRACE_HOME=$ANAPHESPECDIR/PublicDomainPackages/"+self.itemRequestedVersions["PublicDomainPackages"]+"/grace \n\n")
        fileHandle.write("export ANAPHE_REL_DIR=$ANAPHETOP/specific/$PLATF/" + self._vers + "\n\n")
        fileHandle.write("export LHCXX_REL_DIR=$ANAPHE_REL_DIR\n\n")

        return

    # --------------------------------------------------------------------------------

    def lizScript(self) :

        print "installing lizard script in ~/bin/"

        if ( os.path.exists("lizard") ) :
            print "renaming existing file lizard to lizard.orig"
            os.system("mv lizard lizard.orig")
        lizFile = open("lizard", 'w')

	self.commonScript(lizFile)

        lizFile.write("$LHCXX_REL_DIR/bin/startLizard.sh $*\n\n")
        lizFile.write("\n")
        lizFile.close()

        os.chmod("lizard",0755)
        
        return

    # --------------------------------------------------------------------------------

    def aidaConfScript(self) :

        print "installing aida-config script in ~/bin/"

        if ( os.path.exists("aida-config") ) :
            print "renaming existing file aida-config to aida-config.orig"
            os.system("mv aida-config aida-config.orig")
        lizFile = open("aida-config", 'w')

	self.commonScript(lizFile)

        lizFile.write("$LHCXX_REL_DIR/python/bin/aida-config.py $*\n\n")
        lizFile.write("\n")
        lizFile.close()

        os.chmod("aida-config",0755)
        
        return

    # --------------------------------------------------------------------------------

    def setupScript(self) :

        print "installing setupAnaphe script in ~/bin/"

        if ( os.path.exists("setupAnaphe") ) :
            print "renaming existing file setupAnaphe to setupAnaphe.orig"
            os.system("mv setupAnaphe setupAnaphe.orig")
        lizFile = open("setupAnaphe", 'w')

	self.commonScript(lizFile)

        lizFile.write("\n")
        lizFile.close()

        os.chmod("setupAnaphe",0755)
        
        return

    # --------------------------------------------------------------------------------

    def getShellSuffix(self) :
        sfx = ""
        try :
            shellUsed = os.environ["SHELL"]
        except:
            print "cannot determine the shell you are using, default will be bash (and the like)"
            shellUsed = "bash"
            
        if ( string.find(shellUsed, "bash") != -1 or
             string.find(shellUsed, "ksh" ) != -1 or
             string.find(shellUsed, "zsh" ) != -1 or
             string.find(shellUsed, "tcsh" ) != -1 ) :
            sfx = "sh"
        else :
            sfx = "csh"
        return sfx

    # --------------------------------------------------------------------------------

    def setupScriptCsh(self) :

        print "installing setupAnaphe.csh script in ~/bin/"

        if ( os.path.exists("setupAnaphe.csh") ) :
            print "renaming existing file setupAnaphe to setupAnaphe.orig"
            os.system("mv setupAnaphe.csh setupAnaphe.csh.orig")
        lizFile = open("setupAnaphe.csh", 'w')

        lizFile.write("#\n\n")
        lizFile.write("# This file has been created automatically, DO NOT EDIT !\n\n")
        lizFile.write("setenv OS Linux\n\n")
        lizFile.write("setenv PLATF " + self._platf + "\n\n")
        lizFile.write("setenv LHCXXTOP "+self._top+" \n\n")
        if ( self._aidaVers  == "2.2.1" ) :
            lizFile.write("setenv AIDA_DIR "+self._aidaDir+"/cpp"+" \n\n")
        else :
            lizFile.write("setenv AIDA_DIR "+self._aidaDir+"/AIDA/3.0/src/cpp"+" \n\n")
        lizFile.write("setenv ANAPHETOP "+self._top+" \n\n")
        lizFile.write("setenv ANAPHESPECDIR $ANAPHETOP/specific/$PLATF\n\n")
        lizFile.write("source "+ self._top + "/share/LHCXX/" + self._vers + "/scripts/setup-Anaphe.csh\n")

        lizFile.write("if ( $?PYTHONPATH == 0 ) then\n")
        lizFile.write("setenv PYTHONPATH $ANAPHESPECDIR/PublicDomainPackages/"+self.itemRequestedVersions["PublicDomainPackages"]+"/lib\n\n")
        lizFile.write("else \n")
        lizFile.write("setenv PYTHONPATH $ANAPHESPECDIR/PublicDomainPackages/"+self.itemRequestedVersions["PublicDomainPackages"]+"/lib:$PYTHONPATH \n\n")
        lizFile.write("endif\n")

        lizFile.write("setenv PATH $ANAPHESPECDIR/PublicDomainPackages/"+self.itemRequestedVersions["PublicDomainPackages"]+"/grace/bin:$PATH \n\n")
        lizFile.write("setenv PATH $ANAPHESPECDIR/PublicDomainPackages/"+self.itemRequestedVersions["PublicDomainPackages"]+"/bin:$PATH \n\n")
        lizFile.write("setenv GRACE_HOME $ANAPHESPECDIR/PublicDomainPackages/"+self.itemRequestedVersions["PublicDomainPackages"]+"/grace \n\n")
        lizFile.write("setenv ANAPHE_REL_DIR $ANAPHETOP/specific/$PLATF/" + self._vers + "\n\n")

        lizFile.write("\n")
        lizFile.close()

        os.chmod("setupAnaphe.csh",0755)
        
        return

    # --------------------------------------------------------------------------------

    def initVersDir(self) :

        if (not os.path.exists(self._vers) ) : os.mkdir(self._vers)

        for subDir in [ "/bin", "/etc", "/include", "/lib", "/python" ] :
            if ( not os.path.exists(self._vers + subDir) ) :
                os.mkdir(self._vers + subDir)

        return

    # --------------------------------------------------------------------------------

    def show(self) :
        
        print "\n"
        print "$Revision: 1.29 $ "
        print "setup for OS      = '" + self._os    + "'"
        print "          PLATF   = '" + self._platf + "'"
        print "          TopDir  = '" + self._top   + "'"
        print "          version = '" + self._vers  + "'"
        print "\n"

        print "going to install the following packages: "
        for item in self.items :
            print item, self.itemVersions[item]
        
        print "\n"
        return
        
    # --------------------------------------------------------------------------------
    def getAIDA(self, vers) :

	if ( self._aidaAlreadyThere ) :
	   print "AIDA already installed"
	   return

        if ( self.verbose() > 2 ) : print " getting AIDA tarball "
        sys.stdout.flush()

        if ( vers == "2.2.1" ) :
            tarFileName = "aida-"+vers+".tar.gz"
            tarFileURL = "ftp://ftp.slac.stanford.edu/software/freehep/AIDA/v"+vers+"/" + tarFileName
        elif ( vers == "3.0.0" ) :
            tarFileName = "AIDA-"+vers+"-src.tar.gz"
            tarFileURL = "http://pcitapiww.cern.ch/Anaphe/download/" + tarFileName
        else :
            print "FATAL: don't know how to handle AIDA version", vers, ". Aborting"
            sys.exit(-1);

        if ( self.verbose() > 2 ) : print " ... URL : ", tarFileURL
        sys.stdout.flush()

        if (not self.tmpDir) : 
            self.tmpDir = "/tmp/Anaphe/download/"
            
        if (not os.path.exists( self.tmpDir )) :
            ret = os.system(" mkdir -p " + self.tmpDir)
            if (ret != 0) :
                print "FATAL: cannot create tmpDir:", self.tmpDir, " aborting."
                sys.exit(-1);
            
        if ( self.verbose() > 2 ) : print "AIDA ", vers,

        fileName = self.tmpDir + tarFileName

        if ( os.path.exists(fileName) ) :
            if ( self.verbose() > 2 ) : print "file already fetched "
            self.tarFiles.append(tarFileName)
            return
        
        (fileNameRet, info) = urllib.urlretrieve (tarFileURL, fileName)

        if (info.gettype() == "text/plain") :
            tmpFile = open(fileNameRet)
            tmpTxt = tmpFile.readlines()
            tmpFile.close()
            if ( string.find( str(tmpTxt), "404 Not Found" ) != -1 ) :
                print "\nFATAL: Installer> could not find tar file :", tarFileName
                print
                os.remove(fileName)
                sys.exit(-1)
        elif (info.gettype() != "application/x-tar" and
              info.gettype() != "application/octet-stream" ) :
            print "\nERROR: Installer.getPackage> got unknown content-type: ", info.gettype()
            print "      ", info
            sys.exit(-2)

        if ( self.verbose() > 5 ) : print "urlretrieve got: ", fileNameRet, info

        self.tarFiles.append(tarFileName)
        
        if ( self.verbose() > 2) : print " fetched."
        return

    # --------------------------------------------------------------------------------

    def installAIDA(self) :

	if ( self._aidaAlreadyThere ) :
	   print "AIDA already installed"
	   return

        if ( int(string.split(self._vers, ".")[0]) > 4 ) :
           if ( int(string.split(self._aidaVers, ".")[0]) < 3 ) :
              print "FATAL: incompatible AIDA version selected ! "
              print "       for Anaphe version ", self._vers, " AIDA version 3.0.0 or later is needed."
              print "       NOT installing AIDA. Aborting."
              sys.exit()

	self.getAIDA(self._aidaVers)

        if ( self._aidaVers == "2.2.1" ) :
            tarFileName = "aida-"+vers+".tar.gz"
        elif ( self._aidaVers == "3.0.0" ) :
            tarFileName = "AIDA-"+self._aidaVers+"-src.tar.gz"
        else :
            print "FATAL: don't know how to handle AIDA version", vers, ". Aborting"
            sys.exit(-1);

        os.chdir(self._aidaDir)

        tarLogFile = self.tmpDir + "/tarLog"

        ret = os.system("tar zxf " + self.tmpDir + tarFileName + " 2>>" + tarLogFile )
        if ( ret != 0 ) :  # this catches ^C (ret == 2)
           print "error during installation, aborting"
           sys.exit(-1)

        if ( self.verbose() > 2 ) :
           print "done."
           sys.stdout.flush()
            
        return

    # --------------------------------------------------------------------------------

    def installShift(self) :

        pipe = os.popen("locate libshift.so")
        lines = pipe.readlines()
        pipe.close()

        relLibDir = self._top + "/specific/" + self._platf + "/" + self._vers + "/lib"

        if ( len(lines) == 0 ) :
            if ( self.verbose() > 2) : print "installing missing libshift.so into " + relLibDir

            self.getPackage("libshift", "")
            fullScriptName = "libshift--" + string.replace(self._platf,"/","-")
            # ... untar it into the lib dir of the release...
            os.chdir(relLibDir)
            os.system("tar zxf " + self.tmpDir + fullScriptName + ".tgz")

        else:
            if ( self.verbose() > 2) : print "libshift.so already there ..."
            
        return

# ================================================================================

def main(params) :

    printLevel = params.printLevel()

    installer = Installer(params)
    checker   = Checker(params)

    # check for the "environment variables" OS, PLATF, LHCXXTOP, and the Anaphe version
    installer.setEnvVars(checker.chkEnvVars())

    # next check for the other packages and their versions
    installer.updateVersion()

    # show what we got
    if (printLevel > 1) : installer.show()

    # get tar files and install packages
    installer.install()

    # get tar files and install AIDA
    installer.installAIDA()

    # get libshift.so if not yet there 
    installer.installShift()

    # update scripts in ~/bin/
    installer.scripts()
    
    return

# ================================================================================

def usage() :

    print "usage:",sys.argv[0],
    print " --topDir <directory> --version <version> "
    print "       [--platform <platf>]          overrides the automatic determination for PLATF"
    print "       [--aidaDir  <path-to-aida>]   download and installs the AIDA files "
    print "       [--aidaVers <AIDA-version>]   specifies which version of AIDA to install "
    print "       [--tmpDir   <temp-directory>] specify directory for downloaded files"
    print "       [--addQt]                     specifies to also download and install Qt"
    print "       options can be abbreviated as long as they are unique."
    print "       "
    print "       example:"
    print "       "
    print "       anaphe-setup.py --tmp /tmp/Anaphe/download --top /tmp/Anaphe/installation --aidaDir /tmp/Anaphe/AIDA --aidaVers 3.0.0 --vers 5.0.6"
    print "       "

    return

# ================================================================================

if (__name__ == "__main__" ) :

    try:
        optlist, args = getopt.getopt(sys.argv[1:], "?h",
                                      ['help', 'topDir=', 'version=', 'verbose=', 'platform=', 'tmpDir=', 'aidaDir=', 'aidaVers=', 'addQt'])

    except :
        print "\nunknown option.\n"
        usage()
        raise

    params = SetupParams()

    for o, a in optlist:

        if o in ("-?", "-h", "--help"):
            usage()
            sys.exit()

        elif o in ("--topDir",):
            params.tDirSel = os.path.abspath(a)
            if ( params.tDirSel[-1] != "/" ) :
                params.tDirSel = params.tDirSel + "/"
                
        elif o in ("--tmpDir",):
            params.tmpDirSel = os.path.abspath(a)
            if ( params.tmpDirSel[-1] != "/" ) :
                params.tmpDirSel = params.tmpDirSel + "/"
                
        elif o in ("--aidaDir",):
            params.aidaDirSel = os.path.abspath(a)
            if ( params.aidaDirSel[-1] != "/" ) :
                params.aidaDirSel = params.aidaDirSel + "/"
                
        elif o in ("--aidaVers",):
            params.aidaVersSel = a
            # check ... just in case ... :-)
            if ( params.aidaVersSel == "2.2" ): params.aidaVersSel = "2.2.1"
            if ( params.aidaVersSel == "3.0" ): params.aidaVersSel = "3.0.0"
                
        elif o in ("--addQt",):
            params.addQt = 1

        elif o in ("--version",):
            params.versSel = a

        elif o in ("--platform",):
            params.platSel = a

        elif o in ("--verbose",):
            params.prLvlSel = a

	else :
	    print "unknown option: ", o
            sys.exit()


    # check if required options are there:
    if ( params.topDir() == None or
         params.version() == None ) :
        usage()
        sys.exit()

    if ( len(sys.argv) < 5 ) :
        usage()
        sys.exit()
        
    main(params)

    
