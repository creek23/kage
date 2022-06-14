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

#include "kage.h"

ColorData KageStage::stageBG(255, 255, 255);
ColorData KageStage::fillColor(239, 41, 41, 255);
StrokeColorData KageStage::stroke(164, 0, 0, 255);
KageStage::ToolMode KageStage::toolMode = MODE_NONE;
GdkPoint KageStage::moveStageXY;
unsigned int PointData::debug_pts;

bool KageLayer::mouseIsDown = false;
Glib::RefPtr<Gdk::Pixbuf> KageLayer::imageVISIBLE_TRUE;
Glib::RefPtr<Gdk::Pixbuf> KageLayer::imageVISIBLE_FALSE;
Glib::RefPtr<Gdk::Pixbuf> KageLayer::imageLOCKED_TRUE;
Glib::RefPtr<Gdk::Pixbuf> KageLayer::imageLOCKED_FALSE;

Glib::RefPtr<Gdk::Pixbuf> KageStage::imageSHAPE_000;
Glib::RefPtr<Gdk::Pixbuf> KageStage::imageSHAPE_045;
Glib::RefPtr<Gdk::Pixbuf> KageStage::imageSHAPE_090;
Glib::RefPtr<Gdk::Pixbuf> KageStage::imageSHAPE_135;
Glib::RefPtr<Gdk::Pixbuf> KageStage::imageSHAPE_MOVE;
Glib::RefPtr<Gdk::Pixbuf> KageStage::imageSHAPE_NE;
Glib::RefPtr<Gdk::Pixbuf> KageStage::imageSHAPE_NW;
Glib::RefPtr<Gdk::Pixbuf> KageStage::imageSHAPE_SW;
Glib::RefPtr<Gdk::Pixbuf> KageStage::imageSHAPE_SE;
Glib::RefPtr<Gdk::Pixbuf> KageStage::imageSHAPE_ROTATE;
bool KageFrame::_gotFocus;

unsigned int VectorDataManager::idmaker;

unsigned int Kage::TAB_COUNT = 0;

