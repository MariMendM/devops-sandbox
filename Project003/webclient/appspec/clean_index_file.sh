#!/bin/bash

# Necessary due to the default index.html created by apache; CodeDeploy complains that "file already exists" if not removed in the first execution!
rm -f -r /var/www/html/*
