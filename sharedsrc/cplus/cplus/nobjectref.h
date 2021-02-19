#pragma once

#include "basis.h"

template<class, class> struct _NCPPTypeComparer {
    static const bool SAME = false;
};
template<class T> struct _NCPPTypeComparer<T, T> {
    static const bool SAME = true;
};

//use "NObjectRef"
//as super when defining a cpp reference class:

struct NObjectRef {

    NObjectRef();
    NObjectRef(nullptr_t);
    NObjectRef(const NObjectRef &that);
    NObjectRef(NObjectRef &&that) noexcept;

    void operator=(nullptr_t);
    void operator=(const NObjectRef &that);
    void operator=(NObjectRef &&that) noexcept;

    ~NObjectRef();

    void setUnsafeRef(NRef ref);
    NRef unsafeRef() const;

    operator NObject *() const;

    template<class WANT> WANT *ref() const {
        if (_NCPPTypeComparer<WANT, NObject>::SAME) {
            return (WANT *)mRef;
        }
        if (_NCPPTypeComparer<WANT, void>::SAME) {
            return (WANT *)mRef;
        }
        return NULL;
    }

    template<class THAT> THAT unsafeCast() const {
        THAT that = NULL;
        that.setUnsafeRef(mRef);
        return that;
    }

private:
    NRef mRef;
};

//use "ncppclass" and "NCPP_BODY"
//to define a cpp reference class:

template<class CTYPE, class SUPER> struct _NCPPRef : SUPER {
    using SUPER::SUPER;

    using Super   = SUPER;
    using Middler = _NCPPRef<CTYPE, SUPER>;

    operator CTYPE *() const {
        return (CTYPE *)SUPER::unsafeRef();
    }

    template<class WANT> WANT *ref() const {
        if (_NCPPTypeComparer<WANT, CTYPE>::SAME) {
            return (WANT *)SUPER::unsafeRef();
        }
        return SUPER::template ref<WANT>();
    }
};

template<class> struct _NCPPRefOf;
template</* */> struct _NCPPRefOf<NObject> {using Type = NObjectRef;};
template</* */> struct _NCPPRefOf<void>    {using Type = NObjectRef;};

#define ncppclass(ctype, name, super)           \
/**/    struct name;                            \
/**/    template<> struct _NCPPRefOf<ctype> {   \
/**/        using Type = name;                  \
/**/    };                                      \
/**/    struct name : _NCPPRef<ctype, super>

#define NCPP_BODY \
/**/    using Middler::Middler;

//use "nautorelease" or "nautoref"
//to transfer a c reference to cpp reference:

struct _NCPPBridge {
    template<class CTYPE> typename _NCPPRefOf<CTYPE>::Type operator<<(CTYPE *ref) {
        typename _NCPPRefOf<CTYPE>::Type wrap = NULL;
        wrap.setUnsafeRef(ref);
        //invalidate the c reference.
        NRelease(ref);
        return wrap;
    }
    template<class CTYPE> typename _NCPPRefOf<CTYPE>::Type operator=(CTYPE *ref) {
        typename _NCPPRefOf<CTYPE>::Type wrap = NULL;
        wrap.setUnsafeRef(ref);
        return wrap;
    }
};
#define nautorelease _NCPPBridge()<<
#define nautoref     _NCPPBridge()=
