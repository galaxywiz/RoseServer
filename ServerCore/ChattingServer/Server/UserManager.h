#pragma once
#include "stdafx.h"
class User;
class Session;

class UserManager : public Singleton < UserManager >
{
	unordered_map<oid_t, User*> userPool_;

public:
	void insert(User *user)
	{
		oid_t key = user->session()->id();
		userPool_.insert(make_pair(key, user));
	}

	void remove(oid_t id)
	{
		userPool_.erase(id);
	}

	User* at(oid_t id)
	{
 		auto itr = userPool_.lower_bound(id);
		if (itr == userPool_.end()) {
			return nullptr;
		}
		return itr->second;
	}

	size_t size()
	{
		return userPool_.size();
	}
};