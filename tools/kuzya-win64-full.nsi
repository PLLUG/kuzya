; ----------------------------------------
; Installer Script for Kuzya IDE 
; Author: Alex Chmykhalo 2010, 2017
; ----------------------------------------
!include "MUI2.nsh"
!include "WordFunc.nsh"
!include "TextFunc.nsh"
  
;Name and file
Name "kuzya 2.1.12"
Caption "kuzya 2.1.12 - Free IDE for Education"
OutFile "kuzya-2.1.12-win64-full.exe"
  
;Default installation folder
InstallDir "c:\kuzya"
  
;Request application privileges for Windows Vista
RequestExecutionLevel user

;--------------------------------
;Variables
Var /GLOBAL KUZYA_DIR
Var /GLOBAL MSYS_REPO_UNIX_PATH

Var StartMenuFolder
;--------------------------------
;Interface Settings

!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP "${KUZYA_BUILD_ROOT}\tools\NSIS-kuzya2.bmp" ; optional
!define MUI_ABORTWARNING

;--------------------------------
;Pages

!insertmacro MUI_PAGE_LICENSE "${KUZYA_BUILD_ROOT}\LICENSE"
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY  
!insertmacro MUI_PAGE_STARTMENU Application $StartMenuFolder  
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH  
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
  
;--------------------------------
;Languages

!insertmacro MUI_LANGUAGE "English" ;first language is the default language
!insertmacro MUI_LANGUAGE "Ukrainian"
!insertmacro MUI_LANGUAGE "Belarusian"
!insertmacro MUI_LANGUAGE "French"

;--------------------------------
;Installer Sections

InstType "Full (Kuzya IDE + MinGW + Free Pascal)"
InstType "Kuzya IDE Only"

Section "!Kuzya IDE 2.1.12" SecKuzya

	SectionIn 1 2
	;	AddSize 8786

	SetOutPath "$INSTDIR\IDE"  
	File ${KUZYA_BUILD_ROOT}\LICENSE

	SetOutPath "$INSTDIR\IDE\bin"
	File ${KUZYA_BUILD_ROOT}\trunc\bin\*.*
	
	SetOutPath "$INSTDIR\IDE\resources"
	File /r ${KUZYA_BUILD_ROOT}\trunc\resources\*.*
	
	SetOutPath "$INSTDIR\IDE\profiles"
	File /r ${KUZYA_BUILD_ROOT}\trunc\profiles\*.*
	
	SetOutPath "$INSTDIR\IDE\graphics"
	File /r ${KUZYA_BUILD_ROOT}\trunc\graphics\*.*
	
	SetOutPath "$INSTDIR\IDE\doc"
	File /r ${KUZYA_BUILD_ROOT}\trunc\doc\*.*
	
  ;Store installation folder
  ;WriteRegStr HKCU "Software\Modern UI Test" "" $INSTDIR
  
	;Create uninstaller
	WriteUninstaller "$INSTDIR\IDE\Uninstall.exe"
  
    ;Create shortcuts
	!insertmacro MUI_STARTMENU_WRITE_BEGIN Application    
    CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
    CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Kuzya Free IDE.lnk" "$INSTDIR\IDE\bin\kuzya.exe"
    CreateShortCut "$SMPROGRAMS\$StartMenuFolder\GPL Licence v. 3.0.lnk" "$INSTDIR\IDE\LICENSE"
    CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Uninstall Kuzya.lnk" "$INSTDIR\IDE\Uninstall.exe"
	!insertmacro MUI_STARTMENU_WRITE_END

SectionEnd


SectionGroup "!MSYS2 + Compilers and Tools" SecGroupMSYS2

	Section "MSYS2 Core Components" SecMSYS2Core
		SectionIn 1
		SetOutPath "$INSTDIR\msys64"
		File /r ${KUZYA_PACKAGE_ROOT}\msys64\*.*
	SectionEnd

SectionGroupEnd	
	
;SectionGroupEnd
;--------------------------------
;Installer Functions

Function .onInit
	System::Call 'kernel32::CreateMutexA(i 0, i 0, t "Kuzya_Install) i .r1 ?e'
	Pop $R0
	StrCmp $R0 0 +3
	MessageBox MB_OK|MB_ICONEXCLAMATION "The Kuzya IDE installer is already running."
	Abort
	!insertmacro MUI_LANGDLL_DISPLAY
FunctionEnd

Function .onVerifyInstDir
	${WordReplace} "$INSTDIR" "\" "\\" "+" $KUZYA_DIR
	${WordReplace} "$INSTDIR" "\" "/" "+" $MSYS_REPO_UNIX_PATH
	${WordReplace} "$MSYS_REPO_UNIX_PATH" "c:" "c" "+" $MSYS_REPO_UNIX_PATH
	StrCpy $MSYS_REPO_UNIX_PATH "$MSYS_REPO_UNIX_PATH/msys2-dist-repository"
FunctionEnd

Function .onInstSuccess
		FileOpen $0 $INSTDIR\IDE\bin\settings.ini a
		FileSeek $0 0 END
		FileWrite $0 "$\r$\nc%2B%2B\bcc\options=-1 -2 -N -O2 -v -y -wall"
		FileWrite $0 "$\r$\nc%2B%2B\g%2B%2B\options=-O2 --static -I $KUZYA_DIR\\IDE\\graphics\\c"
		FileWrite $0 "$\r$\npascal\fpc\options=-O2 -g -Ci -Co -Cr -Ct"
		FileWrite $0 "$\r$\npascal\bpc\options=-$G+ -$N+ -$Q+ -$R+ -U..\\UNITS"
		FileClose $0 
