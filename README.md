# tv-managing-system
System designated to implement in company easy TV display system.
It allows deploy TVs (endpoints) based on linux (e.g. Raspberry Pi) and manage all of them using single pc.

>
> \[!NOTE]
>
> Project created for [100commitow](https://100commitow.pl/) challange.

## Overview
![System scheme](scheme.jpg "System scheme")

System consists of 3 main parts:
- **Server** - that manages whole system
- **Endpoint** - any display connected to PC
- **Client** - that allows to manage system (change settings etc.)

Main purpose of this system is to allow you to easily setup display system (e.g. in company) and give you user-friendly access to manage it.

Very important is that, this system is opensource, so you can customize it how you want and adapt to your needs.

## Usage
TODO

## Installation
TODO
### Server
`sudo make clean install`

### Client
IDK - TODO

### Endpoint
`sudo make clean install`


## Description
### Server
*Linux*

Server is responsible for managing the entire system. It's main function is to communicate with endpoint to distribute configuration and manage them. The server doesn't hosts any resources (like images). It can be obtained with external solutions (e.g. Samba). Server performs various functions including: processing change requests, providing responses to queries, checking the connection with the endpoints, and ensuring that client is always updated with latest configuration.

Server also provides interface to manage configurations, endpoints and server it self with Client application.

TODO

### Client
*Windows*

Client application gives end-user ability to easily manage the system. It provides GUI based communication with server, so it is user-friendly. You can check actual status of endpoints, manage global / per endpoint configuration and reload endpoints settings.

### Endpoint
*Linux*

Endpoint allows to display images on TV (any display connected to machine), using X11 library. Its main advantage is that is written in C++, so it allows to obtain low system requirements. That allows you to make low cost and practical display system (e.g. TV connected to Raspberry Pi). Endpoint program gets settings from server. Files to be displayed are obtained from path. To make fully centrally managed system (meaning easy access to files on endpoint) you can setup SMB server, mount it on every endpoint and configure path on tv-managing-system server to this mountpoint.

## Configuration
TODO

## Security
At this point of developement all connections are not secured (in plain text). Also passwords are stored in plain text.

It is planned to implement hashed passwords and SSL secured connections.

## Contribution
**ToDo:** [here](TODO.md)

> **SEGFAULT COUNTER**: 6
