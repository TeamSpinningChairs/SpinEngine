/******************************************************************************/
/*!
\file   ScriptComponent.h
\author Sagnik Chowdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Scriptcomponents bind components from Zilch to the engine.

As a condition of your accessing this area, you agree to be bound by the
following terms and conditions :

The games software was created by students of DigiPen Institute of
Technology(DigiPen), and all copyright and other rights in such is owned by
DigiPen.While DigiPen allows you to access, download and use the software
for non - commercial, home use you hereby expressly agree that you will not
otherwise copy, distribute, modify, or(to the extent not otherwise
permitted by law) decompile, disassemble or reverse engineer the games
software.

THE GAMES SOFTWARE IS MADE AVAILABLE BY DIGIPEN AS - IS AND WITHOUT
WARRANTY OF ANY KIND BY DIGIPEN.DIGIPEN HEREBY EXPRESSLY DISCLAIMS ANY SUCH
WARRANTY, INCLUDING, WITHOUT LIMITATION, ANY IMPLIED WARRANTY OF
MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE. WITHOUT LIMITING THE
GENERALITY OF THE FOREGOING, DIGIPEN SHALL NOT BE LIABLE IN DAMAGES OR
OTHERWISE FOR ANY DAMAGE OR INJURY FROM DOWNLOADING, DECOMPRESSING, RUNNING
OR ATTEMPTING TO RUN, USING OR OTHERWISE DEALING WITH, IN ANY WAY, THE
GAMES SOFTWARE CONTAINED IN THIS AREA, NOR SHALL DIGIPEN BE LIABLE FOR ANY
INCIDENTAL, CONSEQUENTIAL, EXEMPLARY OR OTHER TYPES OF DAMAGES ARISING FROM
ACCESS TO OR USE OF THE GAMES SOFTWARE.

YOU HEREBY AGREE TO INDEMNIFY, DEFEND AND HOLD HARMLESS DIGIPEN AND ITS
DIRECTORS, OFFICERS, EMPLOYEES, AGENTS, CONSULTANTS AND CONTRACTORS AGAINST
ALL LIABILITY OF ANY KIND ARISING OUT OF YOUR DOWNLOADING, DECOMPRESSING,
RUNNING OR ATTEMPTING TO RUN, USING OR OTHERWISE DEALING WITH, IN ANY WAY,
THE GAMES SOFTWARE. DIGIPEN MAKES NO WARRANTIES OR REPRESENTATIONS THAT THE
GAMES SOFTWARE IS FREE OF MALICIOUS PROGRAMMING, INCLUDING, WITHOUT
LIMITATION, VIRUSES, TROJAN HORSE PROGRAMS, WORMS, MACROS AND THE LIKE.AS
THE PARTY ACCESSING THE GAMES SOFTWARE IT IS YOUR RESPONSIBILITY TO GUARD
AGAINST AND DEAL WITH THE EFFECTS OF ANY SUCH MALICIOUS PROGRAMMING.
*/
/******************************************************************************/

//!makes sure this is only included once
#pragma once
#include "Precompiled.h"
#include "JSONSerializer.h"
#include "ZilchCompiledLib.h"



  //NOTES-------------------------------------------------------------
  //To later check if a GameComponent is a ZilchComponent:
  //GameComponent *c = ....
  //ZilchComponent *zc = dynamic_cast<ZilchComponent*>(c);
  //if(zc != 0) // is ZilchComponent

  //pass string str.c_str() to convert to const char*
  //------------------------------------------------------------------

  class ZilchComponent : public IComponent
  {
  public:
    ZilchDeclareDerivedType(ZilchComponent, IComponent);

    //Constructor. Initialize with owner and component name.
	ZilchComponent();

    //!Updated every frame.
    //void Update(const double &dt);
	Handle ThisHandle;
	void Serialize(DynamicElement* props, Serializer::DataNode* data);
	//virtual void Serialize(Serializer::DataNode* data) {};
	virtual void Create(IEntity* owner);
	virtual void Update(float dt);
    //!Initialize the system.
    virtual bool Initialize();

	virtual void Destroy(){};

	virtual void Release();
	virtual void DeleteThis();
    //Destructor
	virtual ~ZilchComponent() override;


  private:
    //Private data ----------------------------------
    //Zilch class name
    Zilch::String* ZilchClassName;

  }; //class scriptcomponent
 //namespace framework