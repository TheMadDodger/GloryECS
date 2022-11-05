# GloryECS
## _Custom ECS and reflection library for [Glory](https://github.com/TheMadDodger/Glory/tree/master)_

## GloryECS
GloryECS is a basic custom Entity Component System, tailored for the Glory Game Engine, but can be used in any project.

### Features

- Create Entities
- Add any type as a component to an entity
- Components of the same type are next to each other in memory (just like an ECS is supposed to do)
- Iterate over all components on an entity
- Iterate over all components of a type
- Component reordering
- Link components to a UUID

### Usage
#### EntityRegistry
The `EntityRegistry` is the place to go for anything Entity related. This is where you add/remove/get components and entities.
In order to use the ECS you must make an instance of this registry, either as a pointer or as a value;

```cpp
EntityRegistry m_Registry;
EntityRegistry* m_pRegistry;
```

This can for example be a member value of your own scene class.

To create an entity simply call the `CreateEntity` function on the registry, you can also use `CreateEntity<T>(Args&&... args)` to create an entity with a default component.

```cpp
// Creates a new entity and returns its EntityID
GloryECS::EntityID entity = m_Registry.CreateEntity();
// Creates a new entity with a Position component and returns its EntityID
GloryECS::EntityID entity = m_Registry.CreateEntity<Position>();
```

### Components
You can define components as a class or as a struct.
```cpp
struct Position
{
    Position() : _x(0.0f), _y(0.0f) {}
    Position(float x, float y) : _x(x), _y(y) {}

    float _x;
    float _y;
};

class Position
{
    Position() : _x(0.0f), _y(0.0f) {}
    Position(float x, float y) : _x(x), _y(y) {}

    float _x;
    float _y;
};
```
**Make sure your components have a default constructor! Your compiler will complain if you don't!**

You can then add these components to any entity by using the `AddComponent<T>(Args&&... args)` function.
```cpp
// Creates a new entity
GloryECS::EntityID entity = m_Registry.CreateEntity();
// Creates a Position component and calls its default constructor, then adds it to the entity
Position& position = m_Registry.AddComponent<Position>();
// Creates a Position component and calls its custom constructor, then adds it to the entity
Position& position = m_Registry.AddComponent<Position>(6.0f, 9.0f);
```

It should be noted that this ECS allows muultiple instances of the same component on one entity, however, there is no functionality to retreive the duplicates with the `Get` function, it will always return the first one.

Other methods of the `EntityRegistry` class:
```cpp
// Creates a new entity
EntityID CreateEntity();
// Clears all components on the entity and removes it
void DestroyEntity(EntityID entity);
// Registers a component by already creating its `TypeView` with no components
template<typename Component>
void RegisterComponent();
// Creates an entity with a component, the Args are passed to your custom constructor
template<typename Component, typename... Args>
EntityID CreateEntity(Args&&... args);
// Creates an entity with a component, assigns the UUID to the component rather than generating a new one
// Usefull for serialization systems
template<typename Component, typename... Args>
EntityID CreateEntity(Glory::UUID uuid, Args&&... args);
// Adds a component to an entity
template<typename Component, typename... Args>
Component& AddComponent(EntityID entity, Args&&... args);
// Adds a component to an entity and assigns its UUID rather than generating a new one
template<typename Component, typename... Args>
Component& AddComponent(EntityID entity, Glory::UUID uuid, Args&&... args);
// Creates a component from a type hash (std::hash<std::type_index>) and assigns a UUID to it, returns the address to the component
void* CreateComponent(EntityID entityID, size_t typeHash, Glory::UUID uuid);
// Gets the templated TypeView of a component
template<typename Component>
TypeView<Component>* GetTypeView();
// Gets the BaseTypeView of a component
BaseTypeView* GetTypeView(size_t typeHash);
// Gets the EntityView of an entity
EntityView* GetEntityView(EntityID entity);
// Gets the address of a component by UUID on an entity
void* GetComponentAddress(EntityID entityID, Glory::UUID componentID);
// Returns true if the entity has the specified component
template<typename Component>
bool HasComponent(EntityID entity);
// Gets a component by reference on an entity
// Throws an exception if the component does not exist on the entity
template<typename Component>
Component& GetComponent(EntityID entity)l
// Removes a component from an entity
// Throws an exception if the component does not exist on the entity
template<typename Component>
void RemoveComponent(EntityID entity);
// Removes a component by type hash from an entity
void RemoveComponent(EntityID entity, size_t typeHash);
// Removes the nth component on a component
void RemoveComponentAt(EntityID entity, size_t index);
// Returns the number of components on an entity
size_t ComponentCount(EntityID entity);
// Removes all components on an entity
void Clear(EntityID entity);
// Counts the number of active entities
const size_t Alive() const;
// Returns true if the entity is active
const bool IsValid(EntityID entity) const;
// Returns the number of active TypeViews
const size_t TypeViewCount() const;
// Gets the begin iterator of the TypeView map
std::map<size_t, BaseTypeView*>::iterator GetTypeViewIterator();
// Gets the end iterator of the TypeView map
std::map<size_t, BaseTypeView*>::iterator GetTypeViewIteratorEnd();
```

