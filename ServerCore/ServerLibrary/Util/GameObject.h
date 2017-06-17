#pragma once
#include "stdafx.h"

class Object
{
	wstr_t			allocFile_;
	int				allocLine_;
};

class NameObject
{
	wstr_t name_;
public:
	wstr_t& name() 
	{
		return name_;
	}

	void setName(wstr_t name)
	{
		name_ = name;
	}
};

class Work
{
public:
	virtual void tick() = 0;
	virtual void wakeup() {};
	virtual void suspend() {};
	virtual void stop() {};
	virtual void start() {};
};

class GameObject : public NameObject, public Work
{
	POINT		position_;
	float		direction_;

public:
	// 클래스 이름 마다 소멸자 이름이 다르므로, free로 통일 시키자
	virtual ~GameObject() 
	{
		this->free();
	}
	virtual void initialize() {};
	virtual void free() {};
};