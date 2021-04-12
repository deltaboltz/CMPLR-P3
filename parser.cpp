#include <iostream>
#include <string>
#include <fstream>
#include "scanner.h"
#include "token.h"
#include "node.h"

using namespace std;

static token t;
static std::istream in(NULL);
static void parseErr(std::string expected);

static node<std::string> Program();
static node<std::string> Block();
static node<std::string> Vars();
static node<std::string> Expr();
static node<std::string> N();
static node<std::string> A();
static node<std::string> M();
static node<std::string> R();
static node<std::string> Stats();
static node<std::string> mStat();
static node<std::string> Stat();
static node<std::string> In();
static node<std::string> Out();
static node<std::string> If();
static node<std::string> Loop();
static node<std::string> Assign();
static node<std::string> R0();
static node<std::string> Label();
static node<std::string> Goto();

node<std::string> parser(std::istream& stream)
{
  in.rdbuf(stream.rdbuf());
  t = scan(in);

  node<std::string> root = Program();

  if(t.id != eoftk)
  {
    parseErr("EOFTK");
  }

  return root;
}

static node<std::string> Program()
{
  node<std::string> root("<program>");

  root.insert(Vars());

  if(t.id == keyword && !t.instance.compare("main"))
  {
    root.insert(t);
    t = scan(in);
    root.insert(Block());
  }

  return root;
}

static node<std::string> Block()
{
  node<std::string> root("<block>");

  if(t.id == keyword && !t.instance.compare("begin"))
  {
    root.insert(t);
    t = scan(in);

    root.insert(Vars());
    root.insert(Stats());

    if(t.id == keyword && !t.instance.compare("end"))
    {
      root.insert(t);
      t = scan(in);
      return root;
    }
    parseErr("kwTK: 'end'");
  }
  parseErr("kwTK: 'begin'");
  return root;
}

static node<std::string> Vars()
{
  node<std::string> root("<vars>");

  if(t.id == keyword && !t.instance.compare("data"))
  {
    root.insert(t);
    t = scan(in);

    if(t.id == identifier)
    {
      root.insert(t);
      t = scan(in);

      if(t.id == opordel && !t.instance.compare(":"))
      {
        root.insert(t);
        t = scan(in);
        if(t.id == opordel && !t.instance.compare("="))
        {
          root.insert(t);
          t = scan(in);

          if(t.id == integer)
          {
            root.insert(t);
            t = scan(in);
            root.insert(Vars());
            if(t.id == opordel && !t.instance.compare(";"))
            {
              root.insert(t);
              t = scan(in);
              return root;
            }
            return root;
          }
          parseErr("numTK");
        }
        parseErr("opTK: '='");
      }
      parseErr("opTK: ':'");
    }
    parseErr("idTK");
  }
  return root;
}

static node<std::string> Expr()
{
  node<std::string> root("<expr>");

  root.insert(N());

  if(t.id == opordel && !t.instance.compare("-"))
  {
    root.insert(t);
    t = scan(in);
    root.insert(Expr());
    return root;
  }

  return root;
}

static node<std::string> N()
{
  node<std::string> root("<N>");

  root.insert(A());

  if(t.id == opordel && !t.instance.compare("/"))
  {
    root.insert(t);
    t = scan(in);
    root.insert(N());
    return root;
  }
  else if(t.id == opordel && !t.instance.compare("*"))
  {
    root.insert(t);
    t = scan(in);
    root.insert(N());
    return root;
  }
  return root;
}

static node<std::string> A()
{
  node<std::string> root("<A>");

  root.insert(M());

  if(t.id == opordel && !t.instance.compare("+"))
  {
    root.insert(t);
    t = scan(in);
    root.insert(A());
    return root;
  }

  return root;
}

static node<std::string> M()
{
  node<std::string> root("<M>");

  if(t.id == opordel && !t.instance.compare("*"))
  {
    root.insert(t);
    t = scan(in);
    root.insert(M());
    return root;
  }
  else
  {
    root.insert(R());
    return root;
  }

  return root;
}

