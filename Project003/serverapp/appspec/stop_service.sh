#!/bin/bash

echo Stopping...
rm -f -r /home/ubuntu/serverapp/*

#Kill process already running (nothing to kill will fail command, and || will execute exit 0)
sudo kill $(ps -ef | pgrep -x restapi.app) || exit 0



