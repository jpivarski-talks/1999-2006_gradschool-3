# Microsoft Developer Studio Project File - Name="Rio" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Rio - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Rio.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Rio.mak" CFG="Rio - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Rio - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Rio - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Rio - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Win32"
# PROP Intermediate_Dir "..\Win32"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /MD /D "WIN32" /GX /GR /FD /c /I ".."
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:..\Win32\Rio.arc

# Begin Custom Build
..\Win32\Rio.dll : ..\Win32\Rio.arc
	if exist ..\Win32\Rio.lib del ..\Win32\Rio.lib 
	if exist ..\Win32\Rio.exp del ..\Win32\Rio.exp 
	if exist ..\Win32\Rio.dll del ..\Win32\Rio.dll 
	dumpbin /out:tmp /symbols ..\Win32\Rio.arc
	owindef.exe Rio < tmp > Rio.def
	lib.exe /nologo /machine:ix86 /def:Rio.def /out:..\Win32\Rio.lib
	link.exe /nologo /dll /out:..\Win32\Rio.dll ..\Win32\Rio.exp \
..\Win32\Trees.obj \
..\Win32\Deflate.obj \
..\Win32\Inflate.obj \
..\Win32\Bits.obj \
..\Win32\smanip.obj \
..\Win32\Debug.obj \
..\Win32\Lib.obj \
..\Win32\Printer.obj \
..\Win32\Out.obj \
..\Win32\Date.obj \
..\Win32\Arguments.obj \
..\Win32\Map.obj \
..\Win32\Object.obj \
..\Win32\Named.obj \
..\Win32\List.obj \
..\Win32\ObjArray.obj \
..\Win32\FreeSegment.obj \
..\Win32\Class.obj \
..\Win32\StreamerElement.obj \
..\Win32\StreamerInfo.obj \
..\Win32\CoreDictionary.obj \
..\Win32\Buffer.obj \
..\Win32\Key.obj \
..\Win32\Directory.obj \
..\Win32\File.obj \
..\Win32\BaseLeaf.obj \
..\Win32\Leaf.obj \
..\Win32\LeafObject.obj \
..\Win32\Basket.obj \
..\Win32\Branch.obj \
..\Win32\BranchObject.obj \
..\Win32\Tree.obj \
..\Win32\TreeDictionary.obj
	del Rio.def
# End Custom Build

!ELSEIF  "$(CFG)" == "Rio - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Rio___Win32_Debug"
# PROP BASE Intermediate_Dir "Rio___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Win32Debug"
# PROP Intermediate_Dir "..\Win32Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /MD /D "WIN32" /GX /GR /FD /c /I ".." /ZI /Od
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:..\Win32Debug\Rio.arc

# Begin Custom Build
..\Win32Debug\Rio.dll : ..\Win32Debug\Rio.arc
	if exist ..\Win32Debug\Rio.lib del ..\Win32Debug\Rio.lib 
	if exist ..\Win32Debug\Rio.exp del ..\Win32Debug\Rio.exp 
	if exist ..\Win32Debug\Rio.dll del ..\Win32Debug\Rio.dll 
	dumpbin /out:tmp /symbols ..\Win32Debug\Rio.arc
	owindef.exe Rio < tmp > Rio.def
	lib.exe /nologo /machine:ix86 /def:Rio.def /out:..\Win32Debug\Rio.lib
	link.exe /nologo /dll /out:..\Win32Debug\Rio.dll ..\Win32Debug\Rio.exp \
