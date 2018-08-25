// Bring in gtest
#include <gtest/gtest.h>

#include "imm_raukf.h"

TEST(ImmRaukf, initializeProperly)
{
  double px        = 0.7;
  double py        = 10.9;
  double timestamp = 111111;
  int target_id    = 1;
  Eigen::VectorXd init_meas = Eigen::VectorXd(2);
  init_meas << px, py;
  IMM_RAUKF ukf;
  ukf.initialize(init_meas, timestamp, target_id);
  EXPECT_DOUBLE_EQ( 0.7, ukf.x_merge_(0));
  EXPECT_DOUBLE_EQ(10.9, ukf.z_pred_cv_(1));
}

TEST(ImmRaukf, initializeCovarianceQProperly)
{
  IMM_RAUKF ukf;
  double dt  = 0.1;
  double yaw = 0;
  ukf.initCovarQs(dt, yaw);
  EXPECT_NEAR(6.920451055917801e-310, ukf.q_cv_(0,0), 0.00001);
}

TEST(ImmRaukf, checkPredictionForSUKF)
{
  double px        = -9.61675;
  double py        = -3.49989;
  double timestamp = 1.31701e+09;
  int target_id = 1;
  Eigen::VectorXd init_meas = Eigen::VectorXd(2);
  init_meas << px, py;
  IMM_RAUKF ukf;
  ukf.initialize(init_meas, timestamp, target_id);
  double dt  = 0.103288;
  ukf.predictionSUKF(dt);
  EXPECT_NEAR(-9.61675, ukf.x_ctrv_(0,0), 0.00001);
  EXPECT_NEAR(0.959423, ukf.k_ctrv_(0,0), 0.00001);
}

TEST(ImmRaukf, checkUpdateForSUKF)
{
  //frame 0-------
  double px        = -9.61675;
  double py        = -3.49989;
  double timestamp = 1.31701e+09;
  int target_id = 1;
  Eigen::VectorXd init_meas = Eigen::VectorXd(2);
  init_meas << px, py;
  IMM_RAUKF ukf;
  ukf.initialize(init_meas, timestamp, target_id);
  // frame 1-----
  double dt  = 0.103288;
  ukf.predictionSUKF(dt);
  std::vector<autoware_msgs::DetectedObject> object_vec;
  autoware_msgs::DetectedObject dd;
  dd.pose.position.x = -9.86449 ;
  dd.pose.position.y =  -2.81236;
  object_vec.push_back(dd);
  ukf.updateSUKF(object_vec);
  double tested_x =ukf.x_ctrv_(0,0);
  double tested_p = ukf.p_ctrv_(0,0);
  EXPECT_NEAR(-9.8544286784548873, tested_x, 0.00001);
  EXPECT_NEAR(0.0217008, tested_p, 0.00001);
}

// Run all the tests that were declared with TEST()
int main(int argc, char **argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}