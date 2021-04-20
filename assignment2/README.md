## CMPE 279 - SW Security Techs

#### Programming Assignment 2

In this assignment, the parent and child processes do not share the same local variables.

After forking, the child process replaces itself with a new process (same process name though './server').

The file descriptor from the parent is passed to the child as an argument.

The parent process waits for the child process to exit, while the child blocks until a connection is present.

The child lowers its privilege using a passwd entry with name of 'nobody'.

---

Email: ward.huang@sjsu.edu

Student ID: 013776178
