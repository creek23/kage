/*
 * Kage Studio - a simple free and open source vector-based 2D animation software
 * Copyright (C) 2011~2022  Mj Mendoza IV
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.  Or, see <https://www.gnu.org/licenses/>.
 * 
 */
//#include "kage.h"

#include "gtkkageapp.h"
#include <iostream>

using namespace std;

vector<string> g_argv;
vector<string> argHandler(int &argc, char *argv[]) {
	int i;
	vector<string> l_argv;
	string l_tmp;
		for (i = 0; i < argc; ++i) {
			l_argv.push_back(argv[i]);
		}
	return l_argv;
}

int main(int argc, char* argv[]) {
	g_argv = argHandler(argc, argv);
	//App_Path = getFilePath(g_argv[0]);
	//App_EXEName = getFileName(g_argv[0]);
	string l_filePath = "";
	cout << "KAGE g_argv " << g_argv.size() << endl;
	if (g_argv.size() > 0) {
		if (g_argv.size() > 1) {
			l_filePath = g_argv[1];
		}
		for (unsigned int i = 0; i < g_argv.size(); ++i) {
			cout << " * `" << g_argv[i] << "'" << endl;
		}
	}
	
	auto application = GtkKageApp::create(l_filePath);

	// Start the application, showing the initial window,
	// and opening extra views for any files that it is asked to open,
	// for instance as a command-line parameter.
	// run() will return when the last window has been closed.
	return application->run();
}