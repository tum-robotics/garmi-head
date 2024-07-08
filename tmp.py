from garmi_head import _core
import time

ctrl = _core.Control(800,50,0.001)

ctrl.set_control(-0.6,0.2)

time.sleep(2)

ctrl.set_control(0, 0)

for i in range(200):
    print(ctrl.get_position())
    time.sleep(0.1)

ctrl.stop()
