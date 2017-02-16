/*
exaResultSet.h and exaResultSet.cpp
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


#ifndef EXASOCKETS_EXARESULTSET_H
#define EXASOCKETS_EXARESULTSET_H

#define EXA_BOOLEAN 301
#define EXA_CHAR 302
#define EXA_DATE 303
#define EXA_DECIMAL 304
#define EXA_DOUBLE 305
#define EXA_GEOMETRY 306
#define EXA_INTERVAL_DS 307
#define EXA_INTERVAL_YM 308
#define EXA_TIMESTAMP 309
#define EXA_TIMESTAMP_TZ 310
#define EXA_VARCHAR 311

#include <cstdint>
#include <vector>
#include <memory>
#include <cstddef>
#include <cstring>
#include <string>
#include <iostream>
#include <ctime>


class exaTblColumn {
public:
    static exaTblColumn *create(char *name, int datatype,
                                int precision = 0, int scale = 0, int size = 0, char *charset = "",
                                bool w_local_tz = false,
                                int fraction = 0, int srid = 0);

    exaTblColumn(char *name, int datatype, int precision = 0, int scale = 0, int size = 0, char *charset = "",
                 bool w_local_tz = false, int fraction = 0, int srid = 0) {};

    virtual ~exaTblColumn() {};

    //! Returns a void pointer to the value, or a nullptr if the value is NULL.
    /*!
     *
     * @param position A 64 Bit Integer specifying the row position, from 0.
     * @return A void pointer to the value that may then be static_casted. If the actual value is NULL, a nullptr is returned.
     */
    virtual void *operator[](size_t row) = 0;
    
    //! Returns a void pointer to the embedded std::vector
    /*!
    *
    * @return A void pointer to the std::vector.
    */
    virtual void *as_std_vector() = 0;

    //! Returns the value as a 32 Bit integer.
    /*!
     *
     * @param position The (row-)position index within the column, starting from 0.
     * @return A 32 Bit integer, which is undefined if the value is NULL.
     */
//    virtual int32_t intVal(size_t row) = 0;

    //! Appends a data object to the column.
    /*!
     *
     * @param value A void pointer to the data object.
     * @param numRows A boolean setting the NULL flag for this object, i.e. indicating if the value is NULL. A value
     *  that is valid towards the data type must nevertheless be given (alternatively a nullptr).
     */
    virtual void appendData(const void *value, bool null = false) = 0;

    //! Returns a boolean on whether the row specified contains a NULL value.
    virtual bool is_null(size_t rowno);

    //! Returns the number of rows contained in the column.
    virtual size_t count() const = 0;

    int type() {
        return datatype;
    }

    char *getName() const {
        return name;
    }

    void setName(char *name) {
        exaTblColumn::name = name;
    }

    int getPrecision() const;

    void setPrecision(int precision);

    int getScale() const;

    void setScale(int scale);

    int getSize() const;

    void setSize(int size);

    const char *getCharacterSet() const;

    void setCharacterSet(const char *characterSet);

    bool isWithLocalTimeTone() const;

    void setWithLocalTimeTone(bool withLocalTimeTone);

    int getFraction() const;

    void setFraction(int fraction);

    int getSrid() const;

    void setSrid(int srid);

protected:
    char* name;
    int datatype;
    int precision = 0;
    int scale = 0;
    int size = 0;
    char *characterSet = "";
    bool withLocalTimeTone = 0;
    int fraction = 0;
    int srid = 0;

    std::vector<bool> nulls;
};

template<typename T>
class exaColumn : public exaTblColumn {
public:
    exaColumn(char *name, int datatype, int precision = 0, int scale = 0, int size = 0, char *charset = "",
              bool w_local_tz = false,
              int fraction = 0, int srid = 0) : exaTblColumn(name, datatype, precision,
                                                             scale, size, charset, w_local_tz, fraction, srid) {
        this->name = name;
        this->datatype = datatype;
        this->precision = precision;
        this->scale = scale;
        this->size = size;
        this->characterSet = charset;
        this->withLocalTimeTone = w_local_tz;
        this->fraction = fraction;
        this->srid = srid;

    };
    // exaColumn(std::vector<T> *data);
    // exaColumn(char *data);
    // ~exaColumn();

    void *operator[](size_t row);

    //   int32_t intVal(size_t row);

    void appendData(const void *value, const bool null = false);

    size_t count() const;
    
    void *as_std_vector() {
      // returns a pointer to the vector
      return &this->data;
    }

protected:
    std::vector<T> data;
};


class exaResultSetHandler {
public:
    //  exaResultSetHandler();
    //  ~exaResultSetHandler();

    //! Add an exaTblColumn.
    void addColumn(exaTblColumn *c);

    //! Add an exaTblColumn.
    void addColumn(std::shared_ptr<exaTblColumn> c);

    // int removeColumn();

    //! Returns the number of columns contained in the result set.
    size_t cols();

    //! Returns the number of rows contained in the result set.
    size_t rows();

    exaTblColumn &operator[](size_t col);

    //   int getRows(int start_row_no, int stop_row_no = -1);
    //   int fetchRows(size_t limit, size_t offset);

    std::vector<std::shared_ptr<exaTblColumn>> &getColumns() {
        return this->columns;
    }

protected:
    std::vector<std::shared_ptr<exaTblColumn>> columns;
    int handle;
};


#endif //EXASOCKETS_EXARESULTSET_H
