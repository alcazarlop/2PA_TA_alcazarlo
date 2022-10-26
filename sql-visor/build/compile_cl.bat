@ECHO OFF
@cls
del *.obj *.pdb *.ilk *.exe

cl /nologo /Zi /EHs /MDd /W4 -D_CRT_SECURE_NO_WARNINGS -I ..\include -I ..\deps\nfd\include -I ..\deps\sdl2\include -I ..\deps\imgui\ -I ..\deps\sqlite\include\ /c ..\src\window_controller.cc
cl /nologo /Zi /EHs /MDd /W4 -D_CRT_SECURE_NO_WARNINGS -I ..\include -I ..\deps\nfd\include -I ..\deps\sdl2\include -I ..\deps\imgui\ -I ..\deps\sqlite\include\ /c ..\src\imgui_controller.cc
cl /nologo /Zi /EHs /MDd /W4 -D_CRT_SECURE_NO_WARNINGS -I ..\include -I ..\deps\nfd\include -I ..\deps\sdl2\include -I ..\deps\imgui\ -I ..\deps\sqlite\include\ /c ..\src\sql_controller.cc
cl /nologo /Zi /EHs /MDd /W4 -D_CRT_SECURE_NO_WARNINGS -I ..\include -I ..\deps\nfd\include -I ..\deps\sdl2\include -I ..\deps\imgui\ -I ..\deps\sqlite\include\ /c ..\src\game_controller.cc
cl /nologo /Zi /EHs /MDd /W4 -D_CRT_SECURE_NO_WARNINGS -I ..\include -I ..\deps\nfd\include -I ..\deps\sdl2\include -I ..\deps\imgui\ -I ..\deps\sqlite\include\ /c ..\src\info_controller.cc
cl /nologo /Zi /EHs /MDd /W4 -D_CRT_SECURE_NO_WARNINGS -I ..\include -I ..\deps\nfd\include -I ..\deps\sdl2\include -I ..\deps\imgui\ -I ..\deps\sqlite\include\ /c ..\src\main.cc

cl /nologo /Zi /EHs /MDd /W4 -D_CRT_SECURE_NO_WARNINGS -I ..\include -I ..\deps\nfd\include -I ..\deps\sdl2\include -I ..\deps\imgui\ /c ..\deps\imgui\imgui.cpp
cl /nologo /Zi /EHs /MDd /W4 -D_CRT_SECURE_NO_WARNINGS -I ..\include -I ..\deps\nfd\include -I ..\deps\sdl2\include -I ..\deps\imgui\ /c ..\deps\imgui\imgui_draw.cpp
cl /nologo /Zi /EHs /MDd /W4 -D_CRT_SECURE_NO_WARNINGS -I ..\include -I ..\deps\nfd\include -I ..\deps\sdl2\include -I ..\deps\imgui\ /c ..\deps\imgui\imgui_tables.cpp
cl /nologo /Zi /EHs /MDd /W4 -D_CRT_SECURE_NO_WARNINGS -I ..\include -I ..\deps\nfd\include -I ..\deps\sdl2\include -I ..\deps\imgui\ /c ..\deps\imgui\imgui_widgets.cpp
cl /nologo /Zi /EHs /MDd /W4 -D_CRT_SECURE_NO_WARNINGS -I ..\include -I ..\deps\nfd\include -I ..\deps\sdl2\include -I ..\deps\imgui\ /c ..\deps\imgui\imgui_impl_sdl.cpp
cl /nologo /Zi /EHs /MDd /W4 -D_CRT_SECURE_NO_WARNINGS -I ..\include -I ..\deps\nfd\include -I ..\deps\sdl2\include -I ..\deps\imgui\ /c ..\deps\imgui\imgui_impl_sdlrenderer.cpp

cl /nologo /Zi /EHs /MDd /W4 /Fe:..\bin\main_d.exe *.obj /link /SUBSYSTEM:CONSOLE ..\deps\sdl2\lib\x86\SDL2.lib ..\deps\sdl2\lib\x86\SDL2main.lib ..\deps\imgui\lib\imgui_d.lib ..\deps\sqlite\lib\sqlite3_d.lib ..\deps\nfd\lib\Debug\x86\nfd_d.lib opengl32.lib shell32.lib user32.lib gdi32.lib comctl32.lib Ole32.lib

