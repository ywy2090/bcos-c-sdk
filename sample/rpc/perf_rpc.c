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
 * @file rpc.c
 * @author: octopus
 * @date 2021-08-24
 */

#include <bcos-c-sdk/bcos_sdk_c.h>
#include <bcos-c-sdk/bcos_sdk_c_common.h>
#include <bcos-c-sdk/bcos_sdk_c_error.h>
#include <bcos-c-sdk/bcos_sdk_c_rpc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void usage(void)
{
    printf("Desc: rpc methods call test\n");
    printf("Usage: rpc <host> <port> <ssl type> <group_id>\n");
    printf("Example:\n");
    printf("   ./rpc 127.0.0.1 20200 ssl group0\n");
    printf("   ./rpc 127.0.0.1 20200 sm_ssl group0\n");
    exit(0);
}


// callback for rpc interfaces
void on_recv_resp_callback(struct bcos_sdk_c_struct_response* resp)
{
    if (resp->error != 0)
    {
        printf("\t something is wrong, error: %d, errorMessage: %s\n", resp->error, resp->desc);
        exit(-1);
    }
    else
    {
        printf(" \t recv rpc resp from server ===>>>> resp: %s\n", (char*)resp->data);
    }
}

void* thread_function(void* arg) {

    void* sdk = bcos_sdk_create((struct bcos_sdk_c_config *)arg);

    bcos_sdk_start(sdk);
      
      usleep(1000);

      bcos_rpc_get_block_number(sdk, (char*)"group0", NULL, on_recv_resp_callback, sdk);

      bcos_sdk_stop(sdk);
      bcos_sdk_destroy(sdk);
}

int main(int argc, char** argv)
{
    if (argc < 5)
    {
        usage();
    }

    // option params
    const char* host = argv[1];
    int port = atoi(argv[2]);
    const char* type = argv[3];
    const char* group = argv[4];

    printf(" [RPC] params ===>>>> \n");
    printf(" \t # host: %s\n", host);
    printf(" \t # port: %d\n", port);
    printf(" \t # type: %s\n", type);
    printf(" \t # group: %s\n", group);

    int is_sm_ssl = 1;
    char* pos = strstr(type, "sm_ssl");
    if (pos == NULL)
    {
        is_sm_ssl = 0;
    }


    struct bcos_sdk_c_config* config = bcos_sdk_create_config(is_sm_ssl, (char*)host, port);
   
    // check success or not
    int error = bcos_sdk_get_last_error();
    if (error != 0)
    {
        printf(" bcos_sdk_create_by_config_file failed, error: %d, errorMessage: %s\n", error,
            bcos_sdk_get_last_error_msg());
        exit(-1);
    }
    // bcos_sdk_c_config_destroy(config);  
    
    pthread_t threads[20];

    int rc;
    long t;
    for(t = 0; t < 20; t++) {
        printf("In main: creating thread %ld\n", t);
        rc = pthread_create(&threads[t], NULL, thread_function, (void *)config);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    for(t = 0; t < 20; t++) {
        pthread_join(threads[t], NULL);
    }


    return EXIT_SUCCESS;
}
