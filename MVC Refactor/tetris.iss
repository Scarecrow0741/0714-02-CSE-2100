[Setup]
AppName=Tetris
AppVersion=1.0
DefaultDirName={pf}\Tetris
DefaultGroupName=Tetris

[Files]
Source: "tetris.exe"; DestDir: "{app}"
Source: "*.dll"; DestDir: "{app}"
Source: "docs\*"; DestDir: "{app}\docs"; Flags: recursesubdirs
Source: "lib\*"; DestDir: "{app}\lib"; Flags: recursesubdirs
Source: "src\*"; DestDir: "{app}\src"; Flags: recursesubdirs

