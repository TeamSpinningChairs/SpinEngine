import Zero
import Events
import Property
import VectorMath

class Hack_Return:
    def DefineProperties(self):
        #self.Lives = Property.Int(9)
        pass

    def Initialize(self, initializer):
        Zero.Connect(self.Space, Events.LogicUpdate, self.OnLogicUpdate)
        self.Player = self.Space.FindObjectByName("Player")

    def OnLogicUpdate(self, UpdateEvent):
        
        if(self.Player.HackStats.Hack1_Return == 1):
        
            ProjectilePosition = self.Owner.Transform.Translation
            
            Direction = self.Player.Transform.Translation - ProjectilePosition
            
            Direction.normalize()
            
            self.Owner.RigidBody.Velocity = VectorMath.Vec3(0,0,0)
            
            self.Owner.RigidBody.ApplyLinearVelocity(6 * Direction)
            
            

Zero.RegisterComponent("Hack_Return", Hack_Return)