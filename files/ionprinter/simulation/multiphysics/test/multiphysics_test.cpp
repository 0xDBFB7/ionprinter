// FooTest.cpp
#include "Foo.hpp"
#include "QUnit.hpp"
#include <iostream>

int main() {
    QUnit::UnitTest qunit(std::cerr, QUnit::normal);

    Foo f;
    QUNIT_IS_EQUAL(42, f.theAnswer());

    return qunit.errors();
}
