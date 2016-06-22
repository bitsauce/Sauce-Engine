#ifndef SAUCE_RESOURCE_MANAGER_H
#define SAUCE_RESOURCE_MANAGER_H

#include <Sauce/Config.h>
#include <Sauce/Common/Game.h>

/*********************************************************************
**	Resource manager												**
**********************************************************************/

BEGIN_SAUCE_NAMESPACE

template<typename T>
using Resource = shared_ptr<T>;

template<typename T>
using ResourceGuard = weak_ptr<T>;

enum ResourceType
{
	RESOURCE_TYPE_TEXTURE,
	RESOURCE_TYPE_SHADER,
	RESOURCE_TYPE_FONT
};

class SAUCE_API ResourceDesc
{
public:
	ResourceDesc(ResourceType type, const string &name) :
		m_type(type),
		m_name(name)
	{
	}

	string getName() const
	{
		return m_name;
	}

	ResourceType getType() const
	{
		return m_type;
	}

private:
	const ResourceType m_type;
	const string m_name;
};

/**
 * \brief This class handles resource loading and handling.
 *
 * Resources in this context are objects which are loaded from the hard drive
 * and which should only be read once.
 *
 * Resources are loaded as follows \code Resouce<ResourceClass> resource = game->getResourceManager()->get<ResourceClass>("ResourceName"); \endcode
 * Where ResourceClass is the class name of the type of resource you are trying to load.
 * ResouceManager can currently load image files, font files and shaders.
 */

// TODO: Separate resources from instances of resources.
// 
// For example changing the color of a Resource<Font> loaded with ResourceManager
// will change the color of all instances loaded with ResourceManager.
//  
// To fix this, separate the Font class into FontInstance and FontResource,
// where FontInstance holds all instance related data, and FontResurce the
// shader resource data.
class SAUCE_API ResourceManager
{
	friend class Game;
public:
	template<typename T>
	Resource<T> get(string name)
	{
		// Check if resource is already loaded
		{
			map<string, void*>::iterator itr;
			if((itr = m_resources.find(name)) != m_resources.end())
			{
				ResourceGuard<T> *guard = static_cast<ResourceGuard<T>*>(itr->second);
				if(!guard->expired())
				{
					return guard->lock();
				}
				else
				{
					delete guard;
				}
			}
		}

		// The resource is not loaded, check if we have a resource descriptor
		{
			map<string, ResourceDesc*>::iterator itr;
			if((itr = m_resourceDesc.find(name)) != m_resourceDesc.end())
			{
				Resource<T> resource(new T(itr->second));
				if(resource)
				{
					m_resources[name] = new ResourceGuard<T>(resource);
					return resource;
				}
			}
		}

		// Resource was not found
		// TODO: When a resource is not found, return a dummy resource instead of a nullptr.
		LOG("Resource '%s' was not found", name.c_str());
		return 0;
	}

private:
	ResourceManager(const string &resourceFile);

	map<string, void*> m_resources;
	map<string, ResourceDesc*> m_resourceDesc;
};

END_SAUCE_NAMESPACE

#endif // SAUCE_RESOURCE_MANAGER_H