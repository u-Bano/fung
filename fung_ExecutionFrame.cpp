#include"fung_ExecutionFrame.hpp"
#include"fung_context.hpp"




namespace fung{




ExecutionFrame::
ExecutionFrame(std::string const&  fn_name, Function const&  fn, List&&  valls):
function_name(fn_name),
function(&fn),
begin(fn.get_body().cbegin()),
current(fn.get_body().cbegin()),
end(fn.get_body().cend())
{
  auto&  parals = fn.get_parameter_list();

    if(parals.size() != valls.length())
    {
      throw Error("%s 引数の数が一致しない",function_name.data());
    }


  auto  it = valls.begin();

    for(auto&  para: parals)
    {
        if(para.get_value_kind() != it->get_kind())
        {
          auto&  a = Value::to_string(para.get_value_kind());
          auto&  b = Value::to_string(       it->get_kind());

          throw Error("%s %s <- %s 引数の型が一致しない",fn_name.data(),a.data(),b.data());
        }


      argument_list.emplace_back(para.get_name(),std::move(*it++));
    }
}


ExecutionFrame::
ExecutionFrame(StatementList const&  stmtls):
function_name(""),
function(nullptr),
begin(stmtls.cbegin()),
current(stmtls.cbegin()),
end(stmtls.cend())
{
}




void
ExecutionFrame::
update_argument_list(List&&  valls)
{
  auto  it = valls.begin();

    for(auto&  v: argument_list)
    {
      v.value = std::move(*it++);
    }
}


void
ExecutionFrame::
append_variable(Variable&&  var)
{
   for(auto&  v: variable_list)
   {
       if(v.name == var.name)
       {
         throw Error("すでに%sという名前の変数がある",var.name.data());
       }
   }


  variable_list.emplace_back(std::move(var));
}


Variable const*
ExecutionFrame::
find_variable(std::string const&  name) const
{
    for(auto&  var: variable_list)
    {
        if(var.name == name)
        {
          return &var;
        }
    }


    for(auto&  var: argument_list)
    {
        if(var.name == name)
        {
          return &var;
        }
    }


  return nullptr;
}


ExecutionResult
ExecutionFrame::
step(Context&  ctx, Value&  retval_buf)
{
    if(current == end)
    {
      return ExecutionResult::error;
    }


  auto&  stmt = *current++;

    switch(stmt.get_kind())
    {
  case(StatementKind::null):
      break;
  case(StatementKind::interrupt):
      return ExecutionResult::interrupted;
      break;
  case(StatementKind::print):
      printf("[print stmt] ");

      stmt.get_expression()->print();

      printf(" -> ");

      stmt.get_expression()->evaluate(ctx).print();

      printf("\n");
      break;
  case(StatementKind::let):
      ctx.entry(stmt.get_identifier(),stmt.get_expression());
      break;
  case(StatementKind::return_):
      retval_buf = stmt.get_expression()->evaluate(ctx);
      return ExecutionResult::value_was_returned;
      break;
    }


  return ExecutionResult::no_value_was_returned;
}


}




