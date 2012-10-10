:: C:\dev\marmalade\6.0\s3e\python\python.exe C:\dev\marmalade\6.0\extensions\builder\make_extension.py s3eGameCircle.s4e --force --platform=android

C:\dev\marmalade\6.0\s3e\python\python.exe C:\dev\marmalade\6.0\s3e\makefile_builder\mkb.py --open-with-wmlaunchpad s3eGameCircle_android_java.mkb
@if errorlevel 1 goto :eof
C:\dev\marmalade\6.0\s3e\python\python.exe C:\dev\marmalade\6.0\s3e\makefile_builder\mkb.py --open-with-wmlaunchpad s3eGameCircle_android.mkb --debug
@if errorlevel 1 goto :eof
C:\dev\marmalade\6.0\s3e\python\python.exe C:\dev\marmalade\6.0\s3e\makefile_builder\mkb.py --open-with-wmlaunchpad s3eGameCircle_android.mkb --release
@if errorlevel 1 goto :eof
