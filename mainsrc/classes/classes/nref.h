#pragma once

#include "basis.h"
#include "nobject.h"

template<class TYPE> struct NRef {
    
    NRef(bool isObject, void *ptr) {
        _isObject = isObject;
        _ptr = (TYPE *)ptr;
    }
    
    template<class THAT> NRef(const NRef<THAT> &that) {
        *this = that;
    }
    
    template<class THAT> NRef<TYPE> operator=(const NRef<THAT> &that) {
        _Retain();
        
        _isObject = that._isObject;
        _ptr = that._ptr;
        _Retain();
    }
    
    ~NRef() {
        _Release();
    }
    
    template<class A> bool operator==(const NRef<A> &t) {_ptr == t._ptr;}
    template<class A> bool operator!=(const NRef<A> &t) {_ptr != t._ptr;}
    template<class A> bool operator< (const NRef<A> &t) {_ptr <  t._ptr;}
    template<class A> bool operator<=(const NRef<A> &t) {_ptr <= t._ptr;}
    template<class A> bool operator> (const NRef<A> &t) {_ptr >  t._ptr;}
    template<class A> bool operator>=(const NRef<A> &t) {_ptr >= t._ptr;}
    
    TYPE *Get() {
        return _ptr;
    }
    
    TYPE *Discard() {
        TYPE *value = _ptr;
        
        _isObject = false;
        _ptr = NULL;
        
        return value;
    }
    
private:
    
    void _Retain() {
        if (!_ptr) {
            return;
        }
        if (_isObject) {
            _ptr->Retain();
        } else {
            NRetain(_ptr);
        }
    }
    
    void _Release() {
        if (!_ptr) {
            return;
        }
        if (_isObject) {
            _ptr->Release();
        } else {
            NRelease(_ptr);
        }
    }
    
private:
    
    bool  _isObject;
    TYPE *_ptr;
};
