
// see http://dirk.eddelbuettel.com/code/rcpp/Rcpp-modules.pdf p. 8

#include <stdlib.h> // atof()
#include <Rcpp.h>

#undef Realloc // Realloc & Free get overwritte by R.h; see http://stackoverflow.com/questions/11588765/using-rcpp-with-windows-specific-includes
#undef Free

#include "exasockets/exasockets.h"
using namespace Rcpp;


class R_exa_ws_connection : public exasockets_connection {

public:
  R_exa_ws_connection(std::string server,
                      uint16_t port,
                      std::string clientName,
                      std::string username,
                      std::string password,
//                      int pwd_len,
                      bool autocommit = true,
                      bool use_compression = false,
                      uint64_t sessionId = 0)
    : exasockets_connection(server.c_str(),
      port,
      clientName.c_str(),
      username.c_str(),
      password.c_str(),
      password.length(), // Rcpp can only deal with max 6 parameters for constructores, see line 390
      autocommit,
      use_compression,
      sessionId) {};


#define FILL_R_CHARACTER_VECTOR(vector, startrow, VALUE)                \
    size_t row = startrow;                                              \
    for (auto &item : VALUE) {                                          \
        if (item.IsNull()) {                                            \
            vector = NA_STRING;                                    \
        } else {                                                        \
            vector =item.GetString();                              \
        }                                                               \
        row++;                                                          \
    }

#define FILL_R_NUMERIC_VECTOR(vector, startrow, VALUE)                              \
size_t row = startrow;                                                              \
for (auto &item : VALUE) {                                                          \
    if (item.IsNull()) {                                                            \
        vector = NA_REAL;                                                      \
    } else {                                                                        \
        vector = item.GetDouble();                                             \
    }                                                                               \
    row++;                                                                          \
}

#define FILL_R_BOOLEAN_VECTOR(vector, startrow, VALUE)                   \
size_t row = startrow;                                                   \
for (auto &item : VALUE) {                                               \
    if (item.IsNull()) {                                                 \
        vector = NA_LOGICAL;                                        \
    } else {                                                             \
        vector = item.GetBool();                                    \
    }                                                                    \
    row++;                                                               \
}


  static Rcpp::List create_df_from_RapidJSON_Document(const rapidjson::Value& JSONresultSet) {
    // will initialise the R List and insert data if contained in the JSON resultset (if no handle is present)
    // see http://stackoverflow.com/questions/8631197/constructing-a-data-frame-in-rcpp
    // TODO: date & timestamp values are currently returned as strings

    Rcpp::List cols(JSONresultSet["numColumns"].GetUint64());
    Rcpp::CharacterVector namevec;
    const size_t nrows = JSONresultSet["numRows"].GetUint64();


    bool hasHandle = JSONresultSet.HasMember("resultSetHandle");


    for (auto i = 0; i < cols.length(); i++) { //for each column
      namevec.push_back(JSONresultSet["columns"][i]["name"].GetString());

      switch (StringToExaDatatype(JSONresultSet["columns"][i]["dataType"]["type"].GetString())) {
      case EXA_BOOLEAN : {
        Rcpp::LogicalVector v(nrows);
        if (!hasHandle) {
          FILL_R_BOOLEAN_VECTOR(v(row), 0, JSONresultSet["data"][i].GetArray())
        }
        cols[i] = v;
        break;
      }
        case EXA_CHAR : {
          Rcpp::CharacterVector v(nrows);
          if (!hasHandle) {
            FILL_R_CHARACTER_VECTOR(v(row), 0, JSONresultSet["data"][i].GetArray())
          }
          cols[i] = v;
          break;
        }
        case EXA_DATE : { // TODO: use adequate R data type
          Rcpp::CharacterVector v(nrows);
          if (!hasHandle) {
          FILL_R_CHARACTER_VECTOR(v(row), 0, JSONresultSet["data"][i].GetArray())
          }
          cols[i] = v;
          break;
        }
      case EXA_DECIMAL : {
        Rcpp::NumericVector v(nrows); //TODO: use R integer vector if possible
        if (!hasHandle) {
          size_t row = 0;
          for (auto &item : JSONresultSet["data"][i].GetArray()) {
            if (item.IsNull()) {
              v[row] = NA_REAL;
            } else { // EXASOL sends whole numbers as int, but numbers with a comma as strings to keep C exactness.
              if (item.IsString()) {
                v[row] = atof(item.GetString());
              } else if (item.IsInt()) {
                v[row] = item.GetDouble();
              }
            }
          }
        }
        cols[i] = v;
        break;
      }
        case EXA_DOUBLE : {
          Rcpp::NumericVector v(nrows);
          if (!hasHandle) {
            FILL_R_NUMERIC_VECTOR(v(row), 0, JSONresultSet["data"][i].GetArray())
          }
          cols[i] = v;
          break;
        }
        case EXA_TIMESTAMP : { // TODO: use adequate timestamp type
          Rcpp::CharacterVector v(nrows);
          if (!hasHandle) {
          FILL_R_CHARACTER_VECTOR(v(row), 0, JSONresultSet["data"][i].GetArray())
          }
          cols[i] = v;
          break;
        }
        case EXA_VARCHAR : {
          Rcpp::CharacterVector v(nrows);
          if (!hasHandle) {
          FILL_R_CHARACTER_VECTOR(v(row), 0, JSONresultSet["data"][i].GetArray())
          }
          cols[i] = v;
          break;
        }
        } // end of switch
    } // end of for

    cols.attr("names") = namevec;
   // Rcpp::DataFrame df(cols);
    return cols;
  };


