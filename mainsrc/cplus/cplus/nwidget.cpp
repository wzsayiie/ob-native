#include "nwidget.h"
#include "basis.h"

NWidget::NWidget() {
    _count = 1;
}

NWidget::~NWidget() {
}

NWidget *NWidget::Retain() {
    nsynwith(this) {
        _count += 1;
    }
    return this;
}

void NWidget::Release() {
    nsynwith(this) {
        if (--_count <= 0) {
            delete this;
        }
    }
}
