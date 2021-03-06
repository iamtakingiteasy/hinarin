//
// Created by user on 3/23/18.
//

#include "bind_nuklear.h"
#include "hinarin.h"
#include <stdlib.h>

void hinarin_nuklear_render_add(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);

    for (xsIntegerValue i = 0; i < xsToInteger(xsArgc); i++) {
        if (!xsHas(xsArg(i), xsID("render")) || !xsIsInstanceOf(xsGet(xsArg(i), xsID("render")), xsFunctionPrototype)) {
            xsTypeError("nuklear.render() arguments must have .render() function on them");
            return;
        }
    }

    linked_slot *tail = NULL;

    if (hinarin->renderers == NULL) {
        tail = calloc(1, sizeof(linked_slot));
    } else {
        tail = hinarin->renderers;
        while (tail->next != NULL) {
            tail = tail->next;
        }
    }
    for (xsIntegerValue i = 0; i < xsToInteger(xsArgc); i++) {
        if (hinarin->renderers == NULL) {
            hinarin->renderers = tail;
        } else {
            tail->next = calloc(1, sizeof(linked_slot));
            tail = tail->next;
        }
        tail->slot = xsArg(i);
    }
}

static struct nk_rect toRect(xsMachine *the, xsSlot slot) {
    struct nk_rect rect = {0};
    rect.x = (float) xsToNumber(xsGet(slot, xsID("x")));
    rect.y = (float) xsToNumber(xsGet(slot, xsID("y")));
    rect.w = (float) xsToNumber(xsGet(slot, xsID("w")));
    rect.h = (float) xsToNumber(xsGet(slot, xsID("h")));
    return rect;
}

static xsSlot fromRect(xsMachine *the, struct nk_rect rect) {
    xsSlot obj = xsNewObject();
    xsSet(obj, xsID("x"), xsNumber(rect.x));
    xsSet(obj, xsID("y"), xsNumber(rect.y));
    xsSet(obj, xsID("w"), xsNumber(rect.w));
    xsSet(obj, xsID("h"), xsNumber(rect.h));
    return obj;
}

static struct nk_vec2 toVec(xsMachine *the, xsSlot slot) {
    struct nk_vec2 vec = {0};
    vec.x = (float) xsToNumber(xsGet(slot, xsID("x")));
    vec.y = (float) xsToNumber(xsGet(slot, xsID("y")));
    return vec;
}

static xsSlot fromVec(xsMachine *the, struct nk_vec2 vec) {
    xsSlot obj = xsNewObject();
    xsSet(obj, xsID("x"), xsNumber(vec.x));
    xsSet(obj, xsID("y"), xsNumber(vec.y));
    return obj;
}

static struct nk_scroll toScroll(xsMachine *the, xsSlot slot) {
    struct nk_scroll scroll = {0};
    scroll.x = (nk_uint) xsToInteger(xsGet(slot, xsID("x")));
    scroll.y = (nk_uint) xsToInteger(xsGet(slot, xsID("y")));
    return scroll;
}

static struct nk_color toColor(xsMachine *the, xsSlot slot) {
    struct nk_color color = {0};
    color.a = (nk_byte) xsToInteger(xsGet(slot, xsID("a")));
    color.b = (nk_byte) xsToInteger(xsGet(slot, xsID("b")));
    color.g = (nk_byte) xsToInteger(xsGet(slot, xsID("g")));
    color.r = (nk_byte) xsToInteger(xsGet(slot, xsID("r")));
    return color;
}

void hinarin_nuklear_begin(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsStringValue title = xsToString(xsArg(0));
    struct nk_rect bounds = toRect(the, xsArg(1));
    xsIntegerValue flags = xsToInteger(xsArg(2));
    xsResult = xsBoolean(nk_begin(hinarin->nuklear, title, bounds, (nk_flags) flags));
}

void hinarin_nuklear_begin_titled(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsStringValue name = xsToString(xsArg(0));
    xsStringValue title = xsToString(xsArg(1));
    struct nk_rect bounds = toRect(the, xsArg(2));
    xsIntegerValue flags = xsToInteger(xsArg(3));
    xsResult = xsBoolean(nk_begin_titled(hinarin->nuklear, name, title, bounds, (nk_flags) flags));
}

void hinarin_nuklear_end(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    nk_end(hinarin->nuklear);
}

void hinarin_nuklear_window_find(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsStringValue name = xsToString(xsArg(0));
    struct nk_window *window = nk_window_find(hinarin->nuklear, name);
    if (window) {
        xsResult = xsNewHostObject(NULL);
        xsSetHostData(xsResult, window);
    }
}

void hinarin_nuklear_window_get_bounds(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsResult = fromRect(the, nk_window_get_bounds(hinarin->nuklear));
}

void hinarin_nuklear_window_get_position(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsResult = fromVec(the, nk_window_get_position(hinarin->nuklear));
}

void hinarin_nuklear_window_get_size(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsResult = fromVec(the, nk_window_get_size(hinarin->nuklear));
}

void hinarin_nuklear_window_get_width(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsResult = xsNumber(nk_window_get_width(hinarin->nuklear));
}

