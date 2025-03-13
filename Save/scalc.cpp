
#include <windows.h>

#include "pch.h"
#include <cstdint>
#include <ctime>  // 
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <float.h>
#include <errno.h>
#include <limits>

#include "scalc.h"
//#include "sfmts.h"
#include "sfunc.h"

#pragma warning(disable : 4996)
#pragma warning(disable : 4244)

#define _WIN_
#define INT_FORMAT      "ll"

calculator::calculator(int cfg)
{
  errpos = 0;
  tmp_var_count = 0;
  err[0] = '\0';
  scfg = cfg;
  sres[0] = '\0';
  memset(hash_table, 0, sizeof hash_table);
  memset(v_stack, 0, sizeof v_stack);
  //randomize();

  add(tsFFUNC1, "abs", (void*)(float__t(*)(float__t))fabsl);
  //add(tsIFFUNC3, "aeq", (void*)(int_t(*)(float__t, float__t, int_t))nearly_equal);
  add(tsFFUNC1, "erf", (void*)(float__t(*)(float__t))Erf);
  add(tsFFUNC1, "acos", (void*)(float__t(*)(float__t))Acos);
  add(tsFFUNC1, "asin", (void*)(float__t(*)(float__t))Asin);
  add(tsFFUNC1, "atan", (void*)(float__t(*)(float__t))Atan);
  add(tsFFUNC1, "arctg", (void*)(float__t(*)(float__t))Atan);
  add(tsFFUNC2, "atan2", (void*)(float__t(*)(float__t,float__t))Atan2l);
  add(tsFFUNC2, "hypot", (void*)(float__t(*)(float__t,float__t))Hypot);
  add(tsFFUNC1, "cos", (void*)(float__t(*)(float__t))Cos);
  add(tsFFUNC1, "cosh", (void*)(float__t(*)(float__t))Cosh);
  add(tsFFUNC1, "ch", (void*)(float__t(*)(float__t))Cosh);
  add(tsFFUNC1, "exp", (void*)(float__t(*)(float__t))Exp);
  add(tsFFUNC1, "log", (void*)(float__t(*)(float__t))Log);
  add(tsFFUNC1, "log10", (void*)(float__t(*)(float__t))Lg);
  add(tsFFUNC1, "np", (void*)(float__t(*)(float__t))NP);
  add(tsFFUNC1, "db", (void*)(float__t(*)(float__t))DB);
  add(tsFFUNC1, "anp", (void*)(float__t(*)(float__t))ANP);
  add(tsFFUNC1, "adb", (void*)(float__t(*)(float__t))ADB);
  add(tsFFUNC1, "sin", (void*)(float__t(*)(float__t))Sin);
  add(tsFFUNC1, "sinh", (void*)(float__t(*)(float__t))Sinh);
  add(tsFFUNC1, "sh", (void*)(float__t(*)(float__t))Sinh);
  add(tsFFUNC1, "tan", (void*)(float__t(*)(float__t))Tan);
  add(tsFFUNC1, "tanh", (void*)(float__t(*)(float__t))Tanh);
  add(tsFFUNC1, "th", (void*)(float__t(*)(float__t))Tanh);
  add(tsFFUNC1, "sqrt", (void*)(float__t(*)(float__t))Sqrt);
  add(tsFFUNC1, "ctanh", (void*)(float__t(*)(float__t))Ctanh);
  add(tsFFUNC1, "cth", (void*)(float__t(*)(float__t))Ctanh);
  add(tsFFUNC1, "arsh", (void*)(float__t(*)(float__t))Arsh);
  add(tsFFUNC1, "arch", (void*)(float__t(*)(float__t))Arch);
  add(tsFFUNC1, "arth", (void*)(float__t(*)(float__t))Arth);
  add(tsFFUNC1, "arcth", (void*)(float__t(*)(float__t))Arcth);
  add(tsFFUNC1, "float", (void*)To_float);
  add(tsIFUNC1, "int", (void*)To_int);
  add(tsIFUNC2, "gcd", (void*)(int_t(*)(int_t,int_t))Gcd);
  add(tsIFUNC2, "invmod", (void*)(int_t(*)(int_t,int_t))Invmod);
  add(tsIFUNC1, "prime", (void*)Prime);
  add(tsPFUNCn, "fprn", (void*)(int_t(*)(char*, char*, int args, value*))fprn);
  add(tsPFUNCn, "prn", (void*)(int_t(*)(char*, char*, int args, value*))fprn);
  add(tsPFUNCn, "printf", (void*)(int_t(*)(char*, char*, int args, value*))fprn);
  add(tsSIFUNC1, "datatime", (void*)datatime);
  add(tsFFUNC1, "ln", (void*)(float__t(*)(float__t))Log);
  add(tsFFUNC1, "lg", (void*)(float__t(*)(float__t))Lg);
  add(tsFFUNC1, "exp10", (void*)(float__t(*)(float__t))Exp10);
  add(tsFFUNC1, "arcsin", (void*)(float__t(*)(float__t))Asin);
  add(tsFFUNC1, "arccos", (void*)(float__t(*)(float__t))Acos);
  add(tsFFUNC1, "sing", (void*)(float__t(*)(float__t))Sing);
  add(tsFFUNC1, "cosg", (void*)(float__t(*)(float__t))Cosg);
  add(tsFFUNC1, "tg", (void*)(float__t(*)(float__t))Tan);
  add(tsFFUNC1, "root2", (void*)(float__t(*)(float__t))Sqrt);
  add(tsFFUNC2, "pow", (void*)(float__t(*)(float__t,float__t))Pow);
  add(tsFFUNC1, "tgg", (void*)(float__t(*)(float__t))Tgg);
  add(tsFFUNC1, "ctg", (void*)(float__t(*)(float__t))Ctg);
  add(tsFFUNC1, "ctgg", (void*)(float__t(*)(float__t))Ctgg);
  add(tsFFUNC1, "frac", (void*)(float__t(*)(float__t))Frac);
  add(tsFFUNC1, "round", (void*)(float__t(*)(float__t))Round);
  add(tsIFUNC1, "not", (void*)Not);
  add(tsIFUNC1, "now", (void*)Now);
  add(tsFFUNC2, "min", (void*)(float__t(*)(float__t,float__t))Min);
  add(tsFFUNC2, "max", (void*)(float__t(*)(float__t,float__t))Max);
  add(tsFFUNC1, "log2", (void*)(float__t(*)(float__t))Log2);
  add(tsFFUNC1, "fact", (void*)(float__t(*)(float__t))Factorial);
  add(tsFFUNC2, "logn", (void*)(float__t(*)(float__t,float__t))Logn);
  add(tsFFUNC1, "root3", (void*)(float__t(*)(float__t))Root3);
  add(tsFFUNC1, "cbrt", (void*)(float__t(*)(float__t))Root3);
  add(tsFFUNC2, "rootn", (void*)(float__t(*)(float__t,float__t))Rootn);
  add(tsFFUNC1, "swg", (void*)(float__t(*)(float__t))Swg);
  add(tsFFUNC1, "sswg", (void*)(float__t(*)(float__t))SSwg);
  add(tsFFUNC1, "aswg", (void*)(float__t(*)(float__t))Aswg);
  add(tsFFUNC1, "awg", (void*)(float__t(*)(float__t))Awg);
  add(tsFFUNC1, "sawg", (void*)(float__t(*)(float__t))SAwg);
  add(tsFFUNC1, "aawg", (void*)(float__t(*)(float__t))Aawg);
  add(tsFFUNC1, "cs", (void*)(float__t(*)(float__t))Cs);
  add(tsFFUNC1, "acs", (void*)(float__t(*)(float__t))Acs);
  add(tsFFUNC1, "rnd", (void*)(float__t(*)(float__t))Random);
  add(tsFFUNC3, "vout", (void*)(float__t(*)(float__t, float__t, float__t))Vout);
  add(tsFFUNC3, "cmp", (void*)(float__t(*)(float__t, float__t, float__t))Cmp);


  add(tsFFUNC2, "ee", (void*)(float__t(*)(float__t,float__t))Ee);
#define M_PI		3.14159265358979323846
#define M_E		2.7182818284590452354
  addfvar("pi", M_PI);
  addfvar("e", M_E);
  addivar("max32",    0x7fffffff);
  addivar("maxint",   0x7fffffff);
  addivar("maxu32",   0xffffffff);
  addivar("maxuint",  0xffffffff);
  addivar("max64",    0x7fffffffffffffffull);
  addivar("maxlong",  0x7fffffffffffffffull);
  addivar("maxu64",   0xffffffffffffffffull);
  addivar("maxulong", 0xffffffffffffffffull);
  addivar("timezone", -_timezone/3600);
  addivar("daylight", _daylight);
  addivar("tz", _daylight-_timezone/3600);
}

calculator::~calculator(void)
{
 symbol* sp;
 symbol* nsp;

 for(int i = 0; i < hash_table_size; i++)
  {
   if ((sp = hash_table[i]) != NULL)
    {
     do
      {
       nsp = sp->next;
       free(sp->name);
       delete sp;
       sp = nsp;
       hash_table[i] = NULL;
      }
     while (nsp);
    }
  }
}

