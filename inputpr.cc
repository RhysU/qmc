/*  QMC: simplification tool (by using the Quine - McClusky process)
    Copyright (C) 2000  Thomas Pollak

    This program is free software; you can redistribute it and/or
    modify it under the expression_buffers of the GNU General Public License as
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

    QMC: Vereinfachungsprogramm fuer logische expression_buffere nach Quine - McClusky
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
  file: inputpr.cc - input processor

  begin: 01/2000
  email: thomas@pollaknet.at
  web: http://qmc.pollaknet.at
*/

//Headers
#include <cqmc>
#include <cstring>


/*

  bracket_check

*/


//Debug - Switch
//#define DEBUG

#ifdef DEBUG
  #include <iostream>
#endif

using namespace std;

//check brackets
unsigned char bracket_check(char *expression_buffer, unsigned char &num_vars, char *vars)
{

  //local variables
  char chksum = 0;
  unsigned char  counter ,i , swap=0;
  num_vars = 0 ;


  //initialize array
  for(counter=0; counter<MAX_NUM_VARS; counter++)
  {
    vars[counter]='#';
  }

  //close array with zero
  vars[counter]='\0';

  //MAINLOOP - step trough the boolean expression_buffer-string
  for(i=0;i<strlen(expression_buffer);i++)
  {

    //check current character
    if(expression_buffer[i]==SYMB_BEGIN_SUB)
    {
      chksum++;
      swap=0;
    }
    else if(expression_buffer[i]==SYMB_END_SUB)
    {
      chksum--;
    }
    else if( (expression_buffer[i]==SYMB_AND) || (expression_buffer[i]==SYMB_OR) )
    {
      if( swap )
      {
        //bad order of control char!
        return(4);
      }

      swap=1;
    }
    else if( expression_buffer[i]==SYMB_NOT )
    {
      if( swap==2 )
      {
        //bad order of NOT char!
        return(5);
      }

      swap=2;
    }
    else if( (expression_buffer[i]==SYMB_FALSE) || (expression_buffer[i]==SYMB_TRUE) )
    {
      swap=0;
    }
    else
    {
      //create new object if it isn't existing or if it isn't a control char
      //and increase the number of vars
      counter=0;

      while(vars[counter]!=expression_buffer[i])
      {
        counter++;

        if(counter>MAX_NUM_VARS)
        {
          counter=0;

          while(vars[counter]!='#')
          {
            counter++;

            //end function if the maximum of variables is reached
            if(counter>MAX_NUM_VARS)
              return(3);
          }

          vars[counter]=expression_buffer[i];
          num_vars++;
        }
      }

      swap=0;
    }

    //end function if there is a mistake with the brackets
    if(chksum<0)
    {
       // negative result of brackets!
       return(2);
    }

    #ifdef DEBUG
      cerr << "\ninputpr.cc [bracket_check]: runtime variables: " << vars << " number: " << (unsigned short)num_vars << " - run " << (unsigned short)i << " of " << strlen(expression_buffer)-1;
    #endif
  }


  //end function if not zero
  if(chksum!=0)
  {
    //result is not zero! (chksum!=0)
    return(2);
  }

  //end-check
  if(expression_buffer[strlen(expression_buffer)-1]==SYMB_AND || expression_buffer[strlen(expression_buffer)-1]==SYMB_OR || expression_buffer[strlen(expression_buffer)-1]==SYMB_NOT)
  {
    //last char inpossible
    return(6);
  }

  //end function if there is no variable
  /*if(num_vars==0)
  {
    //no variable found!
    return(1);
  }*/

  #ifdef DEBUG
    cerr << endl;
  #endif

  //send OK to the main function
  return(0);
}

/*

  remove blanks from buffer

*/
void remove_blanks(char *buffer, unsigned short int max)
{
  unsigned short int counter=0;//, new_counter=0;
 // char *new_buffer=NULL;

 // new_buffer = new char[strlen(buffer)];

  #ifdef DEBUG
    cerr << "\ninputpr.cc [remove_blanks]: buffer with blanks [" << buffer << "], strlen " << strlen(buffer);
    cerr << "\ninputpr.cc [remove_blanks]: max allowed size is " << max << endl;
  #endif


  while( buffer[counter]!=0 || counter>max )
  {
    if( buffer[counter] == BLANK_CHAR )
    {
      buffer[counter]=0;
      //if(counter+1<(unsigned short int)strlen(buffer)) buffer=strcat(buffer,&buffer[counter+1]);
      buffer=strcat(buffer,&buffer[counter+1]);
    }
    else
    {
      counter ++;
    }
  }
  /*
  while(buffer[counter]!=0 || counter>max)
  {
    if(buffer[counter++]!=BLANK_CHAR)
    {
      new_buffer[new_counter++]=buffer[counter-1];
    }
  }

  new_buffer[new_counter]=0;

  strcpy(buffer,new_buffer);
*/
  #ifdef DEBUG
    cerr << "\ninputpr.cc [remove_blanks]: buffer without blanks [" << new_buffer << "], strlen " << strlen(new_buffer);
    cerr << "\ninputpr.cc [remove_blanks]: max allowed size is " << max << endl;
  #endif

}

//preparser
void preparse(char* expression_buffer)
{
  unsigned char counter = 0;

  while( expression_buffer[counter+1] != 0 )
  {
    if( expression_buffer[counter] == '/' )
    {
      switch( expression_buffer[counter+1] )
      {
        case '/':
        expression_buffer[counter]=0;
        expression_buffer = strcat(expression_buffer,&expression_buffer[counter+2]);
        counter--;
        break;

        case '1':
        expression_buffer[counter]='0';
        expression_buffer[counter+1]=0;
        expression_buffer = strcat(expression_buffer,&expression_buffer[counter+1]);
        break;

        case '0':
        expression_buffer[counter]='1';
        expression_buffer[counter+1]=0;
        expression_buffer = strcat(expression_buffer,&expression_buffer[counter+2]);
        break;

        default:
        break;
      }
    }
    counter++;
  }


}

