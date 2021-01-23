#pragma once

#include "basis.h"
#include "nany.h"

template<class T> struct NRef {

    NRef(T *ptr) {
        _ptr = ptr;
    }

    template<class A> NRef(const NRef<A> &that) {
        _ptr = _Retain<A>(that.Get());
    }

    template<class A> NRef<T> operator=(const NRef<A> &that) {
        _Release(_ptr);
        _ptr = _Retain<A>(that.Get());
        return *this;
    }

    NRef<T> operator=(std::nullptr_t) {
        _Release(_ptr);
        _ptr = NULL;
        return *this;
    }

    ~NRef() {
        _Release(_ptr);
    }

    bool operator==(std::nullptr_t) const {return _ptr == NULL;}
    bool operator!=(std::nullptr_t) const {return _ptr != NULL;}

    template<class A> bool operator==(const NRef<A> &a) const {return _ptr == a.Get();}
    template<class A> bool operator!=(const NRef<A> &a) const {return _ptr != a.Get();}
    template<class A> bool operator< (const NRef<A> &a) const {return _ptr <  a.Get();}
    template<class A> bool operator<=(const NRef<A> &a) const {return _ptr <= a.Get();}
    template<class A> bool operator> (const NRef<A> &a) const {return _ptr >  a.Get();}
    template<class A> bool operator>=(const NRef<A> &a) const {return _ptr >= a.Get();}

    template<class A> NRef<A> Cast() const {
        return NRef<A>(_Retain<A>(_ptr));
    }

    T *Retain() const {
        return _Retain<T>(_ptr);
    }

    T *Get() const {
        return _ptr;
    }

    operator T *() const {
        return _ptr;
    }

private:

    template<class A> A *_Retain(NAny    *p) {return p ? (A *)p->Retain() : NULL;}
    template<class A> A *_Retain(NObject *p) {return p ? (A *)NRetain(p)  : NULL;}

    void _Release(NAny    *p) {if (p) {p->Release();}}
    void _Release(NObject *p) {if (p) {NRelease(p); }}

private:

    T *_ptr;
};

struct NRefTransfer {
    template<class T> NRef<T> operator<<(T *ptr) const {
        return NRef<T>(ptr);
    }
};

#define nauto NRefTransfer()<<
