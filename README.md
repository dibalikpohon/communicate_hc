# communicate_hc

I don't know what is this  
It sends some message each other  
It only runs on Linux... yes, sorry for the inconveniene

## Message structuring
```
+=================|========================+  
| message length  |      message body      |
|   (8 bytes)     |      (may be long)     |
|  or equiv. to   |                        |
|   std::size_t   |                        |
+=================|========================+
```

## Makefiles
```make main_client``` to create the client-side main file  
```make main_host``` to create the host-side main file

## Role
### host
The host, owns the chat. Host opens the chat by executing
```main_host <port>``` where `<port>` is the port that
you want to open. e.g. 8052  
Currently, only one client can connect to a host.

### Client
The client, connects to a chat. Client connects to a chat
by executing ```main_client <address> <port>``` where `<address>`
is the host's address that owns a chat and
`<port>` is the port that client should connec to.

### Example
A host has the IP address `192.168.100.111` and the host
wants to open the chat on port 9910. The host should type
```main_host 9910```.  
A client wants to connect to a host `192.168.100.111` on
port 9910. The client should type this ```main_client 192.168.100.111 9910```
