@echo off

pushd build
cl ..\code\win32_tiny_16_basic.c /MTd /nologo /Od /std:c17 /TC /W4 /WX /ZI /link gdi32.lib user32.lib /INCREMENTAL:NO
popd