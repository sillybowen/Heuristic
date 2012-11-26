SERVER
=============

To run the server, use a command like the following:

java -jar portfolio-1.0.0.jar -p 54321 -n 5 -m mode-1

 -p The port number to listen on.
 -n The number of expected players.
 -m The game mode. Accepted values are mode-1 or mode-2.
 -c Optional. For game mode 2. The class favorability file as specified by Prof. Shasha. Defaults to favorability.txt in same dir as jar.
 -g Optional. Display the gui? Accepted values are true or false. Defaults to false.
 -f Optional. Use this file of gambles and links. If absent a new file is generated. 

It is recommended that you start by ignoring the optional flags. You can use them to fine tune your program. 

At the time of this writing the gui is not integrated with the server. There will be a notification when the gui is ready.

Note that the default favorability file will be different at competition time.

CLIENT
===========

To run the random client, use a command like the following:

java -jar portfolio-client.jar 54321

Where the single argument is the port the server is using.