void calculator::varlist(void (*f)(char*, float__t))
{
 symbol* sp;
 for (int i = 0; i < hash_table_size; i++)
  {
   if ((sp = hash_table[i]) != NULL)
    {
     do
      {
       if ((f) && (sp->tag == tsVARIABLE))
        {
         f(sp->name, (float__t)sp->val.get());
        }
       sp = sp->next;
      }
     while (sp);
    }
  }
}

void calculator::varlist(void (*f)(char*, value*))
{
  symbol* sp;
  for (int i = 0; i < hash_table_size; i++)
    {
      if ((sp = hash_table[i]) != NULL)
        {
          do
            {
              if ((f) && (sp->tag == tsVARIABLE))
               {
                f(sp->name, &sp->val);
               }
              sp = sp->next;
            }
          while (sp);
        }
    }
}

unsigned calculator::string_hash_function(char* p)
{
  unsigned h = 0, g;
  while (*p)
    {
      if (scfg & UPCASE) h = (h << 4) + toupper(*p++);
      else h = (h << 4) + *p++;

      if ((g = h & 0xF0000000) != 0)
        {
          h ^= g >> 24;
        }
      h &= ~g;
    }
  return h;
}

symbol* calculator::add(t_symbol tag, const char* name, void* func)
{
  char *uname = strdup(name);

  unsigned h = string_hash_function(uname) % hash_table_size;
  symbol* sp;
  for (sp = hash_table[h]; sp != NULL; sp = sp->next)
    {
      if (scfg & UPCASE)
       {
#ifdef _WIN_
        if (stricmp(sp->name, uname) == 0) return sp;
#else  /*_WIN_*/
	if (strcasecmp(sp->name, uname) == 0) return sp;
#endif /*_WIN_*/
       }
      else
       {
        if (strcmp(sp->name, uname) == 0) return sp;
       }
    }
  sp = new symbol;
  sp->tag = tag;
  sp->func = func;
  sp->name = uname;
  sp->val.tag = tvINT;
  sp->val.ival = 0;
  sp->next = hash_table[h];
  hash_table[h] = sp;
  return sp;
}

void calculator::addfvar(const char* name, float__t val)
{
 symbol* sp = add(tsVARIABLE, name);
 sp->val.tag = tvFLOAT;
 sp->val.fval = val;
}

bool calculator::checkvar(const char* name)
{
  char *uname = strdup(name);

  unsigned h = string_hash_function(uname) % hash_table_size;
  symbol* sp;
  for (sp = hash_table[h]; sp != NULL; sp = sp->next)
    {
      if (scfg & UPCASE)
       {
#ifdef _WIN_
        if (stricmp(sp->name, uname) == 0) return true;
#else  /*_WIN_*/
	if (strcasecmp(sp->name, uname) == 0) return true;
#endif /*_WIN_*/
       }
      else
       {
        if (strcmp(sp->name, uname) == 0) return true;
       }
    }
  return false;
}

void calculator::addivar(const char* name, int_t val)
{
 symbol* sp = add(tsVARIABLE, name);
 sp->val.tag = tvINT;
 sp->val.ival = val;
}

int calculator::hscanf(char* str, int_t &ival, int &nn)
{
 int_t res = 0;
 char c;
 int n = 0;
 while (c = *str++, c && (n < 16))
  {
   if ((c >= '0') && (c <= '9')) {res = res * 16 + (c - '0'); n++;}
   else
   if ((c >= 'A') && (c <= 'F')) {res = res * 16 + (c - 'A') + 0xA; n++;}
   else
   if ((c >= 'a') && (c <= 'f')) {res = res * 16 + (c - 'a') + 0xa; n++;}
   else break;
  }
 ival = res;
 nn = n;
 if (n) scfg |= HEX;
 return 0;
}

int calculator::bscanf(char* str, int_t &ival, int &nn)
{
 int_t res = 0;
 char c;
 int n = 0;

 while (c = *str++, c && (n < 64))
  {
   if ((c >= '0') && (c <= '1')) {res = res * 2 + (c - '0'); n++;}
   else break;
  }
 ival = res;
 nn = n;
 if (n) scfg |= fBIN;
 return 0;
}

int calculator::oscanf(char* str, int_t &ival, int &nn)
{
 int_t res = 0;
 char c;
 int n = 0;

 while (c = *str++, c && (n < 24))
  {
   if ((c >= '0') && (c <= '7')) {res = res * 8 + (c - '0'); n++;}
   else break;
  }
 ival = res;
 nn = n;
 if (n) scfg |= OCT;
 return 0;
}

int calculator::xscanf(char* str, int len, int_t &ival, int &nn)
{
 int_t res = 0;
 char c;
 int n=0;
 int hmax, omax;
 int max;

 switch (len)
  {
   case 1:
    max = 0x100;
    hmax = 3;
    omax = 3;
   break;
   case 2:
    max = 0x10000;
    hmax = 5;
    omax = 6;
   break;
   default: max = 0;
  }
 switch (*str)
  {
    case '0':  case '1':  case '2': case '3':
     {
      while (c = *str++, c && (n < omax))
      {
       if ((c >= '0') && (c <= '7')) {res = res * 8 + (c - '0'); n++;}
       else break;
      }
     if (res >= max) n--;
     if (n) scfg |= OCT;
     }
    break;

    case 'x':
    case 'X':
     str++; n++;
     while (c = *str++, c && (res < max) && (n < hmax))
      {
       if ((c >= '0') && (c <= '9')) {res = res * 16 + (c - '0'); n++;}
       else
       if ((c >= 'A') && (c <= 'F')) {res = res * 16 + (c - 'A') + 0xA; n++;}
       else
       if ((c >= 'a') && (c <= 'f')) {res = res * 16 + (c - 'a') + 0xa; n++;}
       else break;
      }
     if (res >= max) n--;
     if (n) scfg |= HEX;
    break;

    case 'a':
     res = '\007'; n = 1;
     scfg |= ESC;
    break;

    case 'f':
     res = 255u; n = 1;
     scfg |= ESC;
    break;

    case 'v':
     res = '\x0b'; n = 1;
     scfg |= ESC;
    break;

    case 'E':  case 'e':
     res = '\033'; n = 1;
     scfg |= ESC;
    break;

    case 't':
     res = '\t'; n = 1;
     scfg |= ESC;
    break;

    case 'n':
     res = '\n'; n = 1;
     scfg |= ESC;
    break;

    case 'r':
     res = '\r'; n = 1;
     scfg |= ESC;
    break;

    case 'b':
     res = '\b'; n = 1;
     scfg |= ESC;
    break;

    case '\\':
     res = '\\'; n = 1;
     scfg |= ESC;
    break;
  }
 ival = res;
 nn = n;
 return 0;
}


float__t calculator::dstrtod(char *s, char **endptr)
{
 const char cdeg[] = {'`', '\'', '\"'}; //` - degrees, ' - minutes, " - seconds
 const float__t mdeg[] = {M_PI/180.0, M_PI/(180.0*60), M_PI/(180.0*60*60)};
 float__t res = 0;
 float__t d;
 char* end = s;

 for(int i = 0; i < 3; i++)
  {
   d = strtod(end, &end);
   do
    {
     if (*end == cdeg[i])
      {
       res += d * mdeg[i];
       end++;
       scfg |= DEG;
       break;
      }
     else i++;
    }
   while (i < 3);
  }
 *endptr = end;
 return res;
}

//1:c1:y1:d1:h1:m1:s  => 189377247661s
float__t calculator::tstrtod(char *s, char **endptr)
{
 const float__t dms[] =
   {(60.0*60.0*60.0*24.0*365.25*100.0),(60.0*60.0*24.0*365.25),
    (60.0*60.0*24.0), (60.0*60.0), 60.0, 1.0};
 const char cdt[] =  {'c', 'y', 'd', 'h', 'm', 's'};
 float__t res = 0;
 float__t d;
 char* end = s;

 for(int i = 0; i < 6; i++)
  {
   d = strtod(end, &end);
   do
    {
     if ((*end == ':') && (*(end+1) == cdt[i]))
      {
       res += d * dms[i];
       end += 2;
       scfg |= DAT;
       break;
      }
     else i++;
    }
   while (i < 6);
  }
 *endptr = end;
 return res;
}

// http://searchstorage.techtarget.com/sDefinition/0,,sid5_gci499008,00.html
// process expression like 1k56 => 1.56k (maximum 3 digits)
void calculator::engineering(float__t mul, char * &fpos, float__t &fval)
{
 int fract = 0;
 int div = 1;
 int n = 3; //maximum 3 digits
 while(*fpos && (*fpos >= '0') && ((*fpos <= '9')) && n--)
  {
   div *= 10;
   fract *= 10;
   fract += *fpos++-'0';
   scfg |= ENG;
  }
 fval *= mul;
 fval += (fract*mul)/div;
 scfg |= SCF;
}

