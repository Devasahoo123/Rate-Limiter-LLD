#pragma once

enum class RateLimiterType {
    TokenBucket,
    FixedWindow,
    SlidingWindow
};