#pragma once

struct NAny {

    NAny();
    virtual ~NAny();

    virtual NAny *Retain ();
    virtual void  Release();

private:
    int mCount;
};
