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
 * @file        src/cyad/CommandlineParser/PolicyParsingException.h
 * @author      Aleksander Zdyb <a.zdyb@samsung.com>
 * @version     1.0
 * @brief       PolicyParsingException
 */

#ifndef SRC_CYAD_COMMANDLINEPARSER_POLICYPARSINGEXCEPTION_H_
#define SRC_CYAD_COMMANDLINEPARSER_POLICYPARSINGEXCEPTION_H_

#include <exceptions/Exception.h>

namespace Cynara {

class PolicyParsingException : public Exception {
public:
    PolicyParsingException() {
        m_message = "Invalid policy";
    }

    virtual const std::string &message(void) const {
        return m_message;
    }

private:
    std::string m_message;
};

} /* namespace Cynara */

#endif /* SRC_CYAD_COMMANDLINEPARSER_POLICYPARSINGEXCEPTION_H_ */
