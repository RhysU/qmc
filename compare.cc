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
  file: compare.cc - engine file

  begin: 03/2000
  email: thomas@pollaknet.at
  web: http://qmc.pollaknet.at
*/

//Headers
#include <cqmc>
#include <iostream>
//#include <vector>

using namespace std;

//Debug - switch
//#define DEBUG
//#define DEBUG2


/*

  compare (engine)

*/


//new sub function (temporary solution)
void* resize(BOOL *old_array, unsigned short int size, unsigned short int size_add)
{
  unsigned short int cnt;
  BOOL *new_array=NULL;

  new_array = new BOOL[size+size_add];

  for(cnt=0;cnt<size;cnt++) new_array[cnt] = old_array[cnt];

  return (void *)new_array;
}


void *compare(BOOL *lh, unsigned char num_vars, unsigned short &high, unsigned short int options)
{
  //objects
  BOOL *newlh=NULL, swap;
  //vector <short int> disj_expr, final, disj_temp, disj_swap;

  //local variables
  bool *check = NULL, prgm_error = 0;
  unsigned char index=0, dindex = 0, diff, h=0;
  unsigned short i=0, j=0, ready = 0, t=0;

  //iso-fix
  check = new bool[high];

  #ifdef DEBUG

    cerr << endl << endl << "compare.cc [compare]: STARTING FUNCTION";
    cerr << " num_vars=" << (unsigned short)num_vars << "; current comb's " << high;
    cerr << "; running test...";

    for(i=0;i<high;i++)
    {
      cerr << "\ncompare.cc [compare]: " << i << ". ";

      for(h=0;h<num_vars;h++)
        cerr << lh[i*num_vars+h].getid() << lh[i*num_vars+h].value();
    }

    cerr << "\ncompare.cc [compare]: finished!" << endl;

  #endif


  //Status
  verbose_msg("\nstarting recursion",options);

  for(i=0;i<high;i++)
  {
    check[i]=0;
  }

  newlh = new BOOL[num_vars];

  //compare evry term
  for(i=0;i<high-1;i++)
  {
    #ifdef DEBUG
      cerr << "\ncompare.cc [compare]: run " <<  i << " of " << high << "-1";
    #endif

    for(j=i+1;j<high;j++)
    {
      diff=0;

      for(index=0;index<num_vars;index++)
      {
        if((lh[ i * num_vars + index ].getid()=='#')&&(lh[ j * num_vars + index ].getid()=='#'))
        {
          if(lh[ i * num_vars + index ].value()!=lh[ j * num_vars + index ].value())
          {
            diff++;
            dindex = index;
          }
        }
        else if((lh[ i * num_vars + index ].getid()=='-')&&(lh[ j * num_vars + index ].getid()=='-'))
        {
          //dummy command
          diff=diff;
        }
        else
        {
          diff=2;
        }
      }

      if( diff == 1 )
      {
        #ifdef DEBUG
          cerr << "\ncompare.cc [compare]: equal values [" << i << ',' << j;
          cerr << "], ready=" << ready << "; (" << (unsigned short)dindex+1;
          cerr << "/" << (unsigned short)num_vars << ") [";

          for(h=0;h<num_vars;h++)
            cerr << lh[i*num_vars+h].getid() << lh[i*num_vars+h].value();

          cerr << "][";

          for(h=0;h<num_vars;h++)
            cerr << lh[j*num_vars+h].getid() << lh[j*num_vars+h].value();

          cerr << "]";
        #endif

        //prevent dublication (mark faked for newlh)
        swap.setid(lh[j*num_vars+dindex].getid());
        lh[j*num_vars+dindex].setid('-');

        if( !eval_dub(newlh,ready,lh,j,num_vars) )
        {
          //transfer values from lh to newlh
          for(h=0;h<num_vars;h++)
          {
            newlh[ ready * num_vars + h ].setvalue(lh[ j * num_vars + h ].value());
            newlh[ ready * num_vars + h ].setid(lh[ j * num_vars + h ].getid());

            if( lh[j*num_vars+h].getid()!='#' && lh[j*num_vars+h].getid()!='-' )  prgm_error=1;
          }

          newlh[ready*num_vars+dindex].setid('-');

          newlh = (BOOL *)resize(newlh, ++ready*num_vars, num_vars);
        }

        //(unmark)
        lh[j*num_vars+dindex].setid(swap.getid());

        check[i]=1;
        check[j]=1;
      }
    }

    if(!(i%32)) verbose_msg(".", options);
  }

  if(prgm_error)
  {
    #ifdef DEBUG
      cerr << "\n*** compare.cc [compare]: error! id failed: " << lh[j*num_vars+h].getid();
      cerr << " loop: i:" << i << " j:" << j << " high=" << high << "; ***" << endl;
    #endif

    cout << endl << "program-error in modul analyse.cc" << endl << flush;

    return((void *)0x00);
  }

  //jump back if there is no twice existing term (only one layer)
  if( ready==0 )
  {
    //Status
    verbose_msg("\nreturning...\n", options);

    return((void *)lh);
  }


  #ifdef DEBUG
    cerr << "\ncompare.cc [compare]: preparing for next run... ready=" << ready << ";" << endl;
  #endif

  //check for unchanged objects
  for(i=0;i<high;i++)
  {
    if( !check[i] )
    {
      for(h=0;h<num_vars;h++)
      {
        newlh[ready*num_vars+h].setvalue(lh[i*num_vars+h].value());
        newlh[ready*num_vars+h].setid(lh[i*num_vars+h].getid());
      }

      ready++;

      for(t=num_vars*ready;t<num_vars*(ready+1);t++)
      {
        newlh[t] = new BOOL;
      }
    }

  }

  high=ready;

  #ifdef DEBUG
    cerr << "done!";
    cerr << "\ncompare.cc [compare]: call function once again!" << endl;
  #endif

  //call the function once again
  return(compare(newlh,num_vars,high,options));
}
