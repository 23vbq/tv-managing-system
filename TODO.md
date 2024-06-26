# ToDo
## Main
- [ ] Description (Readme)
- [ ] Documentation
- [ ] Server
  - [x] Configuration
    - [x] Loading configuration
    - [x] Storing configuration
  - [ ] Socket
    - [x] Creating socket
    - [x] Making connection
    - [x] Multiple connections
    - [ ] Cleanup code
    - [x] Callback function for every read
  - [ ] Command handling
    - [x] System for handling
    - [ ] Real functions to use
    - [ ] Cleanup and testing
  - [ ] Endpoint settings
    - [x] Struct
    - [x] Loading from file
    - [ ] Creating default file
    - [x] Sending through socket
    - [x] Saving settings into file
  - [ ] Endpoint manager - in progress
  - [ ] Requests handling
  - [ ] Makefile
    - [ ] Create config path
    - [ ] Copy default config to config path
- [ ] Client
  - [ ] **Rewrite to Qt**
  - [ ] Socket
    - [x] Creating socket
    - [x] Connecting
    - [x] Send and Receive
    - [ ] Cleanup, improvements and testing
  - [ ] Getting data
    - [x] List of endpoints
    - [x] Endpoint settings
    - [ ] Is endpoint connected
    - [x] Auto load endpoints list after connection
    - [ ] Move reload button to top menu
  - [ ] GUI
    - [x] Connection window
    - [x] Showing list of endpoints
    - [ ] Showing endpoint settings
  - [ ] Sending data
- [ ] Endpoint
  - [ ] X11
  - [ ] Wait mode on no connection
- [ ] Automatic sync

## Authentication
- [ ] Basic auth for clients to server
- [ ] Basic auth for server to endpoints
- [ ] SSL for encrypting traffic
- [ ] More secure way to store client password on server side
- [ ] More secure way for connection server-endpoint
