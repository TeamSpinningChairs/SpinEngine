#include "Precompiled.h"

class IComponent;

class ComponentCreator
{
public:
	ComponentCreator(std::string componentName)
		:m_ComponentCreatorName(componentName)
	{
	}

	std::string m_ComponentCreatorName;
	///Create the component

	virtual IComponent * Create(IEntity* owner) = 0;
};

///Templated helper class used to make concrete component creators.
template<typename T>
class ComponentCreatorType : public ComponentCreator
{
public:
	ComponentCreatorType(std::string componentName)
		:ComponentCreator(componentName)
	{
	}

	virtual IComponent* Create(IEntity* owner)
	{
		return new T(owner);
	}
};



//Register component macro
#define RegisterComponent(type) AddComponentCreator( #type, new ComponentCreatorType<type>(#type) );