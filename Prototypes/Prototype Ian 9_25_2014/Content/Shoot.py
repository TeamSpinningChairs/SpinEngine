import Zero
import Events
import Property
import VectorMath

Vec3 = VectorMath.Vec3

class Shoot:
    def DefineProperties(self):
        self.ShotActive = Property.Int(0)
    
    def Initialize(self, initializer):
        Zero.Connect(self.Space, Events.LogicUpdate, self.OnLogicUpdate)
        
    def OnLogicUpdate(self, UpdateEvent):
        if(Zero.Keyboard.KeyIsPressed(Zero.Keys.Q)):
            if(self.ShotActive == 0):
                Zero.Engine.CrashEngine()
                
                #self.Shoot()
            
    def Shoot(self):
        Shot = self.Space.Create("Bullet")
        Shot.Transform.Translation = self.Owner.Transform.Translation + Vec3(1.25,0,0)
        Shot.RigidBody.ApplyLinearVelocity(Vec3(10,0,0))
        self.ShotActive = 1

Zero.RegisterComponent("Shoot", Shoot)