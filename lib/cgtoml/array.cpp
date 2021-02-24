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
#include "array.h"

namespace cg {
namespace toml {

/* The Array class */
class Array {
 public:
  /* The data of the array */
  using Data = std::shared_ptr<const cpptoml::array>;

  /* The for each function for values */
  template <typename T>
  using ForEachValueFunction = std::function<void(const T*, gpointer)>;

  /* The for each function for arrays of values */
  using ForEachArrayFunction = std::function<void(CgTomlArray *, gpointer )>;

  /* Constructor */
  Array(Data data) :
      data_(std::move(data)) {
  }

  /* Destructor */
  virtual ~Array() {
  }

  /* Calls the given callback for values */
  template <typename T>
  void ForEachValue(ForEachValueFunction<T> func, gpointer user_data) const {
    for (const std::shared_ptr<cpptoml::value<T>>& v : data_->array_of<T>()) {
      if (v) {
        const T val = v->get();
        func(&val, user_data);
      } else {
        func(nullptr, user_data);
      }
    }
  }

  /* Calls the given callback for arrays of values */
  void ForEachArray(ForEachArrayFunction func, gpointer user_data) const {
    for (const Data& val : data_->nested_array()) {
      gconstpointer d = static_cast<gconstpointer>(&val);
      g_autoptr (CgTomlArray) a = cg_toml_array_new(d);
      func(a, user_data);
    }
  }

 private:
  /* Copy Constructor */
  Array(const Array&) = delete;

  /* Move Constructor */
  Array(Array &&) = delete;

  /* Copy-Assign Constructor */
  Array& operator=(const Array&) = delete;

  /* Move-Assign Constructr */
  Array& operator=(Array &&) = delete;

 private:
  /* The data array */
  const Data data_;
};

}  /* namespace toml */
}  /* namespace cg */

struct _CgTomlArray
{
  const cg::toml::Array *data;
};

G_DEFINE_BOXED_TYPE(CgTomlArray, cg_toml_array, cg_toml_array_ref,
    cg_toml_array_unref)

CgTomlArray *
cg_toml_array_new (gconstpointer data)
{
  g_return_val_if_fail (data, nullptr);

  try {
    g_autoptr(CgTomlArray) self = g_rc_box_new (CgTomlArray);

    /* Set the data */
    const cg::toml::Array::Data *d =
        static_cast<const cg::toml::Array::Data *>(data);
    self->data = new cg::toml::Array {*d};

    return static_cast<CgTomlArray *>(g_steal_pointer (&self));
  } catch (std::bad_alloc& ba) {
    g_critical ("Could not create CgTomlArray: %s", ba.what());
    return nullptr;
  } catch (...) {
    g_critical ("Could not create CgTomlArray");
    return nullptr;
  }
}

CgTomlArray *
cg_toml_array_ref (CgTomlArray * self)
{
  return static_cast<CgTomlArray *>(
    g_rc_box_acquire (static_cast<gpointer>(self)));
}

void
cg_toml_array_unref (CgTomlArray * self)
{
  static void (*free_func)(gpointer) = [](gpointer p){
    CgTomlArray *a = static_cast<CgTomlArray *>(p);
    delete a->data;
  };
  g_rc_box_release_full (self, free_func);
}

void
cg_toml_array_for_each_boolean (const CgTomlArray *self,
    CgTomlArrayForEachBoolFunc func, gpointer user_data)
{
  self->data->ForEachValue<bool>([&](const bool *v, gpointer d){
      if (v) {
        const gboolean b = *v ? TRUE : FALSE;
        func(&b, d);
      } else {
        func(nullptr, d);
      }
    }, user_data);
}

void
cg_toml_array_for_each_int64 (const CgTomlArray *self,
    CgTomlArrayForEachInt64Func func, gpointer user_data)
{
  self->data->ForEachValue<int64_t>(func, user_data);
}

void
cg_toml_array_for_each_double (const CgTomlArray *self,
    CgTomlArrayForEachDoubleFunc func, gpointer user_data)
{
  self->data->ForEachValue<double>(func, user_data);
}

void
cg_toml_array_for_each_string (const CgTomlArray *self,
    CgTomlArrayForEachStringFunc func, gpointer user_data)
{
  self->data->ForEachValue<std::string>([&](const std::string *v, gpointer d){
      func(v ? v->c_str() : nullptr, d);
    }, user_data);
}

void
cg_toml_array_for_each_array (const CgTomlArray *self,
    CgTomlArrayForEachArrayFunc func, gpointer user_data)
{
  self->data->ForEachArray(func, user_data);
}
