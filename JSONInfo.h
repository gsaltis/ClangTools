/*****************************************************************************
 * FILE NAME    : JSONInfo.h
 * DATE         : March 27 2023
 * COPYRIGHT    : Copyright (C) 2023 by Gregory R Saltis
 *****************************************************************************/
#ifndef _jsoninfo_h_
#define _jsoninfo_h_

/*****************************************************************************!
 * Global Headers
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <StringUtils.h>
#include <JSONOut.h>

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/

/*****************************************************************************!
 * Exported Macros
 *****************************************************************************/

/*****************************************************************************!
 * Exported Type : JSONInfo
 *****************************************************************************/
struct _JSONInfo
{
  int                                   count;
  string                                name;
  JSONOut**                             elements;
};
typedef struct _JSONInfo JSONInfo;
/*****************************************************************************!
 * Exported Type : JSONInfoList
 *****************************************************************************/
struct _JSONInfoList
{
  int                                   count;
  JSONInfo**                            elements;
};
typedef struct _JSONInfoList JSONInfoList;

/*****************************************************************************!
 * Exported Data
 *****************************************************************************/

/*****************************************************************************!
 * Exported Functions
 *****************************************************************************/
JSONInfo*
JSONInfoCreate
(string InName);

void
JSONInfoDestroy
(JSONInfo* InInfo);

void
JSONInfoAddElement
(JSONInfo* InInfo, JSONOut* InJSON);

int
JSONInfoGetCount
(JSONInfo* InInfo);

string
JSONInfoGetName
(JSONInfo* InInfo);

JSONOut*
JSONInfoGetElementByIndex
(JSONInfo* InInfo, int InIndex);

JSONInfoList*
JSONInfoListCreate
();

void
JSONInfoListDestroy
(JSONInfoList* InList);

void
JSONInfoListAddInfoElement
(JSONInfoList* InList, JSONInfo* InInfo);

JSONInfo*
JSONInfoListFindInfoElementByName
(JSONInfoList* InList, string InName);

JSONInfo*
JSONInfoListFindInfoElementByIndex
(JSONInfoList* InList, int InIndex);

bool
JSONInfoListInfoElementExistsByName
(JSONInfoList* InList, string InName);

bool
JSONInfoListInfoElementExistsByIndex
(JSONInfoList* InList, int InIndex);

int
JSONInfoListGetCount
(JSONInfoList* InList);

#endif /* _jsoninfo_h_*/
