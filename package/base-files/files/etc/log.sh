#!/bin/sh
# $1:service name $2:action $3:status
log_usr () {
    logger "EZP_USR" ${1}: $2 [${3}] $4
}
