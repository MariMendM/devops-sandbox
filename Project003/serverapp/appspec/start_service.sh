#!/bin/bash

echo Starting...
chmod +x /home/ubuntu/serverapp/restapi.app

#Runs app in background (nohup keep running after the shell session ends)
#Output redirected to /dev/null to prevent creation of nohup.out file
nohup /home/ubuntu/serverapp/restapi.app 8080 &>/dev/null &
