#ifndef FUNG_Value_HPP_INCLUDED
#define FUNG_Value_HPP_INCLUDED


#include"fung_ValueKind.hpp"
#include"fung_pointer.hpp"
#include<vector>
#include<string>


namespace fung{



class Function;
class Value;


struct
TailCalling
{
  List&&  list;

  TailCalling(List&&  ls): list(std::move(ls)){}

};


union
ValueData
{
  int     integer;
  bool    boolean;
  char    character;
  String   string;

  Value*  value;

  List  list;

  Function const*  function;

  Pointer  pointer;

   ValueData(){}
  ~ValueData(){}
};


class
Value
{
  ValueKind  kind=ValueKind::null;
  ValueData  data;

public:
   Value(){}
   explicit Value(ValueKind  k): kind(k){}
   explicit Value(int  i);
   explicit Value(bool  b);
   explicit Value(char  c);
   explicit Value(std::string const&  s);
   explicit Value(String&&  s);
   explicit Value(Value*  v);
   explicit Value(TailCalling&&  tc);
   explicit Value(List&&  ls);
   explicit Value(Function const&  fn);
   explicit Value(Pointer&&  ptr);
   Value(Value const&  rhs) noexcept{*this = rhs;}
   Value(Value&&       rhs) noexcept{*this = std::move(rhs);}
  ~Value(){clear();}

  Value&  operator=(Value const&  rhs) noexcept;
  Value&  operator=(Value&&       rhs) noexcept;

  ValueData*        operator->()      {return &data;}
  ValueData const*  operator->() const{return &data;}

  bool  operator==(ValueKind   k) const{return kind == k;}
  bool  operator!=(ValueKind   k) const{return kind != k;}

  bool  is_undefined() const{return kind == ValueKind::undefined;}

  operator bool() const{return kind != ValueKind::null;}

  void  clear();

  ValueKind  get_kind() const{return kind;}

  Value  convert_to(ValueKind  k) const;

  Value  convert_to_boolean() const;
  Value  convert_to_string() const;
  Value  convert_to_integer() const;

  Value  get_property(std::string const&  name) const;

  void  print() const;

  static ValueKind  to_kind(std::string const&  s);
  static std::string const&  to_string(ValueKind  k);

  static Value  add(    Value const&  lhs, Value const&  rhs);
  static Value  sub(    Value const&  lhs, Value const&  rhs);
  static Value  mul(    Value const&  lhs, Value const&  rhs);
  static Value  div(    Value const&  lhs, Value const&  rhs);
  static Value  rem(    Value const&  lhs, Value const&  rhs);
  static Value  shl(    Value const&  lhs, Value const&  rhs);
  static Value  shr(    Value const&  lhs, Value const&  rhs);
  static Value  bit_or( Value const&  lhs, Value const&  rhs);
  static Value  bit_and(Value const&  lhs, Value const&  rhs);
  static Value  bit_xor(Value const&  lhs, Value const&  rhs);
  static Value  eq(     Value const&  lhs, Value const&  rhs);
  static Value  neq(    Value const&  lhs, Value const&  rhs);
  static Value  lt(     Value const&  lhs, Value const&  rhs);
  static Value  lteq(   Value const&  lhs, Value const&  rhs);
  static Value  gt(     Value const&  lhs, Value const&  rhs);
  static Value  gteq(   Value const&  lhs, Value const&  rhs);

  static Value  sus(Value const&  v, size_t   i);

  static Value  log_not(Value const&  lhs);
  static Value  bit_not(Value const&  lhs);
  static Value      neg(Value const&  lhs);
  static Value      der(Value const&  lhs);

};


extern Value  const undefined;


}




#endif




