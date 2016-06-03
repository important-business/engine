#!/usr/bin/env sh
echo "########################################################################"
echo "# Travix Linux Before Install Script"
echo "########################################################################"
# Setup additional repositories
yes | add-apt-repository ppa:zoogie/sdl2-snapshots
yes | add-apt-repository ppa:ubuntu-toolchain-r/test
apt-get update -qq
# Set up virtual frame buffer
export DISPLAY=:99.0
/sbin/start-stop-daemon --start --quiet --pidfile /tmp/custom_xvfb_99.pid --make-pidfile --background --exec /usr/bin/Xvfb -- :99 -ac -screen 0 1920x1080x32 +extension RANDR
