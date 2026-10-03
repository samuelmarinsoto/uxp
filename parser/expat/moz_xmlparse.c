/* -*- Mode: C; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "moz_expat.h"
#include "lib/xmlparse.c"

/* omoon: this copy is THE expat the whole-archive static link serves to
   the native consumers too (fontconfig parses fonts.conf through it).
   MOZILLA_CLIENT drops the "unused API" entry points they need, so
   provide the two that are actually referenced. XML_ErrorString is the
   upstream switch (error strings are descriptive only). */
XML_Parser XMLCALL
XML_ParserCreate(const XML_Char *encodingName)
{
  return XML_ParserCreate_MM(encodingName, NULL, NULL);
}

/* omoon: canonical body, verbatim from lib/xmlparse.c (the
   MOZILLA_CLIENT build drops it; fontconfig needs it). XML_L is defined
   inside xmlparse.c's char16_t branch, which MOZILLA_CLIENT skips. */
#ifndef XML_L
#define XML_L(x) x
#endif
const XML_LChar *XMLCALL
XML_ErrorString(enum XML_Error code) {
  switch (code) {
  case XML_ERROR_NONE:
    return NULL;
  case XML_ERROR_NO_MEMORY:
    return XML_L("out of memory");
  case XML_ERROR_SYNTAX:
    return XML_L("syntax error");
  case XML_ERROR_NO_ELEMENTS:
    return XML_L("no element found");
  case XML_ERROR_INVALID_TOKEN:
    return XML_L("not well-formed (invalid token)");
  case XML_ERROR_UNCLOSED_TOKEN:
    return XML_L("unclosed token");
  case XML_ERROR_PARTIAL_CHAR:
    return XML_L("partial character");
  case XML_ERROR_TAG_MISMATCH:
    return XML_L("mismatched tag");
  case XML_ERROR_DUPLICATE_ATTRIBUTE:
    return XML_L("duplicate attribute");
  case XML_ERROR_JUNK_AFTER_DOC_ELEMENT:
    return XML_L("junk after document element");
  case XML_ERROR_PARAM_ENTITY_REF:
    return XML_L("illegal parameter entity reference");
  case XML_ERROR_UNDEFINED_ENTITY:
    return XML_L("undefined entity");
  case XML_ERROR_RECURSIVE_ENTITY_REF:
    return XML_L("recursive entity reference");
  case XML_ERROR_ASYNC_ENTITY:
    return XML_L("asynchronous entity");
  case XML_ERROR_BAD_CHAR_REF:
    return XML_L("reference to invalid character number");
  case XML_ERROR_BINARY_ENTITY_REF:
    return XML_L("reference to binary entity");
  case XML_ERROR_ATTRIBUTE_EXTERNAL_ENTITY_REF:
    return XML_L("reference to external entity in attribute");
  case XML_ERROR_MISPLACED_XML_PI:
    return XML_L("XML or text declaration not at start of entity");
  case XML_ERROR_UNKNOWN_ENCODING:
    return XML_L("unknown encoding");
  case XML_ERROR_INCORRECT_ENCODING:
    return XML_L("encoding specified in XML declaration is incorrect");
  case XML_ERROR_UNCLOSED_CDATA_SECTION:
    return XML_L("unclosed CDATA section");
  case XML_ERROR_EXTERNAL_ENTITY_HANDLING:
    return XML_L("error in processing external entity reference");
  case XML_ERROR_NOT_STANDALONE:
    return XML_L("document is not standalone");
  case XML_ERROR_UNEXPECTED_STATE:
    return XML_L("unexpected parser state - please send a bug report");
  case XML_ERROR_ENTITY_DECLARED_IN_PE:
    return XML_L("entity declared in parameter entity");
  case XML_ERROR_FEATURE_REQUIRES_XML_DTD:
    return XML_L("requested feature requires XML_DTD support in Expat");
  case XML_ERROR_CANT_CHANGE_FEATURE_ONCE_PARSING:
    return XML_L("cannot change setting once parsing has begun");
  /* Added in 1.95.7. */
  case XML_ERROR_UNBOUND_PREFIX:
    return XML_L("unbound prefix");
  /* Added in 1.95.8. */
  case XML_ERROR_UNDECLARING_PREFIX:
    return XML_L("must not undeclare prefix");
  case XML_ERROR_INCOMPLETE_PE:
    return XML_L("incomplete markup in parameter entity");
  case XML_ERROR_XML_DECL:
    return XML_L("XML declaration not well-formed");
  case XML_ERROR_TEXT_DECL:
    return XML_L("text declaration not well-formed");
  case XML_ERROR_PUBLICID:
    return XML_L("illegal character(s) in public id");
  case XML_ERROR_SUSPENDED:
    return XML_L("parser suspended");
  case XML_ERROR_NOT_SUSPENDED:
    return XML_L("parser not suspended");
  case XML_ERROR_ABORTED:
    return XML_L("parsing aborted");
  case XML_ERROR_FINISHED:
    return XML_L("parsing finished");
  case XML_ERROR_SUSPEND_PE:
    return XML_L("cannot suspend in external parameter entity");
  /* Added in 2.0.0. */
  case XML_ERROR_RESERVED_PREFIX_XML:
    return XML_L(
        "reserved prefix (xml) must not be undeclared or bound to another namespace name");
  case XML_ERROR_RESERVED_PREFIX_XMLNS:
    return XML_L("reserved prefix (xmlns) must not be declared or undeclared");
  case XML_ERROR_RESERVED_NAMESPACE_URI:
    return XML_L(
        "prefix must not be bound to one of the reserved namespace names");
  /* Added in 2.2.5. */
  case XML_ERROR_INVALID_ARGUMENT: /* Constant added in 2.2.1, already */
    return XML_L("invalid argument");
    /* Added in 2.3.0. */
  case XML_ERROR_NO_BUFFER:
    return XML_L(
        "a successful prior call to function XML_GetBuffer is required");
  /* Added in 2.4.0. */
  case XML_ERROR_AMPLIFICATION_LIMIT_BREACH:
    return XML_L(
        "limit on input amplification factor (from DTD and entities) breached");
  /* Added in 2.6.4. */
  case XML_ERROR_NOT_STARTED:
    return XML_L("parser not started");
  }
  return NULL;
}

