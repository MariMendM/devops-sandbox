# Project001

## Content

* [Demo description](#demo-description)
* [Repo files overview](#repo-files-overview)
* [Preparing Environment](#preparing-environment)
  * [Create AWS CloudFormation stack](#create-aws-cloudformation-stack)
  * [Finish Jenkins installation](#finish-jenkins-installation)
  * [Configure Jenkins for integration to GitHub](#configure-jenkins-for-integration-to-github)
  * [Configure Jenkins for integration to Docker/DockerHub](#configure-jenkins-for-integration-to-docker/dockerhub)
  * [Configure Email Notifications ](#configure-email-notifications)
  * [Configure Jenkins Pipeline](#configure-jenkins-pipeline)

## Demo description

A Jenkins pipeline triggered by GitHub's webhook (at pushes) builds a Docker image to host a website (simply an index.html) using Apache. The image is also uploaded to DockerHub.

If using AWS EC2 instance to run the demo, there is a CloudFormation file included to deploy everything necessary.

```diff
# Reminder: all diagrams included in documentation are Draw.io's editable layered PNGs.
```

## Repo files overview

* File [jenkinsfile](jenkinsfile) for pipeline (groovy):
  * clones this GitHub folder (Prooject001);
  * builds corresponding dockerfile;
  * tests built image;
  * uploads image into DockerHub with tags 'latest' and Jenkins' build_number;
  * cleans local images
  * <details><summary>see corresponding diagram</summary><img src="documents/jenkins-diagram.png"></details>
* File [dockerfile](dockerfile):
  * built from image httpd;
  * copies website folder to Apache's standard folder;
  * uses port 80
* File [cloudformation.yml](cloudformation.yml):
  * creates very (very!) simple VPC
    * 1 subnet (public);
    * 1 route table (for public subnet);
    * 1 internet gateway (route in public route table);
    * 1 network NACL (for public traffic) <a name="firewallrules"></a>
      * HTTP/HTTPS/Ephemeral allowed for In/Outbound to CIDR 0.0.0.0/0
      * SMTP allowed for Outbound to CIDR 0.0.0.0/0
	  * SSH allowed for In/Outbound to CIDR provided for maintenance
      * \*\*\* If not using AWS stack for demo, remember to apply the above firewall rules
  * creates security group for instance
    * HTTP/HTTPS/8080 allowed for Inbound to CIDR 0.0.0.0/0 (8080 for Jenkins)
	* SSH allowed for Inbound to CIDR provided for maintenance
  * creates an EC2 instance
    * t2.micro;
    * ubuntu 20.04;
    * Jenkins and Docker installed by cloud init
  * <details><summary>see corresponding diagram</summary><img src="documents/cloudformation-diagram.png"></details>
* File [jenkins-pipeline-config.xml](jenkins-pipeline-config.xml):
  * export of pipeline's configuration.

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
  * Ip4ServerConnection: IP or CIDR block from machines that can SSH EC2 public\* instances

\* If not using AWS stack for demo, do not forget firewall rules as described in [CloudFormation description](#firewallrules)

### Finish Jenkins installation

1. Open jenkins
   * Jenkins URL is output by CloudFormation under "URL of Jenkins installed in EC2"
   * IP of server can be retrieved from URL
1. Connect to server by SSH to acquire unlock password
   * sudo cat /var/lib/jenkins/secrets/initialAdminPassword (or folder indicated on Jenkins' welcome screen)
1. Install plugins
   * names
     * Github Plugin
     * Docker Pipeline
     * Email Extension Plugin
   * some of they may not be available from installing screen; necessary to install later, after Jenkins initialization
1. Create user/password
1. Save and finish

### Configure Jenkins for integration to GitHub

Details at [GitHub integration](https://github.com/MariMendM/devops-sandbox/wiki/Jenkins#JenkinsGitHub). The subsection describing webhook configuration requires Jenkins' pipeline already created.

### Configure Jenkins for integration to Docker/DockerHub

Details at [Configuring Docker/DockerHub integration](https://github.com/MariMendM/devops-sandbox/wiki/Jenkins#JenkinsDocker)

### Configure Email Notifications

Details at [Configuring Extended email notification](https://github.com/MariMendM/devops-sandbox/wiki/Jenkins#JenkinsExtEmailNot)

### Configure Jenkins pipeline

Pipeline can be created based in [jenkins-pipeline-config.xml](jenkins-pipeline-config.xml). Main points to remeber:
* Parameterize pipeline with variables, as required by jenkinsfile
* Define configuration "Pipeline script from SCM", indicating "Project001/jenkinsfile" as script path
* For GitHub's webhook to work, the pipeline must be manually run at least once
