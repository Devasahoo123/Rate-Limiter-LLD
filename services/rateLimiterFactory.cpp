#pragma once
#include <memory>
#include <stdexcept>
#include "rateLimiter.cpp"
#include "../rateLimitertype/tockenBucket.cpp"
#include "../rateLimitertype/fixWindow.cpp"
#include "../rateLimitertype/slidingWindow.cpp"
using namespace std;

class RateLimiterFactory {
public:
    static unique_ptr<RateLimiter> createRateLimiter(RateLimiterType type, const RateLimitingConfig& config) {
        switch (type) {
            case RateLimiterType::TokenBucket:
                return make_unique<tokenBucketRateLimiter>(config);
            case RateLimiterType::FixedWindow:
                return make_unique<fixedWindowRateLimiter>(config);
            case RateLimiterType::SlidingWindow:
                return make_unique<slidingWindowRateLimiter>(config);
        }
        throw invalid_argument("Invalid rate limiter type");
    }
};