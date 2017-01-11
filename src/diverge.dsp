# Microsoft Developer Studio Project File - Name="diverge" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=diverge - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "diverge.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "diverge.mak" CFG="diverge - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "diverge - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "diverge - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "diverge - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "diverge___Win32_Release"
# PROP BASE Intermediate_Dir "diverge___Win32_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "diverge___Win32_Release"
# PROP Intermediate_Dir "diverge___Win32_Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "diverge - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "diverge___Win32_Debug"
# PROP BASE Intermediate_Dir "diverge___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "diverge___Win32_Debug"
# PROP Intermediate_Dir "diverge___Win32_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "QT" /D "GUI" /D "BOOTSTRAP" /D "QT_DLL" /D "UNICODE" /D "QT_THREAD_SUPPORT" /Fp"Debug/diverge.pch" /YX /Fo"Debug/" /Fd"Debug/" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib imm32.lib winmm.lib wsock32.lib imm32.lib wsock32.lib winmm.lib $(QTDIR)\lib\qt-mt230nc.lib $(QTDIR)\lib\qtmain.lib opengl32.lib glu32.lib /nologo /subsystem:windows /pdb:"Debug/diverge.pdb" /debug /machine:I386 /out:"Debug/diverge.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "diverge - Win32 Release"
# Name "diverge - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\align.cpp
# End Source File
# Begin Source File

SOURCE=.\alignment_view.cpp
# End Source File
# Begin Source File

SOURCE=.\ancestral_compute.cpp
# End Source File
# Begin Source File

SOURCE=.\ancestral_seq_tab.cpp
# End Source File
# Begin Source File

SOURCE=.\asymmetric_test_one.cpp
# End Source File
# Begin Source File

SOURCE=.\asymmetric_test_one_tab.cpp
# End Source File
# Begin Source File

SOURCE=.\cluster.cpp
# End Source File
# Begin Source File

SOURCE=.\clustering_tab.cpp
# End Source File
# Begin Source File

SOURCE=.\effective_number.cpp
# End Source File
# Begin Source File

SOURCE=.\effective_number_sites_tab.cpp
# End Source File
# Begin Source File

SOURCE=.\false_discovery_rate.cpp
# End Source File
# Begin Source File

SOURCE=.\false_discovery_rate_tab.cpp
# End Source File
# Begin Source File

SOURCE=.\fdr_graph_view.cpp
# End Source File
# Begin Source File

SOURCE=.\func_dist_tab.cpp
# End Source File
# Begin Source File

SOURCE=.\graph_view.cpp
# End Source File
# Begin Source File

SOURCE=.\graphics_utils.cpp
# End Source File
# Begin Source File

SOURCE=.\gu2001.cpp
# End Source File
# Begin Source File

SOURCE=.\gu2001_method_tab.cpp
# End Source File
# Begin Source File

SOURCE=.\gu99.cpp
# End Source File
# Begin Source File

SOURCE=.\gu99_method_tab.cpp
# End Source File
# Begin Source File

SOURCE=.\guwidget.cpp
# End Source File
# Begin Source File

SOURCE=.\gz97.cpp
# End Source File
# Begin Source File

SOURCE=.\GZf2.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\mainwindow.cpp
# End Source File
# Begin Source File

SOURCE=.\method_tab.cpp
# End Source File
# Begin Source File

SOURCE=.\moc_alignment_view.cpp
# End Source File
# Begin Source File

SOURCE=.\moc_ancestral_seq_tab.cpp
# End Source File
# Begin Source File

SOURCE=.\moc_asymmetric_test_one_tab.cpp
# End Source File
# Begin Source File

SOURCE=.\moc_clustering_tab.cpp
# End Source File
# Begin Source File

SOURCE=.\moc_effective_number_sites_tab.cpp
# End Source File
# Begin Source File

SOURCE=.\moc_false_discovery_rate_tab.cpp
# End Source File
# Begin Source File

SOURCE=.\moc_func_dist_tab.cpp
# End Source File
# Begin Source File

SOURCE=.\moc_graph_view.cpp
# End Source File
# Begin Source File

SOURCE=.\moc_gu2001_method_tab.cpp
# End Source File
# Begin Source File

SOURCE=.\moc_gu99_method_tab.cpp
# End Source File
# Begin Source File

SOURCE=.\moc_guwidget.cpp
# End Source File
# Begin Source File

SOURCE=.\moc_mainwindow.cpp
# End Source File
# Begin Source File

SOURCE=.\moc_method_tab.cpp
# End Source File
# Begin Source File

SOURCE=.\moc_pdb_viewer.cpp
# End Source File
# Begin Source File

SOURCE=.\moc_qlistview_wcs.cpp
# End Source File
# Begin Source File

SOURCE=.\moc_rvs_tab.cpp
# End Source File
# Begin Source File

SOURCE=.\moc_sequences_tab.cpp
# End Source File
# Begin Source File

SOURCE=.\moc_tip_window.cpp
# End Source File
# Begin Source File

SOURCE=.\moc_tree_view.cpp
# End Source File
# Begin Source File

SOURCE=.\moc_type_one_method_tab.cpp
# End Source File
# Begin Source File

