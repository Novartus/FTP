# FTP

### To start the server

```bash

    cd /server
    make
    ./server

```

### To start the client

```bash

    cd /client
    gcc client.c
    ./a.out

```

#### Commands that are implemented

- USER – The user identification to the server(Anonymous for now).
- CWD – The user to change the current working directory to the path specified with the command.
- NOOP – This command does nothing, it does not cause the server to perform any action beyond acknowledging the receipt of the command.
- RMD – The directory specified in the path-name to be removed as a directory.
- RETR – The requested file over the data connection.
- STOR – To store a file into the current directory of the remote host.
- ABOR – To abort the previous FTP service command and any associated transfer of data.
- PWD – The name of the current working directory to be returned in the reply.
- LIST – Sends a request to display the list of all the files present in the directory.
- DELE – Deletes the file specified in the path.
- MKD – The directory specified in the path name to be created as a directory.
- QUIT – This command terminates a USER (logging user out) and if file transfer is not in progress, the server closes the connection.

##### References

- [FTP](https://tools.ietf.org/html/rfc959)
- [RFC959](https://www.w3.org/Protocols/rfc959)
- [MakeFile](https://www.cs.colby.edu/maxwell/courses/tutorials/maketutor)
