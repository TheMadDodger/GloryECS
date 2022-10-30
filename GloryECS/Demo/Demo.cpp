#include "pch.h"

int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    GloryReflect::Reflect::CreateReflectInstance();
    
    {
        GloryReflect::Reflect::RegisterType<ReflectableComponent>();

        const GloryReflect::TypeData* pTypeData = ReflectableComponent::GetTypeData();

        ReflectableComponent test(10.0f, 69, "Hello World!");

        int a = 0;
        const GloryReflect::FieldData* pFieldData = pTypeData->GetFieldData(1);
        pFieldData->Get(&test, &a);

        a = 420;
        pFieldData->Set(&test, &a);

        GloryECS::EntityRegistry reg;
        GloryECS::EntityID entity = reg.CreateEntity<Position>();
        Position& position = reg.GetComponent<Position>(entity);
        MyComponent& component = reg.AddComponent<MyComponent>(entity, 5);
    
        for (size_t x = 0; x < 100; x++)
        {
            for (size_t y = 0; y < 100; y++)
            {
                entity = reg.CreateEntity<Position>(x * 50.0f, y * 50.0f);
            }
        }

        const GloryReflect::FieldData* pArrayFieldData = pTypeData->GetFieldData(3);
        void* pArrayAddress = pArrayFieldData->GetAddress(&test);
        size_t size = GloryReflect::Reflect::ArraySize(pArrayAddress, pArrayFieldData->ArrayElementType());
        GloryReflect::Reflect::ResizeArray(pArrayAddress, pArrayFieldData->ArrayElementType(), 5);
        size = GloryReflect::Reflect::ArraySize(pArrayAddress, pArrayFieldData->ArrayElementType());
    }

    GloryReflect::Reflect::DestroyReflectInstance();

    _CrtDumpMemoryLeaks();

    return 0;
}
