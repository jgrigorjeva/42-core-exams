## Sockets
Sockets are a special form of files in UNIX systems; therefore, they have their file descriptors. The `int socket(int domain, int type, int protocol);` createds an "endpoint for communication" (socket), and returns a file descriptor of the created socket (of -1 on error). In our case, `AF_INET` (IPv4 Internet protocols) is used as the `domain`, and `SOCK_STREAM` is used for the`type`. No need to specify a protocol in our case, so `protocol = 0`

### Sets of file descriptors
`int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);`

select()  allows a program to monitor multiple file descriptors, waiting until one or more of the file descriptors become "ready" for some class of I/O operation (e.g., input possible).  A file descriptor is considered ready if it is  possible  to  perform  a
corresponding I/O operation (e.g., read, or a sufficiently small write) without blocking.


The select() function operates on three sets of file descriptors, which allow caller to watch three classes of events. The sets are declared with the type `fd_set`. 

In the set `readfds`, fds are watched do see if they are ready for reading. After select() has returned, `readfds` will be cleared of all file descriptors except for those that are ready for reading. Therefore, the set needs to contain all the available fds we might want to read from, and only the ones ready for reading reamin after the select() call.

The set `writefds` containes fds that are watched to see if they are ready for writing. Similarly, after the call to select(), only the ready fds are left.

The third set `exceptfds` containes fds that are watched for "exceptional conditions". We do not need to watch for such conditions in our miniserv, so se can use "0" instead.

Since we use the `select()` in a loop, the `readfds` and `writefds` need to be reset to all fds at the beginnig of the loop, so that we watch all of them.

#### Modification of the sets
The contents of an fd set can be manipulated by macros: 
- FD_ZERO(&set) - removes all fds from a set (e.g. to initialize the set)
- FD_SET(fd_to_be_added, &set) - adds an fd to a set
- FD_CLR(fd_to_be_removed, &set) - removes an fd from a set
- FD_ISSET(fd_to_check, &set) - returnes nonzero if fd is present in a set, and zero if not