void hinarin_nuklear_window_get_height(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsResult = xsNumber(nk_window_get_height(hinarin->nuklear));
}

void hinarin_nuklear_window_get_panel(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    struct nk_panel *panel = nk_window_get_panel(hinarin->nuklear);
    if (panel) {
        xsResult = xsNewHostObject(NULL);
        xsSetHostData(xsResult, panel);
    }
}

void hinarin_nuklear_window_get_content_region(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsResult = fromRect(the, nk_window_get_content_region(hinarin->nuklear));
}

void hinarin_nuklear_window_get_content_region_min(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsResult = fromVec(the, nk_window_get_content_region_min(hinarin->nuklear));
}

void hinarin_nuklear_window_get_content_region_max(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsResult = fromVec(the, nk_window_get_content_region_max(hinarin->nuklear));
}

void hinarin_nuklear_window_get_content_region_size(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsResult = fromVec(the, nk_window_get_content_region_size(hinarin->nuklear));
}

void hinarin_nuklear_window_get_canvas(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    struct nk_command_buffer *canvas = nk_window_get_canvas(hinarin->nuklear);
    if (canvas) {
        xsResult = xsNewHostObject(NULL);
        xsSetHostData(xsResult, canvas);
    }
}

void hinarin_nuklear_window_has_focus(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsResult = xsBoolean(nk_window_has_focus(hinarin->nuklear));
}

void hinarin_nuklear_window_is_collapsed(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsStringValue name = xsToString(xsArg(0));
    xsResult = xsBoolean(nk_window_is_collapsed(hinarin->nuklear, name));
}

void hinarin_nuklear_window_is_closed(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsStringValue name = xsToString(xsArg(0));
    xsResult = xsBoolean(nk_window_is_closed(hinarin->nuklear, name));
}

void hinarin_nuklear_window_is_hidden(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsStringValue name = xsToString(xsArg(0));
    xsResult = xsBoolean(nk_window_is_hidden(hinarin->nuklear, name));
}

void hinarin_nuklear_window_is_active(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsStringValue name = xsToString(xsArg(0));
    xsResult = xsBoolean(nk_window_is_active(hinarin->nuklear, name));
}

void hinarin_nuklear_window_is_hovered(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsResult = xsBoolean(nk_window_is_hovered(hinarin->nuklear));
}

void hinarin_nuklear_window_is_any_hovered(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsResult = xsBoolean(nk_window_is_any_hovered(hinarin->nuklear));
}

void hinarin_nuklear_item_is_any_active(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsResult = xsBoolean(nk_item_is_any_active(hinarin->nuklear));
}

void hinarin_nuklear_window_set_bounds(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsStringValue name = xsToString(xsArg(0));
    struct nk_rect bounds = toRect(the, xsArg(1));
    nk_window_set_bounds(hinarin->nuklear, name, bounds);
}

void hinarin_nuklear_window_set_position(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsStringValue name = xsToString(xsArg(0));
    struct nk_vec2 position = toVec(the, xsArg(1));
    nk_window_set_position(hinarin->nuklear, name, position);
}

void hinarin_nuklear_window_set_size(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsStringValue name = xsToString(xsArg(0));
    struct nk_vec2 size = toVec(the, xsArg(1));
    nk_window_set_size(hinarin->nuklear, name, size);
}

void hinarin_nuklear_window_set_focus(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsStringValue name = xsToString(xsArg(0));
    nk_window_set_focus(hinarin->nuklear, name);
}

void hinarin_nuklear_window_close(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsStringValue name = xsToString(xsArg(0));
    nk_window_close(hinarin->nuklear, name);
}

void hinarin_nuklear_window_collapse(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsStringValue name = xsToString(xsArg(0));
    xsBooleanValue collapsed = xsToBoolean(xsArg(1));
    nk_window_collapse(hinarin->nuklear, name, collapsed ? NK_MINIMIZED : NK_MAXIMIZED);
}

void hinarin_nuklear_window_collapse_if(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsStringValue name = xsToString(xsArg(0));
    xsBooleanValue collapsed = xsToBoolean(xsArg(1));
    xsBooleanValue commit = xsToBoolean(xsArg(2));
    nk_window_collapse_if(hinarin->nuklear, name, collapsed ? NK_MINIMIZED : NK_MAXIMIZED, commit);
}

void hinarin_nuklear_window_show(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsStringValue name = xsToString(xsArg(0));
    xsBooleanValue shown = xsToBoolean(xsArg(1));
    nk_window_show(hinarin->nuklear, name, shown ? NK_SHOWN: NK_HIDDEN);
}

void hinarin_nuklear_window_show_if(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsStringValue name = xsToString(xsArg(0));
    xsBooleanValue shown = xsToBoolean(xsArg(1));
    xsBooleanValue commit = xsToBoolean(xsArg(2));
    nk_window_show_if(hinarin->nuklear, name, shown ? NK_SHOWN : NK_HIDDEN, commit);
}

void hinarin_nuklear_layout_set_min_row_height(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsNumberValue height = xsToNumber(xsArg(0));
    nk_layout_set_min_row_height(hinarin->nuklear, (float) height);
}

