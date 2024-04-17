# Car Application

Full-Stack Application used by Users to control a SunFounder Pi-Car

## Description

The application contains the following inside the `app` directory: 

- **rpi**
    - Allows the raspberry pi to communicate to the **backend** and handle messages from it. 

    - There are 3 sub-projects in this project which handle different aspects of the raspberry pi.

- **admin_panel**
    - Main user interface for directly controlling **rpi**.

- **other**
    - Many sub-projects which support the main project: **rpi**.

    - The code to handle the **behaviour_tree** are contained in the `behaviour_tree` & `behaviour_tree_car` projects.

    - The code which controls the RPLidar is here aswell in the `rplidar` directory, it was ported from Python with the help of ChatGPT.

## Getting Started

### Dependencies

* [XMake](https://xmake.io/#/guide/installation) - Buildsystem for C++
* [Node.js](https://nodejs.org/en) - Used to build the frontend
* [pnpm](https://pnpm.io/installation) - Used to cache the dependencies from npm
* [Electron](https://www.electronjs.org/) - Used to bundle html and js files into a desktop app

### Executing program

To build the **backend** / **rpi** folder, you would have to run the following command:
```
xmake
```
This will automatically install all the dependencies from the folder and build the executable file.

If you want to build the **admin_panel**, view the README.md in it's directory for instructions.

## Authors

[@Chi-EEE](https://github.com/Chi-EEE)

## License

This project is licensed under the MIT License - see the LICENSE.md file for details

## Acknowledgments

[README Template](https://gist.github.com/DomPizzie/7a5ff55ffa9081f2de27c315f5018afc)