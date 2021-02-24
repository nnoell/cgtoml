/* CgToml
 *
 * Copyright © 2019 Collabora Ltd.
 * Copyright © 2021 Julian Bouzas
 *
 * SPDX-License-Identifier: MIT
 */

/* C++ STL */
#include <functional>

/* CPPTOML */
#include <include/cpptoml.h>

/* TOML */
#include "private.h"
#include "table.h"

namespace cg {
namespace toml {

/* The Table class */
class Table {
 public:
  /* The data of the array */
  using Data = std::shared_ptr<const cpptoml::table>;

  /* Constructor */
  Table(Data data) :
    data_(std::move(data)) {
  }

  /* Destructor */
  virtual ~Table() {
  }

  /* Determines if this table contains the given key */
  bool Contains(const std::string& key) const {
    return data_->contains(key);
  }

  /* Gets a value */
  template <typename T>
  bool GetValue(const std::string& key, T *val, bool qualified) const {
    g_return_val_if_fail (val, false);
    const cpptoml::option<T> opt =
        qualified ? data_->get_qualified_as<T>(key) : data_->get_as<T>(key);
    if (!opt)
      return false;
    *val = *opt;
    return true;
  }

  /* Gets an array of values */
  std::shared_ptr<const cpptoml::array> GetArray(const std::string& key,
      bool qualified) const {
    return qualified ? data_->get_array_qualified(key) : data_->get_array(key);
  }

  /* Gets an array of tables */
  std::shared_ptr<const cpptoml::table_array> GetTableArray(
      const std::string& key, bool qualified) const {
    return qualified ? data_->get_table_array_qualified(key) :
        data_->get_table_array(key);
  }

  /* Gets a nested table */
  Data GetTable(const std::string& key, bool qualified) const {
    return qualified ? data_->get_table_qualified(key) : data_->get_table(key);
  }

 private:
  /* Copy Constructor */
  Table(const Table&) = delete;

  /* Move Constructor */
  Table(Table &&) = delete;

  /* Copy-Assign Constructor */
  Table& operator=(const Table&) = delete;

  /* Move-Assign Constructr */
  Table& operator=(Table &&) = delete;

 private:
  /* The data table */
  const Data data_;
};

/* The Array Table class */
class TableArray {
 public:
  /* The data of the array */
  using Data = std::shared_ptr<const cpptoml::table_array>;

  /* The for each function for arrays of tables */
  using ForEachFunction = std::function<void(CgTomlTable *, gpointer)>;

  /* Constructor */
  TableArray(Data data) :
      data_(std::move(data)) {
  }

  /* Destructor */
  virtual ~TableArray() {
  }

  /* Calls the given callback for arrays of values */
  void ForEach(ForEachFunction func, gpointer user_data) const {
    for (const auto& table : *data_) {
      gconstpointer p = static_cast<gconstpointer>(&table);
      g_autoptr (CgTomlTable) t = cg_toml_table_new(p);
      func(t, user_data);
    }
  }

 private:
  /* Copy Constructor */
  TableArray(const TableArray&) = delete;

  /* Move Constructor */
  TableArray(TableArray &&) = delete;

  /* Copy-Assign Constructor */
  TableArray& operator=(const TableArray&) = delete;

  /* Move-Assign Constructr */
  TableArray& operator=(TableArray &&) = delete;

 private:
  /* The data array */
  const Data data_;
};

}  /* namespace toml */
}  /* namespace cg */

struct _CgTomlTable
{
  cg::toml::Table *data;
};

G_DEFINE_BOXED_TYPE(CgTomlTable, cg_toml_table, cg_toml_table_ref,
  cg_toml_table_unref)

struct _CgTomlTableArray
{
  const cg::toml::TableArray *data;
};

G_DEFINE_BOXED_TYPE(CgTomlTableArray, cg_toml_table_array,
    cg_toml_table_array_ref, cg_toml_table_array_unref)

CgTomlTable *
cg_toml_table_new (gconstpointer data)
{
  g_return_val_if_fail (data, nullptr);

  try {
    g_autoptr (CgTomlTable) self = g_rc_box_new (CgTomlTable);

    /* Set the data */
    const cg::toml::Table::Data *d =
        static_cast<const cg::toml::Table::Data *>(data);
    self->data = new cg::toml::Table {*d};

    return static_cast<CgTomlTable *>(g_steal_pointer (&self));
  } catch (std::bad_alloc& ba) {
    g_critical ("Could not create CgTomlTable: %s", ba.what());
    return nullptr;
  } catch (...) {
    g_critical ("Could not create CgTomlTable");
    return nullptr;
  }
}

