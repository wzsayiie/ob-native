#include "nwidget.h"
#include "basis.h"

NWidget::NWidget() {
    mCount = 1;
}

NWidget::~NWidget() {
}

NWidget *NWidget::Retain() {
    nsynwith(this) {
        mCount += 1;
    }
    return this;
}

void NWidget::Release() {
    nsynwith(this) {
        if (--mCount <= 0) {
            delete this;
        }
    }
}