  static void append_data_from_Rapid_JSON_Document_to_df(Rcpp::List& cols, size_t start_pos, const rapidjson::Value& JSONresultSet, const rapidjson::Value& JSONdata) {

    for (auto i = 0; i < cols.length(); i++) { //for each column

 //     const rapidjson::Value& JSONdata  = JSONresultSet["data"];
      switch (StringToExaDatatype(JSONresultSet["columns"][i]["dataType"]["type"].GetString())) {
      case EXA_BOOLEAN : {
        Rcpp::LogicalVector v = cols[i];
        FILL_R_BOOLEAN_VECTOR(v[row], start_pos, JSONdata[i].GetArray())
        break;
      }
      case EXA_CHAR : {
          Rcpp::CharacterVector v = cols[i];
        FILL_R_CHARACTER_VECTOR(v[row], start_pos, JSONdata[i].GetArray())
          break;
      }
      case EXA_DATE : {
          Rcpp::CharacterVector v = cols[i];
        FILL_R_CHARACTER_VECTOR(v[row], start_pos, JSONdata[i].GetArray())
        break;
      }
      case EXA_DECIMAL : {
          Rcpp::NumericVector v = cols[i];
          size_t row = start_pos;
          for (auto &item : JSONdata[i].GetArray()) {
            if (item.IsNull()) {
              v[row] = NA_REAL;
            } else { // EXASOL sends whole numbers as int, but numbers with a comma as strings to keep C exactness.
              // Since in R everything is a double...
              if (item.IsString()) {
                v[row] = atof(item.GetString());
              } else if (item.IsInt()) {
                v[row] = item.GetDouble();
              }
            }
            row++;
          }
          cols[i] = v;
          break;
      }
      case EXA_DOUBLE : {
          Rcpp::NumericVector v = cols[i];
        FILL_R_NUMERIC_VECTOR(v[row], start_pos, JSONdata[i].GetArray())
          break;
      }
      case EXA_TIMESTAMP : {
          Rcpp::CharacterVector v = cols[i];
        FILL_R_CHARACTER_VECTOR(v[row], start_pos, JSONdata[i].GetArray())
        break;
      }
      case EXA_VARCHAR : {
          Rcpp::CharacterVector v = cols[i];
        FILL_R_CHARACTER_VECTOR(v[row], start_pos, JSONdata[i].GetArray())
        break;
      }

      } // end of switch
    } // end of for
  };


