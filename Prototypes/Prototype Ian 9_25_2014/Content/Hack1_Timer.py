import Zero
import Events
import Property
import VectorMath

DestroyDelay = .9

class Hack1_Timer:
    def DefineProperties(self):
        #self.Lives = Property.Int(9)
        pass
        
    def Initialize(self, initializer):
        Zero.Connect(self.Space, Events.LogicUpdate, self.OnLogicUpdate)
        self.DestroyTimer = 0.0
        self.Player = self.Space.FindObjectByName("Player")
      
    def OnLogicUpdate(self, UpdateEvent):
        
        self.DestroyTimer += UpdateEvent.Dt
                
        if(self.DestroyTimer > DestroyDelay):
            self.DestroyTimer = 0.0
            self.Player.HackStats.Hack1_Return = 1


Zero.RegisterComponent("Hack1_Timer", Hack1_Timer)