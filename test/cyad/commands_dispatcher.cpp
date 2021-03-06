/*
 * Copyright (c) 2014-2015 Samsung Electronics Co., Ltd All Rights Reserved
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
 * @file        test/cyad/commands_dispatcher.cpp
 * @author      Aleksander Zdyb <a.zdyb@samsung.com>
 * @version     1.0
 * @brief       Tests for CommandsDispatcher
 */

#include <cstring>
#include <ostream>
#include <tuple>
#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cynara-admin-types.h>
#include <cynara-error.h>
#include <cynara-policy-types.h>

#include <common/types/PolicyKey.h>
#include <common/types/PolicyResult.h>
#include <common/types/PolicyType.h>
#include <cyad/CynaraAdminPolicies.h>
#include <cyad/CommandlineParser/CyadCommand.h>
#include <cyad/CommandsDispatcher.h>

#include "CyadCommandlineDispatcherTest.h"
#include "helpers.h"

/**
 * @brief   Dispatcher should not touch admin API on help or error
 * @test    Scenario:
 * - Prepare some parsing results not requiring API calls
 * - Check if no API calls were made
 */
TEST_F(CyadCommandlineDispatcherTest, noApi) {
    using ::testing::_;
    using ::testing::Return;

    Cynara::CommandsDispatcher dispatcher(m_io, m_adminApi, m_errorApi);

    Cynara::CyadCommand result;
    Cynara::HelpCyadCommand helpResult;
    Cynara::ErrorCyadCommand errorResult("Fake error");

    dispatcher.execute(result);
    dispatcher.execute(helpResult);
    dispatcher.execute(errorResult);
}

TEST_F(CyadCommandlineDispatcherTest, deleteBucket) {
    using ::testing::_;
    using ::testing::Return;
    using ::testing::StrEq;
    using ::testing::IsNull;

    Cynara::CommandsDispatcher dispatcher(m_io, m_adminApi, m_errorApi);
    Cynara::DeleteBucketCyadCommand result("test-bucket");

    EXPECT_CALL(m_adminApi,
            cynara_admin_set_bucket(_, StrEq("test-bucket"), CYNARA_ADMIN_DELETE, IsNull()))
        .WillOnce(Return(CYNARA_API_SUCCESS));

    dispatcher.execute(result);
}

TEST_F(CyadCommandlineDispatcherTest, setBucket) {
    using ::testing::_;
    using ::testing::Return;
    using ::testing::StrEq;
    using ::testing::IsNull;
    using ::testing::NotNull;
    using Cynara::PolicyBucketId;
    using Cynara::PolicyType;
    using Cynara::PolicyResult;

    addDescriptions({ { 42, "hitchhiker" } });

    Cynara::CommandsDispatcher dispatcher(m_io, m_adminApi, m_errorApi);

    typedef std::string RawPolicyType;
    typedef std::string Metadata;
    typedef std::tuple<PolicyBucketId, PolicyType, RawPolicyType, Metadata> BucketData;
    typedef std::vector<BucketData> Buckets;
    const Buckets buckets = {
        BucketData("test-bucket-1", CYNARA_ADMIN_ALLOW, "ALLOW", ""),
        BucketData("test-bucket-2", CYNARA_ADMIN_DENY, "DENY", ""),
        BucketData("test-bucket-3", CYNARA_ADMIN_BUCKET, "BUCKET", "other-bucket"),
        BucketData("test-bucket-2", CYNARA_ADMIN_NONE, "NONE", ""),
        BucketData("test-bucket-4", 42, "hitchhiker", "douglas-noel-adams") };

    for (const auto &bucket : buckets) {
        const auto &bucketId = std::get<0>(bucket);
        const auto &policyType = std::get<1>(bucket);
        const auto &rawPolicyType = std::get<2>(bucket);
        const auto &metadata = std::get<3>(bucket);

        SCOPED_TRACE(bucketId);

        Cynara::SetBucketCyadCommand result(bucketId, { rawPolicyType, metadata });

        if (metadata.empty() == false) {
            EXPECT_CALL(m_adminApi,
                    cynara_admin_set_bucket(_, StrEq(bucketId.c_str()), policyType,
                                            StrEq(metadata.c_str())))
                .WillOnce(Return(CYNARA_API_SUCCESS));
        } else {
            EXPECT_CALL(m_adminApi,
                    cynara_admin_set_bucket(_, StrEq(bucketId.c_str()), policyType, IsNull()))
                .WillOnce(Return(CYNARA_API_SUCCESS));
        }

        dispatcher.execute(result);
    }
}

TEST_F(CyadCommandlineDispatcherTest, setPolicy) {
    using ::testing::_;
    using ::testing::Return;

    Cynara::CommandsDispatcher dispatcher(m_io, m_adminApi, m_errorApi);
    Cynara::SetPolicyCyadCommand result("test-bucket", { "allow", "" },
                                        { "client", "user", "privilege" });

    Cynara::CynaraAdminPolicies expectedPolicies;
    expectedPolicies.add("test-bucket", { CYNARA_ADMIN_ALLOW, "" },
                         { "client", "user", "privilege"} );
    expectedPolicies.seal();

    EXPECT_CALL(m_adminApi, cynara_admin_set_policies(_, AdmPolicyListEq(expectedPolicies.data())))
        .WillOnce(Return(CYNARA_API_SUCCESS));

    dispatcher.execute(result);
}

