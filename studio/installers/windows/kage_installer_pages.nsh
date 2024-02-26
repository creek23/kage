BrandingText "Kage Studio"
Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "kagestudio-${PRODUCT_VERSION}${KSE_UI}.exe"
InstallDir "$LOCALAPPDATA\Kage\Studio"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show

Section "Core (2.7 MB)" SEC01
  SectionIn RO
  SetOutPath "$INSTDIR"
  SetOverwrite ifnewer
  
  
  File "${FILE_PATH}\kagestudio.exe"
  File "${FILE_PATH}\LICENSE.txt"
  File "${FILE_PATH}\README.txt"
  File "${FILE_PATH}\kage.ico"
  File "${FILE_PATH}\ksf.ico"

  ;Store installation folder
  WriteRegStr HKLM "Software\Kage Studio" "" $INSTDIR
SectionEnd

Section "Demo (0.1 MB)" SEC02
  SetOutPath "$INSTDIR"
  SetOverwrite ifnewer

  File "${FILE_PATH}\demo.ksf"
  ;File "${FILE_PATH}\demo2.kage"
  ;File "${FILE_PATH}\demo2.ksf"
  
SectionEnd

Section "Gtkmm (53.4 MB)" SEC03
  SetOutPath "$INSTDIR"
  SetOverwrite ifnewer
  
  File "${FILE_PATH}\gdbus.exe"
  File "${FILE_PATH}\libatk-1.0-0.dll"
  File "${FILE_PATH}\libatkmm-1.6-1.dll"
  File "${FILE_PATH}\libbrotlicommon.dll"
  File "${FILE_PATH}\libbrotlidec.dll"
  File "${FILE_PATH}\libbz2-1.dll"
  File "${FILE_PATH}\libcairo-2.dll"
  File "${FILE_PATH}\libcairo-gobject-2.dll"
  File "${FILE_PATH}\libcairomm-1.0-1.dll"
  File "${FILE_PATH}\libdatrie-1.dll"
  File "${FILE_PATH}\libepoxy-0.dll"
  File "${FILE_PATH}\libexpat-1.dll"
  File "${FILE_PATH}\libffi-8.dll"
  File "${FILE_PATH}\libfontconfig-1.dll"
  File "${FILE_PATH}\libfreetype-6.dll"
  File "${FILE_PATH}\libfribidi-0.dll"
  File "${FILE_PATH}\libgcc_s_seh-1.dll"
  File "${FILE_PATH}\libgdk_pixbuf-2.0-0.dll"
  File "${FILE_PATH}\libgdk-3-0.dll"
  File "${FILE_PATH}\libgdkmm-3.0-1.dll"
  File "${FILE_PATH}\libgio-2.0-0.dll"
  File "${FILE_PATH}\libgiomm-2.4-1.dll"
  File "${FILE_PATH}\libglib-2.0-0.dll"
  File "${FILE_PATH}\libglibmm-2.4-1.dll"
  File "${FILE_PATH}\libgmodule-2.0-0.dll"
  File "${FILE_PATH}\libgobject-2.0-0.dll"
  File "${FILE_PATH}\libgraphite2.dll"
  File "${FILE_PATH}\libgtk-3-0.dll"
  File "${FILE_PATH}\libgtkmm-3.0-1.dll"
  File "${FILE_PATH}\libharfbuzz-0.dll"
  File "${FILE_PATH}\libiconv-2.dll"
  File "${FILE_PATH}\libintl-8.dll"
  File "${FILE_PATH}\libpango-1.0-0.dll"
  File "${FILE_PATH}\libpangocairo-1.0-0.dll"
  File "${FILE_PATH}\libpangoft2-1.0-0.dll"
  File "${FILE_PATH}\libpangomm-1.4-1.dll"
  File "${FILE_PATH}\libpangowin32-1.0-0.dll"
  File "${FILE_PATH}\libpcre2-8-0.dll"
  File "${FILE_PATH}\libpixman-1-0.dll"
  File "${FILE_PATH}\libpng16-16.dll"
  File "${FILE_PATH}\libsigc-2.0-0.dll"
  File "${FILE_PATH}\libstdc++-6.dll"
  File "${FILE_PATH}\libthai-0.dll"
  File "${FILE_PATH}\libwinpthread-1.dll"
  File "${FILE_PATH}\zlib1.dll"
  
  SetOutPath "$INSTDIR\share\glib-2.0\schemas\"
  SetOverwrite ifnewer
  File "${FILE_PATH}\share\glib-2.0\schemas\gschemas.compiled"
  
  SetOutPath "$INSTDIR\lib\gdk-pixbuf-2.0\2.10.0\"
  SetOverwrite ifnewer
  File "${FILE_PATH}\lib\gdk-pixbuf-2.0\2.10.0\loaders.cache"
  
  SetOutPath "$INSTDIR\lib\gdk-pixbuf-2.0\2.10.0\loaders"
  SetOverwrite ifnewer
  File "${FILE_PATH}\lib\gdk-pixbuf-2.0\2.10.0\loaders\libpixbufloader-ani.dll"
  File "${FILE_PATH}\lib\gdk-pixbuf-2.0\2.10.0\loaders\libpixbufloader-avif.dll"
  File "${FILE_PATH}\lib\gdk-pixbuf-2.0\2.10.0\loaders\libpixbufloader-bmp.dll"
  File "${FILE_PATH}\lib\gdk-pixbuf-2.0\2.10.0\loaders\libpixbufloader-gif.dll"
  File "${FILE_PATH}\lib\gdk-pixbuf-2.0\2.10.0\loaders\libpixbufloader-heif.dll"
  File "${FILE_PATH}\lib\gdk-pixbuf-2.0\2.10.0\loaders\libpixbufloader-icns.dll"
  File "${FILE_PATH}\lib\gdk-pixbuf-2.0\2.10.0\loaders\libpixbufloader-ico.dll"
  File "${FILE_PATH}\lib\gdk-pixbuf-2.0\2.10.0\loaders\libpixbufloader-jpeg.dll"
  File "${FILE_PATH}\lib\gdk-pixbuf-2.0\2.10.0\loaders\libpixbufloader-png.dll"
  File "${FILE_PATH}\lib\gdk-pixbuf-2.0\2.10.0\loaders\libpixbufloader-pnm.dll"
  File "${FILE_PATH}\lib\gdk-pixbuf-2.0\2.10.0\loaders\libpixbufloader-qtif.dll"
  File "${FILE_PATH}\lib\gdk-pixbuf-2.0\2.10.0\loaders\libpixbufloader-svg.dll"
  File "${FILE_PATH}\lib\gdk-pixbuf-2.0\2.10.0\loaders\libpixbufloader-tga.dll"
  File "${FILE_PATH}\lib\gdk-pixbuf-2.0\2.10.0\loaders\libpixbufloader-tiff.dll"
  File "${FILE_PATH}\lib\gdk-pixbuf-2.0\2.10.0\loaders\libpixbufloader-xbm.dll"
  File "${FILE_PATH}\lib\gdk-pixbuf-2.0\2.10.0\loaders\libpixbufloader-xpm.dll"
  
  SetOutPath "$INSTDIR\share\icons"
  SetOverwrite ifnewer
  
  File "${FILE_PATH}\share\icons\icons.zip" ;; to be unzipped on post-install
  
