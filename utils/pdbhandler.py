# -*- coding: utf-8 -*-

try:
    from PySide import QtCore
except:
    from PyQt4.QtCore import pyqtSlot as Slot
    from PyQt4 import QtCore

class PDBHandler(QObject):
    def __init__(self):
        QObject.__init__(self)
        pass
