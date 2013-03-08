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

#include "Physics/Capsule.h"
#include "Physics/Point.h"
#include "Physics/Sphere.h"

#include "ShapeTestBase.h"

namespace Magnum { namespace Physics { namespace Test {

class CapsuleTest: public Corrade::TestSuite::Tester, ShapeTestBase {
    public:
        CapsuleTest();

        void applyTransformation();
        void collisionPoint();
        void collisionSphere();
};

CapsuleTest::CapsuleTest() {
    addTests({&CapsuleTest::applyTransformation,
              &CapsuleTest::collisionPoint});
}

void CapsuleTest::applyTransformation() {
    Physics::Capsule3D capsule({1.0f, 2.0f, 3.0f}, {-1.0f, -2.0f, -3.0f}, 7.0f);

    capsule.applyTransformationMatrix(Matrix4::rotation(Deg(90.0f), Vector3::zAxis()));
    CORRADE_COMPARE(capsule.transformedA(), Vector3(-2.0f, 1.0f, 3.0f));
    CORRADE_COMPARE(capsule.transformedB(), Vector3(2.0f, -1.0f, -3.0f));
    CORRADE_COMPARE(capsule.radius(), 7.0f);

    /* Apply average scaling to radius */
    capsule.applyTransformationMatrix(Matrix4::scaling({Constants::sqrt3(), -Constants::sqrt2(), 2.0f}));
    CORRADE_COMPARE(capsule.transformedRadius(), Constants::sqrt3()*7.0f);
}

void CapsuleTest::collisionPoint() {
    Physics::Capsule3D capsule({-1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, 2.0f);
    Physics::Point3D point({2.0f, 0.0f, 0.0f});
    Physics::Point3D point1({2.9f, 1.0f, 0.0f});
    Physics::Point3D point2({1.0f, 3.1f, 0.0f});

    randomTransformation(capsule);
    randomTransformation(point);
    randomTransformation(point1);
    randomTransformation(point2);

    VERIFY_COLLIDES(capsule, point);
    VERIFY_COLLIDES(capsule, point1);
    VERIFY_NOT_COLLIDES(capsule, point2);
}

void CapsuleTest::collisionSphere() {
    Physics::Capsule3D capsule({-1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, 2.0f);
    Physics::Sphere3D sphere({3.0f, 0.0f, 0.0f}, 0.9f);
    Physics::Sphere3D sphere1({3.5f, 1.0f, 0.0f}, 0.6f);
    Physics::Sphere3D sphere2({1.0f, 4.1f, 0.0f}, 1.0f);

    randomTransformation(capsule);
    randomTransformation(sphere);
    randomTransformation(sphere1);
    randomTransformation(sphere2);

    VERIFY_COLLIDES(capsule, sphere);
    VERIFY_COLLIDES(capsule, sphere1);
    VERIFY_NOT_COLLIDES(capsule, sphere2);
}

}}}

CORRADE_TEST_MAIN(Magnum::Physics::Test::CapsuleTest)
