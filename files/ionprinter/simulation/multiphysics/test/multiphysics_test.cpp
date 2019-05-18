#include "multiphysics.hpp"
#include "QUnit.hpp"
#include <iostream>

int main() {
    QUnit::UnitTest qunit(std::cerr, QUnit::normal);

    QUNIT_IS_EQUAL(42, 42);

    return qunit.errors();
}
