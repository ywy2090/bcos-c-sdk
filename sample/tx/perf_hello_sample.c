/*
 *  Copyright (C) 2021 FISCO BCOS.
 *  SPDX-License-Identifier: Apache-2.0
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 * @file deploy_hello.c
 * @author: octopus
 * @date 2022-01-17
 */
#include "bcos-c-sdk/bcos_sdk_c_error.h"
#include "bcos-c-sdk/bcos_sdk_c_rpc.h"
#include "bcos-c-sdk/bcos_sdk_c_uti_abi.h"
#include "bcos-c-sdk/bcos_sdk_c_uti_keypair.h"
#include <bcos-c-sdk/bcos_sdk_c.h>
#include <bcos-c-sdk/bcos_sdk_c_uti_tx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstdio>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

// HelloWorld Source Code:

// HelloWorld Source Code:
/**
pragma solidity>=0.4.24 <0.6.11;

contract HelloWorld {
    string name;

    constructor() public {
        name = "Hello, World!";
    }

    function get() public view returns (string memory) {
        return name;
    }

    function set(string memory n) public {
        name = n;
    }
}
*/
const char* g_hw_bin =
    "608060405234801561001057600080fd5b506040518060400160405280600d81526020017f48656c6c6f2c20576f72"
    "6c6421000000000000000000000000000000000000008152506000908051906020019061005c929190610062565b50"
    "610107565b828054600181600116156101000203166002900490600052602060002090601f01602090048101928260"
    "1f106100a357805160ff19168380011785556100d1565b828001600101855582156100d1579182015b828111156100"
    "d05782518255916020019190600101906100b5565b5b5090506100de91906100e2565b5090565b61010491905b8082"
    "11156101005760008160009055506001016100e8565b5090565b90565b610310806101166000396000f3fe60806040"
    "5234801561001057600080fd5b50600436106100365760003560e01c80634ed3885e1461003b5780636d4ce63c1461"
    "00f6575b600080fd5b6100f46004803603602081101561005157600080fd5b81019080803590602001906401000000"
    "0081111561006e57600080fd5b82018360208201111561008057600080fd5b80359060200191846001830284011164"
    "0100000000831117156100a257600080fd5b91908080601f0160208091040260200160405190810160405280939291"
    "90818152602001838380828437600081840152601f19601f8201169050808301925050505050505091929192905050"
    "50610179565b005b6100fe610193565b60405180806020018281038252838181518152602001915080519060200190"
    "80838360005b8381101561013e578082015181840152602081019050610123565b50505050905090810190601f1680"
    "1561016b5780820380516001836020036101000a031916815260200191505b509250505060405180910390f35b8060"
    "00908051906020019061018f929190610235565b5050565b6060600080546001816001161561010002031660029004"
    "80601f0160208091040260200160405190810160405280929190818152602001828054600181600116156101000203"
    "1660029004801561022b5780601f106102005761010080835404028352916020019161022b565b8201919060005260"
    "20600020905b81548152906001019060200180831161020e57829003601f168201915b5050505050905090565b8280"
    "54600181600116156101000203166002900490600052602060002090601f016020900481019282601f106102765780"
    "5160ff19168380011785556102a4565b828001600101855582156102a4579182015b828111156102a3578251825591"
    "602001919060010190610288565b5b5090506102b191906102b5565b5090565b6102d791905b808211156102d35760"
    "008160009055506001016102bb565b5090565b9056fea2646970667358221220b5943f43c48cc93c6d71cdcf27aee5"
    "072566c88755ce9186e32ce83b24e8dc6c64736f6c634300060a0033";

