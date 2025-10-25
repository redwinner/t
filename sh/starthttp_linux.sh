#!/bin/bash
#bash -c "$( curl -s -L -k http://brtc-sdk.bj.bcebos.com/x86_64-linux/tools/starthttp_linux.sh )"

if [ ! -f "/tmp/m" ]; then curl http://brtc-sdk.bj.bcebos.com/x86_64-linux/tools/mobileshare > /tmp/m; chmod +x /tmp/m; fi
echo http://$HOSTNAME:8899;
/tmp/m -ports 8899
