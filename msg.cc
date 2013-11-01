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
    GEWAEHRLEISTUNG - sogar ohne die implizite Gewährleistung
    der MARKTREIFE oder der EIGNUNG FUER EINEN BESTIMMTEN ZWECK.
    Details finden Sie in der GNU General Public License.

    Sie sollten eine Kopie der GNU General Public License zusammen
    mit diesem Programm erhalten haben. Falls nicht, schreiben Sie
    an die Free Software Foundation, Inc., 675 Mass Ave, Cambridge,
    MA 02139, USA.
*/

/*
  file: msg.cc

  begin: 04/2000
  redesign: 10/2003
  email: thomas@pollaknet.at
  web: http://qmc.pollaknet.at
*/

//Headers
#include <cqmc>
#include <iostream>

using namespace std;

//output verion information
void version_msg(void)
{
    cout << "QMC: simplification tool (by using the Quine - McClusky process)" <<endl;
    cout << "Copyright (C) 2000  Thomas Pollak" << endl << endl;

    cout << "This program is free software; you can redistribute it and/or" << endl;
    cout << "modify it under the terms of the GNU General Public License as" << endl;
    cout << "published by the Free Software Foundation; either version 2 of" << endl;
    cout << "the License, or any later version." << endl << endl;

    cout << "This program is distributed in the hope that it will be useful," << endl;
    cout << "but WITHOUT ANY WARRANTY; without even the implied warranty of" << endl;
    cout << "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU" << endl;
    cout << "General Public License for more details." << endl << endl;

    cout << "You should have received a copy of the GNU General Public License" << endl;
    cout << "along with this program; if not, write to the Free Software" << endl;
    cout << "Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA." << endl << endl;

    cout << "Web:             " << QMC_URL << endl;
    cout << "Version:         " << QMC_VERSION << endl;
    cout << "Last update:     " << QMC_DATE << endl;
}

//output syntax information
void syntax_msg(void)
{
    cout << "input syntax:" << endl;;
    cout << "  +    logical OR    a + b" << endl;
    cout << "  *    logical AND   a * b" << endl;
    cout << "  /    logical NOT   /a" << endl;
    cout << "  1    TRUE" << endl;
    cout << "  0    FALSE" << endl;
    cout << "  []   BRACKETS      [a + b] * [c + d]" << endl;
    cout << "all other symbols are recognized as variable (blanks excluded)" << endl << endl;

    cout << "sample:" << endl;
    cout << "       qmc -s \"[a*b]+/a*[/c+d]+0*/e\"" << endl << endl;

    cout << ">in some cases the Quine-McClusky procedure enlarges the result<" << endl;
}

//output help information
void help_msg(void)
{
    cout << "Usage: qmc [OPTION]" << endl;
    cout << "Simplification tool by using the Quine - McClusky process." << endl << endl;

    cout << "  -a, --avoidast       avoid astericks at the output" << endl;
    cout << "  -d, --describesyntax descibe the syntax for the EQU" << endl;
    cout << "  -g, --gettable       get boolean table" << endl;
    cout << "  -G  --gethtmltable   get boolean table as HTML output" << endl;
    cout << "  -h, --help           display this help and exit" << endl;
    cout << "  -p, --prompt         prompt for equation" << endl;
    cout << "  -s, --source EQU     set one side of the equation" << endl;
    cout << "  -t, --table          set boolean table" << endl;
    cout << "  -T, --time           display used time" << endl;
    cout << "  -v, --verbose        print progress" << endl;
    cout << "      --version        output version information and exit" << endl << endl;

    cout << "A few samples:" << endl;
    cout << "       qmc -s \"/[a+b]*[/b*c]\"" << endl;
    cout << "       qmc -g -s \"x + //y * [ z + /z * y ] * 1\"" << endl;
    cout << "       qmc -T -s \"a*/b+c+d+e*/c+f*e\"" << endl;
    cout << "       qmc -p" << endl << endl;
    cout << "       (symbols are recognized as single char's)" << endl << endl;

    cout << "Project Home: http://qmc.pollaknet.at" << endl;
    cout << "Please report bugs to <thomas@pollaknet.at>." << endl << flush;
}

//output usage info
void usage_msg(void)
{

        cout << "qmc: too few arguments" << endl;
        cout << "Try `qmc --help' for more information." << endl << flush;
}

//universal verbose msgs
void verbose_msg(char *msg, unsigned short int options)
{
  if( options & PROTOCOL && ! ( options & HTML_OUTPUT ) )
   cout << msg << flush;
}


