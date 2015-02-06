import Zero
import Events
import Property
import VectorMath

class HackDestroy:
    def DefineProperties(self):
        self.HackNum = Property.Int(0)
        pass

    def Initialize(self, initializer):
        Zero.Connect(self.Owner, Events.CollisionStarted, self.OnCollisionStarted)
        self.Player = self.Space.FindObjectByName("Player")
    
    def OnCollisionStarted(self, CollisionEvent):
        otherObject = CollisionEvent.OtherObject
        if(otherObject.Name == "Player"):

            if(self.HackNum == 0 and self.Player.HackStats.Hack1_Return == 1):
                print("hack 1 return")
                self.Owner.Destroy()
                self.Player.HackStats.Hack1_Return = 0
                self.Player.HackStats.Hack1_Active = 0
            
            elif(self.HackNum == 1 and self.Player.HackStats.Hack2_Return == 1):
                print("hack 2 return")
                self.Owner.Destroy()
                self.Player.HackStats.Hack2_Return = 0
                self.Player.HackStats.Hack2_Active = 0
                
Zero.RegisterComponent("HackDestroy", HackDestroy)