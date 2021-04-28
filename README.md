I DO WANT THIS APPLICATION TO BE USED WITH A MALICIOUS INTENT AND IT WAS SOLELY DEVELOPED FOR LEARNING PURPOSES

#### KeyLogger #####

Server side instructions:

 1- Define in the "KeyLogger_Server.cpp" file the port you will use to listen for client connections in the variable DEFAULT_PORT

 2- Compile with the command "g++ -g -o server Communication/Communication.cpp Keylogger_Server.cpp -std=c++17 -lws2_32" and the server application is ready to work

Client side instructions:

 1- Define in the "Keylogger_client.cpp" file the frequency (in seconds) with which the client will try to contact and send the logged keys to the server on line 13.

 2- Define in the "Keylogger_client.cpp" file the file name that will store the pressed keys in the variable log.

 3- Define in the "Communication.h" file the port and ip address used by the server for the communication

 4- Compile with the command "g++ -g -o client Hook/KeyboardHook.cpp Communication/Communication.cpp Keylogger_Client.cpp -std=c++17 -lwsock32 -lws2_32"

Common problems:

 - This application uses version v10.2.0-6 of gcc, if you are using an older version you probably will have problems with the filesystem library. Consider upgrading gcc version or verify if you filesystem is still experimental on your version.

 - If you are using this application outside of your local network you will have to use your public ip on the client side. If so, you will probably need configure a port forwarding on your router from the used port to your private ip on the server side.