void calculator::scientific(char * &fpos, float__t &fval)
 {
  if (*(fpos-1) == 'E') fpos--;
  switch (*fpos)
    {
     case '\"': //Inch
      if (scfg & FRI)
       {
        fpos++;
        //fval *= 25.4e-3;
        engineering(25.4e-3, fpos, fval);
       }
     break;
     case 'Y':
       fpos++;
       if (*fpos == 'B')
        {
         fpos++;
         fval *= 1.20892582e+24;  //2**80
         scfg |= CMP;
        }
       else engineering(1e24, fpos, fval);
       break;
     case 'Z':
       fpos++;
       if (*fpos == 'B')
        {
         fpos++;
	 fval *= 1.180591620717411e+21;  //2**70
         scfg |= CMP;
        }
       else engineering(1e21, fpos, fval);
       break;
     case 'E':
       fpos++;
       if (*fpos == 'B')
        {
         fpos++;
         fval *= 1152921504606846976ull; //2**60
         scfg |= CMP;
        }
       else engineering(1e18, fpos, fval);
       break;
     case 'P':
       fpos++;
       if (*fpos == 'B')
        {
         fpos++;
         fval *= 1125899906842624ull; //2**50
         scfg |= CMP;
        }
       else engineering(1e15, fpos, fval);
       break;
     case 'T':
       fpos++;
       if (*fpos == 'B')
        {
         fpos++;
         fval *= 1099511627776ull; //2**40
         scfg |= CMP;
        }
       else engineering(1e12, fpos, fval);
       break;
     case 'G':
       fpos++;
       if (*fpos == 'B')
        {
         fpos++;
         fval *= 1073741824ull;  //2**30
         scfg |= CMP;
        }
       else engineering(1e9, fpos, fval);
       break;
     case 'M':
       fpos++;
       if (*fpos == 'B')
        {
         fpos++;
         fval *= 1048576;  //2**20
         scfg |= CMP;
        }
       else engineering(1e6, fpos, fval);
       break;
     case 'K':
       fpos++;
       if (*fpos == 'B')
        {
         fpos++;
         fval *= 1024;  //2**10
         scfg |= CMP;
        }
       else engineering(1e3, fpos, fval);
       break;
     case 'R':
       fpos++;
       engineering(1, fpos, fval);
       break;
     case 'h':
       fpos++;
       engineering(1e2, fpos, fval);
       break;
     case 'k':
       fpos++;
       engineering(1e3, fpos, fval);
       break;
     case 'D':
       fpos++;
       engineering(1e1, fpos, fval);
       break;
     case 'd':
       fpos++;
       if (*fpos == 'a')
        {
         fpos++;
         engineering(1e1, fpos, fval);
        }
       else engineering(1e-1, fpos, fval);
       break;
     case 'c':
       fpos++;
       engineering(1e-2, fpos, fval);
       break;
     case 'm':
       fpos++;
       engineering(1e-3, fpos, fval);
       break;
     case 'u':
       fpos++;
       engineering(1e-6, fpos, fval);
       break;
     case 'n':
       fpos++;
       engineering(1e-9, fpos, fval);
       break;
     case 'p':
       fpos++;
       engineering(1e-12, fpos, fval);
       break;
     case 'f':
       fpos++;
       engineering(1e-15, fpos, fval);
       break;
     case 'a':
       fpos++;
       engineering(1e-18, fpos, fval);
       break;
     case 'z':
       fpos++;
       engineering(1e-21, fpos, fval);
       break;
     case 'y':
       fpos++;
       engineering(1e-24, fpos, fval);
       break;
    }
 }

void calculator::error(int pos, const char* msg)
{
  sprintf(err, "Error: %s at %i\n\n", msg, pos);
  errpos = pos;
}

