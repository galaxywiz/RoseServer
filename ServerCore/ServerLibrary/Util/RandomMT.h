#pragma once
#include "stdafx.h"

#define RAND(type, maxVal)       (type) RandomMT::getInstance().rand(maxVal)

class RandomMT : public Singleton<RandomMT>
{
    uint64_t rand(int maxVal)
    {
		static uint32_t seed = 0;
        //MT19937 난수 엔진
		std::mt19937 engine((uint32_t)time(nullptr) + seed++);
        //UINT64 범위 = 약 1844경 = 18,446,744,073,709,551,615(16진수 0xFFFFFFFFFFFFFFFF)를 주기
        std::uniform_int_distribution<uint64_t> distribution(0, UINT64_MAX); 
        //rand 생성 함수포인터 bind
        auto generator = bind(distribution, engine);

        return (uint64_t)(generator() % maxVal);
    }
};