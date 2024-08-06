#include "control.h"
#include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_MODULE(control, m) {
  py::class_<head_control::HeadControlNode>(m, "Control", R"delim(
      Initializes a control loop with the GARMI head.
  )delim")
      .def(py::init<const double, const double, const double>(),
           py::arg("k_p") = 800.0, py::arg("k_d") = 50.0,
           py::arg("pt1_filter") = 0.001)
      .def("stop", &head_control::HeadControlNode::stop)
      .def("set_control", &head_control::HeadControlNode::set_control,
           py::call_guard<py::gil_scoped_release>(), py::arg("pan"),
           py::arg("tilt"))
      .def(
          "get_position",
          [](head_control::HeadControlNode &self) {
            // Lambda function to get both theta values
            std::vector<double> position;
            position.push_back(self.s_.get_theta(0));
            position.push_back(self.s_.get_theta(1));
            return position;
          },
          py::call_guard<py::gil_scoped_release>())
      .def(
          "get_velocity",
          [](head_control::HeadControlNode &self) {
            // Lambda function to get both theta values
            std::vector<double> velocity;
            velocity.push_back(self.s_.get_dtheta(0));
            velocity.push_back(self.s_.get_dtheta(1));
            return velocity;
          },
          py::call_guard<py::gil_scoped_release>());
}
