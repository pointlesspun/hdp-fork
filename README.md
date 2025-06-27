Winutils Build Hadoop 3.5.0
===========================

This fork contain adaptations to build WinUtils which is needed to run Apache Spark on Windows. 

For the latest information about Hadoop, please visit our website at:

   http://hadoop.apache.org/

and our wiki, at:

   https://cwiki.apache.org/confluence/display/HADOOP/


Setup for building WinUtils
---------------------------

In order to build WinUtils we'll generallty follow the steps in Building.txt, however we will be making some changes and have different requirements.

The building.txt recommends "dev-support/docker/Dockerfile_windows_10. It is highly recommended to use this" [Building.txt]. However this requires Docker and a version of windows 11 professional to run windows containers, both were not available to me as I can only use Rancher and have windows 11 home. So we're building this from scratch.

We build WinUtils using:

* Windows 11 Home (was Windows 10)
* openjdk version "23.0.1" 2024-10-15 (Was JDK 1.8) 
* Apache Maven 3.9.9 (was Maven 3.0 or later (maven.apache.org))
* Boost 1.86.0 (boost.org) is required but I believe this comes in via bootstrap-vcpkg.bat in  a later step
* libprotoc 28.2 (was Protocol Buffers 3.21.12 (https://github.com/protocolbuffers/protobuf/tags))
* cmake version 4.0.3 (was CMake 3.19 or newer (cmake.org))
* Visual Studio 2022 (visualstudio.com) (was Visual Studio 2019 (visualstudio.com)
* Zlib (zlib.net, if building native code bindings for zlib) (I believe this comes in via bootstrap-vcpkg.bat in  a later step)
* Git (preferably, get this from https://git-scm.com/download/win since the package also contains
       Unix command-line tools that are needed during packaging).
* Python (python.org, for generation of docs using 'mvn site')
* Internet connection for first build (to fetch all Maven and Hadoop dependencies)
* 7Zip to read tars mostly. 

Note: the most changes are the result of using VS2022 and Protobuf 28.2. We needed up to date versions, but I suspect building WinUtil with the recommended versions will make it somewhat easier.

> "Keep the source code tree in a short path to avoid running into problems related
> to Windows maximum path length limitation (for example, C:\hdc)." 

[Building.txt] This is STILL a problem on Windows 11, so follow this advice.

> "There is one support command file located in dev-support called win-paths-eg.cmd.
> It should be copied somewhere convenient and modified to fit your needs."
> 
> "win-paths-eg.cmd sets up the environment for use. You will need to modify this
> file. It will put all of the required components in the command path,
> configure the bit-ness of the build, and set several optional components."[Building.txt]
> 

Basically you'll need to go this file and change the paths to match the setup you have on your computer.


We use vcpkg (https://github.com/microsoft/vcpkg.git) for installing Boost, Protocol buffers,
OpenSSL and Zlib dependencies. Run the following commands to setup these dependencies.
> git clone https://github.com/microsoft/vcpkg.git
> cd vcpkg
> git fetch --all
> git checkout 2025.03.19
> .\bootstrap-vcpkg.bat
(Assuming that vcpkg was checked out at C:\vcpkg and Hadoop at C:\hadoop)
> copy C:\hadoop\dev-support\docker\vcpkg\vcpkg.json C:\vcpkg
> .\vcpkg.exe install --x-install-root .\installed

Set the following environment variables -
(Assuming that vcpkg was checked out at C:\vcpkg)
> set PROTOBUF_HOME=C:\vcpkg\installed\x64-windows
> set MAVEN_OPTS=-Xmx2048M -Xss128M"

Additionally you might need to add the following settings and add other tools to your path, once vcpkg has been installed:

powershell
```
set PATH = %PATH%";C:\your\vcpkg\path\vcpkg"
set PATH = %PATH%";"C:\your\cmake\path\cmake-4.0.3"
```

```
set ZLIB_HOME="C:\your\vcpkg\path\vcpkg\installed\x64-windows"
```


Building WinUtils
-----------------

We will build one project at the time. To just build WinUtils we need to build the `hadoop-common-project`. 

Open the "C:\hdp\hadoop-common-project\hadoop-common\src\main\winutils.sln" with visual studio and retarget both projects to the latest version (right click each project and choose 'retarget projects').

Make sure the preprocessor definitions for both projects include (or define the marcos)

```
WSCE_CONFIG_DIR=prod;
WSCE_CONFIG_FILE=config.json
```

Run the x64 native Tools Command prompt, which should give you the following:

```cmd
**********************************************************************
** Visual Studio 2022 Developer Command Prompt v17.14.6
** Copyright (c) 2025 Microsoft Corporation
**********************************************************************
[vcvarsall.bat] Environment initialized for: 'x64'

C:\Program Files\Microsoft Visual Studio\2022\Community>
```
Make sure to run the `dev-support/win-paths-eg.cmd` file and have the zlib path set (`set ZLIB_HOME="C:\your\vcpkg\path\vcpkg\installed\x64-windows"`). 

We will be building with

```cmd
mvn clean install -Denforcer.skip=true -Dhttps.protocols=TLSv1.2 -Dwindows.cmake.c.flags="/w" -Dwindows.cmake.cxx.flags="/w" -DskipTests -DskipDocs -Dmaven.javadoc.skip=true -Pnative-win,dist -Dskip.platformToolsetDetection -Drequire.openssl -Drequire.test.libhadoop -Pyarn-ui  -Dshell-executable="%GIT_HOME%\bin\bash.exe" -Dtar -Dopenssl.prefix=C:\Users\lassc\Code\tools\vcpkg\vcpkg\installed\x64-windows -Dcmake.prefix.path=C:\Users\lassc\Code\tools\vcpkg\vcpkg\installed\x64-windows -Dwindows.cmake.toolchain.file=C:\Users\lassc\Code\tools\vcpkg\vcpkg\scripts\buildsystems\vcpkg.cmake -Dwindows.cmake.build.type=RelWithDebInfo -Dwindows.build.hdfspp.dll=off -Dwindows.no.sasl=on -Duse.platformToolsetVersion=v143
```
Note: since we're compiling using the VS2022 toolset, the important bit here is the "-Duse.platformToolsetVersion=v143"

When compiling, the first error we encounter is:

```cmd
 C:\hdp\hadoop-common-project\hadoop-common\src\main\winutils\include\winutils.h(129,7): error C2733: 'GetFileInformationByName': you cannot overload a function with 'extern "C"' linkage [C:\hdp\hadoop-common-project\hadoop-common\src\main\winutils\libwinutils.vcxproj]
 ```

 The quickest way is to go into the winutils projects and replace the name (in all files in the project) by something like: `GetFileInformationByNameEx`

After this the winutils project should compile, however you will run into Cmake related issues after this. 

If you only need WinUtils however you can pickup the hadoop-common-3.5.0 tar which contains the winutils executables from `C:\hdp\hadoop-common-project\hadoop-common\target`.

In a future installment we'll go into building the other artifacts that have "traditionally" been a part of the WinUtils deployment. 