..\Win32Debug\Trees.obj \
..\Win32Debug\Deflate.obj \
..\Win32Debug\Inflate.obj \
..\Win32Debug\Bits.obj \
..\Win32Debug\smanip.obj \
..\Win32Debug\Debug.obj \
..\Win32Debug\Lib.obj \
..\Win32Debug\Printer.obj \
..\Win32Debug\Out.obj \
..\Win32Debug\Date.obj \
..\Win32Debug\Arguments.obj \
..\Win32Debug\Map.obj \
..\Win32Debug\Object.obj \
..\Win32Debug\Named.obj \
..\Win32Debug\List.obj \
..\Win32Debug\ObjArray.obj \
..\Win32Debug\FreeSegment.obj \
..\Win32Debug\Class.obj \
..\Win32Debug\StreamerElement.obj \
..\Win32Debug\StreamerInfo.obj \
..\Win32Debug\CoreDictionary.obj \
..\Win32Debug\Buffer.obj \
..\Win32Debug\Key.obj \
..\Win32Debug\Directory.obj \
..\Win32Debug\File.obj \
..\Win32Debug\BaseLeaf.obj \
..\Win32Debug\Leaf.obj \
..\Win32Debug\LeafObject.obj \
..\Win32Debug\Basket.obj \
..\Win32Debug\Branch.obj \
..\Win32Debug\BranchObject.obj \
..\Win32Debug\Tree.obj \
..\Win32Debug\TreeDictionary.obj
	del Rio.def
# End Custom Build

!ENDIF 

# Begin Target

# Name "Rio - Win32 Release"
# Name "Rio - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE="..\source\zip\Trees.c"
# End Source File
# Begin Source File

SOURCE="..\source\zip\Deflate.c"
# End Source File
# Begin Source File

SOURCE="..\source\zip\Inflate.c"
# End Source File
# Begin Source File

SOURCE=..\source\zip\Bits.c
# End Source File
# Begin Source File

SOURCE="..\source\Core\smanip.cxx"
# End Source File
# Begin Source File

SOURCE="..\source\Core\Debug.cxx"
# End Source File
# Begin Source File

SOURCE="..\source\Core\Lib.cxx"
# End Source File
# Begin Source File

SOURCE="..\source\Core\Printer.cxx"
# End Source File
# Begin Source File

SOURCE="..\source\Core\Out.cxx"
# End Source File
# Begin Source File

SOURCE="..\source\Core\Date.cxx"
# End Source File
# Begin Source File

SOURCE="..\source\Core\Arguments.cxx"
# End Source File
# Begin Source File

SOURCE="..\source\Core\Map.cxx"
# End Source File
# Begin Source File

SOURCE="..\source\Core\Object.cxx"
# End Source File
# Begin Source File

SOURCE="..\source\Core\Named.cxx"
# End Source File
# Begin Source File

SOURCE="..\source\Core\List.cxx"
# End Source File
# Begin Source File

SOURCE="..\source\Core\ObjArray.cxx"
# End Source File
# Begin Source File

SOURCE="..\source\Core\FreeSegment.cxx"
# End Source File
# Begin Source File

SOURCE="..\source\Core\Class.cxx"
# End Source File
# Begin Source File

SOURCE="..\source\Core\StreamerElement.cxx"
# End Source File
# Begin Source File

SOURCE="..\source\Core\StreamerInfo.cxx"
# End Source File
# Begin Source File

SOURCE="..\source\Core\CoreDictionary.cxx"
# End Source File
# Begin Source File

SOURCE="..\source\Core\Buffer.cxx"
# End Source File
# Begin Source File

SOURCE="..\source\Core\Key.cxx"
# End Source File
# Begin Source File

SOURCE="..\source\Core\Directory.cxx"
# End Source File
# Begin Source File

SOURCE="..\source\Core\File.cxx"
# End Source File
# Begin Source File

SOURCE="..\source\Tree\BaseLeaf.cxx"
# End Source File
# Begin Source File

SOURCE="..\source\Tree\Leaf.cxx"
# End Source File
# Begin Source File

SOURCE="..\source\Tree\LeafObject.cxx"
# End Source File
# Begin Source File

SOURCE="..\source\Tree\Basket.cxx"
# End Source File
# Begin Source File

SOURCE="..\source\Tree\Branch.cxx"
# End Source File
# Begin Source File

SOURCE="..\source\Tree\BranchObject.cxx"
# End Source File
# Begin Source File

SOURCE="..\source\Tree\Tree.cxx"
# End Source File
# Begin Source File

SOURCE="..\source\Tree\TreeDictionary.cxx"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# End Target
# End Project
