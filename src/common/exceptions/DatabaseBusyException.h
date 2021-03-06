/*
 * Copyright (c) 2014 Samsung Electronics Co., Ltd All Rights Reserved
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */
/**
 * @file        src/common/exceptions/DatabaseBusyException.h
 * @author      Aleksander Zdyb <a.zdyb@samsung.com>
 * @version     1.0
 * @brief       This file defines exception thrown when database is busy
 */

#ifndef SRC_COMMON_EXCEPTIONS_DATABASEBUSYEXCEPTION_H_
#define SRC_COMMON_EXCEPTIONS_DATABASEBUSYEXCEPTION_H_

#include <string>

#include <exceptions/DatabaseException.h>

namespace Cynara {

class DatabaseBusyException : public DatabaseException {
public:
    DatabaseBusyException() : m_message("Database busy") {};
    virtual ~DatabaseBusyException() {};

    const std::string &message(void) const {
        return m_message;
    }

private:
    std::string m_message;
};

} /* namespace Cynara */

#endif /* SRC_COMMON_EXCEPTIONS_DATABASEBUSYEXCEPTION_H_ */
