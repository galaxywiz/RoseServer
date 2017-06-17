#pragma once
#include "stdafx.h"

class User : public GameObject
{
	Session *session_;
public:
	User(Session *session)
	{
		session_ = session;
	}
	
	Session* session()
	{
		return session_;
	}

	void tick()
	{
		
	}
};