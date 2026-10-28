#pragma once
#include <string>
#include "userTier.cpp"
using namespace std;

class User {
private:
    string userId;
    UserTier tier;

public:
    User(const string& id, UserTier userTier) : userId(id), tier(userTier) {}

    const string& getUserId() const { return userId; }
    UserTier getTier() const { return tier; }
};