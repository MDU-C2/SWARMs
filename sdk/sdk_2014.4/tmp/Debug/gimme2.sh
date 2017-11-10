#!/bin/bash
USERNAME=ubuntu
HOSTNAME=192.168.1.1
SCRIPT="'date -u +%Y%m%d%H%M.%S'"

ssh -l ${USERNAME} ${HOSTNAME} "${SCRIPT}"
