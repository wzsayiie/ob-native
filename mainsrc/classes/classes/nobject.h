#pragma once

struct NObject {

    NObject();
    virtual ~NObject();

    virtual NObject *Retain ();
    virtual void     Release();

private:
    int _count;
};
