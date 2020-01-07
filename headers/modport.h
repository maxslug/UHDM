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
 * File:   modport.h
 * Author:
 *
 * Created on December 14, 2019, 10:03 PM
 */

#ifndef MODPORT_H
#define MODPORT_H

namespace UHDM {

  class modport : public BaseClass {
  public:
    // Implicit constructor used to initialize all members,
    // comment: modport();
    ~modport() final {}
    
    BaseClass* get_vpiParent() const { return vpiParent_; }

    void set_vpiParent(BaseClass* data) { vpiParent_ = data; }

    unsigned int get_uhdmParentType() const { return uhdmParentType_; }

    void set_uhdmParentType(unsigned int data) { uhdmParentType_ = data; }

    std::string get_vpiFile() const { return SymbolFactory::getSymbol(vpiFile_); }

    void set_vpiFile(std::string data) { vpiFile_ = SymbolFactory::make(data); }

    unsigned int get_vpiLineNo() const { return vpiLineNo_; }

    void set_vpiLineNo(unsigned int data) { vpiLineNo_ = data; }

    std::string get_vpiName() const { return SymbolFactory::getSymbol(vpiName_); }

    void set_vpiName(std::string data) { vpiName_ = SymbolFactory::make(data); }

    VectorOfio_decl* get_io_decls() const { return io_decls_; }

    void set_io_decls(VectorOfio_decl* data) { io_decls_ = data; }

    interface* get_interface() const { return interface_; }

    void set_interface(interface* data) { interface_ = data; }

    virtual unsigned int getUhdmType() { return uhdmmodport; }   
  private:
    
    BaseClass* vpiParent_;

    unsigned int uhdmParentType_;

    unsigned int vpiFile_;

    unsigned int vpiLineNo_;

    unsigned int vpiName_;

    VectorOfio_decl* io_decls_;

    interface* interface_;

  };

  class modportFactory {
  friend Serializer;
  public:
  static modport* make() {
    modport* obj = new modport();
    objects_.push_back(obj);
    return obj;
  }
  private:
    static std::vector<modport*> objects_;
  };
 	      
  class VectorOfmodportFactory {
  friend Serializer;
  public:
  static std::vector<modport*>* make() {
    std::vector<modport*>* obj = new std::vector<modport*>();
    objects_.push_back(obj);
    return obj;
  }
  private:
  static std::vector<std::vector<modport*>*> objects_;
  };

};

#endif