void hinarin_nuklear_layout_reset_min_row_height(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    nk_layout_reset_min_row_height(hinarin->nuklear);
}

void hinarin_nuklear_layout_widget_bounds(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsResult = fromRect(the, nk_layout_widget_bounds(hinarin->nuklear));
}

void hinarin_nuklear_layout_ratio_from_pixel(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsNumberValue pixel_width = xsToNumber(xsArg(0));
    xsResult = xsNumber(nk_layout_ratio_from_pixel(hinarin->nuklear, (float) pixel_width));
}

void hinarin_nuklear_layout_row_dynamic(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsNumberValue height = xsToNumber(xsArg(0));
    xsIntegerValue cols = xsToInteger(xsArg(1));
    nk_layout_row_dynamic(hinarin->nuklear, (float) height, cols);
}

void hinarin_nuklear_layout_row_static(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsNumberValue height = xsToNumber(xsArg(0));
    xsIntegerValue item_width = xsToInteger(xsArg(1));
    xsIntegerValue cols = xsToInteger(xsArg(2));
    nk_layout_row_static(hinarin->nuklear, (float) height, item_width, cols);
}

void hinarin_nuklear_layout_row_begin(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsBooleanValue dynamic  = xsToBoolean(xsArg(0));
    xsNumberValue row_height = xsToNumber(xsArg(1));
    xsIntegerValue cols = xsToInteger(xsArg(2));
    nk_layout_row_begin(hinarin->nuklear, dynamic ? NK_DYNAMIC : NK_STATIC, (float) row_height, cols);
}

void hinarin_nuklear_layout_row_push(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsNumberValue value = xsToNumber(xsArg(0));
    nk_layout_row_push(hinarin->nuklear, (float) value);
}

void hinarin_nuklear_layout_row_end(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    nk_layout_row_end(hinarin->nuklear);
}

void hinarin_nuklear_layout_row(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsBooleanValue dynamic  = xsToBoolean(xsArg(0));
    xsNumberValue height = xsToNumber(xsArg(1));
    xsIntegerValue cols = xsToInteger(xsArg(2));
    float *ratio = NULL;
    if (xsToInteger(xsArgc) > 3) {
        float r = (float) xsToNumber(xsArg(3));
        ratio = &r;
    }
    nk_layout_row(hinarin->nuklear, dynamic ? NK_DYNAMIC : NK_STATIC, (float) height, cols, ratio);
}

void hinarin_nuklear_layout_row_template_begin(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsNumberValue row_height = xsToNumber(xsArg(0));
    nk_layout_row_template_begin(hinarin->nuklear, (float) row_height);
}

void hinarin_nuklear_layout_row_template_push_dynamic(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    nk_layout_row_template_push_dynamic(hinarin->nuklear);
}

void hinarin_nuklear_layout_row_template_push_variable(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsNumberValue min_width = xsToNumber(xsArg(0));
    nk_layout_row_template_push_variable(hinarin->nuklear, (float) min_width);
}

void hinarin_nuklear_layout_row_template_push_static(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsNumberValue width = xsToNumber(xsArg(0));
    nk_layout_row_template_push_static(hinarin->nuklear, (float) width);
}

void hinarin_nuklear_layout_row_template_end(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    nk_layout_row_template_end(hinarin->nuklear);
}

void hinarin_nuklear_layout_space_begin(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsBooleanValue dynamic  = xsToBoolean(xsArg(0));
    xsNumberValue height = xsToNumber(xsArg(1));
    xsIntegerValue widget_count = xsToInteger(xsArg(2));
    nk_layout_space_begin(hinarin->nuklear, dynamic ? NK_DYNAMIC : NK_STATIC, (float) height, widget_count);
}

void hinarin_nuklear_layout_space_push(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    struct nk_rect bounds = toRect(the, xsArg(0));
    nk_layout_space_push(hinarin->nuklear, bounds);
}

void hinarin_nuklear_layout_space_end(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    nk_layout_space_end(hinarin->nuklear);
}

void hinarin_nuklear_layout_space_bounds(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsResult = fromRect(the, nk_layout_space_bounds(hinarin->nuklear));
}

void hinarin_nuklear_layout_space_to_screen(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    struct nk_vec2 vec = toVec(the, xsArg(0));
    xsResult = fromVec(the, nk_layout_space_to_screen(hinarin->nuklear, vec));
}

void hinarin_nuklear_layout_space_to_local(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    struct nk_vec2 vec = toVec(the, xsArg(0));
    xsResult = fromVec(the, nk_layout_space_to_local(hinarin->nuklear, vec));
}

void hinarin_nuklear_layout_space_rect_to_screen(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    struct nk_rect rect = toRect(the, xsArg(0));
    xsResult = fromRect(the, nk_layout_space_rect_to_screen(hinarin->nuklear, rect));
}

void hinarin_nuklear_layout_space_rect_to_local(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    struct nk_rect rect = toRect(the, xsArg(0));
    xsResult = fromRect(the, nk_layout_space_rect_to_local(hinarin->nuklear, rect));
}

void hinarin_nuklear_group_begin(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsStringValue title = xsToString(xsArg(0));
    xsIntegerValue flags = xsToInteger(xsArg(1));
    xsResult = xsBoolean(nk_group_begin(hinarin->nuklear, title, (nk_flags) flags));
}

