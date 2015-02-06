'''
GamePadScript
Python
Ian Aemmer 

This stock Script is designed to show the default controller properties for the Xbox360 
wired and wireless controller inputs.

GamePadScript has only one property, IndexOfController. This is the number attached to 
a specific controller, so that an object with this script can access that controller. 
'''

import Zero
import Events
import Property
import VectorMath

'''

Gamepad Index:
    0 = A button
'''

class GamePadScript:
    def DefineProperties(self):
        #This is a property that can be changed in level. index 0 for player 1, index 1 for player 2, etc. 
        self.IndexOfController = Property.Int(0)
        pass
        
    def Initialize(self, initializer):
        #Zero.Connect(self.Space, Events.LogicUpdate, self.OnLogicUpdate)
        self.GameController = Zero.Gamepads.GetGamePad(self.IndexOfController)
        pass
        
    def OnLogicUpdate(self, UpdateEvent):
        
        print(self.GameController.IsActive)
        if(self.GameController.IsButtonHeld(Zero.Buttons.A)):
            print("you held the 'a' button!")
        if(self.GameController.IsButtonHeld(Zero.Buttons.B)):
            print("you held the 'b' button!")
        if(self.GameController.IsButtonHeld(Zero.Buttons.X)):
            print("you held the 'x' button!")
        if(self.GameController.IsButtonHeld(Zero.Buttons.Y)):
            print("you held the 'y' button!")
        if(self.GameController.IsButtonHeld(Zero.Buttons.DpadUp)):
            print("you held the 'Dpad Up' button!")
        if(self.GameController.IsButtonHeld(Zero.Buttons.DpadDown)):
            print("you held the 'Dpad Down' button!")
        if(self.GameController.IsButtonHeld(Zero.Buttons.DpadLeft)):
            print("you held the 'Dpad Left' button!")
        if(self.GameController.IsButtonHeld(Zero.Buttons.DpadRight)):
            print("you held the 'Dpad Right' button!")
        if(self.GameController.IsButtonHeld(Zero.Buttons.Start)):
            print("you held the Start button!")
            
        if(self.GameController.IsButtonHeld(Zero.Buttons.AnyButton)):
            #print("you held any of the buttons!")
            self.GameController.Vibrate(0.1, 0.3, 0.3)
            #void Vibrate(float time, float Powerofleftvibrate, float Powerofrightvibrate)
            
        if(self.GameController.IsButtonHeld(Zero.Buttons.Back)):
            print("you held the back button!")
        if(self.GameController.IsButtonHeld(Zero.Buttons.LeftShoulder)):
            print("you held the 'left shoulder' button!")
        if(self.GameController.IsButtonHeld(Zero.Buttons.RightShoulder)):
            print("you held the 'right shoulder' button!")
        if(self.GameController.IsButtonHeld(Zero.Buttons.LeftThumb)):
            print("you held the left stick!")
        if(self.GameController.IsButtonHeld(Zero.Buttons.RightThumb)):
            print("you held the right stick!")
        if(self.GameController.IsButtonHeld(Zero.Buttons.StickDown)):
            print("you held down!")
        if(self.GameController.IsButtonHeld(Zero.Buttons.StickUp)):
            print("you held Up!")
        if(self.GameController.IsButtonHeld(Zero.Buttons.StickLeft)):
            print("you held Left!")
        if(self.GameController.IsButtonHeld(Zero.Buttons.StickRight)):
            print("you held right!")
        

Zero.RegisterComponent("GamePadScript", GamePadScript)