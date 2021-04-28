# Project003

## Content

[Demo description](#demo-description) <br>
[Repo files overview](#repo-files-overview) <br>
[Preparing Environment](#preparing-environment) <br>
... [Create AWS CloudFormation stack](#create-aws-cloudformation-stack) <br>
... [Manually deploy apps](#manually-deploy-apps) <br>
... [Open webclient page](#open-webclient-page) <br>
[How to use](#how-to-use) <br>
[Next steps](#next-steps)

## Demo description

An AWS CloudFormation stack configuration deploys a VPC infrastructure supporting EC2 public and private servers hosting, respectively, a webclient (html/php/javascript) and a serverapp (rest c++ api) communicating between each other. Load Balancers are also provided, the frontend balancer accessed by the user via web browser, and the backend one configured to be accessed by the frontend page. Besides infrastructure, the CloudFormation stack also implements an AWS CodePipeline configuration to update both, front and backend apps.

Once private instances need communication to the internet at some point, NAT translation is needed. Two different options for NAT are provided in this demo:
* Stack containing NAT Gateway (yml named "natgateway"), the NAT managed service from AWS
  * AWS resource charged by hour and by traffic, not included in AWS free tier
  * Two resources included; so, costs apply twice!
* Stack containing NAT Instances (yml named "natinstances")
  * A free-of-charge __\*\*\*__ option while inside free tier period in AWS account
  * Not an option for production environments, but perfectly adequate for this demo!

__\*\*\*__ It can be run free of charges inside AWS free tier period, if account creating stack has not yet reached limits of free tier for resources provided by this stack.

All diagrams included in documentation are Draw.io's editable PNGs.

## Repo files overview

* Folder webclient:
  * simple webpage intended to provide a "visualization" of AWS infrastructure implemented in this demo. It actually only displays AWS EC2 meta-data from instances hosting the demo (documentation [here](webclient/README.md))
   <details><summary>corresponding diagram</summary><img src="webclient/documents/modules-organization-diagram.png"></details>
* Folder serverapp:
  * simple REST API developed in C++ to return json responses, with focus for this demo on the HTTP GET method for path /ec2/instance-id (documentation [here](serverapp/README.md))
   <details><summary>corresponding diagram</summary><img src="serverapp/documents/restapi-logic-diagram.png"></details>
* Files cloudformation\*.yml:
  * creates a VPC
    * 4 subnets (2 public, 2 private);
    * 3 route tables (1 for public, 1 for each private subnet);
    * 1 internet gateway (route in public route table);
    * 2 EIPs for NAT ec2 instances;
    * 2 network NACL (1 for public, 1 for private traffic)
      * HTTP/HTTPS/Ephemeral allowed for In/Outbound to CIDR 0.0.0.0/0
	  * SSH/ICMP allowed for In/Outbound to CIDR provided for maintenance
  * creates security group for public instances hosting webclient
    * HTTP/HTTPS allowed for Inbound to CIDR 0.0.0.0/0
	* SSH/ICMP allowed for Inbound to CIDR provided for maintenance
  * creates security group for private instances hosting serverapp
    * Full inside traffic allowed for Inbound to CIDR VPC (x.x.x.x/16)
  * creates security group for NAT instances
    * HTTP/HTTPS allowed for Inbound to CIDR of both public Subnets (x.x.x.x/24)
  * creates security group for ELB of public instances
    * HTTP allowed for Inbound to CIDR 0.0.0.0/0
  * creates security group for ELB of private instances
    * 8080 allowed for Inbound to CIDR 0.0.0.0/0
  * creates 4 EC2 instances (2 for webclient, 2 for serverapp)
    * t2.micro;
    * ubuntu 20.04;
    * Code-Deploy Agent and prereqs of front/back apps installed by cloud init
  * creates 2 EC2 instances (for NAT)
    * t2.micro;
    * ubuntu 20.04;
    * Configurations to enable IP forwarding and to route packets coming from VPC to outside
  * creates 2 ELBs
    * listener HTTP at port 80 targeting public (webclient) instances
    * listener HTTP at port 8080 targeting private (serverapp) instances
<!--   <details><summary>corresponding diagram</summary><img src="documents/cloudformation-diagram.png"></details> -->
<!--* File [appspec.yml](appspec.yml) and appspec_scripts folder:
  * TODO-->

## Preparing environment

### Create AWS CloudFormation stack

Create stack using one of cloudformation\*.yml files. Parameters:
* General Configuration
  * Environment Name: the name to be used for tagging resources created by stack
  * Environment Type: used to define a conditional when creating ports of NetworkACL and Security Groups
    * MAINTENANCE opens ports such as SSH and Ephemeral (for connections to the hosts and execution of APT, for example)
    * PROD disables these same ports
    * The idea is to always run the creation of the stack using MAINTENANCE, updating after to PROD (and alternate between both as necessary)
    * For this demo, keep it as MAINTENANCE
* Network Configuration
  * VPC IP range: CIDR block for VPC created by stack (cannot be already in use)
  * Public/Private SubnetX VPC IP range: CIDR block for public subnet; must be in accordance to VPC's CIDR block; they cannot conflict with CIDR block from each other
* EC2 Configuration
  * KeyPair for EC2 instances: select an already existent key-pair
  * Ip4ServerConnection: IP or CIDR block from machines that can SSH EC2 public\* instances
  
\* To SSH private instances, it is necessary to jump-host them:
* Enable SSH Agent Forwarding in SSH client in use
* Connect one of public instances using SSH and from inside it, to SSH private instance

### Manually deploy apps

Using SSH, connect to each public EC2 instance (the ones not configured for NAT):
* Copy webclient folder content to /var/www/html
* Edit /var/www/html/config/aws-backend-url replacing
  * \<IP-or-DNS-serverapp\> by DNS from Backend ELB, found in CloudFormation's output as "DNS of ELB Backend"
  * \<port\> by 80

Using SSH from inside one of public servers (jump-host), connect to each private EC2 instance:
* Copy serverapp folder content to /home/ubuntu (or any other location desired)
* Add execution permission to '/home/ubuntu/dependencies/install_dependencies.sh' with command ```chmod +x install_dependencies.sh```
* Run ```./dependencies/install_dependencies.sh```
* Build app with command ```make```
* Execute app running ```./bin/restapi.app 8080 &```

### Open webclient page

Open page using DNS address of Frontend Load Balancer:
   * DNS found in CloudFormation's output as "DNS of ELB Frontend"

## How to use

To refresh the page in the browser triggers the Frontend Load Balancer, alternating the instance-id provided by the targeted instance hosting webclient.

To refresh the page in the browser or to click the button in the page triggers the Backend Load Balancer, alternating the instance-id provided by the targeted instance hosting serverapp.

## Next steps

* Implement AWS CodePipeline within AWS CloudFormation stack (for that, instructions to manually deploy apps were provided).
* Document serverapp (c++ api rest) with Swagger
* Document CloudFormation stack (diagram still missing)
