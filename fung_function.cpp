#include"fung_function.hpp"
#include"fung_context.hpp"
#include"fung_error.hpp"




namespace fung{


Value
Function::
operator()(std::string const&  name, Context&  ctx, std::initializer_list<Value>  args) const
{
  List  buf = args;

  return (*this)(name,ctx,buf);
}


Value
Function::
operator()(std::string const&  name, Context&  ctx, List const&  args) const
{
  ctx.enter(name,*this);

  ctx.entry(parameter_list,args);

    for(auto&  stmt: body)
    {
        if(!stmt.execute(ctx))
        {
          break;
        }
    }


  ctx.leave();

    if(ctx.get_returned_value().get_kind() != return_value_kind)
    {
      throw Error("返り値の型が一致しない");
    }


  return ctx.get_returned_value();
}


void
Function::
print() const
{
  printf("(");

  auto        it = parameter_list.cbegin();
  auto const end = parameter_list.cend();

    if(it != end)
    {
      it++->print();

        while(it != end)
        {
          printf(",");

          it++->print();
        }
    }


  printf(")->%s\n{\n",Value::to_string(return_value_kind).data());

    for(auto&  stmt: body)
    {
      stmt.print();

      printf("\n");
    }


  printf("}\n\n");
}


}




