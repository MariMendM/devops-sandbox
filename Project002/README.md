# Project002

## Content

* [Demo description](#demo-description)
* [Repo files overview](#repo-files-overview)
* [Preparing Environment](#preparing-environment)
  * [Configure controller host to run entry point playbook](#configure-controller-host-to-run-entry-point-playbook)
  * [Create a key-pair](#create-a-key-pair)
  * [Running the provision playbook](#running-the-provision-playbook)
* [How to use](#how-to-use)

## Demo description

Demonstrate Ansible's automated installation/configuration over AWS Cloud. From a host with Ansible installed, a playbook provisions AWS CloudFormation stack, creates both in-memory and file inventories containing the EC2 instances created, and then waits instances be ready to use. After that, the EC2 instances can be used as managed hosts for any other Ansible playbook desired (a testing playbook is provided).

```diff
# Reminder: all diagrams included in documentation are Draw.io's editable layered PNGs.
```

## Repo files overview

* File [cloudformation.yml](cloudformation.yml):
  * Used by 'playbook-provision-environment.yml' to provision AWS stack containing:
    * a VPC
      * 1 subnet (public)
      * 1 route table (for public subnet)
      * 1 internet gateway (route in public route table)
      * 1 network NACL (for public traffic)
        * HTTP/HTTPS/Ephemeral allowed for In/Outbound to CIDR 0.0.0.0/0
		* ICMP allowed for In/Outbound to IP provided as host controller (required by Ansible's playbook)
        * SSH allowed for Inbound to IP provided as host controller (required by Ansible's playbook)
    * security group for instances
      * HTTP/HTTPS allowed for Inbound to CIDR 0.0.0.0/0
      * SSH/ICMP allowed for Inbound to IP provided as host controller (both required by Ansible's playbook)
    * EC2 instances
      * t2.micro
      * ubuntu 20.04
      * Ansible installed by cloud init
  * <details><summary>see CloudFormation diagram</summary><img src="documents/cloudformation-diagram.png"></details>
* File [requirements.yml](requirements.yml):
  * Collections from Ansible Galaxy repository that shall be installed for this demo
* File [playbook-provision-environment.yml](playbook-provision-environment.yml):
  * Entry point of this demo, it automates the provisioning of the AWS Cloud environment and configuration of Ansible managed nodes
  * Contains 4 plays
    * 1 to provision CloudFormation template
    * 1 to create in-memory host inventory, allowing to extended this playbook with plays using this inventory
    * 1 to create host inventory in disk, allowing to run different playbooks
    * 1 to wait EC2 managed nodes to respond (requires ping and SSH allowed from controller host to managed instances)
	* 1 to test in-memory inventory created
* File [playbook-provision-environment.json](playbook-provision-environment.json):
  * Used as variable template for 'playbook-provision-environment.yml'
* File [playbook-testing.yml](playbook-testing.yml):
  * Demo playbook that can be used to test Ansible's inventory file created

## Preparing environment

### Configure controller host to run entry point playbook

The entry point of this demo is the 'playbook-provision-environment.yml' file. To run it, it is necessary to install:

1. Python and pip (some linux systems already have them installed by default):
   ```
   sudo apt install python3 python3-pip -y
   ```
1. Python's boto3:
   ```
   pip install boto3
   ```
1. Ansible:
   ```
   sudo apt-add-repository --yes --update ppa:ansible/ansible
   sudo apt install ansible -y
   ```
1. Required Ansible's collections from Ansible Galaxy repository, using provided 'requirements.yml':
   ```
   ansible-galaxy collection install -r requirements.yml
   ```
   or install them manually:
   ```
   ansible-galaxy collection install amazon.aws community.aws
   ```

### Create a key-pair

When provisioning the CloudFormation template, a key pair for EC2 instances must be provided beforehand. To create one, it is possible to use the AWS EC2 console and download the .pem file, or to [install](https://docs.aws.amazon.com/cli/latest/userguide/install-cliv2-linux.html#cliv2-linux-install) and use the AWS CLI, as follows:
```
aws ec2 create-key-pair --key-name p002keypair --query 'KeyMaterial' --output text > p002keypair.pem
```
The above command is creating a .pem file named 'p002keypair' which is the value predefined in the playbook that provisions the CloudStack. If changed, the respective variable has to be changed too, as explained in next section.

It is also required to close the permissions for the key file generated, otherwise the message "permissions ... are too open" is raised and the key is ignored, preventing SSH connections between controller host and managed hosts:
```
chmod 0400 p002keypair.pem
```
If permissions are not changed, the 'playbook-provision-environment.yml' will certainly fail in the play that waits availability of managed hosts.

### Running the provision playbook

First step is to deploy the repository files into a folder in the host previously configured (to [sparse checkout](https://github.com/MariMendM/devops-sandbox/wiki/Git#GitSparseCheckout) this folder or download its files). To run the 'playbook-provision-environment.yml', some variables have to be defined, namely:
* **aws_cfn_environment_name**:
  * the name to be used for tagging resources created by CloudFormation stack, provisioned by Ansible playbook
  * predefined as 'p002'
* **aws_cfn_vpc_cidr**:
  * CIDR block for VPC created by stack (cannot be already in use inside AWS VPC Service)
  * predefined as '10.10.0.0/16'
* **aws_cfn_pub_subnet1_cidr**:
  * CIDR block for public subnet; must be in accordance to VPC's CIDR block
  * predefined as '10.10.10.0/24'
* **aws_cfn_ip_for_server_connection**:
  * IP from controller host previously configured, to allow SSH and ping connections
  * predefined as '0.0.0.0/0'
* **aws_cfn_ec2_keypair**:
  * the name of the key-pair previously created
  * predefined as 'p002keypair'

All variables have predefined values hard-coded inside 'playbook-provision-environment.yml' file. To use them as predefined, simply run the playbook using the command:
```
ansible-playbook playbook-provision-environment.yml --private-key p002keypair.pem
```

If any of the values is going to be modified, two options are available:
1. Pass the variables by command line when running the playbook, overwriting the values hard-coded inside playbook;  
Best choice when changing only few variables;  
Example: changing only hard-coded AWS US East N.Virginia region 'us-east-1' to AWS South America Sao Paulo 'sa-east-1':
   ```
   ansible-playbook playbook-provision-environment.yml --private-key p002keypair.pem --extra-vars "aws_region=sa-east-1"
   ```
1. Change provided 'playbook-provision-environment.json' as desired and use it to run the playbook;  
Best choice when changing most part of the variables:
   ```
   ansible-playbook playbook-provision-environment.yml --private-key p002keypair.pem --extra-vars "@playbook-provision-environment.json"
   ```

## How to use

The 'playbook-provision-environment.yml' execution have already tested the in-memory inventory created. To check the results, see the output from play named "Test in-memory inventory collecting hostnames", where the hostname of all EC2 instances provisioned by the CloudFormation stack shall be displayed. Any other tasks to be performed over in-memory inventory can be added in the end of this playbook.

To test the inventory file created by play named "Add ec2 instances to Ansible's inventory \/etc\/ansible\/aws", it is possible to simply display the hosts included in the file:
```
ansible -i /etc/ansible/aws_<env_name> --list-hosts all
```
where \<env_name\> must be replaced by the value provided for variable 'aws_cfn_environment_name' (if kept as predefined, the file name is /etc/ansible/aws_p002).

But to go further on testing, it is also possible to run any other playbook desired. A 'playbook-testing.yml' is provided, collecting hostname and installing apache in all hosts. To run it:
```
ansible-playbook -i /etc/ansible/aws_<env_name> playbook-testing.yml --private-key p002keypair.pem
```
