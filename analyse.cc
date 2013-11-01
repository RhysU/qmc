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
  file: analyse.cc

  begin: 01/2000
  email: thomas@pollaknet.at
  web: http://qmc.pollaknet.at
*/

//Headers
#include <cqmc>

/*

analyse

*/

//DEBUG SWITCH
//#define DEBUG

#include <iostream>

using namespace std;

bool analyse(char *expression_buffer,
             unsigned char num_vars,
             BOOL *BO_var,
             unsigned char pos1,
             unsigned char pos2)
{
  //local variables
  unsigned char i,/* j,*/ chksum=0, index;
  bool inv=0;

  #ifdef DEBUG
    cerr << "\nanalyse.cc [analyse]: starting function!\n<";

    for(i=0;i<num_vars;i++)
    {
      cerr << '.' << BO_var[i].value();
    }

    cerr << ">\nTerm:  ";

    for(i=pos1;i<=pos2;i++)
    {
      cerr << expression_buffer[i];
    }
  #endif


  //one char left
  if((pos2-pos1)==0)
  {
    //search for the id

    #ifdef DEBUG
      cerr << "\nanalyse.cc [analyse]: Var -";
    #endif

    i=0;

    do
    {

      //return if equal
      if(BO_var[i].getid()==expression_buffer[pos1])
      {

        #ifdef DEBUG
          cerr << " found! " << BO_var[i].getid() << "->(" << BO_var[i].value() << ')';
        #endif

        return(BO_var[i].value());
      }
      if(expression_buffer[pos1]==SYMB_TRUE)
      {

        #ifdef DEBUG
          cerr << " constant! (1)";
        #endif

        return(1);
      }
      if(expression_buffer[pos1]==SYMB_FALSE)
      {

        #ifdef DEBUG
          cerr << " constant! (0)";
        #endif

        return(0);
      }

      i++;

    }while(i<num_vars);

    #ifdef DEBUG
      cerr << "\nanalyse.cc [analyse]: unknown variable found! (1) ->" << expression_buffer[pos1+1];
    #endif

    cout << endl << "program-error in modul analyse.cc" << endl << flush;
    return(0);
  }

  //two chars left
  if((pos2-pos1)==1)
  {

    #ifdef DEBUG
      cerr << "\nanalyse.cc [analyse]: two chars left!";
    #endif

    //check not char
    if(expression_buffer[pos1]==SYMB_NOT)
    {
      //search for the id and invert
      #ifdef DEBUG
        cerr << "\nanalyse.cc [analyse]: inverted BO_var -";
      #endif

      i=0;

      do
      {
        //return if equal
        if(BO_var[i].getid()==expression_buffer[pos1+1])
        {

          #ifdef DEBUG
            cerr << " found! (/" << BO_var[i].value() << ')';
          #endif

          return(BO_var[i].value()^1);

        }
//now done by preparser...
/*
        if(expression_buffer[pos1]==SYMB_TRUE)
        {

          #ifdef DEBUG
            cerr << " constant! (/1)";
          #endif

          return(0);

        }
        if(expression_buffer[pos1]==SYMB_FALSE)
        {

          #ifdef DEBUG
            cerr << " constant! (/0)";
          #endif

          return(1);

        }
*/
        i++;

      }while(i<num_vars);

      #ifdef DEBUG
        cerr << "\nanalyse.cc [analyse]: unknown variable found! (2) ->" << expression_buffer[pos1] << expression_buffer[pos1+1];
      #endif

      cout << endl << "program-error in modul analyse.cc" << endl << flush;
      return(0);

    }

    //AND control char
    else
    {
      #ifdef DEBUG
        cerr << "\n  analyse.cc [analyse]: * AND without operator! (2)";
      #endif

      return(analyse(expression_buffer,num_vars,BO_var,pos1,pos1)&analyse(expression_buffer,num_vars,BO_var,pos2,pos2));
    }
  }

  #ifdef DEBUG
    cerr << "\n  analyse.cc [analyse]: search for OR (+)";
  #endif


  //or section
  for(i=pos1;i<pos2;i++)
  {
    if(expression_buffer[i]==SYMB_BEGIN_SUB)
    {
      chksum++;
    }
    if(expression_buffer[i]==SYMB_END_SUB)
    {
      chksum--;
    }
    if(chksum==0)
    {

      if(expression_buffer[i]==SYMB_OR)
      {
        #ifdef DEBUG
          cerr << "\n  analyse.cc [analyse]: +fount! cut: " << (unsigned short)pos1 << ' ' << i-1 << " ][ " << i+1 << ' ' << (unsigned short)pos2;
        #endif

        return(analyse(expression_buffer,num_vars,BO_var,pos1,i-1)|analyse(expression_buffer,num_vars,BO_var,i+1,pos2));
      }
    }
  }

  chksum=0;

    //check actual char
    //brackets
    if( expression_buffer[pos1]==SYMB_BEGIN_SUB || expression_buffer[pos1+1]==SYMB_BEGIN_SUB )
    {

      #ifdef DEBUG
        cerr << "\n  analyse.cc [analyse]: brackets found!";
      #endif

      if(expression_buffer[pos1]==SYMB_NOT)
      {
        #ifdef DEBUG
          cerr << " - inverted!";
        #endif

        inv=1;
      }

      chksum++;

      for(index=pos1+1+inv;index<=pos2;index++)
      {
        if(expression_buffer[index]==SYMB_BEGIN_SUB)
        {
          chksum++;
        }
        if(expression_buffer[index]==SYMB_END_SUB)
        {
          chksum--;
        }
        if(chksum==0)
        {

          #ifdef DEBUG
            cerr << "\nanalyse.cc [analyse]: brackets OK!";
          #endif


          if(index+1<pos2)
          {
            if(expression_buffer[index+1]==SYMB_OR)
            {
              #ifdef DEBUG
                cerr << "\nanalyse.cc [analyse]: +or found! cut: ( " << (unsigned short)pos1+1 << ' ' << (unsigned short)index-1 << " ) ][ " << (unsigned short)index+2 << ' ' << (unsigned short)pos2 << "  (mit Klammer!)";
              #endif

              return((analyse(expression_buffer,num_vars,BO_var,pos1+1+inv,index-1)^inv)|analyse(expression_buffer,num_vars,BO_var,index+2,pos2));
            }
            else if(expression_buffer[index+1]==SYMB_END_SUB)
            {

              #ifdef DEBUG
                cerr << "\nanalyse.cc [analyse]: brackets closed!";
              #endif

              return(analyse(expression_buffer,num_vars,BO_var,pos1+1+inv,index-1)^inv);
            }
            else if(expression_buffer[index+1]==SYMB_NOT)
            {

              #ifdef DEBUG
                cerr << "\nanalyse.cc [analyse]: *and found! without op cut: ( " << (unsigned short)pos1+1 << ' ' << (unsigned short)index-1 << " ) ][ " << (unsigned short)index+1 << ' ' << (unsigned short)pos2 << "  (mit Klammer!)";
              #endif

              return((analyse(expression_buffer,num_vars,BO_var,pos1+1+inv,index-1)^inv)&analyse(expression_buffer,num_vars,BO_var,index+1,pos2));
            }
            else if(expression_buffer[index+1]==SYMB_BEGIN_SUB)
            {

              #ifdef DEBUG
                cerr << "\nanalyse.cc [analyse]: *and found! without op cut: ( " << (unsigned short)pos1+1 << ' ' << (unsigned short)index-1 << " ) ][ " << (unsigned short)index+1 << ' ' << (unsigned short)pos2 << "  (mit Klammer!)";
              #endif

              return((analyse(expression_buffer,num_vars,BO_var,pos1+1+inv,index-1)^inv)&analyse(expression_buffer,num_vars,BO_var,index+1,pos2));
            }
            else
            {

              #ifdef DEBUG
                cerr << "\nanalyse.cc [analyse]: *and found! without op cut: ( " << (unsigned short)pos1+1 << ' ' << (unsigned short)index-1 << " ) ][ " << (unsigned short)index+2 << ' ' << (unsigned short)pos2 << "  (mit Klammer!)";
              #endif

              return((analyse(expression_buffer,num_vars,BO_var,pos1+1+inv,index-1)^inv)&analyse(expression_buffer,num_vars,BO_var,index+2,pos2));
            }

          }
          else if(index<pos2)
          {

            #ifdef DEBUG
              cerr << "\nanalyse.cc [analyse]: *and found! without op cut: ( " << (unsigned short)pos1+1 << ' ' << (unsigned short)index-1 << " ) ][ " << (unsigned short)index+2 << ' ' << (unsigned short)pos2 << "  (mit Klammer!)";
            #endif

            return((analyse(expression_buffer,num_vars,BO_var,pos1+1+inv,index-1)^inv)&analyse(expression_buffer,num_vars,BO_var,index+1,pos2));
          }
          else
          {
            #ifdef DEBUG
              cerr << "\n  analyse.cc [analyse]: brackets closed!";
            #endif

            return(analyse(expression_buffer,num_vars,BO_var,pos1+1+inv,pos2-1)^inv);
          }
        }
      }
    }


  #ifdef DEBUG
    cerr << "\nanalyse.cc [analyse]: *and Area (End)";
  #endif

  chksum=0;

  //no mor OR left
  for(i=pos1;i<pos2;i++)
  {
    if(expression_buffer[i]==SYMB_BEGIN_SUB)
    {
      chksum++;
    }
    if(expression_buffer[i]==SYMB_END_SUB)
    {
      chksum--;
    }
    if(chksum==0)
    {
      if(expression_buffer[i]==SYMB_AND)
      {

        #ifdef DEBUG
          cerr << "\n  analyse.cc [analyse]: *and found! (standard) cut: " << (unsigned short)pos1 << ' ' << (unsigned short)i-1 << " ][ " << (unsigned short)i+1 << ' ' << (unsigned short)pos2;
        #endif

        return(analyse(expression_buffer,num_vars,BO_var,pos1,i-1)&analyse(expression_buffer,num_vars,BO_var,i+1,pos2));
      }
    }
  }

  //last posibility: AND operator without AND-char
  if( (pos2-pos1) > 1 )
  {
    #ifdef DEBUG
      cerr << "\n  analyse.cc [analyse]: *and found! last p without op cut: " << (unsigned short)pos1 << ' ' << (unsigned short)pos1 << " ][ " << (unsigned short)pos1+1 << ' ' << (unsigned short)pos2 << " ohne Operator (letzte Instantz)";
    #endif

   
    if(expression_buffer[pos1]==SYMB_NOT)
    {
      return(analyse(expression_buffer,num_vars,BO_var,pos1,pos1+1)&analyse(expression_buffer,num_vars,BO_var,pos1+2,pos2));
    }
    else
    {
      return(analyse(expression_buffer,num_vars,BO_var,pos1,pos1)&analyse(expression_buffer,num_vars,BO_var,pos1+1,pos2)); 
    }
  }

  #ifdef DEBUG
    cerr << "\n\nanalyse.cc [analyse]: error!!";
  #endif

  cout << endl << "program-error in modul analyse.cc" << endl << flush;
  return(0);
}

/*

invert array

*/

void invert_array(bool *barray, unsigned short int array_length)
{
  for(unsigned short int index=0; index < array_length; index++)
    barray[index] = !barray[index];
}