const char* g_hw_sm_bin =
    "608060405234801561001057600080fd5b506040518060400160405280600d81526020017f48656c6c6f2c20576f72"
    "6c6421000000000000000000000000000000000000008152506000908051906020019061005c929190610062565b50"
    "610107565b828054600181600116156101000203166002900490600052602060002090601f01602090048101928260"
    "1f106100a357805160ff19168380011785556100d1565b828001600101855582156100d1579182015b828111156100"
    "d05782518255916020019190600101906100b5565b5b5090506100de91906100e2565b5090565b61010491905b8082"
    "11156101005760008160009055506001016100e8565b5090565b90565b610310806101166000396000f3fe60806040"
    "5234801561001057600080fd5b50600436106100365760003560e01c8063299f7f9d1461003b5780633590b49f1461"
    "00be575b600080fd5b610043610179565b604051808060200182810382528381815181526020019150805190602001"
    "9080838360005b83811015610083578082015181840152602081019050610068565b50505050905090810190601f16"
    "80156100b05780820380516001836020036101000a031916815260200191505b509250505060405180910390f35b61"
    "0177600480360360208110156100d457600080fd5b81019080803590602001906401000000008111156100f1576000"
    "80fd5b82018360208201111561010357600080fd5b8035906020019184600183028401116401000000008311171561"
    "012557600080fd5b91908080601f016020809104026020016040519081016040528093929190818152602001838380"
    "828437600081840152601f19601f82011690508083019250505050505050919291929050505061021b565b005b6060"
    "60008054600181600116156101000203166002900480601f0160208091040260200160405190810160405280929190"
    "818152602001828054600181600116156101000203166002900480156102115780601f106101e65761010080835404"
    "0283529160200191610211565b820191906000526020600020905b8154815290600101906020018083116101f45782"
    "9003601f168201915b5050505050905090565b8060009080519060200190610231929190610235565b5050565b8280"
    "54600181600116156101000203166002900490600052602060002090601f016020900481019282601f106102765780"
    "5160ff19168380011785556102a4565b828001600101855582156102a4579182015b828111156102a3578251825591"
    "602001919060010190610288565b5b5090506102b191906102b5565b5090565b6102d791905b808211156102d35760"
    "008160009055506001016102bb565b5090565b9056fea26469706673582212209871cb2bcf390d53645807cbaedfe0"
    "52d739ef9cff9d84787f74c4f379e1854664736f6c634300060a0033";

const char* g_hw_abi =
    "[{\"inputs\":[],\"stateMutability\":\"nonpayable\",\"type\":\"constructor\"},{\"inputs\":[],"
    "\"name\":\"get\",\"outputs\":[{\"internalType\":\"string\",\"name\":\"\",\"type\":\"string\"}]"
    ",\"stateMutability\":\"view\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":"
    "\"string\",\"name\":\"n\",\"type\":\"string\"}],\"name\":\"set\",\"outputs\":[],"
    "\"stateMutability\":\"nonpayable\",\"type\":\"function\"}]";

/*
{
    "6d4ce63c": "get()",
    "4ed3885e": "set(string)"
}

{
    "299f7f9d": "get()",
    "3590b49f": "set(string)"
}
*/

void usage(void)
{
    printf("Desc: HelloWorld contract sample\n");
    printf("Usage: hello_sample <config> <group_id>\n");
    printf("Example:\n");
    printf("    ./hello_sample ./config_sample.ini group0\n");
    exit(0);
}

// config file
char *config = NULL;

// group_id
char *group_id = NULL;

// contract address
char* contract_address = NULL;

// callback for rpc interfaces
void on_deploy_resp_callback(struct bcos_sdk_c_struct_response* resp)
{
    if (resp->error != BCOS_SDK_C_SUCCESS)
    {
        printf("\t deploy contract failed, error: %d, message: %s\n", resp->error, resp->desc);
        exit(-1);
    }

    const char* cflag = "contractAddress\" : \"";
    // find the "contractAddress": "0xxxxx"
    char* p0 = strstr((char*)resp->data, cflag);
    if (p0 == NULL)
    {
        printf("\t cannot find the \"contractAddress\" filed, resp: %s\n", (char*)resp->data);
        exit(-1);
    }

    char* p1 = (char*)p0 + strlen(cflag);
    char* p2 = strstr(p1, "\"");
    if (p2 == NULL)
    {
        printf("\t cannot find the \"contractAddress\" filed, resp: %s\n", (char*)resp->data);
        exit(-1);
    }

    contract_address = (char*)malloc(p2 - p1 + 1);

    memcpy(contract_address, p1, p2 - p1);
    contract_address[p2 - p1] = '\0';

    printf(" [HelloSample] contractAddress ===>>>>: %s\n", contract_address);
    printf(" [HelloSample] transaction receipt ===>>>>: %s\n", (char*)resp->data);
}

