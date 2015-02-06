import Zero
import Events
import Property
import VectorMath

class Switch_Stats:
    def DefineProperties(self):
        self.active = Property.Int(0)
        
        # if hold is set to 1 
        self.Hack_Hold = Property.Int(0)
        
Zero.RegisterComponent("Switch_Stats", Switch_Stats)