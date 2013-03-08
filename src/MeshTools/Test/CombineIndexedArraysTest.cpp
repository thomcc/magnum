/*
    Copyright © 2010, 2011, 2012 Vladimír Vondruš <mosra@centrum.cz>

    This file is part of Magnum.

    Magnum is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License version 3
    only, as published by the Free Software Foundation.

    Magnum is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License version 3 for more details.
*/

#include <sstream>
#include <TestSuite/Tester.h>

#include "Types.h"
#include "MeshTools/CombineIndexedArrays.h"

namespace Magnum { namespace MeshTools { namespace Test {

class CombineIndexedArraysTest: public Corrade::TestSuite::Tester {
    public:
        CombineIndexedArraysTest();

        void wrongIndexCount();
        void combine();
};

CombineIndexedArraysTest::CombineIndexedArraysTest() {
    addTests({&CombineIndexedArraysTest::wrongIndexCount,
              &CombineIndexedArraysTest::combine});
}

void CombineIndexedArraysTest::wrongIndexCount() {
    std::stringstream ss;
    Error::setOutput(&ss);
    std::vector<UnsignedInt> array;
    std::vector<UnsignedInt> result = MeshTools::combineIndexedArrays(
        std::tuple<const std::vector<UnsignedInt>&, std::vector<UnsignedInt>&>(std::vector<UnsignedInt>{0, 1, 0}, array),
        std::tuple<const std::vector<UnsignedInt>&, std::vector<UnsignedInt>&>(std::vector<UnsignedInt>{3, 4}, array));

    CORRADE_COMPARE(result.size(), 0);
    CORRADE_COMPARE(ss.str(), "MeshTools::combineIndexedArrays(): index arrays don't have the same length, nothing done.\n");
}

void CombineIndexedArraysTest::combine() {
    std::vector<UnsignedInt> array1{ 0, 1 };
    std::vector<UnsignedInt> array2{ 0, 1, 2, 3, 4 };
    std::vector<UnsignedInt> array3{ 0, 1, 2, 3, 4, 5, 6, 7 };

    std::vector<UnsignedInt> result = MeshTools::combineIndexedArrays(
        std::tuple<const std::vector<UnsignedInt>&, std::vector<UnsignedInt>&>(std::vector<UnsignedInt>{0, 1, 0}, array1),
        std::tuple<const std::vector<UnsignedInt>&, std::vector<UnsignedInt>&>(std::vector<UnsignedInt>{3, 4, 3}, array2),
        std::tuple<const std::vector<UnsignedInt>&, std::vector<UnsignedInt>&>(std::vector<UnsignedInt>{6, 7, 6}, array3));

    CORRADE_COMPARE(result, (std::vector<UnsignedInt>{0, 1, 0}));
    CORRADE_COMPARE(array1, (std::vector<UnsignedInt>{0, 1}));
    CORRADE_COMPARE(array2, (std::vector<UnsignedInt>{3, 4}));
    CORRADE_COMPARE(array3, (std::vector<UnsignedInt>{6, 7}));
}

}}}

CORRADE_TEST_MAIN(Magnum::MeshTools::Test::CombineIndexedArraysTest)
