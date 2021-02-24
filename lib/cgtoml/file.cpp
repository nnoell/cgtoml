/* CgToml
 *
 * Copyright © 2019 Collabora Ltd.
 * Copyright © 2021 Julian Bouzas
 *
 * SPDX-License-Identifier: MIT
 */

/* CPPTOML */
#include <include/cpptoml.h>

/* TOML */
#include "private.h"
#include "file.h"

struct _CgTomlFile
{
  char *name;
  CgTomlTable *table;
};

G_DEFINE_BOXED_TYPE(CgTomlFile, cg_toml_file, cg_toml_file_ref,
    cg_toml_file_unref)

CgTomlFile *
cg_toml_file_new (const char *name)
{
  g_return_val_if_fail (name, nullptr);

  try {
    g_autoptr (CgTomlFile) self = g_rc_box_new (CgTomlFile);

    /* Set the name */
    self->name = g_strdup (name);

    /* Set the table by parsing the file */
    std::shared_ptr<cpptoml::table> data = cpptoml::parse_file(name);
    self->table = cg_toml_table_new (static_cast<gconstpointer>(&data));

    return static_cast<CgTomlFile *>(g_steal_pointer (&self));
  } catch (std::bad_alloc& ba) {
    g_critical ("Could not create CgTomlFile from '%s': %s", name, ba.what());
    return nullptr;
  } catch (...) {
    g_critical ("Could not create CgTomlFile from '%s'", name);
    return nullptr;
  }
}

CgTomlFile *
cg_toml_file_ref (CgTomlFile * self)
{
  return static_cast<CgTomlFile *>(
    g_rc_box_acquire (static_cast<gpointer>(self)));
}

void
cg_toml_file_unref (CgTomlFile * self)
{
  static void (*free_func)(gpointer) = [](gpointer p){
    CgTomlFile *f = static_cast<CgTomlFile *>(p);
    g_free (f->name);
    f->name = nullptr;
    cg_toml_table_unref (f->table);
    f->table = nullptr;
  };
  g_rc_box_release_full (self, free_func);
}

const char *
cg_toml_file_get_name (const CgTomlFile *self)
{
  return self->name;
}

CgTomlTable *
cg_toml_file_get_table (const CgTomlFile *self)
{
  return cg_toml_table_ref (self->table);
}
