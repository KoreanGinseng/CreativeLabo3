#include "ResourceManager.h"

bool CResourceManager::LoadTexture(void)
{
    CUtilities::SetCurrentDirectory("Image");
    bool b[] = 
    {
        CTextureAsset::Load("Back_Blue"  , "BackGround/Blue.png"  ),
        CTextureAsset::Load("Back_Brown" , "BackGround/Brown.png" ),
        CTextureAsset::Load("Back_Gray"  , "BackGround/Gray.png"  ),
        CTextureAsset::Load("Back_Green" , "BackGround/Green.png" ),
        CTextureAsset::Load("Back_Pink"  , "BackGround/Pink.png"  ),
        CTextureAsset::Load("Back_Purple", "BackGround/Purple.png"),
        CTextureAsset::Load("Back_Yellow", "BackGround/Yellow.png"),
    };
    CUtilities::SetCurrentDirectory("../");

    for (const auto& it : b)
    {
        if (!it)
        {
            return false;
        }
    }

    return true;
}

bool CResourceManager::LoadSound(void)
{
    return true;
}

bool CResourceManager::LoadFont(void)
{
    return true;
}

bool CResourceManager::LoadResourceFont(void)
{
    bool b[] = 
    {
        CReFontAsset::Load("Main"  , "Font/Main.ttf\0アンニャントロマン\0\0"),
        CReFontAsset::Load("Button", "Font/Main.ttf\0アンニャントロマン\0\0"),
    };

    for (const auto& it : b)
    {
        if (!it)
        {
            return false;
        }
    }

    return true;
}

bool CResourceManager::Load(void)
{
    CUtilities::SetCurrentDirectory("Resource");
    bool b[] = 
    {
        LoadTexture(),
        LoadSound(),
        LoadFont(),
        LoadResourceFont(),
    };

    for (const auto& it : b)
    {
        if (!it)
        {
            return false;
        }
    }

    return true;
}

void CResourceManager::Release(void)
{
    CTextureAsset::Release();
    CSoundAsset  ::Release();
    CFontAsset   ::Release();
    CReFontAsset ::Release();
}

CTexturePtr CResourceManager::GetTexture(const ResourceKey & key)
{
    return CTextureAsset::GetAsset(key);
}

CSoundPtr CResourceManager::GetSound(const ResourceKey & key)
{
    return CSoundAsset::GetAsset(key);
}

CFontPtr CResourceManager::GetFont(const ResourceKey & key)
{
    return CFontAsset::GetAsset(key);
}

CReFontPtr CResourceManager::GetResourceFont(const ResourceKey & key)
{
    return CReFontAsset::GetAsset(key);
}