void
MOZ_XML_SetXmlDeclHandler(XML_Parser parser,
                          XML_XmlDeclHandler xmldecl) {
  XML_SetXmlDeclHandler(parser, xmldecl);
}

XML_Parser
MOZ_XML_ParserCreate_MM(const XML_Char *encoding,
                        const XML_Memory_Handling_Suite *memsuite,
                        const XML_Char *namespaceSeparator) {
  return XML_ParserCreate_MM(encoding, memsuite, namespaceSeparator);
}

void
MOZ_XML_SetElementHandler(XML_Parser parser,
                          XML_StartElementHandler start,
                          XML_EndElementHandler end) {
  XML_SetElementHandler(parser, start, end);
}

void
MOZ_XML_SetCharacterDataHandler(XML_Parser parser,
                                XML_CharacterDataHandler handler) {
  XML_SetCharacterDataHandler(parser, handler);
}

void
MOZ_XML_SetProcessingInstructionHandler(XML_Parser parser,
                                        XML_ProcessingInstructionHandler handler) {
  XML_SetProcessingInstructionHandler(parser, handler);
}

void
MOZ_XML_SetCommentHandler(XML_Parser parser,
                          XML_CommentHandler handler) {
  XML_SetCommentHandler(parser, handler);
}

void
MOZ_XML_SetCdataSectionHandler(XML_Parser parser,
                               XML_StartCdataSectionHandler start,
                               XML_EndCdataSectionHandler end) {
  XML_SetCdataSectionHandler(parser, start, end);
}

void
MOZ_XML_SetDefaultHandlerExpand(XML_Parser parser,
                                XML_DefaultHandler handler) {
  XML_SetDefaultHandlerExpand(parser, handler);
}

