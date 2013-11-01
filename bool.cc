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

    Dieses Programm ist freie Software. Sie koennen es unter
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
  file: bool.cc - main file

  begin: 01/2000
  email: thomas@pollaknet.at
  web: http://qmc.pollaknet.at
*/

//Headers
#include <cqmc>

using namespace std;

/**********
 *        *
 *  BOOL  *
 *        *
 **********/
BOOL::BOOL(bool v, char i)
{
  val=v;         //set start value
  id=i;
}

/************
 *          *
 * SETVALUE *
 *          *
 ************/
void BOOL::setvalue(bool v)
{
  val=v;
}

/*********
 *       *
 * VALUE *
 *       *
 *********/
bool BOOL::value()
{
  return(val);
}

/*********
 *       *
 * SETID *
 *       *
 *********/
void BOOL::setid(char i)
{
  id=i;
}

/*********
 *       *
 * GETID *
 *       *
 *********/
char BOOL::getid()
{
  return(id);
}

/****************
 *              *
 * UND operator *
 *              *
 ****************/              /*
BOOL operator*(BOOL a, BOOL b)
{
  return(a.val&b.value());
}

BOOL operator*(bool a, BOOL b)
{
  return(a&b.value());
}

BOOL operator*(BOOL a, bool b)
{
  return(a.value()&b);
}

                                */
/*****************
 *               *
 * ODER operator *
 *               *
 *****************/             /*
BOOL operator+(BOOL a, BOOL b)
{
  return(a.value()|b.value());
}

BOOL operator+(bool a, BOOL b)
{
  return(a|b.value());
}

BOOL operator+(BOOL a, bool b)
{
  return(a.value()|b);
}
                                  */

/******************
 *                *
 * NICHT operator *
 *                *
 ******************/
void BOOL::invert()
{
  if(val==1)   val=0;
  else         val=1;
}
