# Raspberry Pi Daemon

This will only work for Linux.

## Install Raspberry Pi Daemon
`xmake install --admin rpi_daemon`

## Enable / Disable Daemon to run on startup
`sudo systemctl [enable|disable] rpi_daemon`

## Start / Stop / Restart Daemon
`sudo systemctl [start/stop/restart] rpi_daemon`

## View Logs
`systemctl status rpi_daemon.service`

## Start / Stop / Restart Daemon
`sudo systemctl [start/stop/restart] rpi_daemon`

## Uninstall Raspberry Pi Daemon
`xmake uninstall --admin rpi_daemon`
