
# EXASOL 6 R SDK 

This package provides connectivity and more, to work with an EXASOL database within R. The connection
to the server is implemented via [JSON over websockets](https://github.com/EXASOL/websocket-api).

## Architecture

The package integrates a C++ class via Rcpp module.

The underlying C++ libaries:

+ [websockets](https://www.github.com/marcelboldt/websockets)
+ [exasockets](https://www.github.com/marcelboldt/exasockets)
    - [rapidJSON](https://github.com/miloyip/rapidjson)
    - [openSSL](https://www.openssl.org/)


## Status

Functionality beyond the method ```exa.connect``` is not yet implemented. The C++ library compiles,
but the method fails. There is also the problem that Rcpp needs to be loaded.


### Windows

Build of the DLL successful on Windows with Rtools 3.4 (gcc 4.9.3 -i686-posix-dwarf, Built by MinGW-W64 project)
and Rcpp 0.12.9 - but the test ```con <- exa.connect("192.168.137.10", 8563, "R", "sys", "exasol")``` fails with:

```
Error in new_CppObject_xp(fields$.module, fields$.pointer, ...) : 
  c++ exception (unknown reason)
```


### Linux

For Linux, there is a configure.ac that processes a configure file, generating a src/Makevars
for a successful compile on R 3.3.2, Ubuntu 16.10 - but the connect test freezes.