### TypeView
`TypeView` allows you to iterate over all instances of a component, as well as get a component on a specific entity.
```cpp
GloryECS::EntityID entity = m_Registry.CreateEntity<Position>();
TypeView<Position>* pPositionTypeView = m_Registry.GetTypeView<Position>();
Position& position = pPositionTypeView->Get(entity);

GloryECS::EntityID entity2 = m_Registry.CreateEntity();
Position& position2 = pPositionTypeView->Add(entity);
```

You can also get components directly through the registry.

```cppp
GloryECS::EntityID entity = m_Registry.CreateEntity<Position>();
Position& position = m_Registry.GetComponent<Position>();
```

It is not recommended to use the templated Add method on the `TypeView` class as this will skip `EntityView` creation!

The base class of the type view also allows you to create a new instance of the component and add it to an entity, but this will use the default constructor and will also skip `EntityView` creation! If you want to do this use `EntityRegistry::CreateComponent()` instead!
This returns the memoty adddress to the component as a void*.

```cpp
GloryECS::EntityID entity = m_Registry.CreateEntity();
BaseTypeView* pTypeView = m_Registry.GetTypeView<Position>();
void* pComponentAddress = pTypeView->Create(entity);
```

TypeView has other features as follows:
```cpp
// Removes the component of this type view from an entity if that entity has the component
void Remove(EntityID entityID);
// Returns true if the entity has the component from the type view
bool Contains(EntityID entityID);
// Returns the type hash of the component (std::hash<std::type_index>)
const size_t ComponentTypeHash() const;
// Returns the type index of the component
virtual const std::type_index ComponentType() const = 0;
// Creates a component by calling its default constructor and adds it to the entity, returns memoty address to new component
virtual void* Create(EntityID entityID) = 0;
// Gets the memory address of the component on an entity, nullptr if the entity does not have the component
virtual void* GetComponentAddress(EntityID entityID, size_t number = 0) = 0;

// Templated function to add an entity, not recommended to use since it skips EntityView creation, use EntityRegistry::AddComponent instead
template<typename... Args>
T& Add(EntityID entityID, Args&&... args);
```

### EntityView

An `EntityView` allows you to iterate over an entities components, as well as get components by UUID or get the UUID of a component.
```cpp
GloryECS::EntityID entity = m_Registry.CreateEntity<Position>();
EntityView* pEntityView = m_Registry.GetEntityView(entity);
```

`EntityView` has the following methods:
```cpp
// Gets the number of components on this entity
size_t ComponentCount();
// Gets the type of component at the specified index
size_t ComponentTypeAt(size_t index);
// Gets the begin iterator of the component map
std::map<Glory::UUID, size_t>::iterator GetIterator();
// Gets the end iterator of the component map
std::map<Glory::UUID, size_t>::iterator GetIteratorEnd();
```

### Registering Components
Important to know is that dynamic component creation will fail if a `TypeView` for that component does not yet exist, this is why you should register all your components at initialization of your application/scene. To do this you can use `EntityRegistry::RegisterComponent<T>()`.

```cpp
void OnInitialize()
{
    m_Registry.RegisterComponent<Position>();
}
```

### TODO
GloryECS is currently missing the systems part of an ECS, which allows you to interact with your components. This will be added soon!
The library allows multiple of the same components on the same entity, but there is no way to limit this nor is there a way to get a duplicate component.

## GloryReflect
GloryReflect is a custom macro based reflection library, tailored for the Glory Game Engine, but can be used in any project.

### Features

