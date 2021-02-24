/* CgToml
 *
 * Copyright © 2019 Collabora Ltd.
 * Copyright © 2021 Julian Bouzas
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef __CG_TOML_PRIVATE_H__
#define __CG_TOML_PRIVATE_H__

#include <glib-object.h>

G_BEGIN_DECLS

/* Forward declaration */
struct _CgTomlArray;
typedef struct _CgTomlArray CgTomlArray;
struct _TomlTable;
typedef struct _CgTomlTable CgTomlTable;

CgTomlArray * cg_toml_array_new (gconstpointer data);
CgTomlTable * cg_toml_table_new (gconstpointer data);

G_END_DECLS

#endif