CgTomlTable *
cg_toml_table_ref (CgTomlTable * self)
{
  return static_cast<CgTomlTable *>(
    g_rc_box_acquire (static_cast<gpointer>(self)));
}

void
cg_toml_table_unref (CgTomlTable * self)
{
  static void (*free_func)(gpointer) = [](gpointer p){
    CgTomlTable *t = static_cast<CgTomlTable *>(p);
    delete t->data;
  };
  g_rc_box_release_full (self, free_func);
}

static CgTomlTableArray *
cg_toml_table_array_new (gconstpointer data)
{
  g_return_val_if_fail (data, nullptr);

  try {
    g_autoptr (CgTomlTableArray) self = g_rc_box_new (CgTomlTableArray);

    /* Set the data */
    const cg::toml::TableArray::Data *d =
        static_cast<const cg::toml::TableArray::Data *>(data);
    self->data = new cg::toml::TableArray {*d};

    return static_cast<CgTomlTableArray *>(g_steal_pointer (&self));
  } catch (std::bad_alloc& ba) {
    g_critical ("Could not create CgTomlTableArray: %s", ba.what());
    return nullptr;
  } catch (...) {
    g_critical ("Could not create CgTomlTableArray");
    return nullptr;
  }
}

CgTomlTableArray *
cg_toml_table_array_ref (CgTomlTableArray * self)
{
  return static_cast<CgTomlTableArray *>(
    g_rc_box_acquire (static_cast<gpointer>(self)));
}

void
cg_toml_table_array_unref (CgTomlTableArray * self)
{
  static void (*free_func)(gpointer) = [](gpointer p){
    CgTomlTableArray *at = static_cast<CgTomlTableArray *>(p);
    delete at->data;
  };
  g_rc_box_release_full (self, free_func);
}

gboolean
cg_toml_table_contains (const CgTomlTable *self, const char *key) {
  return self->data->Contains(key);
}

gboolean
cg_toml_table_get_boolean (const CgTomlTable *self, const char *key,
  gboolean *val)
{
  bool v;
  if (!self->data->GetValue<bool>(key, &v, false))
    return false;
  *val = v ? TRUE : FALSE;
  return true;
}

gboolean
cg_toml_table_get_qualified_boolean (const CgTomlTable *self, const char *key,
    gboolean *val)
{
  bool v;
  if (!self->data->GetValue<bool>(key, &v, true))
    return false;
  *val = v ? TRUE : FALSE;
  return true;
}

gboolean
cg_toml_table_get_int8 (const CgTomlTable *self, const char *key, int8_t *val)
{
  return self->data->GetValue<int8_t>(key, val, false);
}

gboolean
cg_toml_table_get_qualified_int8 (const CgTomlTable *self, const char *key,
    int8_t *val)
{
  return self->data->GetValue<int8_t>(key, val, true);
}

gboolean
cg_toml_table_get_uint8 (const CgTomlTable *self, const char *key, uint8_t *val)
{
  return self->data->GetValue<uint8_t>(key, val, false);
}

gboolean
cg_toml_table_get_qualified_uint8 (const CgTomlTable *self, const char *key,
    uint8_t *val)
{
  return self->data->GetValue<uint8_t>(key, val, true);
}

gboolean
cg_toml_table_get_int16 (const CgTomlTable *self, const char *key, int16_t *val)
{
  return self->data->GetValue<int16_t>(key, val, false);
}

gboolean
cg_toml_table_get_qualified_int16 (const CgTomlTable *self, const char *key,
    int16_t *val)
{
  return self->data->GetValue<int16_t>(key, val, true);
}

gboolean
cg_toml_table_get_uint16 (const CgTomlTable *self, const char *key,
    uint16_t *val)
{
  return self->data->GetValue<uint16_t>(key, val, false);
}

gboolean
cg_toml_table_get_qualified_uint16 (const CgTomlTable *self, const char *key,
    uint16_t *val)
{
  return self->data->GetValue<uint16_t>(key, val, true);
}

gboolean
cg_toml_table_get_int32 (const CgTomlTable *self, const char *key, int32_t *val)
{
  return self->data->GetValue<int32_t>(key, val, false);
}

