echo off
cd c:\

echo ************************************************************
echo            AIM104-MOTION-1 Install program. v1.1
echo ************************************************************
echo Installing all files to c:\motion ..........
REM - ROOT DIRECTORY - change the following 2 lines to change destination
md MOTION
cd c:\motion

md EXAMPLES
md DOCS
md INCLUDE
md REGISTER
cd examples
copy a:\examples\*.*
cd ..\docs
copy a:\docs\*.*
cd ..\include
copy a:\include\*.*
cd ..\register
copy a:\register\*.*
cd c:\
  
echo Installation complete.

echo on
