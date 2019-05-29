#include "multiphysics.hpp"
#include "catch.hpp"
#include <iostream>

int main() {
    QUnit::UnitTest qunit(std::cerr, QUnit::normal);

    vector<bool> mesh_present(10000);
    double bounds[6];
    int mesh_geometry[3];
    float mesh_scale[3];
    float translate[3] = {0};
    import_mesh("../10x10x10_cube.stl",mesh_present,bounds,translate);

    // QUNIT_IS_EQUAL(42, 42);

    return qunit.errors();
}
