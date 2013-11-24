#include "gtest/gtest.h"

#include <Eigen/Core>

#include "../src/Environment.h"
#include "../src/Shape.h"
#include "../src/Force.h"
#include "../src/Point.h"


using namespace Eigen;

class EnvironmentTest : public ::testing::Test {
};

TEST_F(EnvironmentTest, Sphere) {
  Environment env;
  Sphere sphere(2.0f);
  env.shape_list.push_back(&sphere);

  // these forces intersect
  env.force_list.push_back(new Force(Point(0.0f, 0.0f, 0.0f), Vector3f(1.0f, 1.0f, 1.0f)));
  env.force_list.push_back(new Force(Point(-1.0f, -1.0f, -1.0f), Vector3f(1.0f, 1.0f, 1.0f)));
  env.force_list.push_back(new Force(Point(-2.0f, -2.0f, -2.0f), Vector3f(1.0f, 1.0f, 1.0f)));

  // this force doesnt intersect
  env.force_list.push_back(new Force(Point(-3.0f, -3.0f, -3.0f), Vector3f(-3.0f, -3.0f, -3.0f)));

  ASSERT_EQ(Force(Point(0.0f, 0.0f, 0.0f), Vector3f(3.0f, 3.0f, 3.0f)), env.externalForce(&sphere));
}