void hinarin_nuklear_group_begin_titled(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsStringValue name = xsToString(xsArg(0));
    xsStringValue title = xsToString(xsArg(1));
    xsIntegerValue flags = xsToInteger(xsArg(2));
    xsResult = xsBoolean(nk_group_begin_titled(hinarin->nuklear, name, title, (nk_flags) flags));
}

void hinarin_nuklear_group_end(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    nk_group_end(hinarin->nuklear);
}

void hinarin_nuklear_group_scrolled_offset_begin(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    nk_uint x_offset = (nk_uint) xsToInteger(xsArg(0));
    nk_uint y_offset = (nk_uint) xsToInteger(xsArg(1));
    xsStringValue title = xsToString(xsArg(2));
    xsIntegerValue flags = xsToInteger(xsArg(3));
    xsResult = xsBoolean(nk_group_scrolled_offset_begin(hinarin->nuklear, &x_offset, &y_offset, title, (nk_flags) flags));
}

void hinarin_nuklear_group_scrolled_begin(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    struct nk_scroll off = toScroll(the, xsArg(0));
    xsStringValue title = xsToString(xsArg(1));
    xsIntegerValue flags = xsToInteger(xsArg(2));
    xsResult = xsBoolean(nk_group_scrolled_begin(hinarin->nuklear, &off, title, (nk_flags) flags));
}

void hinarin_nuklear_group_scrolled_end(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    nk_group_scrolled_end(hinarin->nuklear);
}

void hinarin_nuklear_tree_push(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsBooleanValue node = xsToBoolean(xsArg(0));
    xsStringValue title = xsToString(xsArg(1));
    xsBooleanValue minimized = xsToBoolean(xsArg(2));
    nk_tree_push(hinarin->nuklear, node ? NK_TREE_NODE : NK_TREE_TAB, title, minimized ? NK_MINIMIZED : NK_MAXIMIZED);
}

void hinarin_nuklear_tree_push_id(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsBooleanValue node = xsToBoolean(xsArg(0));
    xsStringValue title = xsToString(xsArg(1));
    xsBooleanValue minimized = xsToBoolean(xsArg(2));
    xsIntegerValue id = xsToInteger(xsArg(3));
    nk_tree_push_id(hinarin->nuklear, node ? NK_TREE_NODE : NK_TREE_TAB, title, minimized ? NK_MINIMIZED : NK_MAXIMIZED, id);
}

void hinarin_nuklear_tree_push_hashed(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsBooleanValue node = xsToBoolean(xsArg(0));
    xsStringValue title = xsToString(xsArg(1));
    xsBooleanValue minimized = xsToBoolean(xsArg(2));
    xsStringValue hash = xsToString(xsArg(3));
    xsIntegerValue id = xsToInteger(xsArg(4));
    nk_tree_push_hashed(hinarin->nuklear, node ? NK_TREE_NODE : NK_TREE_TAB, title, minimized ? NK_MINIMIZED : NK_MAXIMIZED, hash, (int) strlen(hash), id);
}

void hinarin_nuklear_tree_image_push(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsBooleanValue node = xsToBoolean(xsArg(0));
    struct nk_image *img = xsGetHostData(xsArg(1));
    xsStringValue title = xsToString(xsArg(2));
    xsBooleanValue minimized = xsToBoolean(xsArg(3));
    nk_tree_image_push(hinarin->nuklear, node ? NK_TREE_NODE : NK_TREE_TAB, *img, title, minimized ? NK_MINIMIZED : NK_MAXIMIZED);
}

void hinarin_nuklear_tree_image_push_id(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsBooleanValue node = xsToBoolean(xsArg(0));
    struct nk_image *img = xsGetHostData(xsArg(1));
    xsStringValue title = xsToString(xsArg(2));
    xsBooleanValue minimized = xsToBoolean(xsArg(3));
    xsIntegerValue id = xsToInteger(xsArg(4));
    nk_tree_image_push_id(hinarin->nuklear, node ? NK_TREE_NODE : NK_TREE_TAB, *img, title, minimized ? NK_MINIMIZED : NK_MAXIMIZED, id);
}

void hinarin_nuklear_tree_image_push_hashed(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsBooleanValue node = xsToBoolean(xsArg(0));
    struct nk_image *img = xsGetHostData(xsArg(1));
    xsStringValue title = xsToString(xsArg(2));
    xsBooleanValue minimized = xsToBoolean(xsArg(3));
    xsStringValue hash = xsToString(xsArg(4));
    xsIntegerValue id = xsToInteger(xsArg(5));
    nk_tree_image_push_hashed(hinarin->nuklear, node ? NK_TREE_NODE : NK_TREE_TAB, *img, title, minimized ? NK_MINIMIZED : NK_MAXIMIZED, hash, (int) strlen(hash), id);
}

void hinarin_nuklear_tree_pop(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    nk_tree_pop(hinarin->nuklear);
}

