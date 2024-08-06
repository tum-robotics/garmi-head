import time
import garmi_head

ctrl = garmi_head.Control()

for i in range(10):
    print(ctrl.get_position(), ctrl.get_velocity())
    time.sleep(0.1)
