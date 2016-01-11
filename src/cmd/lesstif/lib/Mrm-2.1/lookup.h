/**
 *
 * $Id: lookup.h,v 1.1 2004/08/28 19:22:35 dannybackx Exp $
 *
 * Copyright (C) 1995 Free Software Foundation, Inc.
 * Copyright (C) 1995-2001 LessTif Development Team
 *
 * This file is part of the GNU LessTif Library.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *
 *  Original author:  Geoffrey W. Ritchey
 *                    codesmit@southwind.net
 *
*/ 

#ifndef _LOOKUP_H
#define _LOOKUP_H

#include <Xm/Xm.h>

/* void __MrmGetArgString(int index, char **theString, char *OnCreate); */
char *__MrmLookUpFunction(char *FunctionName);
int __MrmLookUpPredefines(char *, long *);
int __MrmLookUpArgIndex(char *ArgName);
int __MrmLookUpClassIndex(char *ClassName);
char *__MrmArgNamesString(char *ArgName);
int __MrmReadBitmapFileData(char *Name, unsigned int *width, unsigned int *height, 
		   char **data, int *x_hot, int *y_hot);
void __MrmGetArgValues(int index, char **theString, char *Access);		   


typedef struct 
{
    char *Name;
    String theString;
    char Access;
} ArgNamesType;

typedef Widget (*CreateFunction) (Widget, String, ArgList, Cardinal);

typedef struct
{
    char *Name;
    CreateFunction function;
} CreateFunctionsType;

typedef struct
{
    char *Name;
    XtPointer value;
} PredefinedType;


extern ArgNamesType ArgNames[];
extern PredefinedType Predefines[];
extern int ArgSize;
extern int PreSize;

extern CreateFunctionsType CreateFunctions[];
extern int ClassSize;


enum { C = 1, G, CG, S, CS, SG, CSG };

enum {LIST_INDEX, XM_LIST_INDEX};

#endif /* _LOOKUP_H */
