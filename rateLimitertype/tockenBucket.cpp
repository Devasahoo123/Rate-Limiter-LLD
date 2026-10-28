#pragma once
#include <algorithm>
#include <chrono>
#include <unordered_map>
#include "../services/rateLimiter.cpp"
using namespace std;

class tokenBucketRateLimiter : public RateLimiter {
private:
    using Clock = chrono::steady_clock;

    struct Bucket {
        double tokens;
        Clock::time_point lastRefill;
    };

    unordered_map<string, Bucket> buckets;

public:
    tokenBucketRateLimiter(const RateLimitingConfig& cfg)
        : RateLimiter(cfg, RateLimiterType::TokenBucket) {}

    bool allowRequest(const User& user) override {
        auto now = Clock::now();
        double capacity = config.getLimit(user.getTier());
        double refillPerSec = capacity / config.windowInSeconds;

        auto it = buckets.find(user.getUserId());
        if (it == buckets.end()) {
            // new user starts with a full bucket
            it = buckets.emplace(user.getUserId(), Bucket{capacity, now}).first;
        }
        Bucket& b = it->second;

        // refill based on time passed
        double elapsed = chrono::duration<double>(now - b.lastRefill).count();
        b.tokens = min(capacity, b.tokens + elapsed * refillPerSec);
        b.lastRefill = now;

        if (b.tokens >= 1.0) {
            b.tokens -= 1.0;
            return true;
        }
        return false;
    }
};