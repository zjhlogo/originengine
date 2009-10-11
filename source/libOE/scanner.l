%{
#include "XmlGenerator.h"
#include "XmlParser.hpp"

#undef YY_INPUT
#define YY_INPUT(b, r, ms) (r = CXmlGenerator::DumpString(b, ms))
%}

%x comment
%x header
%x node

NAME [_A-Za-z][A-Za-z0-9]*
BLANK [ \t\r\n]
STRING (\"[^\"]*\")|(\'[^\']*\')

%%
"<!--"										BEGIN(comment);
<comment>.|\n								;
<comment>"-->"								BEGIN(INITIAL);

"<?xml"										BEGIN(header); xmllval.nType = PROLOG_HEAD; return PROLOG_HEAD;
<header>"?>"								BEGIN(INITIAL); xmllval.nType = PROLOG_TAIL; return PROLOG_TAIL;

"</"{NAME}{BLANK}*">"						xmllval.nNameIndex = CXmlGenerator::MakeName(yytext); return CLOSE_NODE;

"<"{NAME}									BEGIN(node); xmllval.nNameIndex = CXmlGenerator::MakeName(yytext); return NODE_HEAD;
<node>"/>"									BEGIN(INITIAL); xmllval.nType = NODE_TAIL_CLOSED; return NODE_TAIL_CLOSED;
<node>">"									BEGIN(INITIAL); xmllval.nType = NODE_TAIL_NONCLOSED; return NODE_TAIL_NONCLOSED;

<header,node>{NAME}							xmllval.nNameIndex = CXmlGenerator::MakeName(yytext); return ATTR_NAME;
<header,node>{BLANK}*"="{BLANK}*			xmllval.nType = EQUAL; return EQUAL;
<header,node>{STRING}						xmllval.nValueIndex = CXmlGenerator::MakeValue(yytext); return ATTR_VALUE;

<INITIAL>[^ \t\r\n<>][^<>]*[^ \t\r\n<>]		xmllval.nValueIndex = CXmlGenerator::MakeText(yytext); return TEXT;

<*>{BLANK}									;
<*>.										return ERROR_AND_ABORT;
%%