FunctionEnd
;--------------------------------
;Descriptions

;Language strings
LangString DESC_SecKuzya ${LANG_ENGLISH} "Install Kuzya IDE"
LangString DESC_SecKuzya ${LANG_UKRAINIAN} "Встановити оболонку Kuzya"   
LangString DESC_SecKuzya ${LANG_BELARUSIAN} "Ўсталяваць абалонку Kuzya"   
LangString DESC_SecKuzya ${LANG_FRENCH} "Installer Kuzya IDE"   

;LangString DESC_SecMinGW ${LANG_ENGLISH} "Install MinGW"
;LangString DESC_SecMinGW ${LANG_UKRAINIAN} "Встановити MinGW"  
;LangString DESC_SecMinGW ${LANG_BELARUSIAN} "Ўсталяваць MinGW" 
;LangString DESC_SecMinGW ${LANG_FRENCH} "Installer MinGW" 

;LangString DESC_SecMinGWCore ${LANG_ENGLISH} "Install MinGW Core Components (binutils, gdb debugger, w32api, mingw runtime, mingw32-make)"
;LangString DESC_SecMinGWCore ${LANG_UKRAINIAN} "Встановити основні компоненти MinGW (binutils, gdb debugger, w32api, mingw runtime, mingw32-make)"  
;LangString DESC_SecMinGWCore ${LANG_BELARUSIAN} "Ўсталяваць асноўныя кампаненты MinGW (binutils, gdb debugger, w32api, mingw runtime, mingw32-make)"  
;LangString DESC_SecMinGWCore ${LANG_FRENCH} "Installer les composants essentiels de MinGW (binutils, gdb debugger, w32api, mingw runtime, mingw32-make)"  

;LangString DESC_SecGCCCore ${LANG_ENGLISH} "Install GNU Compiler Collection Core Components and GNU C and C++ Compiler"
;LangString DESC_SecGCCCore ${LANG_UKRAINIAN} "Встановити основні компоненти GNU Compiler Collection та компілятор мови С\C++"  
;LangString DESC_SecGCCCore ${LANG_BELARUSIAN} "Ўсталяваць асноўныя кампаненты GNU Compiler Collection і кампілятар мовы С\C++"  
;LangString DESC_SecGCCCore ${LANG_FRENCH} "Installer les composants essentiels de GNU Compiler Collection et le compilateur des langues С\C++"  

;LangString DESC_SecGroupFPC ${LANG_ENGLISH} "Install Free Pascal"
;LangString DESC_SecGroupFPC ${LANG_UKRAINIAN} "Встановити основні компоненти GNU Compiler Collection та компілятор мови С\C++"  
;LangString DESC_SecGroupFPC ${LANG_BELARUSIAN} "Ўсталяваць Free Pascal"  
;LangString DESC_SecGroupFPC ${LANG_FRENCH} "Installer Free Pascal"  

;LangString DESC_SecFPCCore ${LANG_ENGLISH} "Install Free Pascal Compiler 2.2.4"
;LangString DESC_SecFPCCore ${LANG_UKRAINIAN} "Встановити компілятор мови Pascal (Free Pascal Compiler 2.2.4)"  
;LangString DESC_SecFPCCore ${LANG_BELARUSIAN} "Ўсталяваць кампілятар мовы Pascal (Free Pascal Compiler 2.2.4)"  
;LangString DESC_SecFPCCore ${LANG_FRENCH} "Installer le compilateur de langue Pascal (Free Pascal Compiler 2.2.4)"  
  
;Assign language strings to sections
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
!insertmacro MUI_DESCRIPTION_TEXT ${SecKuzya} $(DESC_SecKuzya)
;!insertmacro MUI_DESCRIPTION_TEXT ${SecGroupMinGW} $(DESC_SecMinGW)
;!insertmacro MUI_DESCRIPTION_TEXT ${SecMinGWCore} $(DESC_SecMinGWCore)
;!insertmacro MUI_DESCRIPTION_TEXT ${SecGCCCore} $(DESC_SecGCCCore)
;!insertmacro MUI_DESCRIPTION_TEXT ${SecGroupFPC} $(DESC_SecGroupFPC)
;!insertmacro MUI_DESCRIPTION_TEXT ${SecFPCCore} $(DESC_SecFPCCore)
!insertmacro MUI_FUNCTION_DESCRIPTION_END
   

;--------------------------------
;Uninstaller Section

Section "Uninstall"

RMDir /r "$INSTDIR\..\msys64"
RMDir /r "$INSTDIR"
!insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder
    
Delete "$SMPROGRAMS\$StartMenuFolder\Kuzya Free IDE.lnk"
Delete "$SMPROGRAMS\$StartMenuFolder\GPL Licence v. 3.0.lnk"
Delete "$SMPROGRAMS\$StartMenuFolder\Uninstall Kuzya.lnk"
RMDir "$SMPROGRAMS\$StartMenuFolder"

SectionEnd
