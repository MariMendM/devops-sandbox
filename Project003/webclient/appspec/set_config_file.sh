#!/bin/bash

echo Writing SSM Parameter for backend URL into config file...

# 'aws ssm get-parameter' needs region configured; but once that CodeDeploy does not have environment variables, it was necessary to
#   configure CLI's environment variable AWS_DEFAULT_REGION and a variable PARAM_NAME into CloudInit for public instances inside CloudFormation
# Note: when scripts are run with sudo/root, they can only access environment variables with option -E. Considering that is not possible to
#   specify such parameter in appspec, this script sources the file created by CloudIniti (containing variables exported)
#
source /etc/profile.d/awscli_envvars.sh
sudo sh -c "echo $(aws ssm get-parameter --region $AWS_DEFAULT_REGION --name $PARAM_NAME --query Parameter.Value --output text) > /var/www/html/config/aws-backend-url"