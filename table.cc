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
    jeder späteren Version.

    Die Veröffentlichung dieses Programms erfolgt in der
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
  file: table.cc - handles HTML outputs

  begin: 04/2000
  email: thomas@pollaknet.at
  web: http://qmc.pollaknet.at
*/

//Include
#include <cqmc>
#include <iostream>

using namespace std;

//Debug - Switch
//#define DEBUG


void tableinput(BOOL *var, unsigned char num_vars, unsigned short &high, bool *b_result, unsigned short help)
{
  unsigned char j;
  unsigned short temp;
  high=0;

  for(j=0;j<num_vars;j++)
  {
    cout << "| " << (char)(j+65) << ' ';
    var[j].setid((char)(j+65));
  }

  cout << "| Q |" << endl;

  for(j=0;j<num_vars+1;j++)  cout << "+---";

  cout << '+' << endl;

  for(unsigned short i=0;i<help;i++)
  {
    temp=i;

    for(j=0;j<num_vars;j++)
    {
      cout << "| " << temp%2 << ' ';
      temp/=2;
    }
    cout << "| ";
    cin >> b_result[i];

    if(b_result[i]) high++;

  }

  #ifdef DEBUG
    cerr <<"\n  TABLEI. high: " << high << endl;
  #endif
}

void tableheadoutput(BOOL *var, unsigned char num_vars, unsigned short int options)
{
  unsigned char j;

  if( options & TABLE_OUTPUT)
  {
    for(j=0;j<num_vars;j++)  cout << "| " << var[j].getid() << ' ';

    cout << "| Q |" << endl;

    for(j=0;j<num_vars+1;j++)  cout << "+---";

    cout << '+' << endl;
  }
  else if( options & HTML_OUTPUT)
  {
    cout << "<HTML>" << endl << "  <HEAD>" << endl << "    <TITLE>QMC simplification tool (by using the Quine - McClusky process)</TITLE>" << endl;
    cout << "  </HEAD>" << endl << "  <BODY BGCOLOR='#ffffff' TEXT='#000000'>" << endl;

    cout << "    <BR><BR><CENTER><I>Note: file created with qmc version " << QMC_VERSION << ", built " << QMC_DATE << ".</I></CENTER><BR><BR>" << endl;

    cout << "    <TABLE BORDER='1' BORDERCOLOR='#cccccc' align='center'>" << endl << "      <TR>";

    for(j=0;j<num_vars;j++)  cout << "        <TD width='25' align='center'>" << var[j].getid() << "</TD>" << endl;

    cout << "        <TD width='25' align='center'>" << "Q" << "</TD>" << endl << "      </TR>" << endl;

  }

}

void tableoutput(bool value, bool nrow, unsigned short int options)
{

  if( options & TABLE_OUTPUT )
  {

    cout << "| " << value << " ";

    if(nrow)
    {
      cout << "|" << endl;
    }

  }
  else if( options & HTML_OUTPUT )
  {

    cout << "        <TD align='center'>" << value << "</TD>" << endl;

    if(nrow)
    {
      cout << "      </TR>" << endl;
    }
  }
}

void tabletailoutput(char *minterm)
{
  
  cout << "    </TABLE>" << endl << "    <CENTER>" << endl << "    <H3>Q=" << minterm << "</H3>" <<endl;
  cout << "    Project Home: <A HREF='http://qmc.pollaknet.at'>http://qmc.pollaknet.at</A><BR><BR>" << endl;
  cout << "    <I>Please report bugs to:<A HREF='mailto:thomas@pollaknet.at'>thomas@pollaknet.at</A></I><BR><BR><BR>" << endl;
  cout << "    </CENTER>" << endl << "  </BODY>" << endl << "</HTML>" << endl;
}