- Generate type and field data in your classes/structs
- Class generation for easy enum conversion
- Register custom templated types
- Array support for std::vector
- Aqcuire typedata from type name or type hash
- Iterate over fields in a type
- Get and Set values through type data
- Create instances from type data

### Generating TypeData and FieldData
To generate `TypeData` and `FieldData` for your custom types, the REFLECTABLE macro can be used. It generates both your fields, and the nescesary data for those fields.
The data is generated as a static getter function, but all data in this function is statis, so it is only ever created once. Which should minimize overhead.
The first parameter in the macro is always the type of your class/struct itself. Using the parameters that follow, you may add up to 20 fields.
A field must be formatted in the following form: (type)(name)

Example:
```cpp
struct Transform
{
	Transform();
	Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);

	REFLECTABLE(Transform,
		(glm::vec3) (Position),
		(glm::quat) (Rotation),
		(glm::vec3) (Scale)
	)
};
```

Any basic type is supported, custom types like structs and classes are also supported as value, pointers are not supported!

You can also have a field to another reflectable struct/class.
Example:
```cpp
struct Struct1
{
	REFLECTABLE(Struct1,
		(float) (Float)
	)
};

struct Struct2
{
	REFLECTABLE(Struct2,
		(Struct1) (RecursionValue)
	)
};
```

### Generating Enum and EnumType
The library comes with a class to easily convert enums to strings and vice verse.
In order to generate the nescesary data for this, you must create your enums using the REFLECTABLE_ENUM macro.
The first parameter of the macro is the enum name, followed by every possible value.

Example:
```cpp
REFLECTABLE_ENUM(MyEnum, One, Two, Three, Four, Five)
```

After using this macro, you can use `Enum<T>` templated class to easily convert from and to a string.

Example:
```cpp
MyEnum e = MyEnum::Three;
std::string stringValue;
Enum<MyEnum>().ToString(e, stringValue); // Will set stringValue = "Three"
stringValue = "One";
Enum<MyEnum>().FromString(stringValue, e); // Will set e = MyEnum::One
```

Both methods can return false when conversion failed, or true when succeeded.

### Registering Custom Types
Now that you have generated your reflection data, you have to register your custom types in the `Reflect` class.
This class is a signleton for the sake of supporting DLLs, so make sure to call the following methods:
```cpp
// Creates the reflection instance and sets the current process as the owner
GloryReflect::Reflect* pReflectInstance = GloryReflect::Reflect::CreateReflectInstance();
// Sets the reflection instance for the current process, owner flag will remain untouched
GloryReflect::Reflect::SetReflectInstance(pReflectInstance);
// If the current process is the owner, destroys the instance, if not owner, it clears the instance value for the current process
GloryReflect::Reflect::DestroyReflectInstance();
```

After creating and setting your reflection instance accross your processes, you can now use the static `GloryReflect::Reflect::RegisterType<T>()` to register yout custom types.
Note that if these types where not made using the REFLECTABLE macro, then the compiler will fail to compile as the function `GetTypeData` is missing from the type.

IMPORTANT: Make sure to register your types in the correct order so that if a type uses another registered type it can find this other type.

Example:
```cpp

struct Vector2
{
	REFLECTABLE(Vector2,
		(float) (x),
		(float) (y)
	)
}

struct Transform
{
    REFLECTABLE(Transform,
		(Vector2) (Position),
		(Vector2) (Rotation),
		(Vector2) (Scale)
	)
}

// Transform has a Vector2 field so we must register Vector2 BEFORE registering Transform!
// If we don't do this no link between the 2 will be generated
GloryReflect::Reflect::RegisterType<Vector2>();
GloryReflect::Reflect::RegisterType<Transform>();
```

To register a reflectable Enum you must call `RegisterEnum<T>()`
`RegisterBasicType<T>()` can be used to register basic types like integer and float, but these are registered for you on instance creation.

### TypeData
The `TypeData` class contains the information of a type, this includes the following:
- Type hash (std::hash<std::type_index>)
- Type name
- Number of fields
- Array containing `FieldData` values

### FieldData
The `FieldData` class contains information about a field on a type, this includes the following:
- `FieldType` the hash of the type of the field
- `ElementType` the hash of the type of the element (used by arrays and custom templated types)
- The name of the field
- A string of the type
- The offset to the memroy address where this data can be found
- The size of the field

