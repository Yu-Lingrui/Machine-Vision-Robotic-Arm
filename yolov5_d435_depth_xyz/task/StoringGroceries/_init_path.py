import sys
import platform
import os
from os.path import dirname, abspath
current_path = os.path.dirname(os.path.abspath(__file__))
sys.path.append(dirname(dirname(dirname(abspath(__file__)))))