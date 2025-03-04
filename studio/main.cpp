/*
 * Kage Studio - a simple free and open source vector-based 2D animation software
 * Copyright (C) 2011~2024  Mj Mendoza IV
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

#include <gtkmm/main.h>

#include "kage.h"


std::vector<std::string> g_argv;
std::vector<std::string> argHandler(int &argc, char *argv[]) {
	int i;
	std::vector<std::string> l_argv;
	std::string l_tmp;
		for (i = 0; i < argc; ++i) {
			l_argv.push_back(argv[i]);
		}
	return l_argv;
}

int main(int argc, char *argv[]) {
	int argc1 = 1;
	auto app = Gtk::Application::create(argc1, argv, "com.krixware.kage"); //, Gio::APPLICATION_HANDLES_COMMAND_LINE);
	
	g_argv = argHandler(argc, argv);
	//App_Path = getFilePath(g_argv[0]);
	//App_EXEName = getFileName(g_argv[0]);
	std::string l_filePath = "";
	std::cout << "KAGE g_argv " << g_argv.size() << std::endl;
	if (g_argv.size() > 0) {
		if (g_argv.size() > 1) {
			l_filePath = g_argv[1];
		}
		for (unsigned int i = 0; i < g_argv.size(); ++i) {
			std::cout << " * `" << g_argv[i] << "'" << std::endl;
		}
	}
	
	Kage kage(l_filePath);
	
	//app->activate();
	
	return app->run(kage); //EXIT_SUCCESS
}
