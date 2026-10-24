#define MyAppName "SushiGram Desktop"
#define MyAppVersion "1.0.0"
#define MyAppExeName "SushiGram.exe"

[Setup]
AppId={{D789EFC1-445E-4F5E-886F-B34399C98C8F}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher=ShadowsocksM
DefaultDirName={autopf}\SushiGram
DefaultGroupName=SushiGram
DisableProgramGroupPage=yes
OutputDir=C:\MR_NagramXF_Yukigram\tdesktop\Installer
OutputBaseFilename=SushiGram-Windows-x64-Setup
Compression=lzma2/ultra64
SolidCompression=yes
ArchitecturesAllowed=x64compatible
ArchitecturesInstallIn64BitMode=x64compatible
PrivilegesRequired=admin
UninstallDisplayName=SushiGram
WizardStyle=modern

[Files]
Source: "C:\MR_NagramXF_Yukigram\tdesktop\out\SushiGram.exe"; DestDir: "{app}"; DestName: "SushiGram.exe"; Flags: ignoreversion

[Icons]
Name: "{group}\SushiGram"; Filename: "{app}\{#MyAppExeName}"; Parameters: "-workdir ""{userappdata}\SushiGram"""
Name: "{autodesktop}\SushiGram"; Filename: "{app}\{#MyAppExeName}"; Parameters: "-workdir ""{userappdata}\SushiGram"""; Tasks: desktopicon

[Tasks]
Name: "desktopicon"; Description: "Create a desktop shortcut"; GroupDescription: "Additional shortcuts:"; Flags: unchecked

[Run]
Filename: "{app}\{#MyAppExeName}"; Parameters: "-workdir ""{userappdata}\SushiGram"""; Description: "Launch SushiGram"; Flags: nowait postinstall skipifsilent










