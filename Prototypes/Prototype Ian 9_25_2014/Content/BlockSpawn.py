import Zero
import Events
import Property
import VectorMath

# code to make the switches increase in size

class BlockSpawn:
    def DefineProperties(self):
        self.ActiveState = Property.Int(0)
        self.Orientation = Property.Int(0)
        self.ExtendLength = Property.Int(5)
        
    def Initialize(self, initializer):
        Zero.Connect(self.Owner, Events.CollisionStarted, self.OnCollisionStarted)
        self.ActiveState = 0;
        
    def OnCollisionStarted(self, CollisionEvent):
        otherObject = CollisionEvent.OtherObject
        if(self.Orientation == 0):
            if(otherObject.Name == "Bullet"):
                
                otherObject.Destroy()
                PlayerPTR = self.Space.FindObjectByName("Player")
                PlayerPTR.HackStats.Hack1_Active = 0
                PlayerPTR.HackStats.Hack2_Active = 0
                
                if(self.Owner.Switch_Stats.active == 0):
                    self.Owner.Transform.Scale = VectorMath.Vec3(self.ExtendLength,1,1)
                    self.Owner.Switch_Stats.active = 1
                
                elif(self.Owner.Switch_Stats.active == 1):
                    self.Owner.Transform.Scale = VectorMath.Vec3(1,1,1)
                    self.Owner.Switch_Stats.active = 0
                    
        elif(self.Orientation == 1):
            if(otherObject.Name == "Bullet"):
                otherObject.Destroy()
                PlayerPTR = self.Space.FindObjectByName("Player")
                PlayerPTR.HackStats.Hack1_Active = 0
                PlayerPTR.HackStats.Hack2_Active = 0
                
                if(self.Owner.Switch_Stats.active == 0):
                    self.Owner.Transform.Scale = VectorMath.Vec3(1,self.ExtendLength,1)
                    self.Owner.Switch_Stats.active = 1
                
                elif(self.Owner.Switch_Stats.active == 1):
                    self.Owner.Transform.Scale = VectorMath.Vec3(1,1,1)
                    self.Owner.Switch_Stats.active = 0

Zero.RegisterComponent("BlockSpawn", BlockSpawn)