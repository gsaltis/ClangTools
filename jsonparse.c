/*****************************************************************************
 * FILE NAME    : jsonparse.c
 * DATE         : March 24 2023
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
MainSourceFilename = NULL;

static string
MainOutputFilename = NULL;

static string
mainProgramName = "Test";

static string
mainElementName = NULL;

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
ProcessInnerNode
(JSONOut* InObject);

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
}

/*****************************************************************************!
 * Function : MainInitialize
 *****************************************************************************/
void
MainInitialize
(void)
{
  
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
  
  if ( argc == 1 ) {
    return;
  }

  for ( i = 1 ; i < argc ; i++ ) {
    command = argv[i];
    if ( StringEqualsOneOf(command, "-h", "--help", NULL) ) {
      MainDisplayHelp();
      exit(EXIT_SUCCESS);
    }

    if ( StringEqualsOneOf(command, "-i", "--input", NULL) ) {
      i++;
      if ( i == argc ) {
        fprintf(stderr, "%s is missing a filename\n", command);
        MainDisplayHelp();
        exit(EXIT_FAILURE);
      }
      if ( MainSourceFilename ){
        FreeMemory(MainSourceFilename);
      }
      MainSourceFilename = StringCopy(argv[i]);
      continue;
    }

    if ( StringEqualsOneOf(command, "-e", "--element", NULL) ) {
      i++;
      if ( i == argc ) {
        fprintf(stderr, "%s is missing an element\n", command);
        MainDisplayHelp();
        exit(EXIT_FAILURE);
      }
      if ( mainElementName ){
        FreeMemory(mainElementName);
      }
      mainElementName = StringCopy(argv[i]);
      continue;
    }

    
    fprintf(stderr, "%s is an unknown command\n", command);
    MainDisplayHelp();
    exit(EXIT_FAILURE);
  }
}

/*****************************************************************************!
 * Function : MainVerifyCommandLine
 *****************************************************************************/
void
MainVerifyCommandLine
(void)
{
  if ( NULL == MainSourceFilename ) {
    fprintf(stderr, "Missing source filename\n");
    exit(EXIT_FAILURE);
  }
  MainOutputFilename = StringConcat(MainSourceFilename, ".json");
}

/*****************************************************************************!
 * Function : MainProcess
 *****************************************************************************/
void
MainProcess
(void)
{
  int                                   i;
  JSONOut*                              json;
  int                                   n;
  char*                                 buffer;
  int                                   filesize;
  FILE*                                 file;
  struct stat                           statbuf;
  JSONOut*                              obj;
  
  file = fopen(MainOutputFilename, "rb");
  if ( NULL == file ) {
    fprintf(stderr, "Could not open %s\n", MainOutputFilename);
    exit(EXIT_FAILURE);
  }
  stat(MainOutputFilename, &statbuf) == 0;

  filesize = statbuf.st_size;
  buffer = (char*)GetMemory(filesize + 1);
  n = fread(buffer, 1, filesize, file);
  if ( n != filesize ) {
    fclose(file);
    fprintf(stderr, "Could not read %s : %s\n", MainOutputFilename, strerror(errno));
    exit(EXIT_FAILURE);
  }
  buffer[filesize] = 0x00;
  fclose(file);
  json = JSONOutFromString(buffer);
  FreeMemory(buffer);
  if ( NULL == json ) {
    fprintf(stderr, "Could not parse %s\n", MainOutputFilename);
    exit(EXIT_FAILURE);
  }
  if ( json->type != JSONOutTypeObject ) {
    JSONOutDestroy(json);
    return;
  }
  for (i = 0; i < json->valueObject->count; i++) {
    obj = json->valueObject->objects[i];
    if ( StringEqual(obj->tag, "inner") ) {
      ProcessInnerNode(obj);
    }
  }
}

/*****************************************************************************!
 * Function : ProcessInnerNode
 *****************************************************************************/
void
ProcessInnerNode
(JSONOut* InObject)
{
  string                                name;
  JSONOut*                              fileObj;
  JSONOut*                              locObj;
  JSONOut*                              nameObj;
  JSONOut*                              kindObj;
  JSONOut*                              obj;
  int                                   i;
  JSONOutArray*                         innerArray;
  bool                                  inTargetFile = false;
  string                                kindString;
  bool                                  haveElement = false;
  
  innerArray = InObject->valueArray;

  printf("[");
  for (i = 0; i < innerArray->count; i++) {
    obj = innerArray->objects[i];
    kindObj = JSONOutFind(obj, "kind");
    kindString = kindObj->valueString;
    nameObj = JSONOutFind(obj, "name");
    locObj  = JSONOutFind(obj, "loc");
    
    if ( locObj ) {
      fileObj = JSONOutFind(locObj, "file");
      if ( fileObj ) {
        if ( StringEqual(fileObj->valueString, MainSourceFilename) ) {
          if ( NULL == mainElementName ) {
            printf("---- %s---- \n", fileObj->valueString);
          }
          inTargetFile = true;
        }
      }
    }
    if ( inTargetFile ) {
      name = "";
      if ( nameObj && nameObj->type == JSONOutTypeString ) {
        name = nameObj->valueString;
      }
      if ( mainElementName == NULL ) {
        printf("%4d : %30s %40s\n", i, kindString, name);
        continue;
      }
      if ( StringEqual(mainElementName, name) ) {
        if ( haveElement ) {
          printf(",");
        }
        printf("\n");
        string st = JSONOutToString(obj, 2, 2);
        printf("%s", st);
        FreeMemory(st);
        haveElement = true;
      }
    }
  }
  printf("\n");
  printf("]\n");
}

/*****************************************************************************!
 * Function : MainDisplayHelp
 *****************************************************************************/
void
MainDisplayHelp
(void)
{
  printf("Usage : %s options\n", mainProgramName);
  printf("  options\n");
  printf("    -h, --help             : Display this information\n");
  printf("    -i, --input filename   : Specify the input file name\n");
}
