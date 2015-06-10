
typedef enum __crlf_conv_enum {
    NO_CRLF_CONVERT,
    CONVERT_CRLF_TO_LF,
    REMOVE_CRLF,
} crlf_conv_enum;

typedef enum __ctrm_conv_enum {
    NO_CTRM_CONVERT,
    CONVERT_CTRM_TO_LF,
    CONVERT_CTRM_TO_CRLF,
    REMOVE_CTRM,
} ctrm_conv_enum;

void print_options(void);
int get_opt_item(int argc, char *argv[]);

