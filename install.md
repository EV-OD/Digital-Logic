# Installation Guide for Digisim on Windows

## Prerequisites

Before installing Digisim, ensure that the following software is installed on your Windows system:

- **Visual Studio** (preferably the latest version)
  - Make sure to include the "Desktop development with C++" workload during installation.
- **CMake** (version 3.10 or later)
- **GTKmm 4** (required for the GUI)
  - Ensure that the GTKmm 4 development environment is set up and the necessary environment variables are configured.

## Building and Installing Digisim

### 1. Clone the Repository

First, clone the Digisim repository to your local machine:

```bash
   git clone https://github.com/EV-OD/Digital-Logic.git
   cd Digital-Logic
```

### 2. Configure and Build the Project with CMake

1. Open a command prompt and navigate to the digisim directory.

2. Create a build directory and navigate into it:

```bash
    mkdir build
    cd build
```

3. Run CMake to configure the project:

    ```bash
    cmake ..
    ```

    Make sure that CMake detects the correct Visual Studio version and GTKmm paths.

4. Build the project:

    ```bash
    cmake --build .
    ```

3. Install Digisim

    To install Digisim, you can copy the executable and its dependencies to your desired location. Alternatively, you can create an installer using CMake or other packaging tools if required.
