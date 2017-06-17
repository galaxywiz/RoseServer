#pragma once
#include "stdafx.h"
using namespace std;

class Stream
{
	size_t offset_;
	size_t readPt_;
	array <UCHAR, SOCKET_BUF_SIZE> stream_;

public:
	Stream()
	{
		readPt_ = 0;
		offset_ = 0;
		ZeroMemory(&stream_, sizeof(stream_));
	}

	UCHAR *stream()
	{
		return stream_.data();
	}

	size_t size()
	{
		return offset_;
	}

	void operator = (Stream &stream)
	{
		this->set(stream.stream(), stream.size());
	}

	void set(UCHAR *stream, size_t size)
	{
		this->offset_ = size;
		memcpy_s(this->stream_.data(), stream_.size(), (void *)stream, size);
	}

	// write
	//------------------------------------------------------------------------//
	bool checkWriteBound(size_t len)
	{
		if (offset_ + len > sizeof(stream_)) {
			SLog(L"! socket stream over.");
			ASSERT(FALSE);
			return false;
		}
		return true;
	}

	template<typename T>
	void operator << (const T &value)
	{
		size_t size = sizeof(value);
		if (this->checkWriteBound(size) == false) {
			return;
		}
		
		memcpy_s((void *)(stream_.data() + offset_), stream_.size() - offset_, (const void *)&value, size);
		offset_ += size;
	}

	template<class T>
	void operator << (const std::vector<T> &value)
	{
		*this << value.size();
		for (auto i : value) {
			*this << i;
		}
	}

	void operator << (std::string value)
	{
		*this << value.length();
		for (auto i : value) {
			*this << i;
		}
	}

	void operator << (wstr_t value)
	{
		*this << (value.length());
		for (auto i : value) {
			*this << i;
		}
	}

	// read
	//------------------------------------------------------------------------//
	bool checkReadBound(size_t len) 
	{
		if (readPt_ + len > offset_) {
			SLog(L"! readOffset : %d, size: %d, totalOffset = %d", readPt_, len, offset_);
			SLog(L"! socket stream has not more memory.");
			ASSERT(FALSE);
			return false;
		}
		return true;
	}

	void read(void *retVal, size_t len)
	{
		memcpy_s(retVal, len, (void *)(stream_.data() + readPt_), len);
		readPt_ += len;
	}

	template<typename T>
	void operator >> (T *retVal)
	{
		size_t size = sizeof(*retVal);
		if (this->checkReadBound(size) == false) {
			return;
		}
		this->read((void *)(retVal), sizeof(T));
	}

	void operator >> (std::string *retVal)
	{
		size_t size;
		*this >> &size;
		if (this->checkReadBound(size) == false) {
			return;
		}

		char *buf = new char[size + 1];
		this->read((void *)(buf), size * sizeof(CHAR));
		buf[size] = '\0';

		retVal->clear();
		*retVal = buf;

		delete buf;
	}

	void operator >> (wstr_t *retVal)
	{
		size_t size;
		*this >> &size;
		if (this->checkReadBound(size) == false) {
			return;
		}

		WCHAR *buf = new WCHAR[size + 1];
		this->read((void *)(buf), size * sizeof(WCHAR));
		buf[size] = '\0';

		retVal->clear();
		*retVal = buf;

		delete buf;
	}

	template<class T>
	void operator >> (std::vector<T> *retVal)
	{
		size_t size;
		*this >> &size;

		for (size_t i = 0; i < size; ++i) {
			T tmp;
			*this >> &tmp;
			retVal->push_back(tmp);
		}
	}
};