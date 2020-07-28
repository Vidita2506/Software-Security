# Software-Security

Labs implemented as part of Software Security course

Assignment 1: Priviledge separation:

Given is a simple C program for socket programming with server and a client.
The server creates a socket on port 80 and listens on it. It accpets client connections and can communicate with the client.
The server needs to run with root privileges to be able to bind to port 80.
But does it need root privilege to listen and accept client connections and for the rest of the communication ?
No it doesn't.

The server program implements privilege separation where it drops the root privilege to a nobody user to avoid any possible attack.

Assignment 2: Address Space Layout Randomization

Follow up to assignment 1, perform ASLR so that the forked process with less privileges does not have same address space as the parent process
running with root privileges.
