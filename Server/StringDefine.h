#pragma once

// config file name
#define CONFIG_FILE_NAME ("config.cfg")

// server type
#define STR_MASTER ("master")
#define STR_CONNECTOR ("connector")
#define STR_SERVER ("server")
#define STR_LOGIN ("login")

// plugin
#define MASTER_PLUGIN ("MasterPlugin")
#define LOGIN_PLUGIN ("LoginPlugin")
#define CONFIG_PLUGIN ("ConfigPlugin")
#define ACTOR_PLUGIN ("ActorPlugin")
#define SERVER_NET_PLUGIN ("ServerNetPlugin")

// server error

// config
#define CAN_NOT_READ_CONFIG_FILE ("cannot read config file")
#define CONFIG_DUPLICATE_SERVER_NAME ("duplicate server names in the configuration file")

// net
#define CAN_NOT_CONNECT_MASTER ("cannot connect to master server")
#define CAN_NOT_CONNECT_OTHER_SERVER ("cannot connect to other server")

// package
#define HTTP_PACKAGE_ERROR ("http package error")

// path
#define CANNOT_GET_REAL_PATH ("cannot get real path")

// server log
#define IS_RUNNING (" is running")
#define IS_ONLINE (" is online")
#define CONNECT_TO_MASTER_SERVER ("connect to master server")
#define CONNECT_SERVER ("connect server: ")
#define BACKEND_PROCESS_ERROR ("backend process error")