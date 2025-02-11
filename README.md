# Space Invaders

This project is an implementation of the classic Space Invaders game for the MSP430F5529 microcontroller.

## Project Structure

. ├── .ccsproject ├── .cproject ├── .gitignore ├── .project ├── .settings/ │ ├── org.eclipse.cdt.codan.core.prefs │ ├── org.eclipse.cdt.debug.core.prefs │ ├── org.eclipse.core.resources.prefs ├── .vscode/ │ ├── c_cpp_properties.json │ ├── launch.json │ ├── settings.json ├── Debug/ │ ├── ccsObjs.opt │ ├── ece2049_demo_ccs7_smj_linkInfo.xml │ ├── ece2049_demo_ccs7_smj.map │ └── ... ├── driverlibHeaders.h ├── fonts/ ├── grlib/ ├── images/ ├── LcdDriver/ ├── lib/ ├── lnk_msp430f5529.cmd ├── macros.ini_initial ├── main.c ├── MSP430F5529.ccxml ├── peripherals.c ├── peripherals.h ├── README.md ├── targetConfigs/ ├── version_history.txt

## Files and Directories

- **main.c**: Contains the main logic of the Space Invaders game.
- **peripherals.c** and **peripherals.h**: Contains functions and definitions for handling peripherals.
- **lnk_msp430f5529.cmd**: Linker command file for the MSP430F5529 microcontroller.
- **driverlibHeaders.h**: Header file for driver library.
- **fonts/**, **grlib/**, **images/**, **LcdDriver/**, **lib/**: Directories containing additional resources and libraries.
- **.vscode/**: Configuration files for Visual Studio Code.
- **.settings/**: Configuration files for the Eclipse IDE.
- **Debug/**: Directory containing debug-related files.
- **targetConfigs/**: Directory containing target configuration files.
- **version_history.txt**: File containing the version history of the project.

## Getting Started

1. Clone the repository to your local machine.
2. Open the project in your preferred IDE (Eclipse or Visual Studio Code).
3. Build the project.
4. Flash the compiled binary to the MSP430F5529 microcontroller.
5. Run the game and enjoy!

## License

This project is licensed under the terms of the MIT license.
