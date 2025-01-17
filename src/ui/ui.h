#ifndef UI_H
#define UI_H

#include "../gfx/gfx.h"
#include "../util/util.h"

#include "hotbar.h"
#include "crosshair.h"

typedef void (*FUIComponent)(void *);

struct UIComponent {
    FUIComponent destroy, render, update, tick;
    void *component;
    bool enabled;
};

#define UI_COMPONENTS_MAX 256

struct UI {
    struct {
        struct UIComponent elements[UI_COMPONENTS_MAX];
        size_t count;
    } components;

    UIHotbar hotbar;
    UICrosshair crosshair;
};

extern struct UIComponent hotbar_init(UIHotbar *self);

#define _UI_COMPONENT(_type, _name, _member)\
    extern struct UIComponent _name##_init(_type *self);\
    assert(self->components.count != UI_COMPONENTS_MAX);\
    self->components.elements[self->components.count++] = _name##_init(&self->_member);\
    self->components.elements[self->components.count - 1].enabled = true;

static inline void ui_init(UI *self) {
    _UI_COMPONENT(UIHotbar, hotbar, hotbar);
    _UI_COMPONENT(UICrosshair, crosshair, crosshair);
}

void ui_destroy(UI *self);
void ui_render(UI *self);
void ui_update(UI *self);
void ui_tick(UI *self);

#endif