  !include "kage_installer_info.nsh"

;SetCompressor lzma

; MUI 1.67 compatible ------
;NSIS Modern User Interface
;Start Menu Folder Selection Example Script
;Written by Joost Verburg

;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"

;--------------------------------
;General

  Unicode True

  ;Request application privileges for Windows Vista
  RequestExecutionLevel admin

;--------------------------------
;Variables

  Var StartMenuFolder

;--------------------------------
;Interface Settings



  ; MUI Settings
  !define MUI_ABORTWARNING
;--------------------------------
  !define MUI_ICON "${FILE_PATH}\kage_installer_setup_ico.ico"
  !define MUI_UNICON "${FILE_PATH}\kage_installer_setup_un_ico.ico"
  
  ; Welcome page
  !define KSE_UI ""
  !define MUI_WELCOMEFINISHPAGE_BITMAP "${FILE_PATH}\kage_installer_setup.bmp"
  !define MUI_UNWELCOMEFINISHPAGE_BITMAP "${FILE_PATH}\kage_installer_setup_un.bmp"
  !insertmacro MUI_PAGE_WELCOME
  ; License page
  !insertmacro MUI_PAGE_LICENSE "${FILE_PATH}\LICENSE.txt"
  
  ; Options page
  !insertmacro MUI_PAGE_COMPONENTS
  ; Install Directory page
  !insertmacro MUI_PAGE_DIRECTORY
  
  ;Start Menu Folder Page Configuration
  !define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKLM" 
  !define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\Kage Studio" 
  !define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"
  
  !insertmacro MUI_PAGE_STARTMENU Application $StartMenuFolder

  !insertmacro MUI_UNPAGE_WELCOME
  ; Instfiles page
  !insertmacro MUI_PAGE_INSTFILES

  ; Finish page
  !define MUI_FINISHPAGE_LINK "Visit Kage Studio website"
          !define MUI_FINISHPAGE_LINK_LOCATION "https://kage.sourceforge.io"
          !define MUI_FINISHPAGE_LINK_COLOR DE3C23
  !insertmacro MUI_PAGE_FINISH

  ; Uninstaller pages
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES

  ; Language files
  !insertmacro MUI_LANGUAGE ${KSE_NSI_LANG}
;  !insertmacro MUI_LANGUAGE "Filipino"
  ;--=VIAddVersionKey Info=--

; MUI end ------

!include "kage_installer_pages.nsh"
!include "kage_installer_versionkey.nsh"
;--=file info=--
