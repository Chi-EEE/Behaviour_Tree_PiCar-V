#!/bin/bash

# Made by ChatGPT

conf_install_dir="etc/rpi_daemon"
install_files=(
    "rpi_daemon usr/bin"
    "systemd/rpi_daemon.service etc/systemd/system"
    "systemd/rpi_daemon.conf $conf_install_dir"
)

on_install() {
    target=$1
    for file_pair in "${install_files[@]}"; do
        src=$(echo "$file_pair" | cut -d' ' -f1)
        dest=$(echo "$file_pair" | cut -d' ' -f2)
        if [[ $dest == "usr/bin" ]]; then
            sudo cp "$src" "/$dest/"
            sudo chmod +x "/$dest/$(basename "$src")"
        else
            sudo mkdir -p "/$dest/"
            sudo cp "$src" "/$dest/"
        fi
        echo "installing $src to /$dest .."
    done
}

on_uninstall() {
    target=$1
    for file_pair in "${install_files[@]}"; do
        src=$(echo "$file_pair" | cut -d' ' -f1)
        dest=$(echo "$file_pair" | cut -d' ' -f2)
        sudo rm -f "/$dest/$(basename "$src")"
        echo "uninstalling $(basename "$src") from /$dest .."
    done
    sudo rm -rf "/$conf_install_dir"
    echo "removing directory /$conf_install_dir .."
}

if [[ $1 == "install" ]]; then
    on_install
elif [[ $1 == "uninstall" ]]; then
    on_uninstall
else
    echo "Usage: $0 {install|uninstall}"
    exit 1
fi
