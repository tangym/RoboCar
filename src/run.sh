# Run GPIO deamon
if pgrep -x "pigpiod" > /dev/null
then
    sudo pigpiod
fi

# Run 5G driver 
if pgrep -x "simcom-cm" > /dev/null
then
    sudo /home/pi/SIM8200_for_RPI/Goonline/simcom-cm &
fi

# Run RoboCar
python3 /home/pi/RoboCar/src/message_client/client.py
