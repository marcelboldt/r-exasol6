
# EXASOL 6 R SDK 

This package provides connectivity and more, to work with an EXASOL database within R. The connection
to the server is implemented via JSON over websockets.

# Architecture

The package integrates a C++ class via Rcpp module.

The underlying C++ libaries:

+ [websockets](https://www.github.com/marcelboldt/websockets)
+ [exasockets](https://www.github.com/marcelboldt/exasockets)
    - [rapidJSON](https://github.com/miloyip/rapidjson)
    - [openSSL](https://www.openssl.org/)


# Status

Build of the DLL successful on Windows with Rtools 3.4 (gcc 4.9.3 -i686-posix-dwarf, Built by MinGW-W64 project)
and Rcpp 0.12.9 - but the test 


```R

con <- exa.connect("192.168.137.10", 8563, "R", "sys", "exasol")

```

fails with

Error in new_CppObject_xp(fields$.module, fields$.pointer, ...) : 
  c++ exception (unknown reason)

For Linux, the makevars file is missing.

R functionality beyond the method ```exa.connect``` are not yet implemented. 
