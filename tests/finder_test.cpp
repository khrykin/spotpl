//
// Created by Dmitry Khrykin on 07.10.2020.
//

#include "finder.h"
#include <catch2/catch.hpp>

using namespace spotpl;

TEST_CASE("Finder", "[finder]") {
    auto pl = playlist();
    auto f = finder(pl, "/some/path");
    // Test publish 1
}