# Digisim - Digital Logic Simulator

## Overview

**Digisim** is a digital logic simulator developed using C++ and Gtkmm 4. It is designed for educational purposes, allowing users to design, simulate, and manage digital circuits. The application supports the creation and reuse of custom chips, facilitating the construction of complex circuits.

## Website
***Find Installer Here [Digisim](https://ev-od.github.io/DigiSem-WebSite/)***

## Features

- **User-Friendly GUI**: Built with Gtkmm 4, providing an intuitive and visually appealing interface.
- **Custom Chip Creation**: Users can create and save custom chips, which can be reused in other designs.
- **Real-Time Simulation**: Immediate feedback on circuit behavior without needing to run simulations manually.
- **Project Management**: Easily save and load projects, allowing for efficient organization of circuit designs.

## Installation

### Prerequisites

- **Gtkmm 4**: Ensure Gtkmm 4 is installed on your system. You can follow the [official installation guide](https://developer.gnome.org/gtkmm-tutorial/stable/).

### Steps

1. **Clone the Repository**

   ```bash
   git clone https://github.com/EV-OD/Digital-Logic.git
   cd Digital-Logic
   ```

2. **Build and Install**

    + For Unix-like systems (Linux, macOS):


        ```bash
        mkdir build
        cd build
        cmake ..
        make
        sudo make install
    + For Windows:

        Follow the instructions provided in the INSTALL.md file in the repository for Windows-specific build steps.
# Usage
## Creating Circuits
1. **Start a New Project**: Use the main menu to create a new project.
2. **Design Your Circuit**: Drag and drop components into the creation area.
3. **Simulate: Real-time** simulation provides immediate feedback.
## Custom Chips
1. **Create Custom Chips**: Design circuits and save them as custom chips.
2. **Reuse Chips**: Load saved chips into new projects to build more complex designs.


# Limitations and Future Work
1. **Editing Custom Chips**: Currently, editing existing custom chips is limited.
2. **Folder Management**: The application does not support deleting or renaming folders.
3. **Pin Naming**: Pins cannot be named, which can make circuit identification challenging.
4. **Sequential Circuits**: Digisim does not currently support the design and simulation of sequential circuits.


## Contributing
Contributions to the Digital Logic Simulator are welcome! If you would like to contribute, please follow these guidelines:

1. Fork the repository and create a new branch for your feature or bug fix.
2. Make your changes and ensure that the code passes all tests.
3. Submit a pull request with a clear description of your changes.

## License
The Digital Logic Simulator is licensed under the [MIT License](https://opensource.org/license/mit).