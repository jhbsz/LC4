#ifdef EZP_SUB_BRAND_APOLLO
{ "hostname", "RR-WC310", 
    NVRAM_NONE,
    0 
},  /* Hostname */
{ "brand", "APOLLO", 
    NVRAM_NONE, 
    0 
},  /* Brand name - CSS style and SSID. */
{ "model", "RR-WC310", 
    NVRAM_NONE,
    0 
},              /* Product model */
#else
{ "hostname", "LAN-Cell", 
        NVRAM_PROTECTED | NVRAM_CUSTOMIZED,
    0 
},  /* Hostname */
{ "brand", "PROX", 
    NVRAM_NONE, 
    0 
},  /* Brand name - CSS style and SSID. */
{ "model", "LAN-Cell 3", 
    NVRAM_PROTECTED | NVRAM_CUSTOMIZED,
    0 
},              /* Product model */
#endif
{ "lang_default", "EN_PROX",
    NVRAM_NONE,
    0
},  /* Default web language support */
{ "prox_serial", "",
    NVRAM_PROTECTED | NVRAM_CUSTOMIZED,
    0
},  /* Default prox serial number is empty*/
