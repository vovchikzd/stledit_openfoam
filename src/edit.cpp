#include "edit.h"

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

void axis_rotate(Facet& facet, Matrix<float, 3, 3>& rotate_matrix) {

    Matrix<float, 4, 3> matrix_facet(facet.normal, facet.normal + 3);
    matrix_facet.push_back(facet.first_vertex, facet.first_vertex + 3);
    matrix_facet.push_back(facet.second_vertex, facet.second_vertex + 3);
    matrix_facet.push_back(facet.third_vertex, facet.third_vertex + 3);

    auto new_facet = matrix_facet * rotate_matrix;

    for (int i = 0; i < 3; ++i) {
        facet.normal[i] = new_facet(0, i);
        facet.first_vertex[i] = new_facet(1, i);
        facet.second_vertex[i] = new_facet(2, i);
        facet.third_vertex[i] = new_facet(3, i);
    }
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

// The more common rotation matrices are transposed for more convenient
// multiplication in the following steps
void get_axis_and_rotate(Facet& facet, const char& axis, const float& angle) {

    if (axis == 'x') {
        Matrix<float, 3, 3> x_rotate{1, 0, 0,
                                     0, std::cos(rad(angle)), std::sin(rad(angle)),
                                     0, -(std::sin(rad(angle))), std::cos(rad(angle))};
        axis_rotate(facet, x_rotate);
    } else if (axis == 'y') {
        Matrix<float, 3, 3> y_rotate{std::cos(rad(angle)), 0, -(std::sin(rad(angle))),
                                     0, 1, 0,
                                     std::sin(rad(angle)), 0, std::cos(rad(angle))};
        axis_rotate(facet, y_rotate);
    } else if (axis == 'z') {
        Matrix<float, 3, 3> z_rotate{std::cos(rad(angle)), std::sin(rad(angle)), 0,
                                     -(std::sin(rad(angle))), std::cos(rad(angle)), 0,
                                     0, 0, 1};
        axis_rotate(facet, z_rotate);
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
    if(axes.empty()) {
        std::string default_order = "xyz";
        for (size_t i = 0, end = angles.size(); i < end; ++i) {
            get_axis_and_rotate(facet, default_order[i], angles[i]);
        }
    } else {
        if (angles.size() == 1) {
            auto angle = angles[0];
            for (auto axis: axes) {
                get_axis_and_rotate(facet, axis, angle);
            }
        } else {           
            for (size_t i = 0, end = axes.size(); i < end; ++i) {
                get_axis_and_rotate(facet, axes[i], angles[i]);
            }
        }
    }
}

void edit(const Parser& parser, std::vector<STL>& objects) {
    for (auto object: objects) {
        for (auto facet: object.facets) {

            if (parser.is_rotate) {
                rotate(facet, parser.rotate_order, parser.rotate_angles);
            }

            if (parser.is_move) {
                move(facet, parser.move_axes, parser.move_lengths);
            }

            if (parser.is_scale) {
                scale(facet, parser.scale_axes, parser.scale_values);
            }
        } 
    }
}
