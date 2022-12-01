# Refer to https://docs.ros.org/en/humble/Installation/Ubuntu-Install-Debians.html

# Check the locale.
if locale | grep -q UTF-8 ; then
    echo "Found a locale supports UTF-8."
else
    echo "No locale supports UTF-8. Installing locale-UTF-8."
    sudo apt update && sudo apt install locales
    sudo locale-gen en_US en_US.UTF-8
    sudo update-locale LC_ALL=en_US.UTF-8 LANG=en_US.UTF-8
    export LANG=en_US.UTF-8
fi

# Ensure the Ubuntu Universe repository is enabled.
sudo apt install software-properties-common
sudo add-apt-repository universe

# Add ROS 2 GPG key and the repository to the sources list.
sudo apt update && sudo apt install curl
sudo curl -sSL https://raw.githubusercontent.com/ros/rosdistro/master/ros.key -o /usr/share/keyrings/ros-archive-keyring.gpg
echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/ros-archive-keyring.gpg] http://packages.ros.org/ros2/ubuntu $(. /etc/os-release && echo $UBUNTU_CODENAME) main" | sudo tee /etc/apt/sources.list.d/ros2.list > /dev/null

# Update apt repository caches
sudo apt update
sudo apt upgrade

# Install bare bone package
# search for "ros-.+-ros-base"
sudo apt install ros-foxy-ros-base

# Setup environment
source /opt/ros/foxy/setup.bash
