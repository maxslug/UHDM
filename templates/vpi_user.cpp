/*
 Do not modify, auto-generated by model_gen.tcl

 Copyright 2019 Alain Dargelas

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

/*
 * File:   vpi_user.cpp
 * Author:
 *
 * Created on December 14, 2019, 10:03 PM
 */
#include <uhdm/sv_vpi_user.h>
#include <uhdm/vhpi_user.h>

#include <string.h>
#if defined(_MSC_VER)
  #define strcasecmp _stricmp
  #define strdup _strdup
#else
  #include <strings.h>
#endif

#include <iostream>
#include <map>
#include <string>
#include <string_view>
#include <vector>

#include <uhdm/Serializer.h>
#include <uhdm/containers.h>
#include <uhdm/uhdm.h>
#include <uhdm/uhdm_types.h>
#include <uhdm/vpi_uhdm.h>

<HEADERS>

using namespace UHDM;

UHDM::design* UhdmDesignFromVpiHandle(vpiHandle hdesign) {
  if (!hdesign) return nullptr;
  UHDM::any* tmp = (UHDM::any*)((uhdm_handle*)hdesign)->object;
  if (tmp->UhdmType() == uhdmdesign)
    return (UHDM::design*)tmp;
  else
    return nullptr;
}

s_vpi_value* String2VpiValue(const std::string& s) {
  s_vpi_value* val = new s_vpi_value;
  val->format = 0;
  val->value.integer = 0;
  val->value.scalar = 0;
  val->value.str = nullptr;
  size_t pos;
  if ((pos = s.find("UINT:")) != std::string::npos) {
    val->format = vpiUIntVal;
    val->value.uint = std::strtoull(s.c_str() + pos + strlen("UINT:"), 0, 10);
  } else if ((pos = s.find("INT:")) != std::string::npos) {
    val->format = vpiIntVal;
    val->value.integer = std::strtoll(s.c_str() + pos + strlen("INT:"), 0, 10);
  } else if ((pos = s.find("SCAL:")) != std::string::npos) {
    val->format = vpiScalarVal;
    const char* const parse_pos = s.c_str() + pos + strlen("SCAL:");
    switch (parse_pos[0]) {
      case 'Z': val->value.scalar = vpiZ; break;
      case 'X': val->value.scalar = vpiX; break;
      case 'H': val->value.scalar = vpiH; break;
      case 'L': val->value.scalar = vpiL; break;
        // Not really clear what the difference between X and DontCare is.
        // Let's parse 'W'eak don't care as this one.
      case 'W': val->value.scalar = vpiDontCare; break;
      default:
        if (strcasecmp(parse_pos, "DontCare") == 0) {
          val->value.scalar = vpiDontCare;
        } else if (strcasecmp(parse_pos, "NoChange") == 0) {
          val->value.scalar = vpiNoChange;
        } else {
          val->value.scalar = atoi(parse_pos);  // Maybe written numerically?
        }
        break;
    }
  } else if ((pos = s.find("BIN:")) != std::string::npos) {
    val->format = vpiBinStrVal;
    val->value.str = strdup(s.c_str() + pos + strlen("BIN:"));
  } else if ((pos = s.find("HEX:")) != std::string::npos) {
    val->format = vpiHexStrVal;
    val->value.str = strdup(s.c_str() + pos + strlen("HEX:"));
  } else if ((pos = s.find("OCT:")) != std::string::npos) {
    val->format = vpiOctStrVal;
    val->value.str = strdup(s.c_str() + pos + strlen("OCT:"));
  } else if ((pos = s.find("STRING:")) != std::string::npos) {
    val->format = vpiStringVal;
    val->value.str = strdup(s.c_str() + pos + strlen("STRING:"));
  } else if ((pos = s.find("REAL:")) != std::string::npos) {
    val->format = vpiRealVal;
    val->value.real = atof(s.c_str() + pos + strlen("REAL:"));
  } else if ((pos = s.find("DEC:")) != std::string::npos) {
    val->format = vpiDecStrVal;
    val->value.str = strdup(s.c_str() + pos + strlen("DEC:"));
  }

  return val;
}

