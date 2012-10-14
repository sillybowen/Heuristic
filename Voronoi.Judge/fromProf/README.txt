##############
 Voronoi Game
##############

- Original by Chris Poultney  <crispy@cat.nyu.edu>
- TCP/IP version and documentation by Charles J. Scheffold <cjs285@nyu.edu>


#######
 Files
#######

voronoi.jar				Voronoi game (compiled Java 1.4 JAR file)
voronoi-player.py		Sample player client application in Python
voronoi-protocol.txt	Description of the protocol used by the game
voronoi-src.zip			Source code


##############
 How it works
##############

The Voronoi game can be played byhumans and computers simultaneously. Humans
play using the mouse and computers play via a network socket connection. This 
allows computer players to run on any machine that has network connectivity to 
the machine running the game.


###########################
 Compiling the source code 
###########################

You must have the Java SDK 1.4 or higher installed!

Unzip voronoi-src.zip into a temporary directory. Change to that directory
and execute the following:

javac *.java
jar cvfm voronoi.jar MANIFEST *.class

This will leave you with a file called "voronoi.jar". You can follow the rest
of the directions in this document using that file.


##################
 Running the game
##################

The compiled JAR file needs Java 1.4 or higher to run. If you have an older 
version of Java, you will need to compile the source code yourself (covered 
above in this document). To load the game, type the following at the command 
prompt:

java -jar voronoi.jar

An optional port number can also be passed on the command line:

java -jar voronoi.jar 999

Will force the game to listen on TCP port 999 for connections from computer 
players. If no port is specified, the game will listen on port 20000.

If everything worked, you will see two windows: the main game and a score 
board.


######################
 Controlling the game
######################

The main window has the following controls:

Game control
------------
Reset:	Resets the game to the initial state

Start:	Starts the game once all players are ready (and computer players are
		connected)

Playing field:	This is where the game is played!


Options
-------
Human Players:	Specifies the number of humans playing. Can be 0 if only 
				computers are playing.

Turns:	The number of turns each player gets in the game. 

Player Time Limit:	The time limit that each player has (total) during the 
					game. Note that this is only for display/competition
					purposes. The game does not stop if an individual player
					runs out of time.

NOTE - WHEN ANY OF THE OPTIONS ARE CHANGED, THE GAME IS AUTOMATICALLY RESET!

You must make sure all options are set before you start connecting clients.
Otherwise you will have to reconnect since the sockets will close during
the reset procedure.


#################
 The score board
#################

The score board displays the current status for each player throughout the 
game. Computer players are assigned numbers as they connect (first player 
is 1, second is 2, etc). Human players are numbered after the computer
players. So if there were 2 computer players and 1 human, the human would
be player number 3.

When it is a player's turn, that player's timer will count down and [] 
brackets will be around the TIME field.


##################
 Playing the game
##################

1. Load the Voronoi game application.

2. Configure the options to your liking.

3. If using computer players, connect them now. 

4. Click the start button.

5. Human players will need to wait for their turn and then place their point
by clicking on the board.

6. When the game is finished, the score board will retain the scores until
the Reset button is pressed.


################################
 Using the sample player client
################################

If you would like to see a sample game played by the computer (using a
totally random strategy), a player client is included. It requires
Python 2.0+ in order to run. It is configured to connect to port 20000 on
the local host so if you are using a different setup for the game, you will 
have to modify the PORT and HOST variables at the top of the file.

Once you've got it set the way you need it, just run:

python voronoi-player.py

This will connect to the game and wait for it to start. You can connect up to
10 players this way. Once you've got 2 or more connected, hit Start and watch
the fun!

You can also play against the computer this way by setting Human Players to 1
and connecting only a single instance of the client.


######
 Bugs
######

I am not a Java programmer and therefore the code is quite scary. Not 
to mention the fact that I had a very short period of time to slap this 
together for our in-class competition. So of course there are at least a 
few little bugs hanging around:

- Sometimes the game tells a computer player that it wins even though it 
has lost. It doesn't affect the scoring though.

- There is no blocking of mouse clicks while computer players are "thinking".
So don't go clicking around the board unless it is your turn, or you want
to break the game!

