# Project005

## Content

* [Demo description](#demo-description)
* [Repo files overview](#repo-files-overview)
* [Preparing Environment](#preparing-environment)
  * [Create AWS CloudFormation stack](#create-aws-cloudformation-stack)
  * [Run playbooks using AWS Systems Manager](#run-playbooks-using-aws-systems-manager)
* [How to use](#how-to-use)
* [Next steps](#next-steps)

## Demo description

Demonstrate Ansible's playbooks on AWS Cloud. The CloudFormation deploys a minimal stack containing EC2 Managed Instances. Steps are provided to use AWS Systems Manager 'Run Command' with command 'AWS-RunAnsiblePlaybook' in order to run playbooks on  instances.

```diff
# Reminder: all diagrams included in documentation are Draw.io's editable layered PNGs.
```

## Repo files overview

* File **[cloudformation.yml](cloudformation.yml)**:
  * creates a VPC
    * 1 subnet (public);
    * 1 route table (for public subnet);
    * 1 internet gateway (route in public route table);
    * 1 network NACL (for public traffic)
      * HTTP/HTTPS/Ephemeral allowed for In/Outbound to CIDR 0.0.0.0/0
      * SSH allowed for In/Outbound to CIDR provided for maintenance
  * creates security group for instances
    * HTTP/HTTPS allowed for Inbound to CIDR 0.0.0.0/0
    * SSH allowed for Inbound to CIDR provided for maintenance
  * creates IAM role and Instance Profile for EC2 istances
    * allowing SSM to manage instances
  * creates EC2 instances
    * t2.micro;
    * ubuntu 20.04;
    * Ansible installed by cloud init
  * <details><summary>see CloudFormation diagram</summary><img src="documents/cloudformation-diagram.png"></details>
* Files **playbook\*.yml**:
  * Samples of Ansible's playbook
  * NOTE: take care with names of plays and tasks; to run a playbook with AWS Systems Manager Run Command, they cannot contain some chars that Ansible usually allows, such as \(\) or \-
    * [playbook01](playbook01.yml): install and configure Apache service
      * 1 play, tasks including 'apt' and 'service'
    * [playbook02](playbook02.yml): sparse checkout of [Project001/website](https://github.com/MariMendM/devops-sandbox/tree/master/Project001/website) and copy of content to Apache's www folder
      * requires to run playbook01 for Apache install
      * 2 plays, tasks including 'line', 'shell', 'lineinfile' and 'copy'
      * variables reused between plays of same host using 'set_fact'
    * [playbook03](playbook03.yml): collect host names from managed nodes
      * 1 play, tasks 'command' and 'shell'

## Preparing environment

### Create AWS CloudFormation stack

In AWSCloudFormation console, create stack using cloudformation.yml file. Parameters:
* General Configuration
  * Environment Name: the name to be used for tagging resources created by stack
* Network Configuration
  * VPC IP range: CIDR block for VPC created by stack (cannot be already in use)
  * Public Subnet VPC IP range: CIDR block for public subnet; must be in accordance to VPC's CIDR block
* EC2 Configuration
  * KeyPair for EC2 instances: select an already existent key-pair
  * Ip4ServerConnection: IP or CIDR block from machines that can SSH EC2 public instances

## How to use

### Run playbooks using AWS Systems Manager

1. Check if [instances were registered](https://github.com/MariMendM/devops-sandbox/wiki/AwsSystemsManager#FleetManager) in Systems Manager's fleet;
1. [Run command](https://github.com/MariMendM/devops-sandbox/wiki/AwsSystemsManager#RunCommand) in Systems Manager:
   * In 'Command document', choose the option 'AWS-RunAnsiblePlaybook';
   * In 'Command parameters', paste content of one of playbook.yml samples or input corresponding GitHub's URL; leave other options in their defaults;
   * In 'Targets', specify instances tags with key equal to 'Name' and value 'p005-ec2' (where p005 will be replaced by whatever 'Environment Name' was input in CloudFormation stack);
1. Follow up the progress of the command. When all instances complete, check results according to what is expected for each Ansible's playbook sample:
   * playbook01.yml: check Apache's installation/start opening IP addresses <sup>:bulb:</sup> of the instances;
   * playbook02.yml: check website opening IP addresses <sup>:bulb:</sup> of the instances;
   * playbook03.yml: check host names of instances in AWS Systems Manager Run Command console, more specifically under 'Targets and Outputs', clicking each instance ID in which command has run;

:bulb: IP addresses of the instances can be seen in CloudFormation's output console of the stack, under 'Public IP for EC2 instances'.

## Next steps

* Add some more playbook samples.
