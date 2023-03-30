/*****************************************************************************
 * FILE NAME    : JSONInfo.c
 * DATE         : March 27 2023
 * PROJECT      : 
 * COPYRIGHT    : Copyright (C) 2023 by Gregory R Saltis
 *****************************************************************************/

/*****************************************************************************!
 * Global Headers
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <MemoryManager.h>

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "JSONInfo.h"

/*****************************************************************************!
 * Local Macros
 *****************************************************************************/

/*****************************************************************************!
 * Local Functions
 *****************************************************************************/

/*****************************************************************************!
 * Local Data
 *****************************************************************************/

/*****************************************************************************!
 * Function : JSONInfoCreate
 *****************************************************************************/
JSONInfo*
JSONInfoCreate
(string InName)
{
  int                                   n;
  JSONInfo*                             info;

  if ( NULL == InName ) {
    return NULL;
  }
  
  n = sizeof(JSONInfo);
  info = (JSONInfo*)GetMemory(n);
  memset(info, 0x00, n);
  info->name = StringCopy(InName);
  return info;
}

/*****************************************************************************!
 * Function : JSONInfoDestroy
 *****************************************************************************/
void
JSONInfoDestroy
(JSONInfo* InInfo)
{
  if ( NULL == InInfo ) {
    return;
  }
  FreeMemory(InInfo->name);
  FreeMemory(InInfo->elements);
  // We don't destroy the individual JSONOut* structures since we don't
  // own them
  FreeMemory(InInfo);
}

/*****************************************************************************!
 * Function : JSONInfoAddElement
 *****************************************************************************/
void
JSONInfoAddElement
(JSONInfo* InInfo, JSONOut* InJSON)
{
  int                                   n;
  JSONOut**                             elements;

  if ( NULL == InInfo || NULL == InJSON ) {
    return;
  }

  n = InInfo->count;

  elements = (JSONOut**)GetMemory((n + 1) * sizeof(JSONOut*));
  for (int i = 0; i < n; i++) {
    elements[i] = InInfo->elements[i];
  }
  elements[n] = InJSON;
  if ( InInfo->count > 0 ) {
    FreeMemory(InInfo->elements);
  }
  InInfo->elements = elements;
  InInfo->count = n + 1;
}

/*****************************************************************************!
 * Function : JSONInfoGetCount
 *****************************************************************************/
int
JSONInfoGetCount
(JSONInfo* InInfo)
{
  if ( NULL == InInfo ) {
    return 0;
  }
  return InInfo->count;
}

/*****************************************************************************!
 * Function : JSONInfoGetName
 *****************************************************************************/
string
JSONInfoGetName
(JSONInfo* InInfo)
{
  if ( NULL == InInfo ) {
    return NULL;
  }
  return InInfo->name;
}

/*****************************************************************************!
 * Function : JSONInfoGetElementByIndex
 *****************************************************************************/
JSONOut*
JSONInfoGetElementByIndex
(JSONInfo* InInfo, int InIndex)
{
  if ( NULL == InInfo ) {
    return NULL;
  }

  if ( InInfo->count >= InIndex ) {
    return NULL;
  }

  return InInfo->elements[InIndex];
}

/*****************************************************************************!
 * Function : JSONInfoListCreate
 *****************************************************************************/
JSONInfoList*
JSONInfoListCreate
()
{
  int                                   n;
  JSONInfoList*                         list;

  n = sizeof(JSONInfoList);
  list = (JSONInfoList*)GetMemory(n);
  memset(list, 0x00, n);
  return list;
}

/*****************************************************************************!
 * Function : JSONInfoListDestroy
 *****************************************************************************/
void
JSONInfoListDestroy
(JSONInfoList* InList)
{
  if ( NULL == InList ) {
    return;
  }
  
  for (int i = 0; i < InList->count; i++) {
    JSONInfoDestroy(InList->elements[i]);
  }
  if ( InList->count > 0 ) {
    FreeMemory(InList->elements);
  }
  FreeMemory(InList);
}

/*****************************************************************************!
 * Function : JSONInfoListAddInfoElement
 *****************************************************************************/
void
JSONInfoListAddInfoElement
(JSONInfoList* InList, JSONInfo* InInfo)
{
  int                                   n;
  JSONInfo**                            elements;
  
  if ( NULL == InList || NULL == InInfo ) {
    return;
  }

  n = InList->count;

  elements = (JSONInfo**)GetMemory((n + 1) * sizeof(JSONInfo*));
  for (int i = 0; i < n; i++) {
    elements[i] = InList->elements[i];
  }
  elements[n] = InInfo;
  if ( InList->count > 0 ) {
    FreeMemory(InList->elements);
  }
  InList->elements = elements;
  InList->count = n + 1;
}

/*****************************************************************************!
 * Function : JSONInfoListFindInfoElementByName
 *****************************************************************************/
JSONInfo*
JSONInfoListFindInfoElementByName
(JSONInfoList* InList, string InName)
{
  if ( NULL == InList || NULL == InName ) {
    return NULL;
  }
  for (int i = 0; i < InList->count; i++) {
    if ( StringEqual(JSONInfoGetName(InList->elements[i]), InName) ) {
      return InList->elements[i];
    }
  }
  return NULL;
}

/*****************************************************************************!
 * Function : JSONInfoListFindInfoElementByIndex
 *****************************************************************************/
JSONInfo*
JSONInfoListFindInfoElementByIndex
(JSONInfoList* InList, int InIndex)
{
  if ( NULL == InList ) {
    return NULL;
  }

  if ( InIndex >= InList->count ) {
    return NULL;
  }
  return InList->elements[InIndex];
}

/*****************************************************************************!
 * Function : JSONInfoListInfoElementExistsByName
 *****************************************************************************/
bool
JSONInfoListInfoElementExistsByName
(JSONInfoList* InList, string InName)
{
  if ( NULL == InList || NULL == InName ) {
    return false;
  }
  for (int i = 0; i < InList->count; i++) {
    if ( StringEqual(JSONInfoGetName(InList->elements[i]), InName) ) {
      return true;
    }
  }
  return false;
}

/*****************************************************************************!
 * Function : JSONInfoListInfoElementExistsByIndex
 *****************************************************************************/
bool
JSONInfoListInfoElementExistsByIndex
(JSONInfoList* InList, int InIndex)
{
  if ( NULL == InList ) {
    return false;
  }

  if ( InIndex >= InList->count ) {
    return false;
  }
  return true;
}

/*****************************************************************************!
 * Function : JSONInfoListGetCount
 *****************************************************************************/
int
JSONInfoListGetCount
(JSONInfoList* InList)
{
  if ( NULL == InList ) {
    return 0;
  }

  return InList->count;
}  

