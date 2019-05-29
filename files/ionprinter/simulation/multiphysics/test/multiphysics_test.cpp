#include "multiphysics.hpp"
#include "QUnit.hpp"
#include <iostream>

int main() {
    QUnit::UnitTest qunit(std::cerr, QUnit::normal);

    QUNIT_IS_EQUAL(42, 41);

    return qunit.errors();
}
