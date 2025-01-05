@echo off
set project=spum

@echo on
mkdir intermediate

@echo on
cl.exe ^
/Zi /Od /JMC ^
/Fe%project%.exe /Fo.\intermediate\ /Fd.\intermediate\ ^
..\source\main.c ^
/I..\external\sokol ^
/MDd ^
/std:c11 ^
/EHa ^
/W3 /wd"4530" /wd"4201" /wd"4577" /wd"4310" /wd"4624" /wd"4099" /wd"4068" /wd"4267" /wd"4244" /wd"4018" ^
/D "FM_DEBUG" /D "_CRT_SECURE_NO_WARNINGS" /D "_SILENCE_CXX17_ALL_DEPRECATION_WARNINGS" /D "SOKOL_GLCORE" ^
/link /DEBUG:FULL /MACHINE:X64 /NOLOGO /SUBSYSTEM:CONSOLE /INCREMENTAL:NO /NOIMPLIB /NOEXP /PDB:.\intermediate\ ^
"user32.lib" "opengl32.lib" "gdi32.lib" "Shell32.lib" "Kernel32.lib" "Advapi32.lib" "Ole32.lib" "OleAut32.lib" 

@echo off
if %errorlevel% neq 0 (
    pause
)