/* CgToml
 *
 * Copyright © 2019 Collabora Ltd.
 * Copyright © 2021 Julian Bouzas
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef __CG_TOML_TABLE_H__
#define __CG_TOML_TABLE_H__

#include <glib-object.h>

#include <stdint.h>

#include "array.h"

G_BEGIN_DECLS

/* CgTomlTable */
GType cg_toml_table_get_type (void);
typedef struct _CgTomlTable CgTomlTable;
CgTomlTable * cg_toml_table_ref (CgTomlTable * self);
void cg_toml_table_unref (CgTomlTable * self);
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CgTomlTable, cg_toml_table_unref)

/* CgTomlTableArray */
GType cg_toml_table_array_get_type (void);
typedef struct _CgTomlTableArray CgTomlTableArray;
CgTomlTableArray * cg_toml_table_array_ref (CgTomlTableArray * self);
void cg_toml_table_array_unref (CgTomlTableArray * self);
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CgTomlTableArray, cg_toml_table_array_unref)

/* API */
gboolean cg_toml_table_contains (const CgTomlTable *self, const char *key);
gboolean cg_toml_table_get_boolean (const CgTomlTable *self, const char *key,
    gboolean *val);
gboolean cg_toml_table_get_qualified_boolean (const CgTomlTable *self,
    const char *key, gboolean *val);
gboolean cg_toml_table_get_int8 (const CgTomlTable *self, const char *key,
    int8_t *val);
gboolean cg_toml_table_get_qualified_int8 (const CgTomlTable *self,
    const char *key, int8_t *val);
gboolean cg_toml_table_get_uint8 (const CgTomlTable *self, const char *key,
    uint8_t *val);
gboolean cg_toml_table_get_qualified_uint8 (const CgTomlTable *self,
    const char *key, uint8_t *val);
gboolean cg_toml_table_get_int16 (const CgTomlTable *self, const char *key,
    int16_t *val);
gboolean cg_toml_table_get_qualified_int16 (const CgTomlTable *self,
    const char *key, int16_t *val);
gboolean cg_toml_table_get_uint16 (const CgTomlTable *self, const char *key,
    uint16_t *val);
gboolean cg_toml_table_get_qualified_uint16 (const CgTomlTable *self,
    const char *key, uint16_t *val);
gboolean cg_toml_table_get_int32 (const CgTomlTable *self, const char *key,
    int32_t *val);
gboolean cg_toml_table_get_qualified_int32 (const CgTomlTable *self,
    const char *key, int32_t *val);
gboolean cg_toml_table_get_uint32 (const CgTomlTable *self, const char *key,
    uint32_t *val);
gboolean cg_toml_table_get_qualified_uint32 (const CgTomlTable *self,
    const char *key, uint32_t *val);
gboolean cg_toml_table_get_int64 (const CgTomlTable *self, const char *key,
    int64_t *val);
gboolean cg_toml_table_get_qualified_int64 (const CgTomlTable *self,
    const char *key, int64_t *val);
gboolean cg_toml_table_get_uint64 (const CgTomlTable *self, const char *key,
    uint64_t *val);
gboolean cg_toml_table_get_qualified_uint64 (const CgTomlTable *self,
    const char *key, uint64_t *val);
gboolean cg_toml_table_get_double (const CgTomlTable *self, const char *key,
    double *val);
gboolean cg_toml_table_get_qualified_double (const CgTomlTable *self,
    const char *key, double *val);
char * cg_toml_table_get_string (const CgTomlTable *self, const char *key);
char * cg_toml_table_get_qualified_string (const CgTomlTable *self,
    const char *key);
CgTomlArray * cg_toml_table_get_array (const CgTomlTable *self, const char *key);
CgTomlArray * cg_toml_table_get_qualified_array (const CgTomlTable *self,
    const char *key);
CgTomlTable * cg_toml_table_get_table (const CgTomlTable *self, const char *key);
CgTomlTable * cg_toml_table_get_qualified_table (const CgTomlTable *self,
    const char *key);
CgTomlTableArray *cg_toml_table_get_array_table (const CgTomlTable *self,
    const char *key);
CgTomlTableArray *cg_toml_table_get_qualified_array_table (
    const CgTomlTable *self, const char *key);
typedef void (*CgTomlTableArrayForEachFunc)(const CgTomlTable *, gpointer);
void cg_toml_table_array_for_each (const CgTomlTableArray *self,
    CgTomlTableArrayForEachFunc func, gpointer uder_data);

G_END_DECLS

#endif
