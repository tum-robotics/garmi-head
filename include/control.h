#include <condition_variable>
#include <driver.h>
#include <iostream>
#include <mutex>
#include <optional>
#include <pybind11/stl.h>
#include <sched.h>
#include <string>
#include <thread>
#include <vector>

using franka_joint_driver::Driver;

namespace head_control {

class State {
public:
  double theta_[2] = {0.0, 0.0};
  double dtheta_[2] = {0.0, 0.0};
  double theta_target_[2] = {0.0, 0.0};
  double tau_j_[2] = {0.0, 0.0};
  double tau_j_d_[2] = {0.0, 0.0};
  std::mutex mux_;

  void set_theta(int index, double theta) {
    mux_.lock();
    theta_[index] = theta;
    mux_.unlock();
  }

  double get_theta(int index) {
    mux_.lock();
    double theta = theta_[index];
    mux_.unlock();
    return theta;
  }

  void set_dtheta(int index, double dtheta) {
    mux_.lock();
    dtheta_[index] = dtheta;
    mux_.unlock();
  }

  double get_dtheta(int index) {
    mux_.lock();
    double dtheta = dtheta_[index];
    mux_.unlock();
    return dtheta;
  }

  void set_theta_target(int index, double theta_target) {
    mux_.lock();
    theta_target_[index] = theta_target;
    mux_.unlock();
  }

  void set_theta_targets(double theta_target_1, double theta_target_2) {
    mux_.lock();
    theta_target_[0] = theta_target_1;
    theta_target_[1] = theta_target_2;
    mux_.unlock();
  }

  double get_theta_target(int index) {
    mux_.lock();
    double theta_goal = theta_target_[index];
    mux_.unlock();
    return theta_goal;
  }

  void set_tau_j(int index, double tau_j) {
    mux_.lock();
    tau_j_[index] = tau_j;
    mux_.unlock();
  }

  double get_tau_j(int index) {
    mux_.lock();
    double tau_j = tau_j_[index];
    mux_.unlock();
    return tau_j;
  }

  void set_tau_j_d(int index, double tau_j_d) {
    mux_.lock();
    tau_j_d_[index] = tau_j_d;
    mux_.unlock();
  }

  double get_tau_j_d(int index) {
    mux_.lock();
    double tau_j_d = tau_j_d_[index];
    mux_.unlock();
    return tau_j_d;
  }
};

class HeadControlNode {
public:
  State s_;
  double k_p_, k_p_target_, k_d_, k_d_target_, pt1_filter_, pt1_filter_target_;
  std::mutex mux_;
  bool running_ = true;
  std::thread control_thread_;
  std::condition_variable event_cv_;
  bool event_signaled_ = false;

  HeadControlNode(const double k_p = 800.0, const double k_d = 50.0,
                  const double pt1_filter = 0.001)
      : k_p_(k_p), k_d_(k_d), pt1_filter_(pt1_filter), k_p_target_(k_p),
        k_d_target_(k_d), pt1_filter_target_(pt1_filter) {
    // Initialize control loop
    init_controller();

    // Wait for the initialization to complete
    std::unique_lock<std::mutex> lock(mux_);
    event_cv_.wait(lock, [this] { return event_signaled_; });
  }

  ~HeadControlNode() { stop(); }

  void stop() {
    running_ = false;
    if (control_thread_.joinable()) {
      control_thread_.join();
    }
  }

  void set_control(const double pan, const double tilt) {
    s_.set_theta_targets(std::min(0.6, std::max(-0.6, pan)),
                         std::min(0.2, std::max(-0.2, tilt)));
  }

  int init_controller() {
    // Control thread
    control_thread_ =
        std::thread(std::bind(&HeadControlNode::control_loop, this));
    const int kThreadPriority = sched_get_priority_max(SCHED_FIFO);
    if (kThreadPriority == -1) {
      std::cout << "Unable to get maximum possible thread priority: ";
      return 1;
    }
    sched_param thread_param{};
    thread_param.sched_priority = kThreadPriority;
    if (pthread_setschedparam(control_thread_.native_handle(), SCHED_FIFO,
                              &thread_param) != 0) {
      std::cout << "Unable to set realtime scheduling";
      return 1;
    }
    control_thread_.detach();
  }

  int control_loop() {
    int num_slaves_ = 2;

    // Initializes the driver
    Driver driver;
    switch (driver.init()) {
    case Driver::Error::kNoError: {
      break;
    };
    case Driver::Error::kCommunicationError: {
      std::cout << "Driver initialization failed" << std::endl;
      std::cout << "Communication error" << std::endl;
      std::cout << "Have you used 'sudo ethercatctl start' ? " << std::endl;
      return 1;
      break;
    }
    default: {
      std::cout << "Unknown error happened." << std::endl;
      return 1;
    }
    }
    bool initial_position_set[num_slaves_] = {false};
    double theta_previous[num_slaves_];
    auto last = std::chrono::steady_clock::now();
    std::chrono::duration<double> diff;
    std::vector<double> theta_d{0.0, 0.0};
    // Control callback is used to read state from and write commands into
    // hardware
    franka_joint_driver::Driver::CallbackFunctionTorque control_callback =
        [&](const std::vector<Driver::State> &state,
            std::vector<Driver::CommandTorque> &command) -> void {
      for (size_t joint_number = 0; joint_number < num_slaves_;
           joint_number++) {

        // Initializes position values only in first sample
        if (!initial_position_set[joint_number]) {
          initial_position_set[joint_number] = true;
          theta_d[joint_number] = state[joint_number].theta;
          theta_previous[joint_number] = state[joint_number].theta;
          s_.set_theta_target(joint_number, state[joint_number].theta);
          last = std::chrono::steady_clock::now();
          if (initial_position_set[0] && initial_position_set[1]) {
            {
              std::lock_guard<std::mutex> lock(mux_);
              event_signaled_ = true;
            }
            event_cv_.notify_one();
          }
        }
        diff = std::chrono::steady_clock::now() - last;

        double dtheta_d = 0;
        double theta = state[joint_number].theta;
        double dtheta = (theta - theta_previous[joint_number]) / diff.count();
        theta_d[joint_number] = theta_d[joint_number] * (1.0 - pt1_filter_) +
                                pt1_filter_ * s_.get_theta_target(joint_number);
        double tau_j = std::min(
            15.0, std::max(-15.0, (theta_d[joint_number] - theta) * k_p_ +
                                      (dtheta_d - dtheta) * k_d_));
        s_.set_dtheta(joint_number, dtheta);
        s_.set_theta(joint_number, theta);
        s_.set_tau_j(joint_number, state[joint_number].tau_j);
        s_.set_tau_j_d(joint_number, tau_j);

        command[joint_number].tau_j_d = tau_j;
        theta_previous[joint_number] = theta;
        if (!running_) {
          command[joint_number].finished = true;
        }
      }
      mux_.lock();
      k_p_ = k_p_ * (1.0 - 0.001) + k_p_target_ * 0.001;
      k_d_ = k_d_ * (1.0 - 0.001) + k_d_target_ * 0.001;
      pt1_filter_ = pt1_filter_ * (1.0 - 0.001) + pt1_filter_target_ * 0.001;
      mux_.unlock();
      last = std::chrono::steady_clock::now();
    };
    // Uses control callback defined in previous section to control the
    // hardware. This method does not return unless an error is detected.
    if (driver.control(control_callback) ==
        Driver::Error::kCommunicationError) {
      std::cout << "Communication error" << std::endl;
      init_controller();
    }
  }
};
} // namespace head_control
