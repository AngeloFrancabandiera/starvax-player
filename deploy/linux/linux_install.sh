#!/bin/sh

sudo cp 90-Velleman.rules  /etc/udev/rules.d/
sudo cp starvaxplayer.desktop  /usr/share/applications/

sudo udevadm trigger
