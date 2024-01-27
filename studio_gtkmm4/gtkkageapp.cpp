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
 * MA 02110-1301, USA.  Or, see <https://www.gnu.org/licenses/>.
 * 
 */

#include "gtkkageapp.h"
#include "kage.h"

GtkKageApp::GtkKageApp(string p_path) : Gtk::Application("com.krixware.kage", Gio::Application::Flags::HANDLES_OPEN) {
	_path = p_path;
	Glib::set_application_name(Glib::UStringView(KageAbout::app_title));
}

Glib::RefPtr<GtkKageApp> GtkKageApp::create(string p_path) {
	return Glib::make_refptr_for_instance<GtkKageApp>(new GtkKageApp(p_path));
}

Kage* GtkKageApp::create_appwindow() {
	auto appwindow = new Kage(_path);
	
	// Make sure that the application runs for as long this window is still open.
	add_window(*appwindow);
	
	// A window can be added to an application with Gtk::Application::add_window()
	// or Gtk::Window::set_application(). When all added windows have been hidden
	// or removed, the application stops running (Gtk::Application::run() returns()),
	// unless Gio::Application::hold() has been called.
	
	// Delete the window when it is hidden.
	appwindow->signal_hide().connect(sigc::bind(sigc::mem_fun(*this,
		&GtkKageApp::on_hide_window), appwindow));

	appwindow->set_show_menubar();
	appwindow->show();
	return appwindow;
}

void GtkKageApp::on_activate() {
	// The application has been started, so let's show a window.
	auto appwindow = create_appwindow();
	appwindow->present();
}

