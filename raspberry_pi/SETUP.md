# First Steps
Install Ubuntu latest version on raspberry pi

## Wifi

To add wifi:
```
sudo nano /etc/netplan/50-cloud-init.yaml
```

Now add:
```
wifis:
    wlan0:
        optional: true
        access-points:
            "SSID":
                password: "password"
        dhcp4: true
```
Run the following to debug the wifi file:
```
sudo netplan -debug generate
```

Run `sudo reboot`

## SSH

Run:
```
sudo apt update
sudo apt install openssh-server
```

To check ssh:
```
sudo systemctl status ssh
```

To allow:
```
sudo ufw allow ssh
```