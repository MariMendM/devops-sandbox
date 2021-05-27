# Web client page

## Description

Simple webpage intended to provide a "visualization" of AWS infrastructure implemented in this demo. It actually only displays AWS EC2 meta-data from instances hosting the demo.

It is a simplified version of [Project003/webclient](https://github.com/MariMendM/devops-sandbox/tree/master/Project003/webclient), where the CloudFormation file provisions 2 EC2 public instances (the ones hosting the webclient) and 2 EC2 private instances (the ones hosting the serverapp). In this case, only public instances are available attached to a Load Balancer, with no serverapp deployed. Refreshing this page will then trigger public Load Balancer coordinating all public instances, displaying the instance-id of the host designated by the LB.

## Repo files overview

* Folder /resources:
  * Folder /css:
    * CSS style files applied to HTML of index.php
  * Folder /images:
    * Images referenced by HTML of index.php
* File [get-aws-frontend-information.php](get-aws-frontend-information.php):
  * Script to send GET request to AWS meta-data instance-id, using URL hardcoded inside script (AWS metadata URL is constant regardless instance requesting information from it)
* File [index.html](index.html):
  * Main webclient page (HTML and JavaScript code)
* <details><summary>Modules organization diagram for webpage</summary><img src="documents/modules-organization-diagram.png"></details>

## How to deploy

### Install Apache and PHP

This webclient only works at AWS EC2 instances, provided the use of instance-id as center of this demo. For that, while following commands (considering debian-based distros) can be used to manually install Apache and PHP, this task will be actually performed by CloudInit of EC2 instances inside CloudFormation. For reference:

```
$ sudo apt install php -y
```
```
$ #Apache...
$ sudo apt install apache2 -y
$ #... adding user 'ubuntu' to apache group
$ sudo usermod -a -G apache ubuntu
$ #... ownership and permissions
$ sudo chown -R ubuntu:apache /var/www
$ sudo chmod 2775 /var/www
$ #... Apache on boot
$ sudo systemctl enable apache2
$ #... starting
$ sudo systemctl start apache2
```

### Copy application to Apache's www folder

Simply copying (or cloning) webclient folder content to Apache's /var/www/html folder. Also automatically performed by CloudFormation.

## How to open

Open webpage http://\<DNS--load-balancer\>/index.html:
* Refresh page to exercise Load Balancer (each time load is targeted in a different public EC2 host, the corresponding EC2 instance-id is displayed).
