#pragma once
#include "stdafx.h"
#include <atomic>

//-------------------------------------------------------------------//
// 타입 정의
typedef UINT64					object_t;
typedef UINT64					oid_t;
typedef int32_t					packet_size_t;

typedef std::time_t				tick_t;
typedef std::thread				thread_t;
typedef std::thread::id			threadId_t;

typedef std::recursive_mutex	lock_t;

typedef std::string				str_t;
typedef std::wstring			wstr_t;

//-------------------------------------------------------------------//
// 크기 정의
#define SIZE_8				8
#define SIZE_64				64
#define SIZE_128			128
#define SIZE_256			256
#define SIZE_1024			1024
#define SIZE_4096			4096
#define SIZE_8192			8192

//-------------------------------------------------------------------//
#define DB_PARAM_SIZE		8192
#define SOCKET_BUF_SIZE		1024 * 10

//-------------------------------------------------------------------//
// 패킷을 type을 맞추기 위한 재정의 C# 기준
typedef unsigned char		Byte;
typedef char				Char;
typedef INT16				Int16;
typedef UINT16				UInt16;
typedef INT32				Int32;
typedef UINT32				UInt32;
typedef INT64				Int64;
typedef UINT64				UInt64;
typedef float				Float;

//for xml
typedef TiXmlDocument		xml_t;
typedef TiXmlElement		xmlNode_t;
typedef TiXmlHandle			xmlHandle_t;