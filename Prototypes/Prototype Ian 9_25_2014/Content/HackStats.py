import Zero
import Events
import Property
import VectorMath

class HackStats:
    def DefineProperties(self):
        self.Hack1_Active = Property.Int(0)
        self.Hack2_Active = Property.Int(0)
        
        self.Hack1_Return = Property.Int(0)
        self.Hack2_Return = Property.Int(0)

Zero.RegisterComponent("HackStats", HackStats)