static node<std::string> R()
{
  node<std::string> root("<R>");

  if(t.id == opordel && !t.instance.compare("("))
  {
    root.insert(t);
    t = scan(in);
    root.insert(Expr());

    if(t.id == opordel && !t.instance.compare(")"))
    {
      root.insert(t);
      t = scan(in);
      return root;
    }
    parseErr("opTK: ')'");
  }
  else if(t.id == identifier)
  {
    root.insert(t);
    t = scan(in);
    return root;
  }
  else if(t.id == integer)
  {
    root.insert(t);
    t = scan(in);
    return root;
  }
  parseErr("idTK or numTK");
  return root;
}

static node<std::string> Stats()
{
  node<std::string> root("<stats>");

  root.insert(Stat());
  root.insert(mStat());

  return root;
}

static node<std::string> mStat()
{
  node<std::string> root("<mStat>");

  if(t.id == keyword && !t.instance.compare("stat"))
  {
    root.insert(Stat());
    root.insert(mStat());

    return root;
  }

  return root;
}

static node<std::string> Stat()
{
  node<std::string> root("<stat>");

  if(t.id == keyword && !t.instance.compare("getter"))
  {
    root.insert(In());
    if(t.id == opordel && !t.instance.compare(";"))
    {
      root.insert(t);
      t = scan(in);
      return root;
    }
    return root;
  }
  else if(t.id == keyword && !t.instance.compare("outter"))
  {
    root.insert(Out());
    if(t.id == opordel && !t.instance.compare(";"))
    {
      root.insert(t);
      t = scan(in);
      return root;
    }
    return root;
  }
  else if(t.id == keyword && !t.instance.compare("begin"))
  {
    root.insert(Block());
    return root;
  }
  else if(t.id == keyword && !t.instance.compare("if"))
  {
    root.insert(If());
    if(t.id == opordel && !t.instance.compare(";"))
    {
      root.insert(t);
      t = scan(in);
      return root;
    }
    return root;
  }
  else if(t.id == keyword && !t.instance.compare("loop"))
  {
    root.insert(Loop());
    if(t.id == opordel && !t.instance.compare(";"))
    {
      root.insert(t);
      t = scan(in);
      return root;
    }
    return root;
  }
  else if(t.id == keyword && !t.instance.compare("assign"))
  {
    root.insert(Assign());
    if(t.id == opordel && !t.instance.compare(";"))
    {
      root.insert(t);
      t = scan(in);
      return root;
    }
    return root;
  }
  else if(t.id == keyword && !t.instance.compare("proc"))
  {
    root.insert(Goto());
    if(t.id == opordel && !t.instance.compare(";"))
    {
      root.insert(t);
      t = scan(in);
      return root;
    }
    return root;
  }
  else if(t.id == keyword && !t.instance.compare("label"))
  {
    root.insert(Label());
    if(t.id == opordel && !t.instance.compare(";"))
    {
      root.insert(t);
      t = scan(in);
      return root;
    }
    return root;
  }

  parseErr("kwTK : {in, out, block, if, loop, assign, goto, or label}");
}

static node<std::string> In()
{
  node<std::string> root("<in>");

  if(t.id == keyword && !t.instance.compare("getter"))
  {
    root.insert(t);
    t = scan(in);

    if(t.id == identifier)
    {
      root.insert(t);
      t = scan(in);
      return root;
    }
    parseErr("idTK or opTK");
  }
  parseErr("kwTK: 'getter'");
  return root;
}

static node<std::string> Out()
{
  node<std::string> root("<Out>");

  if(t.id == keyword && !t.instance.compare("outter"))
  {
    root.insert(t);
    t = scan(in);
    root.insert(Expr());
    return root;
  }
  parseErr("kwTK: 'outter'");
  return root;
}

