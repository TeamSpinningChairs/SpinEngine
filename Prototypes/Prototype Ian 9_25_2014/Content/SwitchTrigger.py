import Zero
import Events
import Property
import VectorMath

class SwitchTrigger:
    def Initialize(self, initializer):
        Zero.Connect(self.Owner, Events.CollisionStarted, self.OnCollisionStarted)
        self.Player = self.Space.FindObjectByName("Player")
    
    def OnCollisionStarted(self, CollisionEvent):
        otherObject = CollisionEvent.OtherObject
        if(otherObject.Name == "Bullet" or otherObject.Name == "Villain"):
            
            if(self.Owner.Switch_Stats.active == 0):
                self.Owner.Switch_Stats.active = 1
            
            else:
                self.Owner.Switch_Stats.active = 0
            
            if(otherObject.Name == "Bullet"):
                otherObject.Destroy()
                self.Player.HackStats.Hack1_Active = 0
                self.Player.HackStats.Hack2_Active = 0


Zero.RegisterComponent("SwitchTrigger", SwitchTrigger)