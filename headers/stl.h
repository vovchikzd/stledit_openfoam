#ifndef STL_H
#define STL_H

#include <vector>

struct Facet {
    float normal[3]{};
    float first_vertex[3]{};
    float second_vertex[3]{};
    float third_vertex[3]{};
    short attribute = 0;
};


struct STL {
    char header[80];
    std::vector<Facet> facets;
};


#endif
