
// DO NOT EDIT THIS FILE - it is machine generated -*- c++ -*-

#ifndef __gnu_java_security_PolicyFile$1__
#define __gnu_java_security_PolicyFile$1__

#pragma interface

#include <java/lang/Object.h>
extern "Java"
{
  namespace gnu
  {
    namespace java
    {
      namespace security
      {
          class PolicyFile;
          class PolicyFile$1;
      }
    }
  }
}

class gnu::java::security::PolicyFile$1 : public ::java::lang::Object
{

public: // actually package-private
  PolicyFile$1(::gnu::java::security::PolicyFile *, ::java::util::List *);
public:
  ::java::lang::Object * run();
public: // actually package-private
  ::gnu::java::security::PolicyFile * __attribute__((aligned(__alignof__( ::java::lang::Object)))) this$0;
private:
  ::java::util::List * val$policyFiles;
public:
  static ::java::lang::Class class$;
};

#endif // __gnu_java_security_PolicyFile$1__
