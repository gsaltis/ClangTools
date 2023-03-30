/*****************************************************************************
 * FILE NAME    : jsonschema.c
 * DATE         : March 27 2023
 * COPYRIGHT    : Copyright (C) 2023 by Gregory R Saltis
 *****************************************************************************/

/*****************************************************************************!
 * Global Headers
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <error.h>
#include <sys/stat.h>
#include <StringUtils.h>
#include <MemoryManager.h>
#include <FileUtils.h>
#include <JSONOut.h>

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/

/*****************************************************************************!
 * Local Macros
 *****************************************************************************/

/*****************************************************************************!
 * Local Data
 *****************************************************************************/
static string
mainFilename = NULL;

static string
mainProgramName = "jsonschema";

static StringList*
kindTypes = NULL;

/*****************************************************************************!
 * Local Functions
 *****************************************************************************/
void
MainDisplayHelp
(void);

void
MainProcess
(void);

void
MainVerifyCommandLine
(void);

void
MainProcessCommandLine
(int argc, char** argv);

void
MainInitialize
(void);

void
JSONGetSchema
(string InBufferString);

void
JSONParseInt
(JSONOut* InJSON, int InIndent);

void
JSONParseString
(JSONOut* InJSON, int InIndent);

void
JSONParseLongLong
(JSONOut* InJSON, int InIndent);

void
JSONParseFloat
(JSONOut* InJSON, int InIndent);

void
JSONParseBool
(JSONOut* InJSON, int InIndent);

void
JSONParseArray
(JSONOut* InJSON, int InIndent);

void
JSONParseObject
(JSONOut* InJSON, int InIndent);

void
MainDisplayTypes
();

/*****************************************************************************!
 * Function : main
 *****************************************************************************/
int
main(int argc, char**argv)
{
  MainInitialize();
  MainProcessCommandLine(argc, argv);
  MainVerifyCommandLine();
  MainProcess();
  MainDisplayTypes();
}

/*****************************************************************************!
 * Function : MainDisplayTypes
 *****************************************************************************/
void
MainDisplayTypes
()
{
  int                                   i;

  for ( i = 0 ; i < kindTypes->stringCount; i++ ) {
    printf("%2d : %s\n", i + 1, kindTypes->strings[i]);
  }
}

/*****************************************************************************!
 * Function : MainInitialize
 *****************************************************************************/
void
MainInitialize
(void)
{
  kindTypes = StringListCreate();
}

/*****************************************************************************!
 * Function : MainProcessCommandLine
 *****************************************************************************/
void
MainProcessCommandLine
(int argc, char** argv)
{
  int                                   i = 0;
  string                                command = NULL;
  
  for ( i = 1 ; i < argc ; i++ ) {
    command = argv[i];
    if ( StringEqualsOneOf(command, "-h", "--help", NULL) ) {
      MainDisplayHelp();
      exit(EXIT_SUCCESS);
    }

    if ( command[0] == '-' ) {
      fprintf(stderr, "%s is an unknown command\n", command);
      MainDisplayHelp();
      exit(EXIT_FAILURE);
    }
    break;
  }

  if ( i + 1 != argc ) {
    fprintf(stderr, "  Missing filename\n");
    MainDisplayHelp();
    exit(EXIT_FAILURE);
  }
  mainFilename = StringCopy(argv[i]);
}

/*****************************************************************************!
 * Function : MainVerifyCommandLine
 *****************************************************************************/
void
MainVerifyCommandLine
(void)
{
  int                                   bytesRead;
  FILE*                                 file;
  struct stat                           statbuf;
  uint32_t                              filesize;
  char*                                 filebuffer;

  file = fopen(mainFilename, "rb");
  if ( NULL == file ) {
    fprintf(stderr, "Could not open file %s : %s\n", mainFilename, strerror(errno));
    exit(EXIT_FAILURE);
  }
  stat(mainFilename, &statbuf);
  filesize = statbuf.st_size;

  filebuffer = (char*)GetMemory(filesize + 1);
  bytesRead = fread(filebuffer, 1, filesize, file);
  if ( bytesRead != filesize ) {
    fclose(file);
    FreeMemory(filebuffer);
    fprintf(stderr, "Could not read %s\n", mainFilename);
    exit(EXIT_FAILURE);
  }
  filebuffer[filesize] = 0x00;
  JSONGetSchema(filebuffer);
  FreeMemory(filebuffer);
  fclose(file);
}

/*****************************************************************************!
 * Function : MainProcess
 *****************************************************************************/
void
MainProcess
(void)
{
  
}

/*****************************************************************************!
 * Function : MainDisplayHelp
 *****************************************************************************/
void
MainDisplayHelp
(void)
{
  printf("Usage : %s options filename\n", mainProgramName);
  printf("  options\n");
  printf("    -h, --help  : Display this information\n");
}

/*****************************************************************************!
 * Function : JSONGetSchema
 *****************************************************************************/
void
JSONGetSchema
(string InBufferString)
{
  JSONOut*                              jsonTop;

  jsonTop = JSONOutFromString(InBufferString);
  if ( NULL == jsonTop ) {
    fprintf(stderr, "Could not parse %s\n", mainFilename);
    return;
  }

  switch ( jsonTop->type ) {
    case JSONOutTypeNone : {
      return;
    }
      
    case JSONOutTypeInt : {
      JSONParseInt(jsonTop, 0);
      return;
    }
      
    case JSONOutTypeLongLong : {
      JSONParseLongLong(jsonTop, 0);
      return;
    }
      
    case JSONOutTypeFloat : {
      JSONParseFloat(jsonTop, 0);
      return;
    }
      
    case JSONOutTypeString : {
      JSONParseString(jsonTop, 0);
      return;
    }
      
    case JSONOutTypeBool : {
      JSONParseBool(jsonTop, 0);
      return;
    }
      
    case JSONOutTypeArray : {
      JSONParseArray(jsonTop, 0);
      return;
    }
      
    case JSONOutTypeObject : {
      JSONParseObject(jsonTop, 0);
      return;
    }
  }    
    
  JSONOutDestroy(jsonTop);
}