You can use a `FieldData` instance to Set or Get a value in an instance of the type, as long as you have the address to a valid instance.
Keep in mind that void pointers are very dangerous and not type sensitive, so your program will crash when trying to access illegal memory or simply return garbage!

```
// Get the value of the field on a specified memory address
void Get(void* pAddress, void* out) const;
// Sets the value of the field on a specified memory address
void Set(void* pAddress, void* value) const;
// Returns the address to the field by offsetting the specified address by the offset
void* GetAddress(void* pAddress) const;
```

### Custom Templated Types
It is possible to register custom templated types as a custom hash + the element hash of the template arg.
This only works on templated types with a single template argument, and the hash must be <=100!
`std::vector` is already registered for you, meaning you can use `std::vector` as fields in your reflectable types, std::vector itself has hash 99 by default.
An example of its usecase would be a reference to a type of asset in your engine, when all assets use the same ID system:

```cpp
template<typename T>
class AssetReference
{
public:
    T* GetAsset()
    {
        T* pAsset = /* get asset */;
        retrun pAsset;
    }

private:
    UUID m_UUID;
}


GloryReflect::Reflect::RegisterTemplatedType("AssetReference", 96/* custom type hash */, sizeof(UUID));
```

The above code would register AssetReference as type 96, any field created using this type after registration will have 98 as type hash and `std::hash<std::type_index>()(typeid(T))` as element type hash.

As you can see, the type has to be registered as a string, this is because the reflection system will search for this in the type name.

It should be noted that the order of registration is really important here, if you were to try to use `AssetReference<T>` inside of the REFLECTABLE macro and then registering this new reflectable type before registering `AssetReference` as a templated type, an exception will trigger as the system cannot find the template type.
The same is true if the template arg is not registered. This is because it needs these to find the type hash and element type hash!

These following hashes are already in use by the core library:
```cpp
enum class CustomTypeHash
{
	Struct = 97,
	Enum = 98,
	Array = 99,
};

```

### Type Factory
Instances of types can be created from a `TypeData`, however this will always call the default constructor, so make sure your types have default constrcutors!
The following static methods can be used to create instances of types, note that they take the type hash of the type you want to create.
```cpp
// Create an instance of a type as a value returned in an std::any
std::any Reflect::CreateAsValue(size_t hash);
// Create an instance of a type as a pointer returned as a void*
void* Reflect::CreateAsPointer(size_t hash);
```

**Note that if the specified type hash does not exist an exception is thrown!**

### Type and Field flags
The user can set custom type and field flags, these are stored as a 64 bit integer so a maximum of 64 custom flags can be set.
Type Flags can be set in the registration function, field flags have to be set afterwards.

```cpp
// Type flags can be set when registering the type
template<typename T>
static const TypeData* Reflect::RegisterType(uint64_t flags = 0);
// Set the flags of a custom field
void Reflect::SetFieldFlags(const FieldData* pFieldData, uint64_t flags = 0);
```

You can query these flags afterwards, an example of a usecase is to have a vec4 be seen as a color value by adding a Color flag to its field flags.

```cpp
// Get the flags of a type
static const uint64_t Reflect::GetTypeFlags(size_t hash);
// Get the flags of a custom field on a type
uint64_t Reflect::GetFieldFlags(const FieldData* pFieldData);
```

### Arrays
The only type of array supported is in the form of an std::vector. The template type registration was used for this.
There are built in classes to help manipulate these arrays, the following static methods are provided in the Reflect class:

```cpp
// Resize an array at a memory address
void Reflect::ResizeArray(void* pArrayAddress, size_t elementTypeHash, size_t newSize);
// Get the size of an array at the specified memory address
const size_t Reflect::ArraySize(void* pArrayAddress, size_t elementTypeHash);
// Get the address of an element in an array at the specified address
void* Reflect::ElementAddress(void* pArrayAddress, size_t elementTypeHash, size_t index);
```

**All these methods require to know the type hash of the arrays element, if the type specified was never registered these methods will simply do nothing.**

# Tech

GloryECS and GloryReflect both use no external libraries nor do they need each other to function, simply download, compile and use.
There is a demo included in the solution, this demo requires both libraries to run.
Both projects make use of the standard template library.

# Installation

1. Download or clone the repository
2. Run `generateprojects.bat` in the `scripts` folder
3. Open the solution and build
4. Run the `Demo` project for a quick demo
5. Use GloryECSStatic.lib and GloryReflectStatic.lib in your own projects

# License
MIT