void
MOZ_XML_SetDoctypeDeclHandler(XML_Parser parser,
                              XML_StartDoctypeDeclHandler start,
                              XML_EndDoctypeDeclHandler end) {
  XML_SetDoctypeDeclHandler(parser, start, end);
}

void
MOZ_XML_SetUnparsedEntityDeclHandler(XML_Parser parser,
                                     XML_UnparsedEntityDeclHandler handler) {
  XML_SetUnparsedEntityDeclHandler(parser, handler);
}

void
MOZ_XML_SetNotationDeclHandler(XML_Parser parser,
                               XML_NotationDeclHandler handler) {
  XML_SetNotationDeclHandler(parser, handler);
}

void
MOZ_XML_SetNamespaceDeclHandler(XML_Parser parser,
                                XML_StartNamespaceDeclHandler start,
                                XML_EndNamespaceDeclHandler end) {
  XML_SetNamespaceDeclHandler(parser, start, end);
}

void
MOZ_XML_SetExternalEntityRefHandler(XML_Parser parser,
                                    XML_ExternalEntityRefHandler handler) {
  XML_SetExternalEntityRefHandler(parser, handler);
}

void
MOZ_XML_SetExternalEntityRefHandlerArg(XML_Parser parser, void *arg) {
  XML_SetExternalEntityRefHandlerArg(parser, arg);
}

void
MOZ_XML_SetReturnNSTriplet(XML_Parser parser, int do_nst) {
  XML_SetReturnNSTriplet(parser, do_nst);
}

void
MOZ_XML_SetUserData(XML_Parser parser, void *p) {
  XML_SetUserData(parser, p);
}

enum XML_Status
MOZ_XML_SetBase(XML_Parser parser, const XML_Char *base) {
  return XML_SetBase(parser, base);
}

const XML_Char *
MOZ_XML_GetBase(XML_Parser parser) {
  return XML_GetBase(parser);
}

int
MOZ_XML_GetSpecifiedAttributeCount(XML_Parser parser) {
  return XML_GetSpecifiedAttributeCount(parser);
}

enum XML_Status
MOZ_XML_Parse(XML_Parser parser, const char *s, int len, int isFinal) {
  return XML_Parse(parser, s, len, isFinal);
}

enum XML_Status
MOZ_XML_StopParser(XML_Parser parser, int resumable) {
  return XML_StopParser(parser, resumable);
}

enum XML_Status
MOZ_XML_ResumeParser(XML_Parser parser) {
  return XML_ResumeParser(parser);
}

XML_Parser
MOZ_XML_ExternalEntityParserCreate(XML_Parser parser,
                                   const XML_Char *context,
                                   const XML_Char *encoding) {
  return XML_ExternalEntityParserCreate(parser, context, encoding);
}

int
MOZ_XML_SetParamEntityParsing(XML_Parser parser,
                              enum XML_ParamEntityParsing parsing) {
  return XML_SetParamEntityParsing(parser, parsing);
}

int
MOZ_XML_SetHashSalt(XML_Parser parser, unsigned long hash_salt) {
  return XML_SetHashSalt(parser, hash_salt);
}

enum XML_Error
MOZ_XML_GetErrorCode(XML_Parser parser)
{
  return XML_GetErrorCode(parser);
}

XML_Size MOZ_XML_GetCurrentLineNumber(XML_Parser parser) {
  return XML_GetCurrentLineNumber(parser);
}

XML_Size MOZ_XML_GetCurrentColumnNumber(XML_Parser parser) {
  return XML_GetCurrentColumnNumber(parser);
}

XML_Index MOZ_XML_GetCurrentByteIndex(XML_Parser parser) {
  return XML_GetCurrentByteIndex(parser);
}

void
MOZ_XML_ParserFree(XML_Parser parser) {
  XML_ParserFree(parser);
}

XML_Bool MOZ_XML_SetReparseDeferralEnabled(XML_Parser parser, int enabled) {
  return XML_SetReparseDeferralEnabled(parser, enabled);
}
