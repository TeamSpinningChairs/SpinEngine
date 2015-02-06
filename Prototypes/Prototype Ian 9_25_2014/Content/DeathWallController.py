import Zero
import Events
import Property
import VectorMath

Vec3 = VectorMath.Vec3

class DeathWallController:
    def DefineProperties(self):
        self.moveSpeed = Property.Float(0.02)
        #self.Lives = Property.Int(9)
        pass

    def Initialize(self, initializer):
        #Zero.Connect(self.Space, Events.LogicUpdate, self.OnLogicUpdate)
        Zero.Connect(self.Owner, Events.CollisionStarted, self.OnCollisionStarted)
        Zero.Connect(self.Space, Events.LogicUpdate, self.OnLogicUpdate)

    def OnCollisionStarted(self, CollisionEvent):
        otherObject = CollisionEvent.OtherObject
        if(otherObject.Name == "Player"):
            print("Game should quit")
            #Zero.quit()
            self.GameSession.Quit()
            #Zero.Game.Quit()
    def OnLogicUpdate(self, UpdateEvent):
        #print("should be moving right")
        self.Owner.Transform.Translation += Vec3(self.moveSpeed,0,0)
        '''
        moveDirection = Vec3(0,0,0)
        moveDirection += Vec3(1,0,0)
        moveDirection.normalize()
        self.Owner.RigidBody.ApplyForce(moveDirection * self.moveSpeed)
        '''
Zero.RegisterComponent("DeathWallController", DeathWallController)