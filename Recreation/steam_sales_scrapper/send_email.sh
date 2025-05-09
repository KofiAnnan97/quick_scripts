#!/bin/bash

# Command line arguments
while getopts "p:" flag; do
  case ${flag} in
    p) SCRIPT_DIR=${OPTARG};;
  esac
done

# Go to script directory
cd $SCRIPT_DIR

# Create log directory
LOG_DIR="$SCRIPT_DIR/log"
if [ ! -d $LOG_DIR ]; then
    mkdir -p $LOG_DIR
fi

# Run script to get game sales
DATE_TIME=$(date +"%Y_%m_%d_%H_%M")
LOG_NAME=$LOG_DIR/$DATE_TIME"_email.log"
./target/release/steam_sales_scrapper --send-email > $LOG_NAME
if [ -f $LOG_NAME ]; then
    echo "$LOG_NAME created"
fi