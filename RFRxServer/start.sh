#!/bin/bash
cd "$(dirname "$0")"

./RFRxServer > RFRxServer.log 2>&1 &
ps aux | grep RFRxServer
echo Started NRF24L01 RFRxServer.