/*****************************************************************************!
 * Function JSONParseInt
 *****************************************************************************/
void
JSONParseInt
(JSONOut* InJSON, int InIndent)
{
  char                                  indentString[128];
  memset(indentString, 0x20, 128);
  indentString[InIndent] = 0x00;

  printf("%s%s : Int\n", indentString, InJSON->tag);
}

/*****************************************************************************!
 * Function JSONParseBool
 *****************************************************************************/
void
JSONParseBool
(JSONOut* InJSON, int InIndent)
{
  char                                  indentString[128];
  memset(indentString, 0x20, 128);
  indentString[InIndent] = 0x00;

  printf("%s%s : Bool\n", indentString, InJSON->tag);
}

/*****************************************************************************!
 * Function JSONParseLongLong
 *****************************************************************************/
void
JSONParseLongLong
(JSONOut* InJSON, int InIndent)
{
  char                                  indentString[128];
  memset(indentString, 0x20, 128);
  indentString[InIndent] = 0x00;

  printf("%s%s : LongLong\n", indentString, InJSON->tag);
}

/*****************************************************************************!
 * Function JSONParseString
 *****************************************************************************/
void
JSONParseString
(JSONOut* InJSON, int InIndent)
{
  char                                  indentString[128];
  memset(indentString, 0x20, 128);
  indentString[InIndent] = 0x00;

  printf("%s%s : String ", indentString, InJSON->tag);
  if ( StringEqualsOneOf(InJSON->tag, "kind", "name", NULL) ) {
    if ( StringEqual(InJSON->tag, "kind") ) {
      if ( ! StringListContains(kindTypes, InJSON->valueString) ) {
        StringListAppend(kindTypes, StringCopy(InJSON->valueString));
      }
    }
    printf("%s", InJSON->valueString);
  }
  printf("\n");
}

/*****************************************************************************!
 * Function JSONParseFloat
 *****************************************************************************/
void
JSONParseFloat
(JSONOut* InJSON, int InIndent)
{
  char                                  indentString[128];
  memset(indentString, 0x20, 128);
  indentString[InIndent] = 0x00;

  printf("%s%s : Float\n", indentString, InJSON->tag);
}

/*****************************************************************************!
 * Function JSONParseArray
 *****************************************************************************/
void
JSONParseArray
(JSONOut* InJSON, int InIndent)
{
  char                                  indentString[128];
  int                                   i;
  JSONOut*                              json;

  memset(indentString, 0x20, 128);
  indentString[InIndent] = 0x00;

  printf("%s", indentString);
  if ( InJSON->tag ) {
    printf("%s ", InJSON->tag);
  }
  printf(" [\n");
  for ( i = 0 ; i < InJSON->valueArray->count; i++ ) {
    json = InJSON->valueArray->objects[i];
    switch ( json->type ) {
      case JSONOutTypeNone : {
        continue;;
      }
        
      case JSONOutTypeInt : {
        JSONParseInt(json, InIndent + 2);
        continue;;
      }
        
      case JSONOutTypeLongLong : {
        JSONParseLongLong(json, InIndent + 2);
        continue;;
      }
        
      case JSONOutTypeFloat : {
        JSONParseFloat(json, InIndent + 2);
        continue;;
      }
        
      case JSONOutTypeString : {
        JSONParseString(json, InIndent + 2);
        continue;;
      }
        
      case JSONOutTypeBool : {
        JSONParseBool(json, InIndent + 2);
        continue;;
      }
        
      case JSONOutTypeArray : {
        JSONParseArray(json, InIndent + 2);
        continue;;
      }
        
      case JSONOutTypeObject : {
        JSONParseObject(json, InIndent + 2);
        continue;;
      }
    }    
  }
  printf("%s]\n", indentString);
}
  
/*****************************************************************************!
 * Function JSONParseObject
 *****************************************************************************/
void
JSONParseObject
(JSONOut* InJSON, int InIndent)
{
  char                                  indentString[128];
  int                                   i;
  JSONOut*                              json;
  
  memset(indentString, 0x20, 128);
  indentString[InIndent] = 0x00;

  printf("%s", indentString);
  if ( InJSON->tag ) {
    printf("%s ", InJSON->tag);
  }

  if ( InJSON->valueObject->count == 0 ) {
    printf("{ }\n");
    return;
  }
  printf("{\n");
  
  for ( i = 0 ; i < InJSON->valueObject->count; i++ ) {
    json = InJSON->valueObject->objects[i];

    switch ( json->type ) {
      case JSONOutTypeNone : {
        continue;;
      }
        
      case JSONOutTypeInt : {
        JSONParseInt(json, InIndent + 2);
        continue;;
      }
        
      case JSONOutTypeLongLong : {
        JSONParseLongLong(json, InIndent + 2);
        continue;;
      }
        
      case JSONOutTypeFloat : {
        JSONParseFloat(json, InIndent + 2);
        continue;;
      }
        
      case JSONOutTypeString : {
        JSONParseString(json, InIndent + 2);
        continue;;
      }
        
      case JSONOutTypeBool : {
        JSONParseBool(json, InIndent + 2);
        continue;;
      }
        
      case JSONOutTypeArray : {
        JSONParseArray(json, InIndent + 2);
        continue;;
      }
        
      case JSONOutTypeObject : {
        JSONParseObject(json, InIndent + 2);
        continue;;
      }
    }    
  }
  printf("%s}\n", indentString);
}

