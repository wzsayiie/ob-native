## introduction

a cross-platform program that has least dependencies on os api and gpu api.
the prefix `ep` of repository name is short for *"`e`x`p`eriment"*,
which means this is a experimental project.

## directory structure

**build scripts:**

for building program and placing them to *BUILD/*.
files without suffix name for mac os, and *.bat for windows.

* build_android
* build_android.bat
* build_ios
* build_osx
* build_win32.bat

**header auxiliary tool:**

these scripts traverse headers and create new homonymic headers
which reference former to *GHEADERS/*.
it will avoid developer adding vast header search paths tediously.

* headers
* headers.bat

**program assets:**

the cross-platform source and resource.

* mainsrc/
* mainres/

**project files:**

these are projects managed by *android studio*, *xcode* or *visual studio*
for different os platform.

* program_android/
* program_ios/
* program_osx/
* program_win32/
