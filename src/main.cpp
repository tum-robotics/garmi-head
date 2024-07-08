#include <pybind11/pybind11.h>
#include "control.h"

int add(int i, int j) { return i + j; }

namespace py = pybind11;

PYBIND11_MODULE(_core, m) {

  py::class_<head_control::HeadControlNode>(m, "Control")
      .def(py::init<const double, const double, const double>(),
           py::arg("k_p") = 800.0,
           py::arg("k_d") = 50.0,
           py::arg("pt1_filter") = 0.001)
      .def("stop", &head_control::HeadControlNode::stop)
      .def("set_control", &head_control::HeadControlNode::set_control, py::call_guard<py::gil_scoped_release>(), py::arg("pan"), py::arg("tilt"))
      .def("get_position", [](head_control::HeadControlNode &self) {
           // Lambda function to get both theta values
           std::vector<double> position;
           position.push_back(self.s_.get_theta(0));
           position.push_back(self.s_.get_theta(1));
           return position;
      }, py::call_guard<py::gil_scoped_release>());
    //   .def("get_duration", &motion::JointTrajectory::getDuration)
    //   .def("get_joint_positions", &motion::JointTrajectory::getJointPositions,
    //        py::arg("time"), py::arg("q") = kinematics::kQDefault,
    //        py::arg("q7") = M_PI_4)
    //   .def("get_joint_velocities", &motion::JointTrajectory::getJointVelocities,
    //        py::arg("time"), py::arg("q") = kinematics::kQDefault,
    //        py::arg("q7") = M_PI_4)
    //   .def("get_joint_accelerations",
    //        &motion::JointTrajectory::getJointAccelerations, py::arg("time"),
    //        py::arg("q") = kinematics::kQDefault, py::arg("q7") = M_PI_4);

  m.doc() = R"pbdoc(
      Pybind11 example plugin
      -----------------------
      .. currentmodule:: python_example
      .. autosummary::
         :toctree: _generate
         add
         subtract
  )pbdoc";

  m.def("add", &add, R"pbdoc(
      Add two numbers
      Some other explanation about the add function.
  )pbdoc");

  m.def("subtract", [](int i, int j) { return i - j; }, R"pbdoc(
      Subtract two numbers
      Some other explanation about the subtract function.
  )pbdoc");
}
