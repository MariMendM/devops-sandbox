# Project006

## Content

* [Demo description](#demo-description)
* [Repo files overview](#repo-files-overview)
* [Preparing Environment](#preparing-environment)
  * [Creating AWS CloudFormation stack](#create-aws-cloudformation-stack)
  * [Opening site](#open-site)
* [How to use](#how-to-use)
  * [Understanding instance availability time](#understanding-instance-availability-time)
    * [Auto Scaling and ELB health checks explained](#auto-scaling-and-elb-health-checks-explained)
  * [Updating Auto Scaling desired configuration](#updating-auto-scaling-desired-configuration)
  * [Checking Auto Scaling configuration taking effect](#checking-auto-scaling-configuration-taking-effect)
* [Next steps](#next-steps)

## Demo description

Demonstrate Ansible's playbook deploying a web page on AWS Cloud with Auto Scaling and Elastic Load Balancer.

The CloudFormation deploys a stack containing an EC2 Template for Auto Scaling, that is associated to an ELB. Systems Manager's State Manager is used to run a command of type 'AWS-ApplyAnsiblePlaybooks' during EC2 instances provisioning by Auto Scaling. The ELB takes part with Health Checks for EC2, controlling traffic and registering/deregistering instances according to Auto Scaling actions.

The ELB's DNS address can be opened to check web page's deployment, displaying IDs of healthy instances provisioned by Auto Scaling.

```diff
# Reminder: all diagrams included in documentation are Draw.io's editable layered PNGs.
```

## Repo files overview

* Folder **website**:
  * simple webpage intended to provide a "visualization" of AWS infrastructure implemented in this demo. It actually only displays AWS EC2 meta-data from instances hosting the demo (documentation [here](website/README.md))
  * <details><summary>see main diagram</summary><img src="website/documents/modules-organization-diagram.png"></details>
* File **[cloudformation.yml](cloudformation.yml)**:
  * creates a VPC
    * 1 subnet (public);
    * 1 route table (for public subnet);
    * 1 internet gateway (route in public route table);
    * 1 network NACL (for public traffic)
      * HTTP/HTTPS/Ephemeral allowed for In/Outbound to CIDR 0.0.0.0/0
      * SSH allowed for In/Outbound to CIDR provided for maintenance
  * creates a security group for instances
    * HTTP/HTTPS allowed for Inbound to CIDR 0.0.0.0/0
    * SSH allowed for Inbound to CIDR provided for maintenance
  * creates security groups
    * 1 for EC2 public instances
      * HTTP/HTTPS allowed for Inbound to CIDR 0.0.0.0/0
      * SSH allowed for Inbound to CIDR provided for maintenance
    * 1 for ELB of public instances
      * HTTP 80 allowed for Inbound to CIDR 0.0.0.0/0
  * creates a IAM role and a Instance Profile for EC2 instances
    * allowing SSM to manage instances
    * allowing Auto Scaling Lifecycle Hook completion
  * creates an EC2 template to launch instances
    * t2.micro;
    * ubuntu 20.04;
    * CLI installed by cloud init
  * creates an application ELB, internet-facing, with listener HTTP at port 80 targeting public instances
  * creates an Auto Scaling group, using EC2 template to launch instances into ELB
  * <details><summary>see CloudFormation diagram</summary><img src="documents/cloudformation-diagram.png"></details>
* Files **[playbook.yml](playbook.yml)**:
  * Playbook to install Apache and PHP, and to sparse checkout and deploy [Project006/website](Project006/website)
    * 2 plays, tasks including 'apt', 'service', 'file, 'linefile', 'shell' and 'copy';
  * NOTE: take care with names of plays and tasks; to run a playbook within AWS Systems Manager documents, they cannot contain some chars that Ansible usually allows, such as \(\) or \-

## Preparing environment

### Creating AWS CloudFormation stack

In AWSCloudFormation console, create stack using cloudformation.yml file. Parameters:
* General Configuration
  * Environment Name: the name to be used for tagging resources created by stack
* Network Configuration
  * VPC IP range: CIDR block for VPC created by stack (cannot be already in use)
  * Public SubnetX VPC IP range: CIDR block for public subnet 1 and 2; they must be in accordance to VPC's CIDR block; they cannot conflict with CIDR block from each other
* EC2 Configuration
  * KeyPair for EC2 instances: select an already existent key-pair
  * Ip4ServerConnection: IP or CIDR block from machines that can SSH EC2 public instances

### Opening site

Open the DNS address provided by CloudFormation to ELB:
   * DNS found in CloudFormation's output as "DNS of Elastic Load Balancer"

It shall display message "503 Service Temporarily Unavailable" and/or "502 Bad Gateway", until everything is in place (AutoScaling and ELB health checks, plus State Manager execution). Next section explains total amount of time expected for the first instance to get fully running and how to test the addition/removal of new instances using Auto Scaling.

## How to use

### Understanding instance availability time

#### Auto Scaling and ELB health checks

<!--Lots of configs that may conflict
They are not expected to deploy a machine in few minutes-->

#### Lifecycle Hook


### Updating Auto Scaling desired configuration

1. Open EC2 console, navigate menu 'Auto Scaling' and click Auto Scaling Group created by stack (if using CloudFormation default parameters, it shall be named 'p006-sclng-grp');
1. Under menu 'Details', click Edit button in 'Group Details:
<p align="center"><img src="documents/<howto-editgrpdetails.png" width="65%" height="65%"></p>
1. Input desired capacity to provision or delete instances:
   * Stack is provided with defaults 0, 1 and 3 for Minimum, Desired and Maximum capacities, respectively;
   * All capacities can be updated; desired capacity is the one that defines number of instances running;
1. Click Update button.

### Checking Auto Scaling configuration taking effect

1. When the update of Auto Scaling configuration is complete, it will start to register/deregister instances according new desired capacity:
   * when registering new instances, it will take time for the new instance(s) to engage in Load Balancer, as [previously explained](#autoscaling-and-elb-health-checks-explained)
   * when deregistering instances, it also takes times; but it shall be considerably shorter
1. Refresh web page a few times: when instances finish registering, their IDs shall eventually appear in the page; when they deregister, they stop appearing.
<p align="center"><img src="documents/<howto-result.png" width="65%" height="65%"></p>

## Next steps

* Add missing documentation.
