/*
 *  Copyright (c) 2014 Samsung Electronics Co., Ltd All Rights Reserved
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License
 */
/**
 * @file        src/client-async/check/CheckData.h
 * @author      Marcin Niesluchowski <m.niesluchow@samsung.com>
 * @version     1.0
 * @brief       This file contains CheckData class for storing
 *              information about asynchronous check request.
 */

#ifndef SRC_CLIENT_ASYNC_CHECK_CHECKDATA_H_
#define SRC_CLIENT_ASYNC_CHECK_CHECKDATA_H_

#include <memory>
#include <string>

#include <types/PolicyKey.h>

#include <callback/ResponseCallback.h>

namespace Cynara {

class CheckData
{
public:
    CheckData(const PolicyKey &key, const std::string &session, const ResponseCallback &callback,
              bool simple)
        : m_key(key), m_session(session), m_callback(callback), m_simple(simple), m_cancelled(false)
    {}
    CheckData(CheckData &&other)
        : m_key(std::move(other.m_key)), m_session(std::move(other.m_session)),
          m_callback(std::move(other.m_callback)), m_simple(other.m_simple),
          m_cancelled(other.m_cancelled) {
        other.m_cancelled = false;
    }
    ~CheckData() {}

    const PolicyKey &key(void) const {
        return m_key;
    }

    const std::string &session(void) const {
        return m_session;
    }

    const ResponseCallback &callback(void) const {
        return m_callback;
    }

    bool isSimple(void) const {
        return m_simple;
    }

    bool cancelled(void) const {
        return m_cancelled;
    }

    void cancel(void) {
        m_cancelled = true;
    }

private:
    PolicyKey m_key;
    std::string m_session;
    ResponseCallback m_callback;
    bool m_simple;
    bool m_cancelled;
};

} // namespace Cynara

#endif // SRC_CLIENT_ASYNC_CHECK_CHECKDATA_H_