SOURCE=.\moc_type_two_method_tab.cpp
# End Source File
# Begin Source File

SOURCE=.\pdb.cpp
# End Source File
# Begin Source File

SOURCE=.\pdb_viewer.cpp
# End Source File
# Begin Source File

SOURCE=.\progress.cpp
# End Source File
# Begin Source File

SOURCE=.\qlistview_wcs.cpp

!IF  "$(CFG)" == "diverge - Win32 Release"

!ELSEIF  "$(CFG)" == "diverge - Win32 Debug"

# ADD CPP /W3
# SUBTRACT CPP /WX /Fr

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources.cpp
# End Source File
# Begin Source File

SOURCE=.\rvs.cpp
# End Source File
# Begin Source File

SOURCE=.\rvs_tab.cpp
# End Source File
# Begin Source File

SOURCE=.\sequence.cpp
# End Source File
# Begin Source File

SOURCE=.\sequences_tab.cpp
# End Source File
# Begin Source File

SOURCE=.\splash_window.cpp
# End Source File
# Begin Source File

SOURCE=.\star_view.cpp
# End Source File
# Begin Source File

SOURCE=.\tip_window.cpp
# End Source File
# Begin Source File

SOURCE=.\tree.cpp
# End Source File
# Begin Source File

SOURCE=.\tree_view.cpp
# End Source File
# Begin Source File

SOURCE=.\type_one.cpp
# End Source File
# Begin Source File

SOURCE=.\type_one_method_tab.cpp
# End Source File
# Begin Source File

SOURCE=.\type_two.cpp
# End Source File
# Begin Source File

SOURCE=.\type_two_method_tab.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\align.h
# End Source File
# Begin Source File

SOURCE=.\alignment_view.h
# End Source File
# Begin Source File

SOURCE=.\ancestral_compute.h
# End Source File
# Begin Source File

SOURCE=.\ancestral_seq_tab.h
# End Source File
# Begin Source File

SOURCE=.\asymmetric_test_one.h
# End Source File
# Begin Source File

SOURCE=.\asymmetric_test_one_tab.h
# End Source File
# Begin Source File

SOURCE=.\cluster.h
# End Source File
# Begin Source File

SOURCE=.\clustering_tab.h
# End Source File
# Begin Source File

SOURCE=.\common.h
# End Source File
# Begin Source File

SOURCE=.\effective_number.h
# End Source File
# Begin Source File

SOURCE=.\effective_number_sites_tab.h
# End Source File
# Begin Source File

SOURCE=.\false_discovery_rate.h
# End Source File
# Begin Source File

SOURCE=.\false_discovery_rate_tab.h
# End Source File
# Begin Source File

SOURCE=.\fdr_graph_view.h
# End Source File
# Begin Source File

SOURCE=.\func_dist_tab.h
# End Source File
# Begin Source File

SOURCE=.\global.h
# End Source File
# Begin Source File

SOURCE=.\graph_view.h
# End Source File
# Begin Source File

SOURCE=.\graphics_utils.h
# End Source File
# Begin Source File

SOURCE=.\gu2001.h
# End Source File
# Begin Source File

SOURCE=.\gu2001_method_tab.h
# End Source File
# Begin Source File

SOURCE=.\gu99.h
# End Source File
# Begin Source File

SOURCE=.\gu99_method_tab.h
# End Source File
# Begin Source File

SOURCE=.\guwidget.h
# End Source File
# Begin Source File

SOURCE=.\gz97.h
# End Source File
# Begin Source File

SOURCE=.\GZf2.h
# End Source File
# Begin Source File

SOURCE=.\mainwindow.h
# End Source File
# Begin Source File

SOURCE=.\matrices.h
# End Source File
# Begin Source File

SOURCE=.\matrix.h
# End Source File
# Begin Source File

SOURCE=.\method_tab.h
# End Source File
# Begin Source File

SOURCE=.\pdb.h
# End Source File
# Begin Source File

SOURCE=.\pdb_viewer.h
# End Source File
# Begin Source File

SOURCE=.\progress.h
# End Source File
# Begin Source File

SOURCE=.\qlistview_wcs.h
# End Source File
# Begin Source File

SOURCE=.\resources.h
# End Source File
# Begin Source File

SOURCE=.\result.h
# End Source File
# Begin Source File

SOURCE=.\rvs.h
# End Source File
# Begin Source File

SOURCE=.\rvs_tab.h
# End Source File
# Begin Source File

SOURCE=.\sequence.h
# End Source File
# Begin Source File

SOURCE=.\sequences_tab.h
# End Source File
# Begin Source File

SOURCE=.\splash_window.h
# End Source File
# Begin Source File

SOURCE=.\star_view.h
# End Source File
# Begin Source File

SOURCE=.\tip_window.h
# End Source File
# Begin Source File

SOURCE=.\tree.h
# End Source File
# Begin Source File

SOURCE=.\tree_view.h
# End Source File
# Begin Source File

SOURCE=.\type_one.h
# End Source File
# Begin Source File

SOURCE=.\type_one_method_tab.h
# End Source File
# Begin Source File

SOURCE=.\type_two.h
# End Source File
# Begin Source File

SOURCE=.\type_two_method_tab.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
