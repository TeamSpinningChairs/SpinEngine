import Zero
import Events
import Property
import VectorMath

class MouseInput:
    def DefineProperties(self):
        pass
        
    def Initialize(self, initializer):
        gameCameraObject = self.Space.FindObjectByName("Camera")
        self.Player = self.Space.FindObjectByName("Player")
        
        Zero.Connect(self.Space, Events.LogicUpdate, self.OnLogicUpdate)
        Zero.Connect(gameCameraObject,  Events.LeftMouseDown, self.LeftMouseDown)
        Zero.Connect(gameCameraObject,  Events.RightMouseDown, self.RightMouseDown)

    def OnLogicUpdate(self, UpdateEvent):
        pass

    def LeftMouseDown(self, ViewportMouseEvent):
        
        if(self.Player.HackStats.Hack1_Active == 0):
            
            MousePosition = ViewportMouseEvent.ToWorldZPlane(0.0)
            
            ProjectilePosition = self.Owner.Transform.Translation
            
            Projectile = self.Space.CreateAtPosition("Bullet1", ProjectilePosition)
            
            Direction = MousePosition - ProjectilePosition
            
            Direction.normalize()
                    
            Projectile.RigidBody.ApplyLinearVelocity(10 * Direction)
            
            self.Player.HackStats.Hack1_Active = 1
            
            
    def RightMouseDown(self, ViewportMouseEvent):
        
        if(self.Player.HackStats.Hack2_Active == 0):
        
            MousePosition = ViewportMouseEvent.ToWorldZPlane(0.0)
            
            ProjectilePosition = self.Owner.Transform.Translation
            
            Projectile = self.Space.CreateAtPosition("Bullet2", ProjectilePosition)
            
            Direction = MousePosition - ProjectilePosition
            
            Direction.normalize()
                    
            Projectile.RigidBody.ApplyLinearVelocity(10 * Direction)
            
            self.Player.HackStats.Hack2_Active = 1

Zero.RegisterComponent("MouseInput", MouseInput)