# sysnet-client-srv
Sysnet client server app for future stuff

# Building
```
$ git clone https://github.com/matteyeux/sysnet-client-srv
$ cd client-srv
$ make 
```

# XML configuration
Using `-x` arg you can run client with a config file written in XML as this :
```
<server_param>
        <ip>192.168.0.3</ip>
        <port>50683</port>
        <cmd>ls</cmd>
</server_param>
```

default config :
- ip = 127.0.0.1
- port = 50683
- cmd = ls
