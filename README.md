# 🖥 tv-managing-system
<p align="center">
<!-- ![TMS Icon](icon-256.png "Icon") -->
  <img src="icon-256.png" height="128px"/><br><br>
  System designated to implement in company easy TV display system.<br>
  It allows deploy TVs (endpoints) based on linux (e.g. Raspberry Pi) and manage all of them using single pc.<br>
</p>

>
> \[!NOTE]
>
> Project created for [100commitow](https://100commitow.pl/) challange.

## 🔍 Overview
![System scheme](scheme.jpg "System scheme")

System consists of 3 main parts:
- **Server** - that manages whole system
- **Endpoint** - any display connected to PC
- **Client** - that allows to manage system (change settings etc.)

Main purpose of this system is to allow you to easily setup display system (e.g. in company) and give you user-friendly access to manage it.

Very important is that, this system is opensource, so you can customize it how you want and adapt to your needs.

## 🕹 Usage
Install server on linux machine that will have access to connect to endpoints and will give access for clients to connect.
After installation and configuration you need to allow connections on firewall to server port.

Next step will be deploying endpoints.
Follow installation process, and after that you can add your endpoints in server configuration.
You can also deploy endpoints on the go (when server is running), but you will need to restart server after changing configuration.

Server and Endpoints are using `syslog` to write some logs. This allows you to verify if everything is working.

Client allows you to manage system.
When server starts you will be able to connect to it using client and change configurations.
You need to provide address, port and credentials to connect to server.

All new endpoints will use global configuration if config is not found.

## 📦 Installation
### Server
To install server daemon you need to build and create default config files. To do that just run below command in repo directory:
```
sudo make clean install
```
**Server do not require any external libraries**

If you don't want to override your configs use *(probably temporary solution)*:
```
sudo make clean ndinstall
```
Next important step will be configuration of basic settings - [read here](#server-2)

After that you can simply run `tmsd.service`. Ex:
```
systemctl start tmsd
```
You can verify if everything is correctly running in syslog.

To disable daemon just stop service. Ex:
```
systemctl stop tmsd
```


### Client
Download installer `.msi` or portable version from [Releases](releases/).
- for installer follow simple installation process *(by default client is installed in `C:\Program Files (x86)\23vbq\TMSC`)*
- for portable just extract `.7z` file *(dll's must be in same directory as executable)*

If you want to build client by your self you need to download and install [Qt](https://www.qt.io/download),
clone repository and open project from `CMakeLists.txt`.

To deploy your build you can use `windeployqt` tool which is installed with Qt.
It will copy all required libraries to directory with executable.
Also you can use [AdvancedInstaller template](client/TMSC.aip) to deploy it in installable package.

### Endpoint
To install endpoint application first build it:
```
sudo make clean install
```

<details>

<summary><b>Required libraries</b></summary>

- libx11

</details>

Or without overriding your existing config:
```
sudo make clean ndinstall
```
Next step will be configuration - [read here](#endpoint-2)

*After configuring endpoint you can add it to server configuration.*

To start endpoint you need to add it to your `.xinitrc`.
If you don't know how to do this here is [xinit configuration](https://wiki.archlinux.org/title/xinit).
Installed binaries are in `/usr/bin` by default.

To run endpoint application use:
```
startx
```

<details>

<summary><b>Required packages</b></summary>

- xorg-server
- xorg-xinit

</details>

For closing endpoint there are two methods:
- Kill it from shell (ex. `pkill tmse`)
- Use keyboard shortcut `ALT + Q`


## 📜 Description
### Server
*Linux*

Server is responsible for managing the entire system. It's primary role is to communicate with endpoint to distribute configuration and manage them. The server doesn't hosts any resources (e.g. images, files). These can be obtained with external solutions like Samba. Server performs various functions including: processing change requests, providing responses to queries, checking the connection with the endpoints, and ensuring that client is always updated with latest configuration.

Server also provides interface to manage configurations, endpoints and server it self with Client application.

Network functions:
- Accepts connection from clients and response to their queries
- Connects to endpoints and sends data to them

### Client
*Windows*

Client application gives end-user ability to easily manage the system. It provides GUI based communication with server, so it is user-friendly. You can check actual status of endpoints, manage global / per endpoint configuration and reload endpoints settings.

### Endpoint
*Linux*

Endpoint allows to display images on TV (any display connected to machine), using X11 library.
Its main advantage is that is written in C++, so it allows to obtain low system requirements. That allows you to make low cost and practical display system (e.g. TV connected to Raspberry Pi).
Endpoint program gets settings from server. Files to be displayed are obtained from provided path. To make fully centrally managed system (meaning easy access to files on endpoint) you can setup SMB server, mount it on every endpoint and configure path on tv-managing-system server to this mountpoint.

## ⚙ Configuration
After configuration you can copy your config files to `.def` files in repo to not be overridden after installing.
In future maybe there will be provided some mechanism to not override your files and easily update configs, if new variables will be required.

### Server
Server configuration is stored in `/etc/tmsd/`.
Default settings are stored in def files in repo and are copied to desired location when installing.

Server uses two files for it's configuration:
- [settings.cfg](server/setting.cfg.def) - to store basic server socket settings
- [authkey](server/authkey.def) - to store authentication key for server *(now it is stored in plain text - [read here](#security)])*

File `settings.cfg` stores configuration of basic server socket settings and also list of endpoints, that server needs to connect to.
<details>

<summary>You need to add your endpoints here.</summary>

> **Name** - unique name of endpoint  
> **Ip** - address of endpoint  
> **Port** - port on that endpoint is listening  
> **AuthKey** - key to authenticate connection to that endpoint

</details>

In `authkey` you only need to store your server password (in plain text), that will need be required for clients to connect.

Configuration of each endpoint is stored in `/etc/tmsd/epconf/` and is automatically generated by server on changes.
By default endpoint configuration is empty [struct](shared/include/endpointsettings.h).
Global endpoint configuration is stored in file named `[Global Settings].ep`.

> [!IMPORTANT]
> **Names of endpoint configurations are created from their names, so name definied in `settings.cfg` must be unique**

### Client
At this point client do not store any settings. Maybe in future it will be able to save IP, certificates etc.

### Endpoint
Endpoint configuration is stored in `/etc/tmse`.
Default settings are stored in def files in repo and are copied to desired location when installing.

Endpoint uses two files for it's configuration:
- [settings.cfg](endpoint/setting.cfg.def) - to store basic server socket settings
- [authkey](endpoint/authkey.def) - to store authentication key for server *(now it is stored in plain text - [read here](#security))* **NOW IS UNUSED - wait for update**

Endpoint do not store major configuration (meaning directory to lookup for images, etc.).
It is provided by server when connection occurs. It prevents endpoint from loading old configuration.
Endpoint waits for server and then will be able to display desired views.
This configuration is stored in [EndpointSettings](shared/include/endpointsettings.h) struct in memory.

## 🔐 Security
At this point of developement all connections are not secured (in plain text). Also passwords are stored in plain text.

It is planned to implement hashed passwords and SSL secured connections.
> [!CAUTION]
> Now this project probably doesn't meet any security standards.
> *(The project was created mainly for learning purposes - maybe in future will be improved)*

## 🛠 Contribution
**ToDo list:** [here](TODO.md)

> **SEGFAULT COUNTER**: 9
