import Zero
import Events
import Property
import VectorMath

class SmoothCameraMovement:
    #target object is an object selected in the properties window for the camera to follow
    targetObject = Property.Cog()
    newTranslation = VectorMath.Vec3(0,0,0)
    #trigger distance is how far from something you have to be before the camera starts to move
    horizontalTriggerDistance = Property.Float(0.0)
    VerticalTriggerDistance = Property.Float(0.0)
    #This is the speed at which the camera will move 
    SmoothMoveSpeed = Property.Float(1.0)
    #makes camera movement smooth or not
    smoothMovement = Property.Bool(True)
    #governs whether the camera is allowed to move, even when trigger distance is met
    moveCamera = Property.Bool(True)
    #this is the Y distance between the target object and the camera (since the camera is tilted) allows tilted camera to track player in center of screen
    YTranslationAlter = Property.Float(0.0)
    
    def Initialize(self, initializer):
        Zero.Connect(self.Space, Events.LogicUpdate, self.OnLogicUpdate) 
        #add to this obejct's rotation a vec3 eaul to (target object's x - this object x , target y - this. y, target z - this.z)
        self.Owner.Transform.Translation += VectorMath.Vec3(self.targetObject.Transform.Translation.x - self.Owner.Transform.Translation.x, self.targetObject.Transform.Translation.y + self.YTranslationAlter - self.Owner.Transform.Translation.y, self.Owner.Transform.Translation.z)
        
        self.newTranslation = self.Owner.Transform.Translation
        #SOMEWHERE I NEED TO KEEP TRACK OF MY STARTING TRANSLATION
        self.StartingTranslation = self.Owner.Transform.Translation
        
    def OnLogicUpdate(self, UpdateEvent):
        currentTranslation = self.Owner.Transform.Translation
        targetTranslation = VectorMath.Vec3(self.targetObject.Transform.Translation.x, self.targetObject.Transform.Translation.y + self.YTranslationAlter,self.targetObject.Transform.Translation.z)
        #these distance variables are the difference between where the main object of focus is(where we want to go), 
        #and the current positon of the camera
        horizontalDistance = targetTranslation.x - currentTranslation.x
        VerticalDistance = targetTranslation.y - currentTranslation.y
        
        followTarget = False
        
        if(horizontalDistance > self.horizontalTriggerDistance):
            followTarget = True
        if(horizontalDistance < -self.horizontalTriggerDistance):
            followTarget = True
        if(VerticalDistance > self.VerticalTriggerDistance ):
            followTarget = True
        if(VerticalDistance < -self.VerticalTriggerDistance):
            followTarget = True
            
        if(self.moveCamera):
            if(followTarget):
                self.newTranslation = VectorMath.Vec3(targetTranslation.x, targetTranslation.y, currentTranslation.z)
            if(self.smoothMovement):
                self.Owner.Transform.Translation = currentTranslation.lerp(self.newTranslation, UpdateEvent.Dt*self.SmoothMoveSpeed)
            else:
                self.Owner.Transform.Translation = self.newTranslation
        pass
Zero.RegisterComponent("SmoothCameraMovement", SmoothCameraMovement)