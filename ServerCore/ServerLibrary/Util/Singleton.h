#pragma once
#include "stdafx.h"

template <class T>
class Singleton
{
protected:
    Singleton() {}
    ~Singleton() {}

public:
    Singleton(const Singleton&);
    Singleton& operator = (const Singleton &);

    static T& getInstance()
    {
        static T instance;
        return instance;
    }
};
