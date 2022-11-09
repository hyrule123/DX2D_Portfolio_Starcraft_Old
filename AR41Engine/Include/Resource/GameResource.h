#pragma once

#include "../Ref.h"

class CGameResource :
    public CRef
{
protected:
    CGameResource();
    CGameResource(const CGameResource& Res);
    virtual ~CGameResource();

    EResourceType m_ResourceType;
    bool m_Essential;

public:
    inline void SetResourceType(const EResourceType& ResourceType);
    inline const EResourceType& GetResourceType() const;
    inline void SetEssential(const bool& b);
    inline const bool& GetEssential() const;
};


inline void CGameResource::SetResourceType(const EResourceType& ResourceType)
{
    m_ResourceType = ResourceType;
}

inline const EResourceType& CGameResource::GetResourceType() const
{
    return m_ResourceType;
}

inline void CGameResource::SetEssential(const bool& b)
{
    m_Essential = b;
}

inline const bool& CGameResource::GetEssential() const
{
    return m_Essential;
}