void GtkKageApp::on_startup() {
	Gtk::Application::on_startup();

	//add_accel_group(m_refUIManager->get_accel_group());
	m_refBuilder = Gtk::Builder::create();
	set_accel_for_action("win.New", "<Primary>n");
	set_accel_for_action("win.OpenKSF", "<Primary>o");
	set_accel_for_action("win.Save", "<Primary>s");
	set_accel_for_action("win.ExportHTML5", "<Primary>5");
	set_accel_for_action("win.ExportHTML5", "<Primary>k");
	set_accel_for_action("win.Close", "<Primary>w");
	set_accel_for_action("win.Quit", "<Primary>q");
	
	set_accel_for_action("win.Duplicate", "<Primary>d");
	
	set_accel_for_action("win.LayerAdd", "<Primary>F7");//<shift>n");
	set_accel_for_action("win.LayerDelete", "<Primary>F8");
	set_accel_for_action("win.RaiseLayer", "<Primary><shift>Page_Up");
	set_accel_for_action("win.LowerLayer", "<Primary><shift>Page_Down");
	set_accel_for_action("win.RaiseToTopLayer", "<Primary><shift>Home");
	set_accel_for_action("win.LowerToBottomLayer", "<Primary><shift>End");
	
	set_accel_for_action("win.ShapeGroup", "<Primary>g");
	set_accel_for_action("win.ShapeUngroup", "<Primary><shift>g");
	set_accel_for_action("win.Raise", "Page_Up");
	set_accel_for_action("win.Lower", "Page_Down");
	set_accel_for_action("win.RaiseToTop", "Home");
	set_accel_for_action("win.LowerToBottom", "End");
	set_accel_for_action("win.FlipHorizontal", "h");
	set_accel_for_action("win.FlipVertical", "v");
	
	set_accel_for_action("win.Play", "<Primary>F9");
	set_accel_for_action("win.PlayFrame", "F9");
	set_accel_for_action("win.Stop", "<shift>F9");
	set_accel_for_action("win.NextFrame", ".");
	set_accel_for_action("win.PreviousFrame", ",");
	set_accel_for_action("win.AddFrame", "F7");
	set_accel_for_action("win.ExtendFrame", "F5");
	set_accel_for_action("win.DuplicateFrame", "F6");
	set_accel_for_action("win.RemoveFrame", "<shift>F7");
	set_accel_for_action("win.CutFrame", "<Primary><shift>x");
	set_accel_for_action("win.CopyFrame", "<Primary><shift>c");
	set_accel_for_action("win.PasteFrame", "<Primary><shift>v");
	set_accel_for_action("win.DeleteFrame", "<Primary><shift>Delete");
	set_accel_for_action("win.TweenFrame", "<Primary>t");
	set_accel_for_action("win.RemoveTweenFrame", "<Primary><shift>t");
//	set_accel_for_action("win.AddLayer", "<Primary>F7");

	set_accel_for_action("win.HelpTopic", "F1");
	set_accel_for_action("win.About", "F2");

	//Layout the actions in a menubar and toolbar:
	Glib::ustring ui_info = 
		"<interface>"
		"	<menu id='MenuBar'>"
		"		<submenu>"//FileMenu
		"			<attribute name='label' translatable='yes'>_File</attribute>"
		"			<section>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>_New</attribute>"
		"					<attribute name='action'>win.New</attribute>"
		"					<attribute name='tooltip'>Create a new file</attribute>"
		"				</item>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>_Open</attribute>"
		"					<attribute name='action'>win.OpenKSF</attribute>"
		"					<attribute name='tooltip'>Open KSF file</attribute>"
		"				</item>"
		"			</section>"
		"			<section>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>_Save</attribute>"
		"					<attribute name='action'>win.Save</attribute>"
		"					<attribute name='tooltip'>Save current file</attribute>"
		"				</item>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>Save _As</attribute>"
		"					<attribute name='action'>win.SaveAs</attribute>"
		"					<attribute name='tooltip'>Save current file As</attribute>"
		"				</item>"
		"			</section>"
		"			<section>"
		"				<submenu>"
		"				<attribute name='label' translatable='yes'>_Export...</attribute>"//ExportMenu
		"					<item>"
		"						<attribute name='label' translatable='yes'>_Export AVI</attribute>"
		"						<attribute name='action'>win.ExportAVI</attribute>"
		"						<attribute name='tooltip'>Export to AVI</attribute>"
		"					</item>"
		"					<item>"
		"						<attribute name='label' translatable='yes'>Export HTML5</attribute>"
		"						<attribute name='action'>win.ExportHTML5</attribute>"
		"						<attribute name='tooltip'>Export to HTML5</attribute>"
		"					</item>"
		"					<item>"
		"						<attribute name='label' translatable='yes'>Export SVG</attribute>"
		"					<attribute name='action'>win.ExportSVG</attribute>"
		"					<attribute name='tooltip'>Export to SVG</attribute>"
		"					</item>"
		"					<item>"
		"						<attribute name='label' translatable='yes'>Export KonsolScript</attribute>"
		"						<attribute name='action'>win.ExportKS</attribute>"
		"						<attribute name='tooltip'>Export to KonsolScript</attribute>"
		"					</item>"
		"					<submenu>"
		"					<attribute name='label' translatable='yes'>PNG...</attribute>"//ExportPNGMenu
		"						<item>"
		"							<attribute name='label' translatable='yes'>Single PNG</attribute>"//
		"					<attribute name='action'>win.ExportPNG</attribute>"
		"					<attribute name='tooltip'>Export to Single PNG</attribute>"
		"						</item>"
		"						<item>"
		"							<attribute name='label' translatable='yes'>Single PNG (Transparent)</attribute>"
		"					<attribute name='action'>win.ExportPNGTransparent</attribute>"
		"					<attribute name='tooltip'>Export with Single PNG (Transparent Background)</attribute>"
		"						</item>"
		"						<item>"
		"							<attribute name='label' translatable='yes'>Export PNG Spritesheet</attribute>"
		"							<attribute name='action'>win.ExportPNGSpritesheet</attribute>"
		"							<attribute name='tooltip'>Export PNG in Spritesheet format (Transparent Background)</attribute>"
		"						</item>"
		"						<item>"
		"							<attribute name='label' translatable='yes'>Export PNG Sequence</attribute>"
		"							<attribute name='action'>win.ExportPNGSequence</attribute>"
		"							<attribute name='tooltip'>Export PNG Sequence</attribute>"
		"						</item>"
		"					</submenu>"
		"				</submenu>"
		"			</section>"
		"			<section>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>_Close</attribute>"
		"					<attribute name='action'>win.Close</attribute>"
		"					<attribute name='tooltip'>Close file</attribute>"
		"				</item>"
		"			</section>"
		"			<section>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>_Quit</attribute>"
		"					<attribute name='action'>win.Quit</attribute>"
		"					<attribute name='tooltip'>Quit</attribute>"
		"				</item>"
		"			</section>"
		"		</submenu>"
		"		<submenu>"//EditMenu
		"			<attribute name='label' translatable='yes'>_Edit</attribute>"
		"			<section>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>_Undo</attribute>"
		"					<attribute name='action'>win.Undo</attribute>"
		"					<attribute name='accel'>&lt;Primary&gt;z</attribute>"
		"					<attribute name='tooltip'>Undo last action</attribute>"
		"				</item>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>_Redo</attribute>"
		"					<attribute name='action'>win.Redo</attribute>"
		"					<attribute name='accel'>&lt;Primary&gt;y</attribute>"
		"					<attribute name='tooltip'>Redo last action</attribute>"
		"				</item>"
		"			</section>"
		"			<section>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>Cu_t</attribute>"
		"					<attribute name='action'>win.Cut</attribute>"
		"					<attribute name='accel'>&lt;Primary&gt;x</attribute>"
		"					<attribute name='tooltip'>Cut selected shape</attribute>"
		"				</item>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>_Copy</attribute>"
		"					<attribute name='action'>win.Copy</attribute>"
		"					<attribute name='accel'>&lt;Primary&gt;c</attribute>"
		"					<attribute name='tooltip'>Copy selected shape</attribute>"
		"				</item>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>_Paste</attribute>"
		"					<attribute name='action'>win.Paste</attribute>"
		"					<attribute name='accel'>&lt;Primary&gt;v</attribute>"
		"					<attribute name='tooltip'>Paste copied shape</attribute>"
		"				</item>"
		"			</section>"
		"			<section>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>Duplic_ate</attribute>"
		"					<attribute name='action'>win.Duplicate</attribute>"
		"					<attribute name='accel'>&lt;Primary&gt;d</attribute>"
		"					<attribute name='tooltip'>Duplicate selected shape</attribute>"
		"				</item>"
		"			</section>"
		"			<section>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>_Delete</attribute>"
		"					<attribute name='action'>win.Delete</attribute>"
		"					<attribute name='accel'>Delete</attribute>"
		"					<attribute name='tooltip'>Delete selected shape</attribute>"
		"				</item>"
		"			</section>"
		"			<section>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>Select Al_l</attribute>"
		"					<attribute name='action'>win.SelectAll</attribute>"
		"					<attribute name='accel'>&lt;Primary&gt;a</attribute>"
		"					<attribute name='tooltip'>Select All</attribute>"
		"				</item>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>D_eselect</attribute>"
		"					<attribute name='action'>win.Deselect</attribute>"
		"					<attribute name='accel'>Escape</attribute>"
		"					<attribute name='tooltip'>Deselect</attribute>"
		"				</item>"
		"			</section>"
		"		</submenu>"
		"		<submenu>"//ViewMenu
		"			<attribute name='label' translatable='yes'>_View</attribute>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>Toggle _Timeline</attribute>"
		"					<attribute name='action'>win.ToggleTimeline</attribute>"
		"					<attribute name='tooltip'>Toggle Timeline</attribute>"
		"				</item>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>Toggle _Properties</attribute>"
		"					<attribute name='action'>win.ToggleProperties</attribute>"
		"					<attribute name='tooltip'>Toggle Properties</attribute>"
		"				</item>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>Toggle _Fill Properties </attribute>"
		"					<attribute name='action'>win.TogglePropertiesFill</attribute>"
		"					<attribute name='tooltip'>Toggle Fill Properties</attribute>"
		"				</item>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>Toggle _Stroke Properties</attribute>"
		"					<attribute name='action'>win.TogglePropertiesStroke</attribute>"
		"					<attribute name='tooltip'>Toggle Stroke Properties</attribute>"
		"				</item>"
		"		</submenu>"
		"		<submenu>"//LayerMenu
		"			<attribute name='label' translatable='yes'>_Layer</attribute>"
		"			<section>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>_Add Layer</attribute>"
		"					<attribute name='action'>win.LayerAdd</attribute>"
		"					<attribute name='tooltip'>Add New Layer</attribute>"
		"				</item>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>Re_name Layer</attribute>"
		"					<attribute name='action'>win.LayerRename</attribute>"
		"					<attribute name='tooltip'>Rename Current Layer</attribute>"
		"				</item>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>Delete Layer</attribute>"
		"					<attribute name='action'>win.LayerDelete</attribute>"
		"					<attribute name='tooltip'>Delete Layer</attribute>"
		"				</item>"
		"			</section>"
		"			<section>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>_Show/Hide Current Layer</attribute>"
		"					<attribute name='action'>win.ShowHideLayer</attribute>"
		"					<attribute name='tooltip'>Toggle Layer Visibility</attribute>"
		"				</item>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>_Lock/Unlock Current Layer</attribute>"
		"					<attribute name='action'>win.LockUnlockLayer</attribute>"
		"					<attribute name='tooltip'>Toggle Access to Layer</attribute>"
		"				</item>"
		"			</section>"
		"			<section>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>Raise Layer</attribute>"
		"					<attribute name='action'>win.RaiseLayer</attribute>"
		"					<attribute name='tooltip'>Raise Current Layer</attribute>"
		"				</item>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>Lower Layer</attribute>"
		"					<attribute name='action'>win.LowerLayer</attribute>"
		"					<attribute name='tooltip'>Lower Current Layer</attribute>"
		"				</item>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>Layer to Top</attribute>"
		"					<attribute name='action'>win.RaiseToTopLayer</attribute>"
		"					<attribute name='tooltip'>Raise Current Layer to Top</attribute>"
		"				</item>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>Layer to Bottom</attribute>"
		"					<attribute name='action'>win.LowerToBottomLayer</attribute>"
		"					<attribute name='tooltip'>Lower Current Layer to Bottom</attribute>"
		"				</item>"
		"			</section>"
		"		</submenu>"
		"		<submenu>"//ObjectMenu
		"			<attribute name='label' translatable='yes'>_Object</attribute>"
		"			<section>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>_Group Objects</attribute>"
		"					<attribute name='action'>win.ShapeGroup</attribute>"
		"					<attribute name='tooltip'>Group Selected Objects</attribute>"
		"				</item>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>_Ungroup Object</attribute>"
		"					<attribute name='action'>win.ShapeUngroup</attribute>"
		"					<attribute name='tooltip'>Ungroup Selected Object</attribute>"
		"				</item>"
		"			</section>"
		"			<section>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>R_aise</attribute>"
		"					<attribute name='action'>win.Raise</attribute>"
		"					<attribute name='tooltip'>Raise Selected Objects</attribute>"
		"				</item>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>_Lower</attribute>"
		"					<attribute name='action'>win.Lower</attribute>"
		"					<attribute name='tooltip'>Lower Selected Objects</attribute>"
		"				</item>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>Raise to _Top</attribute>"
		"					<attribute name='action'>win.RaiseToTop</attribute>"
		"					<attribute name='tooltip'>Lower Selected Objects to Top</attribute>"
		"				</item>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>Lower to _Bottom</attribute>"
		"					<attribute name='action'>win.LowerToBottom</attribute>"
		"					<attribute name='tooltip'>Lower Selected Objects to Bottom</attribute>"
		"				</item>"
		"			</section>"
		"			<section>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>Flip _Horizontal</attribute>"
		"					<attribute name='action'>win.FlipHorizontal</attribute>"
		"					<attribute name='tooltip'>Flip Object Horizontal</attribute>"
		"				</item>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>Flip _Vertical</attribute>"
		"					<attribute name='action'>win.FlipVertical</attribute>"
		"					<attribute name='tooltip'>Flip Object Vertical</attribute>"
		"				</item>"
		"			</section>"
		"			<section>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>Re-center Rotation Point</attribute>"
		"					<attribute name='action'>win.RecenterRotationPoint</attribute>"
		"					<attribute name='tooltip'>Re-center Object's Rotation Point</attribute>"
		"				</item>"
		"			</section>"
		"		</submenu>"
		"		<submenu>"//TimelineMenu
		"			<attribute name='label' translatable='yes'>_Timeline</attribute>"
		"			<section>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>Play</attribute>"
		"					<attribute name='action'>win.Play</attribute>"
		"					<attribute name='tooltip'>Play Animation</attribute>"
		"				</item>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>Play Frame</attribute>"
		"					<attribute name='action'>win.PlayFrame</attribute>"
		"					<attribute name='tooltip'>Play Animation from this Frame</attribute>"
		"				</item>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>Stop</attribute>"
		"					<attribute name='action'>win.Stop</attribute>"
		"					<attribute name='tooltip'>Stop Animation</attribute>"
		"				</item>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>_Next Frame</attribute>"
		"					<attribute name='action'>win.NextFrame</attribute>"
		"					<attribute name='accel'>.</attribute>"
		"					<attribute name='tooltip'>Next Frame</attribute>"
		"				</item>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>_Previous Frame</attribute>"
		"					<attribute name='action'>win.PreviousFrame</attribute>"
		"					<attribute name='accel'>,</attribute>"
		"					<attribute name='tooltip'>Previous Frame</attribute>"
		"				</item>"
		"			</section>"
		"			<section>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>_Add Frame</attribute>"
		"					<attribute name='action'>win.AddFrame</attribute>"
		"					<attribute name='tooltip'>Add Blank Frame</attribute>"
		"				</item>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>_Extend Frame</attribute>"
		"					<attribute name='action'>win.ExtendFrame</attribute>"
		"					<attribute name='tooltip'>Extend Frame</attribute>"
		"				</item>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>_Duplicate Frame</attribute>"
		"					<attribute name='action'>win.DuplicateFrame</attribute>"
		"					<attribute name='tooltip'>Duplicate Frame</attribute>"
		"				</item>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>_Remove Frame</attribute>"
		"					<attribute name='action'>win.RemoveFrame</attribute>"
		"					<attribute name='tooltip'>Remove Frame</attribute>"
		"				</item>"
		"			</section>"
		"			<section>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>Cu_t Frame</attribute>"
		"					<attribute name='action'>win.CutFrame</attribute>"
		"					<attribute name='tooltip'>Cut Frame</attribute>"
		"				</item>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>_Copy Frame</attribute>"
		"					<attribute name='action'>win.CopyFrame</attribute>"
		"					<attribute name='tooltip'>Copy Frame</attribute>"
		"				</item>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>_Paste Frame</attribute>"
		"					<attribute name='action'>win.PasteFrame</attribute>"
		"					<attribute name='tooltip'>Paste Frame</attribute>"
		"				</item>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>_Delete Frame</attribute>"
		"					<attribute name='action'>win.DeleteFrame</attribute>"
		"					<attribute name='tooltip'>Delete Frame</attribute>"
		"				</item>"
		"			</section>"
		"			<section>"
//		"				<item>"
//		"					<attribute name='label' translatable='yes'>Tween</attribute>"//Tween
//		"				</item>"
		"				<item>"
			"					<attribute name='label' translatable='yes'>_Frame Tween</attribute>"
		"					<attribute name='action'>win.TweenFrame</attribute>"
		"					<attribute name='tooltip'>Tween Frame</attribute>"
		"				</item>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>Remove Frame Tween</attribute>"
		"					<attribute name='action'>win.RemoveTweenFrame</attribute>"
		"					<attribute name='tooltip'>Remove Frame Tween</attribute>"
		"				</item>"
		"			</section>"
		"		</submenu>"
		"		<submenu>"//ToolsMenu
		"			<attribute name='label' translatable='yes'>Tools</attribute>"
		"			<section>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>_Select</attribute>"
		"					<attribute name='action'>win.Tool</attribute>"
		"					<attribute name='accel'>s</attribute>"
		"					<attribute name='target' type='i'>1</attribute>"
		"					<attribute name='tooltip'>Select Tool</attribute>"
		"				</item>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>_Node</attribute>"
		"					<attribute name='action'>win.Tool</attribute>"
		"					<attribute name='accel'>a</attribute>"
		"					<attribute name='target' type='i'>2</attribute>"
		"					<attribute name='tooltip'>Node Tool</attribute>"
		"				</item>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>_Poly</attribute>"
		"					<attribute name='action'>win.Tool</attribute>"
		"					<attribute name='accel'>p</attribute>"
		"					<attribute name='target' type='i'>3</attribute>"
		"					<attribute name='tooltip'>Poly Tool</attribute>"
		"				</item>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>Pencil</attribute>"
		"					<attribute name='action'>win.Tool</attribute>"
		"					<attribute name='accel'>b</attribute>"
		"					<attribute name='target' type='i'>4</attribute>"
		"					<attribute name='tooltip'>Pencil Tool</attribute>"
		"				</item>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>_Oval</attribute>"
		"					<attribute name='action'>win.Tool</attribute>"
		"					<attribute name='accel'>o</attribute>"
		"					<attribute name='target' type='i'>5</attribute>"
		"					<attribute name='tooltip'>Oval Tool</attribute>"
		"				</item>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>_Rectangle</attribute>"
		"					<attribute name='action'>win.Tool</attribute>"
		"					<attribute name='accel'>r</attribute>"
		"					<attribute name='target' type='i'>6</attribute>"
		"					<attribute name='tooltip'>Rectangle Tool</attribute>"
		"				</item>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>_Stroke</attribute>"
		"					<attribute name='action'>win.Tool</attribute>"
		"					<attribute name='accel'>k</attribute>"
		"					<attribute name='target' type='i'>7</attribute>"
		"					<attribute name='tooltip'>Stroke Tool</attribute>"
		"				</item>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>_Fill</attribute>"
		"					<attribute name='action'>win.Tool</attribute>"
		"					<attribute name='accel'>g</attribute>"
		"					<attribute name='target' type='i'>8</attribute>"
		"					<attribute name='tooltip'>Fill Tool</attribute>"
		"				</item>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>_Eyedrop</attribute>"
		"					<attribute name='action'>win.Tool</attribute>"
		"					<attribute name='accel'>i</attribute>"
		"					<attribute name='target' type='i'>9</attribute>"
		"					<attribute name='tooltip'>Eyedrop Tool</attribute>"
		"				</item>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>_Zoom</attribute>"
		"					<attribute name='action'>win.Tool</attribute>"
		"					<attribute name='accel'>z</attribute>"
		"					<attribute name='target' type='i'>10</attribute>"
		"					<attribute name='tooltip'>Zoom Tool</attribute>"
		"				</item>"
		"			</section>"
		"		</submenu>"
		"		<submenu>"//HelpMenu
		"			<attribute name='label' translatable='yes'>_Help</attribute>"
		"			<section>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>_Topic</attribute>"
		"					<attribute name='action'>win.HelpTopic</attribute>"
		"					<attribute name='tooltip'>Topic</attribute>"
		"				</item>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>_Website</attribute>"
		"					<attribute name='action'>win.Website</attribute>"
		"					<attribute name='tooltip'>Visit " + KageAbout::app_title + " Website</attribute>"
		"				</item>"
		"			</section>"
		"			<section>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>_Check for Updates...</attribute>"
		"					<attribute name='action'>win.CheckUpdate</attribute>"
		"					<attribute name='tooltip'>Check website for updates</attribute>"
		"				</item>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>_Debug</attribute>"
		"					<attribute name='action'>win.Debug</attribute>"
		"					<attribute name='tooltip'>Debug</attribute>"
		"				</item>"
		"				<item>"
		"					<attribute name='label' translatable='yes'>_About</attribute>"
		"					<attribute name='action'>win.About</attribute>"
		"					<attribute name='tooltip'>About " + KageAbout::app_title + "</attribute>"
		"				</item>"
		"			</section>"
		"		</submenu>"
		"	</menu>"
		"</interface>";
	
	try {
		m_refBuilder->add_from_string(ui_info);
	} catch (const Glib::Error& ex) {
		std::cerr << "Building menus failed: " << ex.what();
	}
	
	//Get the menubar and the app menu, and add them to the application:
	auto object = m_refBuilder->get_object("MenuBar");
	auto gmenu = std::dynamic_pointer_cast<Gio::Menu>(object);
	if (!gmenu) {
		g_warning("GMenu not found");
	} else {
	cout << "on_startup A  ";
		set_menubar(gmenu);
	}
	cout << "on_startup B  ";
}
void GtkKageApp::on_open(const Gio::Application::type_vec_files& files,
	const Glib::ustring& /* hint */) {
	// The application has been asked to open some files,
	// so let's open a new view for each one.
	Kage* appwindow = nullptr;
	auto windows = get_windows();
	if (windows.size() > 0)
		appwindow = dynamic_cast<Kage*>(windows[0]);
	
	if (!appwindow)
		appwindow = create_appwindow();
	
	//for (const auto& file : files)
	//	appwindow->open_file_view(file);
	
	((Kage*)appwindow)->New_onClick(); //not being called
	appwindow->present();
}

void GtkKageApp::on_hide_window(Gtk::Window* window) {
	delete window;
}