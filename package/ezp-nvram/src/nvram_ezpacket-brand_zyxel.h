#ifdef EZP_SUB_BRAND_FEENEY
#if defined(EZP_PROD_CAT_N)
    #define MODEL "Feeney VFG6005N"
#elif defined(EZP_PROD_CAT_A)
    #define MODEL "Feeney VFG6005"
#endif
{ "brand", "FEENEY", 
    NVRAM_NONE,
    0 
},  /* Brand name - CSS style and SSID. */
{ "model", "Feeney Product", 
    NVRAM_PROTECTED | NVRAM_CUSTOMIZED,
    0 
},              /* Product model */
#else
#if defined(EZP_PROD_CAT_N)
    #define MODEL "ZyXEL VFG6005N"
#elif defined(EZP_PROD_CAT_A)
    #define MODEL "ZyXEL VFG6005"
#endif
{ "brand", "ZYUS", 
    NVRAM_NONE,
    0 
},  /* Brand name - CSS style and SSID. */
{ "model", "ZyXEL Product", 
    NVRAM_PROTECTED | NVRAM_CUSTOMIZED,
    0 
},              /* Product model */
#endif
{ "lang_default", "EN",
    NVRAM_PROTECTED | NVRAM_CUSTOMIZED,
    0
},  /* Default web language support */
