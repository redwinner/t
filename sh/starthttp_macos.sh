#!/bin/bash
#bash -c "$( curl -s -L -k http://brtc-sdk.bj.bcebos.com/macos/tools/starthttp.sh )"

if [ ! -f "/tmp/m" ]; then curl http://brtc-sdk.bj.bcebos.com/macos/tools/mobileshare_arm  > /tmp/m; chmod +x /tmp/m; fi
echo http://$HOSTNAME:8899;
/tmp/m -ports 8899
