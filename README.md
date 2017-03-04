
# EXASOL 6 R SDK 

This package provides connectivity and more, to work with an EXASOL database within R. The connection
to the server is implemented via [JSON over websockets](https://github.com/EXASOL/websocket-api).

## Status

It is a prototype, offering the functionality of connecting to EXASOL 6, and executing a query. The result set 
is then returned as a list of vectors, that can be converted to a data.frame within R. Further functionality is not yet implemented, in particular writing is not possible on the protocol side (beyond single-row inserts using SQL (prepared) INSERT statements).

The build succeeds only on Linux, there is also the problem that the Rcpp package needs to be loaded for the package to work.

Apart from the advantage that the approach is potentially independent from further drivers or libraries (ODBC,...), the
performance is far superiour for small operations: connect to a local EXASOL VM, or fetching a single line result set take each ca. 20 ms. Fetching large result sets is well possible, but slower than using the ODBC-based R package for EXASOL 5, which fetches result sets via bulk HTTP transfer. That way it takes 6-7 secs. to fetch the whole nycflights dataset from EXASOL (ca. 375k rows), while the JSON over Websockets appraoch currently needs 18 sec. There may be room for improvement on the client side, but for huge result set this appraochs seems less favourable. 

As a consequence, options to combine Websockets communication with HTTP data transfer may be explored.


### Windows

Build of the DLL successful on Windows with Rtools 3.4 (gcc 4.9.3 -i686-posix-dwarf, Built by MinGW-W64 project)
and Rcpp 0.12.9 - but the test ```con <- exa.connect("192.168.137.10", 8563, "R", "sys", "exasol")``` fails with:

```
Error in new_CppObject_xp(fields$.module, fields$.pointer, ...) : 
  c++ exception (unknown reason)
```

### Linux

For Linux, there is a configure.ac that processes a configure file, generating a src/Makevars
for a successful compile on R 3.3.2, Ubuntu 16.10. The connect works now, and exec_sql returns a list that can be
converted to a data.frame on the R level.

Problem: 
* OSTools::getlogin(); causes freeze, so curently commented out in exasockets.cpp line 89

## Use

For now:

```
library(Rcpp)
library(exasol6)

con <- exa.connect("192.168.1.10", 8563, "R",  "sys", "exasol")
con$exec_sql("SELECT CURRENT_TIMESTAMP")
```

## Architecture

The package integrates a C++ class via Rcpp module.

The underlying C++ libaries:

+ [websockets](https://www.github.com/marcelboldt/websockets)
+ [exasockets](https://www.github.com/marcelboldt/exasockets)
    - [rapidJSON](https://github.com/miloyip/rapidjson)
    - [openSSL](https://www.openssl.org/)

