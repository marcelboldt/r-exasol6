/*
websockets.cpp and websockets.h
http://www.github.com/marcelboldt/EXASockets

 The MIT License (MIT)

Copyright (C) 2016 Marcel Boldt / EXASOL

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

Marcel Boldt <marcel.boldt@exasol.com>

*/

#ifndef EXASOCKETS_EXASOCKETS_H
#define EXASOCKETS_EXASOCKETS_H

#include "../websockets/websockets.h"
#include "osname.h"
#include "exaResultSet.h"

#include "../rapidjson/document.h"
#include "../rapidjson/writer.h"
#include "../rapidjson/stringbuffer.h"
#include "../rapidjson/allocators.h"
#include<iostream>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/bio.h>
#include <openssl/err.h>


 // gethostname, getlogin

#define __USE_POSIX

#include <limits.h> // HOST_NAME_MAX

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 16
#endif

#define DRV_NAME "EXASOCKETS 0.1"

class exasockets_connection {
public:
    //! Establishes a connection to EXASOL and returns an exasockets_connection object.
    /*!
     *
     * @param server  The IP address of the EXASOL node to connect to. E.g. '192.168.137.10'
     * @param port  The Port the EXASOL DB listens on. E.g. 8563
     * @param clientName    The name of the client, as it will be shown e.g. in the CLIENT column in EXA_(USER/ALL/DBA)_SESSIONS. E.g. the name of the frontend.
     * @param username  The EXASOL DB user name for authentification. E.g. 'sys'
     * @param password  The EXASOL user password. E.g. 'exasol'. Transfer is always RSA encrypted.
     * @param pwd_len   The password length. E.g. 6.
     * @param autocommit    If the DBMS is supposed to execute a COMMIT automatically after each statement. Default: true (recommended)
     * @param use_compression   If the result sets are to be transferred compressed. Default: false. Compression is not yet implemented on the client side.
     * @param sessionId     An ID for the session to be created in EXASOL. Per default a random session ID is created (recommended).
     * @return An exasockets_connection object containing an open connection.
     */
    exasockets_connection(const char *server,
                          uint16_t port,
                          const char *clientName,
                          const char *username,
                          const char *password,
                          int pwd_len,
                          bool autocommit = true,
                          bool use_compression = false,
                          uint64_t sessionId = 0 // 0 -> create randomly
    );

    virtual ~exasockets_connection();

    static int StringToExaDatatype(const char *str);

    static char *ExaDatatypeToString(const int type);
    
    //! Disconnect from EXASOL DB.
    /*! If the connection is still alive, a clean disconnect from EXASOL DB is performed.
     *
     * @param throw_message If true, a status message is thrown to stdout.
     * @return 0 in case of success; 1 if EXASOL refuses to disconnect; -1 if the connection had already been dead.
     */
    int disconnect(bool throw_message = true);

    int update_session_attributes();


    //! Sends an SQL statement to EXASOL and fetches the response.
    /*! Sends an SQL stmt and fetches the result set. If a result set handle is received instead of data, then this is returned as an int.
     * Otherwise 0 is returned and the resultSet is stored inside the connection object.
     *
     * @param sql The SQL statement, e.g. 'select * from schema1.tbl1'
     * @return A result handler to be used with fetch() if available, otherwise 0.
     */
    virtual exaResultSetHandler *exec_sql(char *sql);

    //! Fetches a result set.
    /*! Takes a result set handle ID and fetches a result set, to be stored in the object->data.
     *
     * @param resultSetHandle The id of the result set.
     * @param numRows Amount of rows to fetch. Default: 10 000
     * @param startPosition  row offset from which to begin data retrieval (first row of the table: row 1). Default: 1
     * @param numBytes Number of bytes to retrieve. Default: 10 485 760 (10 MB)
     * @return the number of rows fetched.
     */
    int64_t
    fetch(exaResultSetHandler *rs, int resultSetHandle, uint64_t numRows = 10000, uint64_t startPosition = 1,
          uint64_t numBytes = 10485760);

    const char *databaseName() const;
    const char *identifierQuoteString() const;
    int maxDataMessageSize() const;
    int maxIdentifierLength() const;
    int maxVarcharLength() const;
    const char *productName() const;
    int protocolVersion() const;
    const char *releaseVersion() const;
    uint64_t session_id() const;
    const char *timezone() const;
    const char *timeZoneBehaviour() const;
    bool isStatus() const;

    rapidjson::Document resultSet;
    rapidjson::Value *data; // TODO: remove this
    bool json_debug_output = true; // set to true for cmd line output of all JSON elements


protected:
    static void append_data_from_Rapid_JSON_Document(exaResultSetHandler *rs, const rapidjson::Value &JSONdata);
    static exaResultSetHandler *create_exaResultSetHandler_from_RapidJSON_Document(const rapidjson::Value &JSONresultSet);
    
    Websockets_connection *ws_con;
    rapidjson::Document d; // for fetch() - the result set containing a dataset
    std::ifstream tfile;
    char *logfile = nullptr; // if nullptr, then the tempfile will be names "EXASockets %timestamp% %random_number%.tmp"

    const char *c_databaseName;
    const char *c_identifierQuoteString;
    int c_maxDataMessageSize;
    int c_maxIdentifierLength;
    int c_maxVarcharLength;
    const char *c_productName;
    int c_protocolVersion;
    const char *c_releaseVersion;
    uint64_t c_session_id;
    const char *c_timezone;
    const char *c_timeZoneBehaviour; // change to predef
    bool status;


    int ws_send_data(const char *data, int len = -1, int type = 1);

    char *ws_receive_data();


};

#endif //EXASOCKETS_EXASOCKETS_H