t_operator calculator::scan(bool operand, bool percent)
{
  char name[max_expression_length], *np;

  while (isspace(buf[pos])) pos += 1;
  switch (buf[pos++])
    {
    case '\0':
      return toEND;
    case '(':
      return toLPAR;
    case ')':
      return toRPAR;
    case '+':
      if (buf[pos] == '+')
        {
          pos += 1;
          return operand ? toPREINC : toPOSTINC;
        }
      else
      if (buf[pos] == '=')
        {
          pos += 1;
          return toSETADD;
        }
      return operand ? toPLUS : toADD;
    case '-':
      if (buf[pos] == '-')
        {
          pos += 1;
          return operand ? toPREDEC : toPOSTDEC;
        }
      else
      if (buf[pos] == '=')
        {
          pos += 1;
          return toSETSUB;
        }
      return operand ? toMINUS : toSUB;
    case '!':
      if (buf[pos] == '=')
        {
          pos += 1;
          return toNE;
        }
      return operand ? toNOT : toFACT;
    case '~':
      return toCOM;
    case ';':
      return toSEMI;
    case '*':
      if (buf[pos] == '*')
        {
          if (buf[pos+1] == '=')
            {
              pos += 2;
              return toSETPOW;
            }
          pos += 1;
          return toPOW;
        }
      else
      if (buf[pos] == '=')
        {
          pos += 1;
          return toSETMUL;
        }
      return toMUL;
    case '/':
      if (buf[pos] == '=')
        {
          pos += 1;
          return toSETDIV;
        }
      else
      if (buf[pos] == '/')
        {
          pos += 1;
          return toPAR;
        }
      return toDIV;
    case '%':
      if (buf[pos] == '=')
        {
          pos += 1;
          return toSETMOD;
        }
      else
      if (buf[pos] == '%')
        {
          pos += 1;
          return toPERCENT;
        }
      return toMOD;
    case '<':
      if (buf[pos] == '<')
        {
          if (buf[pos+1] == '=')
            {
              pos += 2;
              return toSETASL;
            }
          else
            {
              pos += 1;
              return toASL;
            }
        }
      else
      if (buf[pos] == '=')
        {
          pos += 1;
          return toLE;
        }
      else
      if (buf[pos] == '>')
        {
          pos += 1;
          return toNE;
        }
      return toLT;
    case '>':
      if (buf[pos] == '>')
        {
          if (buf[pos+1] == '>')
            {
              if (buf[pos+2] == '=')
                {
                  pos += 3;
                  return toSETLSR;
                }
              pos += 2;
              return toLSR;
            }
          else
          if (buf[pos+1] == '=')
            {
              pos += 2;
              return toSETASR;
            }
          else
            {
              pos += 1;
              return toASR;
            }
        }
      else
      if (buf[pos] == '=')
        {
          pos += 1;
          return toGE;
        }
      return toGT;
    case '=':
      if (buf[pos] == '=')
        {
          scfg &= ~PAS;
          pos += 1;
          return toEQ;
        }
      if (scfg & PAS) return toEQ;
      else return toSET;
    case ':':
      if (buf[pos] == '=')
        {
          scfg |= PAS;
          pos += 1;
          return toSET;
        }
      error("syntax error");
      return toERROR;
    case '&':
      if (buf[pos] == '&')
        {
          pos += 1;
          return toAND;
        }
      else
      if (buf[pos] == '=')
        {
          pos += 1;
          return toSETAND;
        }
      return toAND;
    case '|':
      if (buf[pos] == '|')
        {
          pos += 1;
          return toOR;
        }
      else
      if (buf[pos] == '=')
        {
          pos += 1;
          return toSETOR;
        }
      return toOR;
    case '^':
      if (scfg & PAS)
       {
        if (buf[pos] == '=')
          {
            pos += 1;
            return toSETPOW;
          }
        return toPOW;
       }
      else
       {
        if (buf[pos] == '=')
          {
            pos += 1;
            return toSETXOR;
          }
        return toXOR;
       }
    case '#':
     if (operand)
      {
       float__t fval;
       char *fpos;
       if (buf[pos])
        {
         fval = Awg(strtod(buf+pos, &fpos));
         v_stack[v_sp].tag = tvFLOAT;
         v_stack[v_sp].fval = fval;
         pos = fpos - buf;
         v_stack[v_sp].pos = pos;
         v_stack[v_sp++].var = NULL;
         return toOPERAND;
        }
       else
        {
         error("bad numeric constant");
         return toERROR;
        }
      }
     else
      {
       if (buf[pos] == '=')
         {
           pos += 1;
           return toSETXOR;
         }
       return toXOR;
      }
    case ',':
      return toCOMMA;
    case '\'':
     {
      int_t ival;
      char* ipos;// , * fpos;
      //wchar_t wc;
      int n = 0;

      if (buf[pos] == '\\')
       {
        xscanf(buf+pos+1, 1, ival, n);
        ipos = buf+pos+n+1;
        if (*ipos == '\'') ipos++;
        else
         {
           error("bad char constant");
           return toERROR;
         }
       }
      else
       {
         ipos = buf+pos+1;
         if (*ipos == '\'')
          {
#ifdef _WCHAR_
#ifdef _WIN_
           if (*(ipos+1) == 'W')
            {
             wchar_t wbuf[2];
             char cbuf[2];

             cbuf[0] = *(ipos-1);
             cbuf[1] = '\0';

             MultiByteToWideChar(CP_OEMCP, 0, (LPSTR)cbuf, -1,
                    (LPWSTR)wbuf, 2);
             ival = *(int*)&wbuf[0];
             ipos+=2;
             scfg |= WCH;
            }
           else
#endif  /*_WIN_*/
#endif /*_WCHAR_*/
            {
             scfg |= CHR;
             ival = *(unsigned char *)(ipos-1);
             v_stack[v_sp].sval = (char *) malloc(STRBUF);
             v_stack[v_sp].sval[0] = *(ipos-1);
             v_stack[v_sp].sval[1] = '\0';
             ipos++;
            }
          }
         else
          {
           char sbuf[STRBUF];
           int sidx = 0;
           ipos = buf+pos;
           while (*ipos && (*ipos != '\'') && (sidx < STRBUF))
            sbuf[sidx++] = *ipos++;
           sbuf[sidx] = '\0';
           if (*ipos == '\'')
            {
             if (sbuf[0]) scfg |= STR;
             v_stack[v_sp].tag = tvSTR;
             v_stack[v_sp].ival = 0;
             v_stack[v_sp].sval = (char *)malloc(STRBUF);
             strcpy(v_stack[v_sp].sval, sbuf);
             pos = ipos - buf+1;
             v_stack[v_sp].pos = pos;
             v_stack[v_sp++].var = NULL;
             return toOPERAND;
            }
           else
            {
             error("bad char constant");
             return toERROR;
            }
          }
       }
      v_stack[v_sp].tag = tvINT;
      v_stack[v_sp].ival = ival;
      pos = ipos - buf;
      v_stack[v_sp].pos = pos;
      v_stack[v_sp++].var = NULL;
      return toOPERAND;
     }
#ifdef _WCHAR_
#ifdef _WIN_
    case 'L':
     {
      int_t ival;
      char* ipos;// , * fpos;
      //wchar_t wc;
      int n = 0;

      if (buf[pos] == '\'')
       {
        if (buf[pos+1] == '\\')
         {
          xscanf(buf+pos+2, 2, ival, n);
          ipos = buf+pos+n+2;
          if (*ipos == '\'') ipos++;
          else
           {
            error("bad char constant");
            return toERROR;
           }
         }
        else
         {
           ipos = buf+pos;
           if (*(ipos+2) == '\'')
            {
             wchar_t wbuf[2];
             char cbuf[2];

             cbuf[0] = *(ipos+1);
             cbuf[1] = '\0';

             MultiByteToWideChar(CP_OEMCP, 0, (LPSTR)cbuf, -1,
                    (LPWSTR)wbuf, 2);
             ival = *(int*)&wbuf[0];
             ipos+=3;
             scfg |= WCH;
            }
           else
            {
             error("bad char constant");
             return toERROR;
            }
         }
        v_stack[v_sp].tag = tvINT;
        v_stack[v_sp].ival = ival;
        pos = ipos - buf;
        v_stack[v_sp].pos = pos;
        v_stack[v_sp++].var = NULL;
        return toOPERAND;
       }
      goto def;
     }
#endif /*_WIN_*/
#endif /*_WCHAR_*/
    case '"':
     {
       char* ipos;// , * fpos;
       char sbuf[STRBUF];
       int sidx = 0;
       ipos = buf+pos;
       while (*ipos && (*ipos != '"') && (sidx < STRBUF))
        sbuf[sidx++] = *ipos++;
       sbuf[sidx] = '\0';
       if (*ipos == '"')
        {
         if (sbuf[0]) scfg |= STR;
         v_stack[v_sp].tag = tvSTR;
         v_stack[v_sp].ival = 0;
         v_stack[v_sp].sval = (char *)malloc(STRBUF);
         strcpy(v_stack[v_sp].sval, sbuf);
         pos = ipos - buf+1;
         v_stack[v_sp].pos = pos;
         v_stack[v_sp++].var = NULL;
         return toOPERAND;
        }
       else
        {
         error("bad char constant");
         return toERROR;
        }
     }
    case '.': case '0': case '1': case '2': case '3': case '4': case '5':
    case '6': case '7': case '8': case '9': case '\\': case '$':
     {
      int_t ival;
      float__t fval;
      int ierr = 0, ferr;
      char *ipos, *fpos;
      int n = 0;

      if (buf[pos-1] == '\\')
       {
        ierr = xscanf(buf+pos, 1, ival, n);
        ipos = buf+pos+n;
       }
      else
      if ((buf[pos-1] == '0') && ((buf[pos] == 'B') || (buf[pos] == 'b')))
       {
        ierr = bscanf(buf+pos+1, ival, n);
        ipos = buf+pos+n+1;
       }
      else
      if ((buf[pos-1] == '0') && ((buf[pos] == 'O') || (buf[pos] == 'o')))
       {
        ierr = oscanf(buf+pos+1, ival, n);
        ipos = buf+pos+n+1;
       }
      else
      if (buf[pos-1] == '$')
       {
        ierr = hscanf(buf+pos, ival, n);
        ipos = buf+pos+n;
       }
      else
      if (buf[pos-1] == '0')
       {
        ierr = sscanf(buf+pos-1, "%" INT_FORMAT "i%n", &ival, &n) != 1;
        ipos = buf+pos-1+n;
        if ((ierr==0)&&((buf[pos] == 'x')||(buf[pos] == 'X'))) scfg |= HEX;
       }
      else
       {
        if (scfg & FFLOAT) {ierr = 0; n = 0;}
        else ierr = sscanf(buf+pos-1, "%" INT_FORMAT "i%n", &ival, &n) != 1;
        ipos = buf+pos-1+n;
       }
      errno = 0;
      fval = strtod(buf+pos-1, &fpos);

      //` - degrees, ' - minutes, " - seconds
      if ((*fpos == '\'') || (*fpos == '`') || (((scfg & FRI)==0)&&(*fpos == '\"')))
        fval = dstrtod(buf+pos-1, &fpos);
      else
      if (*fpos == ':') fval = tstrtod(buf+pos-1, &fpos);
      else
      if (scfg & SCI+FRI) scientific(fpos, fval);
      ferr = errno;
      if (ierr && ferr)
       {
        error("bad numeric constant");
        return toERROR;
       }
      if (v_sp == max_stack_size)
       {                                
        error("stack overflow");
        return toERROR;
       }                              
      if (!ierr && ipos >= fpos)
       {
        v_stack[v_sp].tag = tvINT;
        v_stack[v_sp].ival = ival;
        pos = ipos - buf;
       }
      else
       {
        if (operand && percent && (*fpos == '%'))
         {
          fpos++;
          v_stack[v_sp].tag = tvPERCENT;
         }
        else v_stack[v_sp].tag = tvFLOAT;
        v_stack[v_sp].fval = fval;
        pos = fpos - buf;
       }
      v_stack[v_sp].pos = pos;
      v_stack[v_sp++].var = NULL;
      return toOPERAND;
     }
    default:
    def:
      pos -= 1;
      np = name;
      while (isalnum(buf[pos]) || buf[pos] == '@' ||
             buf[pos] == '_' || buf[pos] == '?')
        {
          *np++ = buf[pos++];
        }
      if (np == buf)
        {
          error("Bad character");
          return toERROR;
        }
      *np = '\0';
      if (buf[pos+1]=='\0') return toEND;
      symbol* sym = add(tsVARIABLE, name);
      if (v_sp == max_stack_size)
        {
          error("stack overflow");
          return toERROR;
        }
      v_stack[v_sp] = sym->val;
      v_stack[v_sp].pos = pos;
      v_stack[v_sp++].var = sym;
      return (sym->tag == tsVARIABLE) ? toOPERAND : toFUNC;
    }
}

static int lpr[toTERMINALS] =
{
  2, 0, 0, 0,       // BEGIN, OPERAND, ERROR, END,
  4, 4,             // LPAR, RPAR
  5, 98, 98, 98,    // FUNC, POSTINC, POSTDEC, FACT
  98, 98, 98, 98, 98, 98, // PREINC, PREDEC, PLUS, MINUS, NOT, COM,
  90,               // POW,
  80, 80, 80, 80, 80,   // toPERCENT, MUL, DIV, MOD, PAR
  70, 70,           // ADD, SUB,
  60, 60, 60,       // ASL, ASR, LSR,
  50, 50, 50, 50,   // GT, GE, LT, LE,
  40, 40,           // EQ, NE,
  38,               // AND,
  36,               // XOR,
  34,               // OR,
  20, 20, 20, 20, 20, 20, 20, //SET, SETADD, SETSUB, SETMUL, SETDIV, SETMOD,
  20, 20, 20, 20, 20, 20, // SETASL, SETASR, SETLSR, SETAND, SETXOR, SETOR,
  8,               // SEMI
  10               // COMMA
};

static int rpr[toTERMINALS] =
{
  0, 0, 0, 1,       // BEGIN, OPERAND, ERROR, END,
  110, 3,           // LPAR, RPAR
  120, 99, 99, 99,  // FUNC, POSTINC, POSTDEC, FACT
  99, 99, 99, 99, 99, 99, // PREINC, PREDEC, PLUS, MINUS, NOT, COM,
  95,               // POW,
  80, 80, 80, 80, 80,   // toPERCENT, MUL, DIV, MOD, PAR
  70, 70,           // ADD, SUB,
  60, 60, 60,       // ASL, ASR, LSR,
  50, 50, 50, 50,   // GT, GE, LT, LE,
  40, 40,           // EQ, NE,
  38,               // AND,
  36,               // XOR,
  34,               // OR,
  25, 25, 25, 25, 25, 25, 25, //SET, SETADD, SETSUB, SETMUL, SETDIV, SETMOD,
  25, 25, 25, 25, 25, 25, // SETASL, SETASR, SETLSR, SETAND, SETXOR, SETOR,
  10,              // SEMI
  15               // COMMA
};


