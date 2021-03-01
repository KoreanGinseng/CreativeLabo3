#pragma once
#include "AssetBase.h"
#include "ResourceFont.h"

using CTexturePtr = std::shared_ptr<CTexture>;
using CSoundPtr   = std::shared_ptr<CSoundBuffer>;
using CFontPtr    = std::shared_ptr<sip::CFontA>;
using CReFontPtr  = std::shared_ptr<sip::CResourceFont>;

using ResourceKey = std::string;

// ********************************************************************************
/// <summary>
/// ëfçﬁä«óùÉNÉâÉX
/// </summary>
// ********************************************************************************
class CResourceManager
{
private:

    CResourceManager(void)  = delete;
    ~CResourceManager(void) = delete;

    using CTextureAsset = sip::CAssetBase<ResourceKey, CTexture>;
    using CSoundAsset   = sip::CAssetBase<ResourceKey, CSoundBuffer>;
    using CFontAsset    = sip::CAssetBase<ResourceKey, sip::CFontA>;
    using CReFontAsset  = sip::CAssetBase<ResourceKey, sip::CResourceFont>;

    static bool LoadTexture     (void);
    static bool LoadSound       (void);
    static bool LoadFont        (void);
    static bool LoadResourceFont(void);

public:

    static bool Load(void);

    static void Release(void);

    static CTexturePtr GetTexture     (const ResourceKey& key);
    static CSoundPtr   GetSound       (const ResourceKey& key);
    static CFontPtr    GetFont        (const ResourceKey& key);
    static CReFontPtr  GetResourceFont(const ResourceKey& key);

};

