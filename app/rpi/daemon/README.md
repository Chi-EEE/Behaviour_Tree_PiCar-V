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
`journalctl -u rpi_daemon`

## Clear Past Logs
`sudo journalctl -m --vacuum-time=1s`

## Reload Daemon
`sudo systemctl reload rpi_daemon`

## Change Config
`sudo nano /etc/rpi_daemon/rpi_daemon.conf`

## Uninstall Raspberry Pi Daemon
`xmake uninstall --admin rpi_daemon`

## Speedrun
`sudo systemctl stop rpi_daemon && xmake && xmake install --admin rpi_daemon && sudo systemctl daemon-reload && sudo systemctl start rpi_daemon`