#include <iostream>
#include "reflection.h"

std::map<std::string, ClassInfo*> CObject::m_classInfoMap; 

void CObject::Register(ClassInfo * classInfo) {
    if (NULL == classInfo)
        return;

    if (m_classInfoMap.find(classInfo->m_name) == m_classInfoMap.end()) {
        m_classInfoMap.insert(std::map<std::string, ClassInfo*>::value_type(classInfo->m_name, classInfo));
    }
}

CObject * CObject::CreateObject(const std::string & className)
{
    std::map<std::string, ClassInfo*>::const_iterator iter = CObject::m_classInfoMap.find(className);
    if (iter == CObject::m_classInfoMap.end())
        return NULL;

    return iter->second->CreateObject();
}

ClassInfo::ClassInfo(std::string className, CreateObjectFunc object)
    : m_name(className), m_object(object)
{ 
    CObject::Register(this);
}


class CDemoObject : public CObject
{
    DECLARE_CLASS(CDemoObject)
public:
    virtual std::string ToString() { return m_classInfo.m_name; }
};

IMPLEMENT_CLASS(CDemoObject)

CObject * FactoryCreate(const std::string & className)
{
    return CObject::CreateObject(className);
}

int main()
{
    CObject * object = FactoryCreate("CDemoObject");
    if (NULL == object)
        return 0;

    std::cout << object->ToString() << std::endl;

    delete object;

    return 0;
}