s_vpi_delay* String2VpiDelays(const std::string& s) {
  std::string scopy = s;
  s_vpi_delay* delay = new s_vpi_delay;
  delay->da = nullptr;
  if (strstr(scopy.c_str(), "#")) {
    scopy.erase(0, 1);
    delay->da = new t_vpi_time;
    delay->no_of_delays = 1;
    delay->time_type = vpiScaledRealTime;
    delay->da[0].low = atoi(scopy.c_str());
    delay->da[0].type = vpiScaledRealTime;
  }
  return delay;
}

std::string VpiValue2String(const s_vpi_value* value) {
  static const std::string kIntPrefix("INT:");
  static const std::string kUIntPrefix("UINT:");
  static const std::string kScalPrefix("SCAL:");
  static const std::string kStrPrefix("STRING:");
  static const std::string kHexPrefix("HEX:");
  static const std::string kOctPrefix("OCT:");
  static const std::string kBinPrefix("BIN:");
  static const std::string kRealPrefix("REAL:");
  static const std::string kDecPrefix("DEC:");

  if (!value) return "";
  switch (value->format) {
    case vpiIntVal: return kIntPrefix + std::to_string(value->value.integer);
    case vpiUIntVal: return kUIntPrefix + std::to_string(value->value.uint);
    case vpiScalarVal: {
      switch (value->value.scalar) {
        case vpi0: return "SCAL:0";
        case vpi1: return "SCAL:1";
        case vpiZ: return "SCAL:Z";
        case vpiX: return "SCAL:X";
        case vpiH: return "SCAL:H";
        case vpiL: return "SCAL:L";
        case vpiDontCare: return "SCAL:DontCare";
        case vpiNoChange: return "SCAL:NoChange";
        default:
          // mmh, some unknown number.
          return kScalPrefix + std::to_string(value->value.scalar);
      }
    }
    case vpiStringVal: return kStrPrefix + value->value.str;
    case vpiHexStrVal: return kHexPrefix + value->value.str;
    case vpiOctStrVal: return kOctPrefix + value->value.str;
    case vpiBinStrVal: return kBinPrefix + value->value.str;
    case vpiDecStrVal: return kDecPrefix + value->value.str;
    case vpiRealVal: return kRealPrefix + std::to_string(value->value.real);
  }

  return "";
}

std::string VpiDelay2String(const s_vpi_delay* delay) {
  std::string result;
  if (delay == nullptr) return result;
  if (delay->da == nullptr) return result;
  switch (delay->time_type) {
    case vpiScaledRealTime: {
      return std::string(std::string("#") + std::to_string(delay->da[0].low));
      break;
    }
    default:
      break;
  }
  return result;
}

vpiHandle NewVpiHandle(const UHDM::BaseClass* object) {
  return reinterpret_cast<vpiHandle>(
      new uhdm_handle(object->UhdmType(), object));
}

static vpiHandle NewHandle(UHDM_OBJECT_TYPE type, const void* object) {
  return reinterpret_cast<vpiHandle>(new uhdm_handle(type, object));
}

vpiHandle vpi_handle_by_index(vpiHandle object, PLI_INT32 indx) { return 0; }

vpiHandle vpi_handle_by_name(PLI_BYTE8* name, vpiHandle refHandle) {
  const uhdm_handle* const handle = (const uhdm_handle*)refHandle;
  const BaseClass* const object = (const BaseClass*)handle->object;
<VPI_HANDLE_BY_NAME_BODY>
}

vpiHandle vpi_handle(PLI_INT32 type, vpiHandle refHandle) {
  const uhdm_handle* const handle = (const uhdm_handle*)refHandle;
  const BaseClass* const object = (const BaseClass*)handle->object;
<VPI_HANDLE_BODY>
}

vpiHandle vpi_handle_multi(PLI_INT32 type, vpiHandle refHandle1,
                           vpiHandle refHandle2, ...) {
  return 0;
}

/* for traversing relationships */

vpiHandle vpi_iterate(PLI_INT32 type, vpiHandle refHandle) {
  const uhdm_handle* const handle = (const uhdm_handle*)refHandle;
  const BaseClass* const object = (const BaseClass*)handle->object;
<VPI_ITERATE_BODY>
}

