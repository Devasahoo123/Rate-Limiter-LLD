#pragma once
#include <chrono>
#include <unordered_map>
#include "../services/rateLimiter.cpp"
using namespace std;

class fixedWindowRateLimiter : public RateLimiter {
private:
    using Clock = chrono::steady_clock;

    struct WindowState {
        Clock::time_point windowStart;
        int count = 0;
    };

    unordered_map<string, WindowState> states;

public:
    fixedWindowRateLimiter(const RateLimitingConfig& cfg)
        : RateLimiter(cfg, RateLimiterType::FixedWindow) {}

    bool allowRequest(const User& user) override {
        auto now = Clock::now();
        int limit = config.getLimit(user.getTier());
        auto window = chrono::seconds(config.windowInSeconds);

        auto it = states.find(user.getUserId());
        if (it == states.end()) {
            it = states.emplace(user.getUserId(), WindowState{now, 0}).first;
        }
        WindowState& s = it->second;

        // current window expired -> start a new one
        if (now - s.windowStart >= window) {
            s.windowStart = now;
            s.count = 0;
        }

        if (s.count < limit) {
            s.count++;
            return true;
        }
        return false;
    }
};