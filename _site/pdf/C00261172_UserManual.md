# User Manual for the Behaviour Tree PiCar-V

## Admin Panel

### Installation

To install the Admin Panel, you would have to run the `.exe` provided in the Final Iteration Release.

### Running the Websocket server

In the Admin Panel, there is a tab called "Manage Websockets", click it and start the Websocket Server.


## Raspberry Pi

Before beginning this, do the steps for the Admin Panel first.

### Configurating the Apps

If you are connected on Wifi with the Raspberry Pi, simply copy the Wifi IP address on the Admin Panel in the "Manage Websockets" tab.  
Otherwise, If you are connected via ethernet, then copy the ethernet IP address and so on.

Afterwards, paste in the IP address into the host fields both in the `settings/config.jsonc` and `rpi_daemon.conf`

### Installing the Daemon

In order to install the Daemon application, follow the steps outlined in the README.md of the daemon's directory.

### Running the TUI application

To run the TUI application, you first have to grant permission to the executable:

`chmod +x ./rpi_tui`

This grants the TUI application permission to be run as an executable.

Afterwards simply run:

`sudo ./rpi_tui`

The reason why it is sudo is because the controllers for the Sunfounder PiCar-V require admin access.

Afterwards, simply select the Raspberry Pi UUID on the Admin Panel and watch as it displays the Lidar 3d projection and Camera.