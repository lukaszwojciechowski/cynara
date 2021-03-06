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
 * @file        src/cyad/CommandlineParser/CyadCommand.cpp
 * @author      Aleksander Zdyb <a.zdyb@samsung.com>
 * @version     1.0
 * @brief       A representation of Cyad command
 */

#include <cyad/CommandsDispatcher.h>

#include "CyadCommand.h"

namespace Cynara {

int CyadCommand::run(CommandsDispatcher &dispatcher) {
    return dispatcher.execute(*this);
}

int ErrorCyadCommand::run(CommandsDispatcher &dispatcher) {
    return dispatcher.execute(*this);
}

int HelpCyadCommand::run(CommandsDispatcher &dispatcher) {
    return dispatcher.execute(*this);
}

int SetBucketCyadCommand::run(CommandsDispatcher &dispatcher) {
    return dispatcher.execute(*this);
}

int DeleteBucketCyadCommand::run(CommandsDispatcher &dispatcher) {
    return dispatcher.execute(*this);
}

int SetPolicyCyadCommand::run(CommandsDispatcher &dispatcher) {
    return dispatcher.execute(*this);
}

int SetPolicyBulkCyadCommand::run(CommandsDispatcher &dispatcher) {
    return dispatcher.execute(*this);
}

int EraseCyadCommand::run(CommandsDispatcher &dispatcher) {
    return dispatcher.execute(*this);
}

int CheckCyadCommand::run(CommandsDispatcher &dispatcher) {
    return dispatcher.execute(*this);
}

int ListPoliciesCyadCommand::run(CommandsDispatcher &dispatcher) {
    return dispatcher.execute(*this);
}

int ListPoliciesDescCyadCommand::run(CommandsDispatcher &dispatcher) {
    return dispatcher.execute(*this);
}

} /* namespace Cynara */
