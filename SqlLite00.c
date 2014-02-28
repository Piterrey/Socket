/*
 * sqlite00.c
 * This file is part of VallauriSoft
 *
 * Copyright (C) 2014 - Ferrari
 *
 * VallauriSoft is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * VallauriSoft is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with VallauriSoft; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301  USA
 */

#include <stdio.h>
#include <sqlite3.h> 

int main(int argc, char* argv[])
{
   sqlite3 *db;
   char *zErrMsg = 0;
   int rc;
   
   if(argc!=2){
   fprintf(stderr,"USAGE: %s DBNAME\n", argv[0]); 
   return(-1);
   }
   char* DBname = argv[1];
   rc = sqlite3_open(DBname, &db);

   if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return(-1);
   }
   
      fprintf(stderr, "Opened database successfully\n");

   sqlite3_close(db);
}
