/*
 Do not modify, auto-generated by model_gen.tcl

 Copyright 2019-2020 Alain Dargelas

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
 * File:   vpi_visitor.cpp
 * Author:
 *
 * Created on December 14, 2019, 10:03 PM
 */

#include <string.h>

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "include/sv_vpi_user.h"
#include "include/vhpi_user.h"

#include "headers/uhdm_types.h"
#include "headers/containers.h"
#include "headers/vpi_uhdm.h"
#include "headers/uhdm.h"
#include "headers/Serializer.h"

using namespace UHDM;

std::string visit_object (vpiHandle obj_h, unsigned int indent) {
  std::string result;
  unsigned int subobject_indent = indent + 2;
  std::string spaces;
  for (unsigned int i = 0; i < indent; i++) spaces += " ";
  std::string objectName = "";
  std::string fileName = "";
  std::string lineNo   = "";
  std::string parent   = "";
  if (unsigned int l = vpi_get(vpiLineNo, obj_h)) {
    lineNo = ", line:" + std::to_string(l);
  }
  unsigned int objType = vpi_get(vpiType, obj_h);
  if (objType == vpiModule || objType == vpiProgram || objType == vpiClassDefn || objType == vpiPackage ||
      objType == vpiInterface || objType == vpiUdp) {
    if (const char* s = vpi_get_str(vpiFile, obj_h))
      fileName = ", file:" +  std::string(s);
  }
  if (vpiHandle par = vpi_handle(vpiParent, obj_h)) {
    if (const char* parentName = vpi_get_str(vpiName, par)) {
      parent = ", parent:" + std::string(parentName);
    }
  }
  if (const char* s = vpi_get_str(vpiName, obj_h)) {
    objectName = s;
  }
  result += spaces + UHDM::VpiTypeName(obj_h) + ": " + objectName + fileName + lineNo + parent + "\n";
  unsigned int objectType = vpi_get(vpiType, obj_h);				     
<OBJECT_VISITORS>
  return result;
}

std::string visit_designs (std::vector<vpiHandle> designs) {
  std::string result;
  for (auto design : designs) {
    result += visit_object(design, 0);
  }
  return result;
}

