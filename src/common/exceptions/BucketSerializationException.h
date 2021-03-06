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
 * @file        src/common/exceptions/BucketSerializationException.h
 * @author      Aleksander Zdyb <a.zdyb@samsung.com>
 * @version     1.0
 * @brief       Implementation of BucketSerializationException
 */
#ifndef SRC_COMMON_EXCEPTIONS_BUCKETSERIALIZATIONEXCEPTION_H_
#define SRC_COMMON_EXCEPTIONS_BUCKETSERIALIZATIONEXCEPTION_H_

#include <exceptions/DatabaseException.h>
#include <types/PolicyBucketId.h>

namespace Cynara {

class BucketSerializationException : public DatabaseException {
public:
    BucketSerializationException(const PolicyBucketId &bucket) : m_bucketId(bucket) {
        m_message = "Could not serialize bucket " + bucketId();
    }
    virtual ~BucketSerializationException() {};

    const std::string &message(void) const {
        return m_message;
    }

    const PolicyBucketId &bucketId(void) const {
        return m_bucketId;
    }

private:
    std::string m_message;
    PolicyBucketId m_bucketId;
};

} /* namespace Cynara */

#endif /* SRC_COMMON_EXCEPTIONS_BUCKETSERIALIZATIONEXCEPTION_H_ */
