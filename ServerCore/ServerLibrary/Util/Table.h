#pragma once
#include "stdafx.h"
//#include <hash_map>
#include <unordered_map>
#include <map>
//#include <hash_set>
#include <unordered_set>
#include <set>
#include <vector>
#include <list>
#include <queue>
#include <deque>
#include <array>
#include <algorithm>
#include <functional>

typedef std::function<void(void *)> EachFunction;
typedef std::function<void(void *, void *)> PairFunction;

#if 0
template <typename KEY, typename VALUE>
class hash_map
{
    typedef std::hash_map<KEY, VALUE>			MAP;

    typedef typename MAP::iterator              ITER;
    typedef typename MAP::const_iterator        CITER;

    MAP     map_;
public:
    MAP&    map()           { return map_; }

    void    clear()         { map_.clear(); }
    size_t  size()          { return map_.size(); }
    bool    empty()         { return map_.empty(); }
    ITER    begin()         { return map_.begin(); }
    ITER    end()           { return map_.end(); }

    VALUE   front()
    {
        if (this->empty()) {
            return (VALUE) nullptr;
        }
        return this->begin()->second();
    }

    VALUE   back()
    {
        if (this->empty()) {
            return (VALUE) nullptr;
        }
        auto iter = this->end();
        --iter;
        return iter->second;
    }

    void    push(KEY key, VALUE value)
    {
		std::pair<KEY, VALUE> node(key, value);
		map_.insert(node);
    }

    VALUE    pop(KEY key)
    {
        auto iter = map_.find(key);
        if (iter == map_.end()) {
            return (VALUE) nullptr;
        }
        VALUE value = iter->second;
        map_.erase(iter);
        return value;
    }

    VALUE   get(KEY key)
    {
		if (this->empty()) {
			return (VALUE) nullptr;
		}
        auto iter = map_.find(key);
        return (iter == this->end()) ? (VALUE) nullptr : iter->second;
    }

    VALUE   next(KEY key, KEY &key2)
    {
        if (this->empty()) {
            return (VALUE) nullptr;
        }
        auto iter = map_.upper_bound(key);
        if (iter == this->end()) {
            return (VALUE) nullptr;
        }
        key2 = iter->first;
        return iter->second;
    }

    void    clearObjects(EachFunction func)
    {
		for (auto iter : map_) {
            if (func) {
                func(iter.second);
                continue;
            }
			SAFE_DELETE(iter.second);
        }
        this->clear();
    }

	void	clearObjects()
	{
		for (auto iter : map_) {
			SAFE_DELETE(iter.second);
        }
        this->clear();
	}

	void    doEach(EachFunction func)
	{
        if (!func) {
            return;
        }
		for (auto iter : map_) {
			func(iter.second);
		}
    }

    void    doPair(PairFunction func)
    {
        if (!func) {
            return;
        }
		for (auto iter : map_) {
			func(iter.second);
        }
    }
};

//-----------------------------------------------------------------//
//리스트 재 정의
//-----------------------------------------------------------------//
template <typename VALUE>
class List
{
public:
    typedef std::list<VALUE>					    LIST;
    typedef typename std::list<VALUE>::iterator	    ITER;
    typedef typename std::list<VALUE>::reverse_iterator 	RITER;

private:
    LIST    	list_;
    DWORD		currentPos_;
    VALUE			*instance_;

public:
    List() {
        currentPos_ = 0;
        instance_ = nullptr;
    }

    //기본 조작
    inline ITER		begin()	    			{ return list_.begin(); }
    inline ITER		end()					{ return list_.end(); }
    inline RITER	rbegin()				{ return list_.rbegin(); }
    inline RITER	rend()					{ return list_.rend(); }

    inline bool		next(ITER &pos, VALUE*& ptr)
    {
        ptr = nullptr;
        if (this->end() == pos)	{
            return false;
        }

        ptr = &(*pos);
        ++pos;
        return true;
    }

    inline ITER		erase(ITER &pos)	
    {
        if (this->end() == pos)	{
            return pos;
        }
        return list_.erase(pos);
    }
    inline void		remove(VALUE data)			{ list_.remove(data); }

    inline void		pushFront(VALUE &data)		{ list_.push_front(data); }
    inline void		pushBack(VALUE &data)		{ list_.push_back(data); }
    inline void		pushBackCopy(VALUE t)		{ PushBack(t); }

    inline VALUE&		front()					{ return list_.front(); }
    inline VALUE&		back()					{ return list_.back(); }

    inline void		popFront()				{ list_.pop_front(); }
    inline void		popBack()				{ list_.pop_back(); }
    inline bool		pop(VALUE &data)
    {
        ITER iter = this->find(data);
        if (iter != this->end()) {
            this->erase(iter);
            return true;
        }
        return false;
    }
    inline bool		rpop(VALUE &data)
    {
		RITER iter = this->rbegin();
		for (; iter != this->rend(); ++iter) {
			if (*iter = data) {
				this->erase((++iter).base());
                return true;
            }
        }
        
        return false;
    }

    inline void		clean(EachFunction func)
    {
        auto iter = this->begin();
        for (; iter != this->end(); ++iter) {
            if (func) {
                func(*iter);
            } else {
                SAFE_DELETE(*iter);
            }
        }
        this->clear();
    }
    inline void		clear()					{ list_.clear(); }

    inline size_t	size()	const			{ return list_.size(); }
    inline bool		empty()	const			{ return (bool)list_.empty(); }

    void	doEach(EachFunction func)
    {
        if (!func) {
            return;
        }
		for (auto iter : list_) {
			func(iter);
        }
    }
    void    doPair(PairFunction func)
    {
        if (!func) {
            return;
        }
		for (auto iter : list_) {
			func(iter);
        }
    }

    //-----------------------------------------------------------------//
    // 알고리즘
    VALUE	find(VALUE &aVal)
	{
		auto iter = std::find(this->begin(), this->end(), aVal); 
		if (iter == this->end()) {
			return (VALUE) nullptr;
		}
		return (VALUE)*iter;
	}
    LIST	&listData()					    { return list_; }
};
#endif