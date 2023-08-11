#include "editor.h"

void axis_move(Facet& facet, const int& axis, const float& length) {
    facet.normal[axis] += length;
    facet.first_vertex[axis] += length;
    facet.second_vertex[axis] += length;
    facet.third_vertex[axis] += length;
}

void axis_scale(Facet& facet, const int& axis, const float& value) {
    facet.normal[axis] *= value;
    facet.first_vertex[axis] *= value;
    facet.second_vertex[axis] *= value;
    facet.third_vertex[axis] *= value;
}

void get_axis_and_move(Facet& facet, const char& axis, const float& length) {

    if (axis == 'x') {
        axis_move(facet, 0, length);
    } else if (axis == 'y') {
        axis_move(facet, 1, length);
    } else if (axis == 'z') {
        axis_move(facet, 2, length);
    }  

}

void get_axis_and_scale(Facet& facet, const char& axis, const float& value) {

    if (axis == 'x') {
        axis_scale(facet, 0, value);
    } else if (axis == 'y') {
        axis_scale(facet, 1, value);
    } else if (axis == 'z') {
        axis_scale(facet, 2, value);
    }  

}

void move(Facet& facet, const std::string& axes,
                        const std::vector<float>& lengths) {

    if (axes.empty()) {
        std::string default_order = "xyz";
        for (size_t i = 0, end = lengths.size(); i < end; ++i) {
            get_axis_and_move(facet, default_order[i], lengths[i]);
        }
    } else {
        if (lengths.size() == 1) {
            auto length = lengths[0];
            for (auto axis: axes) {
                get_axis_and_move(facet, axis, length);
            }
        } else {
            for (size_t i = 0, end = axes.size(); i < end; ++i) {
                get_axis_and_move(facet, axes[i], lengths[i]);
            }
        }
    }

}

void scale(Facet& facet, const std::string& axes,
                         const std::vector<float>& values) {
    if (values.size() == 1 && axes.empty()) {
        std::string default_order = "xyz";
        float value = values[0];
        for (auto axis: default_order) {
            get_axis_and_scale(facet, axis, value);
        }
    } else if (values.size() > 1 && axes.empty()) {
        std::string default_order = "xyz";
        for (size_t i = 0, end = values.size(); i < end; ++i) {
            get_axis_and_scale(facet, default_order[i], values[i]);
        }
    } else {
        for (size_t i = 0, end = axes.size(); i < end; ++i) {
            get_axis_and_scale(facet, axes[i], values[i]);
        }
    }
}

void rotate(Facet& facet, const std::string& axes,
                          const std::vector<float>& angles) {
    // need to implement 'matrix' probably
}
