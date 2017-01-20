#include <iostream>
#include <string.h>
#include "reflection.h"

struct CRuntimeClass CObject::classCObject = 
    { "CObject", NULL, NULL };

CRuntimeClass * CRuntimeClass::m_firstClass = NULL;

static CLASSINIT _init_CObject(&CObject::classCObject);

CRuntimeClass * CObject::GetRuntimeClass()
{
    return RUNTIME_CLASS(CObject);
}

CObject * CRuntimeClass::CreateObject()
{
    if (NULL == m_pfnCreateObject)
        return NULL;
    return (*m_pfnCreateObject)();
}

CRuntimeClass * CRuntimeClass::FromName(const char * className)
{
    for (CRuntimeClass * iter = CRuntimeClass::m_firstClass; 
            NULL != iter->m_nextClass; iter = iter->m_nextClass) {
        if (0 == strcmp(className, iter->m_className)) {
            return iter;
        }
    }
    return NULL;
}

CObject * CRuntimeClass::CreateObject(const char * className) 
{
    CRuntimeClass* runtimeClass = FromName(className);

    if (NULL == runtimeClass)
        return NULL;

    return runtimeClass->CreateObject();
}

class CDemoObject : public CObject
{
    DECLARE_CLASS(CDemoObject)
public:
    virtual const char * string() { return GetRuntimeClass()->m_className; }
};

IMPLEMENT_CLASS(CDemoObject)

class CLargeObject : public CObject
{
    DECLARE_CLASS(CLargeObject)
public:
    virtual const char * string() { return GetRuntimeClass()->m_className; }
};

IMPLEMENT_CLASS(CLargeObject)


int main()
{
    CObject * object = CRuntimeClass::CreateObject("CLargeObject");

    if (NULL == object)
        return 0;

    std::cout << object->string() << std::endl;

    delete object;

    return 0;
}
