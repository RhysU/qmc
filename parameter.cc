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

    Dieses Programm ist freie Software. Sie k”nnen es unter
    den Bedingungen der GNU General Public License, wie von der
    Free Software Foundation herausgegeben, weitergeben und/oder
    modifizieren, entweder unter Version 2 der Lizenz oder
    jeder spaeteren Version.

    Die Veroeffentlichung dieses Programms erfolgt in der
    Hoffnung, dass es Ihnen von Nutzen sein wird, aber OHNE JEDE
    GEWŽHRLEISTUNG - sogar ohne die implizite Gewaehrleistung
    der MARKTREIFE oder der EIGNUNG FUER EINEN BESTIMMTEN ZWECK.
    Details finden Sie in der GNU General Public License.

    Sie sollten eine Kopie der GNU General Public License zusammen
    mit diesem Programm erhalten haben. Falls nicht, schreiben Sie
    an die Free Software Foundation, Inc., 675 Mass Ave, Cambridge,
    MA 02139, USA.
*/

/*
  file: param.cc - parameter check

  begin: 05/2000
  email: thomas@pollaknet.at
  web: http://qmc.pollaknet.at
*/

//Headrs
#include <cqmc>
#include <iostream>
#include <cstring>


using namespace std;

/************************
 *                      *
 * parameter check *
 *                      *
 ************************/
unsigned char param(int argc, char *argv[], unsigned short int &options, char *expression_buffer)
{
  int counter;

  //some input parameters?
  if(argc)
  {
    //check each parameter
    for(counter=1; counter<=argc; counter++)
    {
      if(!strcmp(argv[counter],"--version"))
      {
        version_msg();
	return 0;
      }
      else if( !strcmp(argv[counter],"-s") || !strcmp(argv[counter],"--source") )
      {
        if(argc>counter)
        {
          //copy buffer if valid
          if(strlen(argv[++counter])<=INPUT_BUFFER_SIZE)
          {
            //equ = new char[strlen(argv[counter])+1];
            strcpy(expression_buffer, argv[counter]);
          }
          else
          {
            cout << "input buffer too big" << endl;
            return 0;
          }

          #ifdef DEBUG
            cerr << "\nimput buffer successfully transfered";
            cerr << "\nparameter.cc [main]:    expression_buffer [";
            cerr << equ << "], strlen " << strlen(expression_buffer) << endl;
          #endif

        }
        else
        {
          cout << "no input found" << endl;
          return 0;
        }
      }
      else if( !strcmp(argv[counter],"-v") || !strcmp(argv[counter],"--verbose") )
      {
        options |= PROTOCOL;
      }
      else if( !strcmp(argv[counter],"-t") || !strcmp(argv[counter],"--table") )
      {
        options |= TABLE_INPUT;
      }
      else if( !strcmp(argv[counter],"-g") || !strcmp(argv[counter],"--gettable") )
      {
        options |= TABLE_OUTPUT;
      }
      else if( !strcmp(argv[counter],"-G") || !strcmp(argv[counter],"--gethtmltable") )
      {
        options |= HTML_OUTPUT;
      }
      else if( !strcmp(argv[counter],"-p") || !strcmp(argv[counter],"--prompt") )
      {
        options |= INTERACTIVE;
      }
      else if( !strcmp(argv[counter],"-T") || !strcmp(argv[counter],"--time") )
      {
        options|= TIMER;
      }
      else if( !strcmp(argv[counter],"-a") || !strcmp(argv[counter],"--avoidast") )
      {
        options |= AVOID_ASTERICKS;
      }
      else if( !strcmp(argv[counter],"-D") || !strcmp(argv[counter],"--dummy") )
      {
        //nothing... dummy entry
      }
      else if( !strcmp(argv[counter],"-d") || !strcmp(argv[counter],"--describesyntax") )
      {
        syntax_msg();
        return 0;
      }
      else if(!strcmp(argv[counter],"-h") || !strcmp(argv[counter],"--help"))
      {
        help_msg();
        return 0;
      }
      else
      {
        usage_msg();
        return 0;
      }
    }
  }
  else
  {
    usage_msg();
    return 0;
  }

  return 1;
}


