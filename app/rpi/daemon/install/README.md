# Sunfounder PiCar Daemon

## Installation:

Run the follow commands to install the Daemon:
```bash
sudo chmod +x ./install_script # Grant permission to install_script to run as exe
systemctl stop rpi_daemon # Stop any previous rpi_daemon
sudo ./install_script install # Install the Daemon
sudo systemctl daemon-reload # Reload the Daemon if there was any previous
sudo systemctl start rpi_daemon # Start the new Daemon
```

Or use the single command:
```bash
sudo chmod +x ./install_script && systemctl stop rpi_daemon && sudo ./install_script install && sudo systemctl daemon-reload && sudo systemctl start rpi_daemon
```