TEST_F(CyadCommandlineDispatcherTest, setPolicyWithMetadata) {
    using ::testing::_;
    using ::testing::Return;

    Cynara::CommandsDispatcher dispatcher(m_io, m_adminApi, m_errorApi);
    Cynara::SetPolicyCyadCommand result("test-bucket", { "allow", "metadata" },
                                        Cynara::PolicyKey("client", "user", "privilege"));

    Cynara::CynaraAdminPolicies expectedPolicies;
    expectedPolicies.add("test-bucket", { CYNARA_ADMIN_ALLOW, "metadata" },
                         { "client", "user", "privilege"} );
    expectedPolicies.seal();

    EXPECT_CALL(m_adminApi, cynara_admin_set_policies(_, AdmPolicyListEq(expectedPolicies.data())))
        .WillOnce(Return(CYNARA_API_SUCCESS));

    dispatcher.execute(result);
}

TEST_F(CyadCommandlineDispatcherTest, setPoliciesBulk) {
    using ::testing::_;
    using ::testing::Return;

    Cynara::CommandsDispatcher dispatcher(m_io, m_adminApi, m_errorApi);
    Cynara::SetPolicyBulkCyadCommand result("-");

    // fake stdin ;)
    m_io.file() << "bucket;cli;usr;privilege;0;metadata" << std::endl;
    m_io.file() << "bucket-2;cli;usr;privilege;0xFFFF;" << std::endl;
    m_io.file() << "bucket-3;cli;usr;priv;bucket;bucket-2";

    Cynara::CynaraAdminPolicies expectedPolicies;
    expectedPolicies.add("bucket", { CYNARA_ADMIN_DENY, "metadata" }, {"cli", "usr", "privilege"} );
    expectedPolicies.add("bucket-2", { CYNARA_ADMIN_ALLOW, "" }, {"cli", "usr", "privilege"} );
    expectedPolicies.add("bucket-3", { CYNARA_ADMIN_BUCKET, "bucket-2" }, {"cli", "usr", "priv"} );
    expectedPolicies.seal();

    EXPECT_CALL(m_adminApi, cynara_admin_set_policies(_, AdmPolicyListEq(expectedPolicies.data())))
        .WillOnce(Return(CYNARA_API_SUCCESS));

    dispatcher.execute(result);
}

TEST_F(CyadCommandlineDispatcherTest, setPoliciesBulkInputError) {
    using ::testing::_;
    using ::testing::Return;

    Cynara::CommandsDispatcher dispatcher(m_io, m_adminApi, m_errorApi);
    Cynara::SetPolicyBulkCyadCommand result("-");

    // fake stdin ;)
    m_io.file() << "invalid input" << std::endl;

    dispatcher.execute(result);

    ASSERT_FALSE(m_io.cerrRaw().str().empty());
}

TEST_F(CyadCommandlineDispatcherTest, erase) {
    using ::testing::_;
    using ::testing::Return;
    using ::testing::StrEq;

    Cynara::CommandsDispatcher dispatcher(m_io, m_adminApi, m_errorApi);

    Cynara::EraseCyadCommand command("", true, { "client", "user", "privilege" });

    EXPECT_CALL(m_adminApi, cynara_admin_erase(_, StrEq(""), true, StrEq("client"), StrEq("user"),
                                             StrEq("privilege")))
        .WillOnce(Return(CYNARA_API_SUCCESS));

    dispatcher.execute(command);
}

TEST_F(CyadCommandlineDispatcherTest, check) {
    using ::testing::_;
    using ::testing::DoAll;
    using ::testing::NotNull;
    using ::testing::Return;
    using ::testing::SetArgPointee;
    using ::testing::StrEq;

    Cynara::CommandsDispatcher dispatcher(m_io, m_adminApi, m_errorApi);

    Cynara::CheckCyadCommand command("", true, { "client", "user", "privilege" });
    int result = 42;

    EXPECT_CALL(m_adminApi, cynara_admin_check(_, StrEq(""), true, StrEq("client"), StrEq("user"),
                                             StrEq("privilege"), NotNull(), NotNull()))
        .WillOnce(DoAll(SetArgPointee<6>(result), SetArgPointee<7>(nullptr),
                        Return(CYNARA_API_SUCCESS)));

    dispatcher.execute(command);

    ASSERT_EQ("42;\n", m_io.coutRaw().str());
}

