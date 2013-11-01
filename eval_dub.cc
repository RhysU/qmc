/*  QMC: simplification tool (by using the Quine - McClusky process)
    Copyright (C) 2000  Thomas Pollak

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
  

Auf Deutsch:

    QMC: Vereinfachungsprogramm fuer logische Terme nach Quine - McClusky
    Copyright (C) 2000  Thomas Pollak

    Dieses Programm ist freie Software. Sie können es unter
    den Bedingungen der GNU General Public License, wie von der
    Free Software Foundation herausgegeben, weitergeben und/oder
    modifizieren, entweder unter Version 2 der Lizenz oder
    jeder spaeteren Version.

    Die Veroeffentlichung dieses Programms erfolgt in der
    Hoffnung, dass es Ihnen von Nutzen sein wird, aber OHNE JEDE
    GEWAEHRLEISTUNG - sogar ohne die implizite Gewaehrleistung
    der MARKTREIFE oder der EIGNUNG FUER EINEN BESTIMMTEN ZWECK.
    Details finden Sie in der GNU General Public License.

    Sie sollten eine Kopie der GNU General Public License zusammen
    mit diesem Programm erhalten haben. Falls nicht, schreiben Sie
    an die Free Software Foundation, Inc., 675 Mass Ave, Cambridge,
    MA 02139, USA.
*/

/*
  file: evaldub.cc

  begin: 10/2003
  email: thomas@pollaknet.at
  web: http://qmc.pollaknet.at
*/

//Headers
#include <cqmc>
#include <iostream>

//Debug - switch
//#define DEBUG

using namespace std;

/*

 eval dublicated entries
 (replacement for remdub - prevents dublications in compare)

*/

bool eval_dub(BOOL *newlh,
              unsigned short int high,
              BOOL *lh,
              unsigned short int comp,
              unsigned char num_vars)
{
  unsigned short int i;
  unsigned char h, diff;

  for(i=0;i<high;i++)
  {
        diff=0;

        for(h=0;h<num_vars;h++)
        {
          if( newlh[i*num_vars+h].getid()==lh[comp*num_vars+h].getid() )
          {
            if( newlh[i*num_vars+h].getid()=='-' ||
                newlh[i*num_vars+h].value()==lh[comp*num_vars+h].value() )
            {
              diff++;
            }
          }

        }

        //equal element found!
        if( diff==num_vars )
        {
          return 1;
        }

   }

   return 0;
}