void hinarin_nuklear_tree_state_push(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsBooleanValue node = xsToBoolean(xsArg(0));
    xsStringValue title = xsToString(xsArg(1));
    enum nk_collapse_states state = xsToBoolean(xsArg(2)) ? NK_MINIMIZED : NK_MAXIMIZED;
    nk_tree_state_push(hinarin->nuklear, node ? NK_TREE_NODE : NK_TREE_TAB, title, &state);
}

void hinarin_nuklear_tree_state_image_push(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsBooleanValue node = xsToBoolean(xsArg(0));
    struct nk_image *img = xsGetHostData(xsArg(1));
    xsStringValue title = xsToString(xsArg(2));
    enum nk_collapse_states state = xsToBoolean(xsArg(3)) ? NK_MINIMIZED : NK_MAXIMIZED;
    nk_tree_state_image_push(hinarin->nuklear, node ? NK_TREE_NODE : NK_TREE_TAB, *img, title, &state);
}

void hinarin_nuklear_tree_state_pop(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    nk_tree_state_pop(hinarin->nuklear);
}

void hinarin_nuklear_list_view_new(xsMachine *the) {
    xsResult = xsNewHostObject(free);
    struct nk_list_view *view = calloc(1, sizeof(struct nk_list_view));
    xsSetHostData(xsResult, view);
}

void hinarin_nuklear_list_view_begin(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    struct nk_list_view *view = xsGetHostData(xsArg(0));
    xsStringValue id = xsToString(xsArg(1));
    xsIntegerValue flags = xsToInteger(xsArg(2));
    xsIntegerValue row_height  = xsToInteger(xsArg(3));
    xsIntegerValue row_count  = xsToInteger(xsArg(4));
    xsResult = xsInteger(nk_list_view_begin(hinarin->nuklear, view, id, (nk_flags) flags, row_height, row_count));
}

void hinarin_nuklear_list_view_end(xsMachine *the) {
    struct nk_list_view *view = xsGetHostData(xsArg(0));
    nk_list_view_end(view);
}

void hinarin_nuklear_widget(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    struct nk_rect rect = toRect(the, xsArg(0));
    xsResult = xsInteger(nk_widget(&rect, hinarin->nuklear));
}

void hinarin_nuklear_widget_fitting(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    struct nk_rect rect = toRect(the, xsArg(0));
    struct nk_vec2 vec = toVec(the, xsArg(1));
    xsResult = xsInteger(nk_widget_fitting(&rect, hinarin->nuklear, vec));
}

void hinarin_nuklear_widget_bounds(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsResult = fromRect(the, nk_widget_bounds(hinarin->nuklear));
}

void hinarin_nuklear_widget_position(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsResult = fromVec(the, nk_widget_position(hinarin->nuklear));
}

void hinarin_nuklear_widget_size(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsResult = fromVec(the, nk_widget_size(hinarin->nuklear));
}

void hinarin_nuklear_widget_width(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsResult = xsNumber(nk_widget_width(hinarin->nuklear));
}

void hinarin_nuklear_widget_height(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsResult = xsNumber(nk_widget_height(hinarin->nuklear));
}

void hinarin_nuklear_widget_is_hovered(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsResult = xsBoolean(nk_widget_is_hovered(hinarin->nuklear));
}

void hinarin_nuklear_widget_is_mouse_clicked(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsIntegerValue buttons = xsToInteger(xsArg(0));
    xsResult = xsBoolean(nk_widget_is_mouse_clicked(hinarin->nuklear, (enum nk_buttons) buttons));
}

void hinarin_nuklear_widget_has_mouse_click_down(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsIntegerValue buttons = xsToInteger(xsArg(0));
    xsIntegerValue down = xsToInteger(xsArg(1));
    xsResult = xsBoolean(nk_widget_has_mouse_click_down(hinarin->nuklear, (enum nk_buttons) buttons, down));
}

void hinarin_nuklear_spacing(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsIntegerValue cols = xsToInteger(xsArg(0));
    nk_spacing(hinarin->nuklear, cols);
}

void hinarin_nuklear_text(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsStringValue text = xsToString(xsArg(0));
    xsIntegerValue flags = xsToInteger(xsArg(1));
    nk_text(hinarin->nuklear, text, (int) strlen(text), (nk_flags) flags);
}

void hinarin_nuklear_text_colored(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsStringValue text = xsToString(xsArg(0));
    xsIntegerValue flags = xsToInteger(xsArg(1));
    struct nk_color color = toColor(the, xsArg(2));
    nk_text_colored(hinarin->nuklear, text, (int) strlen(text), (nk_flags) flags, color);
}

void hinarin_nuklear_text_wrap(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsStringValue text = xsToString(xsArg(0));
    nk_text_wrap(hinarin->nuklear, text, (int) strlen(text));
}

void hinarin_nuklear_text_wrap_colored(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsStringValue text = xsToString(xsArg(0));
    struct nk_color color = toColor(the, xsArg(1));
    nk_text_wrap_colored(hinarin->nuklear, text, (int) strlen(text), color);
}

void hinarin_nuklear_label(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsStringValue text = xsToString(xsArg(0));
    xsIntegerValue align = xsToInteger(xsArg(1));
    nk_label(hinarin->nuklear, text, (nk_flags) align);
}