  Rcpp::List R_exec_sql(std::string R_sql) {
    // executes the SQL and returns a dataframe (actually a list which must be converted to df at R level)
    //

BEGIN_RCPP
    const char* sql = R_sql.c_str();

    Rcpp::List df;

    rapidjson::StringBuffer s;
    rapidjson::Writer<rapidjson::StringBuffer> writer(s);

    writer.StartObject();
    writer.Key("command");
    writer.String("execute");
    writer.Key("sqlText");
    writer.String(sql);
    writer.EndObject();
    this->ws_send_data(s.GetString(), s.GetSize(), 1);

    // std::this_thread::sleep_for(std::chrono::milliseconds(3000));

    this->resultSet.Parse(this->ws_receive_data());

    if (!this->resultSet.IsObject()) { // nonsense received
      Rcpp::Rcout << "exec_sql: Response parsing failed." << std::endl;
      throw std::runtime_error("exec_sql: Response parsing failed.");
    } else if (this->resultSet.HasMember("exception")) { // DB had a problem
      Rcpp::Rcout << "DB error: " << this->resultSet["exception"]["text"].GetString() << std::endl;

    } else { // something useful received
      if (this->resultSet["responseData"].HasMember("results")) { // a result received
          Rcpp::Rcout << "result set received" << std::endl;
        const rapidjson::Value &jrs = this->resultSet["responseData"]["results"][0]["resultSet"];
        df = this->create_df_from_RapidJSON_Document(jrs);


        if (this->resultSet["responseData"]["results"][0]["resultSet"].HasMember("resultSetHandle")) {
          // result contains a handle... separate fetching needed
          //  call fetch(), then return the exaResultSet composed
          int h = this->resultSet["responseData"]["results"][0]["resultSet"]["resultSetHandle"].GetInt();
            Rcpp::Rcout << "contains handle: " << h << std::endl;

          this->R_fetch(df, h, 0, 1, (10485760));


        } else {// resultset without a handle received
          // add data to exaResultSet and return it. TODO: Dispose the JSON resultSet afterwards.
          //const rapidjson::Value &data = this->resultSet["responseData"]["results"][0]["resultSet"]["data"];
          //append_data_from_Rapid_JSON_Document_to_df(df, jrs, data);
        }
      }
    }
    return df;
END_RCPP
  };

  int64_t R_fetch(Rcpp::List& rs, int resultSetHandle, uint64_t numRows, uint64_t startPosition,
                                 uint64_t numBytes) {

      // recursively fetches until the number of rows are all transferred
      // stores the json containing the dataset in d, while resultSet keeps the initial JSON containing the handle.
      // also sets the pointer data to the dataset contained in d.

      // std::cout << "Fetch start from row" << startPosition << std::endl;

      rapidjson::StringBuffer s;
      rapidjson::Writer<rapidjson::StringBuffer> writer(s);
      rapidjson::Document d0, d1;  // TODO: remove d0, it is not used.

      uint64_t nr2, nr_here;

      const rapidjson::Value& jrs = this->resultSet["responseData"]["results"][0]["resultSet"];

      if (numRows == 0) {
        numRows = this->resultSet["responseData"]["results"][0]["resultSet"]["numRows"].GetInt();
      }

      writer.StartObject();
      writer.Key("command");
      writer.String("fetch");
      writer.Key("resultSetHandle");
      writer.Int(resultSetHandle);
      writer.Key("startPosition");
      writer.Int64(startPosition - 1);
      writer.Key("numBytes");
      writer.Int64(numBytes);
      writer.EndObject();


      ws_send_data(s.GetString(), s.GetSize(), 1);
      this->d.Parse(ws_receive_data());

      if (!this->d.IsObject()) { // nonsense received
        throw "update_session_attributes: Response parsing failed.";
        return -2;
      } else if (this->d.HasMember("exception")) { // DB had a problem
        std::cout << this->d["exception"]["text"].GetString() << std::endl;
        return -1;
      } else { // something useful received
        if (this->d.HasMember("responseData")) { // a resultset received
          //assert(this->d["responseData"].HasMember("data"));
          this->data = &this->d["responseData"]["data"];

          // append the data to rs
          append_data_from_Rapid_JSON_Document_to_df(rs, startPosition-1, jrs, *this->data);

          nr_here = this->d["responseData"]["numRows"].GetUint64();

          if (nr_here < numRows) { // if not completely fetched...
            nr2 = R_fetch(rs, resultSetHandle, numRows - nr_here, startPosition + nr_here,
                        numBytes); // fetch again, overwriting d
            nr_here += nr2;
          }
          return nr_here;
        }
      }
      return -3;
    };

};

RCPP_MODULE(exa_wscon_module) {

//    Rcpp::Rcout << "This is the module" << std::endl;

  class_<R_exa_ws_connection>("R_exa_ws_connection")
  .constructor<const char *, uint16_t, const char *, const char *, const char *>()
  .method("exec_sql", &R_exa_ws_connection::R_exec_sql);
}