TEST_F(CyadCommandlineDispatcherTest, checkWithMetadata) {
    using ::testing::_;
    using ::testing::DoAll;
    using ::testing::NotNull;
    using ::testing::Return;
    using ::testing::SetArgPointee;
    using ::testing::StrEq;

    Cynara::CommandsDispatcher dispatcher(m_io, m_adminApi, m_errorApi);

    Cynara::CheckCyadCommand command("", true, { "client", "user", "privilege" });
    int result = 42;
    char *resultExtra = strdup("adams");

    EXPECT_CALL(m_adminApi, cynara_admin_check(_, StrEq(""), true, StrEq("client"), StrEq("user"),
                                             StrEq("privilege"), NotNull(), NotNull()))
        .WillOnce(DoAll(SetArgPointee<6>(result), SetArgPointee<7>(resultExtra),
                        Return(CYNARA_API_SUCCESS)));

    dispatcher.execute(command);

    ASSERT_EQ("42;adams\n", m_io.coutRaw().str());
}

TEST_F(CyadCommandlineDispatcherTest, checkWithError) {
    using ::testing::_;
    using ::testing::HasSubstr;
    using ::testing::Invoke;
    using ::testing::NotNull;
    using ::testing::Return;
    using ::testing::StrEq;
    using ::testing::Unused;

    Cynara::CommandsDispatcher dispatcher(m_io, m_adminApi, m_errorApi);

    Cynara::CheckCyadCommand command("", true, { "client", "user", "privilege" });

    auto setErrorMessage = [] (Unused, char *buf, std::size_t buflen) {
        strncpy(buf, "Test error message", buflen);
    };

    EXPECT_CALL(m_adminApi, cynara_admin_check(_, StrEq(""), true, StrEq("client"), StrEq("user"),
                                             StrEq("privilege"), NotNull(), NotNull()))
        .WillOnce(Return(CYNARA_API_UNKNOWN_ERROR));

    // Should we expect some minimal buflen here?
    EXPECT_CALL(m_errorApi, cynara_strerror(CYNARA_API_UNKNOWN_ERROR, NotNull(), _))
        .WillOnce(DoAll(Invoke(setErrorMessage), Return(CYNARA_API_SUCCESS)));

    dispatcher.execute(command);

    ASSERT_THAT(m_io.cerrRaw().str(), HasSubstr("Test error message"));
}

TEST_F(CyadCommandlineDispatcherTest, listPoliciesNone) {
    using ::testing::_;
    using ::testing::DoAll;
    using ::testing::NotNull;
    using ::testing::Return;
    using ::testing::SetArgPointee;
    using ::testing::StrEq;

    Cynara::CommandsDispatcher dispatcher(m_io, m_adminApi, m_errorApi);

    Cynara::ListPoliciesCyadCommand command("", { "client", "user", "privilege" });

    Cynara::CynaraAdminPolicies resultPolicies;
    resultPolicies.seal();
    auto policies = resultPolicies.duplicate();

    EXPECT_CALL(m_adminApi, cynara_admin_list_policies(_, StrEq(""), StrEq("client"), StrEq("user"),
                                                     StrEq("privilege"), NotNull()))
        .WillOnce(DoAll(SetArgPointee<5>(policies), Return(CYNARA_API_SUCCESS)));

    dispatcher.execute(command);

    ASSERT_EQ("", m_io.coutRaw().str());
}

TEST_F(CyadCommandlineDispatcherTest, listPoliciesTwo) {
    using ::testing::_;
    using ::testing::DoAll;
    using ::testing::NotNull;
    using ::testing::Return;
    using ::testing::SetArgPointee;
    using ::testing::StrEq;

    Cynara::CommandsDispatcher dispatcher(m_io, m_adminApi, m_errorApi);

    Cynara::ListPoliciesCyadCommand command("", { "client", "user", "privilege" });

    Cynara::CynaraAdminPolicies resultPolicies;
    resultPolicies.add("bucket", { CYNARA_ADMIN_DENY, "metadata" }, {"cli", "usr", "privilege"} );
    resultPolicies.add("bucket-2", { CYNARA_ADMIN_ALLOW, "" }, {"cli", "usr", "privilege"} );
    resultPolicies.seal();
    auto policies = resultPolicies.duplicate();

    EXPECT_CALL(m_adminApi, cynara_admin_list_policies(_, StrEq(""), StrEq("client"), StrEq("user"),
                                                     StrEq("privilege"), NotNull()))
        .WillOnce(DoAll(SetArgPointee<5>(policies), Return(CYNARA_API_SUCCESS)));

    dispatcher.execute(command);

    ASSERT_EQ("bucket;cli;usr;privilege;0;metadata\nbucket-2;cli;usr;privilege;65535;\n",
              m_io.coutRaw().str());
}

TEST_F(CyadCommandlineDispatcherTest, listPoliciesDesc) {
    using ::testing::_;
    using ::testing::DoAll;
    using ::testing::NotNull;
    using ::testing::Return;
    using ::testing::SetArgPointee;
    using ::testing::HasSubstr;

    addDescriptions({ { 42, "adams" } });

    Cynara::CommandsDispatcher dispatcher(m_io, m_adminApi, m_errorApi);

    Cynara::ListPoliciesDescCyadCommand command;
    dispatcher.execute(command);

    EXPECT_THAT(m_io.coutRaw().str(), HasSubstr("42;adams\n"));
}