void hinarin_nuklear_label_colored(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsStringValue text = xsToString(xsArg(0));
    xsIntegerValue align = xsToInteger(xsArg(1));
    struct nk_color color = toColor(the, xsArg(2));
    nk_label_colored(hinarin->nuklear, text, (nk_flags) align, color);
}

void hinarin_nuklear_label_wrap(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsStringValue text = xsToString(xsArg(0));
    nk_label_wrap(hinarin->nuklear, text);
}

void hinarin_nuklear_label_colored_wrap(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    xsStringValue text = xsToString(xsArg(0));
    struct nk_color color = toColor(the, xsArg(1));
    nk_label_colored_wrap(hinarin->nuklear, text, color);
}

void hinarin_nuklear_image(xsMachine *the) {
    hinarin_context *hinarin = xsGetContext(the);
    struct nk_image *image = xsGetHostData(xsArg(0));
    nk_image(hinarin->nuklear, *image);
}

void hinarin_bind_nuklear(xsMachine *the) {
    xsSlot nuklear_obj  = xsNewObject();
    xsDefine(xsGlobal, xsID("nuklear"), nuklear_obj, xsChangeAll);
    xsDefine(nuklear_obj, xsID("render"), xsNewHostFunction(hinarin_nuklear_render_add, 1), xsDontSet | xsDontDelete);
    
    xsDefine(nuklear_obj, xsID("begin"), xsNewHostFunction(hinarin_nuklear_begin, 3), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("beginTitled"), xsNewHostFunction(hinarin_nuklear_begin_titled, 4), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("end"), xsNewHostFunction(hinarin_nuklear_end, 0), xsDontSet | xsDontDelete);

    xsDefine(nuklear_obj, xsID("windowFind"), xsNewHostFunction(hinarin_nuklear_window_find, 1), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("windowGetBounds"), xsNewHostFunction(hinarin_nuklear_window_get_bounds, 0), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("windowGetPosition"), xsNewHostFunction(hinarin_nuklear_window_get_position, 0), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("windowGetSize"), xsNewHostFunction(hinarin_nuklear_window_get_size, 0), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("windowGetWidth"), xsNewHostFunction(hinarin_nuklear_window_get_width, 0), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("windowGetHeight"), xsNewHostFunction(hinarin_nuklear_window_get_height, 0), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("windowGetPanel"), xsNewHostFunction(hinarin_nuklear_window_get_panel, 0), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("windowGetContentRegion"), xsNewHostFunction(hinarin_nuklear_window_get_content_region, 0), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("windowGetContentRegionMin"), xsNewHostFunction(hinarin_nuklear_window_get_content_region_min, 0), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("windowGetContentRegionMax"), xsNewHostFunction(hinarin_nuklear_window_get_content_region_max, 0), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("windowGetContentRegionSize"), xsNewHostFunction(hinarin_nuklear_window_get_content_region_size, 0), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("windowGetCanvas"), xsNewHostFunction(hinarin_nuklear_window_get_canvas, 0), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("windowHasFocus"), xsNewHostFunction(hinarin_nuklear_window_has_focus, 0), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("windowIsCollapsed"), xsNewHostFunction(hinarin_nuklear_window_is_collapsed, 1), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("windowIsClosed"), xsNewHostFunction(hinarin_nuklear_window_is_closed, 1), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("windowIsHidden"), xsNewHostFunction(hinarin_nuklear_window_is_hidden, 1), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("windowIsActive"), xsNewHostFunction(hinarin_nuklear_window_is_active, 1), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("windowIsHovered"), xsNewHostFunction(hinarin_nuklear_window_is_hovered, 0), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("windowIsAnyHovered"), xsNewHostFunction(hinarin_nuklear_window_is_any_hovered, 0), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("itemIsAnyActive"), xsNewHostFunction(hinarin_nuklear_item_is_any_active, 0), xsDontSet | xsDontDelete);

    xsDefine(nuklear_obj, xsID("windowSetBounds"), xsNewHostFunction(hinarin_nuklear_window_set_bounds, 2), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("windowSetPosition"), xsNewHostFunction(hinarin_nuklear_window_set_position, 2), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("windowSetSize"), xsNewHostFunction(hinarin_nuklear_window_set_size, 2), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("windowSetFocus"), xsNewHostFunction(hinarin_nuklear_window_set_focus, 1), xsDontSet | xsDontDelete);

    xsDefine(nuklear_obj, xsID("windowClose"), xsNewHostFunction(hinarin_nuklear_window_close, 1), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("windowCollapse"), xsNewHostFunction(hinarin_nuklear_window_collapse, 2), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("windowCollapseIf"), xsNewHostFunction(hinarin_nuklear_window_collapse_if, 3), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("windowShow"), xsNewHostFunction(hinarin_nuklear_window_show, 2), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("windowShowIf"), xsNewHostFunction(hinarin_nuklear_window_show_if, 3), xsDontSet | xsDontDelete);

    xsDefine(nuklear_obj, xsID("WINDOW_BORDER"), xsInteger(NK_WINDOW_BORDER), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("WINDOW_MOVABLE"), xsInteger(NK_WINDOW_MOVABLE), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("WINDOW_SCALABLE"), xsInteger(NK_WINDOW_SCALABLE), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("WINDOW_CLOSABLE"), xsInteger(NK_WINDOW_CLOSABLE), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("WINDOW_MINIMIZABLE"), xsInteger(NK_WINDOW_MINIMIZABLE), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("WINDOW_NO_SCROLLBAR"), xsInteger(NK_WINDOW_NO_SCROLLBAR), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("WINDOW_TITLE"), xsInteger(NK_WINDOW_TITLE), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("WINDOW_SCROLL_AUTO_HIDE"), xsInteger(NK_WINDOW_SCROLL_AUTO_HIDE), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("WINDOW_BACKGROUND"), xsInteger(NK_WINDOW_BACKGROUND), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("WINDOW_SCALE_LEFT"), xsInteger(NK_WINDOW_SCALE_LEFT), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("WINDOW_NO_INPUT"), xsInteger(NK_WINDOW_NO_INPUT), xsDontSet | xsDontDelete);

    xsDefine(nuklear_obj, xsID("layoutSetMinRowHeight"), xsNewHostFunction(hinarin_nuklear_layout_set_min_row_height, 1), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("layoutResetMinRowHeight"), xsNewHostFunction(hinarin_nuklear_layout_reset_min_row_height, 0), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("layoutWidgetBounds"), xsNewHostFunction(hinarin_nuklear_layout_widget_bounds, 0), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("layoutRatioFromPixel"), xsNewHostFunction(hinarin_nuklear_layout_ratio_from_pixel, 1), xsDontSet | xsDontDelete);

    xsDefine(nuklear_obj, xsID("layoutRowDynamic"), xsNewHostFunction(hinarin_nuklear_layout_row_dynamic, 2), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("layoutRowStatic"), xsNewHostFunction(hinarin_nuklear_layout_row_static, 3), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("layoutRowBegin"), xsNewHostFunction(hinarin_nuklear_layout_row_begin, 3), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("layoutRowPush"), xsNewHostFunction(hinarin_nuklear_layout_row_push, 1), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("layoutRowEnd"), xsNewHostFunction(hinarin_nuklear_layout_row_end, 0), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("layoutRow"), xsNewHostFunction(hinarin_nuklear_layout_row, 4), xsDontSet | xsDontDelete);

    xsDefine(nuklear_obj, xsID("layoutRowTemplateBegin"), xsNewHostFunction(hinarin_nuklear_layout_row_template_begin, 1), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("layoutRowTemplatePushDynamic"), xsNewHostFunction(hinarin_nuklear_layout_row_template_push_dynamic, 0), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("layoutRowTemplatePushVariable"), xsNewHostFunction(hinarin_nuklear_layout_row_template_push_variable, 1), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("layoutRowTemplatePushStatic"), xsNewHostFunction(hinarin_nuklear_layout_row_template_push_static, 1), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("layoutRowTemplateEnd"), xsNewHostFunction(hinarin_nuklear_layout_row_template_end, 0), xsDontSet | xsDontDelete);

    xsDefine(nuklear_obj, xsID("layoutSpaceBegin"), xsNewHostFunction(hinarin_nuklear_layout_space_begin, 3), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("layoutSpacePush"), xsNewHostFunction(hinarin_nuklear_layout_space_push, 1), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("layoutSpaceEnd"), xsNewHostFunction(hinarin_nuklear_layout_space_end, 0), xsDontSet | xsDontDelete);

    xsDefine(nuklear_obj, xsID("layoutSpaceBounds"), xsNewHostFunction(hinarin_nuklear_layout_space_bounds, 0), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("layoutSpaceToScreen"), xsNewHostFunction(hinarin_nuklear_layout_space_to_screen, 1), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("layoutSpaceToLocal"), xsNewHostFunction(hinarin_nuklear_layout_space_to_local, 1), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("layoutSpaceRectToScreen"), xsNewHostFunction(hinarin_nuklear_layout_space_rect_to_screen, 1), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("layoutSpaceRectToLocal"), xsNewHostFunction(hinarin_nuklear_layout_space_rect_to_local, 1), xsDontSet | xsDontDelete);

    xsDefine(nuklear_obj, xsID("groupBegin"), xsNewHostFunction(hinarin_nuklear_group_begin, 2), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("groupBeginTitled"), xsNewHostFunction(hinarin_nuklear_group_begin_titled, 3), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("groupEnd"), xsNewHostFunction(hinarin_nuklear_group_end, 0), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("groupScrolledOffsetBegin"), xsNewHostFunction(hinarin_nuklear_group_scrolled_offset_begin, 4), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("groupScrolledBegin"), xsNewHostFunction(hinarin_nuklear_group_scrolled_begin, 3), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("groupScrolledEnd"), xsNewHostFunction(hinarin_nuklear_group_scrolled_end, 0), xsDontSet | xsDontDelete);

    xsDefine(nuklear_obj, xsID("treePush"), xsNewHostFunction(hinarin_nuklear_tree_push, 3), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("treePushId"), xsNewHostFunction(hinarin_nuklear_tree_push_id, 4), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("treePushHashed"), xsNewHostFunction(hinarin_nuklear_tree_push_hashed, 5), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("treeImagePush"), xsNewHostFunction(hinarin_nuklear_tree_image_push, 4), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("treeImagePushId"), xsNewHostFunction(hinarin_nuklear_tree_image_push_id, 5), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("treeImagePushHashed"), xsNewHostFunction(hinarin_nuklear_tree_image_push_hashed, 6), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("treePop"), xsNewHostFunction(hinarin_nuklear_tree_pop, 0), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("treeStatePush"), xsNewHostFunction(hinarin_nuklear_tree_state_push, 3), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("treeStateImagePush"), xsNewHostFunction(hinarin_nuklear_tree_state_image_push, 4), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("treeStatePop"), xsNewHostFunction(hinarin_nuklear_tree_state_pop, 0), xsDontSet | xsDontDelete);

    xsDefine(nuklear_obj, xsID("listViewNew"), xsNewHostFunction(hinarin_nuklear_list_view_new, 0), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("listViewBegin"), xsNewHostFunction(hinarin_nuklear_list_view_begin, 5), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("listViewEnd"), xsNewHostFunction(hinarin_nuklear_list_view_end, 1), xsDontSet | xsDontDelete);

    xsDefine(nuklear_obj, xsID("BUTTON_LEFT"), xsInteger(NK_BUTTON_LEFT), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("BUTTON_MIDDLE"), xsInteger(NK_BUTTON_MIDDLE), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("BUTTON_RIGHT"), xsInteger(NK_BUTTON_RIGHT), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("BUTTON_DOUBLE"), xsInteger(NK_BUTTON_DOUBLE), xsDontSet | xsDontDelete);

    xsDefine(nuklear_obj, xsID("WIDGET_INVALID"), xsInteger(NK_WIDGET_INVALID), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("WIDGET_VALID"), xsInteger(NK_WIDGET_VALID), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("WIDGET_ROM"), xsInteger(NK_WIDGET_ROM), xsDontSet | xsDontDelete);

    xsDefine(nuklear_obj, xsID("widget"), xsNewHostFunction(hinarin_nuklear_widget, 1), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("widgetFitting"), xsNewHostFunction(hinarin_nuklear_widget_fitting, 2), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("widgetBounds"), xsNewHostFunction(hinarin_nuklear_widget_bounds, 0), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("widgetPosition"), xsNewHostFunction(hinarin_nuklear_widget_position, 0), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("widgetSize"), xsNewHostFunction(hinarin_nuklear_widget_size, 0), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("widgetWidth"), xsNewHostFunction(hinarin_nuklear_widget_width, 0), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("widgetHeight"), xsNewHostFunction(hinarin_nuklear_widget_height, 0), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("widgetIsHovered"), xsNewHostFunction(hinarin_nuklear_widget_is_hovered, 0), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("widgetIsMouseClicked"), xsNewHostFunction(hinarin_nuklear_widget_is_mouse_clicked, 1), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("widgetHasMouseClickDown"), xsNewHostFunction(hinarin_nuklear_widget_has_mouse_click_down, 2), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("spacing"), xsNewHostFunction(hinarin_nuklear_spacing, 1), xsDontSet | xsDontDelete);

    xsDefine(nuklear_obj, xsID("TEXT_ALIGN_LEFT"), xsInteger(NK_TEXT_ALIGN_LEFT), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("TEXT_ALIGN_CENTERED"), xsInteger(NK_TEXT_ALIGN_CENTERED), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("TEXT_ALIGN_RIGHT"), xsInteger(NK_TEXT_ALIGN_RIGHT), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("TEXT_ALIGN_TOP"), xsInteger(NK_TEXT_ALIGN_TOP), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("TEXT_ALIGN_MIDDLE"), xsInteger(NK_TEXT_ALIGN_MIDDLE), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("TEXT_ALIGN_BOTTOM"), xsInteger(NK_TEXT_ALIGN_BOTTOM), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("TEXT_LEFT"), xsInteger(NK_TEXT_LEFT), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("TEXT_CENTERED"), xsInteger(NK_TEXT_CENTERED), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("TEXT_RIGHT"), xsInteger(NK_TEXT_RIGHT), xsDontSet | xsDontDelete);

    xsDefine(nuklear_obj, xsID("text"), xsNewHostFunction(hinarin_nuklear_text, 2), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("textColored"), xsNewHostFunction(hinarin_nuklear_text_colored, 3), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("textWrap"), xsNewHostFunction(hinarin_nuklear_text_wrap, 1), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("textWrapColored"), xsNewHostFunction(hinarin_nuklear_text_wrap_colored, 2), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("label"), xsNewHostFunction(hinarin_nuklear_label, 2), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("labelColored"), xsNewHostFunction(hinarin_nuklear_label_colored, 2), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("labelWrap"), xsNewHostFunction(hinarin_nuklear_label_wrap, 1), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("labelColoredWrap"), xsNewHostFunction(hinarin_nuklear_label_colored_wrap, 2), xsDontSet | xsDontDelete);
    xsDefine(nuklear_obj, xsID("image"), xsNewHostFunction(hinarin_nuklear_image, 1), xsDontSet | xsDontDelete);
}
