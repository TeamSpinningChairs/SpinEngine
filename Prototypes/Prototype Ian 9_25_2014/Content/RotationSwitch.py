import Zero
import Events
import Property
import VectorMath

class RotationSwitch:
    def DefineProperties(self):
        #self.Lives = Property.Int(9)
        pass

    def Initialize(self, initializer):
        Zero.Connect(self.Space, Events.LogicUpdate, self.OnLogicUpdate)
        
    def OnLogicUpdate(self, UpdateEvent):

        if(self.Owner.Switch_Stats.active == 1):
            self.Owner.Transform.Rotation = VectorMath.Quat(0,0,(3.14159/2))
        else:
            self.Owner.Transform.Rotation = VectorMath.Quat(0,0,0)

Zero.RegisterComponent("RotationSwitch", RotationSwitch)