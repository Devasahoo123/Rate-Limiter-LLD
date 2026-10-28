#pragma once
#include <chrono>
#include <deque>
#include <unordered_map>
#include "../services/rateLimiter.cpp"
using namespace std;

class slidingWindowRateLimiter : public RateLimiter {
private:
    using Clock = chrono::steady_clock;

    unordered_map<string, deque<Clock::time_point>> requestLogs;

public:
    slidingWindowRateLimiter(const RateLimitingConfig& cfg)
        : RateLimiter(cfg, RateLimiterType::SlidingWindow) {}

    bool allowRequest(const User& user) override {
        auto now = Clock::now();
        int limit = config.getLimit(user.getTier());
        auto window = chrono::seconds(config.windowInSeconds);

        auto& log = requestLogs[user.getUserId()];

        // remove timestamps that fell out of the window
        while (!log.empty() && now - log.front() >= window) {
            log.pop_front();
        }

        if ((int)log.size() < limit) {
            log.push_back(now);
            return true;
        }
        return false;
    }
};