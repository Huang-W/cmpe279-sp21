## CMPE 279 - SW Security Techs

#### Programming Assignment 1

In this assignment, the parent and child processes share the same file descriptors (without re-exec).

The fork is done after binding the socket to a local address on port 80 (a privileged operation).

After forking, the parent process waits for the child process to exit, and the child process blocks until a connection is present.

A passwd entry with name of 'nobody' is expected to be present.

---

Email: ward.huang@sjsu.edu

Student ID: 013776178
