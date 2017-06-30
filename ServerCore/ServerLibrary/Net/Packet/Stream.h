#pragma once
#include "stdafx.h"

class Stream
{
	size_t offset_;
	size_t readPt_;
	array <UCHAR, SOCKET_BUF_SIZE> stream_;

public:
	Stream();
	Stream(UCHAR *stream, size_t size);
	void initialize();

	UCHAR *data();
	size_t size();

	void operator = (Stream &stream);
	void set(UCHAR *stream, size_t size);

	// write
	//------------------------------------------------------------------------//
	bool checkWriteBound(size_t len);


	template<class T>
	void operator << (const T &value);
	void operator << (const bool &value);
	void operator << (const INT8 &value);
	void operator << (const UINT8 &value);
	void operator << (const INT16 &value);
	void operator << (const UINT16 &value);
	void operator << (const INT32 &value);
	void operator << (const UINT32 &value);
	void operator << (const INT64 &value);
	void operator << (const UINT64 &value);

	void operator << (const std::vector<wstr_t> &value);

	void operator << (const str_t value);
	void operator << (const wstr_t value);

	// read
	//------------------------------------------------------------------------//
	bool checkReadBound(size_t len);
	void read(void *retVal, size_t len);

	template<class T>
	void operator >> (T *retVal);

	void operator >> (bool *retVal);
	void operator >> (INT8 *retVal);
	void operator >> (UINT8 *retVal);
	void operator >> (INT16 *retVal);
	void operator >> (UINT16 *retVal);
	void operator >> (INT32 *retVal);
	void operator >> (UINT32 *retVal);
	void operator >> (INT64 *retVal);
	void operator >> (UINT64 *retVal);

	void operator >> (std::vector<wstr_t> *retVal);

	void operator >> (str_t *retVal);
	void operator >> (wstr_t *retVal);
};