from __future__ import annotations

import garmi_head._core as m


def test_add():
    assert m.add(2, 3) == 5


def test_subtract():
    assert m.subtract(7, 5) == 2
