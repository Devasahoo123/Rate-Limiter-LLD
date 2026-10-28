#pragma once
#include "../users/userTier.cpp"

struct RateLimitingConfig {
    int maxRequests;          // limit for Free tier per window
    int windowInSeconds;      // window size in seconds
    int premiumMultiplier;    // Premium limit = maxRequests * premiumMultiplier

    RateLimitingConfig(int maxReq, int timeWin, int premiumMul = 10)
        : maxRequests(maxReq), windowInSeconds(timeWin), premiumMultiplier(premiumMul) {}

    int getLimit(UserTier tier) const {
        return tier == UserTier::Premium ? maxRequests * premiumMultiplier : maxRequests;
    }
};