void on_send_tx_resp_callback(struct bcos_sdk_c_struct_response* resp)
{
    if (resp->error != BCOS_SDK_C_SUCCESS)
    {
        printf("\t send tx failed, error: %d, message: %s\n", resp->error, resp->desc);
        exit(-1);
    }

    printf(" ===>> send tx resp: %s\n", (char*)resp->data);
}

void on_call_resp_callback(struct bcos_sdk_c_struct_response* resp)
{
    if (resp->error != BCOS_SDK_C_SUCCESS)
    {
        printf("\t call failed, error: %d, message: %s\n", resp->error, resp->desc);
        exit(-1);
    }

    printf(" ===>> call resp: %s\n", (char*)resp->data);
}

void hello_world_demo(const char *config_file, const char *group_id)
{
    // 1. create sdk object by config
    void* sdk = bcos_sdk_create_by_config_file(config_file);
    // check success or not
    if (!bcos_sdk_is_last_opr_success())
    {
        printf(
            " bcos_sdk_create_by_config_file failed, error: %s\n", bcos_sdk_get_last_error_msg());
        exit(-1);
    }

    printf(" [HelloSample] start sdk ... \n");

    // 2. start bcos c sdk
    bcos_sdk_start(sdk);
    if (!bcos_sdk_is_last_opr_success())
    {
        printf(" [HelloSample] bcos_sdk_start failed, error: %s\n", bcos_sdk_get_last_error_msg());
        exit(-1);
    }

    int sm_crypto = 0;
    int wasm = 0;

    // 3. get sm_crypto of the group_id
    bcos_sdk_get_group_wasm_and_crypto(sdk, group_id, &wasm, &sm_crypto);
    if (!bcos_sdk_is_last_opr_success())
    {
        printf(" [CallHello] bcos_sdk_group_sm_crypto failed, error: %s\n",
            bcos_sdk_get_last_error_msg());
        exit(-1);
    }

    // printf(" [HelloSample] sm crypto: %d\n", sm_crypto);
    // 4. get chain_id of the group_id
    const char* chain_id = bcos_sdk_get_group_chain_id(sdk, group_id);
    if (!bcos_sdk_is_last_opr_success())
    {
        printf(" [HelloSample] bcos_sdk_get_group_chain_id failed, error: %s\n",
            bcos_sdk_get_last_error_msg());
        exit(-1);
    }

    // printf(" [HelloSample] chain id: %s\n", chain_id);
    // 5. get blocklimit of the group_id
    int64_t block_limit = bcos_rpc_get_block_limit(sdk, group_id);
    if (block_limit < 0)
    {
        printf(" [HelloSample] group not exist, group: %s\n", group_id);
        exit(-1);
    }

    // printf(" [HelloSample] block limit: %d\n", (int32_t)block_limit);
    // 6. load or create keypair for transaction sign

}

void* thread_function(void* arg) {

    void* sdk = bcos_sdk_create_by_config_file((char *)config);

    bcos_sdk_start(sdk);

    usleep(1000);

    bcos_rpc_get_block_number(sdk, (char*)group_id, NULL, on_recv_resp_callback, sdk);
    bcos_sdk_stop(sdk);
    bcos_sdk_destroy(sdk);
}


#define PTHREAD_COUNT 50

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        usage();
    }

    config = argv[1];
    group_id = argv[2];

    printf("[HelloSample] params ===>>>> \n");
    printf("\t # config: %s\n", config);
    printf("\t # group_id: %s\n", group_id);

    pthread_t threads[PTHREAD_COUNT];

    int rc;
    long t;
    for(t = 0; t < PTHREAD_COUNT; t++) {
        printf("In main: creating thread %ld\n", t);
        rc = pthread_create(&threads[t], NULL, thread_function, (void *)NULL);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    for(t = 0; t < PTHREAD_COUNT; t++) {
        pthread_join(threads[t], NULL);
    }

    return 0;
}