SectionEnd


Section "Plugins (77.3 MB)" SEC04
  SetOutPath "$INSTDIR"
  SetOverwrite ifnewer

  File "${FILE_PATH}\ffmpeg.exe"
  File "${FILE_PATH}\ffmpeg_LICENSE.txt"
  File "${FILE_PATH}\ffmpeg_README.txt"
SectionEnd

Section -AdditionalIcons
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    
    ;Create shortcuts
    CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
    CreateShortcut "$SMPROGRAMS\$StartMenuFolder\Kage Studio.lnk" "$INSTDIR\kagestudio.exe"
    CreateShortcut "$SMPROGRAMS\$StartMenuFolder\Uninstall Kage Studio.lnk" "$INSTDIR\uninst.exe"
  
  !insertmacro MUI_STARTMENU_WRITE_END
  
  IfFileExists "$INSTDIR\demo.ksf" 0 +2
	CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Demo.lnk" "$INSTDIR\demo.ksf"
  IfFileExists "$INSTDIR\demo2.kage" 0 +2
	CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Demo2.lnk" "$INSTDIR\demo2.kage"
  
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\kagestudio.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\kagestudio.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"

  SetFileAttributes "$INSTDIR\README.txt" READONLY
  SetFileAttributes "$INSTDIR\LICENSE.txt" READONLY

  ;this enables "kage myfile.ksf" to open using command prompt -- not working somehow
  FileOpen $0 "$SYSDIR\kage.bat" w
    IfErrors done
    FileWrite $0 "@echo off$\r$\n"
    FileWrite $0 "$\"$INSTDIR\kagestudio.exe$\" %1$\r$\n"
  FileClose $0
  done:
  
  ;write assoc's
    ;KSF
    WriteRegStr HKCR ".kage" "" "KageStudio.File"
;    WriteRegStr HKCR ".kage\ShellNew" "NullFile" ""
    WriteRegStr HKCR "KageStudio.File" "" "Kage Studio File"
    WriteRegStr HKCR "KageStudio.File\shell\Open\command" "" "$INSTDIR\kagestudio.exe %1"
    WriteRegStr HKCR "KageStudio.File\DefaultIcon" "" "$INSTDIR\kage.ico"
    
  ;write assoc's
    ;KSF
    WriteRegStr HKCR ".ksf" "" "KageScene.File"
