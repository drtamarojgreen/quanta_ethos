import gdb

class SmartPointerPrinter:
    def __init__(self, val):
        self.val = val

    def to_string(self):
        return self.val['px'].dereference()

# Registration logic would go here
