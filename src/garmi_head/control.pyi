"""

      Pybind11 example plugin
      -----------------------
      .. currentmodule:: python_example
      .. autosummary::
         :toctree: _generate
         add
         subtract
  
"""
from __future__ import annotations
__all__ = ['Control']
class Control:
    def __init__(self, k_p: float = 800.0, k_d: float = 50.0, pt1_filter: float = 0.001) -> None:
        ...
    def get_position(self) -> list[float]:
        ...
    def get_velocity(self) -> list[float]:
        ...
    def set_control(self, pan: float, tilt: float) -> None:
        ...
    def stop(self) -> None:
        ...
