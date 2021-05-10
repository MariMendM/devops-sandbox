# Project003

## Content

* [Demo description](#demo-description)
* [Repo files overview](#repo-files-overview)
* [Preparing Environment](#preparing-environment)
  * [Create AWS CodeCommit Repositories](#create-aws-codecommit-repositories)
  * [Create AWS CloudFormation stack](#create-aws-cloudformation-stack)
  * [Open demo](#open-demo)
* [How to use](#how-to-use)
* [Next steps](#next-steps)

## Demo description

An AWS CloudFormation stack configuration deploys a VPC infrastructure supporting EC2 public and private servers hosting, respectively, a webclient (html/php/javascript) and a serverapp (rest c++ api) communicating between each other. Load Balancers are also provided, the frontend balancer accessed by the user via web browser, and the backend one configured to be accessed by the frontend page. Besides infrastructure, the CloudFormation stack also implements an AWS CodePipeline (CodeCommit, CodeBuild and CodeDeploy) configuration to update both, front and backend apps.

<p align="center"><img src="documents/demo-001.png" width="65%" height="65%"></p>

Note: Once private instances need communication to the internet at some point, NAT translation is needed. Two different options for NAT are provided in this demo:
* Stack containing NAT Gateway (yml named "natgateways"), the NAT managed service from AWS
  * AWS resource charged by hour and by traffic, not included in AWS free tier
  * Two resources included; so, costs apply twice!
* Stack containing NAT Instances (yml named "natinstances")
  * A free-of-charge __\*\*\*__ option while inside free tier period in AWS account
  * Not an option for production environments, but perfectly adequate for this demo!

__\*\*\*__ It can be run free of charges inside AWS free tier period, if account creating stack has not yet reached limits of free tier for resources provided by this stack.

```diff
# Reminder: all diagrams included in documentation are Draw.io's editable layered PNGs.
```

## Repo files overview

* Folder **webclient**:
  * simple webpage intended to provide a "visualization" of AWS infrastructure implemented in this demo. It actually only displays AWS EC2 meta-data from instances hosting the demo
  * appspec folder and appspec.yml
    * YAML file and scripts for CodeDeploy implemented by CloudFormation files
  * remaining content
    * Web page source code (documentation [here](webclient/README.md))
    * <details><summary>see corresponding diagram</summary><img src="webclient/documents/modules-organization-diagram.png"></details>
* Folder **serverapp**:
  * simple REST API developed in C++ to return json responses, with focus for this demo on the HTTP GET method for path /ec2/instance-id
  * buildspec.yml
    * YAML file for CodeBuild implemented by CloudFormation files
  * appspec folder and appspec.yml
    * YAML file and scripts for CodeDeploy implemented by CloudFormation files
  * remaining content
    * API source code (documentation [here](serverapp/README.md))
    * <details><summary>see corresponding diagram</summary><img src="serverapp/documents/restapi-logic-diagram.png"></details>
* Files **cloudformation\*.yml**:
  * create one VPC
    * 4 subnets (2 public, 2 private)
    * 3 route tables (1 for public, 1 for each private subnet)
    * 1 internet gateway (route in public route table)
    * For NAT in yaml "natinstances"
      * 2 EIPs for NAT ec2 instances
    * For NAT in yaml "natgateways"
      * 2 NAT Gateways
    * 2 network NACL
      * 1 for public traffic
        * HTTP/HTTPS/Ephemeral allowed for In/Outbound to CIDR 0.0.0.0/0
        * SSH/ICMP allowed for In/Outbound to CIDR provided for maintenance
      * 1 for private traffic
        * Full inside traffic allowed for In/Outbound to CIDR VPC (x.x.x.x/16)
        * HTTP/HTTPS allowed for Outbound to CIDR 0.0.0.0/0
        * Ephemeral allowed for Inbound to CIDR 0.0.0.0/0
  * create Security Groups
    * 1 for public instances hosting webclient
      * HTTP/HTTPS allowed for Inbound to CIDR 0.0.0.0/0
      * SSH/ICMP allowed for Inbound to CIDR provided for maintenance
    * 1 for private instances hosting serverapp
      * Full inside traffic allowed for Inbound to CIDR VPC (x.x.x.x/16)
    * 1 for NAT instances
      * HTTP/HTTPS allowed for Inbound to CIDR of both private Subnets (x.x.x.x/24)
      * SSH allowed for Inbound to CIDR provided for maintenance
    * 1 for ELB of public instances
      * HTTP 80 allowed for Inbound to CIDR 0.0.0.0/0
    * 1 for ELB of private instances
      * HTTP 8080 allowed for Inbound to CIDR 0.0.0.0/0
  * create EC2 Instances
    * 4 to host apps (2 for webclient, 2 for serverapp)
      * t2.micro;
      * ubuntu 20.04;
      * Code-Deploy Agent and prereqs of front/back apps installed by cloud init
    * 2 as NAT instances (applicable only to "natinstances" yaml file)
      * t2.micro;
      * ubuntu 20.04;
      * Configurations to enable IP forwarding and to route packets coming from VPC to outside
  * create ELBs
    * 1 application ELB, internet-facing, with listener HTTP at port 80 targeting public (webclient) instances
    * 1 application ELB, internal, with listener HTTP at port 8080 targeting private (serverapp) instances
    * 1 System Manager Parameter (to store internal ELB's DNS address to be configured into webclient's config file)
  * create IAM roles
    * 1 CodePipeline role
    * 1 CodeBuild role
    * 1 CodeDeploy role
    * 1 EC2 role
    * 1 EC2 instance profile, to attach EC2 role into instances
  * create a pipeline to deploy webclient into public instances, and build/deploy serverapp into private instances
    * 1 S3 Bucket for artifacts
    * 1 CodeBuild (serverapp)
    * 2 CodeDeploys (webclient and serverapp)
    * 1 CodePipeline
      * steps for webclient: source from CodeCommit, and deploy with CodeDeploy
      * steps for serverapp: source from CodeCommit, build with CodeBuild, and deploy with CodeDeploy
	  * <details><summary>see corresponding CodePipeline diagram</summary><img src="documents/codepipeline-diagram.png"></details>
  <!--* <details><summary>see corresponding CloudFormation diagram</summary><img src="documents/cloudformation-diagram.png"></details> -->

## Preparing environment

### Create AWS CodeCommit Repositories

CodePipeline does not support git sparse checkout. For that, instead of use this GitHub as source, CodeCommit shall be used to provide app source codes for CI/CD pipeline:
1. Create CodeCommit repository and upload files from folder webclient
1. Create CodeCommit repository and upload files from folder serverapp

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
  * Ip4ServerConnection: IP or CIDR block from machines that can SSH EC2 public<sup>1</sup> instances
* Repository configuration
  * CodeCommit repository for frontend code: input the name of CodeCommit repository created for webclient in previous section
  * CodeCommit branch name for frontend repo: input the name of the branch to be used in webclient repo created<sup>2</sup>
  * CodeCommit repository for backend code: input the name of CodeCommit repository created for serverapp in previous section
  * CodeCommit branch name for backend repo: input the name of the branch to be used in serverapp repo created<sup>2</sup>

:one: To SSH private instances via jump host:
* Enable SSH Agent Forwarding in SSH client in use
* Connect to one of public instances using SSH and from inside it, then SSH private instance desired

:two: Git normally uses "master" while CodeCommit uses "main"; check the branch names available into AWS CodeCommit console for each repository.

### Open demo

Open the DNS address provided by CloudFormation to Frontend Load Balancer:
   * DNS found in CloudFormation's output as "DNS of ELB Frontend"

## How to use

With browser open:
* To refresh the page in the browser triggers the Frontend Load Balancer, alternating the 'instance-id' provided by the targeted instance hosting webclient;
* To refresh the page in the browser or to click the button in the page triggers the Backend Load Balancer, alternating the 'instance-id' provided by the targeted instance hosting serverapp

IMPORTANT: It takes a few minutes until stack creates all the resources, as well for CI/CD pipelines to finish their jobs. Before to run the demo, follow up in AWS consoles the progress of the tasks (specially for CodePipeline; it has to be 100% finished, otherwise you will only see messages deployed during ec2 instances creation).

## Next steps

* Document CloudFormation stacks (diagram still missing)
