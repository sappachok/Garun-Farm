# Microsoft Developer Studio Project File - Name="Garun" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Garun - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Garun.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Garun.mak" CFG="Garun - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Garun - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Garun - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Garun - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x41e /d "NDEBUG"
# ADD RSC /l 0x41e /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "Garun - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x41e /d "_DEBUG"
# ADD RSC /l 0x41e /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 dxguid.lib dinput.lib glu32.lib opengl32.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Garun - Win32 Release"
# Name "Garun - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\3DMath.cpp
# End Source File
# Begin Source File

SOURCE=.\3ds.cpp
# End Source File
# Begin Source File

SOURCE=.\3dsLoader.cpp
# End Source File
# Begin Source File

SOURCE=.\attack.cpp
# End Source File
# Begin Source File

SOURCE=.\Camera.cpp
# End Source File
# Begin Source File

SOURCE=.\CGfxOpenGL.cpp
# End Source File
# Begin Source File

SOURCE=.\CJPEGFile.cpp
# End Source File
# Begin Source File

SOURCE=.\cow.cpp
# End Source File
# Begin Source File

SOURCE=.\CTargaImage.cpp
# End Source File
# Begin Source File

SOURCE=.\en_boss01.cpp
# End Source File
# Begin Source File

SOURCE=.\en_boss02.cpp
# End Source File
# Begin Source File

SOURCE=.\en_boss03.cpp
# End Source File
# Begin Source File

SOURCE=.\en_boss04.cpp
# End Source File
# Begin Source File

SOURCE=.\en_boss05.cpp
# End Source File
# Begin Source File

SOURCE=.\en_greengoblin.cpp
# End Source File
# Begin Source File

SOURCE=.\en_redgoblin.cpp
# End Source File
# Begin Source File

SOURCE=.\en_redwolf.cpp
# End Source File
# Begin Source File

SOURCE=.\en_whitewolf.cpp
# End Source File
# Begin Source File

SOURCE=.\en_wolf.cpp
# End Source File
# Begin Source File

SOURCE=.\en_worriorgoblin.cpp
# End Source File
# Begin Source File

SOURCE=.\enemy.cpp
# End Source File
# Begin Source File

SOURCE=.\entity.cpp
# End Source File
# Begin Source File

SOURCE=.\garden.cpp
# End Source File
# Begin Source File

SOURCE=.\GFont.cpp
# End Source File
# Begin Source File

SOURCE=.\InputSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\md2.cpp
# End Source File
# Begin Source File

SOURCE=.\MusicSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\player.cpp
# End Source File
# Begin Source File

SOURCE=.\SoundSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\texture.cpp
# End Source File
# Begin Source File

SOURCE=.\weapon.cpp
# End Source File
# Begin Source File

SOURCE=.\winmain.cpp
# End Source File
# Begin Source File

SOURCE=.\world.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\3DMath.h
# End Source File
# Begin Source File

SOURCE=.\3ds.h
# End Source File
# Begin Source File

SOURCE=.\3dsLoader.h
# End Source File
# Begin Source File

SOURCE=.\attack.h
# End Source File
# Begin Source File

SOURCE=.\camera.h
# End Source File
# Begin Source File

SOURCE=.\CGfxOpenGL.h
# End Source File
# Begin Source File

SOURCE=.\CJPEGFile.h
# End Source File
# Begin Source File

SOURCE=.\cow.h
# End Source File
# Begin Source File

SOURCE=.\CTargaImage.h
# End Source File
# Begin Source File

SOURCE=.\dmutil.h
# End Source File
# Begin Source File

SOURCE=.\en_boss01.h
# End Source File
# Begin Source File

SOURCE=.\en_boss02.h
# End Source File
# Begin Source File

SOURCE=.\en_boss03.h
# End Source File
# Begin Source File

SOURCE=.\en_boss04.h
# End Source File
# Begin Source File

SOURCE=.\en_boss05.h
# End Source File
# Begin Source File

SOURCE=.\en_greengoblin.h
# End Source File
# Begin Source File

SOURCE=.\en_redgoblin.h
# End Source File
# Begin Source File

SOURCE=.\en_redwolf.h
# End Source File
# Begin Source File

SOURCE=.\en_whitewolf.h
# End Source File
# Begin Source File

SOURCE=.\en_wolf.h
# End Source File
# Begin Source File

SOURCE=.\en_worriorgoblin.h
# End Source File
# Begin Source File

SOURCE=.\enemy.h
# End Source File
# Begin Source File

SOURCE=.\entity.h
# End Source File
# Begin Source File

SOURCE=.\garden.h
# End Source File
# Begin Source File

SOURCE=.\GFont.h
# End Source File
# Begin Source File

SOURCE=.\InputSystem.h
# End Source File
# Begin Source File

SOURCE=.\item.h
# End Source File
# Begin Source File

SOURCE=.\md2.h
# End Source File
# Begin Source File

SOURCE=.\MusicSystem.h
# End Source File
# Begin Source File

SOURCE=.\object.h
# End Source File
# Begin Source File

SOURCE=.\player.h
# End Source File
# Begin Source File

SOURCE=.\SoundSystem.h
# End Source File
# Begin Source File

SOURCE=.\texture.h
# End Source File
# Begin Source File

SOURCE=.\tree.h
# End Source File
# Begin Source File

SOURCE=.\type_object.h
# End Source File
# Begin Source File

SOURCE=.\vector.h
# End Source File
# Begin Source File

SOURCE=.\weapon.h
# End Source File
# Begin Source File

SOURCE=.\winmain.h
# End Source File
# Begin Source File

SOURCE=.\world.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\main_resource.rc
# End Source File
# End Group
# Begin Source File

SOURCE=.\cursor1.cur
# End Source File
# End Target
# End Project
