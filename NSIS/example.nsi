!define APPNAME "MYTD"
!define UNINSTALLNAME "Uninstall"
!define COMPANYNAME "Minh"
!define HELPURL "http://minhinc.com/product/home.html"
!define UPDATEURL "http://minhinc.com/product/home.html"
!define ABOUTURL "http://minhinc.com/product/about.html"
!define VERSIONMAJOR 0
!define VERSIONMINOR 1
!define VERSIONBUILD 3
!define INSTALLSIZE 24000
!define SOURCEDIRECTORY "C:\temp\qt_static\tempgfd"
!macro BIMAGE IMAGE PARMS
        Push $0
        GetTempFileName $0
        File /oname=$0 "${IMAGE}"
        SetBrandingImage ${PARMS} $0
        Delete $0
        Pop $0
!macroend
outFile "minhytd.exe"
;installDir "$PROGRAMFILES\${COMPANYNAME}\${APPNAME}"
installDir "$PROGRAMFILES"
Name "Minh YouTube Downloader"
LicenseText "License Agreement: You must accept the terms contained in this agreement before continuing with the installation."
LicenseData "${SOURCEDIRECTORY}\license.rtf"
DirText "Setup will create subfolder Minh/MYTD and install Minh YouTube Downloader v0.1. To install in a different folder, click Browse button. Click Install to start the installation."
; "It will create extra folder Minh/MYTD"
XPStyle on
AddBrandingImage top 88
Page license licenseImage
Page directory directoryImage
Page instfiles instImage
;UninstPage uninstConfirm un.uninstImage
UninstPage instfiles un.instImage
Function licenseImage
!insertmacro BIMAGE "${SOURCEDIRECTORY}\licenseicon.bmp" ""
FunctionEnd
Function directoryImage
!insertmacro BIMAGE "${SOURCEDIRECTORY}\directoryicon.bmp" ""
FunctionEnd
Function instImage
!insertmacro BIMAGE "${SOURCEDIRECTORY}\installicon.bmp" ""
FunctionEnd
Function un.instImage
!insertmacro BIMAGE "${SOURCEDIRECTORY}\confirmuninstallation.bmp" ""
FunctionEnd
#Create a default section
section "install"
setOutPath "$INSTDIR\${COMPANYNAME}\${APPNAME}"
file "${SOURCEDIRECTORY}\release\gfd.exe"
file "${SOURCEDIRECTORY}\topicon_XaE_icon.ico"
writeUninstaller "$INSTDIR\${COMPANYNAME}\${APPNAME}\uninstaller.exe"
;createDirectory "$SMPROGRAMS\${COMPANYNAME}"
createDirectory "$SMPROGRAMS\${COMPANYNAME}\${APPNAME}"
createShortCut "$SMPROGRAMS\${COMPANYNAME}\${APPNAME}\${APPNAME}.lnk" "$INSTDIR\${COMPANYNAME}\${APPNAME}\gfd.exe" "" $INSTDIR\${COMPANYNAME}\${APPNAME}\topicon_XaE_icon.ico"
createShortCut "$SMPROGRAMS\${COMPANYNAME}\${APPNAME}\${UNINSTALLNAME}.lnk" "$INSTDIR\${COMPANYNAME}\${APPNAME}\uninstaller.exe" "" $INSTDIR\${COMPANYNAME}\${APPNAME}\topicon_XaE_icon.ico"
#closes the section
#registry information for add/remove programs
        WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "DisplayName" "${COMPANYNAME} - ${APPNAME}"
        WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "UninstallString" "$\"$INSTDIR\${COMPANYNAME}\${APPNAME}\uninstaller.exe$\""
        WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "QuietUninstallString" "$\"$INSTDIR\${COMPANYNAME}\${APPNAME}\uninstaller.exe$\" /S"
        WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "InstallLocation" "$\"$INSTDIR\${COMPANYNAME}\${APPNAME}$\""
;        WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "DisplayIcon" "$\"$INSTDIR\${COMPANYNAME}\${APPNAME}\topicon_XaE_icon.ico$\""
        WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "DisplayIcon" "$\"$INSTDIR\${COMPANYNAME}\${APPNAME}\topicon_XaE_icon.ico$\""
;        WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "Publisher" "$\"${COMPANYNAME}$\""
        WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "Publisher" "${COMPANYNAME}"
        WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "HelpLink" "$\"${HELPURL}$\""
        WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "URLUpdateInfo" "$\"${UPDATEURL}$\""
        WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "URLInfoAbout" "$\"${ABOUTURL}$\""
        WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "DisplayVersion" "${VERSIONMAJOR}.${VERSIONMINOR}.${VERSIONBUILD}"
        WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "VersionMajor" ${VERSIONMAJOR}
        WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "VersionMinor" ${VERSIONMINOR}
        # There is no option for modifying or repairing the install
        WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "NoModify" 1
        WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "NoRepair" 1
        # Set the INSTALLSIZE constant (!defined at the top of this script) so Add/Remove Programs can accurately report the size
        WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "EstimatedSize" ${INSTALLSIZE}

sectionend
section "uninstall"
MessageBox MB_YESNO "Do you want to uninstall?" IDYES true IDNO false
true:
delete $INSTDIR\uninstaller.exe
delete "$SMPROGRAMS\${COMPANYNAME}\${APPNAME}\${APPNAME}.lnk"
delete "$SMPROGRAMS\${COMPANYNAME}\${APPNAME}\${UNINSTALLNAME}.lnk"
RMDir "$SMPROGRAMS\${COMPANYNAME}\${APPNAME}"
RMDir "$SMPROGRAMS\${COMPANYNAME}"
delete $INSTDIR\gfd.exe
delete "$INSTDIR\topicon_XaE_icon.ico"
rmDir $INSTDIR
rmDir "$INSTDIR\.."
DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}"
GoTo next2
false:
Quit
next2:
sectionend
