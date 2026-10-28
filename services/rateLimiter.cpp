#pragma once
#include "rateLimiterType.cpp"
#include "rateLimitingConfig.cpp"
#include "../users/user.cpp"

// abstract rate limiter class
class RateLimiter {
protected:
    RateLimitingConfig config;
    RateLimiterType type;

public:
    RateLimiter(const RateLimitingConfig& cfg, RateLimiterType t) : config(cfg), type(t) {}
    virtual ~RateLimiter() = default;

    virtual bool allowRequest(const User& user) = 0;

    RateLimiterType getType() const { return type; }
};