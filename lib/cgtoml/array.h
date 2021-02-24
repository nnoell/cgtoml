/* CgToml
 *
 * Copyright © 2019 Collabora Ltd.
 * Copyright © 2021 Julian Bouzas
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef __CG_TOML_ARRAY_H__
#define __CG_TOML_ARRAY_H__

#include <glib-object.h>

#include <stdint.h>

G_BEGIN_DECLS

/* CgTomlArray */
GType cg_toml_array_get_type (void);
typedef struct _CgTomlArray CgTomlArray;
CgTomlArray * cg_toml_array_ref (CgTomlArray * self);
void cg_toml_array_unref (CgTomlArray * self);
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CgTomlArray, cg_toml_array_unref)

/* API */
typedef void (*CgTomlArrayForEachBoolFunc)(const gboolean *, gpointer);
void cg_toml_array_for_each_boolean (const CgTomlArray *self,
    CgTomlArrayForEachBoolFunc func, gpointer user_data);
typedef void (*CgTomlArrayForEachInt64Func)(const int64_t *, gpointer);
void cg_toml_array_for_each_int64 (const CgTomlArray *self,
    CgTomlArrayForEachInt64Func func, gpointer user_data);
typedef void (*CgTomlArrayForEachDoubleFunc)(const double *, gpointer);
void cg_toml_array_for_each_double (const CgTomlArray *self,
    CgTomlArrayForEachDoubleFunc func, gpointer user_data);
typedef void (*CgTomlArrayForEachStringFunc)(const char *, gpointer);
void cg_toml_array_for_each_string (const CgTomlArray *self,
    CgTomlArrayForEachStringFunc func, gpointer user_data);
typedef void (*CgTomlArrayForEachArrayFunc)(CgTomlArray *, gpointer);
void cg_toml_array_for_each_array (const CgTomlArray *self,
    CgTomlArrayForEachArrayFunc func, gpointer user_data);

G_END_DECLS

#endif