Kage::Kage(string p_filePath) :
			   _layerManager(this),
			   _framesetManager(this),
			   m_LabelProp("Properties"),
			   m_LabelStageWid("Width"),
			   m_LabelStageHgt("Height"),
			   m_LabelStageBG("Background"),
			   m_LabelStageFPS("FPS"),
			   m_ButtonFill("Fill"),
			   m_LabelFillR("Red"),
			   m_LabelFillG("Green"),
			   m_LabelFillB("Blue"),
			   m_LabelFillAlpha("Alpha"),
			   m_LabelFillRGBA("RGBA"),
			   m_ButtonStroke("Stroke"),
			   m_LabelStrokeR("Red"),
			   m_LabelStrokeG("Green"),
			   m_LabelStrokeB("Blue"),
			   m_LabelStrokeAlpha("Alpha"),
			   m_LabelStrokeRGBA("RGBA"),
			   m_LabelStrokeThickness("Thickness"),
			   m_LabelX("X"),
			   m_LabelY("Y"),
			   m_LabelWidth("Width"),
			   m_LabelHeight("Height"),
			   m_LabelNodeX("X"),
			   m_LabelNodeY("Y"),
			   m_LabelToggleLine("Straighten"),
			   m_LblHolder_Toolbar("Toolbar"),
			   m_LabelTweenX("Tween X"),
			   m_LabelTweenY("Tween Y"),
			   m_KageStage(this),
			   _adjustFillR( Gtk::Adjustment::create(0.0, 0.0, 256.0, 0.1, 1.0, 1.0) ),
			   _adjustFillG( Gtk::Adjustment::create(0.0, 0.0, 256.0, 0.1, 1.0, 1.0) ),
			   _adjustFillB( Gtk::Adjustment::create(0.0, 0.0, 256.0, 0.1, 1.0, 1.0) ),
			   _adjustFillA( Gtk::Adjustment::create(0.0, 0.0, 101.0, 0.1, 1.0, 1.0) ),
			   _scaleFillR(_adjustFillR, Gtk::ORIENTATION_HORIZONTAL),
			   _scaleFillG(_adjustFillG, Gtk::ORIENTATION_HORIZONTAL),
			   _scaleFillB(_adjustFillB, Gtk::ORIENTATION_HORIZONTAL),
			   _scaleFillAplha(_adjustFillA, Gtk::ORIENTATION_HORIZONTAL),
			   _adjustStrokeR( Gtk::Adjustment::create(0.0, 0.0, 256.0, 0.1, 1.0, 1.0) ),
			   _adjustStrokeG( Gtk::Adjustment::create(0.0, 0.0, 256.0, 0.1, 1.0, 1.0) ),
			   _adjustStrokeB( Gtk::Adjustment::create(0.0, 0.0, 256.0, 0.1, 1.0, 1.0) ),
			   _adjustStrokeA( Gtk::Adjustment::create(0.0, 0.0, 101.0, 0.1, 1.0, 1.0) ),
			   _scaleStrokeR(_adjustStrokeR, Gtk::ORIENTATION_HORIZONTAL),
			   _scaleStrokeG(_adjustStrokeG, Gtk::ORIENTATION_HORIZONTAL),
			   _scaleStrokeB(_adjustStrokeB, Gtk::ORIENTATION_HORIZONTAL),
			   _scaleStrokeAlpha(_adjustStrokeA, Gtk::ORIENTATION_HORIZONTAL),
			   _undoRedoManager() {
	m_ContextId = m_Statusbar.get_context_id(KageAbout::app_title);
	KageFrame::_gotFocus = false;
	set_title(KageAbout::app_title);
	set_icon(Gdk::Pixbuf::create_from_resource("/kage/share/icons/default.png"));
	set_border_width(0);
	
	//workaround for known bug about resizing/maximizing main window with gtk3.22-9
	set_default_size(1920,1080);
	maximize();
	//Define the actions:
	m_refActionGroup = Gtk::ActionGroup::create("Actions");
	
	m_refActionGroup->add( Gtk::Action::create("FileMenu", "_File") ); 
	m_refActionGroup->add( Gtk::Action::create("ExportMenu", "_Export...") ); 
	m_refActionGroup->add( Gtk::Action::create("ExportPNGMenu", "_PNG...") ); 
	m_refActionGroup->add( Gtk::Action::create("EditMenu", "_Edit") );
	m_refActionGroup->add( Gtk::Action::create("ViewMenu", "_View") );
	m_refActionGroup->add( Gtk::Action::create("LayerMenu", "_Layer") );
	m_refActionGroup->add( Gtk::Action::create("ObjectMenu", "_Object") );
	m_refActionGroup->add( Gtk::Action::create("TimelineMenu", "_Timeline") );
	m_refActionGroup->add( Gtk::Action::create("ToolsMenu", "T_ools") );
	m_refActionGroup->add( Gtk::Action::create("HelpMenu", "_Help") );
	m_refActionGroup->add(
		Gtk::Action::create("New", Gtk::Stock::NEW, "_New", "Create a new file"),
		sigc::mem_fun(*this, &Kage::New_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("OpenKSF", Gtk::Stock::OPEN, "_Open", "Open KSF file"),
		sigc::mem_fun(*this, &Kage::OpenKSF_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("Save", Gtk::Stock::SAVE, "_Save", "Save current file"),
		sigc::mem_fun(*this, &Kage::Save_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("SaveAs", "Save _As", "Save current file As"),
		sigc::mem_fun(*this, &Kage::SaveAs_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("ExportKS", "Export KonsolScript", "Export to KS"),
		Gtk::AccelKey("<control>K"),
		sigc::mem_fun(*this, &Kage::ExportKS_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("ExportHTML5", "Export HTML5", "Export to HTML5"),
		Gtk::AccelKey("<control>5"),
		sigc::mem_fun(*this, &Kage::ExportHTML5_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("ExportSVG", "Export SVG", "Export to SVG"),
		sigc::mem_fun(*this, &Kage::ExportSVG_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("ExportPNG", "Single PNG", "Export to Single PNG"),
//		Gtk::AccelKey("<control>5"),
		sigc::mem_fun(*this, &Kage::ExportPNG_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("ExportPNGTransparent", "Single PNG (Transparent)", "Export with Single PNG (Transparent Background)"),
//		Gtk::AccelKey("<control>5"),
		sigc::mem_fun(*this, &Kage::ExportPNGTransparent_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("ExportPNGSequence", "PNG Sequence", "Export PNG Sequence"),
//		Gtk::AccelKey("<control>5"),
		sigc::mem_fun(*this, &Kage::ExportPNGSequence_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("ExportAVI", "Export AVI", "Export to AVI"),
//		Gtk::AccelKey("<control>5"),
		sigc::mem_fun(*this, &Kage::ExportAVI_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("Close", Gtk::Stock::CLOSE, "_Close", "Close file"),
		sigc::mem_fun(*this, &Kage::New_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("Quit", Gtk::Stock::QUIT, "_Quit", "Quit"),
		sigc::mem_fun(*this, &Kage::Quit_onClick)
	);
	
	m_refActionGroup->add(
		Gtk::Action::create("Undo", "_Undo", "Undo"),
		Gtk::AccelKey("<control>Z"),
		sigc::mem_fun(*this, &Kage::Undo_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("Redo", "_Redo", "Redo"),
		Gtk::AccelKey("<control>Y"),
		sigc::mem_fun(*this, &Kage::Redo_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("Cut", "Cu_t", "Cut"),
		//Gtk::AccelKey("<control>X"),
		sigc::mem_fun(*this, &Kage::Cut_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("Copy", "_Copy", "Copy"),
		//Gtk::AccelKey("<control>C"),
		sigc::mem_fun(*this, &Kage::Copy_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("Paste", "_Paste", "Paste"),
		//Gtk::AccelKey("<control>V"),
		sigc::mem_fun(*this, &Kage::Paste_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("Delete", "_Delete", "Delete"),
		//Gtk::AccelKey("Delete"),
		sigc::mem_fun(*this, &Kage::Delete_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("Duplicate", "Duplic_ate", "Duplicate"),
		Gtk::AccelKey("<control>D"),
		sigc::mem_fun(*this, &Kage::Duplicate_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("SelectAll", "Select Al_l", "Select All"),
		//Gtk::AccelKey("<control>A"),
		sigc::mem_fun(*this, &Kage::SelectAll_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("Deselect", "D_eselect", "Deselect"),
		//Gtk::AccelKey("Escape"),
		sigc::mem_fun(*this, &Kage::Deselect_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("ToggleTimeline", "Toggle _Timeline", "ToggleTimeline"),
//		Gtk::AccelKey("Escape"),
		sigc::mem_fun(*this, &Kage::ToggleTimeline_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("ToggleProperties", "Toggle _Properties", "ToggleProperties"),
//		Gtk::AccelKey("Escape"),
		sigc::mem_fun(*this, &Kage::ToggleProperties_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("TogglePropertiesFill", "Toggle _Fill Properties", "TogglePropertiesFill"),
//		Gtk::AccelKey("Escape"),
		sigc::mem_fun(*this, &Kage::TogglePropertiesFill_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("TogglePropertiesStroke", "Toggle _Stroke Properties", "TogglePropertiesStroke"),
//		Gtk::AccelKey("Escape"),
		sigc::mem_fun(*this, &Kage::TogglePropertiesStroke_onClick)
	);
	//==================================================================
	m_refActionGroup->add(
		Gtk::Action::create("LayerAdd", "_Add Layer", "Add New Layer"),
		Gtk::AccelKey("<shift><control>N"),
		sigc::mem_fun(*this, &Kage::LayerAdd_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("LayerRename", "Re_name Layer", "Rename Current Layer"),
		Gtk::AccelKey("F2"),
		sigc::mem_fun(*this, &Kage::LayerRename_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("ShowHideLayer", "_Show/Hide Current Layer", "Toggle Layer Visibility"),
//		Gtk::AccelKey("F2"),
		sigc::mem_fun(*this, &Kage::ShowHideLayer_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("LockUnlockLayer", "_Lock/Unlock Current Layer", "Toggle Access to Layer"),
//		Gtk::AccelKey("F2"),
		sigc::mem_fun(*this, &Kage::LockUnlockLayer_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("RaiseLayer", "Raise Layer", "Raise Current Layer"),
		Gtk::AccelKey("<shift><control>Page_Up"),
		sigc::mem_fun(*this, &Kage::LayerMoveUp_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("LowerLayer", "Lower Layer", "Lower Current Layer"),
		Gtk::AccelKey("<shift><control>Page_Down"),
		sigc::mem_fun(*this, &Kage::LayerMoveDown_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("RaiseToTopLayer", "Layer to Top", "Raise Current Layer to Top"),
		Gtk::AccelKey("<shift><control>Home"),
		sigc::mem_fun(*this, &Kage::LayerMoveTop_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("LowerToBottomLayer", "Layer to Bottom", "Lower Current Layer to Bottom"),
		Gtk::AccelKey("<shift><control>End"),
		sigc::mem_fun(*this, &Kage::LayerMoveBottom_onClick)
	);
	//==================================================================
	//==================================================================
	m_refActionGroup->add(
		Gtk::Action::create("ShapeGroup", "_Group Objects", "Group Selected Objects"),
		Gtk::AccelKey("<control>G"),
		sigc::mem_fun(*this, &Kage::ShapeGroup_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("ShapeUngroup", "_Ungroup Object", "Ungroup Selected Object"),
		Gtk::AccelKey("<shift><control>G"),
		sigc::mem_fun(*this, &Kage::ShapeUngroup_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("Raise", "R_aise", "Raise"),
		Gtk::AccelKey("Page_Up"),
		sigc::mem_fun(*this, &Kage::Raise_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("Lower", "_Lower", "Lower"),
		Gtk::AccelKey("Page_Down"),
		sigc::mem_fun(*this, &Kage::Lower_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("RaiseToTop", "Raise to _Top", "RaiseToTop"),
		Gtk::AccelKey("Home"),
		sigc::mem_fun(*this, &Kage::RaiseToTop_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("LowerToBottom", "Lower to _Bottom", "LowerToBottom"),
		Gtk::AccelKey("End"),
		sigc::mem_fun(*this, &Kage::LowerToBottom_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("FlipHorizontal", "Flip _Horizontal", "FlipHorizontal"),
		Gtk::AccelKey("H"),
		sigc::mem_fun(*this, &Kage::FlipHorizontal_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("FlipVertical", "Flip _Vertical", "FlipVertical"),
		Gtk::AccelKey("V"),
		sigc::mem_fun(*this, &Kage::FlipVertical_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("RecenterRotationPoint", "Re-center Rotation Point", "RecenterRotationPoint"),
		sigc::mem_fun(*this, &Kage::RecenterRotationPoint_onClick)
	);
	
	m_refActionGroup->add(
		Gtk::Action::create("Play", "Play", "Play Animation"),
		Gtk::AccelKey("F9"),
		sigc::mem_fun(*this, &Kage::Play_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("PlayFrame", "Play Frame", "Play Animation from this Frame"),
		Gtk::AccelKey("<control>F9"),
		sigc::mem_fun(*this, &Kage::PlayFrame_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("Stop", "Stop", "Stop Animation"),
		Gtk::AccelKey("<shift>F9"),
		sigc::mem_fun(*this, &Kage::Stop_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("PreviousFrame", "_Previous", "Previous Frame"),
		Gtk::AccelKey(","),
		sigc::mem_fun(*this, &Kage::switchToPreviousFrame)
	);
	m_refActionGroup->add(
		Gtk::Action::create("NextFrame", "_Next", "Next Frame"),
		Gtk::AccelKey("."),
		sigc::mem_fun(*this, &Kage::switchToNextFrame)
	);
	m_refActionGroup->add(
		Gtk::Action::create("AddFrame", "_Add Frame", "Add Blank Frame"),
		Gtk::AccelKey("F7"),
		sigc::mem_fun(*this, &Kage::AddFrame_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("ExtendFrame", "_Extend Frame", "Extend Frame"),
		Gtk::AccelKey("F5"),
		sigc::mem_fun(*this, &Kage::ExtendFrame_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("DuplicateFrame", "_Duplicate Frame", "Duplicate Frame"),
		Gtk::AccelKey("F6"),
		sigc::mem_fun(*this, &Kage::DuplicateFrame_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("RemoveFrame", "_Remove Frame", "Remove Frame"),
		Gtk::AccelKey("<shift>F7"),
		sigc::mem_fun(*this, &Kage::RemoveFrame_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("CutFrame", "Cu_t Frame", "Cut Frame"),
		Gtk::AccelKey("<control><shift>X"),
		sigc::mem_fun(*this, &Kage::CutFrame_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("CopyFrame", "_Copy Frame", "Copy Frame"),
		Gtk::AccelKey("<control><shift>C"),
		sigc::mem_fun(*this, &Kage::CopyFrame_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("PasteFrame", "_Paste Frame", "Paste Frame"),
		Gtk::AccelKey("<control><shift>V"),
		sigc::mem_fun(*this, &Kage::PasteFrame_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("DeleteFrame", "_Delete Frame", "Delete Frame"),
		Gtk::AccelKey("<control><shift>Delete"),
		sigc::mem_fun(*this, &Kage::DeleteFrame_onClick)
	);
	/*m_refActionGroup->add(
		Gtk::ToggleAction::create("Tween", "Tween", "Tween", false),
		Gtk::AccelKey("<control>T"),
		sigc::mem_fun(*this, &Kage::Tween_onClick)
	);*/
	m_refActionGroup->add(
		Gtk::Action::create("TweenFrame", "_Frame Tween", "Tween Frame"),
		Gtk::AccelKey("<control>T"),
		sigc::mem_fun(*this, &Kage::TweenFrame_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("RemoveTweenFrame", "Remove Frame Tween", "Remove Tween Frame"),
		Gtk::AccelKey("<control><shift>T"),
		sigc::mem_fun(*this, &Kage::RemoveTweenFrame_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("AddLayer", "_Add Layer", "Add Layer"),
		Gtk::AccelKey("<control>F7"),
		sigc::mem_fun(*this, &Kage::LayerAdd_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("DeleteLayer", "_Delete Layer", "Delete Layer"),
		Gtk::AccelKey("<control>F8"),
		sigc::mem_fun(*this, &Kage::LayerDel_onClick)
	);
	Gtk::RadioAction::Group group_tools;
	m_refActionGroup->add(
		Gtk::RadioAction::create(group_tools, "ToolSelect", "_Select", "Select Tool"),
		Gtk::AccelKey("S"),
		sigc::mem_fun(*this, &Kage::ToolSelect_onClick)
	);
	m_refActionGroup->add(
		Gtk::RadioAction::create(group_tools, "ToolNode", "_Node", "Node Tool"),
		Gtk::AccelKey("A"),
		sigc::mem_fun(*this, &Kage::ToolNode_onClick)
	);
	m_refActionGroup->add(
		Gtk::RadioAction::create(group_tools, "ToolPoly", "_Poly", "Poly Tool"),
		Gtk::AccelKey("P"),
		sigc::mem_fun(*this, &Kage::ToolPoly_onClick)
	);
	m_refActionGroup->add(
		Gtk::RadioAction::create(group_tools, "ToolPencil", "Pencil", "Pencil Tool"),
		Gtk::AccelKey("B"),
		sigc::mem_fun(*this, &Kage::ToolPencil_onClick)
	);
	m_refActionGroup->add(
		Gtk::RadioAction::create(group_tools, "ToolOval", "_Oval", "Oval Tool"),
		Gtk::AccelKey("O"),
		sigc::mem_fun(*this, &Kage::ToolOval_onClick)
	);
	m_refActionGroup->add(
		Gtk::RadioAction::create(group_tools, "ToolRectangle", "_Rectangle", "Rectangle Tool"),
		Gtk::AccelKey("R"),
		sigc::mem_fun(*this, &Kage::ToolRectangle_onClick)
	);
	m_refActionGroup->add(
		Gtk::RadioAction::create(group_tools, "ToolStroke", "_Stroke", "Stroke Tool"),
		Gtk::AccelKey("K"),
		sigc::mem_fun(*this, &Kage::ToolStroke_onClick)
	);
	m_refActionGroup->add(
		Gtk::RadioAction::create(group_tools, "ToolFill", "_Fill", "Fill Tool"),
		Gtk::AccelKey("G"),
		sigc::mem_fun(*this, &Kage::ToolFill_onClick)
	);
	m_refActionGroup->add(
		Gtk::RadioAction::create(group_tools, "ToolEyedrop", "_Eyedrop", "Eyedrop Tool"),
		Gtk::AccelKey("I"),
		sigc::mem_fun(*this, &Kage::ToolEyedrop_onClick)
	);
	m_refActionGroup->add(
		Gtk::RadioAction::create(group_tools, "ToolZoom", "_Zoom", "Zoom Tool"),
		Gtk::AccelKey("Z"),
		sigc::mem_fun(*this, &Kage::ToolZoom_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("HelpTopic", "_Topic", "Topic"),
		Gtk::AccelKey("F1"),
		sigc::mem_fun(*this, &Kage::onActionActivate)
	);
	m_refActionGroup->add(
		Gtk::Action::create("Website", "_Website", "Visit " + KageAbout::app_title + " Website"),
		sigc::mem_fun(*this, &Kage::Website_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("CheckUpdate", "_Check for Updates...", "Check website for updates"),
		sigc::mem_fun(*this, &Kage::CheckUpdate_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("Debug", "_Debug", "Debug"),
		sigc::mem_fun(*this, &Kage::btnDebug_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("About", "_About", "About " + KageAbout::app_title),
		Gtk::AccelKey("F2"),
		sigc::mem_fun(*this, &Kage::btnAbout_onClick)
	);
	
	m_refUIManager = Gtk::UIManager::create();
	m_refUIManager->insert_action_group(m_refActionGroup);
	add_accel_group(m_refUIManager->get_accel_group());
	
	//Layout the actions in a menubar and toolbar:
	Glib::ustring ui_info = 
		"<ui>"
		"	<menubar name='MenuBar'>"
		"		<menu action='FileMenu'>"
		"			<menuitem action='New'/>"
		"			<menuitem action='OpenKSF'/>"
		"			<separator/>"
		"			<menuitem action='Save'/>"
		"			<menuitem action='SaveAs'/>"
		"			<separator/>"
		"			<menu action='ExportMenu'>"
		"				<menuitem action='ExportAVI'/>"
		"				<menuitem action='ExportHTML5'/>"
		"				<menuitem action='ExportSVG'/>"
		"				<menuitem action='ExportKS'/>"
		"				<menu action='ExportPNGMenu'>"
		"					<menuitem action='ExportPNG'/>"
		"					<menuitem action='ExportPNGTransparent'/>"
		"					<menuitem action='ExportPNGSequence'/>"
		"				</menu>"
		"			</menu>"
		"			<separator/>"
		"			<menuitem action='Close'/>"
		"			<separator/>"
		"			<menuitem action='Quit'/>"
		"		</menu>"
		"		<menu action='EditMenu'>"
		"			<menuitem action='Undo'/>"
		"			<menuitem action='Redo'/>"
		"			<separator/>"
		"			<menuitem action='Cut'/>"
		"			<menuitem action='Copy'/>"
		"			<menuitem action='Paste'/>"
		"			<separator/>"
		"			<menuitem action='Duplicate'/>"
		"			<separator/>"
		"			<menuitem action='Delete'/>"
		"			<separator/>"
		"			<menuitem action='SelectAll'/>"
		"			<menuitem action='Deselect'/>"
		"		</menu>"
		"		<menu action='ViewMenu'>"
		"			<menuitem action='ToggleTimeline'/>"
		"			<menuitem action='ToggleProperties'/>"
		"			<menuitem action='TogglePropertiesFill'/>"
		"			<menuitem action='TogglePropertiesStroke'/>"
		"		</menu>"
		"		<menu action='LayerMenu'>"
		"			<menuitem action='LayerAdd'/>"
		"			<menuitem action='LayerRename'/>"
		"			<separator/>"
		"			<menuitem action='ShowHideLayer'/>"
		"			<menuitem action='LockUnlockLayer'/>"
		"			<separator/>"
		"			<menuitem action='RaiseLayer'/>"
		"			<menuitem action='LowerLayer'/>"
		"			<menuitem action='RaiseToTopLayer'/>"
		"			<menuitem action='LowerToBottomLayer'/>"
		"		</menu>"
		"		<menu action='ObjectMenu'>"
		"			<menuitem action='ShapeGroup'/>"
		"			<menuitem action='ShapeUngroup'/>"
		"			<separator/>"
		"			<menuitem action='Raise'/>"
		"			<menuitem action='Lower'/>"
		"			<menuitem action='RaiseToTop'/>"
		"			<menuitem action='LowerToBottom'/>"
		"			<separator/>"
		"			<menuitem action='FlipHorizontal'/>"
		"			<menuitem action='FlipVertical'/>"
		"			<separator/>"
		"			<menuitem action='RecenterRotationPoint'/>"
		"		</menu>"
		"		<menu action='TimelineMenu'>"
		"			<menuitem action='Play'/>"
		"			<menuitem action='PlayFrame'/>"
		"			<menuitem action='Stop'/>"
		"			<menuitem action='NextFrame'/>"
		"			<menuitem action='PreviousFrame'/>"
		"			<separator/>"
		"			<menuitem action='AddFrame'/>"
		"			<menuitem action='ExtendFrame'/>"
		"			<menuitem action='DuplicateFrame'/>"
		"			<menuitem action='RemoveFrame'/>"
		"			<separator/>"
		"			<menuitem action='CutFrame'/>"
		"			<menuitem action='CopyFrame'/>"
		"			<menuitem action='PasteFrame'/>"
		"			<menuitem action='DeleteFrame'/>"
		"			<separator/>"
//		"			<menuitem action='Tween'/>"
		"			<menuitem action='TweenFrame'/>"
		"			<menuitem action='RemoveTweenFrame'/>"
		"			<separator/>"
		"			<menuitem action='AddLayer'/>"
		"			<menuitem action='DeleteLayer'/>"
		"		</menu>"
		"		<menu action='ToolsMenu'>"
		"			<menuitem action='ToolSelect'/>"
		"			<menuitem action='ToolNode'/>"
		"			<menuitem action='ToolPoly'/>"
		"			<menuitem action='ToolPencil'/>"
		"			<menuitem action='ToolOval'/>"
		"			<menuitem action='ToolRectangle'/>"
		"			<menuitem action='ToolStroke'/>"
		"			<menuitem action='ToolFill'/>"
		"			<menuitem action='ToolEyedrop'/>"
		"			<menuitem action='ToolZoom'/>"
		"		</menu>"
		"		<menu action='HelpMenu'>"
		"			<menuitem action='HelpTopic'/>"
		"			<menuitem action='Website'/>"
		"			<separator/>"
		"			<menuitem action='CheckUpdate'/>"
		"			<menuitem action='Debug'/>"
		"			<menuitem action='About'/>"
		"		</menu>"
		"	</menubar>"
		"</ui>";
	
	try {
		m_refUIManager->add_ui_from_string(ui_info);
	} catch(const Glib::Error& ex) {
		std::cerr << "building menus failed: " <<  ex.what();
	}
	
	
	add(m_VBoxRoot);
	Gtk::Widget* pMenuBar = m_refUIManager->get_widget("/MenuBar") ;
	m_VBoxRoot.pack_start(*pMenuBar, Gtk::PACK_SHRINK);
	m_VBoxRoot.pack_start(m_HBoxToolbar, Gtk::PACK_EXPAND_WIDGET);
	m_VBoxRoot.pack_start(m_Statusbar, Gtk::PACK_SHRINK);
		m_Statusbar.pack_end(_labelStatusMouseXY, Gtk::PACK_SHRINK);
	m_HBoxToolbar.pack_start(m_VBoxToolbar_Holder, Gtk::PACK_SHRINK);
		m_VBoxToolbar_Holder.pack_start(m_LblHolder_Toolbar, Gtk::PACK_SHRINK);
		m_VBoxToolbar_Holder.pack_start(m_Separator_Toolbar1, Gtk::PACK_SHRINK);
		
		addToolButton("Select");
		addToolButton("Node");
//		addToolButton("Marquee"); //TODO: implement tool
		m_VBoxToolbar_Holder.pack_start(m_Separator_Toolbar2, Gtk::PACK_SHRINK);
//		addToolButton("Text"); //TODO: implement tool
		addToolButton("Poly");
		addToolButton("Pencil");
		addToolButton("Oval");
		addToolButton("Rectangle");
		m_VBoxToolbar_Holder.pack_start(m_Separator_Toolbar3, Gtk::PACK_SHRINK);
		addToolButton("Stroke");
		addToolButton("Fill");
		addToolButton("Eyedrop"); //TODO: use gtk-color-picker
//		addToolButton("Eraser"); //TODO: implement tool
//		m_VBoxToolbar_Holder.pack_start(m_Separator_Toolbar4, Gtk::PACK_SHRINK);
		addToolButton("Zoom"); //TODO: use gtk-zoom-in
		
	m_HBoxToolbar.pack_start(m_VPane_Timeline, Gtk::PACK_EXPAND_WIDGET);
		m_VPane_Timeline.add1(m_Timeline_HBox);
			m_Timeline_HBox.pack_start(m_Timeline_HPaned);
				m_Timeline_HPaned.set_size_request(100, 100);
				m_Timeline_HPaned.add1(m_Timeline_Layer_VBox);
					m_Timeline_Layer_VBox.pack_start(m_Timeline_Layer_ScrolledWindow);//, Gtk::PACK_SHRINK);
						m_Timeline_Layer_ScrolledWindow.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_EXTERNAL);
							m_Timeline_Layer_ScrolledWindow.add(_layerManager);
							m_Timeline_Layer_ScrolledWindow.set_border_width(0);
							m_Timeline_Layer_ScrolledWindow.set_shadow_type(Gtk::SHADOW_NONE);
							cout << "layer count " << _layerManager.layerCount() << endl;
					m_Timeline_Layer_VBox.pack_start(m_Timeline_Layer_Add_HBox, Gtk::PACK_SHRINK);
						m_Timeline_Layer_Add_HBox.pack_start(m_Timeline_Label, Gtk::PACK_EXPAND_WIDGET);
							m_Timeline_Label.set_label("  Timeline  ");
						m_Timeline_Layer_Add_HBox.pack_start(m_Timeline_Add_Button, Gtk::PACK_SHRINK);
							m_Timeline_Add_Button_pixbuf = Gdk::Pixbuf::create_from_resource("/kage/share/layer/layer_add.png");
								m_Timeline_Add_Button_img = Gtk::Image(m_Timeline_Add_Button_pixbuf);
									m_Timeline_Add_Button.set_image(m_Timeline_Add_Button_img);
										m_Timeline_Add_Button.property_always_show_image();
										m_Timeline_Add_Button.show();
										m_Timeline_Add_Button.set_size_request(20, 20);
										m_Timeline_Add_Button.signal_clicked().connect(sigc::mem_fun(*this, &Kage::LayerAdd_onClick));
						m_Timeline_Layer_Add_HBox.pack_start(m_Timeline_Del_Button, Gtk::PACK_SHRINK);
							m_Timeline_Del_Button_pixbuf = Gdk::Pixbuf::create_from_resource("/kage/share/layer/layer_delete.png");
								m_Timeline_Del_Button_img = Gtk::Image(m_Timeline_Del_Button_pixbuf);
									m_Timeline_Del_Button.set_image(m_Timeline_Del_Button_img);
										m_Timeline_Del_Button.property_always_show_image();
										m_Timeline_Del_Button.show();
										m_Timeline_Del_Button.set_size_request(20, 20);
										m_Timeline_Del_Button.signal_clicked().connect(sigc::mem_fun(*this, &Kage::LayerDel_onClick));
												
						m_Timeline_Layer_Add_HBox.pack_start(_btnLayerMoveTop, Gtk::PACK_SHRINK);
							_btnLayerMoveTop_pixbuf = Gdk::Pixbuf::create_from_resource("/kage/share/layer/move_top.png");
								_btnLayerMoveTop_img = Gtk::Image(_btnLayerMoveTop_pixbuf);
									_btnLayerMoveTop.set_image(_btnLayerMoveTop_img);
										_btnLayerMoveTop.property_always_show_image();
										_btnLayerMoveTop.set_tooltip_text("Move Layer to Top");
										_btnLayerMoveTop.show();
										_btnLayerMoveTop.set_size_request(20, 20);
										_btnLayerMoveTop.signal_clicked().connect(sigc::mem_fun(*this, &Kage::LayerMoveTop_onClick));
						m_Timeline_Layer_Add_HBox.pack_start(_btnLayerMoveUp, Gtk::PACK_SHRINK);
							_btnLayerMoveUp_pixbuf = Gdk::Pixbuf::create_from_resource("/kage/share/layer/move_up.png");
								_btnLayerMoveUp_img = Gtk::Image(_btnLayerMoveUp_pixbuf);
									_btnLayerMoveUp.set_image(_btnLayerMoveUp_img);
										_btnLayerMoveUp.property_always_show_image();
										_btnLayerMoveUp.set_tooltip_text("Move Layer Up");
										_btnLayerMoveUp.set_focus_on_click(false);
										_btnLayerMoveUp.show();
										_btnLayerMoveUp.set_size_request(20, 20);
										_btnLayerMoveUp.signal_clicked().connect(sigc::mem_fun(*this, &Kage::LayerMoveUp_onClick));
						m_Timeline_Layer_Add_HBox.pack_start(_btnLayerMoveDown, Gtk::PACK_SHRINK);
							_btnLayerMoveDown_pixbuf = Gdk::Pixbuf::create_from_resource("/kage/share/layer/move_down.png");
								_btnLayerMoveDown_img = Gtk::Image(_btnLayerMoveDown_pixbuf);
									_btnLayerMoveDown.set_image(_btnLayerMoveDown_img);
										_btnLayerMoveDown.property_always_show_image();
										_btnLayerMoveDown.set_tooltip_text("Move Layer Down");
										_btnLayerMoveDown.set_focus_on_click(false);
										_btnLayerMoveDown.show();
										_btnLayerMoveDown.set_size_request(20, 20);
										_btnLayerMoveDown.signal_clicked().connect(sigc::mem_fun(*this, &Kage::LayerMoveDown_onClick));
						m_Timeline_Layer_Add_HBox.pack_start(_btnLayerMoveBottom, Gtk::PACK_SHRINK);
							_btnLayerMoveBottom_pixbuf = Gdk::Pixbuf::create_from_resource("/kage/share/layer/move_bottom.png");
								_btnLayerMoveBottom_img = Gtk::Image(_btnLayerMoveBottom_pixbuf);
									_btnLayerMoveBottom.set_image(_btnLayerMoveBottom_img);
										_btnLayerMoveBottom.property_always_show_image();
										_btnLayerMoveBottom.set_tooltip_text("Move Layer to Bottom");
										_btnLayerMoveBottom.set_focus_on_click(false);
										_btnLayerMoveBottom.show();
										_btnLayerMoveBottom.set_size_request(20, 20);
										_btnLayerMoveBottom.signal_clicked().connect(sigc::mem_fun(*this, &Kage::LayerMoveBottom_onClick));
						m_Timeline_Layer_Add_HBox.pack_start(_toggleOnion, Gtk::PACK_SHRINK);
							_toggleOnion_pixbuf = Gdk::Pixbuf::create_from_resource("/kage/share/layer/onion.png");
								_toggleOnion_img = Gtk::Image(_toggleOnion_pixbuf);
									_toggleOnion.set_image(_toggleOnion_img);
										_toggleOnion.property_always_show_image();
										_toggleOnion.set_tooltip_text("Toggle Onion Skin");
										_toggleOnion.set_focus_on_click(false);
										_toggleOnion.show();
										_toggleOnion.set_size_request(20, 20);
										_toggleOnion.signal_clicked().connect(sigc::mem_fun(*this, &Kage::ToggleOnion_onClick));
						m_Timeline_Layer_Add_HBox.pack_start(_toggleOnionLayer, Gtk::PACK_SHRINK);
							_toggleOnionLayer_pixbuf = Gdk::Pixbuf::create_from_resource("/kage/share/layer/onion_layer.png");
								_toggleOnionLayer_img = Gtk::Image(_toggleOnionLayer_pixbuf);
									_toggleOnionLayer.set_image(_toggleOnionLayer_img);
										_toggleOnionLayer.property_always_show_image();
										_toggleOnionLayer.set_tooltip_text("Toggle Onion Layer");
										_toggleOnionLayer.set_focus_on_click(false);
										_toggleOnionLayer.show();
										_toggleOnionLayer.set_size_request(20, 20);
										_toggleOnionLayer.signal_clicked().connect(sigc::mem_fun(*this, &Kage::ToggleOnion_onClick));
										
				m_Timeline_HPaned.add2(m_Timeline_Frame_VBox1);
					m_Timeline_Frame_VBox1.pack_start(m_Timeline_Frame_VBox2, Gtk::PACK_EXPAND_WIDGET);
						m_Timeline_Frame_VBox2.pack_start(m_Timeline_Frame_ScrolledWindow);//, Gtk::PACK_SHRINK);
							m_Timeline_Frame_ScrolledWindow.set_policy(Gtk::POLICY_EXTERNAL , Gtk::POLICY_EXTERNAL);
								m_Timeline_Frame_ScrolledWindow.set_border_width(0);
								m_Timeline_Frame_ScrolledWindow.add(_framesetManager);
								m_Timeline_Frame_ScrolledWindow.set_shadow_type(Gtk::SHADOW_NONE);
								m_Timeline_Frame_ScrolledWindow.set_kinetic_scrolling(true);
						m_Timeline_Frame_VBox2.pack_start(m_Timeline_HScrollbar, Gtk::PACK_SHRINK);
							m_Timeline_HScrollbar.set_adjustment(m_Timeline_Frame_ScrolledWindow.get_hadjustment());
					m_Timeline_Frame_VBox1.pack_start(m_Timeline_CurrentFrame, Gtk::PACK_SHRINK);
							m_Timeline_CurrentFrame.set_label("L 1 F 1");
							m_Timeline_CurrentFrame.set_size_request(22, 22);

			m_Timeline_HBox.pack_start(m_Timeline_VScrollbar, Gtk::PACK_SHRINK);
				m_Timeline_VScrollbar.set_adjustment(m_Timeline_Frame_ScrolledWindow.get_vadjustment());
					m_Timeline_Layer_ScrolledWindow.set_vadjustment(m_Timeline_Frame_ScrolledWindow.get_vadjustment());
					
		m_VPane_Timeline.add2(m_HPane_DrawingArea);
			m_HPane_DrawingArea.add1(m_Stage_Pane_ScrolledWindow);
				m_Stage_Pane_ScrolledWindow.add(m_KageStage);
				m_Stage_Pane_ScrolledWindow.set_border_width(0);
				m_Stage_Pane_ScrolledWindow.set_shadow_type(Gtk::SHADOW_NONE);
					m_KageStage.set_size_request(1280, 700);
					m_KageStage.show();
					m_HPane_DrawingArea.add2(m_Box1);
			
			m_Box1.pack_start(m_Property_Pane_ScrolledWindow, Gtk::PACK_EXPAND_WIDGET);
			
				m_Property_Pane_ScrolledWindow.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_EXTERNAL);
				m_Property_Pane_ScrolledWindow.add(m_PropertyBox);
				m_Property_Pane_ScrolledWindow.set_border_width(0);
				m_Property_Pane_ScrolledWindow.set_shadow_type(Gtk::SHADOW_NONE);
//				m_Property_Pane_ScrolledWindow.set_vadjustment(m_Timeline_Frame_ScrolledWindow.get_vadjustment());
			m_Box1.pack_start(m_Property_Pane_VScrollbar, Gtk::PACK_SHRINK);
				m_Property_Pane_VScrollbar.set_adjustment(m_Property_Pane_ScrolledWindow.get_vadjustment());
			
			//Property Pane Label
			m_PropertyBox.pack_start(m_LabelProp, Gtk::PACK_SHRINK);
				m_LabelProp.set_size_request(200, 20);
				m_LabelProp.set_alignment(Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
			
			m_PropertyBox.pack_start(m_Separator, Gtk::PACK_SHRINK);
			//Stage
			m_PropertyBox.pack_start(m_PropStage, Gtk::PACK_SHRINK);
				m_PropStage.set_border_width(10);
				m_PropStage.pack_start(m_PropStageV1);
					m_PropStageV1.set_border_width(4);
					m_PropStageV1.set_spacing(4);
					m_PropStageV1.pack_start(m_LabelStageWid);
					m_PropStageV1.pack_start(m_LabelStageHgt);
					m_PropStageV1.pack_start(m_LabelStageBG);
					m_PropStageV1.pack_start(m_LabelStageFPS);
				m_PropStage.pack_start(m_PropStageV2);
					m_PropStageV2.set_border_width(4);
					m_PropStageV2.set_spacing(4);
					//m_PropStageV2.set_size_request(40, 80);
					m_PropStageV2.pack_start(m_EntryStageWid, Gtk::PACK_SHRINK);
					m_PropStageV2.pack_start(m_EntryStageHgt, Gtk::PACK_SHRINK);
					m_PropStageV2.pack_start(m_ColorButtonStage, Gtk::PACK_SHRINK);
					m_PropStageV2.pack_start(m_EntryStageFPS, Gtk::PACK_SHRINK);
						m_EntryStageWid.set_size_request(20, 24);
						m_EntryStageWid.set_width_chars(4);
						m_EntryStageWid.set_max_length(4);
						m_EntryStageWid.set_text(StringHelper::doubleToString(m_KageStage.stageWidth));
							m_EntryStageWid.signal_activate().connect(
								sigc::mem_fun(*this, &Kage::EntryStageArea_onEnter) );
						m_EntryStageHgt.set_size_request(20, 24);
						m_EntryStageHgt.set_width_chars(4);
						m_EntryStageHgt.set_max_length(4);
						m_EntryStageHgt.set_text(StringHelper::doubleToString(m_KageStage.stageHeight));
							m_EntryStageHgt.signal_activate().connect(
								sigc::mem_fun(*this, &Kage::EntryStageArea_onEnter) );
						m_ColorButtonStage.set_size_request(32, 32);
						//m_ColorButtonStage.set_use_alpha(true);
						m_ColorButtonStage.set_title("Choose Stage Background Color");
						m_ColorButtonStage.set_color(m_KageStage.getStageBG());
							m_ColorButtonStage.signal_color_set().connect(sigc::mem_fun(* this, &Kage::ColorButtonStage_onClick) );
						m_EntryStageFPS.set_size_request(20, 24);
						m_EntryStageFPS.set_width_chars(3);
						m_EntryStageFPS.set_max_length(3);
						m_EntryStageFPS.set_text(StringHelper::unsignedIntegerToString(m_KageStage.fps));
							m_EntryStageFPS.signal_activate().connect(
								sigc::mem_fun(*this, &Kage::EntryStageFPS_onEnter) );
			//Toggle for Fill Properties
				//	m_Timeline_Frame_ScrolledWindow.set_border_width(0);
				//	m_Timeline_Frame_ScrolledWindow.set_shadow_type(Gtk::SHADOW_NONE);

				m_PropertyBox.pack_start(m_PropFillLabel, Gtk::PACK_SHRINK);
					m_PropFillLabel.set_border_width(0);
					m_PropFillLabel.pack_start(m_PropFillLabelV1);
						m_PropFillLabelV1.set_border_width(4);
						m_PropFillLabelV1.set_spacing(4);
						m_PropFillLabelV1.pack_start(m_ButtonFill);
							m_ButtonFill.signal_clicked().connect( sigc::mem_fun(*this, &Kage::TogglePropertiesFill_onClick) );
					m_PropFillLabel.pack_start(m_PropFillLabelV2);
						m_PropFillLabelV2.set_border_width(4);
						m_PropFillLabelV2.set_spacing(4);
						m_PropFillLabelV2.pack_start(m_ColorButtonFill);
							m_ColorButtonFill.set_size_request(32, 32);
							m_ColorButtonFill.set_use_alpha(true);
							m_ColorButtonFill.set_title("Choose Fill Color");
								m_ColorButtonFill.set_color(m_KageStage.getStageBG());
							m_ColorButtonFill.signal_color_set().connect(sigc::mem_fun(* this, &Kage::ColorButtonFill_onClick) );
					
				m_PropertyBox.pack_start(m_PropFill, Gtk::PACK_SHRINK);
					m_PropFill.set_border_width(10);
					m_PropFill.pack_start(m_PropFillV1);
						m_PropFillV1.set_border_width(4);
						m_PropFillV1.set_spacing(4);
						//m_PropFillV1.pack_start(m_LabelFill);
						m_PropFillV1.pack_start(m_LabelFillR);
						m_PropFillV1.pack_start(m_LabelFillG);
						m_PropFillV1.pack_start(m_LabelFillB);
						m_PropFillV1.pack_start(m_LabelFillAlpha);
						m_PropFillV1.pack_start(m_LabelFillRGBA);
					m_PropFill.pack_start(m_PropFillV2);
						m_PropFillV2.set_border_width(4);
						m_PropFillV2.set_spacing(4);
						//m_PropFillV2.pack_start(m_ColorButtonFill);
						m_PropFillV2.pack_start(_scaleFillR, Gtk::PACK_SHRINK);
							_scaleFillR.set_digits(0);
							_scaleFillR.set_value(255);
							_scaleFillR.set_value_pos(Gtk::POS_TOP);
							_scaleFillR.set_draw_value();
							_scaleFillR.signal_value_changed().connect(
								sigc::mem_fun(*this, &Kage::FillR_onChange));
						m_PropFillV2.pack_start(_scaleFillG, Gtk::PACK_SHRINK);
							_scaleFillG.set_digits(0);
							_scaleFillG.set_value(255);
							_scaleFillG.set_value_pos(Gtk::POS_TOP);
							_scaleFillG.set_draw_value();
							_scaleFillG.signal_value_changed().connect(
								sigc::mem_fun(*this, &Kage::FillG_onChange));
						m_PropFillV2.pack_start(_scaleFillB, Gtk::PACK_SHRINK);
							_scaleFillB.set_digits(0);
							_scaleFillB.set_value(255);
							_scaleFillB.set_value_pos(Gtk::POS_TOP);
							_scaleFillB.set_draw_value();
							_scaleFillB.signal_value_changed().connect(
								sigc::mem_fun(*this, &Kage::FillB_onChange));
						m_PropFillV2.pack_start(_scaleFillAplha, Gtk::PACK_SHRINK);
							_scaleFillAplha.set_digits(0);
							_scaleFillAplha.set_value(100);
							_scaleFillAplha.set_value_pos(Gtk::POS_TOP);
							_scaleFillAplha.set_draw_value();
							_scaleFillAplha.signal_value_changed().connect(
								sigc::mem_fun(*this, &Kage::FillAlpha_onChange));
						m_PropFillV2.pack_start(m_EntryFillRGBA, Gtk::PACK_SHRINK);
							m_EntryFillRGBA.set_size_request(20, 24);
							m_EntryFillRGBA.set_width_chars(8);
							m_EntryFillRGBA.set_max_length(8);
							//m_EntryFillRGBA.set_text('random hex as color'));
							m_EntryFillRGBA.signal_activate().connect(
								sigc::mem_fun(*this, &Kage::EntryFillRGBA_onEnter));
				m_PropertyBox.pack_start(m_PropStrokeLabel, Gtk::PACK_SHRINK);
					m_PropStrokeLabel.set_border_width(0);
					m_PropStrokeLabel.pack_start(m_PropStrokeLabelV1);
						m_PropStrokeLabelV1.set_border_width(4);
						m_PropStrokeLabelV1.set_spacing(4);
						m_PropStrokeLabelV1.pack_start(m_ButtonStroke);
							m_ButtonStroke.signal_clicked().connect( sigc::mem_fun(*this, &Kage::TogglePropertiesStroke_onClick) );
					m_PropStrokeLabel.pack_start(m_PropStrokeLabelV2);
						m_PropStrokeLabelV2.set_border_width(4);
						m_PropStrokeLabelV2.set_spacing(4);
						m_PropStrokeLabelV2.pack_start(m_ColorButtonStroke);
							m_ColorButtonStroke.set_size_request(32, 32);
							m_ColorButtonStroke.set_use_alpha(true);
							m_ColorButtonStroke.set_title("Choose Stroke Color");
								m_ColorButtonStroke.set_color(m_KageStage.getStageBG());
							m_ColorButtonStroke.signal_color_set().connect(sigc::mem_fun(* this, &Kage::ColorButtonStroke_onClick) );
				m_PropertyBox.pack_start(m_PropStroke, Gtk::PACK_SHRINK);
					m_PropStroke.set_border_width(10);
					m_PropStroke.pack_start(m_PropStrokeV1);
						m_PropStrokeV1.pack_start(m_LabelStrokeR);
						m_PropStrokeV1.pack_start(m_LabelStrokeG);
						m_PropStrokeV1.pack_start(m_LabelStrokeB);
						m_PropStrokeV1.pack_start(m_LabelStrokeAlpha);
						m_PropStrokeV1.pack_start(m_LabelStrokeRGBA);
						m_PropStrokeV1.pack_start(m_LabelStrokeThickness);
					m_PropStroke.pack_start(m_PropStrokeV2);
						m_PropStrokeV2.set_border_width(4);
						m_PropStrokeV2.set_spacing(4);
						m_PropStrokeV2.pack_start(_scaleStrokeR, Gtk::PACK_SHRINK);
							_scaleStrokeR.set_digits(0);
							_scaleStrokeR.set_value(255);
							_scaleStrokeR.set_value_pos(Gtk::POS_TOP);
							_scaleStrokeR.set_draw_value();
							_scaleStrokeR.signal_value_changed().connect(
								sigc::mem_fun(*this, &Kage::StrokeR_onChange));
						m_PropStrokeV2.pack_start(_scaleStrokeG, Gtk::PACK_SHRINK);
							_scaleStrokeG.set_digits(0);
							_scaleStrokeG.set_value(255);
							_scaleStrokeG.set_value_pos(Gtk::POS_TOP);
							_scaleStrokeG.set_draw_value();
							_scaleStrokeG.signal_value_changed().connect(
								sigc::mem_fun(*this, &Kage::StrokeG_onChange));
						m_PropStrokeV2.pack_start(_scaleStrokeB, Gtk::PACK_SHRINK);
							_scaleStrokeB.set_digits(0);
							_scaleStrokeB.set_value(255);
							_scaleStrokeB.set_value_pos(Gtk::POS_TOP);
							_scaleStrokeB.set_draw_value();
							_scaleStrokeB.signal_value_changed().connect(
								sigc::mem_fun(*this, &Kage::StrokeB_onChange));
						m_PropStrokeV2.pack_start(_scaleStrokeAlpha, Gtk::PACK_SHRINK);
							_scaleStrokeAlpha.set_digits(0);
							_scaleStrokeAlpha.set_value(100);
							_scaleStrokeAlpha.set_value_pos(Gtk::POS_TOP);
							_scaleStrokeAlpha.set_draw_value();
							_scaleStrokeAlpha.signal_value_changed().connect(
								sigc::mem_fun(*this, &Kage::StrokeAlpha_onChange));
						m_PropStrokeV2.pack_start(m_EntryStrokeRGBA, Gtk::PACK_SHRINK);
							m_EntryStrokeRGBA.set_size_request(20, 24);
							m_EntryStrokeRGBA.set_width_chars(8);
							m_EntryStrokeRGBA.set_max_length(8);
							//m_EntryStrokeRGBA.set_text('random hex as color'));
							m_EntryStrokeRGBA.signal_activate().connect(
								sigc::mem_fun(*this, &Kage::EntryStrokeRGBA_onEnter));
						m_PropStrokeV2.pack_start(m_EntryStrokeThickness, Gtk::PACK_SHRINK);
							m_EntryStrokeThickness.set_size_request(20, 24);
							m_EntryStrokeThickness.set_width_chars(2);
							m_EntryStrokeThickness.set_max_length(2);
							m_EntryStrokeThickness.set_text(StringHelper::integerToString((int)KageStage::stroke.getThickness()));
							m_EntryStrokeThickness.signal_activate().connect(
								sigc::mem_fun(*this, &Kage::EntryStrokeThickness_onEnter));
				//X/Y Width/Height
				m_PropertyBox.pack_start(m_propLocationSize, Gtk::PACK_SHRINK);
					m_propLocationSize.set_border_width(10);
					m_propLocationSize.pack_start(m_propLocationSizeV1);
						m_propLocationSizeV1.set_border_width(4);
						m_propLocationSizeV1.set_spacing(4);
						m_propLocationSizeV1.pack_start(m_LabelX);
						m_propLocationSizeV1.pack_start(m_LabelY);
						m_propLocationSizeV1.pack_start(m_LabelWidth);
						m_propLocationSizeV1.pack_start(m_LabelHeight);
					m_propLocationSize.pack_start(m_propLocationSizeV2);
						m_propLocationSizeV2.set_border_width(4);
						m_propLocationSizeV2.set_spacing(4);
						m_propLocationSizeV2.pack_start(m_EntryX, Gtk::PACK_SHRINK);
							m_EntryX.set_size_request(20, 24);
							m_EntryX.set_width_chars(9);
							m_EntryX.set_max_length(9);
							m_EntryX.set_text(StringHelper::doubleToString(m_KageStage.propX));
							m_EntryX.signal_activate().connect(
								sigc::mem_fun(*this, &Kage::EntryX_onEnter));
						m_propLocationSizeV2.pack_start(m_EntryY, Gtk::PACK_SHRINK);
							m_EntryY.set_size_request(20, 24);
							m_EntryY.set_width_chars(9);
							m_EntryY.set_max_length(9);
							m_EntryY.set_text(StringHelper::doubleToString(m_KageStage.propY));
							m_EntryY.signal_activate().connect(
								sigc::mem_fun(*this, &Kage::EntryY_onEnter));
						m_propLocationSizeV2.pack_start(m_EntryWidth, Gtk::PACK_SHRINK);
							m_EntryWidth.set_size_request(20, 24);
							m_EntryWidth.set_width_chars(9);
							m_EntryWidth.set_max_length(9);
							m_EntryWidth.set_text(StringHelper::doubleToString(m_KageStage.propWidth));
							m_EntryWidth.signal_activate().connect(
								sigc::mem_fun(*this, &Kage::EntryWidth_onEnter));
						m_propLocationSizeV2.pack_start(m_EntryHeight, Gtk::PACK_SHRINK);
							m_EntryHeight.set_size_request(20, 24);
							m_EntryHeight.set_width_chars(9);
							m_EntryHeight.set_max_length(9);
							m_EntryHeight.set_text(StringHelper::doubleToString(m_KageStage.propHeight));
							m_EntryHeight.signal_activate().connect(
								sigc::mem_fun(*this, &Kage::EntryHeight_onEnter));
				//X/Y of Node
				m_PropertyBox.pack_start(m_propNodeXY, Gtk::PACK_SHRINK);
					m_propNodeXY.set_border_width(10);
					m_propNodeXY.pack_start(m_propNodeXYV1);
						m_propNodeXYV1.set_border_width(4);
						m_propNodeXYV1.set_spacing(4);
						m_propNodeXYV1.pack_start(m_LabelNodeX);
						m_propNodeXYV1.pack_start(m_LabelNodeY);
						m_propNodeXYV1.pack_start(m_LabelToggleLine);
					m_propNodeXY.pack_start(m_propNodeXYV2);
						m_propNodeXYV2.set_border_width(4);
						m_propNodeXYV2.set_spacing(4);
						m_propNodeXYV2.pack_start(m_EntryNodeX, Gtk::PACK_SHRINK);
							m_EntryNodeX.set_size_request(20, 24);
							m_EntryNodeX.set_width_chars(9);
							m_EntryNodeX.set_max_length(9);
							m_EntryNodeX.set_text(StringHelper::doubleToString(m_KageStage.nodeX));
							m_EntryNodeX.signal_activate().connect(
								sigc::mem_fun(*this, &Kage::EntryNodeX_onEnter));
						m_propNodeXYV2.pack_start(m_EntryNodeY, Gtk::PACK_SHRINK);
							m_EntryNodeY.set_size_request(20, 24);
							m_EntryNodeY.set_width_chars(9);
							m_EntryNodeY.set_max_length(9);
							m_EntryNodeY.set_text(StringHelper::doubleToString(m_KageStage.nodeY));
							m_EntryNodeY.signal_activate().connect(
								sigc::mem_fun(*this, &Kage::EntryNodeY_onEnter));
						m_propNodeXYV2.pack_start(_btnToggleLine, Gtk::PACK_SHRINK);
							_btnToggleLine_pixbuf = Gdk::Pixbuf::create_from_resource("/kage/share/icons/straighten.png");
								_btnToggleLine_img = Gtk::Image(_btnToggleLine_pixbuf);
									_btnToggleLine.set_image(_btnToggleLine_img);
										_btnToggleLine.property_always_show_image();
										_btnToggleLine.show();
										_btnToggleLine.set_size_request(20, 20);
							_btnToggleLine.set_focus_on_click(false);
							_btnToggleLine.signal_clicked().connect(sigc::mem_fun(*this, &Kage::ToggleLine_onClick));
		
				//Frame Tween
				m_PropertyBox.pack_start(m_propFrameTween, Gtk::PACK_SHRINK);
					m_propFrameTween.set_border_width(10);
					m_propFrameTween.pack_start(m_propFrameTweenV1);
						m_propFrameTweenV1.set_border_width(4);
						m_propFrameTweenV1.set_spacing(4);
						m_propFrameTweenV1.pack_start(m_LabelTweenX);
						m_propFrameTweenV1.pack_start(m_LabelTweenY);
					m_propFrameTween.pack_start(m_propFrameTweenV2);
						m_propFrameTweenV2.set_border_width(4);
						m_propFrameTweenV2.set_spacing(4);
						m_propFrameTweenV2.pack_start(m_ComboX);
							m_ComboX.append("Linear");
							m_ComboX.append("Ease In");
							m_ComboX.append("Ease Out");
							m_ComboX.set_active_text("Linear");
							m_ComboX.signal_changed().connect( sigc::mem_fun(*this, &Kage::FrameTween_onChange) );
						m_propFrameTweenV2.pack_start(m_ComboY);
							m_ComboY.append("Linear");
							m_ComboY.append("Ease In");
							m_ComboY.append("Ease Out");
							m_ComboY.set_active_text("Linear");
							m_ComboY.signal_changed().connect( sigc::mem_fun(*this, &Kage::FrameTween_onChange) );
	New_onClick();
	
	registerPropertiesPane();
	m_HPane_DrawingArea.property_position() = _area_properties_pane2;
	
	if (p_filePath != "") {
		ksfPath = p_filePath;
		doOpen();
	}
	_UPDATE_SHAPE_COLORS = false;
		unsigned int l_R = KageStage::fillColor.getR();
		unsigned int l_G = KageStage::fillColor.getG();
		unsigned int l_B = KageStage::fillColor.getB();
		unsigned int l_A = KageStage::fillColor.getA();
		m_EntryFillRGBA.set_text(int255ToHex(l_R) + int255ToHex(l_G) + int255ToHex(l_B) + int255ToHex(l_A));
		_scaleFillR.set_value(l_R);
		_scaleFillG.set_value(l_G);
		_scaleFillB.set_value(l_B);
		_scaleFillAplha.set_value((double) l_A / 255.0f * 100.0f);

		l_R = KageStage::stroke.getR();
		l_G = KageStage::stroke.getG();
		l_B = KageStage::stroke.getB();
		l_A = KageStage::stroke.getA();
		m_EntryStrokeRGBA.set_text(int255ToHex(l_R) + int255ToHex(l_G) + int255ToHex(l_B) + int255ToHex(l_A));
		_scaleStrokeR.set_value(l_R);
		_scaleStrokeG.set_value(l_G);
		_scaleStrokeB.set_value(l_B);
		_scaleStrokeAlpha.set_value((double) l_A / 255.0f * 100.0f);
	_UPDATE_SHAPE_COLORS = true;

	signal_key_press_event().connect( sigc::mem_fun(*this, &Kage::on_key_press_event) );
	signal_key_release_event().connect( sigc::mem_fun(*this, &Kage::on_key_release_event) );
}
bool Kage::on_key_press_event(GdkEventKey *e) {
	cout << "Kage::on_key_press_event " << e->keyval << " " << e->state << endl;
	return Gtk::Window::on_key_press_event(e);
}
bool Kage::on_key_release_event(GdkEventKey *e) {
	cout << "Kage::on_key_release_event" << e->keyval << " " << e->state << endl;
	return Gtk::Window::on_key_release_event(e);
}

void Kage::registerPropertiesPane() {
	_area_properties_pane1  = m_HPane_DrawingArea.get_width() - 30;
	_area_properties_pane2 = m_HPane_DrawingArea.get_width() - 240;
}

Kage::~Kage() {
	//
}

void Kage::displayMouseXY(double p_x, double p_y) {
	_labelStatusMouseXY.set_text("X: " + StringHelper::doubleToString(p_x) + " Y: " + StringHelper::doubleToString(p_y));
}
void Kage::updateStatus(Glib::ustring status_msg) {
	m_Statusbar.push(status_msg, m_ContextId);
}

void Kage::stackDoZoom(PointData p_originBefore, PointData p_originAfter, PointData p_zoomReference, double p_zoomRatio) {
	_undoRedoManager.applyZoom(p_originBefore, p_originAfter, p_zoomReference, p_zoomRatio);
}

void Kage::Undo_onClick() {
	KageDo l_kageDo = _undoRedoManager.undo();
	if (l_kageDo._layer != -1 && l_kageDo._frame != -1) {
		_framesetManager.setCurrentLayer(l_kageDo._layer);
		_framesetManager.setCurrentFrame(l_kageDo._frame); //<-- causes to lose focus on KageStage
		setFrameData(l_kageDo.getVectorData(), true);
		
		forceRenderFrames();
		set_focus(m_KageStage); //return focus on KageStage; should be fixed better when Frame is rewritten
	}
}
void Kage::Redo_onClick() {
	KageDo l_kageDo = _undoRedoManager.redo();
	if (l_kageDo._layer != -1 && l_kageDo._frame != -1) {
		_framesetManager.setCurrentLayer(l_kageDo._layer);
		_framesetManager.setCurrentFrame(l_kageDo._frame); //<-- causes to lose focus on KageStage
		setFrameData(l_kageDo.getVectorData(), true);
		
		forceRenderFrames();
		set_focus(m_KageStage); //return focus on KageStage; should be fixed better when Frame is rewritten
	}
}
void Kage::Cut_onClick() {
	Kage::timestamp_IN(); std::cout << " Kage::Cut_onClick " << KageFrame::_gotFocus << std::endl;
	if (m_KageStage.cutSelectedShapes() == true) {
		forceRenderFrames();
	}
	Kage::timestamp_OUT();
}
void Kage::Copy_onClick() {
	Kage::timestamp_IN(); std::cout << " Kage::Copy_onClick " << KageFrame::_gotFocus << std::endl;
	if (m_KageStage.copySelectedShapes() == true) {
		forceRenderFrames();
	}
	Kage::timestamp_OUT();
}
void Kage::Paste_onClick() {
	Kage::timestamp_IN(); std::cout << " Kage::Paste_onClick " << KageFrame::_gotFocus << std::endl;
	if (m_KageStage.pasteSelectedShapes() == true) {
		forceRenderFrames();
	}
	Kage::timestamp_OUT();
}
void Kage::Duplicate_onClick() {
	if (KageStage::toolMode == KageStage::MODE_SELECT) {
		Kage::timestamp_IN(); std::cout << " Kage::Duplicate_onClick" << std::endl;
		if (isLayerLocked() == false) {
			vector<unsigned int> l_selectedShapes = _framesetManager.duplicateShapes(m_KageStage.getSelectedShapes());
			if (l_selectedShapes.size() > 0) {
				m_KageStage.setSelectedShapes(l_selectedShapes);
				stackDo();
				forceRenderFrames();
			}
		}
		Kage::timestamp_OUT();
	}
}
void Kage::SelectAll_onClick() {
	if (KageStage::toolMode != KageStage::MODE_SELECT) {
		ToolSelect_onClick();
	}
	Kage::timestamp_IN(); std::cout << " Kage::SelectAll_onClick" << std::endl;
	if (m_KageStage.selectAllShapes() == true) {
		forceRenderFrames();
	}
	Kage::timestamp_OUT();
}
void Kage::Deselect_onClick() {
	Kage::timestamp_IN(); std::cout << " Kage::Deselect_onClick" << std::endl;
	if (_isPlaying == true) {
		Stop_onClick();
	} else if (KageStage::toolMode == KageStage::MODE_SELECT) {
		if (m_KageStage.deselectSelectedShapes() == true) {
			forceRenderFrames();
		}
	} else if (KageStage::toolMode == KageStage::MODE_NODE) {
		if (m_KageStage.deselectSelectedNodes() == true) {
			forceRenderFrames();
		}
	} else if (KageStage::toolMode == KageStage::MODE_DRAW_POLY) {
		if (m_KageStage.cancelDrawingPoly() == true) {
			forceRenderFrames();
		}
	}
	
	Kage::timestamp_OUT();
}

void Kage::ToggleTimeline_onClick() {
	Kage::timestamp_IN(); std::cout << " Kage::ToggleTimeline_onClick" << std::endl;
	if (m_VPane_Timeline.property_position() == 30) {
		m_VPane_Timeline.property_position() = _area_timeline_pane;
	} else {
		_area_timeline_pane = m_VPane_Timeline.property_position();
		m_VPane_Timeline.property_position() = 30;
	}
	forceRenderFrames();
	
	Kage::timestamp_OUT();
}
void Kage::ToggleProperties_onClick() {
	Kage::timestamp_IN(); std::cout << " Kage::ToggleProperties_onClick" << std::endl;
	if (m_HPane_DrawingArea.property_position() == _area_properties_pane1) {
		m_HPane_DrawingArea.property_position() = _area_properties_pane2;
	} else {
		m_HPane_DrawingArea.property_position() = _area_properties_pane1;
	}
	forceRenderFrames();
	
	Kage::timestamp_OUT();
}
void Kage::TogglePropertiesFill_onClick() {
	m_PropFill.set_visible(!m_PropFill.get_visible());
	_propFillVisible = m_PropFill.get_visible();
}
void Kage::TogglePropertiesStroke_onClick() {
	m_PropStroke.set_visible(!m_PropStroke.get_visible());
	_propStrokeVisible = m_PropStroke.get_visible();
}

void Kage::ShapeGroup_onClick() {
	if (KageStage::toolMode == KageStage::MODE_SELECT) {
		if (isLayerLocked() == false) {
			vector<unsigned int> l_selectedShapes = _framesetManager.groupSelectedShapes(m_KageStage.getSelectedShapes());
			if (l_selectedShapes.size() > 0) {
				m_KageStage.setSelectedShapes(l_selectedShapes);
				_framesetManager.recenterRotationPoint(l_selectedShapes);
				stackDo();
				forceRenderFrames();
			}
		}
	}
}
void Kage::ShapeUngroup_onClick() {
	if (KageStage::toolMode == KageStage::MODE_SELECT) {
		Kage::timestamp_IN(); std::cout << " Kage::ShapeUngroup_onClick" << std::endl;
		
		if (isLayerLocked() == false) {
			vector<unsigned int> l_selectedShapes = _framesetManager.ungroupSelectedShapes(m_KageStage.getSelectedShapes());
			if (l_selectedShapes.size() > 0) {
				m_KageStage.setSelectedShapes(l_selectedShapes);
				_framesetManager.recenterRotationPoint(l_selectedShapes);
				stackDo();
				forceRenderFrames();
			}
		}
		Kage::timestamp_OUT();
	}
}

void Kage::Raise_onClick() {
	if (KageStage::toolMode == KageStage::MODE_SELECT) {
		Kage::timestamp_IN(); std::cout << " Kage::Raise_onClick" << std::endl;
		
		if (isLayerLocked() == false) {
			vector<unsigned int> l_selectedShapes = _framesetManager.raiseSelectedShape(m_KageStage.getSelectedShapes());
			if (l_selectedShapes.size() > 0) {
				m_KageStage.setSelectedShapes(l_selectedShapes);
				stackDo();
				forceRenderFrames();
			}
		}
		Kage::timestamp_OUT();
	}
}
void Kage::Lower_onClick() {
	if (KageStage::toolMode == KageStage::MODE_SELECT) {
		Kage::timestamp_IN(); std::cout << " Kage::Lower_onClick" << std::endl;
		
		if (isLayerLocked() == false) {
			vector<unsigned int> l_selectedShapes = _framesetManager.lowerSelectedShape(m_KageStage.getSelectedShapes());
			if (l_selectedShapes.size() > 0) {
				m_KageStage.setSelectedShapes(l_selectedShapes);
				stackDo();
				forceRenderFrames();
			}
		}
		Kage::timestamp_OUT();
	}
}
void Kage::RaiseToTop_onClick() {
	if (KageStage::toolMode == KageStage::MODE_SELECT) {
		Kage::timestamp_IN(); std::cout << " Kage::RaiseToTop_onClick" << std::endl;
		
		if (isLayerLocked() == false) {
			vector<unsigned int> l_selectedShapes = _framesetManager.raiseToTopSelectedShape(m_KageStage.getSelectedShapes());
			if (l_selectedShapes.size() > 0) {
				m_KageStage.setSelectedShapes(l_selectedShapes);
				stackDo();
				forceRenderFrames();
			}
		}
		Kage::timestamp_OUT();
	}
}
void Kage::LowerToBottom_onClick() {
	if (KageStage::toolMode == KageStage::MODE_SELECT) {
		Kage::timestamp_IN(); std::cout << " Kage::LowerToBottom_onClick" << std::endl;
		
		if (isLayerLocked() == false) {
			vector<unsigned int> l_selectedShapes = _framesetManager.lowerToBottomSelectedShape(m_KageStage.getSelectedShapes());
			if (l_selectedShapes.size() > 0) {
				m_KageStage.setSelectedShapes(l_selectedShapes);
				stackDo();
				forceRenderFrames();
			}
		}
		Kage::timestamp_OUT();
	}
}

void Kage::FlipHorizontal_onClick() {
	if (KageStage::toolMode == KageStage::MODE_SELECT) {
		Kage::timestamp_IN(); std::cout << " Kage::FlipHorizontal_onClick" << std::endl;
		
		if (isLayerLocked() == false) {
			if (_framesetManager.flipHorizontalSelectedShape(m_KageStage.getSelectedShapes()) == true) {
				stackDo();
				forceRenderFrames();
			}
		}
		
		Kage::timestamp_OUT();
	}
}
void Kage::FlipVertical_onClick() {
	if (KageStage::toolMode == KageStage::MODE_SELECT) {
		Kage::timestamp_IN(); std::cout << " Kage::FlipVertical_onClick" << std::endl;
		
		if (isLayerLocked() == false) {
			if (_framesetManager.flipVerticalSelectedShape(m_KageStage.getSelectedShapes()) == true) {
				stackDo();
				forceRenderFrames();
			}
		}
		
		Kage::timestamp_OUT();
	}
}

void Kage::RecenterRotationPoint_onClick() {
	if (KageStage::toolMode == KageStage::MODE_SELECT) {
		Kage::timestamp_IN(); std::cout << " Kage::RecenterRotationPoint_onClick" << std::endl;
		
		if (isLayerLocked() == false) {
			if (_framesetManager.recenterRotationPoint(m_KageStage.getSelectedShapes()) == true) {
				stackDo();
				forceRenderFrames();
			}
		}
		
		Kage::timestamp_OUT();
	}
}

void Kage::Delete_onClick() {
	if (KageStage::toolMode == KageStage::MODE_SELECT) {
		Kage::timestamp_IN(); std::cout << " Kage::Delete_onClick SHAPE" << std::endl;
		if (m_KageStage.deleteSelectedShapes() == true) {
			stackDo();
			forceRenderFrames();
		}
		Kage::timestamp_OUT();
	} else if (KageStage::toolMode == KageStage::MODE_NODE) {
		Kage::timestamp_IN(); std::cout << " Kage::Delete_onClick NODE" << std::endl;
		if (m_KageStage.deleteSelectedNodes() == true) {
			forceRenderFrames();
		}
		Kage::timestamp_OUT();
	}
}

void Kage::AddFrame_onClick() {
	_framesetManager.addFrame();
	
	refreshUI();
}
void Kage::ExtendFrame_onClick() {
	_framesetManager.extendFrame();
	switchToNextFrame();
	
	refreshUI();
}
void Kage::DuplicateFrame_onClick() {
	_framesetManager.duplicateFrame();
	
	refreshUI();
}
void Kage::RemoveFrame_onClick() {
	_framesetManager.removeFrame();
		forceRenderFrames();
	
	refreshUI();
}

void Kage::refreshUI() {
	show_all();
	if (KageStage::toolMode == KageStage::MODE_SELECT
			|| KageStage::toolMode == KageStage::ToolMode::MODE_DRAW_RECT
			|| KageStage::toolMode == KageStage::ToolMode::MODE_DRAW_OVAL
			|| KageStage::toolMode == KageStage::ToolMode::MODE_DRAW_POLY
			|| KageStage::toolMode == KageStage::ToolMode::MODE_DRAW_PENCIL) {
		propNodeXYSetVisible(false);
		propStageSetVisible(false);
	} else if (KageStage::toolMode == KageStage::MODE_NODE) {
		propFillStrokeSetVisible(false);
		propStageSetVisible(false);
		propShapePropertiesSetVisible(false);
	} else if (KageStage::toolMode == KageStage::MODE_NONE) {
		propNodeXYSetVisible(false);
		propFillStrokeSetVisible(false);
		propShapePropertiesSetVisible(false);
	}
	

	unsigned int l_tween = getTween();
	if (l_tween > 0) {
		unsigned int l_tweenX = l_tween / 10;
		unsigned int l_tweenY = l_tween - (l_tweenX * 10);
		
		if (l_tweenX == 1) {
			m_ComboX.set_active_text("Linear");
		} else if (l_tweenX == 2) {
			m_ComboX.set_active_text("Ease In");
		} else if (l_tweenX == 3) {
			m_ComboX.set_active_text("Ease Out");
		}
		
		if (l_tweenY == 1) {
			m_ComboY.set_active_text("Linear");
		} else if (l_tweenY == 2) {
			m_ComboY.set_active_text("Ease In");
		} else if (l_tweenY == 3) {
			m_ComboY.set_active_text("Ease Out");
		}
		
		propFrameTweenSetVisible(true);
	} else {
		propFrameTweenSetVisible(false);
	}
}

/**
 * Deletes a Frame via Selecting All then Deleting selected shapes
 */
void Kage::CutFrame_onClick() {
	if (m_KageStage.selectAllShapes() == true) {
		if (m_KageStage.cutSelectedShapes() == true) {
			stackDo();
			forceRenderFrames();
		}
	}
}
/**
 * Copies Frame's full content
 */
void Kage::CopyFrame_onClick() {
	if (m_KageStage.selectAllShapes() == true) {
		if (m_KageStage.copySelectedShapes() == true) {
			forceRenderFrames();
		}
	}
}
/**
 * Overwrites Frame's current content via Selecting All then
 * Deleting selected shapes then pasting buffer
 */
void Kage::PasteFrame_onClick() {
	if (m_KageStage.selectAllShapes() == true) {
		m_KageStage.deleteSelectedShapes();
	}
	if (m_KageStage.pasteSelectedShapes() == true) {
		stackDo();
		forceRenderFrames();
	}
}
/**
 * Deletes a Frame via Selecting All then Deleting selected shapes
 */
void Kage::DeleteFrame_onClick() {
	if (doDeleteFrame() == true) {
		stackDo();
		forceRenderFrames();
	}
}
	bool Kage::doDeleteFrame() {
		if (m_KageStage.selectAllShapes() == true) {
			return m_KageStage.deleteSelectedShapes();
		}
		return false;
	}
void Kage::Tween_onClick() {
/*	Kage::timestamp_IN(); std::cout << " Kage::Tween_onClick" << std::endl;
	if (isLayerLocked() == false) {
		if (_framesetManager.getTween() != 0) {
			//set tween
			unsigned int l_tweenType  = 11;
			m_ComboX.set_active_text("Linear");
			m_ComboY.set_active_text("Linear");
			if (_framesetManager.setTween(l_tweenType) == true) {
//				stackDo();
				propFrameTweenSetVisible(true);
				
				forceRenderFrames();
			}
		} else if (_framesetManager.setTween(0) == true) {
//				stackDo();
				propFrameTweenSetVisible(false);
				forceRenderFrames();
			}
		}
	}
	
	Kage::timestamp_OUT();*/
}
void Kage::TweenFrame_onClick() {
	Kage::timestamp_IN(); std::cout << " Kage::TweenFrame_onClick" << std::endl;
	unsigned int l_tweenType  = 11;
	m_ComboX.set_active_text("Linear");
	m_ComboY.set_active_text("Linear");
	if (isLayerLocked() == false) {
		if (_framesetManager.setTween(l_tweenType) == true) {
//			stackDo();
			propFrameTweenSetVisible(true);
			forceRenderFrames();
		}
	}
	
	Kage::timestamp_OUT();
}

void Kage::RemoveTweenFrame_onClick() {
	Kage::timestamp_IN(); std::cout << " Kage::RemoveTweenFrame_onClick" << std::endl;
	
	if (isLayerLocked() == false) {
		if (_framesetManager.setTween(0) == true) {
//			stackDo();
			propFrameTweenSetVisible(false);
			forceRenderFrames();
		}
	}
	
	Kage::timestamp_OUT();
}

void Kage::switchToPreviousFrame() {
	_framesetManager.switchToPreviousFrame();
		forceRenderFrames();
	
	refreshUI();
}

void Kage::switchToNextFrame() {
	_framesetManager.switchToNextFrame();
		forceRenderFrames();
	
	refreshUI();
}

/**
 * Called by KageFramesetManager::setCurrentFrame
 */
void Kage::updateFrameLabel() {
	m_Timeline_CurrentFrame.set_label("L " + StringHelper::unsignedIntegerToString(getCurrentLayer()) + " F " + StringHelper::unsignedIntegerToString(getCurrentFrame()));
}

void Kage::LayerAdd_onClick() {
	_framesetManager.addFrameset(_layerManager.addLayer());
	std::cout << "Layer Count: " << _layerManager.layerCount() << std::endl;
	refreshUI();
	setCurrentFrame(getCurrentFrame());
	updateStatus("New Layer Added");
}
void Kage::LayerRename_onClick() {
	_layerManager.renameLayer();
}
void Kage::ShowHideLayer_onClick() {
	_layerManager.toggleVisibility();
}
void Kage::LockUnlockLayer_onClick() {
	_layerManager.toggleLock();
}
void Kage::LayerDel_onClick() {
	if (_layerManager.layerCount() > 1) {
		_framesetManager.deleteFrameset(getCurrentLayer());
		_layerManager.deleteLayer();
	}
	std::cout << "Layer Delete Button clicked." << std::endl;
}
/**
 * NOTE: It's important that we rearrange FramesMananger first before
 * LayerManager because Current Layer is being referenced by FramesManager
 * from LayerManager. 
 */
void Kage::LayerMoveTop_onClick() {
	if (_framesetManager.moveToTop() == true && _layerManager.moveToTop() == true) {
		forceRenderFrames();
	}
}
/**
 * NOTE: It's important that we rearrange FramesMananger first before
 * LayerManager because Current Layer is being referenced by FramesManager
 * from LayerManager. 
 */
void Kage::LayerMoveUp_onClick() {
	if (_framesetManager.moveUp() == true && _layerManager.moveUp() == true) {
		forceRenderFrames();
	}
}
/**
 * NOTE: It's important that we rearrange FramesMananger first before
 * LayerManager because Current Layer is being referenced by FramesManager
 * from LayerManager. 
 */
void Kage::LayerMoveDown_onClick() {
	if (_framesetManager.moveDown() == true && _layerManager.moveDown() == true) {
		forceRenderFrames();
	}
}
/**
 * NOTE: It's important that we rearrange FramesMananger first before
 * LayerManager because Current Layer is being referenced by FramesManager
 * from LayerManager. 
 */
void Kage::LayerMoveBottom_onClick() {
	if (_framesetManager.moveToBottom() == true && _layerManager.moveToBottom() == true) {
		forceRenderFrames();
	}
}

void Kage::ToggleOnion_onClick() {
	std::cout << "ToggleOnion_onClick " << std::endl;
	//reflect onion in rendering
	forceRenderFrames();
}

void Kage::toolsButtonToggle(string p_toolTip) {
	//toggleButtons is vector
	unsigned int tsize = toggleButtons.size();
	unsigned int i;
	for (i = 0; i < tsize; ++i) {
		if (toggleButtons[i]->get_tooltip_text() == p_toolTip) {
			toggleButtons[i]->set_active(true);
		} else {
			if (toggleButtons[i]->get_active() == true) {
				toggleButtons[i]->set_active(false);
			}
		}
	}
	m_KageStage.unpressKeys();
}

void Kage::ToolButtons_onClick(Gtk::ToggleButton *p_sourceButton) {
	if (p_sourceButton->get_active() == 1) {
		if (currentTool->get_tooltip_text() == "Poly Tool") {
			m_KageStage.cancelDrawingPoly();
		}
		
		currentTool = p_sourceButton;
		if (p_sourceButton->get_tooltip_text() == "Select Tool") {
			ToolSelect_onClick();
		} else if (p_sourceButton->get_tooltip_text() == "Node Tool") {
			ToolNode_onClick();
		} else if (p_sourceButton->get_tooltip_text() == "Poly Tool") {
			ToolPoly_onClick();
		} else if (p_sourceButton->get_tooltip_text() == "Oval Tool") {
			ToolOval_onClick();
		} else if (p_sourceButton->get_tooltip_text() == "Rectangle Tool") {
			ToolRectangle_onClick();
		} else if (p_sourceButton->get_tooltip_text() == "Pencil Tool") {
			ToolPencil_onClick();
		} else if (p_sourceButton->get_tooltip_text() == "Stroke Tool") {
			ToolStroke_onClick();
		} else if (p_sourceButton->get_tooltip_text() == "Fill Tool") {
			ToolFill_onClick();
		} else if (p_sourceButton->get_tooltip_text() == "Eyedrop Tool") {
			ToolEyedrop_onClick();
		} else if (p_sourceButton->get_tooltip_text() == "Zoom Tool") {
			ToolZoom_onClick();
//		} else if (p_sourceButton->get_tooltip_text() == "Stroke Tool") {
//			KageStage::toolMode = KageStage::MODE_MOVE_STAGE;
		} else {
			std::cout << p_sourceButton->get_tooltip_text() << ".active " << p_sourceButton->get_active() << " Button clicked." << std::endl;
		}
	}
}
void Kage::ToolSelect_onClick() {
	toolsButtonToggle("Select Tool");
	propStageSetVisible(true);
	propFillStrokeSetVisible(false);
	propShapePropertiesSetVisible(false);
	propNodeXYSetVisible(false);
	KageStage::toolMode = KageStage::MODE_SELECT;
	m_KageStage.initNodeTool();
	forceRenderFrames();
}

void Kage::ToolNode_onClick() {
	toolsButtonToggle("Node Tool");
	propStageSetVisible(false);
	propFillStrokeSetVisible(false);
	propShapePropertiesSetVisible(false);
	propNodeXYSetVisible(true);
	m_ColorButtonFill.set_color(m_KageStage.getFill());
	m_ColorButtonStroke.set_color(m_KageStage.getStroke());
	KageStage::toolMode = KageStage::MODE_NODE;
	m_KageStage.initNodeTool();
	forceRenderFrames();
}

void Kage::ToolPoly_onClick() {
	toolsButtonToggle("Poly Tool");
	propStageSetVisible(false);
	propFillStrokeSetVisible(true);
	propShapePropertiesSetVisible(false);
	propNodeXYSetVisible(false);
	m_ColorButtonFill.set_color(m_KageStage.getFill());
	m_ColorButtonStroke.set_color(m_KageStage.getStroke());
	KageStage::toolMode = KageStage::MODE_DRAW_POLY;
	m_KageStage.initNodeTool();
}

void Kage::ToolPencil_onClick() {
	toolsButtonToggle("Pencil Tool");
	propStageSetVisible(false);
	propFillStrokeSetVisible(true);
	propShapePropertiesSetVisible(false);
	propNodeXYSetVisible(false);
	m_ColorButtonFill.set_color(m_KageStage.getFill());
	m_ColorButtonStroke.set_color(m_KageStage.getStroke());
	KageStage::toolMode = KageStage::MODE_DRAW_PENCIL;
	m_KageStage.initNodeTool();
}

void Kage::ToolOval_onClick() {
	toolsButtonToggle("Oval Tool");
	propStageSetVisible(false);
	propFillStrokeSetVisible(true);
	propShapePropertiesSetVisible(false);
	propNodeXYSetVisible(false);
	m_ColorButtonFill.set_color(m_KageStage.getFill());
	m_ColorButtonStroke.set_color(m_KageStage.getStroke());
	KageStage::toolMode = KageStage::MODE_DRAW_OVAL;
	m_KageStage.initNodeTool();
}

void Kage::ToolRectangle_onClick() {
	toolsButtonToggle("Rectangle Tool");
	propStageSetVisible(false);
	propFillStrokeSetVisible(true);
	propShapePropertiesSetVisible(false);
	propNodeXYSetVisible(false);
	m_ColorButtonFill.set_color(m_KageStage.getFill());
	m_ColorButtonStroke.set_color(m_KageStage.getStroke());
	KageStage::toolMode = KageStage::MODE_DRAW_RECT;
	m_KageStage.initNodeTool();
}

void Kage::ToolStroke_onClick() {
	toolsButtonToggle("Stroke Tool");
	propStageSetVisible(false);
	propFillStrokeSetVisible(true);
	propShapePropertiesSetVisible(false);
	propNodeXYSetVisible(false);
	m_ColorButtonFill.set_color(m_KageStage.getFill());
	m_ColorButtonStroke.set_color(m_KageStage.getStroke());
	KageStage::toolMode = KageStage::MODE_STROKE;
//	m_KageStage.initNodeTool();
//	forceRenderFrames();
}

void Kage::ToolFill_onClick() {
	toolsButtonToggle("Fill Tool");
	propStageSetVisible(false);
	propFillStrokeSetVisible(true);
	propShapePropertiesSetVisible(false);
	propNodeXYSetVisible(false);
	m_ColorButtonFill.set_color(m_KageStage.getFill());
	m_ColorButtonStroke.set_color(m_KageStage.getStroke());
	KageStage::toolMode = KageStage::MODE_FILL;
//	m_KageStage.initNodeTool();
//	forceRenderFrames();
}

void Kage::ToolEyedrop_onClick() {
	toolsButtonToggle("Eyedrop Tool");
	propStageSetVisible(false);
	propFillStrokeSetVisible(true);
	propShapePropertiesSetVisible(false);
	propNodeXYSetVisible(false);
	m_ColorButtonFill.set_color(m_KageStage.getFill());
	m_ColorButtonStroke.set_color(m_KageStage.getStroke());
	KageStage::toolMode = KageStage::MODE_EYEDROP;
	m_KageStage.initNodeTool();
	forceRenderFrames();
}

void Kage::ToolZoom_onClick() {
	toolsButtonToggle("Zoom Tool");
	propStageSetVisible(true);
	propFillStrokeSetVisible(false);
	propShapePropertiesSetVisible(false);
	propNodeXYSetVisible(false);
	m_ColorButtonFill.set_color(m_KageStage.getFill());
	m_ColorButtonStroke.set_color(m_KageStage.getStroke());
	KageStage::toolMode = KageStage::MODE_ZOOM;
	m_KageStage.initNodeTool();
	forceRenderFrames();
}

void Kage::propStageSetVisible(bool p_visible) {
	m_PropStage.set_visible(p_visible);
}

void Kage::propFillStrokeSetVisible(bool p_visible) {
	m_PropFillLabel.set_visible(p_visible);
	m_PropStrokeLabel.set_visible(p_visible);
	if (p_visible == true) {
		m_PropFill.set_visible(_propFillVisible);
		m_PropStroke.set_visible(_propStrokeVisible);
	} else {
		m_PropFill.set_visible(p_visible);
		m_PropStroke.set_visible(p_visible);
	}
}

void Kage::propShapePropertiesSetVisible(bool p_visible) {
	updateShapeProperties();
	m_propLocationSize.set_visible(p_visible);
}

void Kage::propNodeXYSetVisible(bool p_visible) {
	updateNodeXY();
	m_propNodeXY.set_visible(p_visible);
}

void Kage::propFrameTweenSetVisible(bool p_visible) {
	m_propFrameTween.set_visible(p_visible);
}

void Kage::updateColors() {
	_UPDATE_SHAPE_COLORS = false;
		m_ColorButtonFill.set_color(m_KageStage.getFill());
		_scaleFillR.set_value(KageStage::fillColor.getR());
		_scaleFillG.set_value(KageStage::fillColor.getG());
		_scaleFillB.set_value(KageStage::fillColor.getB());
		_scaleFillAplha.set_value((double) KageStage::fillColor.getA()*100.0f/255.0f);
		m_ColorButtonStroke.set_color(m_KageStage.getStroke());
		_scaleStrokeR.set_value(KageStage::stroke.getR());
		_scaleStrokeG.set_value(KageStage::stroke.getG());
		_scaleStrokeB.set_value(KageStage::stroke.getB());
		_scaleStrokeAlpha.set_value((double) KageStage::stroke.getA()*100.0f/255.0f);
		m_EntryStrokeThickness.set_text(StringHelper::unsignedIntegerToString(KageStage::stroke.getThickness()));
	_UPDATE_SHAPE_COLORS = true;
}

void Kage::updateShapeProperties() {
	m_EntryX.set_text(StringHelper::StringHelper::doubleToString(m_KageStage.propX));
	m_EntryY.set_text(StringHelper::StringHelper::doubleToString(m_KageStage.propY));
	m_EntryWidth.set_text(StringHelper::StringHelper::doubleToString(m_KageStage.propWidth));
	m_EntryHeight.set_text(StringHelper::StringHelper::doubleToString(m_KageStage.propHeight));
}

void Kage::updateNodeXY() {
//	cout << " m_KageStage.nodeX " << m_KageStage.nodeX << endl;
	m_EntryNodeX.set_text(StringHelper::StringHelper::doubleToString(m_KageStage.nodeX));
//	cout << " m_EntryNodeX " << m_EntryNodeX.get_text() << endl;
	m_EntryNodeY.set_text(StringHelper::StringHelper::doubleToString(m_KageStage.nodeY));
}

void Kage::stackDo() {
	KageDo l_kageDo = _undoRedoManager.previewUndo();
	unsigned int l_layer = l_kageDo._layer;
	unsigned int l_frame = l_kageDo._frame;
	if (l_layer != -1 && l_frame != -1) {
		if (       l_layer != getCurrentLayer()
				|| l_frame != getCurrentFrame()) {
			_undoRedoManager.stackDo(getCurrentLayer(), getCurrentFrame(), _undoBase);
		}
	} else {
		_undoRedoManager.stackDo(getCurrentLayer(), getCurrentFrame(), _undoBase);
		
	}
	
	_undoRedoManager.stackDo(getCurrentLayer(), getCurrentFrame(), getFrameData().getVectorData());
	set_title("*" + ksfPath + " - " + KageAbout::app_title);
}

void Kage::Quit_onClick() {
	GdkEventAny *any_event;
	if (on_delete_event(any_event) == false) {
		hide();
	}
}

bool Kage::on_delete_event(GdkEventAny* any_event) {
	if (get_title()[0] != '*') {
		return false;
	}
	
	Gtk::MessageDialog l_prompt = Gtk::MessageDialog("Are you sure you want to quit?", false, Gtk::MessageType::MESSAGE_QUESTION, Gtk::ButtonsType::BUTTONS_YES_NO, true);
		l_prompt.set_position(Gtk::WIN_POS_CENTER);
		int l_response = l_prompt.run();
	
	if (l_response == Gtk::ResponseType::RESPONSE_YES) {
		return false;
	}

	return true;
}

void Kage::addDataToFrame(VectorDataManager v, bool p_force) {
	if (p_force || _layerManager.getLayer()->isLocked() == false) {
		_framesetManager.addDataToFrame(v);
	}
}
/**
 * Returns Frame's VectorDataManager if visible, but can be forced to by-pass.
 * \param p_force to by-pass "Is Visible" blocking
 * \return Frame's VectorDataManager.
 */
VectorDataManager Kage::getFrameData(bool p_force) {
	if (p_force || _layerManager.getLayer()->isVisible()) {
		return _framesetManager.getFrameData();
	} else {
		VectorDataManager l_nullReturn;
		return l_nullReturn;
	}
}

/**
 * For use on Onion Skin feature.  Returns Frame's VectorDataManager
 * only if it's visible.
 * \param p_frame is from 1 to total frame count, select any
 * \return Frame's VectorDataManager.
 */
VectorDataManager Kage::getFrameDataAt(unsigned int p_frame) {
	if (_layerManager.getLayer()->isVisible()) {
		return _framesetManager.getFrameDataAt(p_frame);
	} else {
		VectorDataManager l_nullReturn;
		return l_nullReturn;
	}
}

void Kage::setFrameData(VectorDataManager p_vectorsData) {
	setFrameData(p_vectorsData, false);
}
void Kage::setFrameData(VectorDataManager p_vectorsData, bool p_force) {
	if (p_force == true
			|| _layerManager.getLayer()->isLocked() == false) {
		_framesetManager.setFrameData(p_vectorsData);
	}
}

unsigned int Kage::getTween() {
	return _framesetManager.getTween();
}

bool Kage::isFrameEmpty() {
	return (_framesetManager.getFrameData().getVectorData().size() > 0);
}

void Kage::forceRenderFrames() {
	Kage::timestamp_IN(); cout << " Kage::forceRenderFrames " << endl;
	m_KageStage.invalidateToRender();
	renderFrames();

	Kage::timestamp_OUT();
}


void Kage::renderOnionFrames() {
	Kage::timestamp_IN(); std::cout << " Kage::renderOnionFrames" << std::endl;
	unsigned int l_layerCount = _layerManager.layerCount();
	unsigned int l_frameCount = _framesetManager.frameCount();
	
	unsigned int l_currentLayer = getCurrentLayer();
	unsigned int l_currentFrame = getCurrentFrame();
	double l_alpha[5] = {0.07, 0.12, 1.0, 0.12, 0.07};
		for (int f = (int)l_currentFrame-2; f <= (int)l_currentFrame+2; ++f) {
			if (f < 1) {
				continue;
			} else if (f > l_frameCount) {
				continue;
			}
			
			unsigned int l_frame = (unsigned int) f;
			for (unsigned int i = 1; i <= l_layerCount; ++i) {
				_framesetManager.setCurrentLayer(i);
				if (l_frame == l_currentFrame) {
					m_KageStage.renderFrame(m_KageStage.cr);
				} else {
					vector<VectorData> l_vectorData = getFrameDataAt(l_frame).getVectorData();
					m_KageStage.renderOnionFrame(m_KageStage.cr, l_vectorData, l_alpha[f-(l_currentFrame-2)]);
				}
			}
		}
	setCurrentLayer(l_currentLayer);
	Kage::timestamp_OUT();
}
/**
 * Called on any Stage activity to update displayed shapes.
 * This renders all frameN in all layers, where frameN is the currentFrame
 */
void Kage::renderFrames() {
	Kage::timestamp_IN();
	cout << " Kage::renderFrames" << endl;
	
	m_KageStage.clearScreen(m_KageStage.cr);
	
	if (_toggleOnion.get_active() == true) {
		renderOnionFrames();
		Kage::timestamp_OUT();
		return;
	}
	
	unsigned int l_layerCount = _layerManager.layerCount();
	unsigned int l_currentLayer = getCurrentLayer();
		for (unsigned int i = 1; i <= l_layerCount; ++i) {
			_framesetManager.setCurrentLayer(i);
			m_KageStage.renderFrame(m_KageStage.cr);
		}
	setCurrentLayer(l_currentLayer);
	
	Kage::timestamp_OUT();
}

void Kage::renderFramesBelowCurrentLayer() {
	Kage::timestamp_IN();
	cout << " Kage::renderFramesBelowCurrentLayer" << endl;
	m_KageStage.clearScreen(m_KageStage.cr);
	
	if (_toggleOnion.get_active() == true) {
		renderOnionFrames();
		Kage::timestamp_OUT();
		return;
	}
	
	unsigned int l_currentLayer = getCurrentLayer();
		for (unsigned int i = 1; i < l_currentLayer; ++i) {
			_framesetManager.setCurrentLayer(i);
			if (_toggleOnionLayer.get_active() == false) {
				m_KageStage.renderFrame(m_KageStage.cr);
			} else {
				vector<VectorData> l_vectorData = getFrameData().getVectorData();
				m_KageStage.renderOnionFrame(m_KageStage.cr, l_vectorData, 0.25);
			}
		}
	setCurrentLayer(l_currentLayer);
	
	Kage::timestamp_OUT();
}
void Kage::renderFramesAboveCurrentLayer() {
	Kage::timestamp_IN();
	cout << " Kage::renderFramesAboveCurrentLayer" << endl;
	unsigned int l_layerCount = _layerManager.layerCount();
	unsigned int l_currentLayer = getCurrentLayer();
		for (unsigned int i = (l_currentLayer + 1); i <= l_layerCount; ++i) {
			_framesetManager.setCurrentLayer(i);
			if (_toggleOnionLayer.get_active() == false) {
				m_KageStage.renderFrame(m_KageStage.cr);
			} else {
				vector<VectorData> l_vectorData = getFrameData().getVectorData();
				m_KageStage.renderOnionFrame(m_KageStage.cr, l_vectorData, 0.25);
			}
		}
	setCurrentLayer(l_currentLayer);
	Kage::timestamp_OUT();
}

bool Kage::isLayerLocked() {
	return _layerManager.isLayerLocked();
}
unsigned int Kage::getCurrentLayer() {
	return _layerManager.getCurrentLayer();
}

void Kage::setCurrentLayer(unsigned int p_layer) {
	if (_layerManager.getCurrentLayer() != p_layer) {
		std::cout << "setting undoBase A setCurrentLayer" << std::endl;
		_undoBase = getFrameData(true).getVectorData(); //for use later by stackDo()
		_layerManager.setCurrentLayer(p_layer);
	} else {
		Kage::timestamp_IN();
		std::cout << "same layer!" << std::endl;
		Kage::timestamp_OUT();
	}
}
unsigned int Kage::getCurrentFrame() {
	return _framesetManager.getCurrentFrame();
}
void Kage::setCurrentLayerByID(unsigned int p_layerID) {
	cout << "setting undoBase B setCurrentLayerByID " << endl;
	_undoBase = getFrameData(true).getVectorData(); //for use later by stackDo()
	_layerManager.setCurrentLayerByID(p_layerID);
}

void Kage::setCurrentFrame(unsigned int p_layer) {
	cout << "setting undoBase C setCurrentFrame " << endl;
	_undoBase = getFrameData(true).getVectorData(); //for use later by stackDo()
	_framesetManager.setCurrentFrame(p_layer);
}

void Kage::New_onClick() {
	if (continueNewFileWithUnsavedWork() == false) {
		return;
	}
	
	_layerManager.removeAllLayers();
	_framesetManager.removeAllFrames();
	_framesetManager.addFrameset(_layerManager.addLayer());
	
	_undoRedoManager.clear();
	stackDo();
	
	refreshUI();
	
	_framesetManager.setCurrentLayer(1);
	_framesetManager.setCurrentFrame(1);
	
	currentTool = toggleButtons[0];
	currentTool->set_active(true);
	
	m_KageStage.cleanSlate();
	
	m_EntryStageWid.set_text(StringHelper::doubleToString(m_KageStage.stageWidth));
	m_EntryStageHgt.set_text(StringHelper::doubleToString(m_KageStage.stageHeight));
	m_EntryStageFPS.set_text(StringHelper::unsignedIntegerToString(m_KageStage.fps));
	
	m_KageStage.invalidateToRender();
	
	ksfPath = "Untitled";
	set_title(ksfPath + " - " + KageAbout::app_title);
	updateStatus("Ready");
}

bool Kage::continueNewFileWithUnsavedWork() {
	if (get_title()[0] != '*') {
		return true;
	}
	
	Gtk::MessageDialog l_prompt = Gtk::MessageDialog("File is not saved. Do you want to discard unsaved content?", false, Gtk::MessageType::MESSAGE_QUESTION, Gtk::ButtonsType::BUTTONS_YES_NO, true);
		l_prompt.set_position(Gtk::WIN_POS_CENTER);
		int l_response = l_prompt.run();
	
	if (l_response == Gtk::ResponseType::RESPONSE_YES) {
		return true;
	}

	return false;
}

void Kage::OpenKSF_onClick() {
	Gtk::FileChooserDialog dialog("Open Kage Studio File", Gtk::FILE_CHOOSER_ACTION_OPEN);
	dialog.set_transient_for( * this);
		dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
		dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);
		dialog.set_default_size(800, 600);
	
	auto filter_kage = Gtk::FileFilter::create();
		filter_kage->set_name(KageAbout::app_title + " Files");
		filter_kage->add_mime_type("text/x-kage");
		filter_kage->add_pattern("*.ksf");
			dialog.add_filter(filter_kage);
	//Show the dialog and wait for a user response:
	int result = dialog.run();
	
	//Handle the response:
	switch (result) {
		case Gtk::RESPONSE_OK:
			New_onClick();
			ksfPath = dialog.get_filename();
			cout << "uri:" << dialog.get_uri() << endl;
			
			doOpen();
			break;
//		default:
//			std::cout << "clicked " << result << endl;
	}
}
void Kage::doOpen() {
	int l_len = strlen(ksfPath.c_str()) - 4;
	if (StringHelper::toLower(ksfPath).substr(l_len, 4) != ".ksf") {
		ksfPath = ksfPath + ".ksf";
	}
	
	string l_ksfContent = BasicXml::openXMLFile(ksfPath);
	
	//cout << "Loaded... \n" << l_ksfContent << endl;
	parseKSF(l_ksfContent);
	_undoRedoManager.clear();
	stackDo();
	propFillStrokeSetVisible(false);
	propShapePropertiesSetVisible(false);
	propNodeXYSetVisible(false);
	set_title(ksfPath + " - " + KageAbout::app_title);
}

void Kage::Save_onClick() {
	if (ksfPath != "Untitled") {
		doSave(ksfPath);
		return;
	}
	doSaveDialog("Save File");
}
void Kage::SaveAs_onClick() {
	doSaveDialog("Save File As");
}

void Kage::doSaveDialog(string p_title) {
	Gtk::FileChooserDialog dialog(p_title, Gtk::FILE_CHOOSER_ACTION_SAVE);
	dialog.set_transient_for( *this);
		dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
		dialog.add_button(Gtk::Stock::SAVE, Gtk::RESPONSE_OK);
		dialog.set_do_overwrite_confirmation(true);
	
	auto filter_kage = Gtk::FileFilter::create();
		filter_kage->set_name(KageAbout::app_title + " Files");
		filter_kage->add_mime_type("text/x-kage");
		filter_kage->add_pattern("*.ksf");
			dialog.add_filter(filter_kage);
	
	int result = dialog.run();
	
	switch (result) {
		case Gtk::RESPONSE_OK:
			cout << "uri:" << dialog.get_uri() << endl;
			doSave(dialog.get_filename());
			break;
//		default:
//			std::cout << "clicked " << result << endl;
	}
}
void Kage::doSave(string p_filename) {
	int l_len = strlen(p_filename.c_str()) - 4;
	if (StringHelper::toLower(p_filename).substr(l_len, 4) != ".ksf") {
		p_filename = p_filename + ".ksf";
	}
	ksfPath = p_filename;
	ksfInited = false;
	ksfFile.close();
	
	unsigned int l_lMax = _layerManager.layerCount();
	unsigned int i;
		unsigned int l_fMax = _framesetManager.frameCount();
		unsigned int j;
			unsigned int l_currentLayer;
			unsigned int l_currentFrame;
	
	saveKageStudio(ksfPath, "<KageStudio version=\"2020.03.10\">");
	saveKageStudio(ksfPath, "<stage width=\"" + StringHelper::StringHelper::doubleToString(m_KageStage.stageWidth) + "\" height=\"" + StringHelper::StringHelper::doubleToString(m_KageStage.stageHeight) + "\" " +
							"background=\"rgb(" + StringHelper::integerToString(m_KageStage.stageBG.getR()) + ", " + StringHelper::integerToString(m_KageStage.stageBG.getG()) + ", " + StringHelper::integerToString(m_KageStage.stageBG.getB()) + ")\" " +
							"fps=\"" + StringHelper::integerToString(m_KageStage.fps) + "\" " +
							"layers=\"" + StringHelper::integerToString(l_lMax) + "\" " +
							"frames=\"" + StringHelper::integerToString(l_fMax) + "\" />");
		l_currentLayer = getCurrentLayer();
		l_currentFrame = getCurrentFrame();
			for (i = 1; i <= l_lMax; i++) {
				_framesetManager.setCurrentLayer(i);
				saveKageStudio(ksfPath, "<layer" + StringHelper::unsignedIntegerToString(i) + " label=\"" + _layerManager.getLabel() + "\" visible=\"" + (_layerManager.isLayerVisible()?"true":"false") + "\" locked=\"" + (_layerManager.isLayerLocked()?"true":"false") + "\">");
				for (j = 1; j <= l_fMax; ++j) {
					KageFrame *l_frame = _framesetManager.getFrameAt(j);
					KageFrame::extension l_extension = l_frame->getExtension();
					string l_tween = "";
					if (l_frame->getTween() > 0) {
						l_tween = " tween=\"" + StringHelper::unsignedIntegerToString(l_frame->getTween()) + "\"";
					}
					if (l_extension == KageFrame::EXTENSION_NOT) {
						saveKageStudio(ksfPath, "<frame" + StringHelper::unsignedIntegerToString(j) + l_tween + ">");
						_framesetManager.setCurrentFrame(j);
						saveKageStudio(ksfPath, saveFrame() + "</frame" + StringHelper::unsignedIntegerToString(j) + ">");
					} else if (l_extension == KageFrame::EXTENSION_START) {
						saveKageStudio(ksfPath, "<frame" + StringHelper::unsignedIntegerToString(j) + " extend=\"start\"" + l_tween + ">");
						_framesetManager.setCurrentFrame(j);
						saveKageStudio(ksfPath, saveFrame() + "</frame" + StringHelper::unsignedIntegerToString(j) + ">");
					} else if (l_extension == KageFrame::EXTENSION_MID) {
						saveKageStudio(ksfPath, "<frame" + StringHelper::unsignedIntegerToString(j) + " extend=\"mid\"" + l_tween + " />");
					} else if (l_extension == KageFrame::EXTENSION_END) {
						saveKageStudio(ksfPath, "<frame" + StringHelper::unsignedIntegerToString(j) + " extend=\"end\"" + l_tween + " />");
					}
				}
				saveKageStudio(ksfPath, "</layer" + StringHelper::unsignedIntegerToString(i) + ">");
			}
		setCurrentLayer(l_currentLayer);
		_framesetManager.setCurrentFrame(l_currentFrame);
	if (saveKageStudio(ksfPath, "</KageStudio>") == true) {
		updateStatus("Saved to " + ksfPath);
		
		set_title(ksfPath + " - " + KageAbout::app_title);
		//TODO: zip saved file -- should it be zipped?
	} else {
		updateStatus("Unable to save file!  Try saving to a different location");
	}
}
void Kage::ExportKS_onClick() {
	Gtk::FileChooserDialog dialog("Export to KonsolScript", Gtk::FILE_CHOOSER_ACTION_SAVE);
	dialog.set_transient_for( * this);
		//Add response buttons the the dialog:
		dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
		dialog.add_button(Gtk::Stock::SAVE, Gtk::RESPONSE_OK);
		dialog.set_do_overwrite_confirmation(true);
		
		dialog.set_filename(ksfPath);
	
	auto filter_ks = Gtk::FileFilter::create();
		filter_ks->set_name("KonsolScript Files");
		filter_ks->add_mime_type("text/konsolscript");
		filter_ks->add_pattern("*.ks");
			dialog.add_filter(filter_ks);
	int result = dialog.run();
	
	switch (result) {
		case Gtk::RESPONSE_OK:
			string ksPath = dialog.get_filename();
			ksfInited = false;
			ksfFile.close();
			
			exportKonsolScript(ksPath, "Var:Number bgcolor;\n");
			exportKonsolScript(ksPath, "function kagestudio_screencls() {");
			exportKonsolScript(ksPath, "\tDraw:RectFill(0, 0, " + StringHelper::StringHelper::doubleToString(m_KageStage.stageWidth) + ", " + StringHelper::StringHelper::doubleToString(m_KageStage.stageHeight) + ", bgcolor, screen)");
			exportKonsolScript(ksPath, "}");
			exportKonsolScript(ksPath, "function render() {");
			exportKonsolScript(ksPath, "\tkagestudio_screencls()");
			exportKonsolScript(ksPath, "\tks_f1()");
			exportKonsolScript(ksPath, "}");
			exportKonsolScript(ksPath, "function ks_f1() {");
			exportKonsolScript(ksPath, "\t" + dumpFrame(true));
			exportKonsolScript(ksPath, "}");
			exportKonsolScript(ksPath, "function main() {");
			exportKonsolScript(ksPath, "\t//add variable initialization...");
			exportKonsolScript(ksPath, "\tKonsol:RGB(" + StringHelper::integerToString(m_KageStage.stageBG.getR()) + ", " + StringHelper::integerToString(m_KageStage.stageBG.getG()) + "," + StringHelper::integerToString(m_KageStage.stageBG.getB()) + ", bgcolor)");
			if (exportKonsolScript(ksPath, "}") == true) {
				updateStatus("Exported to " + ksPath);
			} else {
				updateStatus("Unable to export!  Please try a different directory.");
			}
			break;
	}
}

void Kage::ExportHTML5_onClick() {
	Gtk::FileChooserDialog dialog("Export to HTML5", Gtk::FILE_CHOOSER_ACTION_SAVE);
	dialog.set_transient_for( * this);
		//Add response buttons the the dialog:
		dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
		dialog.add_button(Gtk::Stock::SAVE, Gtk::RESPONSE_OK);
		dialog.set_do_overwrite_confirmation(true);
	
	auto filter_html5 = Gtk::FileFilter::create();
		filter_html5->set_name("HTML5 Files");
		filter_html5->add_mime_type("text/html");
		filter_html5->add_pattern("*.html");
			dialog.add_filter(filter_html5);
	int result = dialog.run();
	
	switch (result) {
		case Gtk::RESPONSE_OK:
			expPath = dialog.get_filename();
			cout << "uri:" << dialog.get_uri() << endl;
			expInited = false;
			expFile.close();
			
			unsigned int l_lMax = _layerManager.layerCount();
			unsigned int i;
				unsigned int l_fMax = _framesetManager.frameCount();
				unsigned int j;
					unsigned int t;
			
			exportHtml5(expPath, "<html>\n<head>\n<title>Exported from " + KageAbout::app_title + " v" + KageAbout::app_version + " </title>\n<script type='text/javascript'>\n");
			exportHtml5(expPath, "var screen;\n");
			exportHtml5(expPath, "var frame = 1;\n");
			exportHtml5(expPath, "var frameMax = " + StringHelper::unsignedIntegerToString(l_fMax) + ";\n");
			exportHtml5(expPath, "function kagestudio() {");
			exportHtml5(expPath, "\tmain();");
			exportHtml5(expPath, "\tscreen = document.getElementById('screen').getContext('2d');");
			if (l_fMax > 1) {
				exportHtml5(expPath, "\tsetInterval(kagestudio_loop, " + StringHelper::integerToString(1000/m_KageStage.fps) + ");");
				exportHtml5(expPath, "}");
				exportHtml5(expPath, "function kagestudio_screencls() {");
				exportHtml5(expPath, "\tscreen.fillStyle = \"rgb(" + StringHelper::integerToString(m_KageStage.stageBG.getR()) + ", " + StringHelper::integerToString(m_KageStage.stageBG.getG()) + "," + StringHelper::integerToString(m_KageStage.stageBG.getB()) + ")\";");
				exportHtml5(expPath, "\tscreen.fillRect(0, 0, " + StringHelper::doubleToString(m_KageStage.stageWidth) + ", " + StringHelper::doubleToString(m_KageStage.stageHeight) + ");");
				exportHtml5(expPath, "}");
				exportHtml5(expPath, "function kagestudio_loop() {");
				exportHtml5(expPath, "\tkagestudio_screencls()");
				exportHtml5(expPath, "\twindow['ks_f' + frame]();");
				exportHtml5(expPath, "\tframe++;");
				exportHtml5(expPath, "\tif (frame > frameMax) { frame = 1; }");
				exportHtml5(expPath, "}");
			} else {
				exportHtml5(expPath, "\tscreen.fillStyle = \"rgb(" + StringHelper::integerToString(m_KageStage.stageBG.getR()) + ", " + StringHelper::integerToString(m_KageStage.stageBG.getG()) + "," + StringHelper::integerToString(m_KageStage.stageBG.getB()) + ")\";");
				exportHtml5(expPath, "\tscreen.fillRect(0, 0, " + StringHelper::doubleToString(m_KageStage.stageWidth) + ", " + StringHelper::doubleToString(m_KageStage.stageHeight) + ");");
				exportHtml5(expPath, "\tks_f1();");
				exportHtml5(expPath, "}");
			}
				t = getCurrentLayer();
				unsigned int f = getCurrentFrame();
					for (j = 1; j <= l_fMax; ++j) {
						_framesetManager.setCurrentFrame(j);
						exportHtml5(expPath, "function ks_f" + StringHelper::unsignedIntegerToString(j) + "() {");
						for (i = 1; i <= l_lMax; i++) {
							_framesetManager.setCurrentLayer(i);
							exportHtml5(expPath, dumpFrame(false));
						}
						exportHtml5(expPath, "}\n");
					}
				setCurrentFrame(f);
				setCurrentLayer(t);
			exportHtml5(expPath, "function main() {\n\t//add variable initialization...\n}");
			if (exportHtml5(expPath, "</script>\n</head>\n<body align='center' onload='kagestudio();' bgcolor='#101010'>\n<canvas id='screen' width='" + StringHelper::doubleToString(m_KageStage.stageWidth) + "' height='" + StringHelper::doubleToString(m_KageStage.stageHeight) + "' style='display: block; margin: auto;'></canvas>\n</body>\n</html>") == true) {
				updateStatus("Exported to " + expPath);
			} else {
				updateStatus("Unable to export!  Please try a different directory.");
			}
			break;
	}
}

//https://www.w3schools.com/graphics/svg_reference.asp
void Kage::ExportSVG_onClick() {
	Gtk::FileChooserDialog dialog("Export to Scalable Vector Graphics", Gtk::FILE_CHOOSER_ACTION_SAVE);
	dialog.set_transient_for( * this);
		//Add response buttons the the dialog:
		dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
		dialog.add_button(Gtk::Stock::SAVE, Gtk::RESPONSE_OK);
		dialog.set_do_overwrite_confirmation(true);
	
	auto filter_svg = Gtk::FileFilter::create();
		filter_svg->set_name("SVG Files");
		filter_svg->add_mime_type("image/svg+xml");
		filter_svg->add_pattern("*.svg");
			dialog.add_filter(filter_svg);
	int result = dialog.run();
	
	switch (result) {
		case Gtk::RESPONSE_OK:
			expPath = dialog.get_filename();
			cout << "uri:" << dialog.get_uri() << endl;
			expInited = false;
			expFile.close();
			
			unsigned int l_lMax = _layerManager.layerCount();
			unsigned int i;
				unsigned int l_fMax = _framesetManager.frameCount();
				unsigned int j;
					unsigned int t;
			
			exportSvg(expPath, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>");
			exportSvg(expPath, "<!-- Created with " + KageAbout::app_title + " v" + KageAbout::app_version + " (" + KageAbout::app_website + ") -->\n");
			exportSvg(expPath, "<svg width=\"" + StringHelper::doubleToString(m_KageStage.stageWidth) + "\" height=\"" + StringHelper::doubleToString(m_KageStage.stageHeight) + "\" version=\"1.1\">");

				t = getCurrentLayer();
				string l_layerToSVG = "";
				string l_frameToSVG = "";
				unsigned int f = getCurrentFrame();
					//for (j = 1; j <= l_fMax; ++j) {
					//	_framesetManager.setCurrentFrame(j);
						for (i = 1; i <= l_lMax; i++) {
							_framesetManager.setCurrentLayer(i);
							
							l_layerToSVG =  "<g\n\tinkscape:label=\"" + _layerManager.getLabel() + "\"\n" +
											"\tinkscape:groupmode=\"layer\"\n" +
											"\tid=\"layer" + StringHelper::unsignedIntegerToString(i) + "\">\n";
							l_frameToSVG = "\t\t\n" + dumpFrameToSvg();
							exportSvg(expPath, l_layerToSVG + l_frameToSVG + "\n</g>");
						}
					//}
				setCurrentLayer(t);
			
			if (exportSvg(expPath, "</svg>") == true) {
				updateStatus("Exported to " + expPath);
			} else {
				updateStatus("Unable to export!  Please try a different directory.");
			}
			break;
	}
}

void Kage::ExportPNG_onClick() {
	doExportPNGDialog("Export to PNG", false);
}

void Kage::ExportPNGTransparent_onClick() {
	doExportPNGDialog("Export to PNG (Transparent)", true);
}
void Kage::doExportPNGDialog(string p_title, bool p_transparent) {
	Gtk::FileChooserDialog dialog(p_title, Gtk::FILE_CHOOSER_ACTION_SAVE);
	dialog.set_transient_for( * this);
		//Add response buttons the the dialog:
		dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
		dialog.add_button(Gtk::Stock::SAVE, Gtk::RESPONSE_OK);
		dialog.set_do_overwrite_confirmation(true);
		
		string l_exportPNG = "";
		int l_len = strlen(ksfPath.c_str()) - 4;
			l_exportPNG = StringHelper::toLower(ksfPath).substr(0, l_len) + ".png";
		cout << " setting to " << l_exportPNG << endl; 
		dialog.set_filename(l_exportPNG);
	
	auto filter_png = Gtk::FileFilter::create();
		filter_png->set_name("Portable Network Graphics");
		filter_png->add_mime_type("image/png");
		filter_png->add_pattern("*.png");
			dialog.add_filter(filter_png);
	int result = dialog.run();
	
	switch (result) {
		case Gtk::RESPONSE_OK:
			string l_pngPath = dialog.get_filename();
			cout << "uri:" << dialog.get_uri() << endl;
			
			int l_len = strlen(l_pngPath.c_str()) - 4;
			if (StringHelper::toLower(l_pngPath).substr(l_len, 4) != ".png") {
				l_pngPath = l_pngPath + ".png";
			}
			
			if (m_KageStage.renderToPNG(l_pngPath, p_transparent) == true) {
				updateStatus("Exported to " + l_pngPath);
			} else {
				updateStatus("Unable to exported!  Please try a different directory.");
			}
			break;
	}
}

void Kage::ExportPNGSequence_onClick() {
	Gtk::FileChooserDialog dialog("Export to PNG Sequence", Gtk::FILE_CHOOSER_ACTION_SAVE);
	dialog.set_transient_for( * this);
		//Add response buttons the the dialog:
		dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
		dialog.add_button(Gtk::Stock::SAVE, Gtk::RESPONSE_OK);
	
	auto filter_png = Gtk::FileFilter::create();
		filter_png->set_name("Portable Network Graphics");
		filter_png->add_mime_type("image/png");
		filter_png->add_pattern("*.png");
			dialog.add_filter(filter_png);
	int result = dialog.run();
	
	switch (result) {
		case Gtk::RESPONSE_OK:
			string l_pngPath = dialog.get_filename();
			cout << "uri:" << dialog.get_uri() << endl;
			string l_pngSequencePath;
			int l_len = strlen(l_pngPath.c_str()) - 4;
			if (StringHelper::toLower(l_pngPath).substr(l_len, 4) == ".png") {
				l_pngPath = l_pngPath.substr(0, l_len);
			}
			
			unsigned int l_lMax = _layerManager.layerCount();
			unsigned int i;
				unsigned int l_fMax = _framesetManager.frameCount();
				unsigned int j;
					unsigned int t;
					unsigned int f;
				
				
				GdkPoint l_tempOrigin;
				l_tempOrigin.x = m_KageStage.origin.x;
				l_tempOrigin.y = m_KageStage.origin.y;
				
				m_KageStage.origin.x = 0;
				m_KageStage.origin.y = 0;
						
				t = getCurrentLayer();
				f = getCurrentFrame();
					
					for (j = 1; j <= l_fMax; ++j) {
						_framesetManager.setCurrentFrame(j);
						
						Cairo::RefPtr<Cairo::ImageSurface> surface = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, m_KageStage.stageWidth, m_KageStage.stageHeight);
						Cairo::RefPtr<Cairo::Context> l_context = Cairo::Context::create(surface);
						
							for (i = 1; i <= l_lMax; i++) {
								_framesetManager.setCurrentLayer(i);
								m_KageStage.renderFrame(l_context);
							}
						
						if (j < 10) {
							l_pngSequencePath = l_pngPath + "0000" + StringHelper::unsignedIntegerToString(j) + ".png";
						} else if (j < 100) {
							l_pngSequencePath = l_pngPath + "000" + StringHelper::unsignedIntegerToString(j) + ".png";
						} else if (j < 1000) {
							l_pngSequencePath = l_pngPath + "00" + StringHelper::unsignedIntegerToString(j) + ".png";
						} else if (j < 10000) {
							l_pngSequencePath = l_pngPath + "0" + StringHelper::unsignedIntegerToString(j) + ".png";
						} else if (j < 100000) {
							l_pngSequencePath = l_pngPath + StringHelper::unsignedIntegerToString(j) + ".png";
						}
						CairoKage::writeToPNG(l_pngSequencePath, surface);
					}
				setCurrentFrame(f);
				setCurrentLayer(t);
			
				m_KageStage.origin.x = l_tempOrigin.x;
				m_KageStage.origin.y = l_tempOrigin.y;
			
			updateStatus("Exported to " + l_pngSequencePath);
			break;
	}
}

void Kage::ExportAVI_onClick() {
	Gtk::FileChooserDialog dialog("Export to AVI", Gtk::FILE_CHOOSER_ACTION_SAVE);
	dialog.set_transient_for( * this);
		//Add response buttons the the dialog:
		dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
		dialog.add_button(Gtk::Stock::SAVE, Gtk::RESPONSE_OK);
		dialog.set_do_overwrite_confirmation(true);
		
		string l_exportAVI = "";
		int l_len = strlen(ksfPath.c_str()) - 4;
			l_exportAVI = StringHelper::toLower(ksfPath).substr(0, l_len) + ".avi";
		cout << " setting to " << l_exportAVI << endl; 
		dialog.set_filename(l_exportAVI);
		
		
	auto filter_avi = Gtk::FileFilter::create();
		filter_avi->set_name("Audio Video Interleave files");
		filter_avi->add_mime_type("video/x-msvideo");
		filter_avi->add_pattern("*.avi");
			dialog.add_filter(filter_avi);
	int result = dialog.run();
	
	switch (result) {
		case Gtk::RESPONSE_OK:
			string l_pngPath = dialog.get_filename();
			cout << "uri:" << dialog.get_uri() << endl;
			string l_pngSequencePath;
			int l_len = strlen(l_pngPath.c_str()) - 4;
			if (StringHelper::toLower(l_pngPath).substr(l_len, 4) == ".avi") {
				l_pngPath = l_pngPath.substr(0, l_len);
			}
			
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
			if (runExternal(".\\ffmpeg", "-version") == false) {
#else
			if (runExternal("ffmpeg", "-version") == false) {
#endif
				updateStatus("FFMPEG is not installed; opening FFMPEG Download page...");
				openWebsite("https://www.ffmpeg.org/download.html");
					return;
			}
			
			unsigned int l_lMax = _layerManager.layerCount();
			unsigned int i;
				unsigned int l_fMax = _framesetManager.frameCount();
				unsigned int j;
					unsigned int t;
					unsigned int f;
				
				if (l_fMax < m_KageStage.fps) {
					updateStatus("Unable to export " + l_pngPath + ".avi; try adding more Frames to your animation.");
					return;
				}
				
				GdkPoint l_tempOrigin;
				l_tempOrigin.x = m_KageStage.origin.x;
				l_tempOrigin.y = m_KageStage.origin.y;
				
				m_KageStage.origin.x = 0;
				m_KageStage.origin.y = 0;
						
				t = getCurrentLayer();
				f = getCurrentFrame();
					
					for (j = 1; j <= l_fMax; ++j) {
						_framesetManager.setCurrentFrame(j);
						
						Cairo::RefPtr<Cairo::ImageSurface> surface = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, m_KageStage.stageWidth, m_KageStage.stageHeight);
						Cairo::RefPtr<Cairo::Context> l_context = Cairo::Context::create(surface);
						
							m_KageStage.clearScreen(l_context);
							for (i = 1; i <= l_lMax; i++) {
								_framesetManager.setCurrentLayer(i);
								m_KageStage.renderFrame(l_context, true);
							}
						
						if (j < 10) {
							l_pngSequencePath = l_pngPath + "0000" + StringHelper::unsignedIntegerToString(j) + ".png";
						} else if (j < 100) {
							l_pngSequencePath = l_pngPath + "000" + StringHelper::unsignedIntegerToString(j) + ".png";
						} else if (j < 1000) {
							l_pngSequencePath = l_pngPath + "00" + StringHelper::unsignedIntegerToString(j) + ".png";
						} else if (j < 10000) {
							l_pngSequencePath = l_pngPath + "0" + StringHelper::unsignedIntegerToString(j) + ".png";
						} else if (j < 100000) {
							l_pngSequencePath = l_pngPath + StringHelper::unsignedIntegerToString(j) + ".png";
						}
						CairoKage::writeToPNG(l_pngSequencePath, surface);
					}
					//workaround to FFMPEG-bug: see https://sourceforge.net/p/kage/tickets/25/
					if ((l_fMax % m_KageStage.fps) != 0) {
						unsigned int l_padding = m_KageStage.fps - ((l_fMax % m_KageStage.fps));
						for (j = 1; j <= l_padding; ++j) {
							_framesetManager.setCurrentFrame(l_fMax);
							
							Cairo::RefPtr<Cairo::ImageSurface> surface = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, m_KageStage.stageWidth, m_KageStage.stageHeight);
							Cairo::RefPtr<Cairo::Context> l_context = Cairo::Context::create(surface);
							
								m_KageStage.clearScreen(l_context);
								for (i = 1; i <= l_lMax; i++) {
									_framesetManager.setCurrentLayer(i);
									m_KageStage.renderFrame(l_context, true);
								}
							
							if (l_fMax+j < 10) {
								l_pngSequencePath = l_pngPath + "0000" + StringHelper::unsignedIntegerToString(l_fMax+j) + ".png";
							} else if (l_fMax+j < 100) {
								l_pngSequencePath = l_pngPath + "000" + StringHelper::unsignedIntegerToString(l_fMax+j) + ".png";
							} else if (l_fMax+j < 1000) {
								l_pngSequencePath = l_pngPath + "00" + StringHelper::unsignedIntegerToString(l_fMax+j) + ".png";
							} else if (l_fMax+j < 10000) {
								l_pngSequencePath = l_pngPath + "0" + StringHelper::unsignedIntegerToString(l_fMax+j) + ".png";
							} else if (l_fMax+j < 100000) {
								l_pngSequencePath = l_pngPath + StringHelper::unsignedIntegerToString(l_fMax+j) + ".png";
							}
							CairoKage::writeToPNG(l_pngSequencePath, surface);
						}
					}
					//end of workaround
				setCurrentFrame(f);
				setCurrentLayer(t);
			
				m_KageStage.origin.x = l_tempOrigin.x;
				m_KageStage.origin.y = l_tempOrigin.y;
			
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
			if (runExternal(".\\ffmpeg", "-y -framerate " + StringHelper::unsignedIntegerToString(m_KageStage.fps) + " -i \"" + l_pngPath + "%05d.png\" -b:v 2048k \"" + l_pngPath + ".avi\"")) {
				if (runExternal("del", "/f \"" + l_pngPath + "*.png\"")) {
#else
			if (runExternal("ffmpeg", "-y -framerate " + StringHelper::unsignedIntegerToString(m_KageStage.fps) + " -i '" + l_pngPath + "%05d.png' -b:v 2048k '" + l_pngPath + ".avi'")) {
				if (runExternal("find", dialog.get_current_folder() + " -wholename '" + l_pngPath + "*.png' -delete")) {
#endif
					updateStatus("Exported to " + l_pngPath + ".avi");
					return;
				}
			}
			updateStatus("Unable to export " + l_pngPath + ".avi");
			break;
	}
}

void Kage::ProjectSave_onClick() {
	Gtk::FileChooserDialog dialog("Please choose a folder", Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);
//	dialog.set_transient_for( * this);
	//Add response buttons the the dialog:
	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	dialog.add_button("Select", Gtk::RESPONSE_OK);
	int result = dialog.run();
	//Handle the response:
	switch(result) {
		case(Gtk::RESPONSE_OK):
			std::cout << "Select clicked." << std::endl;
			std::cout << "Folder selected: " << dialog.get_filename() << std::endl;
			break;
		case(Gtk::RESPONSE_CANCEL):
			std::cout << "Cancel clicked." << std::endl;
			break;
		default:
			std::cout << "Unexpected button clicked." << std::endl;
			break;
	}
}

void Kage::onActionActivate() {
	std::cout << "Action activated." << std::endl;
}

void Kage::FrameTween_onChange() {
	Glib::ustring nameX = m_ComboX.get_active_text();
	Glib::ustring nameY = m_ComboY.get_active_text();
	unsigned int l_tweenX = 0;
	unsigned int l_tweenY = 0;
	if (nameX == "Linear") {
		l_tweenX = 10;
	} else if (nameX == "Ease In") {
		l_tweenX = 20;
	} else if (nameX == "Ease Out") {
		l_tweenX = 30;
	}
	if (nameY == "Linear") {
		l_tweenY = 1;
	} else if (nameY == "Ease In") {
		l_tweenY = 2;
	} else if (nameY == "Ease Out") {
		l_tweenY = 3;
	}
	
	_framesetManager.setTween(l_tweenX + l_tweenY);
	std::cout << "FrameTween_onChange X = " << nameX << " Y = " << nameY << " l_tweenX " << (l_tweenX+l_tweenY) << std::endl;
}

void Kage::Play_onClick() {
	Stop_onClick();
	tickCounter = 0;
	frameCounter = 0;
	_isPlaying = true;
	m_tick_id = m_KageStage.add_tick_callback( sigc::mem_fun(*this, &Kage::on_tick) );
}

void Kage::PlayFrame_onClick() {
	Play_onClick();
	tickCounter = 0;
	frameCounter = getCurrentFrame()-1;
}

void Kage::Stop_onClick() {
	if (_isPlaying == true) {
		m_KageStage.remove_tick_callback(m_tick_id);
		_isPlaying = false;
	}
}

bool Kage::on_tick(const Glib::RefPtr<Gdk::FrameClock>& frame_clock) {
	if (m_start_time == 0) {
		m_start_time = frame_clock->get_frame_time();
	}
	
	const gint64 current_time = frame_clock->get_frame_time();
	const double f = ((current_time - m_start_time) % cycle_time) / (double)cycle_time;
	
	if (f >= m_KageStage.fpsElapse) {
		//m_start_time = 0;
	}
	
	++tickCounter;
	if (tickCounter >= (60/m_KageStage.fps)) { //working for 60Hz
		tickCounter = 0;
		
		++frameCounter;
		if (frameCounter > _framesetManager.frameCount()) {
			frameCounter = _framesetManager.frameCount();
			
			m_KageStage.remove_tick_callback(m_tick_id);
			_isPlaying = false;
		}
		
		_framesetManager.setCurrentFrame(frameCounter);
		//renderFrames(); ^setCurrentFrame automatically calls renderFrames, somehow
	}
	
	return true;
}
void Kage::updateEntryFillRGBA() {
	unsigned int l_R = KageStage::fillColor.getR();
	unsigned int l_G = KageStage::fillColor.getG();
	unsigned int l_B = KageStage::fillColor.getB();
	unsigned int l_A = KageStage::fillColor.getA();
	m_EntryFillRGBA.set_text(int255ToHex(l_R) + int255ToHex(l_G) + int255ToHex(l_B) + int255ToHex(l_A));
}
void Kage::FillR_onChange() {
	double l_red = _scaleFillR.get_value();
	Gdk::RGBA l_rgba = m_ColorButtonFill.get_rgba();
		l_rgba.set_red( l_red / 255.0f );
	m_ColorButtonFill.set_rgba(l_rgba);
	
	KageStage::fillColor.setR(l_red);
		updateEntryFillRGBA();
	m_KageStage.updateShapeColor(_UPDATE_SHAPE_COLORS, _UPDATE_SHAPE_COLORS);
}
void Kage::FillG_onChange() {
	double l_green = _scaleFillG.get_value();
	Gdk::RGBA l_rgba = m_ColorButtonFill.get_rgba();
		l_rgba.set_green( l_green / 255.0f );
	m_ColorButtonFill.set_rgba(l_rgba);
	
	KageStage::fillColor.setG(l_green);
		updateEntryFillRGBA();
	m_KageStage.updateShapeColor(_UPDATE_SHAPE_COLORS, _UPDATE_SHAPE_COLORS);
}
void Kage::FillB_onChange() {
	double l_blue = _scaleFillB.get_value();
	Gdk::RGBA l_rgba = m_ColorButtonFill.get_rgba();
		l_rgba.set_blue( l_blue / 255.0f );
	m_ColorButtonFill.set_rgba(l_rgba);
	
	KageStage::fillColor.setB(l_blue);
		updateEntryFillRGBA();
	m_KageStage.updateShapeColor(_UPDATE_SHAPE_COLORS, _UPDATE_SHAPE_COLORS);
}
void Kage::FillAlpha_onChange() {
	double l_alpha = _scaleFillAplha.get_value();
	Gdk::RGBA l_rgba = m_ColorButtonFill.get_rgba();
		l_rgba.set_alpha( l_alpha / 100.f );
	m_ColorButtonFill.set_rgba(l_rgba);
	
	KageStage::fillColor.setA((int) (l_alpha * 256.0f/ 100.0f));
		updateEntryFillRGBA();
	m_KageStage.updateShapeColor(_UPDATE_SHAPE_COLORS, _UPDATE_SHAPE_COLORS);
}

void Kage::EntryFillRGBA_onEnter() {
	//see: https://sourceforge.net/p/kage/tickets/66/?limit=25#7488
	string l_input = m_EntryFillRGBA.get_text();
	unsigned int l_inputLength = l_input.length();
	unsigned int l_R = 0;
	unsigned int l_G = 0;
	unsigned int l_B = 0;
	unsigned int l_A = KageStage::fillColor.getA();
	
	string l_sR = "";
	string l_sG = "";
	string l_sB = "";
	string l_sA = "";
	
	if (l_inputLength == 1) {
		l_sR = l_input + l_input;
		l_sG = l_input + l_input;
		l_sB = l_input + l_input;
	} else if (l_inputLength == 2) {
		l_sR = l_input;
		l_sG = l_input;
		l_sB = l_input;
	} else if (l_inputLength == 3) {
		l_sR = l_input.substr(0,1) + l_input.substr(0,1); //get R
		l_sG = l_input.substr(1,1) + l_input.substr(1,1); //get G
		l_sB = l_input.substr(2,1) + l_input.substr(2,1); //get B
	} else if (l_inputLength == 4) {
		l_sR = l_input.substr(0,1) + l_input.substr(0,1); //get R
		l_sG = l_input.substr(1,1) + l_input.substr(1,1); //get G
		l_sB = l_input.substr(2,1) + l_input.substr(2,1); //get B
		l_sA = l_input.substr(3,1) + l_input.substr(3,1); //get A
		l_A = hexToInt255(l_sA);
	} else if (l_inputLength == 5) {
		l_sR = l_input.substr(0,2); //get R
		l_sG = l_input.substr(2,2); //get G
		l_sB = l_input.substr(4,1) + l_input.substr(0,1); //get B
	} else if (l_inputLength == 6) {
		l_sR = l_input.substr(0,2); //get R
		l_sG = l_input.substr(2,2); //get G
		l_sB = l_input.substr(4,2); //get B
	} else if (l_inputLength == 7) {
		//apply as is
		l_sR = l_input.substr(0,2); //get R
		l_sG = l_input.substr(2,2); //get G
		l_sB = l_input.substr(4,2); //get B
		if (int255ToHex(l_A).length() > 1) {
			l_sA = l_input.substr(6,1) + int255ToHex(l_A).substr(1,1); //get A
		} else {
			l_sA = l_input.substr(6,1) + int255ToHex(l_A); //get A
		}
		l_A = hexToInt255(l_sA);
	} else if (l_inputLength == 8) {
		//apply as is
		l_sR = l_input.substr(0,2); //get R
		l_sG = l_input.substr(2,2); //get G
		l_sB = l_input.substr(4,2); //get B
		l_sA = l_input.substr(6,2); //get A
		l_A = hexToInt255(l_sA);
	}
	l_R = hexToInt255(l_sR);
	l_G = hexToInt255(l_sG);
	l_B = hexToInt255(l_sB);

	cout << "\n\nRGBA A " << l_sR << "\t" << l_sG << "\t" << l_sB << "\t" << l_sA << endl;
	cout << "\n\nRGBA B " << l_R  << "\t" << l_G  << "\t" << l_B  << "\t" << l_A << endl << endl << endl;
	_scaleFillR.set_value(l_R);
	_scaleFillG.set_value(l_G);
	_scaleFillB.set_value(l_B);
	_scaleFillAplha.set_value((double) l_A / 255.0f * 100.0f);
	KageStage::fillColor.setR(l_R);
	KageStage::fillColor.setG(l_G);
	KageStage::fillColor.setB(l_B);
	KageStage::fillColor.setA(l_A);
	
	Gdk::RGBA l_rgba = m_ColorButtonFill.get_rgba();
		l_rgba.set_red  ( l_R / 255.f );
		l_rgba.set_blue ( l_G / 255.f );
		l_rgba.set_green( l_B / 255.f );
		l_rgba.set_alpha( l_A / 255.f );
			m_ColorButtonFill.set_rgba(l_rgba);
	
	m_KageStage.updateShapeColor(_UPDATE_SHAPE_COLORS, _UPDATE_SHAPE_COLORS);
}


void Kage::updateEntryStrokeRGBA() {
	unsigned int l_R = KageStage::stroke.getR();
	unsigned int l_G = KageStage::stroke.getG();
	unsigned int l_B = KageStage::stroke.getB();
	unsigned int l_A = KageStage::stroke.getA();
	m_EntryStrokeRGBA.set_text(int255ToHex(l_R) + int255ToHex(l_G) + int255ToHex(l_B) + int255ToHex(l_A));
}

void Kage::StrokeR_onChange() {
	double l_red = _scaleStrokeR.get_value();
	Gdk::RGBA l_rgba = m_ColorButtonStroke.get_rgba();
		l_rgba.set_red( l_red / 255.0f );
	m_ColorButtonStroke.set_rgba(l_rgba);
	
	KageStage::stroke.setR(l_red);
		updateEntryStrokeRGBA();
	m_KageStage.updateShapeColor(_UPDATE_SHAPE_COLORS, _UPDATE_SHAPE_COLORS);
}
void Kage::StrokeG_onChange() {
	double l_green = _scaleStrokeG.get_value();
	Gdk::RGBA l_rgba = m_ColorButtonStroke.get_rgba();
		l_rgba.set_green( l_green / 255.0f );
	m_ColorButtonStroke.set_rgba(l_rgba);
	
	KageStage::stroke.setG(l_green);
		updateEntryStrokeRGBA();
	m_KageStage.updateShapeColor(_UPDATE_SHAPE_COLORS, _UPDATE_SHAPE_COLORS);
}
void Kage::StrokeB_onChange() {
	double l_blue = _scaleStrokeB.get_value();
	Gdk::RGBA l_rgba = m_ColorButtonStroke.get_rgba();
		l_rgba.set_blue( l_blue / 255.0f );
	m_ColorButtonStroke.set_rgba(l_rgba);
	
	KageStage::stroke.setB(l_blue);
		updateEntryStrokeRGBA();
	m_KageStage.updateShapeColor(_UPDATE_SHAPE_COLORS, _UPDATE_SHAPE_COLORS);
}
void Kage::StrokeAlpha_onChange() {
	double l_alpha = _scaleStrokeAlpha.get_value();
	Gdk::RGBA l_rgba = m_ColorButtonStroke.get_rgba();
		l_rgba.set_alpha( l_alpha / 100.f );
	m_ColorButtonStroke.set_rgba(l_rgba);
	
	KageStage::stroke.setA((int) (l_alpha * 256.0f/ 100.0f));
	m_KageStage.updateShapeColor(_UPDATE_SHAPE_COLORS, _UPDATE_SHAPE_COLORS);
}


void Kage::EntryStrokeRGBA_onEnter() {
	//see: https://sourceforge.net/p/kage/tickets/66/?limit=25#7488
	string l_input = m_EntryStrokeRGBA.get_text();
	unsigned int l_inputLength = l_input.length();
	unsigned int l_R = 0;
	unsigned int l_G = 0;
	unsigned int l_B = 0;
	unsigned int l_A = KageStage::stroke.getA();
	
	string l_sR = "";
	string l_sG = "";
	string l_sB = "";
	string l_sA = "";
	
	if (l_inputLength == 1) {
		l_sR = l_input + l_input;
		l_sG = l_input + l_input;
		l_sB = l_input + l_input;
	} else if (l_inputLength == 2) {
		l_sR = l_input;
		l_sG = l_input;
		l_sB = l_input;
	} else if (l_inputLength == 3) {
		l_sR = l_input.substr(0,1) + l_input.substr(0,1); //get R
		l_sG = l_input.substr(1,1) + l_input.substr(1,1); //get G
		l_sB = l_input.substr(2,1) + l_input.substr(2,1); //get B
	} else if (l_inputLength == 4) {
		l_sR = l_input.substr(0,1) + l_input.substr(0,1); //get R
		l_sG = l_input.substr(1,1) + l_input.substr(1,1); //get G
		l_sB = l_input.substr(2,1) + l_input.substr(2,1); //get B
		l_sA = l_input.substr(3,1) + l_input.substr(3,1); //get A
		l_A = hexToInt255(l_sA);
	} else if (l_inputLength == 5) {
		l_sR = l_input.substr(0,2); //get R
		l_sG = l_input.substr(2,2); //get G
		l_sB = l_input.substr(4,1) + l_input.substr(0,1); //get B
	} else if (l_inputLength == 6) {
		l_sR = l_input.substr(0,2); //get R
		l_sG = l_input.substr(2,2); //get G
		l_sB = l_input.substr(4,2); //get B
	} else if (l_inputLength == 7) {
		//apply as is
		l_sR = l_input.substr(0,2); //get R
		l_sG = l_input.substr(2,2); //get G
		l_sB = l_input.substr(4,2); //get B
		if (int255ToHex(l_A).length() > 1) {
			l_sA = l_input.substr(6,1) + int255ToHex(l_A).substr(1,1); //get A
		} else {
			l_sA = l_input.substr(6,1) + int255ToHex(l_A); //get A
		}
		l_A = hexToInt255(l_sA);
	} else if (l_inputLength == 8) {
		//apply as is
		l_sR = l_input.substr(0,2); //get R
		l_sG = l_input.substr(2,2); //get G
		l_sB = l_input.substr(4,2); //get B
		l_sA = l_input.substr(6,2); //get A
		l_A = hexToInt255(l_sA);
	}
	l_R = hexToInt255(l_sR);
	l_G = hexToInt255(l_sG);
	l_B = hexToInt255(l_sB);

	cout << "\n\nRGBA A " << l_sR << "\t" << l_sG << "\t" << l_sB << "\t" << l_sA << endl;
	cout << "\n\nRGBA B " << l_R  << "\t" << l_G  << "\t" << l_B  << "\t" << l_A << endl << endl << endl;
	_scaleStrokeR.set_value(l_R);
	_scaleStrokeG.set_value(l_G);
	_scaleStrokeB.set_value(l_B);
	_scaleStrokeAlpha.set_value((double) l_A / 255.0f * 100.0f);
	KageStage::stroke.setR(l_R);
	KageStage::stroke.setG(l_G);
	KageStage::stroke.setB(l_B);
	KageStage::stroke.setA(l_A);
	
	Gdk::RGBA l_rgba = m_ColorButtonStroke.get_rgba();
		l_rgba.set_red  ( l_R / 255.f );
		l_rgba.set_blue ( l_G / 255.f );
		l_rgba.set_green( l_B / 255.f );
		l_rgba.set_alpha( l_A / 255.f );
			m_ColorButtonStroke.set_rgba(l_rgba);
	
	m_KageStage.updateShapeColor(_UPDATE_SHAPE_COLORS, _UPDATE_SHAPE_COLORS);
}
void Kage::EntryStrokeThickness_onEnter() {
	unsigned int l_uint = StringHelper::toUnsignedInteger(m_EntryStrokeThickness.get_text());
	KageStage::stroke.setThickness(l_uint);
	m_EntryStrokeThickness.set_text(StringHelper::unsignedIntegerToString(KageStage::stroke.getThickness()));
	m_KageStage.updateShapeColor(_UPDATE_SHAPE_COLORS, _UPDATE_SHAPE_COLORS);
}

void Kage::EntryX_onEnter() {
	double l_dbl = StringHelper::toDouble(m_EntryX.get_text());
	
	m_EntryX.set_text(StringHelper::doubleToString(l_dbl));
	m_KageStage.updateShapeX(l_dbl);
}

void Kage::EntryY_onEnter() {
	double l_dbl = StringHelper::toDouble(m_EntryY.get_text());
	
	m_EntryY.set_text(StringHelper::doubleToString(l_dbl));
	m_KageStage.updateShapeY(l_dbl);
}

void Kage::EntryWidth_onEnter() {
	double l_dbl = StringHelper::toDouble(m_EntryWidth.get_text());
	
	m_EntryWidth.set_text(StringHelper::doubleToString(l_dbl));
	m_KageStage.updateShapeWidth(l_dbl);
}

void Kage::EntryHeight_onEnter() {
	double l_dbl = StringHelper::toDouble(m_EntryHeight.get_text());
	
	m_EntryHeight.set_text(StringHelper::doubleToString(l_dbl));
	m_KageStage.updateShapeHeight(l_dbl);
}

void Kage::EntryNodeX_onEnter() {
	double l_dbl = StringHelper::toDouble(m_EntryNodeX.get_text());
	
	m_EntryNodeX.set_text(StringHelper::doubleToString(l_dbl));
	m_KageStage.updateNodeX(l_dbl);
}

void Kage::EntryNodeY_onEnter() {
	double l_dbl = StringHelper::toDouble(m_EntryNodeY.get_text());
	
	m_EntryNodeY.set_text(StringHelper::doubleToString(l_dbl));
	m_KageStage.updateNodeY(l_dbl);
}

void Kage::ToggleLine_onClick() {
	Kage::timestamp_IN(); std::cout << " Kage::ToggleLine_onClick" << std::endl;
	if (m_KageStage.toggleLineSelectedNodes() == true) {
		forceRenderFrames();
	}
	Kage::timestamp_OUT();
}

void Kage::addToolButton(const Glib::ustring &label) {
	toggleButtons.push_back(Gtk::manage(new Gtk::ToggleButton()));
		m_VBoxToolbar_Holder.pack_start(*toggleButtons.back(), Gtk::PACK_SHRINK);
			Glib::RefPtr<Gdk::Pixbuf> l_pixbuf;
//			l_pixbuf = Gdk::Pixbuf::create_from_resource("/kage/share/icons/" + label + "_tango.png");
			l_pixbuf = Gdk::Pixbuf::create_from_resource("/kage/share/icons/" + label + ".png");
			
			toggleButtons_pixbuf.push_back(l_pixbuf);
			toggleButtons_img.push_back(Gtk::Image((toggleButtons_pixbuf.back())));
				(*toggleButtons.back()).set_image((toggleButtons_img.back()));
				toggleButtons_img.back().show();
				
				(*toggleButtons.back()).property_always_show_image();
				(*toggleButtons.back()).show();
			
			(*toggleButtons.back()).set_focus_on_click(false);
			(*toggleButtons.back()).set_tooltip_text(label + " Tool");
			(*toggleButtons.back()).signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &Kage::ToolButtons_onClick), toggleButtons.back()));
}

void Kage::btnDebug_onClick() {
	//print vectors in Stage
	m_KageStage.printVectors();
}

void Kage::btnAbout_onClick() {
	KageAbout* pDialog = new KageAbout(*this);
		pDialog->run();
	delete pDialog;
}

void Kage::CheckUpdate_onClick() {
	//TODO: Use WGET to query and get latest version, for now:
	openWebsite(KageAbout::app_website + "/update.php?version="+KageAbout::app_version);
}

void Kage::Website_onClick() {
	openWebsite(KageAbout::app_website);
}

void Kage::openWebsite(string p_url) {
	GError *error = NULL;
	gtk_show_uri_on_window(NULL, p_url.c_str(), gtk_get_current_event_time(), &error);
	if (error) {
		cout << error->message << endl;
		cout << "Launching default web browser..." << endl;
		runExternal("start", p_url);
	}
}

bool Kage::fileWrite(std::string p_path, std::string p_msg, std::ofstream &p_file, bool &p_flag) {
	if (p_flag == false) {
		p_file.open(p_path.c_str(), ios::trunc);
		p_flag = true;
	} else {
		if (p_file.is_open() == false) {
			p_file.open(p_path.c_str(), ios::app);
		}
	}
	
	if (p_file.is_open() == false) {
		return false;
	}
	
	p_file << p_msg << std::endl;
	
	return true;
}

bool Kage::dump(string p_path, string p_msg) {
	return fileWrite(p_path, p_msg, ksfFile, ksfInited);
}
bool Kage::saveKageStudio(string p_path, string p_msg) {
	int l_len = strlen(p_path.c_str()) - 4;
	if (StringHelper::toLower(p_path).substr(l_len, 4) != ".ksf") {
		p_path = p_path + ".ksf";
	}
	
	return fileWrite(p_path, p_msg, ksfFile, ksfInited);
}

bool Kage::exportHtml5(string p_path, string p_msg) {
	int l_len = strlen(p_path.c_str()) - 5;
	if (StringHelper::toLower(p_path).substr(l_len, 5) != ".html") {
		p_path = p_path + ".html";
	}
	
	return fileWrite(p_path, p_msg, expFile, expInited);
}
bool Kage::exportSvg(string p_path, string p_msg) {
	int l_len = strlen(p_path.c_str()) - 4;
	if (StringHelper::toLower(p_path).substr(l_len, 4) != ".svg") {
		p_path = p_path + ".svg";
	}
	
	return fileWrite(p_path, p_msg, expFile, expInited);
}

bool Kage::exportKonsolScript(string p_path, string p_msg) {
	int l_len = strlen(p_path.c_str()) - 3;
	if (StringHelper::toLower(p_path).substr(l_len, 3) != ".ks") {
		p_path = p_path + ".ks";
	}
	
	return fileWrite(p_path, p_msg, ksfFile, ksfInited);
}

bool Kage::dtrace(string p_msg) {
	return dump(ksfPath, p_msg);
}

void Kage::EntryStageArea_onEnter() {
	string t = m_EntryStageWid.get_text();
		m_KageStage.stageWidth = StringHelper::toDouble(t);
		m_EntryStageWid.set_text(StringHelper::doubleToString(m_KageStage.stageWidth));
	t = m_EntryStageHgt.get_text();
		m_KageStage.stageHeight = StringHelper::toDouble(t);
		m_EntryStageHgt.set_text(StringHelper::doubleToString(m_KageStage.stageHeight));
	m_KageStage.invalidateToRender();
}
void Kage::EntryStageFPS_onEnter() {
	string t = m_EntryStageFPS.get_text();
	cout << "m_KageStage.fps " << m_KageStage.fps << " t " << t << endl;
	m_KageStage.fps = StringHelper::toUnsignedInteger(t);
	cout << "m_KageStage.fps ..." << m_KageStage.fps << " t " << t << endl;
	m_EntryStageFPS.set_text(StringHelper::unsignedIntegerToString(m_KageStage.fps));
}

void Kage::ColorButtonStage_onClick() {
	m_Color = m_ColorButtonStage.get_color();
//	m_KageStage.modify_bg(Gtk::STATE_NORMAL, m_Color);
	m_KageStage.setStageBG(m_Color);
}
void Kage::ColorButtonFill_onClick() {
	m_Color = m_ColorButtonFill.get_color(); //<-- deprecate this
	Gdk::RGBA l_rgba = m_ColorButtonFill.get_rgba();
	double l_red = l_rgba.get_red();
	double l_green = l_rgba.get_green();
	double l_blue = l_rgba.get_blue();
	double l_alpha = l_rgba.get_alpha();
	
	_scaleFillR.set_value(l_red * 255.0f);
	_scaleFillG.set_value(l_green * 255.0f);
	_scaleFillB.set_value(l_blue * 255.0f);
	_scaleFillAplha.set_value(l_alpha * 255.0f);
	KageStage::fillColor.setR((int) (l_red * 255.0f));
	KageStage::fillColor.setG((int) (l_green * 255.0f));
	KageStage::fillColor.setB((int) (l_blue * 255.0f));
	KageStage::fillColor.setA((int) (l_alpha * 255.0f));
	
	m_KageStage.setFill(m_Color);
	m_KageStage.updateShapeColor(_UPDATE_SHAPE_COLORS, _UPDATE_SHAPE_COLORS);
}

void Kage::ColorButtonStroke_onClick() {
	m_Color = m_ColorButtonStroke.get_color(); //<-- deprecate this
	Gdk::RGBA l_rgba = m_ColorButtonStroke.get_rgba();
	double l_alpha = l_rgba.get_alpha();
	
	_scaleStrokeAlpha.set_value(l_alpha * 100.0f);
	KageStage::stroke.setA((int) (l_alpha * 255.0f));
	
	m_KageStage.setStroke(m_Color);
	m_KageStage.updateShapeColor(_UPDATE_SHAPE_COLORS, _UPDATE_SHAPE_COLORS);
}

void Kage::updateSelectedShapeColor(bool p_doFill, bool p_doStroke) {
	double l_alphaValue;
	if (p_doFill == true) {
		m_Color = m_ColorButtonFill.get_color();
		m_KageStage.setFill  (m_Color);
		
		l_alphaValue = _scaleFillAplha.get_value();
		m_KageStage.fillColor.setA((int) (l_alphaValue * 255.0f/ 100.0f));
	}
	if (p_doStroke == true) {
		m_Color = m_ColorButtonStroke.get_color();
		m_KageStage.setStroke(m_Color);
		
		l_alphaValue = _scaleFillAplha.get_value();
		m_KageStage.stroke.setA((int) (l_alphaValue * 255.0f/ 100.0f));
		
		unsigned int l_uint = StringHelper::toUnsignedInteger(m_EntryStrokeThickness.get_text());
		KageStage::stroke.setThickness(l_uint);
	}
	m_KageStage.updateShapeColor(p_doFill, p_doStroke);
}

string Kage::saveFrame() {
	vector<VectorData> v = getFrameData(true).getVectorData();
	
	ostringstream l_ostringstream;
	unsigned int vsize = v.size();//-1;
	unsigned int fillCtr = 0;
	ColorData fcolor;
	StrokeColorData scolor;
	StrokeColorData scolorPrev;
	PointData p;
	bool doStroke = false;
	for (unsigned int i = 0; i < vsize; ++i) {
		switch (v[i].vectorType) {
			case VectorData::TYPE_CLOSE_PATH:
				l_ostringstream << "\t<closepath/>\n";
				break;
			case VectorData::TYPE_INIT:
				if (v[i].points.size() == 1) {
					l_ostringstream << "<init>" << v[i].points[0].x << " " << v[i].points[0].y << "</init>\n";
				} else {
					l_ostringstream << "<init/>\n";
				}
				break;
			case VectorData::TYPE_TEXT: break;
			case VectorData::TYPE_FILL:
				fcolor = v[i].fillColor;
					l_ostringstream << "<fill color=\"rgba" << v[i].fillColor.toString() << "\">\n";
				fillCtr++;
				break;
			case VectorData::TYPE_ENDFILL:
//				if (fillCtr > 0) {
						l_ostringstream << "</fill>\n";
//				}
				if (fillCtr > 0) {
					fillCtr--;
				}
				break;
			case VectorData::TYPE_STROKE:
				l_ostringstream << "\t<stroke color=\"rgba" << ColorData(v[i].stroke).toString() << "\" thickness=\"" + StringHelper::doubleToString(v[i].stroke.getThickness()) + "\"/>\n";
				scolor = v[i].stroke;
					if (!scolorPrev.equalTo(scolor) || scolorPrev.getThickness() != scolor.getThickness()) {
						doStroke = true;
						scolorPrev = scolor.clone();
					}
				break;
			case VectorData::TYPE_MOVE:
				l_ostringstream << "\t<move>" << v[i].points[0].x << " " << v[i].points[0].y << "</move>\n";
				break;
			case VectorData::TYPE_LINE: //currently not used
				l_ostringstream << "\t<line>" << v[i].points[0].x << " " << v[i].points[0].y << "</line>\n";
				break;
			case VectorData::TYPE_CURVE_QUADRATIC:
			case VectorData::TYPE_CURVE_CUBIC:
				l_ostringstream << "\t<curve>" << v[i].points[0].x << " " << v[i].points[0].y << " " << v[i].points[1].x << " " << v[i].points[1].y << " " << v[i].points[2].x << " " << v[i].points[2].y << "</curve>\n";
				break;
			case VectorData::TYPE_IMAGE:
				//2 '1st is for X/Y, 2nd is for width/height  -- ?!?
				break;
		}
	}
	return l_ostringstream.str();
}

string Kage::dumpFrame(bool bKS) {
	vector<VectorData> v = getFrameData(true).getVectorData();
	
	ostringstream l_ostringstream;
	unsigned int vsize = v.size();//-1;
	unsigned int fillCtr = 0;
	ColorData fcolor;
	ColorData fcolorPrev(0, 0, 0);
	StrokeColorData scolor;
	StrokeColorData scolorPrev(0, 0, 0);
		scolorPrev.setThickness(0);
	PointData p;
	bool l_donePrevFColor = false;
	bool l_initPrevFColor = false;
	bool l_donePrevSColor = false;
	bool l_initPrevSColor = false;
	bool l_donePrevLine = false;
	for (unsigned int i = 0; i < vsize; ++i) {
		switch (v[i].vectorType) {
			case VectorData::TYPE_CLOSE_PATH:
				l_ostringstream << "screen.closePath();\n";
				break;
			case VectorData::TYPE_INIT: break;
			case VectorData::TYPE_TEXT: break;
			case VectorData::TYPE_FILL:
				fcolor = v[i].fillColor;
				if (bKS == true) {
					l_ostringstream << "\nDraw:BeginFill(0x" << int255ToHex(fcolor.getR()) << int255ToHex(fcolor.getG()) << int255ToHex(fcolor.getB()) << ", " << fcolor.getA() << ", screen)\n";
				} else {
					l_ostringstream << "\nscreen.beginPath();\n";
				}
				l_donePrevFColor = false;
				fillCtr++;
				break;
			case VectorData::TYPE_ENDFILL:
				if (fillCtr > 0) {
					if (bKS == true) {
						l_ostringstream << "Draw:EndFill(screen)\n";
					} else {
						if (l_initPrevFColor == false || fcolorPrev.equalTo(fcolor) == false && l_donePrevFColor == false) {
							if (fcolor.getA() == 255) {
								l_ostringstream << "screen.fillStyle = '#" << int255ToHex(fcolor.getR()) << int255ToHex(fcolor.getG()) << int255ToHex(fcolor.getB()) << "';\n";
							} else {
								l_ostringstream << "screen.fillStyle = 'rgba(" << fcolor.getR() << "," << fcolor.getG() << "," << fcolor.getB() << "," << (fcolor.getA()/255.0f) << ")';\n";
							}
							l_donePrevFColor = true;
							l_initPrevFColor = true;
						}
						l_ostringstream << "screen.fill();\n";
						fcolorPrev = fcolor.clone();
					}
				}
					if (scolor.getThickness() > 0) {
						if (bKS == false) {
							if (l_initPrevSColor == false || scolorPrev.equalTo(scolor) == false && l_donePrevSColor == false) {
								if (scolor.getA() == 255) {
									l_ostringstream << "screen.strokeStyle = '#" << int255ToHex(scolor.getR()) << int255ToHex(scolor.getG()) << int255ToHex(scolor.getB()) << "';\n";
								} else {
									l_ostringstream << "screen.strokeStyle = 'rgba(" << scolor.getR() << "," << scolor.getG() << "," << scolor.getB() << "," << (scolor.getA()/255.0f) << ")';\n";
								}
								l_donePrevSColor = true;
								l_initPrevSColor = true;
							}
							if (scolorPrev.getThickness() != scolor.getThickness() && l_donePrevLine == false) {
								l_ostringstream << "screen.lineWidth = " << scolor.getThickness() << ";\n";
								l_ostringstream << "screen.lineCap = 'round';\n";
								l_donePrevLine = true;
							}
							l_ostringstream << "screen.stroke();\n";
							scolorPrev = scolor.clone();
						}
					}
				if (fillCtr > 0) {
					fillCtr--;
				}
				break;
			case VectorData::TYPE_STROKE:
				scolor = v[i].stroke.clone();
				if (bKS == true) {
					l_ostringstream << "Draw:LineStyle(" << scolor.getThickness() << ", 0x" << int255ToHex(scolor.getR()) << int255ToHex(scolor.getG()) << int255ToHex(scolor.getB()) << ", " << scolor.getA() << ", screen)\n";
				} else {
					if (scolorPrev.equalTo(scolor) == false) {
						l_donePrevSColor = false;
					}
					if (scolorPrev.getThickness() != scolor.getThickness()) {
						l_donePrevLine = false;
					}
				}
				break;
			case VectorData::TYPE_MOVE:
				if (bKS == true) {
					l_ostringstream << "Draw:MoveTo(" << v[i].points[0].x << ", " << v[i].points[0].y << ", screen)\n";
				} else {
					l_ostringstream << "screen.moveTo(" << v[i].points[0].x << ", " << v[i].points[0].y << ");\n";
				}
				
				p.x = v[i].points[0].x;
				p.y = v[i].points[0].y;
				break;
			case VectorData::TYPE_LINE:
				if (bKS == true) {
					l_ostringstream << "Draw:LineTo(" << v[i].points[0].x << ", " << v[i].points[0].y << ", screen)\n";
				} else {
					l_ostringstream << "screen.lineTo(" << v[i].points[0].x << ", " << v[i].points[0].y << ");\n";
				}
				
				p.x = v[i].points[0].x;
				p.y = v[i].points[0].y;
				break;
			case VectorData::TYPE_CURVE_QUADRATIC:
				if (bKS == true) {
					l_ostringstream << "Draw:CurveTo(" << p.x << ", " << p.y << ", " << v[i].points[0].x << ", " << v[i].points[0].y << ", " << v[i].points[1].x << ", " << v[i].points[1].y << ", screen)\n";
				} else {
					l_ostringstream << "screen.quadraticCurveTo(" << p.x << ", " << p.y << ", " << v[i].points[0].x << ", " << v[i].points[0].y << ", " << v[i].points[1].x << ", " << v[i].points[1].y << ");\n";
				}
				
				p.x = v[i].points[1].x;
				p.y = v[i].points[1].y;
				break;
			case VectorData::TYPE_CURVE_CUBIC:
				if (bKS == true) {
//					l_ostringstream << "Draw:CubicCurveTo(" << p.x << ", " << p.y << ", " << v[i].points[0].x << ", " << v[i].points[0].y << ", " << v[i].points[1].x << ", " << v[i].points[1].y << ", screen)\n";
					l_ostringstream << "Draw:CubicCurveTo(" << v[i].points[0].x << ", " << v[i].points[0].y << ", " << v[i].points[1].x << ", " << v[i].points[1].y << ", " << v[i].points[2].x << ", " << v[i].points[2].y << ", screen)\n";
				} else {
//					l_ostringstream << "screen.bezierCurveTo(" << p.x << ", " << p.y << ", " << v[i].points[0].x << ", " << v[i].points[0].y << ", " << v[i].points[1].x << ", " << v[i].points[1].y << ");\n";
					l_ostringstream << "screen.bezierCurveTo(" << v[i].points[0].x << ", " << v[i].points[0].y << ", " << v[i].points[1].x << ", " << v[i].points[1].y << ", " << v[i].points[2].x << ", " << v[i].points[2].y << ");\n";
				}
				//p.x = v[i].points[1].x;
				//p.y = v[i].points[1].y;
				break;
			case VectorData::TYPE_IMAGE:
				//2 '1st is for X/Y, 2nd is for width/height  -- ?!?
				break;
		}
	}
	return l_ostringstream.str();
}

string Kage::dumpFrameToSvg() {
	vector<VectorData> v = getFrameData(true).getVectorData();
	
	ostringstream l_ostringstream;
	unsigned int vsize = v.size();
	unsigned int fillCtr = 0;
	ColorData fcolor;
	ColorData fcolorPrev(0, 0, 0);
	StrokeColorData scolor;
	StrokeColorData scolorPrev(0, 0, 0);
		scolorPrev.setThickness(0);
	PointData p;
	bool l_donePrevFColor = false;
	bool l_initPrevFColor = false;
	bool l_donePrevSColor = false;
	bool l_initPrevSColor = false;
	bool l_donePrevLine = false;
	bool l_initFlag = false;
	for (unsigned int i = 0; i < vsize; ++i) {
		switch (v[i].vectorType) {
			case VectorData::TYPE_CLOSE_PATH:
				l_ostringstream << "\"";
				if (fillCtr > 0) {
					l_ostringstream << " fill=\"#" << int255ToHex(fcolor.getR()) << int255ToHex(fcolor.getG()) << int255ToHex(fcolor.getB()) << "\"";

					fcolorPrev = fcolor.clone();
				}
					if (scolor.getThickness() > 0) {
						l_ostringstream << " stroke=\"#" << int255ToHex(scolor.getR()) << int255ToHex(scolor.getG()) << int255ToHex(scolor.getB()) << "\"";
						l_ostringstream << " stroke-width=\"" << scolor.getThickness() << "\"";
						
						scolorPrev = scolor.clone();
					}
				if (fillCtr > 0) {
					fillCtr--;
				}
				l_ostringstream << " />\n";
				
				break;
			case VectorData::TYPE_INIT:
				if (l_initFlag == true) {
					l_ostringstream << "\t</g>\n";
					l_initFlag = false;
				}
				
				l_initFlag = true;
				if (v[i].points.size() == 1) {
					l_ostringstream << "\t<g inkscape:transform-center-x=\"" << StringHelper::doubleToString(v[i].points[0].x) << "\" " <<
						                     "inkscape:transform-center-y=\"" << StringHelper::doubleToString(v[i].points[0].y) << "\">\n";
				} else {
					l_ostringstream << "\t<g>\n";
				}
				break;
			case VectorData::TYPE_TEXT: break;
			case VectorData::TYPE_FILL:
				fcolor = v[i].fillColor;
				l_ostringstream << "\t\t<path d=\"";
				
				l_donePrevFColor = false;
				fillCtr++;
				break;
			case VectorData::TYPE_ENDFILL: break;
			case VectorData::TYPE_STROKE:
				scolor = v[i].stroke.clone();
					if (scolorPrev.equalTo(scolor) == false) {
						l_donePrevSColor = false;
					}
					if (scolorPrev.getThickness() != scolor.getThickness()) {
						l_donePrevLine = false;
					}
				break;
			case VectorData::TYPE_MOVE:
				l_ostringstream << " M " << v[i].points[0].x << " " << v[i].points[0].y;
				
				p.x = v[i].points[0].x;
				p.y = v[i].points[0].y;
				break;
			case VectorData::TYPE_LINE:
				l_ostringstream << " L " << v[i].points[0].x << " " << v[i].points[0].y;
				
				p.x = v[i].points[0].x;
				p.y = v[i].points[0].y;
				break;
			case VectorData::TYPE_CURVE_QUADRATIC:
				l_ostringstream << " Q " << p.x << " " << p.y << " " << v[i].points[0].x << " " << v[i].points[0].y << " " << v[i].points[1].x << " " << v[i].points[1].y;
				
				p.x = v[i].points[1].x;
				p.y = v[i].points[1].y;
				break;
			case VectorData::TYPE_CURVE_CUBIC:
				l_ostringstream << "C " << v[i].points[0].x << " " << v[i].points[0].y << " " << v[i].points[1].x << " " << v[i].points[1].y << " " << v[i].points[2].x << " " << v[i].points[2].y;
				
				break;
			case VectorData::TYPE_IMAGE:
				//2 '1st is for X/Y, 2nd is for width/height  -- ?!?
				break;
		}
	}
	
	if (l_initFlag == true) {
		l_ostringstream << "\t</g>";
	}
	return l_ostringstream.str();
}

string Kage::int255ToHex(unsigned int p) {
	int l;
	string r = "";
	string rr = "";
	l = ((int)p / 16);
	if (l > 0) {
		rr = int15ToHex(l);
		r = int15ToHex(p - (l * 16));
	} else {
		rr = "0";
		r = int15ToHex(p);
	}
	return rr + r;
}

unsigned int Kage::hexToInt255(string p) {
	unsigned int l_A = 0;
	unsigned int l_B = 0;

	string r  = "0";
	string rr = "0";

	if (p.length() > 1) {
		rr = p.substr(0,1); //if AB rr gets A
		r  = p.substr(1,1); //if AB r  gets B
	}
	if (p.length() == 1) {
		r  = p;
	}
	cout << "\n\t A " << rr << "\t" << r << endl;
	l_A = hexToInt15(rr[0]) * 16;
	l_B = hexToInt15(r[0]);
	cout << "\t B " << l_A << "\t" << l_B << "\t" << (l_A + l_B) << endl;
	return l_A + l_B;
}

char Kage::int15ToHex(unsigned int p) {
	if (p > 15) {
		return ' ';
	} else if (p == 15) {
		return 'F';
	} else if (p == 14) {
		return 'E';
	} else if (p == 13) {
		return 'D';
	} else if (p == 12) {
		return 'C';
	} else if (p == 11) {
		return 'B';
	} else if (p == 10) {
		return 'A';
	} else if (p > 0) {
		return p+48; //change 48 to ASCII of 1
	} else if (p == 0) {
		return '0';
	}
	return '\0';
}

unsigned int Kage::hexToInt15(char p) {
	if (p == 'F') {
		return 15;
	} else if (p == 'E') {
		return 14;
	} else if (p == 'D') {
		return 13;
	} else if (p == 'C') {
		return 12;
	} else if (p == 'B') {
		return 11;
	} else if (p == 'A') {
		return 10;
	} else if (p > 48 && p <= 57) {
		return p-48; //change ASCII 1 to 48-48
	}
	
	return 0;
}

string Kage::openTextFile(string p_path) {
	char ch;
	string l_string = "";
	fstream fin(p_path, fstream::in);
	while (fin >> noskipws >> ch) {
		l_string += ch;
	}
	return l_string;
}

vector<double> Kage::parseNumbers(string p_numbers) {
	vector<string> l_XYs = StringHelper::split(p_numbers, " ");
	vector<double> l_numbers;
	for (unsigned int i = 0; i < l_XYs.size(); ++i) {
		l_numbers.push_back(StringHelper::toDouble(l_XYs[i]));
	}
	return l_numbers;
}
vector<int> Kage::parseColorString(string p_color) {
	vector<int> l_colors;
	if (p_color.length() > 4 && p_color.substr(0, 5) == "rgba(") {
		vector<string> l_rgba = StringHelper::split(p_color.substr(5), ", ");
		l_colors.push_back(StringHelper::toInteger(l_rgba[0]));
		l_colors.push_back(StringHelper::toInteger(l_rgba[1]));
		l_colors.push_back(StringHelper::toInteger(l_rgba[2]));
		l_colors.push_back(StringHelper::toInteger(StringHelper::split(l_rgba[3], ")")[0]));
	} else if (p_color.length() > 3 && p_color.substr(0, 4) == "rgb(") {
		vector<string> l_rgb = StringHelper::split(p_color.substr(4), ", ");
		l_colors.push_back(StringHelper::toInteger(l_rgb[0]));
		l_colors.push_back(StringHelper::toInteger(l_rgb[1]));
		l_colors.push_back(StringHelper::toInteger(StringHelper::split(l_rgb[2], ")")[0]));
	} else {
		cout << "?!?" << p_color << "\n";
	}
	return l_colors;
}

void Kage::parseKSF_Children(vector<XmlTag> p_children) {
	for (unsigned int i = 0; i < p_children.size(); ++i) {
		string l_tagname = p_children[i].getName();
		vector<XmlTagProperty> l_properties = p_children[i].getProperties();
		if (l_tagname.substr(0, 5) == "layer") {
			unsigned int l_layer = StringHelper::toUnsignedInteger(l_tagname.substr(5));
			cout << "\t\t\t\t\tl_tagname LAYER\t" << l_layer << "\t" << l_properties.size() << endl;
			while (l_layer > _framesetManager.layerCount()) {
				_framesetManager.addFrameset(_layerManager.addLayer());
				_framesetManager.setCurrentLayer(_layerManager.layerCount());
			}
			
			for (unsigned int l_propertyIndex = 0; l_propertyIndex < l_properties.size(); ++l_propertyIndex) {
				cout << "\t\t\t\t\t\tA l_properties[" << l_propertyIndex << "].getName() " << l_properties[l_propertyIndex].getName() << " = " << l_properties[l_propertyIndex].getValue() << " ? " << StringHelper::toBoolean(l_properties[l_propertyIndex].getValue()) << endl;
				if (l_properties[l_propertyIndex].getName() == "label") {
					_layerManager.setLabel(l_properties[l_propertyIndex].getValue());
				} else if (l_properties[l_propertyIndex].getName() == "visible") {
					_layerManager.setVisible(StringHelper::toBoolean(l_properties[l_propertyIndex].getValue()));
				} else if (l_properties[l_propertyIndex].getName() == "locked") {
					_layerManager.setLock(StringHelper::toBoolean(l_properties[l_propertyIndex].getValue()));
				}
			}
			_framesetManager.setCurrentLayer(l_layer);
		} else if (l_tagname.substr(0, 5) == "frame") {
			unsigned int l_frame = StringHelper::toUnsignedInteger(l_tagname.substr(5));
			while (l_frame > _framesetManager.frameCount()) {
				_framesetManager.addFrame();
			}
			_framesetManager.setCurrentFrame(l_frame);
			for (unsigned int l_propertyIndex = 0; l_propertyIndex < l_properties.size(); ++l_propertyIndex) {
				if (l_properties[l_propertyIndex].getName() == "extend") {
					if (l_properties[l_propertyIndex].getValue() == "start") {
						_framesetManager.setFrameExtension(KageFrame::EXTENSION_START);
					} else if (l_properties[l_propertyIndex].getValue() == "mid") {
						_framesetManager.setFrameExtension(KageFrame::EXTENSION_MID);
					} else if (l_properties[l_propertyIndex].getValue() == "end") {
						_framesetManager.setFrameExtension(KageFrame::EXTENSION_END);
					}
				} else if (l_properties[l_propertyIndex].getName() == "tween") {
					string l_tweenValue = l_properties[l_propertyIndex].getValue();
					if (l_tweenValue == "true") {
						_framesetManager.forceSetTween(11);
					} else if (l_tweenValue == "false") {
						_framesetManager.forceSetTween(0);
					} else {
						_framesetManager.forceSetTween(StringHelper::toUnsignedInteger(l_tweenValue));
					}
				}
			}
		} else if (l_tagname == "init") {
			VectorDataManager v;
			
			vector<double> l_numbers = parseNumbers(p_children[i]._value); //XY for rotation anchor
			if (l_numbers.size() == 2) {
				v.addInit(PointData(l_numbers[0], l_numbers[1]));
			} else {
				v.addInit();
			}
			addDataToFrame(v, true);
		} else if (l_tagname == "fill") {
			VectorDataManager v;
			string l_color = l_properties[0].getValue(); 
			if (l_color.length() > 3 && l_color.substr(0, 3) == "rgb") {
				//RGB/RGBA
				vector<int> l_colors = parseColorString(l_properties[0].getValue()); //color
					v.addFill(ColorData(l_colors[0], l_colors[1], l_colors[2], l_colors[3]));
			} else {
				//possible gradient ID
				v.addFill(l_color);
			}
			addDataToFrame(v, true);
		} else if (l_tagname == "stroke") {
			VectorDataManager v;
			vector<int> l_colors = parseColorString(l_properties[0].getValue()); //color
				StrokeColorData l_stroke = StrokeColorData(l_colors[0], l_colors[1], l_colors[2], l_colors[3]);
				l_stroke.setThickness(StringHelper::toDouble(l_properties[1].getValue()));//thickness
				v.addLineStyle(l_stroke);
			addDataToFrame(v, true);
		} else if (l_tagname == "move") {
			VectorDataManager v;
			vector<double> l_numbers = parseNumbers(p_children[i]._value); //XY
				v.addMove(PointData(l_numbers[0], l_numbers[1]));
			addDataToFrame(v, true);
		} else if (l_tagname == "cubiccurve" || l_tagname == "curve") {
			VectorDataManager v;
			vector<double> l_numbers = parseNumbers(p_children[i]._value); //XYs
				v.addCubic(
					PointData(l_numbers[0], l_numbers[1]),
					PointData(l_numbers[2], l_numbers[3]),
					PointData(l_numbers[4], l_numbers[5]));
			addDataToFrame(v, true);
		} else if (l_tagname == "closepath") {
			VectorDataManager v;
				v.addClosePath();
			addDataToFrame(v, true);
		} else if (l_tagname == "stage") {
			for (unsigned int j = 0; j < l_properties.size(); ++j) {
				if (l_properties[j].getName() == "width") {
					m_KageStage.stageWidth = StringHelper::toUnsignedInteger(l_properties[j].getValue());
					m_EntryStageWid.set_text(StringHelper::unsignedIntegerToString(m_KageStage.stageWidth));
				} else if (l_properties[j].getName() == "height") {
					m_KageStage.stageHeight = StringHelper::toUnsignedInteger(l_properties[j].getValue());
					m_EntryStageHgt.set_text(StringHelper::unsignedIntegerToString(m_KageStage.stageHeight));
				} else if (l_properties[j].getName() == "fps") {
					m_KageStage.fps = StringHelper::toUnsignedInteger(l_properties[j].getValue());
					m_EntryStageFPS.set_text(StringHelper::unsignedIntegerToString(m_KageStage.fps));
				} else if (l_properties[j].getName() == "background") {
					vector<int> l_colors = parseColorString(l_properties[j].getValue());
					m_KageStage.stageBG = ColorData(l_colors[0], l_colors[1], l_colors[2]);
					m_ColorButtonStage.set_color(m_KageStage.getStageBG());
				}
			}
		}
		parseKSF_Children(p_children[i]._children);
		if (l_tagname == "fill") {
			VectorDataManager v;
				v.addEndFill();
			addDataToFrame(v, true);
		}
	}
	m_KageStage.invalidateToRender();
	refreshUI();
}
void Kage::parseKSF(string p_content) {
	BasicXml _xml;
	std::cout << "parsing... " << p_content.length() << std::endl;
	if (_xml.parse(p_content)) {
		if (_xml.tokenize()) {
			XmlTag l_root = _xml.getRoot();
			if (l_root.getName() == "KageStudio") {
				vector<XmlTagProperty> l_xmlTagProperties = l_root.getProperties();
				if (l_xmlTagProperties.size() > 0
						&& l_xmlTagProperties[0].getName() == "version"
						&&
						(	   l_xmlTagProperties[0].getValue() == "2019.10.14"
							|| l_xmlTagProperties[0].getValue() == "2020.03.10"
						)
					) {
					KageFramesetManager::LOADING_MODE = true;
						parseKSF_Children(l_root._children);
						updateStatus("Loaded " + ksfPath);
					KageFramesetManager::LOADING_MODE = false;
				}
			}
		} else {
			//unable to tokenize KSF
			updateStatus("Loading Error 1: Unabe to load " + ksfPath);
		}
	} else {
		//unable to parse KSF
		updateStatus("Loading Error 2: Unabe to load " + ksfPath);
	}
}

bool Kage::runExternal(string p_cmd, string p_param) {
	string l_cmd = p_cmd + " " + p_param;
	
	if (system(NULL)) {
		int i = system(l_cmd.c_str());
		std::cout << "\n\n\nKage::runExternal i " << i << std::endl;
		std::cout << "\n\n\nl_cmd " << l_cmd << std::endl;
		if (i == 0) {
			// no problem
		} else if (i == 1) { //Windows
			return false; /* p_cmd does not exists*/
		} else if (i == 32512) { //GNU
			return false; /* p_cmd not found */
		}
		return true;
	}
	return false;
}

typedef std::chrono::system_clock Clock;

void Kage::timestamp() {
//	time_t rawtime;
//	time(&rawtime);
//	struct tm * timeinfo = localtime (&rawtime);
	
	auto now = Clock::now();
	auto seconds = std::chrono::time_point_cast<std::chrono::seconds>(now);
	auto fraction = now - seconds;
	time_t cnow = Clock::to_time_t(now);
	struct tm * timeinfo = localtime (&cnow);
	
	auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(fraction);
	
	if (timeinfo->tm_hour < 10) {
		std::cout << "0";
	}
	std::cout << timeinfo->tm_hour << ":";
	
	if (timeinfo->tm_min < 10) {
		std::cout << "0";
	}
	std::cout << timeinfo->tm_min << ":";
	
	if (timeinfo->tm_sec < 10) {
		std::cout << "0";
	}
	std::cout << timeinfo->tm_sec << ".";
	
	int l_ms = milliseconds.count();
	if (l_ms < 10) {
		std::cout << "0";
	}
	if (l_ms < 100) {
		std::cout << "0";
	}
	std::cout << l_ms << " ";
	
	//std::cout << timeinfoB->tm_hour << ":" << timeinfoB->tm_min << ":" << timeinfoB->tm_sec << "." << milliseconds.count();
	for (unsigned int i = 0; i < Kage::TAB_COUNT; ++i) {
		std::cout << "    ";
	}
}

void Kage::timestamp_IN() {
	Kage::TAB_COUNT++;
	#ifdef KAGE_DEBUG
		Kage::timestamp();
	#endif
}

void Kage::timestamp_OUT() {
	Kage::TAB_COUNT--;
	if (Kage::TAB_COUNT == -1) {
		Kage::TAB_COUNT = 0;
	}
}
