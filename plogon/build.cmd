scons platform=android target=template_debug arch=arm64 ANDROID_HOME=C:\android_sdk android_api_level=24
scons platform=android target=template_release arch=arm64 ANDROID_HOME=C:\android_sdk android_api_level=24
scons platform=windows target=template_debug arch=x64
scons platform=windows target=template_release arch=x64
copy godot_scannur\bin\*.dll ..\project\bin\ /Y
copy godot_scannur\bin\*.so ..\project\bin\lib*.so /Y