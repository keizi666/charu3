# Microsoft Developer Studio Project File - Name="Charu3" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Charu3 - Win32 Debug_UniCode
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "Charu3.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "Charu3.mak" CFG="Charu3 - Win32 Debug_UniCode"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "Charu3 - Win32 Release" ("Win32 (x86) Application" 用)
!MESSAGE "Charu3 - Win32 Debug" ("Win32 (x86) Application" 用)
!MESSAGE "Charu3 - Win32 Release_UniCode" ("Win32 (x86) Application" 用)
!MESSAGE "Charu3 - Win32 Debug_UniCode" ("Win32 (x86) Application" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Charu3 - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 winmm.lib imm32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "Charu3 - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib imm32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "Charu3 - Win32 Release_UniCode"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Charu3___Win32_Release_UniCode"
# PROP BASE Intermediate_Dir "Charu3___Win32_Release_UniCode"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_Uni"
# PROP Intermediate_Dir "Release_Uni"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_UNICODE" /D "_MBCS" /U "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_UNICODE" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 winmm.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 winmm.lib imm32.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "Charu3 - Win32 Debug_UniCode"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Charu3___Win32_Debug_UniCode"
# PROP BASE Intermediate_Dir "Charu3___Win32_Debug_UniCode"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_Uni"
# PROP Intermediate_Dir "Debug_Uni"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_UNICODE" /D "_MBCS" /U "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_UNICODE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 winmm.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib imm32.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Charu3 - Win32 Release"
# Name "Charu3 - Win32 Debug"
# Name "Charu3 - Win32 Release_UniCode"
# Name "Charu3 - Win32 Debug_UniCode"
# Begin Group "メインC++"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Charu3.cpp
# End Source File
# Begin Source File

SOURCE=.\Charu3.rc
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# End Group
# Begin Group "メインヘッダ"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Charu3.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# End Group
# Begin Group "設定クラス"

# PROP Default_Filter ""
# Begin Group "ダイアログ"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\OptEtc.cpp
# End Source File
# Begin Source File

SOURCE=.\OptEtc.h
# End Source File
# Begin Source File

SOURCE=.\OptFifo.cpp
# End Source File
# Begin Source File

SOURCE=.\OptFifo.h
# End Source File
# Begin Source File

SOURCE=.\OptKeySet.cpp
# End Source File
# Begin Source File

SOURCE=.\OptKeySet.h
# End Source File
# Begin Source File

SOURCE=.\OptKeySetEditDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\OptKeySetEditDlg.h
# End Source File
# Begin Source File

SOURCE=.\OptMainDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\OptMainDialog.h
# End Source File
# Begin Source File

SOURCE=.\OptPopup.cpp
# End Source File
# Begin Source File

SOURCE=.\OptPopup.h
# End Source File
# Begin Source File

SOURCE=.\OptVisual.cpp
# End Source File
# Begin Source File

SOURCE=.\OptVisual.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Init.cpp
# End Source File
# Begin Source File

SOURCE=.\Init.h
# End Source File
# Begin Source File

SOURCE=.\MyHotkeyCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\MyHotkeyCtrl.h
# End Source File
# End Group
# Begin Group "一般クラス"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ClipBord.cpp
# End Source File
# Begin Source File

SOURCE=.\ClipBord.h
# End Source File
# Begin Source File

SOURCE=.\General.cpp
# End Source File
# Begin Source File

SOURCE=.\General.h
# End Source File
# Begin Source File

SOURCE=.\MyEditCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\MyEditCtrl.h
# End Source File
# Begin Source File

SOURCE=.\PaletStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\PaletStatic.h
# End Source File
# Begin Source File

SOURCE=.\StringWork.cpp
# End Source File
# Begin Source File

SOURCE=.\StringWork.h
# End Source File
# End Group
# Begin Group "機能ダイアログ"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AddDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\AddDialog.h
# End Source File
# Begin Source File

SOURCE=.\EditDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\EditDialog.h
# End Source File
# Begin Source File

SOURCE=.\MyFileDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\MyFileDialog.h
# End Source File
# Begin Source File

SOURCE=.\SerchDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\SerchDialog.h
# End Source File
# End Group
# Begin Group "あまり変更しない"

# PROP Default_Filter ""
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\c2p256_16_run.ico
# End Source File
# Begin Source File

SOURCE=.\res\c2p256_16_stop.ico
# End Source File
# Begin Source File

SOURCE=.\res\Charu2_aqua.ico
# End Source File
# Begin Source File

SOURCE=.\res\Charu3.ico
# End Source File
# Begin Source File

SOURCE=.\res\Charu3.rc2
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\itemIcon.bmp
# End Source File
# Begin Source File

SOURCE=.\res\itemIconPink.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ChildView.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildView.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "ツリー＆データ"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Charu3Tree.cpp
# End Source File
# Begin Source File

SOURCE=.\Charu3Tree.h
# End Source File
# Begin Source File

SOURCE=.\MyTreeDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\MyTreeDialog.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
