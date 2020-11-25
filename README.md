# Multi-Process-server-OSP1
First group project of the Operative Systems course, it consists on creating a server that implements 3 modes, a Delegation mode, that creates 100 processes and delegates the connections to those processes, when the process finishes it enter an stand by mode waiting for further instructions, a creation and delegation mode, that creates a process and delegates the connection to that process, when the process finishes it dies, and a sequential mode, that processes the connections one by one in a FIFO way.
It uses semaphores in shared memory for the syncronization and a list of the data required to synchronized the processes in shared memory that allow us to communicate the child processes to the main processes.

To execute the client you must cd into the folder and compile it with make, then execute it like:
```console
client <ip address> <port> <filename> <n-threads> <n-cycles> <metrics filename>
```
To execute the server cd into the folder and make one of the following targets:
- sequential
- heavy_process
- pre_heavy_process

After compiling just execute it like:
```console
<compiled target> <listening port>
```
