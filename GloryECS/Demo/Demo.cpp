#include "pch.h"

void ReflectableComponent_OnAdd(GloryECS::EntityRegistry* pRegistry, GloryECS::EntityID entity, ReflectableComponent& component)
{
    std::cout << "ReflectableComponent added to entity " << entity << std::endl;
}

void Position_OnAdd(GloryECS::EntityRegistry* pRegistry, GloryECS::EntityID entity, Position& component)
{
    std::cout << "Position added to entity " << entity << std::endl;
}

void MyComponent_OnAdd(GloryECS::EntityRegistry* pRegistry, GloryECS::EntityID entity, MyComponent& component)
{
    std::cout << "MyComponent added to entity " << entity << std::endl;
}

int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    GloryReflect::Reflect::CreateReflectInstance();
    GloryECS::ComponentTypes::CreateInstance();
    
    {
        // Reflection demo
        GloryReflect::Reflect::RegisterEnum<TestEnum>();
        GloryReflect::Reflect::RegisterType<ReflectableComponent>();

        const GloryReflect::TypeData* pEnumTypeData = GloryReflect::Reflect::GetTyeData("enum TestEnum");

        TestEnum e;
        std::string out;
        GloryReflect::Enum<TestEnum>().FromString("TE_One", e);
        e = TestEnum::TE_Two;
        GloryReflect::Enum<TestEnum>().ToString(e, out);

        const GloryReflect::TypeData* pTypeData = ReflectableComponent::GetTypeData();

        ReflectableComponent test(10.0f, 69, "Hello World!");

        int a = 0;
        const GloryReflect::FieldData* pFieldData = pTypeData->GetFieldData(4);
        pFieldData->Get(&test, &a);

        a = 420;
        pFieldData->Set(&test, &a);

        const GloryReflect::FieldData* pArrayFieldData = pTypeData->GetFieldData(3);
        void* pArrayAddress = pArrayFieldData->GetAddress(&test);
        size_t size = GloryReflect::Reflect::ArraySize(pArrayAddress, pArrayFieldData->ArrayElementType());
        GloryReflect::Reflect::ResizeArray(pArrayAddress, pArrayFieldData->ArrayElementType(), 5);
        size = GloryReflect::Reflect::ArraySize(pArrayAddress, pArrayFieldData->ArrayElementType());

        void* pElementAddress = GloryReflect::Reflect::ElementAddress(pArrayAddress, pArrayFieldData->ArrayElementType(), 4);
        float* value = (float*)pElementAddress;
        *value = 5.0f;
        GloryReflect::Reflect::ResizeArray(pArrayAddress, pArrayFieldData->ArrayElementType(), 10);





        // ECS demo
        GloryECS::ComponentTypes::RegisterComponent<Position>();
        GloryECS::ComponentTypes::RegisterComponent<ReflectableComponent>();
        GloryECS::ComponentTypes::RegisterComponent<MyComponent>();

        GloryECS::EntityRegistry reg;
        reg.RegisterInvokaction<ReflectableComponent>(GloryECS::InvocationType::OnAdd, ReflectableComponent_OnAdd);
        reg.RegisterInvokaction<Position>(GloryECS::InvocationType::OnAdd, Position_OnAdd);
        reg.RegisterInvokaction<MyComponent>(GloryECS::InvocationType::OnAdd, MyComponent_OnAdd);
        GloryECS::EntityID entity = reg.CreateEntity();
        reg.CreateComponent(entity, GloryReflect::Reflect::Hash(typeid(ReflectableComponent)), Glory::UUID());

        entity = reg.CreateEntity<Position>();
        Position& position = reg.GetComponent<Position>(entity);
        MyComponent& component = reg.AddComponent<MyComponent>(entity, 5);
        ReflectableComponent& rComp = reg.AddComponent<ReflectableComponent>(entity);
        GloryECS::EntityView* pEntityView = reg.GetEntityView(entity);
        pEntityView->SetComponentIndex(0, 2);
        pEntityView->SwapComponentIndex(1, 2);
    
        for (size_t x = 0; x < 100; x++)
        {
            for (size_t y = 0; y < 100; y++)
            {
                entity = reg.CreateEntity<Position>(x * 50.0f, y * 50.0f);
            }
        }
    }

    GloryReflect::Reflect::DestroyReflectInstance();
    GloryECS::ComponentTypes::DestroyInstance();

    _CrtDumpMemoryLeaks();

    return 0;
}
