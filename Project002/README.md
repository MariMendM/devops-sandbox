# Project002

## Content

* [Demo description](#demo-description)
* [Repo files overview](#repo-files-overview)
* [Preparing Environment](#preparing-environment)
  * [Create AWS CloudFormation stack](#create-aws-cloudformation-stack)
* [Next steps](#next-steps)

## Demo description

Demonstrate Ansible's playbook with simple tasks.

```diff # Reminder: all diagrams included in documentation are Draw.io's editable layered PNGs.```

## Repo files overview

* File [cloudformation.yml](cloudformation.yml):
  * File [cloudformation.yml](cloudformation.yml):
  * creates very (very!) simple VPC
    * 1 subnet (public);
    * 1 route table (for public subnet);
    * 1 internet gateway (route in public route table);
    * 1 network NACL (for public traffic)
      * HTTP/HTTPS/Ephemeral allowed for In/Outbound to CIDR 0.0.0.0/0
	  * SSH allowed for In/Outbound to CIDR provided for maintenance
  * creates security group for instance
    * HTTP/HTTPS allowed for Inbound to CIDR 0.0.0.0/0
	* SSH allowed for Inbound to CIDR provided for maintenance
  * creates an EC2 instance
    * t2.micro;
    * ubuntu 20.04;
    * Ansible installed by cloud init
  * <details><summary>see corresponding diagram</summary><img src="documents/cloudformation-diagram.png"></details>

## Preparing environment

### Create AWS CloudFormation stack

Create stack using cloudformation.yml\* file. Parameters:
* General Configuration
  * Environment Name: the name to be used for tagging resources created by stack
* Network Configuration
  * VPC IP range: CIDR block for VPC created by stack (cannot be already in use)
  * Public Subnet VPC IP range: CIDR block for public subnet; must be in accordance to VPC's CIDR block
* EC2 Configuration
  * KeyPair for EC2 instances: select an already existent key-pair
  * Ip4ServerConnection: IP or CIDR block from machines that can SSH EC2 public instances

## Next steps

* Add section "Configure Ansible controller/nodes" inside "Preparing Environment"
