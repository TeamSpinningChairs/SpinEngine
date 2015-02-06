import Zero
import Events
import Property
import VectorMath
import math
import Keys

Vec3 = VectorMath.Vec3

class PlayerMovement:

    moveSpeed = Property.Float(10.0)
    jumpSpeed = Property.Float(1)
    MaxJumps = Property.Int(2)
    
    def Initialize(self, initializer):
        Zero.Connect(self.Space, Events.LogicUpdate, self.OnLogicUpdate)
        self.OnGround = False
        self.jumpsRemaining = 1
        Zero.Gamepads.GetGamePad(1)
        
    def OnLogicUpdate(self, UpdateEvent):
        self.UpdatePlayerImput()
        self.ApplyMovement(UpdateEvent)
        self.UpdateGroundState()
        self.ApplyJumping()
        self.UpdateJumpState()
        
        # input controls
    def UpdatePlayerImput(self):
        
        '''
        self.moveleft = Zero.Buttons.DpadLeft
        self.moveright = Zero.Buttons.DpadRight
        self.Jump = Zero.Buttons.A
        self.ApplyJumping
        '''
        
        self.moveleft = Zero.Keyboard.KeyIsDown(Zero.Keys.Left)
        self.moveright = Zero.Keyboard.KeyIsDown(Zero.Keys.Right)
        self.Jump = Zero.Keyboard.KeyIsPressed(Zero.Keys.Up)
        self.ApplyJumping
        
    def ApplyMovement(self, UpdateEvent):
        moveDirection = Vec3(0,0,0)
        
        if(self.moveright):
            moveDirection += Vec3(1,0,0)
        if(self.moveleft):
            moveDirection += Vec3(-1,0,0)
            
        moveDirection.normalize()
        self.Owner.RigidBody.ApplyForce(moveDirection * self.moveSpeed)
    
    
    def ApplyJumping(self):
        if(self.canJump() and self.Jump):
            self.SubtractJumps()
            self.Owner.RigidBody.ApplyLinearImpulse(Vec3(0,7.5,0) * self.jumpSpeed)
    
        
    def canJump(self):
        canJump = (self.OnGround) or (self.jumpsRemaining > 1)
        return canJump
        
    def UpdateGroundState(self):
        self.OnGround = False
        for contactHolder in self.Owner.Collider.Contacts:
            if(contactHolder.IsGhost):
                continue
                
            self.OnGround = True
            return
        
    def SubtractJumps(self):
        self.jumpsRemaining -= 1
        if(self.jumpsRemaining < 0):
            self.jumpsRemaining = 0
    
    def UpdateJumpState(self):
        if(self.OnGround):
            self.jumpsRemaining = self.MaxJumps
        
        
Zero.RegisterComponent("PlayerMovement", PlayerMovement)