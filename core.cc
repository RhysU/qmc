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
    Free Software Foundation herausgegeben, weitb_resulteben und/oder
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
  file: core.cc - (part of old main file)

  begin: 10/2003
  email: thomas@pollaknet.at
  web: http://qmc.pollaknet.at
*/

//Include
#include <cqmc>
#include <iostream>
#include <cstring>
#include <ctime>
#include <vector>

using namespace std;

/*
unsigned short pow(unsigned short x, unsigned short y)
{
  return( y>1 ? pow(x,--y)*x : x );
}
*/

unsigned short dual_pow(unsigned char x)
{
  unsigned short int result = 2;

  while( --x > 0 ) result *= 2;

  return result;
}

/*

  core (old submain)

*/
int core(char *expression_buffer, unsigned short int options)
{


  /**************
   *  variables *
   **************/

  bool *b_result = NULL, optimize=0;
  char *vars = NULL, *simple_expression_buffer = NULL;
  unsigned char num_vars, expr_length, j, check;
  unsigned short i, help, high, index, temp;

  clock_t  start_time, stop_time;

  BOOL *BO_var = NULL, *lh = NULL;

  #ifdef DEBUG
    cerr << "\ncore started";
    cerr << "\ncore.cc [core]: input_buffer [" << expression_buffer;
    cerr << "], strlen " <<strlen(expression_buffer) << endl;
  #endif

  if( options & INTERACTIVE )
  {
    //get input (string)
    cout << "enter expression: " << flush;
    cin >> expression_buffer;
  }


  /*******************
   *  check brackets *
   *******************/

  if( options & TABLE_INPUT )
  {
    cout << "\nnumber of variables: ";
    cin >> help;
    num_vars = (unsigned char)help;

    //create object arry
    BO_var = new BOOL[num_vars];

    //number of possibilities
    //help=pow((unsigned short)2,(unsigned short)num_vars);
    help = dual_pow(num_vars);

    //create boolean array
    b_result = new bool[help];

    //call input procedure
    tableinput(BO_var,num_vars,high,b_result,help);

    //strcpy(expression_buffer,"NOTHING");
  }

  //start timer
  start_time = clock();

  if( !( options & TABLE_INPUT ) )
  {
    //allocate memory for the variables
    vars = new char[MAX_NUM_VARS+1] ;

    #ifdef DEBUG
      cerr << "\ncore.cc [core]: expression_buffer=" << expression_buffer << "; (from input)";
    #endif

    //remove any blank charakter from input buffer
    remove_blanks(expression_buffer, INPUT_BUFFER_SIZE);

    #ifdef DEBUG
      cerr << "\ncore.cc [core]: expression_buffer=" << expression_buffer << "; (without blanks)";
    #endif

    //preparse the string
    preparse(expression_buffer);

    #ifdef DEBUG
      cerr << "\ncore.cc [core]: expression_buffer=" << expression_buffer << "; (parsed)";
    #endif

    if(strlen(expression_buffer)<1)
    {
      cout << "nothing to calc!" << endl;

      //program ends
      return(0);
    }

    //check the input string
    if((check=bracket_check(expression_buffer,num_vars,vars))!=0)
    {
      cout << "bad input, use the correct syntax and try again. code: ";
      cout << (unsigned short)check << endl;

      //program ends
      return(0);
    }

    //Status
    verbose_msg("\nsyntax checked...", options);

    #ifdef DEBUG
      cerr << "\ncore.cc [core]: bracket check executed";
    #endif

    //create object array
    BO_var= new BOOL[num_vars];

    #ifdef DEBUG
      cerr << " - transfering ID to boolean objects ";
    #endif

    //transfering to objects
    for(i=0;i<num_vars;i++)
    {
      BO_var[i].setid(vars[i]);

      #ifdef DEBUG
        cerr << BO_var[i].getid() << ',';
      #endif
    }

    //Status
    verbose_msg("\nvariables assigned...", options);

    /***********
     * analyse *
     ***********/

    expr_length = (unsigned char)strlen(expression_buffer);


    #ifdef DEBUG
      cerr << "\ncore.cc [core]: input_buffer[" << expression_buffer;
      cerr << "], strlen " << strlen(expression_buffer) << "; num_vars=";
      cerr << (unsigned short)num_vars <<"; expr_length=";
      cerr << (unsigned short)expr_length << ";" << endl;
    #endif


    //number of possible combinations
    //help=pow((unsigned short)2,(unsigned short)num_vars);
    help = dual_pow( num_vars );

    //create boolean array
    b_result = new bool[help];

    #ifdef DEBUG
      cerr << "\ncore.cc [core]: number of comb's=" << help << "; verfication: " << endl;
    #endif

    //set level (leayer) to zero
    high=0;

    //create output header for boolean table
    tableheadoutput(BO_var, num_vars, options);

    verbose_msg("\nstarting analyse function", options);

    //start analyse function 2^num_vars times
    //and write the result into the table
    for(i=0;i<help;i++)
    {
      #ifdef DEBUG
        cerr << "\ncore.cc [core]: transfering combination <";
      #endif

      temp=i;

      for(j=0;j<num_vars;j++)
      {
        //code inside loop should be changed
        #ifdef DEBUG
          cerr << '.';
        #endif

        //set 0,1
        BO_var[j].setvalue(temp%2);

        #ifdef DEBUG
          cerr << BO_var[j].value();
        #endif

        //create entry for boolean table
        tableoutput(BO_var[j].value(),0,options);

        temp/=2;

      }

      #ifdef DEBUG
        cerr << ">";
      #endif

      //if analyse==1 then increase high
      if((b_result[i]=analyse(expression_buffer,num_vars,BO_var,0,expr_length-1))==1)
      {
        high++;

        //create entry for boolean table (at the end)
        tableoutput(1,1,options);
      }
      else
      {
        //create entry for boolean table (at the end)
        tableoutput(0,1,options);
      }

      if( !(i%4) ) verbose_msg(".", options);

      #ifdef DEBUG
        cerr << "\ncore.cc [core]: b_result[" << i << "]=" << b_result[i];
      #endif
    }
  }


  /******************
   * optimize table *
   ******************/

  if( high+1 == help )
  {
    optimize=1;
    verbose_msg("\nrecalculating...", options);

    invert_array(b_result,help);
    high = help - high;
  }

  /*
  if( high > (help/2) )
  {
    optimize=1;
    verbose_msg("\nrecalculating...", options);

    invert_array(b_result,help);
    high = help - high;
  }
  */

  /******************
   * set LOW, HIGH  *
   ******************/

  #ifdef DEBUG
    cerr << "\ncore.cc [core]: analyse process finished!";
    cerr << "\ncore.cc [core]: number of true comb's: " << high << "; num_vars=";
    cerr << (unsigned short)num_vars << "; product=" << high*num_vars << ";" << endl;
  #endif


  //create boolean array to compare
  //lh = new BOOL[(high+1)*num_vars];
  lh = new BOOL[high*num_vars];

  index=0;

  //Status
  verbose_msg("\npreparing for comparison...", options);


  //loop for (number of possibilties)
  for(i=0;i<help;i++)
  {
    if(b_result[i]==1)
    {
      temp=i;

      for(j=0;j<num_vars;j++)
      {
        //set 1,0
        lh[index*num_vars+j].setvalue(temp%2);
        lh[index*num_vars+j].setid('#');
        temp/=2;
      }

      index++;
    }
  }


  #ifdef DEBUG
    cerr << "\ncore.cc [core]: results transfered to boolean objects - running check..." << endl;

    for(i=0; i < high; i++)
    {

      cerr << "content LH [";

      for(j=0; j < num_vars; j++) cerr << lh[j+i*num_vars].getid() << lh[j+i*num_vars].value();

      cerr << "] ";
      if ( !( (i+1)%4 ) ) cerr << endl;
    }
  #endif


  /************
   * compare *
   ************/

  //status
  verbose_msg("\nrunning comparison...\n", options);

  #ifdef DEBUG
    cerr << "\n\ncore.cc [core]: calling compare function..." << endl;
  #endif

  //start compare function
  if((lh=(BOOL *)compare(lh,num_vars,high,options))==0x00)
  {
    return(-1);
  }

  #ifdef DEBUG
    cerr << "\ncore.cc [core]: compare function finished without errors";
    cerr << "\ncore.cc [core]: creating minimized output..." << endl;
  #endif

  /****************************
   * create simple expression *
   ****************************/

  //create char array (string)
  //not sure about the correct size...
  simple_expression_buffer = new char[high*num_vars*3];

  //status
  verbose_msg("\ngenerating output string...\n", options);

  //create simple expression_buffer (string)
  create_result(BO_var,lh,num_vars,high,simple_expression_buffer, optimize,options);

  #ifdef DEBUG
    cerr << "\ncore.cc [core]: output_buffer generated - all done" << endl << endl;
  #endif

  /**********
   * Output *
   **********/

   //stop timer
  stop_time = clock();

  if( options & TIMER )
    cout << "used time: " << (double)( stop_time - start_time ) / CLOCKS_PER_SEC << "sec" << endl;

  if( options & HTML_OUTPUT)
  {
    tabletailoutput(simple_expression_buffer);
  }
  else
  {
    cout << "result: " << simple_expression_buffer << endl;
  }

  return 0;
}

