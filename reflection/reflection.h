#include <map>

#ifndef __REFLECTION_H__
#define __REFLECTION_H__

//! author : chenyao

#undef DECLARE_CLASS
#define DECLARE_CLASS(name) \
    public: \
        static CObject * CreateObject(); \
    private: \
        static ClassInfo m_classInfo;

#undef IMPLEMENT_CLASS
#define IMPLEMENT_CLASS(name) \
    CObject * name::CreateObject() { return new name; } \
    ClassInfo name::m_classInfo(#name, name::CreateObject);

class CObject;
typedef CObject * (*CreateObjectFunc)();

class ClassInfo
{
public:
    ClassInfo(std::string className, CreateObjectFunc object);
    CObject * CreateObject() {
        if (m_object) {
            return m_object();
        }
        return NULL;
    }

public:
    std::string m_name;
    CreateObjectFunc m_object;
};

class CObject
{
public:
    ~CObject() {}
    virtual std::string ToString() = 0;

public:
    static CObject * CreateObject(const std::string & className);
    static void Register(ClassInfo * classInfo);
    static std::map<std::string, ClassInfo*> m_classInfoMap; 
};

#endif
