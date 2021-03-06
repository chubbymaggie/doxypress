/*************************************************************************
 *
 * Copyright (C) 2014-2017 Barbara Geller & Ansel Sermersheim 
 * Copyright (C) 1997-2014 by Dimitri van Heesch.
 * All rights reserved.    
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation under the terms of the GNU General Public License version 2
 * is hereby granted. No representations are made about the suitability of
 * this software for any purpose. It is provided "as is" without express or
 * implied warranty. See the GNU General Public License for more details.
 *
 * Documents produced by DoxyPress are derivative works derived from the
 * input used in their production; they are not affected by this license.
 *
*************************************************************************/

#ifndef CPPVALUE_H
#define CPPVALUE_H

#include <stdio.h>

/** A class representing a C-preprocessor value. */
class CPPValue
{
 public:
   enum Type { Int, Float };

   CPPValue(long val = 0) : type(Int) {
      v.l = val;
   }
   CPPValue(double val) : type(Float) {
      v.d = val;
   }

   operator double () const {
      return type == Int ? (double)v.l : v.d;
   }
   operator long ()   const {
      return type == Int ? v.l : (long)v.d;
   }

   bool isInt() const {
      return type == Int;
   }

   void print() const {
      if (type == Int) {
         printf("(%ld)\n", v.l);
      } else {
         printf("(%f)\n", v.d);
      }
   }

 private:
   Type type;

   union {
      double d;
      long l;
   } v;
};

extern CPPValue parseOctal();
extern CPPValue parseDecimal();
extern CPPValue parseHexadecimal();
extern CPPValue parseCharacter();
extern CPPValue parseFloat();

#endif
