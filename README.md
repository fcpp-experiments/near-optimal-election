# Near-Optimal Knowledge-Free Resilient Leader Election

This repository contains reproducible experiments (in FCPP) and algorithms (in C++, for finding the best parameter _g_) for a leader election algorithm submitted to [Automatica](https://www.journals.elsevier.com/automatica).
For any issues with reproducing the experiments, please contact [Giorgio Audrito](mailto:giorgio.audrito@gmail.com).


## Setup

The next sections contain the FCPP setup instructions for the various supported OSs. Jump to the section dedicated to your system of choice and ignore the others.

### Windows

Pre-requisites:
- [Git Bash](https://gitforwindows.org) (for issuing unix-style commands)
- [MinGW-w64](http://mingw-w64.org) ([builds 8.1.0](http://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/installer/mingw-w64-install.exe/download))
- [CMake 3.9](https://cmake.org) (or higher)

During CMake installation, make sure you select to add `cmake` to the `PATH` (at least for the current user).
During MinGW installation, make sure you select "posix" threads (should be the default) and not "win32" threads. After installing MinGW, you need to add its path to the environment variable `PATH`. The default path should be:
```
C:\Program Files (x86)\mingw-w64\i686-8.1.0-posix-dwarf-rt_v6-rev0\mingw32\bin
```
but the actual path may vary depending on your installation.

### Linux

Pre-requisites:
- Xorg-dev package (X11)
- G++ 9 (or higher)
- CMake 3.9 (or higher)

To install these packages in Ubuntu, type the following command:
```
sudo apt-get install xorg-dev g++ cmake
```
In Fedora, the `xorg-dev` package is not available. Instead, install the packages:
```
libX11-devel libXinerama-devel.x86_6 libXcursor-devel.x86_64 libXi-devel.x86_64 libXrandr-devel.x86_64 mesa-libGL-devel.x86_64
```

### MacOS

Pre-requisites:
- Xcode Command Line Tools
- CMake 3.9 (or higher)

To install them, assuming you have the [brew](https://brew.sh) package manager, type the following commands:
```
xcode-select --install
brew install cmake
```

### Virtual Machines

If you use a VM with a graphical interface, refer to the section for the operating system installed on it.

**Warning:** the graphical simulations are based on OpenGL, and common Virtual Machine software (e.g., VirtualBox) has faulty support for OpenGL. If you rely on a virtual machine for graphical simulations, it might work provided that you select hardware virtualization (as opposed to software virtualization). However, it is recommended to use the native OS whenever possible.


## Execution

In order to execute the experiments, type the following command in a terminal:
```
> ./make.sh gui run -O all
```
Running the above command, you should see output about building the executables then the graphical simulation should pop up while the console will show the most recent `stdout` and `stderr` outputs of the application, together with resource usage statistics (both on RAM and CPU).  During the execution, log files will be generated in the `output/` repository sub-folder. A batch of multiple simulations will also be launched, for which individual simulation results will be logged in the `output/raw/` subdirectory, with the overall resume in the `output/` directory. Plots will be produced in the `plot/` repository sub-folder. If you only want to execute one of the targets, you can use one of the following commands:
```
> ./make.sh run -O batch
> ./make.sh gui run -O graphic
> ./make.sh run -O parameter
```

### Graphical User Interface

The graphical simulation will open a window displaying the simulation scenario, initially still: you can start running the simulation by pressing `P` (current simulated time is displayed in the bottom-left corner). While the simulation is running, network statistics will be periodically printed in the console. You can interact with the simulation through the following keys:

- `Esc` to end the simulation
- `P` to stop/resume
- `O`/`I` to speed-up/slow-down simulated time
- `L` to show/hide connection links between nodes
- `G` to show/hide the grid on the reference plane and node pins
- `M` enables/disables the marker for selecting nodes
- `left-click` on a selected node to open a window with node details
- `C` resets the camera to the starting position
- `Q`,`W`,`E`,`A`,`S`,`D` to move the simulation area along orthogonal axes
- `right-click`+`mouse drag` to rotate the camera
- `mouse scroll` for zooming in and out
- `left-shift` added to the camera commands above for precision control
- any other key will show/hide a legenda displaying this list

Hovering on a node will also display its UID in the top-left corner.


## Project Inspection

This project consists of several source files:

- **lib/election_compare.hpp**. This contains the C++ code (using the FCPP library) of the leader election algoritms that are run in the simulations.
- **lib/simulation_setup.hpp**. This contains the simulation setup of the simulations.
- **lib/frac.hpp**, **lib/func.hpp**, **lib/max_deque.hpp**, **lib/sq2.hpp**. These contain helper classes for the parameter optimisation.
- **run/batch.hpp**. This contains the launcher of batch simulations.
- **run/graphic.hpp**. This contains the launcher of graphical simulations.
- **run/parameter.hpp**. This contains the parameter optimisation code.
