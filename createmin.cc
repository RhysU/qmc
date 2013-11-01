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
  file: createmin.cc

  begin: 03/2000
  email: thomas@pollaknet.at
  web: http://qmc.pollaknet.at
*/

//Headers
#include <cqmc>

//Debug - Switch
//#define DEBUG
//#define DEBUG2

#ifdef DEBUG
  #include <iostream>
#endif

using namespace std;

/*

create simple expresion (the result)

*/
void create_result(BOOL *BO_var,
                   BOOL *lh,
                   unsigned char num_vars,
                   unsigned short high,
                   char *buffer,
                   bool optimize,
                   unsigned short int options)
{

  //local variables
  bool expr;
  unsigned char j, check=0;
  unsigned short i, tindex=0;

  #ifdef DEBUG
    cerr << "\ncreatemin.cc [create_result]: high=" << high << "; check...";

    for(i=0;i<high*num_vars;i++)
    {
      if((i+num_vars)%num_vars==0 )
      {
        cerr << "\n    " << i/num_vars << '.';
      }

      cerr << '[' << lh[i].getid() << lh[i].value() << "] ";
    }

    cerr << "\ncreatemin.cc [create_result]: finished!" << endl;
  #endif


  //rotate
  for(i=0;i<high;i++)
  {
    #ifdef DEBUG
      cerr << "\n\ncreatemin.cc [create_result]: new expression" << endl;
    #endif

    expr=0;

    for(j=0;j<num_vars;j++)
    {
      #ifdef DEBUG2
        cerr << "\ncreatemin.cc [create_result]: ID=" << lh[i*num_vars+j].getid() << "; val=" << lh[i*num_vars+j].value() << "; i " << i << ", of " << high << "; index (j) " << (unsigned short)j << ", of " << (unsigned short)num_vars;
      #endif

      //check id
      if(lh[i*num_vars+j].getid()!='-')
      {
        if(expr==0)
        {
           expr=1;
        }
        else
        {
          if(optimize)
          {
            buffer[tindex]='+';
            tindex++;
          }
          else if( !(options & AVOID_ASTERICKS) )
          {
            buffer[tindex]='*';
            tindex++;
          }
        }

        //invert if LOW
        if(lh[i*num_vars+j].value()==0)
        {
          if(!optimize)
          {
            buffer[tindex]='/';
            tindex++;
          }

          #ifdef DEBUG2
            cerr << "\ncreatemin.cc [create_result]: invertierd! ";
          #endif
        }
        else if(optimize)
        {
          buffer[tindex]='/';
          tindex++;
        }

        #ifdef DEBUG2
          cerr << " - [" << lh[i*num_vars+j].getid() << lh[i*num_vars+j].value() << "]  OK!";
        #endif

        //transfer content
        buffer[tindex]=BO_var[j].getid();
        tindex++;
        check++;

        #ifdef DEBUG2
          cerr << " - original: [" << BO_var[j].getid() << ']';
        #endif

        #ifdef DEBUG
          buffer[tindex]=0;
          cerr << "\ncreatemin.cc [create_result]: buffer=" << buffer << ";";
        #endif
      }
    }

    //use or if buffer exsists
    if(check!=0)
    {
      if(optimize)
      {
        if( !(options & AVOID_ASTERICKS) ) buffer[tindex]='*';
      }
      else
      {
        buffer[tindex]='+';
      }
      tindex++;
      check=0;
    }
  }

  #ifdef DEBUG
    cerr << "\n\ncreatemin.cc [create_result]: string created.";
  #endif

  if(high==0)
  {
    buffer[0]='0';
    buffer[1]='\0';
  }
  else if(tindex==0)
  {
    buffer[0]='1';
    buffer[1]='\0';
  }
  else
  {
    //bufferinate string
    buffer[tindex-1]='\0';
  }

  #ifdef DEBUG
    cerr << "\ncreatemin.cc [create_result]: string terminated.";
  #endif
}