bool calculator::assign()
{
 value& v = v_stack[v_sp-1];
 if (v.var == NULL)
  {
   error(v.pos, "variable expected");
   return false;
  }
 else
  {
   v.var->val = v;
   return true;
  }
}

float__t calculator::evaluate(char* expression, __int64 * piVal)
{
  char var_name[16];
  bool operand = true;
  bool percent = false;
  int n_args = 0;
  const __int64 i64maxdbl = 0x7feffffffffffffeull;
  const __int64 i64mindbl = 0x0010000000000001ull;
  const double maxdbl = *(double*)&i64maxdbl;
  const double mindbl = *(double*)&i64mindbl;
  //const float__t qnan = 0.0/0.0;
  const float__t qnan = std::numeric_limits<float__t>::quiet_NaN();

  buf = expression;
  v_sp = 0;
  o_sp = 0;
  pos = 0;
  err[0] = '\0';
  o_stack[o_sp++] = toBEGIN;

  memset(sres, 0, STRBUF);
  while (true)
    {
     next_token:
      int op_pos = pos;
      int oper = scan(operand, percent);
      if (oper == toERROR)
       {
        return qnan;
       }
      switch(oper)
       {
        case toMUL:
        case toDIV:
        case toMOD:
        case toPOW:
        case toPAR:
        case toADD:
        case toSUB:
        case toCOMMA:
        //case toPERCENT:
         percent = true;
        break;
        default:
         percent = false;
       }
      if (!operand)
       {
        if (!BINARY(oper) && oper != toEND && oper != toPOSTINC
            && oper != toPOSTDEC && oper != toRPAR && oper != toFACT)
         {
          error(op_pos, "operator expected");
          return qnan;
         }
        if (oper != toPOSTINC && oper != toPOSTDEC && oper != toRPAR
            && oper != toFACT)
         {
          operand = true;
         }
       }
      else
       {
        if (oper == toOPERAND)
         {
          operand = false;
          n_args += 1;
          continue;
         }
        if (BINARY(oper) || oper == toRPAR)
         {
          error(op_pos, "operand expected");
          return qnan;
         }
       }
      n_args = 1;
      while (lpr[o_stack[o_sp-1]] >= rpr[oper])
        {
          int cop = o_stack[--o_sp];
          if (BINARY(cop) && (v_sp < 2))
           {
            error("Unexpected end of expression");
            return qnan;
           }

          switch (cop)
           {
            case toBEGIN:
              if (oper == toRPAR)
                {
                  error("Unmatched ')'");
                  return qnan;
                }
              if (oper != toEND) error("Unexpected end of input");
              if (v_sp == 1)
                {
                  if (scfg & UTMP)
                   {
                    sprintf(var_name, "@%d", ++tmp_var_count);
                    add(tsVARIABLE, var_name)->val = v_stack[0];
                   }
                  if (v_stack[0].tag == tvINT)
                    {
                     if (piVal) *piVal = v_stack[0].ival;
                     if (v_stack[0].sval)
                      {
                       strcpy(sres, v_stack[0].sval);
                       free(v_stack[0].sval);
                       v_stack[0].sval = NULL;
                      }
                     else sres[0] = '\0';
                     return v_stack[0].ival;
                    }
                  else
                    {
                     if (piVal) *piVal = (__int64)v_stack[0].fval;
                     if (v_stack[0].sval)
                      {
                       strcpy(sres, v_stack[0].sval);
                       free(v_stack[0].sval);
                       v_stack[0].sval = NULL;
                      }
                     else sres[0] = '\0';
                     return v_stack[0].fval;
                    }
                }
              else
              if (v_sp != 0) error("Unexpected end of expression");
              return qnan;

            case toCOMMA:
              n_args += 1;
              continue;

            case toSEMI:
              if ((v_stack[v_sp-1].tag == tvINT) &&
                  (v_stack[v_sp-2].tag == tvINT))
                {
                 v_stack[v_sp-2].ival = v_stack[v_sp-1].ival;
                }
              else
              if ((v_stack[v_sp-1].tag == tvSTR) &&
                  (v_stack[v_sp-2].tag == tvSTR))
                {
                 strcpy(v_stack[v_sp-2].sval, v_stack[v_sp-1].sval);
                }
              else
              if ((v_stack[v_sp-1].tag == tvSTR) ||
                  (v_stack[v_sp-2].tag == tvSTR))
                {
                 error(v_stack[v_sp-2].pos, "Illegal string operation");
                 return qnan;
                }
              else
                {
                 v_stack[v_sp-2].fval = v_stack[v_sp-1].get();
                 v_stack[v_sp-2].tag = tvFLOAT;
                }
              v_sp -= 1;
              v_stack[v_sp-1].var = NULL;
              break;

            case toADD:
            case toSETADD:
              if ((v_stack[v_sp-1].tag == tvINT) &&
                  (v_stack[v_sp-2].tag == tvINT))
                {
                 v_stack[v_sp-2].ival += v_stack[v_sp-1].ival;
                }
              else
              if ((v_stack[v_sp-1].tag == tvSTR) &&
                  (v_stack[v_sp-2].tag == tvSTR))
                {
                 strcat(v_stack[v_sp-2].sval, v_stack[v_sp-1].sval);
                }
              else
              if ((v_stack[v_sp-1].tag == tvSTR) ||
                  (v_stack[v_sp-2].tag == tvSTR))
                {
                 error(v_stack[v_sp-2].pos, "Illegal string operation");
                 return qnan;
                }
              else
                {
                 if (v_stack[v_sp-1].tag == tvPERCENT)
                  {
                   float__t left = v_stack[v_sp-2].get();
                   float__t right = v_stack[v_sp-1].get();
                   v_stack[v_sp-2].fval = left+(left*right/100.0);
                  }
                 else v_stack[v_sp-2].fval =
                    v_stack[v_sp-2].get() + v_stack[v_sp-1].get();

                 v_stack[v_sp-2].tag = tvFLOAT;
                }
              v_sp -= 1;
              if (cop == toSETADD)
               {
                if (!assign()) return qnan;
               }
              v_stack[v_sp-1].var = NULL;
              break;

            case toSUB:
            case toSETSUB:
              if ((v_stack[v_sp-1].tag == tvSTR) ||
                  (v_stack[v_sp-2].tag == tvSTR))
               {
                error(v_stack[v_sp-2].pos, "Illegal string operation");
                return qnan;
               }
              else
              if ((v_stack[v_sp-1].tag == tvINT) &&
                  (v_stack[v_sp-2].tag == tvINT))
               {
                v_stack[v_sp-2].ival -= v_stack[v_sp-1].ival;
               }
              else
               {
                if (v_stack[v_sp-1].tag == tvPERCENT)
                 {
                  float__t left = v_stack[v_sp-2].get();
                  float__t right = v_stack[v_sp-1].get();
                  v_stack[v_sp-2].fval = left-(left*right/100.0);
                 }
                else v_stack[v_sp-2].fval =
                   v_stack[v_sp-2].get() - v_stack[v_sp-1].get();
                v_stack[v_sp-2].tag = tvFLOAT;
               }
              v_sp -= 1;
              if (cop == toSETSUB)
               {
                if (!assign()) return qnan;
               }
              v_stack[v_sp-1].var = NULL;
              break;

            case toMUL:
            case toSETMUL:
              if ((v_stack[v_sp-1].tag == tvINT) &&
                  (v_stack[v_sp-2].tag == tvINT))
               {
                v_stack[v_sp-2].ival *= v_stack[v_sp-1].ival;
               }
              else
              if ((v_stack[v_sp-1].tag == tvSTR) ||
                  (v_stack[v_sp-2].tag == tvSTR))
               {
                error(v_stack[v_sp-2].pos, "Illegal string operation");
                return qnan;
               }
              else
               {
                if (v_stack[v_sp-1].tag == tvPERCENT)
                 {
                  float__t left = v_stack[v_sp-2].get();
                  float__t right = v_stack[v_sp-1].get();
                  v_stack[v_sp-2].fval = left*(left*right/100.0);
                 }
                else v_stack[v_sp-2].fval =
                   v_stack[v_sp-2].get() * v_stack[v_sp-1].get();
                v_stack[v_sp-2].tag = tvFLOAT;
               }
              v_sp -= 1;
              if (cop == toSETMUL)
               {
                if (!assign()) return qnan;
               }
              v_stack[v_sp-1].var = NULL;
              break;

            case toDIV:
            case toSETDIV:
              if ((v_stack[v_sp-1].tag == tvSTR) ||
                  (v_stack[v_sp-2].tag == tvSTR))
               {
                error(v_stack[v_sp-2].pos, "Illegal string operation");
                return qnan;
               }
              else
              if (v_stack[v_sp-1].get() == 0.0)
               {
                error(v_stack[v_sp-2].pos, "Division by zero");
                return qnan;
               }
              if (v_stack[v_sp-1].tag == tvINT && v_stack[v_sp-2].tag == tvINT)
               {
                v_stack[v_sp-2].ival /= v_stack[v_sp-1].ival;
               }
              else
               {
                if (v_stack[v_sp-1].tag == tvPERCENT)
                 {
                  float__t left = v_stack[v_sp-2].get();
                  float__t right = v_stack[v_sp-1].get();
                  v_stack[v_sp-2].fval = left/(left*right/100.0);
                 }
                else v_stack[v_sp-2].fval =
                   v_stack[v_sp-2].get() / v_stack[v_sp-1].get();
                v_stack[v_sp-2].tag = tvFLOAT;
               }
             v_sp -= 1;
             if (cop == toSETDIV)
              {
               if (!assign()) return qnan;
              }
             v_stack[v_sp-1].var = NULL;
            break;

            case toPAR:
             if ((v_stack[v_sp-1].tag == tvSTR) ||
                 (v_stack[v_sp-2].tag == tvSTR))
              {
               error(v_stack[v_sp-2].pos, "Illegal string operation");
               return qnan;
              }
             else
             if ((v_stack[v_sp-1].get() == 0.0) ||
                 (v_stack[v_sp-2].get() == 0.0))
              {
               error(v_stack[v_sp-2].pos, "Division by zero");
               return qnan;
              }
             if (v_stack[v_sp-1].tag == tvPERCENT)
              {
               float__t left = v_stack[v_sp-2].get();
               float__t right = v_stack[v_sp-1].get();
               v_stack[v_sp-2].fval = 1/(1/left+1/(left*right/100.0));
              }
             else v_stack[v_sp-2].fval = 1/(1/v_stack[v_sp-1].get()+1/v_stack[v_sp-2].get());
             v_stack[v_sp-2].tag = tvFLOAT;
             v_sp -= 1;
             v_stack[v_sp-1].var = NULL;
            break;

            case toPERCENT:
             if ((v_stack[v_sp-1].tag == tvSTR) ||
                 (v_stack[v_sp-2].tag == tvSTR))
               {
                 error(v_stack[v_sp-2].pos, "Illegal string operation");
                 return qnan;
               }
             else
             if ((v_stack[v_sp-1].get() == 0.0) ||
                 (v_stack[v_sp-2].get() == 0.0))
              {
               error(v_stack[v_sp-2].pos, "Division by zero");
               return qnan;
              }
             if (v_stack[v_sp-1].tag == tvPERCENT)
              {
               float__t left = v_stack[v_sp-2].get();
               float__t right = v_stack[v_sp-1].get();
               right = left*right/100.0;
               v_stack[v_sp-2].fval = 100.0*(left-right)/right;
              }
             else
              {
               float__t left = v_stack[v_sp-2].get();
               float__t right = v_stack[v_sp-1].get();
               v_stack[v_sp-2].fval = 100.0*(left-right)/right;
              }
             v_stack[v_sp-2].tag = tvFLOAT;
             v_sp -= 1;
             v_stack[v_sp-1].var = NULL;
            break;

            case toMOD:
            case toSETMOD:
              if ((v_stack[v_sp-1].tag == tvSTR) ||
                  (v_stack[v_sp-2].tag == tvSTR))
               {
                error(v_stack[v_sp-2].pos, "Illegal string operation");
                return qnan;
               }
              else
              if (v_stack[v_sp-1].get() == 0.0)
               {
                error(v_stack[v_sp-2].pos, "Division by zero");
                return qnan;
               }
              if (v_stack[v_sp-1].tag == tvINT && v_stack[v_sp-2].tag == tvINT)
               {
                v_stack[v_sp-2].ival %= v_stack[v_sp-1].ival;
               }
              else
               {
                if (v_stack[v_sp-1].tag == tvPERCENT)
                 {
                  float__t left = v_stack[v_sp-2].get();
                  float__t right = v_stack[v_sp-1].get();
                  v_stack[v_sp-2].fval = fmod(left, left*right/100.0);
                 }
                else v_stack[v_sp-2].fval =
                    fmod(v_stack[v_sp-2].get(), v_stack[v_sp-1].get());
                v_stack[v_sp-2].tag = tvFLOAT;
               }
              v_sp -= 1;
              if (cop == toSETMOD)
               {
                if (!assign()) return qnan;
               }
              v_stack[v_sp-1].var = NULL;
              break;

            case toPOW:
            case toSETPOW:
              if ((v_stack[v_sp-1].tag == tvSTR) ||
                  (v_stack[v_sp-2].tag == tvSTR))
                {
                  error(v_stack[v_sp-2].pos, "Illegal string operation");
                  return qnan;
                }
              else
              if (v_stack[v_sp-1].tag == tvINT && v_stack[v_sp-2].tag == tvINT)
               {
                v_stack[v_sp-2].ival =
                  (int_t)pow((float__t)v_stack[v_sp-2].ival,
                             (float__t)v_stack[v_sp-1].ival);
               }
              else
               {
                if (v_stack[v_sp-1].tag == tvPERCENT)
                 {
                  float__t left = v_stack[v_sp-2].get();
                  float__t right = v_stack[v_sp-1].get();
                  v_stack[v_sp-2].fval = pow(left, left*right/100.0);
                 }
                else v_stack[v_sp-2].fval =
                   pow(v_stack[v_sp-2].get(), v_stack[v_sp-1].get());
                 v_stack[v_sp-2].tag = tvFLOAT;
               }
              v_sp -= 1;
              if (cop == toSETPOW)
               {
                if (!assign()) return qnan;
               }
              v_stack[v_sp-1].var = NULL;
              break;

            case toAND:
            case toSETAND:
              if ((v_stack[v_sp-1].tag == tvSTR) ||
                  (v_stack[v_sp-2].tag == tvSTR))
               {
                error(v_stack[v_sp-2].pos, "Illegal string operation");
                return qnan;
               }
              else
              if (v_stack[v_sp-1].tag == tvINT && v_stack[v_sp-2].tag == tvINT)
               {
                v_stack[v_sp-2].ival &= v_stack[v_sp-1].ival;
               }
              else
               {
                v_stack[v_sp-2].ival =
                  v_stack[v_sp-2].get_int() & v_stack[v_sp-1].get_int();
                v_stack[v_sp-2].tag = tvINT;
               }
              v_sp -= 1;
              if (cop == toSETAND)
               {
                if (!assign()) return qnan;
               }
              v_stack[v_sp-1].var = NULL;
              break;

            case toOR:
            case toSETOR:
              if ((v_stack[v_sp-1].tag == tvSTR) ||
                  (v_stack[v_sp-2].tag == tvSTR))
                {
                  error(v_stack[v_sp-2].pos, "Illegal string operation");
                  return qnan;
                }
              else
              if (v_stack[v_sp-1].tag == tvINT && v_stack[v_sp-2].tag == tvINT)
                {
                  v_stack[v_sp-2].ival |= v_stack[v_sp-1].ival;
                }
              else
                {
                  v_stack[v_sp-2].ival =
                    v_stack[v_sp-2].get_int() | v_stack[v_sp-1].get_int();
                  v_stack[v_sp-2].tag = tvINT;
                }
              v_sp -= 1;
              if (cop == toSETOR)
                {
                  if (!assign()) return qnan;
                }
              v_stack[v_sp-1].var = NULL;
              break;

            case toXOR:
            case toSETXOR:
              if ((v_stack[v_sp-1].tag == tvSTR) ||
                  (v_stack[v_sp-2].tag == tvSTR))
                {
                  error(v_stack[v_sp-2].pos, "Illegal string operation");
                  return qnan;
                }
              else
              if (v_stack[v_sp-1].tag == tvINT && v_stack[v_sp-2].tag == tvINT)
                {
                  v_stack[v_sp-2].ival ^= v_stack[v_sp-1].ival;
                }
              else
                {
                  v_stack[v_sp-2].ival =
                    v_stack[v_sp-2].get_int() ^ v_stack[v_sp-1].get_int();
                  v_stack[v_sp-2].tag = tvINT;
                }
              v_sp -= 1;
              if (cop == toSETXOR)
                {
                  if (!assign()) return qnan;
                }
              v_stack[v_sp-1].var = NULL;
              break;

            case toASL:
            case toSETASL:
              if ((v_stack[v_sp-1].tag == tvSTR) ||
                  (v_stack[v_sp-2].tag == tvSTR))
                {
                  error(v_stack[v_sp-2].pos, "Illegal string operation");
                  return qnan;
                }
              else
              if (v_stack[v_sp-1].tag == tvINT && v_stack[v_sp-2].tag == tvINT)
                {
                  v_stack[v_sp-2].ival <<= v_stack[v_sp-1].ival;
                }
              else
                {
                  v_stack[v_sp-2].ival =
                    v_stack[v_sp-2].get_int() << v_stack[v_sp-1].get_int();
                  v_stack[v_sp-2].tag = tvINT;
                }
              v_sp -= 1;
              if (cop == toSETASL)
                {
                  if (!assign()) return qnan;
                }
              v_stack[v_sp-1].var = NULL;
              break;

            case toASR:
            case toSETASR:
              if ((v_stack[v_sp-1].tag == tvSTR) ||
                  (v_stack[v_sp-2].tag == tvSTR))
                {
                  error(v_stack[v_sp-2].pos, "Illegal string operation");
                  return qnan;
                }
              else
              if (v_stack[v_sp-1].tag == tvINT && v_stack[v_sp-2].tag == tvINT)
                {
                  v_stack[v_sp-2].ival >>= v_stack[v_sp-1].ival;
                }
              else
                {
                  v_stack[v_sp-2].ival =
                    v_stack[v_sp-2].get_int() >> v_stack[v_sp-1].get_int();
                  v_stack[v_sp-2].tag = tvINT;
                }
              v_sp -= 1;
              if (cop == toSETASR)
                {
                  if (!assign()) return qnan;
                }
              v_stack[v_sp-1].var = NULL;
              break;

            case toLSR:
            case toSETLSR:
              if ((v_stack[v_sp-1].tag == tvSTR) ||
                  (v_stack[v_sp-2].tag == tvSTR))
                {
                  error(v_stack[v_sp-2].pos, "Illegal string operation");
                  return qnan;
                }
              else
              if (v_stack[v_sp-1].tag == tvINT && v_stack[v_sp-2].tag == tvINT)
                {
                  v_stack[v_sp-2].ival =
                    (unsigned_t)v_stack[v_sp-2].ival >> v_stack[v_sp-1].ival;
                }
              else
                {
                  v_stack[v_sp-2].ival = (unsigned_t)v_stack[v_sp-2].get_int()
                                         >> v_stack[v_sp-1].get_int();
                  v_stack[v_sp-2].tag = tvINT;
                }
              v_sp -= 1;
              if (cop == toSETLSR)
                {
                  if (!assign()) return qnan;
                }
              v_stack[v_sp-1].var = NULL;
              break;

            case toEQ:
              if (v_stack[v_sp-1].tag == tvINT && v_stack[v_sp-2].tag == tvINT)
                {
                  v_stack[v_sp-2].ival =
                    v_stack[v_sp-2].ival == v_stack[v_sp-1].ival;
                }
              else
              if (v_stack[v_sp-1].tag == tvSTR && v_stack[v_sp-2].tag == tvSTR)
                {
                  v_stack[v_sp-2].ival = (strcmp(v_stack[v_sp-2].sval, v_stack[v_sp-1].sval) == 0);
                  v_stack[v_sp-2].tag = tvINT;
                  free(v_stack[v_sp-2].sval);
                  v_stack[v_sp-2].sval = NULL;
                }
              else
                {
                  v_stack[v_sp-2].ival =
                    v_stack[v_sp-2].get() == v_stack[v_sp-1].get();
                  v_stack[v_sp-2].tag = tvINT;
                }
              v_sp -= 1;
              v_stack[v_sp-1].var = NULL;
              break;

            case toNE:
              if (v_stack[v_sp-1].tag == tvINT && v_stack[v_sp-2].tag == tvINT)
                {
                  v_stack[v_sp-2].ival =
                    v_stack[v_sp-2].ival != v_stack[v_sp-1].ival;
                }
              else
              if (v_stack[v_sp-1].tag == tvSTR && v_stack[v_sp-2].tag == tvSTR)
                {
                  v_stack[v_sp-2].ival = (strcmp(v_stack[v_sp-2].sval, v_stack[v_sp-1].sval) != 0);
                  v_stack[v_sp-2].tag = tvINT;
                  free(v_stack[v_sp-2].sval);
                  v_stack[v_sp-2].sval = NULL;
                }
              else
                {
                  v_stack[v_sp-2].ival =
                    v_stack[v_sp-2].get() != v_stack[v_sp-1].get();
                  v_stack[v_sp-2].tag = tvINT;
                }
              v_sp -= 1;
              v_stack[v_sp-1].var = NULL;
              break;

            case toGT:
              if (v_stack[v_sp-1].tag == tvINT && v_stack[v_sp-2].tag == tvINT)
                {
                  v_stack[v_sp-2].ival =
                    v_stack[v_sp-2].ival > v_stack[v_sp-1].ival;
                }
              else
              if (v_stack[v_sp-1].tag == tvSTR && v_stack[v_sp-2].tag == tvSTR)
                {
                  v_stack[v_sp-2].ival = (strcmp(v_stack[v_sp-2].sval, v_stack[v_sp-1].sval) > 0);
                  v_stack[v_sp-2].tag = tvINT;
                  free(v_stack[v_sp-2].sval);
                  v_stack[v_sp-2].sval = NULL;
                }
              else
                {
                  v_stack[v_sp-2].ival =
                      v_stack[v_sp-2].get() > v_stack[v_sp-1].get();
                  v_stack[v_sp-2].tag = tvINT;
                }
              v_sp -= 1;
              v_stack[v_sp-1].var = NULL;
              break;

            case toGE:
              if (v_stack[v_sp-1].tag == tvINT && v_stack[v_sp-2].tag == tvINT)
                {
                  v_stack[v_sp-2].ival =
                    v_stack[v_sp-2].ival >= v_stack[v_sp-1].ival;
                }
              else
              if (v_stack[v_sp-1].tag == tvSTR && v_stack[v_sp-2].tag == tvSTR)
                {
                  v_stack[v_sp-2].ival = (strcmp(v_stack[v_sp-2].sval, v_stack[v_sp-1].sval) >= 0);
                  v_stack[v_sp-2].tag = tvINT;
                  free(v_stack[v_sp-2].sval);
                  v_stack[v_sp-2].sval = NULL;
                }
              else
                {
                  v_stack[v_sp-2].ival =
                    v_stack[v_sp-2].get() >= v_stack[v_sp-1].get();
                  v_stack[v_sp-2].tag = tvINT;
                }
              v_sp -= 1;
              v_stack[v_sp-1].var = NULL;
              break;

            case toLT:
              if (v_stack[v_sp-1].tag == tvINT && v_stack[v_sp-2].tag == tvINT)
                {
                  v_stack[v_sp-2].ival =
                    v_stack[v_sp-2].ival < v_stack[v_sp-1].ival;
                }
              else
              if (v_stack[v_sp-1].tag == tvSTR && v_stack[v_sp-2].tag == tvSTR)
                {
                  v_stack[v_sp-2].ival = (strcmp(v_stack[v_sp-2].sval, v_stack[v_sp-1].sval) < 0);
                  v_stack[v_sp-2].tag = tvINT;
                  free(v_stack[v_sp-2].sval);
                  v_stack[v_sp-2].sval = NULL;
                }
              else
                {
                  v_stack[v_sp-2].ival =
                    v_stack[v_sp-2].get() < v_stack[v_sp-1].get();
                  v_stack[v_sp-2].tag = tvINT;
                }
              v_sp -= 1;
              v_stack[v_sp-1].var = NULL;
              break;

            case toLE:
              if (v_stack[v_sp-1].tag == tvINT && v_stack[v_sp-2].tag == tvINT)
                {
                  v_stack[v_sp-2].ival =
                    v_stack[v_sp-2].ival <= v_stack[v_sp-1].ival;
                }
              else
              if (v_stack[v_sp-1].tag == tvSTR && v_stack[v_sp-2].tag == tvSTR)
                {
                  v_stack[v_sp-2].ival = (strcmp(v_stack[v_sp-2].sval, v_stack[v_sp-1].sval) <= 0);
                  v_stack[v_sp-2].tag = tvINT;
                  free(v_stack[v_sp-2].sval);
                  v_stack[v_sp-2].sval = NULL;
                }
              else
                {
                  v_stack[v_sp-2].ival =
                    v_stack[v_sp-2].get() <= v_stack[v_sp-1].get();
                  v_stack[v_sp-2].tag = tvINT;
                }
              v_sp -= 1;
              v_stack[v_sp-1].var = NULL;
              break;

            case toPREINC:
              if ((v_stack[v_sp-1].tag == tvSTR) ||
                  (v_stack[v_sp-2].tag == tvSTR))
                {
                  error(v_stack[v_sp-2].pos, "Illegal string operation");
                  return qnan;
                }
              else
              if (v_stack[v_sp-1].tag == tvINT)
                {
                  v_stack[v_sp-1].ival += 1;
                }
              else
                {
                  v_stack[v_sp-1].fval += 1;
                }
              if (!assign()) return qnan;
              v_stack[v_sp-1].var = NULL;
              break;

            case toPREDEC:
              if ((v_stack[v_sp-1].tag == tvSTR) ||
                  (v_stack[v_sp-2].tag == tvSTR))
                {
                  error(v_stack[v_sp-2].pos, "Illegal string operation");
                  return qnan;
                }
              else
              if (v_stack[v_sp-1].tag == tvINT)
                {
                  v_stack[v_sp-1].ival -= 1;
                }
              else
                {
                  v_stack[v_sp-1].fval -= 1;
                }
              if (!assign()) return qnan;
              v_stack[v_sp-1].var = NULL;
              break;

            case toPOSTINC:
              if ((v_stack[v_sp-1].tag == tvSTR) ||
                  (v_stack[v_sp-2].tag == tvSTR))
                {
                  error(v_stack[v_sp-2].pos, "Illegal string operation");
                  return qnan;
                }
              else
              if (v_stack[v_sp-1].var == NULL)
                {
                  error(v_stack[v_sp-1].pos, "Varaibale expected");
                  return qnan;
                }
              if (v_stack[v_sp-1].var->val.tag == tvINT)
                {
                  v_stack[v_sp-1].var->val.ival += 1;
                }
              else
                {
                  v_stack[v_sp-1].var->val.fval += 1;
                }
              v_stack[v_sp-1].var = NULL;
              break;

            case toPOSTDEC:
              if ((v_stack[v_sp-1].tag == tvSTR) ||
                  (v_stack[v_sp-2].tag == tvSTR))
                {
                  error(v_stack[v_sp-2].pos, "Illegal string operation");
                  return qnan;
                }
              else
              if (v_stack[v_sp-1].var == NULL)
                {
                  error(v_stack[v_sp-1].pos, "Varaibale expected");
                  return qnan;
                }
              if (v_stack[v_sp-1].var->val.tag == tvINT)
                {
                  v_stack[v_sp-1].var->val.ival -= 1;
                }
              else
                {
                  v_stack[v_sp-1].var->val.fval -= 1;
                }
              v_stack[v_sp-1].var = NULL;
              break;

            case toFACT:
              if ((v_stack[v_sp-1].tag == tvSTR) ||
                  (v_stack[v_sp-2].tag == tvSTR))
                {
                  error(v_stack[v_sp-2].pos, "Illegal string operation");
                  return qnan;
                }
              else
              if (v_stack[v_sp-1].tag == tvINT)
                {
                  v_stack[v_sp-1].ival = (int_t)Factorial((float__t)v_stack[v_sp-1].ival);
                }
              else
                {
                  v_stack[v_sp-1].fval = (float__t)Factorial((float__t)v_stack[v_sp-1].fval);
                }
              v_stack[v_sp-1].var = NULL;

            break;

            case toSET:
              if ((v_sp < 2) || (v_stack[v_sp-2].var == NULL))
                {
                  if (v_sp < 2) error("Variabale expected");
                  else error(v_stack[v_sp-2].pos, "Variabale expected");
                  return qnan;
                }
              else
                {
                  v_stack[v_sp-2]=v_stack[v_sp-2].var->val=v_stack[v_sp-1];
                }
              v_sp -= 1;
              v_stack[v_sp-1].var = NULL;
              break;

            case toNOT:
              if ((v_stack[v_sp-1].tag == tvSTR) ||
                  (v_stack[v_sp-2].tag == tvSTR))
                {
                  error(v_stack[v_sp-2].pos, "Illegal string operation");
                  return qnan;
                }
              else
              if (v_stack[v_sp-1].tag == tvINT) 
                {
                  v_stack[v_sp-1].ival = !v_stack[v_sp-1].ival;
                }
              else
                {
                  v_stack[v_sp-1].ival = !v_stack[v_sp-1].fval;
                  v_stack[v_sp-1].tag = tvINT;
                }
              v_stack[v_sp-1].var = NULL;
              break;

            case toMINUS:
              if (v_stack[v_sp-1].tag == tvINT)
                {
                  v_stack[v_sp-1].ival = -v_stack[v_sp-1].ival;
                }
              else
                {
                  v_stack[v_sp-1].fval = -v_stack[v_sp-1].fval;
                }
              // no break

            case toPLUS:
              v_stack[v_sp-1].var = NULL;
              break;

            case toCOM:
              if (v_stack[v_sp-1].tag == tvINT)
                {
                  v_stack[v_sp-1].ival = ~v_stack[v_sp-1].ival;
                }
              else
                {
                  v_stack[v_sp-1].ival = ~(int)v_stack[v_sp-1].fval;
                  v_stack[v_sp-1].tag = tvINT;
                }
              v_stack[v_sp-1].var = NULL;
              break;

            case toRPAR:
              error("mismatched ')'");
              return qnan;

            case toFUNC:
              error("'(' expected");
              return qnan;

            case toLPAR:
              if (oper != toRPAR)
                {
                  error("')' expected");
                  return qnan;
                }

              if (o_stack[o_sp-1] == toFUNC)
                {
                  symbol* sym = v_stack[v_sp-n_args-1].var;
                  if (sym)
                  {
                  switch (sym->tag)
                    {
                    case tsIFUNC1:
                      if (n_args != 1)
                        {
                          error(v_stack[v_sp-n_args-1].pos,
                                "Function should take one argument");
                          return qnan;
                        }
                      v_stack[v_sp-2].ival =
                        (*(int_t(*)(int_t))sym->func)(v_stack[v_sp-1].get_int());
                      v_stack[v_sp-2].tag = tvINT;
                      v_sp -= 1;
                      break;

                    case tsIFUNC2:
                      if (n_args != 2)
                        {
                          error(v_stack[v_sp-n_args-1].pos,
                                "Function should take two arguments");
                          return qnan;
                        }
                      v_stack[v_sp-3].ival =
                        (*(int_t(*)(int_t,int_t))sym->func)
                        (v_stack[v_sp-2].get_int(), v_stack[v_sp-1].get_int());
                      v_stack[v_sp-3].tag = tvINT;
                      v_sp -= 2;
                      break;

                    case tsIFFUNC3:
                      if (n_args != 3)
                        {
                          error(v_stack[v_sp-n_args-1].pos,
                                "Function should take three arguments");
                          return qnan;
                        }
                      v_stack[v_sp-4].ival =
                        (*(int_t(*)(double, double, int_t))sym->func)
                        (v_stack[v_sp-3].get_dbl(), v_stack[v_sp-2].get_dbl(), v_stack[v_sp-1].get_int());
                      v_stack[v_sp-4].tag = tvINT;
                      v_sp -= 3;
                    break;

                    case tsFFUNC1:
                      if (n_args != 1)
                        {
                          error(v_stack[v_sp-n_args-1].pos,
                                "Function should take one argument");
                          return qnan;
                        }
                      v_stack[v_sp-2].fval =
                        (*(float__t(*)(float__t))sym->func)(v_stack[v_sp-1].get());
                      v_stack[v_sp-2].tag = tvFLOAT;
                      v_sp -= 1;
                      break;

                    case tsFFUNC2:
                      if (n_args != 2)
                        {
                          error(v_stack[v_sp-n_args-1].pos,
                                "Function should take two arguments");
                          return qnan;
                        }
                      v_stack[v_sp-3].fval =
                        (*(float__t(*)(float__t, float__t))sym->func)
                        (v_stack[v_sp-2].get(), v_stack[v_sp-1].get());
                      v_stack[v_sp-3].tag = tvFLOAT;
                      v_sp -= 2;
                      break;

                    case tsFFUNC3:
                      if (n_args != 3)
                        {
                          error(v_stack[v_sp-n_args-1].pos,
                                "Function should take three arguments");
                          return qnan;
                        }

                      if (v_stack[v_sp-1].tag == tvPERCENT)
                        v_stack[v_sp-1].fval /= 100;

                      v_stack[v_sp-4].fval =
                        (*(float__t(*)(float__t, float__t, float__t))sym->func)
                        (v_stack[v_sp-3].get(), v_stack[v_sp-2].get(), v_stack[v_sp-1].get());
                      v_stack[v_sp-4].tag = tvFLOAT;
                      v_sp -= 3;
                    break;

                    case tsPFUNCn:
                      if (n_args < 1)
                        {
                          error(v_stack[v_sp-n_args-1].pos,
                                "Function should take one or more arguments");
                          return qnan;
                        }
                      v_stack[v_sp-n_args-1].tag = tvSTR;
                      v_stack[v_sp-n_args-1].ival = 0;
                      v_stack[v_sp-n_args-1].sval = (char *)malloc(STRBUF);

                      (*(int_t(*)(char *,char *, int, value*))sym->func)
                       (v_stack[v_sp-n_args-1].sval,
                        v_stack[v_sp-n_args].get_str(),
                        n_args-1, &v_stack[v_sp-n_args+1]);
                      strcpy(sres, v_stack[v_sp-n_args-1].sval);
                      if (n_args > 1)
                       {
                        #ifdef _long_double_
                        //DBL_MAX
                        v_stack[v_sp-n_args-1].ival = v_stack[v_sp-n_args+1].ival;
                        if (v_stack[v_sp-n_args+1].fval > maxdbl) v_stack[v_sp-n_args-1].fval = qnan;
                        else v_stack[v_sp-n_args-1].fval = v_stack[v_sp-n_args+1].fval;
                        //v_stack[v_sp-n_args-1].fval = v_stack[v_sp-n_args+1].fval;
                        #else /*_long_double_*/
                        v_stack[v_sp-n_args-1].ival = v_stack[v_sp-n_args+1].ival;
                        //v_stack[v_sp-n_args-1].fval = v_stack[v_sp-n_args+1].fval;
                        #endif /*_long_double_*/
                       }
                      v_sp -= n_args;
                    break;
                    case tsSIFUNC1:
                      if (n_args != 1)
                        {
                          error(v_stack[v_sp-n_args-1].pos,
                                "Function should take one argument");
                          return qnan;
                        }
                      v_stack[v_sp-2].ival =
                        (*(int_t(*)(char *))sym->func)(v_stack[v_sp-1].get_str());
                      v_stack[v_sp-2].tag = tvINT;
                      v_sp -= 1;
                    break;
                    default:
                      error("Invalid expression");
                    }
                  }
                  v_stack[v_sp-1].var = NULL;
                  o_sp -= 1;
                  n_args = 1;
                }
              else
              if (n_args != 1)
                {
                  error("Function call expected");
                  return qnan;
                }
              goto next_token;
            default:
              error("syntax error");
           }
        }
      if (o_sp == max_stack_size)
        {
          error("operator stack overflow");
          return qnan;
        }
      o_stack[o_sp++] = oper;
    }
}

