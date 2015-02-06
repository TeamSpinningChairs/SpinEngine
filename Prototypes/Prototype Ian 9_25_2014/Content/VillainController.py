import Zero
import Events
import Property
import VectorMath
Vec3 = VectorMath.Vec3
class VillainController:
    def DefineProperties(self):
        #self.Lives = Property.Int(9)
        self.moveSpeed = Property.Float(2.0)
        self.DefaultMoveTime = Property.Int(200)
        
    def Initialize(self, initializer):
        Zero.Connect(self.Space, Events.LogicUpdate, self.OnLogicUpdate)
        Zero.Connect(self.Owner, Events.CollisionStarted, self.OnCollisionStarted)
        Zero.Connect(self.Owner, Events.CollisionEnded, self.OnCollisionEnded)
        self.TheWall = self.Space.FindObjectByName("Death Wall")
        self.OutOfTime = False
        self.CurrentlyAttached = False
        self.Timer = self.DefaultMoveTime
        self.ConnectedToWall = True
        pass

    def OnLogicUpdate(self, UpdateEvent):
        self.UpdatePlayerImput()
        self.ApplyMovement(UpdateEvent)
        if(self.ConnectedToWall == False):
            self.Timer = self.Timer - 1
            if(self.Timer == 0):
                self.OutOfTime = True
                self.Owner.SphereCollider.Ghost = True
        print(self.Timer)
        if(self.TheWall.Transform.Translation.x > self.Owner.Transform.Translation.x):
            self.Owner.Transform.Translation += Vec3(1,0,0)
        print(self.Owner.SphereCollider.Ghost)
        
    def OnCollisionStarted(self, CollisionEvent):
        otherObject = CollisionEvent.OtherObject
        if(otherObject.Name == "Player"):
            if(self.CurrentlyAttached == False):
                otherObject.RigidBody.DynamicState = 2;
                otherObject.AttachToRelative(self.Owner)
                self.CurrentlyAttached = True
                self.OutOfTime = True
                self.Owner.Transform.Translation += Vec3(0,0,1)
                self.Owner.SphereCollider.Ghost = True
        if(otherObject.Name == "Death Wall"):
            self.OutOfTime = False
            self.Timer = self.DefaultMoveTime
            self.ConnectedToWall = True
            self.Owner.RigidBody.Velocity = Vec3(0,0,0)
            self.Owner.SphereCollider.Ghost = False
            
    def OnCollisionEnded(self, CollisionEvent):
        otherObject = CollisionEvent.OtherObject
        if(otherObject.Name == "Death Wall"):
            self.ConnectedToWall = False
            
    def UpdatePlayerImput(self):
        
        self.moveleft = Zero.Keyboard.KeyIsDown(Zero.Keys.A)
        self.moveright = Zero.Keyboard.KeyIsDown(Zero.Keys.D)
        self.moveup = Zero.Keyboard.KeyIsDown(Zero.Keys.W)
        self.movedown = Zero.Keyboard.KeyIsDown(Zero.Keys.S)
        
    def ApplyMovement(self, UpdateEvent):
        self.moveDirection = Vec3(0,0,0)
        
        if(self.OutOfTime == True):
            self.moveDirection += Vec3(-5,0,0) 
            #print("wat")           
            #print(self.moveDirection)
        else:
                
            if(self.moveright):
                self.moveDirection += Vec3(1,0,0)
            if(self.moveleft):
                self.moveDirection += Vec3(-1,0,0)
            if(self.moveup):
                self.moveDirection += Vec3(0,1,0)
            if(self.movedown):
                self.moveDirection += Vec3(0,-1,0)
                
            self.moveDirection.normalize()
        #self.VectorCheck = vecself.Owner.Transform.Translation
        #print(self.Owner.Transform.Translation.Vec3.
        #if(self.Owner.Transform.Translation.Vec3.x ):
            #Vec3.x
        if(self.TheWall.Transform.Translation.x < self.Owner.Transform.Translation.x):
            #print("behind wall!")
            #VectorMath.Vec3.x
            self.Owner.RigidBody.ApplyForce(self.moveDirection * self.moveSpeed)
    

Zero.RegisterComponent("VillainController", VillainController)