gboolean
cg_toml_table_get_qualified_int32 (const CgTomlTable *self, const char *key,
    int32_t *val)
{
  return self->data->GetValue<int32_t>(key, val, true);
}

gboolean
cg_toml_table_get_uint32 (const CgTomlTable *self, const char *key,
    uint32_t *val)
{
  return self->data->GetValue<uint32_t>(key, val, false);
}

gboolean
cg_toml_table_get_qualified_uint32 (const CgTomlTable *self, const char *key,
    uint32_t *val)
{
  return self->data->GetValue<uint32_t>(key, val, true);
}

gboolean
cg_toml_table_get_int64 (const CgTomlTable *self, const char *key, int64_t *val)
{
  return self->data->GetValue<int64_t>(key, val, false);
}

gboolean
cg_toml_table_get_qualified_int64 (const CgTomlTable *self, const char *key,
    int64_t *val)
{
  return self->data->GetValue<int64_t>(key, val, true);
}

gboolean
cg_toml_table_get_uint64 (const CgTomlTable *self, const char *key,
    uint64_t *val)
{
  return self->data->GetValue<uint64_t>(key, val, false);
}

gboolean
cg_toml_table_get_qualified_uint64 (const CgTomlTable *self, const char *key,
    uint64_t *val)
{
  return self->data->GetValue<uint64_t>(key, val, true);
}

gboolean
cg_toml_table_get_double (const CgTomlTable *self, const char *key, double *val)
{
  return self->data->GetValue<double>(key, val, false);
}

gboolean
cg_toml_table_get_qualified_double (const CgTomlTable *self, const char *key,
    double *val)
{
  return self->data->GetValue<double>(key, val, true);
}

char *
cg_toml_table_get_string (const CgTomlTable *self, const char *key)
{
  std::string str;
  return self->data->GetValue<std::string>(key, &str, false) ?
      g_strdup (str.c_str()) : nullptr;
}

char *
cg_toml_table_get_qualified_string (const CgTomlTable *self, const char *key)
{
  std::string str;
  return self->data->GetValue<std::string>(key, &str, true) ?
      g_strdup (str.c_str()) : nullptr;
}

CgTomlArray *
cg_toml_table_get_array (const CgTomlTable *self, const char *key)
{
  std::shared_ptr<const cpptoml::array> array =
      self->data->GetArray(key, false);
  return array ?
      cg_toml_array_new (static_cast<gconstpointer>(&array)) :
      nullptr;
}

CgTomlArray *
cg_toml_table_get_qualified_array (const CgTomlTable *self, const char *key)
{
  std::shared_ptr<const cpptoml::array> array =
      self->data->GetArray(key, true);
  return array ?
      cg_toml_array_new (static_cast<gconstpointer>(&array)) :
      nullptr;
}

CgTomlTable *
cg_toml_table_get_table (const CgTomlTable *self, const char *key)
{
  cg::toml::Table::Data table = self->data->GetTable(key, false);
  return table ?
      cg_toml_table_new (static_cast<gconstpointer>(&table)) :
      nullptr;
}

CgTomlTable *
cg_toml_table_get_qualified_table (const CgTomlTable *self, const char *key)
{
  cg::toml::Table::Data table = self->data->GetTable(key, true);
  return table ?
      cg_toml_table_new (static_cast<gconstpointer>(&table)) :
      nullptr;
}

CgTomlTableArray *
cg_toml_table_get_array_table (const CgTomlTable *self, const char *key)
{
  std::shared_ptr<const cpptoml::table_array> array_table =
      self->data->GetTableArray(key, false);
  return array_table ?
      cg_toml_table_array_new (static_cast<gconstpointer>(&array_table)) :
      nullptr;
}

CgTomlTableArray *
cg_toml_table_get_qualified_array_table (const CgTomlTable *self,
    const char *key)
{
  std::shared_ptr<const cpptoml::table_array> array_table =
      self->data->GetTableArray(key, true);
  return array_table ?
      cg_toml_table_array_new (static_cast<gconstpointer>(&array_table)) :
      nullptr;
}

void
cg_toml_table_array_for_each (const CgTomlTableArray *self,
    CgTomlTableArrayForEachFunc func, gpointer user_data)
{
  self->data->ForEach(func, user_data);
}
