import Zero
import Events
import Property
import VectorMath

DestroyDelay = .5

class BulletTimer:
    def DefineProperties(self):
        self.HackNum = Property.Int(0)
    
    def Initialize(self, initializer):
        Zero.Connect(self.Space, Events.LogicUpdate, self.OnLogicUpdate)
        self.DestroyTimer1 = 0.0
        self.DestroyTimer2 = 0.0
        self.Player = self.Space.FindObjectByName("Player")
      
    def OnLogicUpdate(self, UpdateEvent):
        
        if(self.HackNum == 0):
            self.DestroyTimer1 += UpdateEvent.Dt
        
        if(self.HackNum == 1):
            self.DestroyTimer2 += UpdateEvent.Dt
        
        if(self.DestroyTimer1 > DestroyDelay):
            self.DestroyTimer1 = 0.0
            self.Player.HackStats.Hack1_Return = 1
                
        elif(self.DestroyTimer2 > DestroyDelay):
            self.DestroyTimer2 = 0.0
            self.Player.HackStats.Hack2_Return = 1
            
Zero.RegisterComponent("BulletTimer", BulletTimer)