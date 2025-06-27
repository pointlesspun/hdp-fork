/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 // winutils.mc 
 // EventLog messages for Hadoop winutils service.
 // The following are the categories of events.
//
//  Values are 32 bit values laid out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      R - is a reserved bit
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//
//
// Define the facility codes
//


//
// Define the severity codes
//


//
// MessageId: SERVICE_CATEGORY
//
// MessageText:
//
// Service Events
//
#define SERVICE_CATEGORY                 ((WORD)0x00000001L)

//
// MessageId: LOG_CATEGORY
//
// MessageText:
//
// Task Events
//
#define LOG_CATEGORY                     ((WORD)0x00000002L)

 // The following are the message definitions.
//
// MessageId: MSG_CHECK_ERROR
//
// MessageText:
//
// %1. Error %2: %3.
//
#define MSG_CHECK_ERROR                  ((DWORD)0x00000080L)

//
// MessageId: MSG_RPC_SERVICE_HAS_STARTED
//
// MessageText:
//
// The LPC server is listenning.
//
#define MSG_RPC_SERVICE_HAS_STARTED      ((DWORD)0x00000100L)

//
// MessageId: MSG_RPC_SERVICE_HAS_STOPPED
//
// MessageText:
//
// The LPC server has stopped listenning.
//
#define MSG_RPC_SERVICE_HAS_STOPPED      ((DWORD)0x00000200L)

