#include <iostream>
#include <chrono>
#include "services/rateLimiterFactory.cpp"
using namespace std;

// simple wait without threads
void waitSeconds(int seconds) {
    auto end = chrono::steady_clock::now() + chrono::seconds(seconds);
    while (chrono::steady_clock::now() < end) { }
}

void simulate(RateLimiterType type, const string& name) {
    // Free: 3 requests per 5 seconds, Premium: 30 requests per 5 seconds
    RateLimitingConfig config(3, 5);
    auto limiter = RateLimiterFactory::createRateLimiter(type, config);

    User freeUser("u1", UserTier::Free);
    User premiumUser("u2", UserTier::Premium);

    cout << "=== " << name << " ===\n";

    cout << "Free user, 5 quick requests:\n";
    for (int i = 1; i <= 5; i++) {
        cout << "  request " << i << ": "
             << (limiter->allowRequest(freeUser) ? "ALLOWED" : "BLOCKED") << "\n";
    }

    cout << "Premium user, 5 quick requests:\n";
    for (int i = 1; i <= 5; i++) {
        cout << "  request " << i << ": "
             << (limiter->allowRequest(premiumUser) ? "ALLOWED" : "BLOCKED") << "\n";
    }

    cout << "Waiting 5 seconds...\n";
    waitSeconds(5);

    cout << "Free user after waiting: "
         << (limiter->allowRequest(freeUser) ? "ALLOWED" : "BLOCKED") << "\n\n";
}

int main() {
    simulate(RateLimiterType::FixedWindow, "Fixed Window");
    simulate(RateLimiterType::SlidingWindow, "Sliding Window");
    simulate(RateLimiterType::TokenBucket, "Token Bucket");
    return 0;
}