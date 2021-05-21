## introduction

a cross-platform program framework that has least dependencies on os api.
the prefix `ep` of repository name is short for *"experiment"*,
which means this is a experimental project.

## directory structure

**program assets:**

the cross-platform codes and resources.
the *appxx* directories are used to contain user codes and resources,
and the *sharedxx* are shared libraries.

- appsrc/
- appres/
- sharedsrc/
- sharedres/

**build scripts:**

for building program and placing them to *BUILD/*.
files without suffix name for mac os, and *.bat for windows.

- build_android
- build_android.bat
- build_ios
- build_osx
- build_win32.bat

**header auxiliary tool:**

these scripts traverse headers and create new homonymic headers
which reference former to *HEADERS/*.
it will avoid developer adding vast header search paths tediously.

- genheaders
- genheaders.bat

**project files:**

these are projects managed by *android studio*, *xcode* or *visual studio*
for different os platform.

- program_android/
- program_ios/
- program_osx/
- program_win32/
