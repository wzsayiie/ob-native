#pragma once

struct NWidget {

    NWidget();
    virtual ~NWidget();

    virtual NWidget *Retain ();
    virtual void     Release();

private:
    int _count;
};
