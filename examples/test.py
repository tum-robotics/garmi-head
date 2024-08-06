from __future__ import annotations

import time

import garmi_head

ctrl = garmi_head.Control()

for __ in range(10):
    print(ctrl.get_position(), ctrl.get_velocity())  # noqa: T201
    time.sleep(0.1)
