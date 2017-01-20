#ifndef __REFLECTION_H__
#define __REFLECTION_H__

//! author : chenyao

#undef RUNTIME_CLASS
#define RUNTIME_CLASS(class_name) ((CRuntimeClass*)(&class_name::class##class_name))

#undef DECLARE_CLASS
#define DECLARE_CLASS(class_name) \
    public: \
        static CObject * CreateObject(); \
        static CRuntimeClass class##class_name; \
        virtual CRuntimeClass * GetRuntimeClass();

#undef IMPLEMENT_CLASS
#define IMPLEMENT_CLASS(class_name) \
    CObject * class_name::CreateObject() \
        { return new class_name; } \
    CRuntimeClass class_name::class##class_name = \
        { #class_name, class_name::CreateObject, NULL }; \
    CRuntimeClass * class_name::GetRuntimeClass() \
        { return RUNTIME_CLASS(class_name); } \
    CLASSINIT _init_##class_name(RUNTIME_CLASS(class_name));

class CObject;

struct CRuntimeClass
{
    const char * m_className;
    CObject * (*m_pfnCreateObject)();
    CObject * CreateObject();

    static CRuntimeClass * FromName(const char * className);
    static CObject * CreateObject(const char * className);

    static CRuntimeClass * m_firstClass;
    CRuntimeClass * m_nextClass;       // linked list of registered classes
};

struct CLASSINIT { 
    CLASSINIT(CRuntimeClass* newClass) { 
        newClass->m_nextClass = CRuntimeClass::m_firstClass;
        CRuntimeClass::m_firstClass = newClass;
    } 
};

class CObject
{
protected:
    CObject() {}

public:
    virtual ~CObject() {}
    virtual CRuntimeClass * GetRuntimeClass();
    static struct CRuntimeClass classCObject;

private:
    CObject(const CObject& object);              // no implementation
    CObject& operator=(const CObject& object);   // no implementation

public:
    virtual const char * string() = 0;
};

#endif
