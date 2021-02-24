/* CgToml
 *
 * Copyright © 2019 Collabora Ltd.
 * Copyright © 2021 Julian Bouzas
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef __CG_TOML_FILE_H__
#define __CG_TOML_FILE_H__

#include <glib-object.h>

#include "table.h"

G_BEGIN_DECLS

/* CgTomlFile */
GType cg_toml_file_get_type (void);
typedef struct _CgTomlFile CgTomlFile;
CgTomlFile * cg_toml_file_new (const char *name);
CgTomlFile * cg_toml_file_ref (CgTomlFile * self);
void cg_toml_file_unref (CgTomlFile * self);
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CgTomlFile, cg_toml_file_unref)

/* API */
const char * cg_toml_file_get_name (const CgTomlFile *self);
CgTomlTable * cg_toml_file_get_table (const CgTomlFile *self);

G_END_DECLS

#endif
