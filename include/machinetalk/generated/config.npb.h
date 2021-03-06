/* Automatically generated nanopb header */
/* Generated by nanopb-0.3.0 at Sun Feb  1 12:40:11 2015. */

#ifndef PB_CONFIG_NPB_H_INCLUDED
#define PB_CONFIG_NPB_H_INCLUDED
#include <pb.h>

#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Enum definitions */
typedef enum _pb_ApplicationType {
    pb_ApplicationType_QT5_QML = 1,
    pb_ApplicationType_GLADEVCP = 2,
    pb_ApplicationType_JAVASCRIPT = 3
} pb_ApplicationType;

typedef enum _pb_FileContent {
    pb_FileContent_CLEARTEXT = 1,
    pb_FileContent_ZLIB = 2
} pb_FileContent;

/* Struct definitions */
typedef struct _pb_File {
    pb_callback_t name;
    pb_FileContent encoding;
    pb_callback_t blob;
} pb_File;

typedef struct _pb_Application {
    pb_callback_t name;
    pb_callback_t description;
    bool has_type;
    pb_ApplicationType type;
    pb_callback_t weburi;
    pb_callback_t file;
} pb_Application;

/* Default values for struct fields */

/* Initializer values for message structs */
#define pb_File_init_default                     {{{NULL}, NULL}, (pb_FileContent)0, {{NULL}, NULL}}
#define pb_Application_init_default              {{{NULL}, NULL}, {{NULL}, NULL}, false, (pb_ApplicationType)0, {{NULL}, NULL}, {{NULL}, NULL}}
#define pb_File_init_zero                        {{{NULL}, NULL}, (pb_FileContent)0, {{NULL}, NULL}}
#define pb_Application_init_zero                 {{{NULL}, NULL}, {{NULL}, NULL}, false, (pb_ApplicationType)0, {{NULL}, NULL}, {{NULL}, NULL}}

/* Field tags (for use in manual encoding/decoding) */
#define pb_File_name_tag                         1
#define pb_File_encoding_tag                     2
#define pb_File_blob_tag                         3
#define pb_Application_name_tag                  1
#define pb_Application_description_tag           2
#define pb_Application_type_tag                  3
#define pb_Application_weburi_tag                4
#define pb_Application_file_tag                  5

/* Struct field encoding specification for nanopb */
extern const pb_field_t pb_File_fields[4];
extern const pb_field_t pb_Application_fields[6];

/* Maximum encoded size of messages (where known) */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
