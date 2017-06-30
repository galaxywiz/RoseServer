#pragma once
#include "stdafx.h"
#include "Stream.h"

Stream::Stream()
{
	this->initialize();
}

Stream::Stream(UCHAR *stream, size_t size)
{
	this->initialize();
	this->set(stream, size);
}

void Stream::initialize()
{
	readPt_ = 0;
	offset_ = 0;
	ZeroMemory(&stream_, sizeof(stream_));
}

UCHAR *Stream::data()
{
	return stream_.data();
}

size_t Stream::size()
{
	return offset_;
}

void Stream::operator = (Stream &stream)
{
	this->set(stream.data(), stream.size());
}

void Stream::set(UCHAR *data, size_t size)
{
	this->offset_ = size;
	memcpy_s(this->stream_.data(), stream_.size(), (void *)data, size);
}

// write
//------------------------------------------------------------------------//
bool Stream::checkWriteBound(size_t len)
{
	if (offset_ + len > sizeof(stream_)) {
		SLog(L"! socket stream over.");
		ASSERT(FALSE);
		return false;
	}
	return true;
}

#define STREAM_WRITE(value)						\
	INT32 size = sizeof(value);					\
	if (this->checkWriteBound(size) == false) {	\
		return;									\
	}											\
	memcpy_s((void *)(stream_.data() + offset_), stream_.size() - offset_, (const void *)&value, size);\
	offset_ += size;

template<class T>
void Stream::operator << (const T &value)
{
	STREAM_WRITE(value);
}
void Stream::operator << (const bool &value)
{
	STREAM_WRITE(value);
}
void Stream::operator << (const INT8 &value)
{
	STREAM_WRITE(value);
}
void Stream::operator << (const UINT8 &value)
{
	STREAM_WRITE(value);
}
void Stream::operator << (const INT16 &value)
{
	STREAM_WRITE(value);
}
void Stream::operator << (const UINT16 &value)
{
	STREAM_WRITE(value);
}
void Stream::operator << (const INT32 &value)
{
	STREAM_WRITE(value);
}
void Stream::operator << (const UINT32 &value)
{
	STREAM_WRITE(value);
}
void Stream::operator << (const INT64 &value)
{
	STREAM_WRITE(value);
}
void Stream::operator << (const UINT64 &value)
{
	STREAM_WRITE(value);
}

void Stream::operator << (const std::vector<wstr_t> &value)
{
	*this << value.size();
	for (auto i : value) {
		*this << i;
	}
}

void Stream::operator << (const str_t value)
{
	*this << (Int32)value.length();
	for (auto i : value) {
		*this << i;
	}
}

void Stream::operator << (const wstr_t value)
{
	*this << (Int32)(value.length());
	for (auto i : value) {
		*this << i;
	}
}

// read
//------------------------------------------------------------------------//
bool Stream::checkReadBound(size_t len)
{
	if (readPt_ + len > offset_) {
		SLog(L"! readOffset : %d, size: %d, totalOffset = %d", readPt_, len, offset_);
		SLog(L"! socket stream has not more memory.");
		ASSERT(FALSE);
		return false;
	}
	return true;
}

void Stream::read(void *retVal, size_t len)
{
	memcpy_s(retVal, len, (void *)(stream_.data() + readPt_), len);
	readPt_ += len;
}

#define STREAM_READ(type, retVal)				\
	size_t size = sizeof(type);					\
	if (this->checkReadBound(size) == false) {	\
		return;									\
	}											\
	this->read((void *)(retVal), size);	

template<class T>
void Stream::operator >> (T *retVal)
{
	STREAM_READ(T, retVal);
}

void Stream::operator >> (bool *retVal)
{
	STREAM_READ(bool, retVal);
}
void Stream::operator >> (INT8 *retVal)
{
	STREAM_READ(INT8, retVal);
}
void Stream::operator >> (UINT8 *retVal)
{
	STREAM_READ(UINT8, retVal);
}
void Stream::operator >> (INT16 *retVal)
{
	STREAM_READ(INT16, retVal);
}
void Stream::operator >> (UINT16 *retVal)
{
	STREAM_READ(UINT16, retVal);
}
void Stream::operator >> (INT32 *retVal)
{
	STREAM_READ(INT32, retVal);
}
void Stream::operator >> (UINT32 *retVal)
{
	STREAM_READ(UINT32, retVal);
}
void Stream::operator >> (INT64 *retVal)
{
	STREAM_READ(INT64, retVal);
}
void Stream::operator >> (UINT64 *retVal)
{
	STREAM_READ(UINT64, retVal);
}

void Stream::operator >> (std::vector<wstr_t> *retVal)
{
	size_t size;
	*this >> &size;

	for (size_t i = 0; i < size; ++i) {
		wstr_t tmp;
		*this >> &tmp;
		retVal->push_back(tmp);
	}
}

void Stream::operator >> (str_t *retVal)
{
	INT32 size;
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

void Stream::operator >> (wstr_t *retVal)
{
	INT32 size;
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