static node<std::string> If()
{
  node<std::string> root("<if>");

  if(t.id == keyword && !t.instance.compare("if"))
  {
    root.insert(t);
    t = scan(in);

    if(t.id == opordel && !t.instance.compare("["))
    {
      root.insert(t);
      t = scan(in);

      root.insert(Expr());
      root.insert(R0());
      root.insert(Expr());

      if(t.id == opordel && !t.instance.compare("]"))
      {
        root.insert(t);
        t = scan(in);
        root.insert(Stat());
        return root;
      }
      parseErr("opTK: ']'");
    }
    parseErr("opTK: '['");
  }
  parseErr("kwTK: 'if'");

  return root;
}

static node<std::string> Loop()
{
  node<std::string> root("<loop>");

  if(t.id == keyword && !t.instance.compare("loop"))
  {
    root.insert(t);
    t = scan(in);

    if(t.id == opordel && !t.instance.compare("["))
    {
      root.insert(t);
      t = scan(in);

      root.insert(Expr());
      root.insert(R0());
      root.insert(Expr());

      if(t.id == opordel && !t.instance.compare("]"))
      {
        root.insert(t);
        t = scan(in);
      }
      root.insert(Stat());
      return root;
      parseErr("opTK: ']'");
    }
    parseErr("opTK: '['");
  }
  parseErr("kwTK: 'loop'");
  return root;
}

static node<std::string> Assign()
{
  node<std::string> root("<assign>");
  if(t.id == keyword && !t.instance.compare("assign"))
  {
    root.insert(t);
    t = scan(in);
    if(t.id == identifier)
    {
      root.insert(t);
      t = scan(in);

      if(t.id == opordel && !t.instance.compare(":"))
      {
        root.insert(t);
        t = scan(in);

        if(t.id == opordel && !t.instance.compare("="))
        {
          root.insert(t);
          t = scan(in);

          root.insert(Expr());
          return root;
        }
      }
      parseErr("opTK: ':='");
    }
    parseErr("idTK");
    return root;
  }
}

static node<std::string> R0()
{
  node<std::string> root("<R0>");

  if(t.id == opordel && !t.instance.compare("=>"))
  {
    root.insert(t);
    t = scan(in);
    return root;
  }

  if(t.id == opordel && !t.instance.compare("=<"))
  {
    root.insert(t);
    t = scan(in);
    return root;
  }
  if(t.id == opordel && !t.instance.compare("=="))
  {
    root.insert(t);
    t = scan(in);
    return root;
  }
  if(t.id == opordel && !t.instance.compare("["))
  {
    root.insert(t);
    t = scan(in);

    if(t.id == opordel && !t.instance.compare("=="))
    {
      root.insert(t);
      t = scan(in);
    }

    if(t.id == opordel && !t.instance.compare("]"))
    {
      root.insert(t);
      t = scan(in);
      return root;
    }
    parseErr("opTK: ']'");
  }
  else if(t.id == opordel && !t.instance.compare("%"))
  {
    root.insert(t);
    t = scan(in);
    return root;
  }
  parseErr("opTK: '=', '<', '>', '%'");
  return root;
}



static node<std::string> Label()
{
  node<std::string> root("<label>");

  if(t.id == keyword && !t.instance.compare("void"))
  {
    root.insert(t);
    t = scan(in);

    if(t.id == identifier)
    {
      root.insert(t);
      t = scan(in);

      return root;
    }
    parseErr("idTK");
  }
  parseErr("kwTK: 'void'");
  return root;
}

static node<std::string> Goto()
{
  node<std::string> root("<goto>");

  if(t.id == keyword && !t.instance.compare("proc"))
  {
    root.insert(t);

    t = scan(in);

    if(t.id == identifier)
    {
      root.insert(t);
      t = scan(in);
      return root;
    }
    parseErr("idTK");
  }
  parseErr("kwTK: 'proc'");
  return root;
}

static void parseErr(std::string expected)
{
  cout << "\n PARSE ERR \n";

  cout << "Line" << t.posString() << "  \"" << t.instance;

  cout << "\" does not match the expected token : " << expected << ".\n";

  exit(-1);
}
