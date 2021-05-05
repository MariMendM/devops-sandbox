# DevOps Sandbox Projects

Sandbox for demos of tools usually involved in DevOps loop.

---

## Project001
[![Generic badge](https://img.shields.io/badge/Status-Finished-green.svg)](https://shields.io/)

**Purpose:** Demonstrate a Jenkins' pipeline triggered by GitHub's webhook, building a Docker image to host a very simple website (documentation [here](Project001/README.md)).

  <details>
    <summary>Details</summary>
    <img src="Project001/documents/jenkins-diagram.png">
  </details>

**Tools and technologies:** Jenkins; GitHub; Docker; DockerHub; AWS CloufFormation with VPC (Subnet/RTables/NACL/SecGroup/IGateway) and EC2 (instances); YAML

---

## Project002
[![Generic badge](https://img.shields.io/badge/Status-NotStarted-red.svg)](https://shields.io/)

**Purpose:** Demonstrate Ansible's playbook with simple tasks (documentation [here](Project002/README.md)).

  <details>
    <summary>Details</summary>
    <img src="Project002/documents/ansible-diagram.png">
  </details>

**Tools and technologies:** Ansible; AWS CloufFormation with VPC (Subnet/RTables/NACL/SecGroup/IGateway) and EC2 (instances); YAML

---

## Project003
[![Generic badge](https://img.shields.io/badge/Status-InWork-yellow.svg)](https://shields.io/)

**Purpose:** Demonstrate AWS CloudFormation stack creation, complemented by AWS CodeDeploy's website deployment (documentation [here](Project003/README.md)).

  <details>
    <summary>Details</summary>
    <img src="Project003/documents/cloudformation-diagram.png">
	<img src="Project003/documents/codedeploy-diagram.png">
  </details>

**Tools and technologies:** AWS CloufFormation with VPC (Subnet/RTables/NACL/SecGroup/IGateway/NATGateway/NATinstances/EIP), EC2 (Instances/ELB), CodePipeline (CodeCommit/CodeBuild/CodeDeploy) and IAM (Roles/InstanceProfiles); C++; HTML; PHP; JSON; YAML

---

## Project004
[![Generic badge](https://img.shields.io/badge/Status-Finished-green.svg)](https://shields.io/)

**Purpose:** Demonstrate AWS CloudFormation stack creation to work with Docker Swarm (documentation [here](Project004/README.md)).

  <details>
    <summary>Details</summary>
    <img src="Project004/documents/cloudformation-diagram.png">
  </details>

**Tools and technologies:** Docker Swarm; Docker-Compose; AWS CloufFormation with VPC (Subnet/RTables/NACL/SecGroup/IGateway) and EC2; YAML