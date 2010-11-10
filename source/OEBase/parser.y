/*!
 * \file parser.y
 * \date 9-10-2009 17:13:08
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 * 
 * Usage: yacc -p xml --defines=XmlParser.hpp --output=XmlParser.cpp parser.y
 * 
 */
%{
#include "XmlGenerator.h"
%}

%union
{
	int nType;
	int nNameIndex;
	int nValueIndex;
	COEXmlAttribute_Impl* pXmlAttribute;
	COEXmlNode_Impl* pXmlNode;
};

%token <nType> PROLOG_HEAD PROLOG_TAIL EQUAL NODE_TAIL_CLOSED NODE_TAIL_NONCLOSED ERROR_AND_ABORT
%token <nNameIndex> CLOSE_NODE NODE_HEAD ATTR_NAME
%token <nValueIndex> ATTR_VALUE TEXT

%type <pXmlAttribute> attrs attr
%type <pXmlNode> document prolog nodes node

%%
document : prolog node											{
																	CXmlGenerator::SetDocumentNode($1, $2);
																}
	;

prolog : PROLOG_HEAD attrs PROLOG_TAIL							{
																	$$ = CXmlGenerator::CreateNode(CXmlGenerator::MakeName("xml"), CXmlGenerator::INVALID_STRING_INDEX);
																	$$->LinkAttribute($2);
																}
	;

nodes : /* empty */												{
																	$$ = NULL;
																}

	| nodes node												{
																	if ($1)
																	{
																		$1->LinkSibling($2);
																		$$ = $1;
																	}
																	else
																	{
																		$$ = $2;
																	}
																}
	;

attrs : /* empty */												{
																	$$ = NULL;
																}

	| attrs attr												{
																	if ($1)
																	{
																		$1->LinkSibling($2);
																		$$ = $1;
																	}
																	else
																	{
																		$$ = $2;
																	}
																}
	;

node : TEXT														{
																	$$ = CXmlGenerator::CreateNode(CXmlGenerator::INVALID_STRING_INDEX, $1);
																}

	| NODE_HEAD attrs NODE_TAIL_CLOSED							{
																	$$ = CXmlGenerator::CreateNode($1, CXmlGenerator::INVALID_STRING_INDEX);
																	$$->LinkAttribute($2);
																}

	| NODE_HEAD attrs NODE_TAIL_NONCLOSED nodes CLOSE_NODE		{
																	$$ = CXmlGenerator::CreateNode($1, CXmlGenerator::INVALID_STRING_INDEX);
																	$$->LinkAttribute($2);
																	$$->LinkChild($4);
																}
	;

attr : ATTR_NAME EQUAL ATTR_VALUE								{
																	$$ = CXmlGenerator::CreateAttribute($1, $3);
																}
	;
%%
