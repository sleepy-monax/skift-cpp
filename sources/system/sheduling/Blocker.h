#pragma once

namespace system::sheduling
{

class Blocker
{
private:
public:
    Blocker() {}

    virtual ~Blocker() {}

    virtual bool is_block() = 0;

    virtual void unblock() = 0;
};

} // namespace system::sheduling
