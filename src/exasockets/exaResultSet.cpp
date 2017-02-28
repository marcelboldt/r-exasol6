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

#include "exaResultSet.h"

exaTblColumn *
exaTblColumn::create(char *name, int datatype, int precision, int scale, int size, char *charset, bool w_local_tz,
                     int fraction, int srid) {

    switch (datatype) {
        case EXA_BOOLEAN :
            return new exaColumn<bool>(name, EXA_BOOLEAN);
        case EXA_CHAR :
            return new exaColumn<std::string>(name, EXA_CHAR);
        case EXA_DATE :
            return new exaColumn<std::string>(name, EXA_DATE);
        case EXA_DECIMAL :
            return new exaColumn<std::string>(name, EXA_DECIMAL);
        case EXA_DOUBLE :
            return new exaColumn<double>(name, EXA_DOUBLE);
            /*  case EXA_GEOMETRY :
                  this->data = new exaColumn<char[20]>();
                  break; */
            /*   case EXA_INTERVAL_DS :
                   this->data = new exaColumn<char[20]>();
                   break; */
            /*   case EXA_INTERVAL_YM :
                   this->data = new exaColumn<char[20]>();
                   break; */
        case EXA_TIMESTAMP :
            return new exaColumn<std::string>(name, EXA_TIMESTAMP);
            /*    case EXA_TIMESTAMP_TZ :
                    this->data = new exaColumn<char[20]>();
                    break; */
        case EXA_VARCHAR :
            return new exaColumn<std::string>(name, EXA_VARCHAR);
        default:
            throw "unknown datatype";
    }
}


bool exaTblColumn::is_null(size_t rowno) {
    return this->nulls[rowno];
}

int exaTblColumn::getPrecision() const {
    return precision;
}

void exaTblColumn::setPrecision(int precision) {
    exaTblColumn::precision = precision;
}

int exaTblColumn::getScale() const {
    return scale;
}

void exaTblColumn::setScale(int scale) {
    exaTblColumn::scale = scale;
}

int exaTblColumn::getSize() const {
    return size;
}

void exaTblColumn::setSize(int size) {
    exaTblColumn::size = size;
}

const char *exaTblColumn::getCharacterSet() const {
    return characterSet;
}

/*
void exaTblColumn::setCharacterSet(char* characterSet) {
    exaTblColumn::characterSet = characterSet;
}*/

bool exaTblColumn::isWithLocalTimeTone() const {
    return withLocalTimeTone;
}

void exaTblColumn::setWithLocalTimeTone(bool withLocalTimeTone) {
    exaTblColumn::withLocalTimeTone = withLocalTimeTone;
}

int exaTblColumn::getFraction() const {
    return fraction;
}

void exaTblColumn::setFraction(int fraction) {
    exaTblColumn::fraction = fraction;
}

int exaTblColumn::getSrid() const {
    return srid;
}

void exaTblColumn::setSrid(int srid) {
    exaTblColumn::srid = srid;
}


template <typename T>
size_t exaColumn<T>::count() const  {
    return this->data.size();
}


template<typename T>
void exaColumn<T>::appendData(const void *value, const bool null) {
    if (value == nullptr) {
        data.push_back(*new T);
    } else {
        this->data.push_back(*static_cast<const T *>(value));
    }
    try {
        this->nulls.push_back(null);
    } catch (...) {
        this->data.pop_back();
        throw std::runtime_error("Failed to insert null-value in column");
    }
}

template<typename T>
void *exaColumn<T>::operator[](size_t row) {
    if (this->is_null(row)) return nullptr;
    return new T(this->data[row]);
}

/*
template<typename T>
int32_t exaColumn<T>::intVal(size_t row) {
    return this->data[row];
}
 */


void exaResultSetHandler::addColumn(exaTblColumn *c) {

    std::shared_ptr<exaTblColumn> c_sh(c);
    this->columns.push_back(c_sh);
}

void exaResultSetHandler::addColumn(std::shared_ptr<exaTblColumn> c) {

    this->columns.push_back(c);
}

size_t exaResultSetHandler::cols() {
    return this->columns.size();
}

size_t exaResultSetHandler::rows() {
    if (this->columns.size() == 0) return 0;
    return this->columns[0]->count();
}

exaTblColumn &exaResultSetHandler::operator[](size_t col) {
    return *this->columns[col];
}



