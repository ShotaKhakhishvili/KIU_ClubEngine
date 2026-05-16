# ClubEngine

ClubEngine is a small C++ game/engine framework. The engine source lives in this repository, while game projects can live anywhere on disk. Projects connect to the engine through the `CLUBENGINE_ROOT` environment variable, a `.clubproject` file, and the ClubEngine tool scripts.

The engine requires **C++20** and **CMake 3.20** or newer. Third-party libraries (GLFW, GLAD, GLM) are bundled — no separate downloads needed.

---

## Table of Contents

- [Windows Setup](#windows-setup)
  - [1. Install dependencies](#1-install-dependencies-windows)
  - [2. First-time tool setup](#2-first-time-tool-setup-windows)
  - [3. Build the engine](#3-build-the-engine-windows)
  - [4. Create a project](#4-create-a-project-windows)
  - [5. Build and run your project](#5-build-and-run-your-project-windows)
  - [6. Register an existing project](#6-register-an-existing-project-windows)
  - [7. Reset the environment](#7-reset-the-environment-windows)
- [Linux Setup](#linux-setup)
  - [1. Install dependencies](#1-install-dependencies-linux)
  - [2. First-time tool setup](#2-first-time-tool-setup-linux)
  - [3. Build the engine](#3-build-the-engine-linux)
  - [4. Create a project](#4-create-a-project-linux)
  - [5. Build and run your project](#5-build-and-run-your-project-linux)
  - [6. Register an existing project](#6-register-an-existing-project-linux)
  - [7. Reset the environment](#7-reset-the-environment-linux)
- [Engine Association](#engine-association)
- [Folder Layout](#folder-layout)

---

---

# Windows Setup

---

## 1. Install dependencies (Windows)

ClubEngine on Windows uses **MSYS2 UCRT64 / MinGW** for its compiler and build tools.

**Step 1 — Install MSYS2**

> **Note:** MSVC and Visual Studio are not supported. The build system requires
> MinGW via MSYS2 specifically — even if you already have Visual Studio installed,
> you still need to complete this step.

Download and install MSYS2 from [https://www.msys2.org](https://www.msys2.org).

**Step 2 — Open the MSYS2 UCRT64 terminal** (not the default MSYS2 terminal — specifically the UCRT64 one) and run:

```bash
pacman -S mingw-w64-ucrt-x86_64-gcc
pacman -S mingw-w64-ucrt-x86_64-cmake
pacman -S mingw-w64-ucrt-x86_64-make
```

**Step 3 — Add MinGW to your Windows PATH**

Add the following folder to your Windows system `PATH` environment variable:

```
C:\msys64\ucrt64\bin
```

To do this: Search for **"Edit the system environment variables"** in the Start menu → Environment Variables → select `Path` under System variables → New → paste the path above.

**Step 4 — Verify from PowerShell**

Close and reopen PowerShell, then check:

```powershell
gcc --version
cmake --version
mingw32-make --version
```

All three should print version numbers. If any command is not found, double-check the PATH entry and reopen PowerShell.

---

## 2. First-time tool setup (Windows)

This registers `CLUBENGINE_ROOT` and adds the ClubEngine tools to your PATH so you can run `club-*` commands from anywhere.

Open PowerShell, navigate to the ClubEngine root folder, and run:

```powershell
.\Tools\Windows\club-init.bat
```

Then **close and reopen PowerShell** for the changes to take effect.

Verify the setup:

```powershell
echo $env:CLUBENGINE_ROOT
where.exe club-build-engine
where.exe club-new-project
where.exe club-build-project
```

All four should return valid paths. If any are missing, re-run `club-init.bat` and reopen the terminal.

---

## 3. Build the engine (Windows)

Run this from anywhere after completing the tool setup:

```powershell
club-build-engine Debug
```

Replace `Debug` with `Release` for an optimized build.

After a successful build, the following folders will be created inside the ClubEngine root:

```
Build\Win64\Debug\0.1.0\
Binaries\Win64\Debug\0.1.0\
Intermediate\Win64\Debug\0.1.0\
```

The version number (`0.1.0`) comes from the `VERSION` field in the root `CMakeLists.txt`. Both `Debug` and `Release` builds can coexist — they output to separate folders.

---

## 4. Create a project (Windows)

Build the engine first. Then run this from anywhere:

```powershell
club-new-project MyGame 0.1
```

The first argument is the project name (also used as the folder name). The second is the engine line to associate with — see [Engine Association](#engine-association) below.

You can also pass a full path:

```powershell
club-new-project C:\Users\YourName\Projects\MyGame 0.1
```

This creates the following structure:

```
MyGame\
  MyGame.clubproject
  CMakeLists.txt
  Config\
    DefaultEngine.ini
    DefaultInput.ini
    DefaultGame.ini
    DefaultRender.ini
  Content\
  Source\
    Entry.cpp
    MyGame\
      Public\
        MyGame.h
      Private\
        MyGame.cpp
```

The project is automatically registered after creation.

---

## 5. Build and run your project (Windows)

Navigate to the project root, then build:

```powershell
cd C:\Users\YourName\Projects\MyGame
club-build-project Debug
```

The build script automatically reads the `.clubproject` file, finds a compatible built engine, and compiles the project.

After a successful build the executable is at:

```
MyGame\Binaries\Win64\Debug\0.1.0\MyGame.exe
```

Run it directly from the terminal:

```powershell
.\Binaries\Win64\Debug\0.1.0\MyGame.exe
```

---

## 6. Register an existing project (Windows)

New projects are registered automatically when created. If you move a project folder or want to register a project that was created manually, run this from the project root:

```powershell
club-register-project
```

Registered projects are stored in:

```
<ClubEngine root>\Saved\ProjectRegistry.json
```

Projects are identified by their full path, so two projects with the same name in different directories can both be registered.

---

## 7. Reset the environment (Windows)

To remove `CLUBENGINE_ROOT` and the ClubEngine tools from your PATH:

```powershell
club-clean-env
```

Or run it directly:

```powershell
.\Tools\Windows\club-clean-env.bat
```

Reopen PowerShell after running.

---

---

# Linux Setup

> **Note:** Linux support is functional but has received limited testing. You may encounter rough edges. Please report any issues.

---

## 1. Install dependencies (Linux)

On Ubuntu or Debian:

```bash
sudo apt update
sudo apt install build-essential cmake ninja-build
```

Because GLFW is bundled and compiled from source, it also needs its system development headers. Install them with:

```bash
sudo apt install libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libgl1-mesa-dev
```

Without these headers, the GLFW build will fail even though no separate download is required.

Verify the build tools:

```bash
g++ --version
cmake --version
ninja --version
```

All three should print version numbers.

---

## 2. First-time tool setup (Linux)

This registers `CLUBENGINE_ROOT` and adds the ClubEngine tools to your PATH.

From the ClubEngine root:

```bash
chmod +x Tools/Unix/*.sh
./Tools/Unix/club-init.sh
source ~/.bashrc
```

If you use zsh:

```bash
source ~/.zshrc
```

Verify the setup:

```bash
echo $CLUBENGINE_ROOT
which club-build-engine.sh
which club-new-project.sh
which club-build-project.sh
```

All four should return valid values. If any are missing, re-run `club-init.sh` and re-source your shell config.

---

## 3. Build the engine (Linux)

Run this from anywhere after completing the tool setup:

```bash
club-build-engine.sh Debug
```

Replace `Debug` with `Release` for an optimized build.

After a successful build, the following folders will be created inside the ClubEngine root:

```
Build/Linux64/Debug/0.1.0/
Binaries/Linux64/Debug/0.1.0/
Intermediate/Linux64/Debug/0.1.0/
```

If Ninja is installed it will be used automatically; otherwise CMake falls back to the default Make generator.

---

## 4. Create a project (Linux)

Build the engine first. Then run this from anywhere:

```bash
club-new-project.sh MyGame 0.1
```

The first argument is the project name (also used as the folder name). The second is the engine line to associate with — see [Engine Association](#engine-association) below.

You can also pass a full path:

```bash
club-new-project.sh ~/projects/MyGame 0.1
```

This creates the following structure:

```
MyGame/
  MyGame.clubproject
  CMakeLists.txt
  Config/
    DefaultEngine.ini
    DefaultInput.ini
    DefaultGame.ini
    DefaultRender.ini
  Content/
  Source/
    Entry.cpp
    MyGame/
      Public/
        MyGame.h
      Private/
        MyGame.cpp
```

The project is automatically registered after creation.

---

## 5. Build and run your project (Linux)

Navigate to the project root, then build:

```bash
cd ~/projects/MyGame
club-build-project.sh Debug
```

The build script automatically reads the `.clubproject` file, finds a compatible built engine, and compiles the project.

After a successful build the executable is at:

```
MyGame/Binaries/Linux64/Debug/0.1.0/MyGame
```

Run it:

```bash
./Binaries/Linux64/Debug/0.1.0/MyGame
```

---

## 6. Register an existing project (Linux)

New projects are registered automatically when created. If you move a project folder or want to register a project that was created manually, run this from the project root:

```bash
club-register-project.sh
```

Registered projects are stored in:

```
<ClubEngine root>/Saved/ProjectRegistry.json
```

Projects are identified by their full path, so two projects with the same name in different directories can both be registered.

---

## 7. Reset the environment (Linux)

To remove `CLUBENGINE_ROOT` and the ClubEngine tools from your PATH:

```bash
club-clean-env.sh
```

Then re-source your shell config or open a new terminal.

---

---

# Engine Association

Each project has a `.clubproject` file that specifies which engine version it requires:

```json
{
    "FileVersion": 1,
    "EngineAssociation": "0.1"
}
```

`EngineAssociation` is a version *line*, not an exact version. `0.1` matches any built engine with a `0.1.x` patch version:

| EngineAssociation | Matches | Does not match |
|---|---|---|
| `0.1` | `0.1.0`, `0.1.1`, `0.1.2` | `0.2.0`, `1.0.0` |
| `0.2` | `0.2.0`, `0.2.3` | `0.1.0`, `1.0.0` |

When building a project, the build script scans the `Binaries/` folder for the newest built engine that matches the association. If no match is found, the build fails with a clear error message.

---

# Folder Layout

```
ClubEngine/
  CMakeLists.txt          — Root build file (defines engine version)
  Engine/
    Source/Runtime/       — Engine source modules
    ThirdParty/           — Bundled libraries (GLFW, GLAD, GLM)
    Config/               — Base engine config files
    Programs/             — Standalone tools (e.g. AssetImporter)
  Templates/
    Project/              — Template used by club-new-project
  Tools/
    Windows/              — .bat scripts for Windows
    Unix/                 — .sh scripts for Linux
  Saved/
    ProjectRegistry.json  — List of registered projects (auto-generated)
  Build/                  — CMake build trees (generated)
  Binaries/               — Compiled engine output (generated)
  Intermediate/           — Intermediate build artifacts (generated)
```

`Build/`, `Binaries/`, and `Intermediate/` are all generated — they are not in source control and can be deleted to do a clean rebuild.

Game project assets should always live in the project's own `Content/` folder, never inside the engine source tree.
