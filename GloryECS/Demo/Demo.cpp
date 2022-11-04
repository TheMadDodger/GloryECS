#include "pch.h"

int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    GloryReflect::Reflect::CreateReflectInstance();
    
    {
        GloryReflect::Reflect::RegisterType<ReflectableComponent>();

        GloryReflect::Reflect::RegisterEnum<TestEnum>();

        const GloryReflect::TypeData* pEnumTypeData = GloryReflect::Reflect::GetTyeData("enum TestEnum");


        GloryReflect::EnumType* pEnumType = GloryReflect::Reflect::GetEnumType(std::hash<std::type_index>()(typeid(TestEnum)));

        TestEnum e;
        std::string out;
        GloryReflect::Enum<TestEnum>().FromString("TE_One", e);
        e = TestEnum::TE_Two;
        GloryReflect::Enum<TestEnum>().ToString(e, out);

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

        void* pElementAddress = GloryReflect::Reflect::ElementAddress(pArrayAddress, pArrayFieldData->ArrayElementType(), 4);
        float* value = (float*)pElementAddress;
        *value = 5.0f;
        GloryReflect::Reflect::ResizeArray(pArrayAddress, pArrayFieldData->ArrayElementType(), 10);
    }

    GloryReflect::Reflect::DestroyReflectInstance();

    _CrtDumpMemoryLeaks();

    return 0;
}
