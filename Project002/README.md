# Project002

## Demo description

Demonstrate Ansible's playbook with simple tasks.

## Repo files overview

* File [cloudformation.yml](cloudformation.yml):
  * creates simple VPC
    * 1 subnet (public);
    * 1 route table (for public subnet);
    * 1 internet gateway (route in public route table);
    * 1 network NACL (for public traffic)
      * HTTP/HTTPS/SSH/Ephemeral for In/Outbound
  * creates security group for instance
    * HTTP/HTTPS/SSH for Inbound
  * creates 3 EC2 instances
    * t2.micro;
    * ubuntu 20.04;
    * Ansible installed by cloud init
   <details><summary>corresponding diagram</summary><img src="documents/cloudformation-diagram.png"></details>

## Preparing environment

### Create AWS CloudFormation stack

Create stack using cloudformation.yml
* EC2 instances will already have Ansible installed

### Configure Ansible controller/nodes (TO-DO)

1. Connect to server by SSH...
1. Run...