;    WriteRegStr HKCR ".ksf\ShellNew" "NullFile" ""
    WriteRegStr HKCR "KageScene.File" "" "Kage Scene File"
    WriteRegStr HKCR "KageScene.File\shell\Open\command" "" "$INSTDIR\kagestudio.exe %1"
    WriteRegStr HKCR "KageScene.File\DefaultIcon" "" "$INSTDIR\ksf.ico"
    
    ;WriteRegStr HKCR "dir" "entry" "value" -- template
    
    ;WriteRegStr HKCR "Kage Studio.File\shell\Open\command" "" "KSE_PATH %1"
  
  ;unzip Adwaita.zip then delete after
  InitPluginsDir
    ; Call plug-in. Push filename to ZIP first, and the dest. folder last.
    nsisunz::UnzipToLog "$INSTDIR\share\icons\icons.zip" "$INSTDIR\share\icons\"
     
    ; Always check result on stack
    Pop $0
    StrCmp $0 "success" ok
      DetailPrint "$0" ;print error message to log
    ok:
    Delete "$INSTDIR\share\icons\icons.zip"
  
SectionEnd

LangString DESC_Section1 ${LANG_ENGLISH} "Kage Studio core files."
;LangString DESC_Section1 ${LANG_FILIPINO} "Kage Studio lamang."
LangString DESC_Section2 ${LANG_ENGLISH} "Simple Demo."
;LangString DESC_Section2 ${LANG_FILIPINO} "Simpleng Demo."
LangString DESC_Section3 ${LANG_ENGLISH} "Kage Studio is written with Gtkmm 3.0. If you are not sure if you have Gtkmm on your machine, it is highly recommended to keep it selected."
;LangString DESC_Section3 ${LANG_FILIPINO} "Ang Kage Studio ay isinulat kasama ng Gtkmm 3.0. Kung hindi ka sigurado na ikaw ay may Gtkmm sa iyong laptop/desktop, mariing hayaang naka-check ang Gtkmm."
LangString DESC_Section4 ${LANG_ENGLISH} "Plugins enable Kage Studio to import/export media files in/out of Kage Studio. If you are not sure if you have Kage Studio plugins on your machine, it is highly recommended to keep it selected."
;LangString DESC_Section4 ${LANG_FILIPINO} "Ang Plugins ang katulong ng Kage Studio upang maka import/export ng media files papasok/palabas ng Kage Studio. Kung hindi ka sigurado na ikaw ay mayKage Studio plugins sa iyong laptop/desktop, mariing hayaang naka-check ang Plugins."

!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC01} $(DESC_Section1)
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC02} $(DESC_Section2)
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC03} $(DESC_Section3)
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC04} $(DESC_Section4)
!insertmacro MUI_FUNCTION_DESCRIPTION_END

Function .onInit
 
  ReadRegStr $R0 HKLM \
  "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}" \
  "UninstallString"
  StrCmp $R0 "" done
 
  MessageBox MB_OKCANCEL|MB_ICONEXCLAMATION \
  "${PRODUCT_NAME} is already installed. $\n$\nClick `OK` to remove the \
  previous version or `Cancel` to cancel this upgrade." \
  IDOK uninst
  Abort
 
;Run the uninstaller
uninst:
  ClearErrors
  Exec $INSTDIR\uninst.exe ; instead of the ExecWait line
 
done:
 
FunctionEnd

Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "Restart your PC to completely uninstall $(^Name)."
FunctionEnd

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Uninstall $(^Name)?" IDYES +2
  Abort
FunctionEnd

Section Uninstall
  Delete "$INSTDIR\uninst.exe"
  
  RMDir /r "$INSTDIR"
  
  IfFileExists "$INSTDIR\demo.ksf" 0 +2
	Delete "$SMPROGRAMS\$StartMenuFolder\Demo.lnk"
  IfFileExists "$INSTDIR\demo2.kage" 0 +2
  	Delete "$SMPROGRAMS\$StartMenuFolder\Demo2.lnk"
  
  !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder
    
  RMDir /r "$SMPROGRAMS\$StartMenuFolder"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  
  ;remove reg entries made by installer
  DeleteRegKey HKCR ".kage"
  DeleteRegKey HKCR "Kage Studio"
  DeleteRegKey HKCR "KageStudio.File"
  
  DeleteRegKey HKCR ".ksf"
  DeleteRegKey HKCR "Kage Scene"
  DeleteRegKey HKCR "KageScene.File"
  
  DeleteRegKey /ifempty HKLM "Software\Kage Studio"
  
  SetAutoClose true
SectionEnd
