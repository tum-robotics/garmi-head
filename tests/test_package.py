from __future__ import annotations

import importlib.metadata

import garmi_head as m


def test_version():
    assert importlib.metadata.version("garmi_head") == m.__version__
