# Behaviour Tree PiCar-V

Full-Stack Application used by users to control a SunFounder Pi-Car.

## Description

The application contains the following inside the `app` directory: 

- **rpi**
  - Allows the Raspberry Pi to communicate with the **backend** and handle messages from it.
  - There are three sub-projects within this project, each handling different aspects of the Raspberry Pi.

- **admin_panel**
  - Main user interface for directly controlling the **rpi**.

- **other**
  - Contains multiple sub-projects that support the main project, **rpi**.
  - The code for handling the **behaviour_tree** is contained in the `behaviour_tree` and `behaviour_tree_car` projects.
  - The code that controls the RPLidar is also here, in the `rplidar` directory. It was ported from Python with the help of ChatGPT.

For detailed API documentation, please refer to the [API Documentation](API.md).

## Getting Started

### Dependencies

* [XMake](https://xmake.io/#/guide/installation) - Build system for C++
* [Node.js](https://nodejs.org/en) - Used to build the frontend
* [pnpm](https://pnpm.io/installation) - Used to cache the dependencies from npm
* [Electron](https://www.electronjs.org/) - Used to bundle HTML and JS files into a desktop app

### Executing Program

To build the **backend** / **rpi** folder, run the following command:

```sh
xmake
```

This will automatically install all the dependencies from the folder and build the executable file.

To build the **admin_panel**, view the README.md in its directory for instructions.

## Authors

[@Chi-EEE](https://github.com/Chi-EEE)

## License

This project is licensed under the MIT License - see the LICENSE.md file for details.

## Acknowledgments

[README Template](https://gist.github.com/DomPizzie/7a5ff55ffa9081f2de27c315f5018afc)