vpiHandle vpi_scan(vpiHandle iterator) {
  if (!iterator) return 0;
  uhdm_handle* handle = (uhdm_handle*)iterator;
  const std::vector<const BaseClass*>* vect = (const std::vector<const BaseClass*>*)handle->object;
  if (handle->index < vect->size()) {
    const BaseClass* const object = vect->at(handle->index);
    uhdm_handle* h = new uhdm_handle(object->UhdmType(), object);
    ++handle->index;
    return (vpiHandle) h;
  }
  return nullptr;
}

PLI_INT32 vpi_free_object(vpiHandle object) {
  return vpi_release_handle(object);
}

PLI_INT32 vpi_release_handle(vpiHandle object) {
  delete (uhdm_handle*)object;
  return 0;
}

/* for processing properties */

PLI_INT32 vpi_get(PLI_INT32 property, vpiHandle object) {
  if (!object) {
    std::cout << "VPI ERROR: Bad usage of vpi_get" << std::endl;
    return 0;
  }

  // At this point, the implementation is exactly the same as for 64 bit,
  // but we truncate.
  return (PLI_INT32)vpi_get64(property, object);
}

PLI_INT64 vpi_get64(PLI_INT32 property, vpiHandle object) {
  if (!object) {
    std::cout << "VPI ERROR: Bad usage of vpi_get64" << std::endl;
    return 0;
  }

  const uhdm_handle* const handle = (const uhdm_handle*)object;
  const BaseClass* const obj = (const BaseClass*)handle->object;
<VPI_GET_BODY>
}

PLI_BYTE8* vpi_get_str(PLI_INT32 property, vpiHandle object) {
  if (!object) {
    std::cout << "VPI ERROR: Bad usage of vpi_get_str" << std::endl;
    return 0;
  }
  const uhdm_handle* const handle = (const uhdm_handle*)object;
  const BaseClass* const obj = (const BaseClass*)handle->object;
<VPI_GET_STR_BODY>
}

/* delay processing */

void vpi_get_delays(vpiHandle object, p_vpi_delay delay_p) {
  delay_p->da = nullptr;
  if (!object) {
    std::cout << "VPI ERROR: Bad usage of vpi_get_delay" << std::endl;
    return;
  }
  const uhdm_handle* const handle = (const uhdm_handle*)object;
  const BaseClass* const obj = (const BaseClass*)handle->object;
<VPI_GET_DELAY_BODY>
}

void vpi_put_delays(vpiHandle object, p_vpi_delay delay_p) {}

/* value processing */

void vpi_get_value(vpiHandle vexpr, p_vpi_value value_p) {
  value_p->format = 0;
  if (!vexpr) {
    std::cout << "VPI ERROR: Bad usage of vpi_get_value" << std::endl;
    return;
  }
  const uhdm_handle* const handle = (const uhdm_handle*)vexpr;
  const BaseClass* const obj = (const BaseClass*)handle->object;
<VPI_GET_VALUE_BODY>
}

vpiHandle vpi_put_value(vpiHandle object, p_vpi_value value_p,
                        p_vpi_time time_p, PLI_INT32 flags) {
  return 0;
}

void vpi_get_value_array(vpiHandle object, p_vpi_arrayvalue arrayvalue_p,
                         PLI_INT32* index_p, PLI_UINT32 num) {}

void vpi_put_value_array(vpiHandle object, p_vpi_arrayvalue arrayvalue_p,
                         PLI_INT32* index_p, PLI_UINT32 num) {}

/* time processing */

void vpi_get_time(vpiHandle object, p_vpi_time time_p) {}

PLI_INT32 vpi_get_data(PLI_INT32 id, PLI_BYTE8* dataLoc, PLI_INT32 numOfBytes) {
  return 0;
}

PLI_INT32 vpi_put_data(PLI_INT32 id, PLI_BYTE8* dataLoc, PLI_INT32 numOfBytes) {
  return 0;
}

void* vpi_get_userdata(vpiHandle obj) { return 0; }

PLI_INT32 vpi_put_userdata(vpiHandle obj, void* userdata) { return 0; }

vpiHandle vpi_handle_by_multi_index(vpiHandle obj, PLI_INT32 num_index,
                                    PLI_INT32* index_array) {
  return 0;
}
