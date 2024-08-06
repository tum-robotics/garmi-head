"""

Copyright (c) 2024 Jean Elsner. All rights reserved.

garmi-head: Python control module for the GARMI robot head.
"""
from __future__ import annotations
from garmi_head.control import Control
from . import _version
from . import control
__all__: list = ['__version__', 'Control']
__version__: str = '0.1.dev12+g11087fd'
