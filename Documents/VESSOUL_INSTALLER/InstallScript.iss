; TCR compliant Sample install script.
; Basically put this file in the root for what you want included in the installer,
;   then put everything into the subdirectories as listed in the installer below.
;   Remember to change the AppId
;   Thanks to Dan Weiss (dweiss@digipen.edu) for the original version.
;
; !!NOTE!!
; There are three steps that you must perform in order to customize this script
;   for your project (see "TODO" below):
;   1) Generate a new GUID for your game.
;   2) Search-and-Replace "MyGame" with your game name.
;   3) Update the redistributables required for your game.

[Setup]
; !!NOTE!!: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; TODO: (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{9BFD762A-CF61-4569-8B51-4C7288CD9A9B}}

; Standard app data stuff
; TODO: Search-and-Replace "GameTest" with your game name
AppName=Vessoul
AppVerName=Vessoul 1.0
AppPublisher=DigiPen Institute of Technology
AppPublisherURL=http://www.digipen.edu/
AppSupportURL=http://www.digipen.edu/

; Default path to the file storage directory.
; {pf} is the default program files directory set by Windows
DefaultDirName={pf}\DigiPen\Vessoul

; Start menu directory
DefaultGroupName=DigiPen\Vessoul

; Output directory for the installer.
OutputDir=.\INSTALLER

; Setup executable installer
OutputBaseFilename=Vessoul_Setup

; Path to the DigiPen EULA (Needed to pass TCRs)
LicenseFile=INSTALLERFILES\DigiPen_EULA.txt

; Compression scheme for the installer. Check Inno Setup help files for more options.
Compression=lzma
SolidCompression=yes

; Path to the icon for the installer (TCR check requires custom icon)
SetupIconFile=INSTALLERFILES\Setup-Icon.ico

; This allows the installer to run without admin privileges, which means you can't install
; in the Program Files, or change the registry. This is done to allow installation on Sandbox
; or other intermediate directory
PrivilegesRequired=none

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
; Creates an installer option to allow/disallow desktop shortcut
; Checked by default
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"

; This is the list of files that the installer should grab and install.
;
; Destination Directory Notes
;   {app} is the root directory that you will be installing to.
;   {temp} is a temporary directory that Windows sets that gets deleted after the
;      installer quits.
;   {userdocs} is the directory for My Documents/Documents on Windows XP, Vista, and 7.
;
; For more information on default paths to install to, check the "Constants" article
;   in the Inno Setup 5 documentation.
;
; I recommend placing any installers for required stuff (DirectX, PhysX, etc)
;   in the general structure below to keep things neat for you.
[Files]
; The game directoy is exactly what you want your install directory in program files to look like
Source: GAMEDIRECTORY\*; DestDir: {app}; Flags: ignoreversion recursesubdirs createallsubdirs
; Include the redistributable programs and install them to the temp directory
Source: REDIST\*; DestDir: {tmp}; Flags: ignoreversion
;Source: REDIST\*; DestDir: {tmp}; Flags: ignoreversion
;Source: .\REDIST\vcredist_x64.exe; DestDir: {tmp}; Flags: ignoreversion
;Source: .\REDIST\dxwebsetup.exe; DestDir: {tmp}; Flags: ignoreversion



; Include the redistributable programs and install them to the temp directory
; TODO: Update this list with the correct redistributables for your game.
;       If your game uses DirectX, then please refer to the following forum posting:
;	        https://distance.digipen.edu/mod/forum/discuss.php?d=19674&parent=39276
;Source: .\REDIST\*; DestDir: {tmp}; Flags: ignoreversion

; This is the list of shortcuts that the installer will setup for you.
; Of note, this will create the uninstaller automatically.
;
; Directory Notes
;   {group} is the start menu location that the game will install shortcuts to.
;   {commondesktop} is your Windows desktop directory.
[Icons]
Name: {group}\Vessoul; Filename: {app}\vessoul_game.exe; WorkingDir: {app}; IconFilename: "{app}\Setup-Icon.ico"
Name: {group}\{cm:UninstallProgram,Vessoul}; Filename: {uninstallexe}
Name: {commondesktop}\Vessoul; Filename: {app}\vessoul_game.exe; Tasks: desktopicon; WorkingDir: {app}; IconFilename: "{app}\Setup-Icon.ico"

; List of items to execute in the installer.
; Note that this will run all executables in their silent versions as required by the TCRs.
;
; The last item being run is the installer option to automatically launch the game after
;   the installer exits as required by the TCRs.
; TODO: Update this list with the correct redistributables for your game.
[Run]
Filename: {tmp}\vcredist_x86.exe; Parameters: /q; StatusMsg: Installing Visual C++ 2013 Redistributable...
;Filename: {tmp}\vcredist_x64.exe; Parameters: /q; StatusMsg: Installing Visual C++ 2013 Redistributable...
Filename: {tmp}\DXSETUP.exe; Parameters: /silent; StatusMsg: Installing DirectX9 Redistributable...
Filename: {app}\vessoul_game.exe; Description: {cm:LaunchProgram,Vessoul}; Flags: nowait postinstall skipifsilent
