"""
Copyright (c) 2024 Jean Elsner. All rights reserved.

garmi-head: Python control module for the GARMI robot head.
"""

from __future__ import annotations

from ._version import version as __version__

from .control import Control

__all__ = ["__version__", "Control"]
