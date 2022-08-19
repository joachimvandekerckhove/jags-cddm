!define APP_NAME "JAGS-wiener"
!define PUBLISHER "JAGS-wiener"

;Name used for JAGS Wiener module registry keys
!define WIENER_KEYNAME "${APP_NAME}-${VERSION}"
;Name visible to users, for shortcuts, installation directories, etc.
!define WIENER_VISIBLE_NAME  "${APP_NAME} ${VERSION}"

;JAGS Variables
!define J_PUBLISHER "JAGS"
!define JAGS_KEYNAME "JAGS-4.0.0"

!define MULTIUSER_MUI
!define MULTIUSER_EXECUTIONLEVEL Highest
!define MULTIUSER_INSTALLMODE_COMMANDLINE

!define INSTDIR_REG_ROOT "SHELL_CONTEXT"
!define INSTDIR_REG_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${WIENER_KEYNAME}"

!define MULTIUSER_INSTALLMODE_INSTDIR "${J_PUBLISHER}\${JAGS_KEYNAME}"
!define MULTIUSER_INSTALLMODE_INSTDIR_REGISTRY_KEY "SOFTWARE\${J_PUBLISHER}\${JAGS_KEYNAME}"
!define MULTIUSER_INSTDIR_REGISTRY_VALUENAME "InstallDir"
!define UNINSTALL_LOG "Uninstall-wiener"

!addincludedir ${JAGSINC}
!include AdvUninstLog.nsh
!include MultiUser64.nsh
!include "MUI2.nsh"
!include "Sections.nsh"
!include "x64.nsh"
!include "LogicLib.nsh"
 
Name "${WIENER_VISIBLE_NAME}"
OutFile "${APP_NAME}-${VERSION}.exe"

!define APP_REG_KEY "Software\${PUBLISHER}\${WIENER_KEYNAME}"
!define PUB_REG_KEY "Software\${PUBLISHER}"


; Installer pages

!insertmacro UNATTENDED_UNINSTALL

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE ${LICENSE}
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

!insertmacro MUI_LANGUAGE "English"

Section #Default section

   WriteRegStr ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "InstallDir" "$INSTDIR"
   WriteRegStr ${INSTDIR_REG_ROOT} "${APP_REG_KEY}"     "InstallDir" "$INSTDIR"

   # Information for uninstaller
   WriteRegStr ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "DisplayName" "${WIENER_VISIBLE_NAME}"
   WriteRegStr ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "UninstallString" "${UNINST_EXE}"
   WriteRegStr ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "Publisher" "${PUBLISHER}"
   WriteRegStr ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "DisplayVersion" "${VERSION}"
   WriteRegStr ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "URLInfoAbout" "http://jags-wiener.sourceforge.net"
   WriteRegDWORD ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "NoModify" 1
   WriteRegDWORD ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "NoRepair" 1
   WriteRegStr ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "InstallLocation" "$INSTDIR"

SectionEnd

Section "32-bit installation" Sec32

   SetOutPath "$INSTDIR\i386\modules"
   !insertmacro UNINSTALL.LOG_OPEN_INSTALL
   File /r inst32\lib\JAGS\modules-${MAJOR}\wiener*
   !insertmacro UNINSTALL.LOG_CLOSE_INSTALL

   Push @JAGS_HOME@               #text to be replaced
   Push $INSTDIR\i386             #replace with
   Push all                       #replace all occurrences
   Push all                       #replace all occurrences

SectionEnd #32-bit installation

Section "64-bit installation" Sec64

   SetOutPath "$INSTDIR\x64\modules"
   !insertmacro UNINSTALL.LOG_OPEN_INSTALL
   File /r inst64\lib\JAGS\modules-${MAJOR}\wiener*
   !insertmacro UNINSTALL.LOG_CLOSE_INSTALL

   Push @JAGS_HOME@               #text to be replaced
   Push $INSTDIR\x64              #replace with
   Push all                       #replace all occurrences
   Push all                       #replace all occurrences

SectionEnd #64-bit installation

!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${Sec32} "Files for 32-bit Windows"
  !insertmacro MUI_DESCRIPTION_TEXT ${Sec64} "Files for 64-bit Windows"
!insertmacro MUI_FUNCTION_DESCRIPTION_END

Function .onInit
   !insertmacro MULTIUSER_INIT
   !insertmacro UNINSTALL.LOG_PREPARE_INSTALL
   ${If} ${RunningX64}
      ;Nothing to do
   ${Else}
      ; Deselect and hide 64-bit section
      Push $0
      SectionGetFlags ${Sec64} $0
      IntOp $0 $0 & ${SECTION_OFF}
      SectionSetFlags ${Sec64} $0
      SectionSetText  ${Sec64} ""
      Pop $0
      ; Enforce 32-bit selection
      Push $1
      SectionGetFlags ${Sec32} $1
#      IntOp $1 $1 & ${SF_SELECTED}
      IntOp $1 $1 | ${SF_RO}
      SectionSetFlags ${Sec32} $1
      Pop $1
   ${EndIf}
FunctionEnd

Function .onInstSuccess
   ;create/update log always within .onInstSuccess function
   !insertmacro UNINSTALL.LOG_UPDATE_INSTALL
FunctionEnd

Section "Uninstall"

   ;uninstall from path, must be repeated for every install logged path individually
   !insertmacro UNINSTALL.LOG_UNINSTALL "$INSTDIR\i386\modules"
   !insertmacro UNINSTALL.LOG_UNINSTALL "$INSTDIR\x64\modules"
   !insertmacro UNINSTALL.LOG_UNINSTALL "$INSTDIR"
   !insertmacro UNINSTALL.LOG_END_UNINSTALL

   DeleteRegKey /ifempty ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}"
   DeleteRegKey /ifempty ${INSTDIR_REG_ROOT} "${APP_REG_KEY}"
   DeleteRegKey /ifempty ${INSTDIR_REG_ROOT} "${PUB_REG_KEY}"

SectionEnd # end of uninstall section

Function un.onInit
   !insertmacro MULTIUSER_UNINIT
   !insertmacro UNINSTALL.LOG_BEGIN_UNINSTALL
FunctionEnd

