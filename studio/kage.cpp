
#include "kage.h"
#include "about.cpp"

const Glib::ustring app_title = "Kage Studio";

bool KageFrame::mouseIsDown = false;
ColorData KageStage::stageBG(255, 255, 255);
ColorData KageStage::fillColor(0, 0, 255, 255);
StrokeColorData KageStage::stroke(255, 0, 0, 255);
KageStage::ToolMode KageStage::toolMode = MODE_NONE;
GdkPoint KageStage::moveStageXY;
unsigned int PointData::debug_pts;

bool KageLayer::mouseIsDown = false;
Glib::RefPtr<Gdk::Pixbuf> KageLayer::imageVISIBLE_TRUE;
Glib::RefPtr<Gdk::Pixbuf> KageLayer::imageVISIBLE_FALSE;
Glib::RefPtr<Gdk::Pixbuf> KageLayer::imageLOCKED_TRUE;
Glib::RefPtr<Gdk::Pixbuf> KageLayer::imageLOCKED_FALSE;
				
Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageNULL;
Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageNULL_CUR;
Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageBLANK;
Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageBLANK_CUR;
Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageDRAWN;
Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageDRAWN_CUR;
Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageEXTENSION;
Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageEXTENSION_CUR;
Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageSELECTED;
Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageSELECTED_CUR;

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

unsigned int KageFramesManager::currentFrame;
unsigned int VectorDataManager::idmaker;

Kage::Kage() : m_KageLayerManager(this),
			   m_KageFramesManager(this),
			   m_LabelProp("Properties"),
			   m_LabelStageWid("Width"),
			   m_LabelStageHgt("Height"),
			   m_LabelStageBG("Background"),
			   m_LabelStageFPS("FPS"),
			   m_LabelFill("Fill"),
			   m_LabelFillAlpha("Fill Alpha"),
			   m_LabelStroke("Stroke"),
			   m_LabelStrokeAlpha("Stroke Alpha"),
			   m_LabelStrokeThickness("Thickness"),
			   m_LabelX("X"),
			   m_LabelY("Y"),
			   m_LabelWidth("Width"),
			   m_LabelHeight("Height"),
			   m_LabelNodeX("X"),
			   m_LabelNodeY("Y"),
			   m_LabelToggleLine("Straighten"),
			   m_LblHolder_Toolbar("Toolbar"),
			   m_KageStage(this),
			   _undoRedoManager() {
	m_ContextId = m_Statusbar.get_context_id(app_title);
	
	set_title("Kage Studio");
	set_icon(Gdk::Pixbuf::create_from_resource("/kage/share/icons/default.png"));
	set_border_width(0);
	
	//workaround for known bug about resizing/maximizing main window with gtk3.22-9
	set_default_size(1920,1080);
	maximize();
	//Define the actions:
	m_refActionGroup = Gtk::ActionGroup::create("Actions"); //It also works with no name, which is probably better if there is only one.
	
	//In real life, the details would not be supplied _in addition _ the stock IDs.
	
	//Add normal Actions:
	m_refActionGroup->add( Gtk::Action::create("FileMenu", "_File") ); 
	m_refActionGroup->add( Gtk::Action::create("ExportMenu", "_Export...") ); 
	m_refActionGroup->add( Gtk::Action::create("ExportPNGMenu", "_PNG...") ); 
	m_refActionGroup->add( Gtk::Action::create("EditMenu", "_Edit") );
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
		Gtk::Action::create("Quit", Gtk::Stock::QUIT, "_Quit", "Quit"),
		sigc::mem_fun(*this, &Kage::onButtonClick)
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
		Gtk::AccelKey("<control>X"),
		sigc::mem_fun(*this, &Kage::Cut_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("Copy", "_Copy", "Copy"),
		Gtk::AccelKey("<control>C"),
		sigc::mem_fun(*this, &Kage::Copy_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("Paste", "_Paste", "Paste"),
		Gtk::AccelKey("<control>V"),
		sigc::mem_fun(*this, &Kage::Paste_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("Delete", "_Delete", "Delete"),
		Gtk::AccelKey("Delete"),
		sigc::mem_fun(*this, &Kage::Delete_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("Duplicate", "Duplic_ate", "Duplicate"),
		Gtk::AccelKey("<control>D"),
		sigc::mem_fun(*this, &Kage::Duplicate_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("SelectAll", "Select Al_l", "Select All"),
		Gtk::AccelKey("<control>A"),
		sigc::mem_fun(*this, &Kage::SelectAll_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("Deselect", "D_eselect", "Deselect"),
		Gtk::AccelKey("Escape"),
		sigc::mem_fun(*this, &Kage::Deselect_onClick)
	);
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
		Gtk::Action::create("FlipHorizontal", "Flip _Vertical", "FlipHorizontal"),
		Gtk::AccelKey("H"),
		sigc::mem_fun(*this, &Kage::FlipHorizontal_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("FlipVertical", "Flip _Vertical", "FlipVertical"),
		Gtk::AccelKey("V"),
		sigc::mem_fun(*this, &Kage::FlipVertical_onClick)
	);
	////Add Toggle Actions:
	//m_refActionGroup->add(
		//Gtk::ToggleAction::create("Bold", Gtk::Stock::BOLD, "_Bold", "Bold", true /* is_active */),
		//sigc::mem_fun(*this, &Kage::onActionActivate)
	//);
	
	m_refActionGroup->add(
		Gtk::Action::create("Play", "Play", "Play Animation"),
		Gtk::AccelKey("F9"),
		sigc::mem_fun(*this, &Kage::Play_onClick)
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
		sigc::mem_fun(*this, &Kage::onActionActivate)
	);
	m_refActionGroup->add(
		Gtk::Action::create("DuplicateFrame", "_Duplicate Frame", "Duplicate Frame"),
		Gtk::AccelKey("F6"),
		sigc::mem_fun(*this, &Kage::DuplicateFrame_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("AddLayer", "_Add Layer", "Add Layer"),
		Gtk::AccelKey("<control>F7"),
		sigc::mem_fun(*this, &Kage::LayerAdd_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("DeleteLayer", "_Delete Layer", "Delete Layer"),
		Gtk::AccelKey("<control>F8"),
		sigc::mem_fun(*this, &Kage::onActionActivate)
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
		Gtk::Action::create("Website", "_Website", "Visit Kage Studio Website"),
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
		Gtk::Action::create("About", "_About", "About Kage Studio"),
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
		"			<separator/>"
		"			<menu action='ExportMenu'>"
		"				<menuitem action='ExportAVI'/>"
		"				<menuitem action='ExportHTML5'/>"
		"				<menuitem action='ExportKS'/>"
		"				<menu action='ExportPNGMenu'>"
		"					<menuitem action='ExportPNG'/>"
		"					<menuitem action='ExportPNGTransparent'/>"
		"					<menuitem action='ExportPNGSequence'/>"
		"				</menu>"
		"			</menu>"
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
		"		</menu>"
		"		<menu action='TimelineMenu'>"
		"			<menuitem action='Play'/>"
		"			<menuitem action='NextFrame'/>"
		"			<menuitem action='PreviousFrame'/>"
		"			<separator/>"
		"			<menuitem action='AddFrame'/>"
		"			<menuitem action='ExtendFrame'/>"
		"			<menuitem action='DuplicateFrame'/>"
		"			<separator/>"
		"			<menuitem action='AddLayer'/>"
		"			<menuitem action='DeleteLayer'/>"
		"		</menu>"
		"		<menu action='ToolsMenu'>"
		"			<menuitem action='ToolSelect'/>"
		"			<menuitem action='ToolNode'/>"
		"			<menuitem action='ToolPoly'/>"
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
	m_HBoxToolbar.pack_start(m_VBoxToolbar_Holder, Gtk::PACK_SHRINK);
		m_VBoxToolbar_Holder.pack_start(m_LblHolder_Toolbar, Gtk::PACK_SHRINK);
		m_VBoxToolbar_Holder.pack_start(m_Separator_Toolbar1, Gtk::PACK_SHRINK);
		
		addToolButton("Select");
		addToolButton("Node");
//		addToolButton("Marquee"); //TODO: implement tool
		m_VBoxToolbar_Holder.pack_start(m_Separator_Toolbar2, Gtk::PACK_SHRINK);
//		addToolButton("Text"); //TODO: implement tool
		addToolButton("Poly");
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
				m_Timeline_HPaned.add1(m_Timeline_Layer_VBox);
					m_Timeline_Layer_VBox.pack_start(m_Timeline_Layer_ScrolledWindow);//, Gtk::PACK_SHRINK);
						m_Timeline_Layer_ScrolledWindow.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_NEVER);
							m_Timeline_Layer_ScrolledWindow.add(m_KageLayerManager);
							cout << "layer count " << m_KageLayerManager.layerCount() << endl;
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
				m_Timeline_HPaned.add2(m_Timeline_Frame_VBox1);
					m_Timeline_Frame_VBox1.pack_start(m_Timeline_Frame_VBox2, Gtk::PACK_EXPAND_WIDGET);
						m_Timeline_Frame_VBox2.pack_start(m_Timeline_Frame_ScrolledWindow);//, Gtk::PACK_SHRINK);
							m_Timeline_Frame_ScrolledWindow.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_NEVER);
								m_Timeline_Frame_ScrolledWindow.set_border_width(0);
								m_Timeline_Frame_ScrolledWindow.add(m_KageFramesManager);
								m_Timeline_Frame_ScrolledWindow.set_shadow_type(Gtk::SHADOW_NONE);
						m_Timeline_Frame_VBox2.pack_start(m_Timeline_HScrollbar, Gtk::PACK_SHRINK);
							m_Timeline_HScrollbar.set_adjustment(m_Timeline_Frame_ScrolledWindow.get_hadjustment());
					m_Timeline_Frame_VBox1.pack_start(m_Timeline_CurrentFrame, Gtk::PACK_SHRINK);
							m_Timeline_CurrentFrame.set_label("L 1 F 1");
							m_Timeline_CurrentFrame.set_size_request(20, 20);

			m_Timeline_HBox.pack_start(m_Timeline_VScrollbar, Gtk::PACK_SHRINK);
				m_Timeline_VScrollbar.set_adjustment(m_Timeline_Frame_ScrolledWindow.get_vadjustment());
					m_Timeline_Layer_ScrolledWindow.set_vadjustment(m_Timeline_Frame_ScrolledWindow.get_vadjustment());
					
		m_VPane_Timeline.add2(m_HPane_DrawingArea);
			m_HPane_DrawingArea.add1(m_KageStage);
			m_KageStage.set_size_request(1280, 700);
			m_KageStage.show();
			m_HPane_DrawingArea.add2(m_Box1);
			
			m_LabelProp.set_size_request(200, 20);
			m_LabelProp.set_alignment(Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
			m_Box1.pack_start(m_LabelProp, Gtk::PACK_SHRINK);
			
			m_Box1.pack_start(m_Separator, Gtk::PACK_SHRINK);
			//Stage
			m_Box1.pack_start(m_PropStage, Gtk::PACK_SHRINK);
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
						m_EntryStageWid.set_text(StringHelper::StringHelper::doubleToString(m_KageStage.stageWidth));
							m_EntryStageWid.signal_activate().connect(
								sigc::mem_fun(*this, &Kage::EntryStageArea_onEnter) );
						m_EntryStageHgt.set_size_request(20, 24);
						m_EntryStageHgt.set_width_chars(4);
						m_EntryStageHgt.set_max_length(4);
						m_EntryStageHgt.set_text(StringHelper::StringHelper::doubleToString(m_KageStage.stageHeight));
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
			//Fill & Stroke
			m_Box1.pack_start(m_PropFillStroke, Gtk::PACK_SHRINK);
				m_PropFillStroke.set_border_width(10);
				m_PropFillStroke.pack_start(m_PropFillStrokeV1);
					m_PropFillStrokeV1.set_border_width(4);
					m_PropFillStrokeV1.set_spacing(4);
					m_PropFillStrokeV1.pack_start(m_LabelFill);
					m_PropFillStrokeV1.pack_start(m_LabelFillAlpha);
					m_PropFillStrokeV1.pack_start(m_LabelStroke);
					m_PropFillStrokeV1.pack_start(m_LabelStrokeAlpha);
					m_PropFillStrokeV1.pack_start(m_LabelStrokeThickness);
				m_PropFillStroke.pack_start(m_PropFillStrokeV2);;
					m_PropFillStrokeV2.set_border_width(4);
					m_PropFillStrokeV2.set_spacing(4);
					m_PropFillStrokeV2.pack_start(m_ColorButtonFill);
						m_ColorButtonFill.set_size_request(32, 32);
						m_ColorButtonFill.set_use_alpha(true);
						m_ColorButtonFill.set_title("Choose Fill Color");
							m_ColorButtonFill.set_color(m_KageStage.getStageBG());
						m_ColorButtonFill.signal_color_set().connect(sigc::mem_fun(* this, &Kage::ColorButtonFill_onClick) );
					m_PropFillStrokeV2.pack_start(m_EntryFillAlpha, Gtk::PACK_SHRINK);
						m_EntryFillAlpha.set_size_request(20, 24);
						m_EntryFillAlpha.set_width_chars(3);
						m_EntryFillAlpha.set_max_length(3);
						m_EntryFillAlpha.set_text(StringHelper::integerToString((int)KageStage::fillColor.getA()));
						m_EntryFillAlpha.signal_activate().connect(
							sigc::mem_fun(*this, &Kage::EntryFillAlpha_onEnter));
					m_PropFillStrokeV2.pack_start(m_ColorButtonStroke);
						m_ColorButtonStroke.set_size_request(32, 32);
						m_ColorButtonStroke.set_use_alpha(true);
						m_ColorButtonStroke.set_title("Choose Stroke Color");
							m_ColorButtonStroke.set_color(m_KageStage.getStageBG());
						m_ColorButtonStroke.signal_color_set().connect(sigc::mem_fun(* this, &Kage::ColorButtonStroke_onClick) );
					m_PropFillStrokeV2.pack_start(m_EntryStrokeAlpha, Gtk::PACK_SHRINK);
						m_EntryStrokeAlpha.set_size_request(20, 24);
						m_EntryStrokeAlpha.set_width_chars(3);
						m_EntryStrokeAlpha.set_max_length(3);
						m_EntryStrokeAlpha.set_text(StringHelper::integerToString((int)KageStage::stroke.getA()));
						m_EntryStrokeAlpha.signal_activate().connect(
							sigc::mem_fun(*this, &Kage::EntryStrokeAlpha_onEnter));
					m_PropFillStrokeV2.pack_start(m_EntryStrokeThickness, Gtk::PACK_SHRINK);
						m_EntryStrokeThickness.set_size_request(20, 24);
						m_EntryStrokeThickness.set_width_chars(2);
						m_EntryStrokeThickness.set_max_length(2);
						m_EntryStrokeThickness.set_text(StringHelper::integerToString((int)KageStage::stroke.getThickness()));
						m_EntryStrokeThickness.signal_activate().connect(
							sigc::mem_fun(*this, &Kage::EntryStrokeThickness_onEnter));
			//X/Y Width/Height
 			m_Box1.pack_start(m_propLocationSize, Gtk::PACK_SHRINK);
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
						m_EntryX.set_text(StringHelper::StringHelper::doubleToString(m_KageStage.propX));
						m_EntryX.signal_activate().connect(
							sigc::mem_fun(*this, &Kage::EntryX_onEnter));
					m_propLocationSizeV2.pack_start(m_EntryY, Gtk::PACK_SHRINK);
						m_EntryY.set_size_request(20, 24);
						m_EntryY.set_width_chars(9);
						m_EntryY.set_max_length(9);
						m_EntryY.set_text(StringHelper::StringHelper::doubleToString(m_KageStage.propY));
						m_EntryY.signal_activate().connect(
							sigc::mem_fun(*this, &Kage::EntryY_onEnter));
					m_propLocationSizeV2.pack_start(m_EntryWidth, Gtk::PACK_SHRINK);
						m_EntryWidth.set_size_request(20, 24);
						m_EntryWidth.set_width_chars(9);
						m_EntryWidth.set_max_length(9);
						m_EntryWidth.set_text(StringHelper::StringHelper::doubleToString(m_KageStage.propWidth));
						m_EntryWidth.signal_activate().connect(
							sigc::mem_fun(*this, &Kage::EntryWidth_onEnter));
					m_propLocationSizeV2.pack_start(m_EntryHeight, Gtk::PACK_SHRINK);
						m_EntryHeight.set_size_request(20, 24);
						m_EntryHeight.set_width_chars(9);
						m_EntryHeight.set_max_length(9);
						m_EntryHeight.set_text(StringHelper::StringHelper::doubleToString(m_KageStage.propHeight));
						m_EntryHeight.signal_activate().connect(
							sigc::mem_fun(*this, &Kage::EntryHeight_onEnter));
			//X/Y of Node
 			m_Box1.pack_start(m_propNodeXY, Gtk::PACK_SHRINK);
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
						m_EntryNodeX.set_text(StringHelper::StringHelper::doubleToString(m_KageStage.nodeX));
						m_EntryNodeX.signal_activate().connect(
							sigc::mem_fun(*this, &Kage::EntryNodeX_onEnter));
					m_propNodeXYV2.pack_start(m_EntryNodeY, Gtk::PACK_SHRINK);
						m_EntryNodeY.set_size_request(20, 24);
						m_EntryNodeY.set_width_chars(9);
						m_EntryNodeY.set_max_length(9);
						m_EntryNodeY.set_text(StringHelper::StringHelper::doubleToString(m_KageStage.nodeY));
						m_EntryNodeY.signal_activate().connect(
							sigc::mem_fun(*this, &Kage::EntryNodeY_onEnter));
					m_propNodeXYV2.pack_start(m_ToggleLine, Gtk::PACK_SHRINK);
						m_ToggleLine.add_pixlabel("shared/icons/straighten.png","");
						m_ToggleLine.set_focus_on_click(false);
						m_ToggleLine.signal_clicked().connect(sigc::mem_fun(*this, &Kage::ToggleLine_onClick));
	
	New_onClick();
}

Kage::~Kage() {
	//
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
		m_KageFramesManager.setCurrentLayer(l_kageDo._layer);
		m_KageFramesManager.setCurrentFrame(l_kageDo._frame);
		setFrameData(l_kageDo.getVectorData());
		
		forceRenderFrames();
	}
}
void Kage::Redo_onClick() {
	KageDo l_kageDo = _undoRedoManager.redo();
	if (l_kageDo._layer != -1 && l_kageDo._frame != -1) {
		m_KageFramesManager.setCurrentLayer(l_kageDo._layer);
		m_KageFramesManager.setCurrentFrame(l_kageDo._frame);
		setFrameData(l_kageDo.getVectorData());
		
		forceRenderFrames();
	}
}
void Kage::Cut_onClick() {
	if (KageStage::toolMode == KageStage::MODE_SELECT) {
		Kage::timestamp();
		std::cout << " Kage::Cut_onClick" << std::endl;
		if (m_KageStage.cutSelectedShapes() == true) {
			forceRenderFrames();
		}
	}
}
void Kage::Copy_onClick() {
	if (KageStage::toolMode == KageStage::MODE_SELECT) {
		Kage::timestamp();
		std::cout << " Kage::Copy_onClick" << std::endl;
		if (m_KageStage.copySelectedShapes() == true) {
			forceRenderFrames();
		}
	}
}
void Kage::Paste_onClick() {
	if (KageStage::toolMode == KageStage::MODE_SELECT) {
		Kage::timestamp();
		std::cout << " Kage::Paste_onClick" << std::endl;
		if (m_KageStage.pasteSelectedShapes() == true) {
			forceRenderFrames();
		}
	}
}
void Kage::Duplicate_onClick() {
	if (KageStage::toolMode == KageStage::MODE_SELECT) {
		Kage::timestamp();
		std::cout << " Kage::Duplicate_onClick" << std::endl;
		if (m_KageStage.duplicateShapes() == true) {
			forceRenderFrames();
		}
	}
}
void Kage::SelectAll_onClick() {
	if (KageStage::toolMode == KageStage::MODE_SELECT) {
		Kage::timestamp();
		std::cout << " Kage::SelectAll_onClick" << std::endl;
		if (m_KageStage.selectAllShapes() == true) {
			forceRenderFrames();
		}
	}
}
void Kage::Deselect_onClick() {
	Kage::timestamp();
	std::cout << " Kage::Deselect_onClick" << std::endl;
	if (KageStage::toolMode == KageStage::MODE_SELECT) {
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
}
void Kage::ShapeGroup_onClick() {
	if (KageStage::toolMode == KageStage::MODE_SELECT) {
		Kage::timestamp();
		std::cout << " Kage::ShapeGroup_onClick" << std::endl;
		if (m_KageStage.groupSelectedShapes() == true) {
			forceRenderFrames();
		}
	}
}
void Kage::ShapeUngroup_onClick() {
	if (KageStage::toolMode == KageStage::MODE_SELECT) {
		Kage::timestamp();
		std::cout << " Kage::ShapeUngroup_onClick" << std::endl;
		if (m_KageStage.ungroupSelectedShapes() == true) {
			forceRenderFrames();
		}
	}
}
void Kage::Raise_onClick() {
	if (KageStage::toolMode == KageStage::MODE_SELECT) {
		Kage::timestamp();
		std::cout << " Kage::Raise_onClick" << std::endl;
		if (m_KageStage.raiseSelectedShape() == true) {
			forceRenderFrames();
		}
	}
}
void Kage::Lower_onClick() {
	if (KageStage::toolMode == KageStage::MODE_SELECT) {
		Kage::timestamp();
		std::cout << " Kage::Lower_onClick" << std::endl;
		if (m_KageStage.lowerSelectedShape() == true) {
			forceRenderFrames();
		}
	}
}
void Kage::RaiseToTop_onClick() {
	if (KageStage::toolMode == KageStage::MODE_SELECT) {
		Kage::timestamp();
		std::cout << " Kage::RaiseToTop_onClick" << std::endl;
		if (m_KageStage.raiseToTopSelectedShape() == true) {
			forceRenderFrames();
		}
	}
}
void Kage::LowerToBottom_onClick() {
	if (KageStage::toolMode == KageStage::MODE_SELECT) {
		Kage::timestamp();
		std::cout << " Kage::LowerToBottom_onClick" << std::endl;
		if (m_KageStage.lowerToBottomSelectedShape() == true) {
			forceRenderFrames();
		}
	}
}
void Kage::FlipHorizontal_onClick() {
	if (KageStage::toolMode == KageStage::MODE_SELECT) {
		Kage::timestamp();
		std::cout << " Kage::FlipHorizontal_onClick" << std::endl;
		if (m_KageStage.flipHorizontalSelectedShape() == true) {
			forceRenderFrames();
		}
	}
}
void Kage::FlipVertical_onClick() {
	if (KageStage::toolMode == KageStage::MODE_SELECT) {
		Kage::timestamp();
		std::cout << " Kage::FlipVertical_onClick" << std::endl;
		if (m_KageStage.flipVerticalSelectedShape() == true) {
			forceRenderFrames();
		}
	}
}
void Kage::Delete_onClick() {
	if (m_KageStage._gotFocus == true) {
		if (KageStage::toolMode == KageStage::MODE_SELECT) {
			Kage::timestamp();
			std::cout << " Kage::Delete_onClick SHAPE" << std::endl;
			if (m_KageStage.deleteSelectedShapes() == true) {
				stackDo();
				forceRenderFrames();
			}
		} else if (KageStage::toolMode == KageStage::MODE_NODE) {
			Kage::timestamp();
			std::cout << " Kage::Delete_onClick NODE" << std::endl;
			if (m_KageStage.deleteSelectedNodes() == true) {
				forceRenderFrames();
			}
		}
	} else {
		//TODO: check if focus is on any of the Frames 
	}
}

void Kage::AddFrame_onClick() {
	m_KageFramesManager.addFrame();
	m_KageFramesManager.setCurrentFrame(KageFramesManager::currentFrame + 1);
	show_all();
}

void Kage::DuplicateFrame_onClick() {
	m_KageFramesManager.addFrame();
	
	KageFrame *l_frame = m_KageFramesManager.getFrame();
	VectorDataManager l_vectorsData;
	if (l_frame) {
		l_vectorsData = l_frame->vectorsData.clone();
	}
	
	m_KageFramesManager.setCurrentFrame(KageFramesManager::currentFrame + 1);
	
	m_KageFramesManager.getFrame()->vectorsData = l_vectorsData;
	
	show_all();
}

void Kage::switchToPreviousFrame() {
	if (KageFramesManager::currentFrame > 1) {
		m_KageFramesManager.setCurrentFrame(KageFramesManager::currentFrame - 1);
		show_all();
	}
}

void Kage::switchToNextFrame() {
	if (KageFramesManager::currentFrame < m_KageFramesManager.frameCount()) {
		m_KageFramesManager.setCurrentFrame(KageFramesManager::currentFrame + 1);
		show_all();
	}
}

/**
 * Called by KageFramesManager::setCurrentFrame
 */
void Kage::updateFrameLabel() {
	m_Timeline_CurrentFrame.set_label("L " + StringHelper::unsignedIntegerToString(getCurrentLayer()) + " F " + StringHelper::unsignedIntegerToString(KageFramesManager::currentFrame));
}

void Kage::LayerAdd_onClick() {
	m_KageLayerManager.addLayer();
	m_KageFramesManager.addFrameManager(m_KageLayerManager.layerCount());
	std::cout << "Layer Count: " << m_KageLayerManager.layerCount() << std::endl;
	show_all();
	updateStatus("New Layer Added");
}
void Kage::LayerDel_onClick() {
	std::cout << "Layer Delete Button clicked." << std::endl;
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
}

void Kage::onToolButtonsClick(Gtk::ToggleButton *p_sourceButton) {
	if (p_sourceButton->get_active() == 1) {
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
	propLocationSizeSetVisible(false);
	propNodeXYSetVisible(false);
	KageStage::toolMode = KageStage::MODE_SELECT;
	m_KageStage.initNodeTool();
	forceRenderFrames();
}

void Kage::ToolNode_onClick() {
	toolsButtonToggle("Node Tool");
	propStageSetVisible(false);
	propFillStrokeSetVisible(false);
	propLocationSizeSetVisible(false);
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
	propLocationSizeSetVisible(false);
	propNodeXYSetVisible(false);
	m_ColorButtonFill.set_color(m_KageStage.getFill());
	m_ColorButtonStroke.set_color(m_KageStage.getStroke());
	KageStage::toolMode = KageStage::MODE_DRAW_POLY;
	m_KageStage.initNodeTool();
}

void Kage::ToolOval_onClick() {
	toolsButtonToggle("Oval Tool");
	propStageSetVisible(false);
	propFillStrokeSetVisible(true);
	propLocationSizeSetVisible(false);
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
	propLocationSizeSetVisible(false);
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
	propLocationSizeSetVisible(false);
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
	propLocationSizeSetVisible(false);
	propNodeXYSetVisible(false);
	m_ColorButtonFill.set_color(m_KageStage.getFill());
	m_ColorButtonStroke.set_color(m_KageStage.getStroke());
	KageStage::toolMode = KageStage::MODE_FILL;
//	m_KageStage.initNodeTool();
//	forceRenderFrames();
}

void Kage::ToolEyedrop_onClick() {
	toolsButtonToggle("Eyedrop Tool");
	m_PropStage.set_visible(false);
	m_PropFillStroke.set_visible(true);
	propLocationSizeSetVisible(false);
	propNodeXYSetVisible(false);
	m_ColorButtonFill.set_color(m_KageStage.getFill());
	m_ColorButtonStroke.set_color(m_KageStage.getStroke());
	KageStage::toolMode = KageStage::MODE_EYEDROP;
	m_KageStage.initNodeTool();
	forceRenderFrames();
}

void Kage::ToolZoom_onClick() {
	toolsButtonToggle("Zoom Tool");
	m_PropStage.set_visible(true);
	m_PropFillStroke.set_visible(false);
	propLocationSizeSetVisible(false);
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
	m_PropFillStroke.set_visible(p_visible);
}

void Kage::propLocationSizeSetVisible(bool p_visible) {
	m_propLocationSize.set_visible(p_visible);
}

void Kage::propNodeXYSetVisible(bool p_visible) {
	m_propNodeXY.set_visible(p_visible);
}

void Kage::updateColors() {
	m_ColorButtonFill.set_color(m_KageStage.getFill());
	m_EntryFillAlpha.set_text(StringHelper::unsignedIntegerToString(KageStage::fillColor.getA()));
	m_ColorButtonStroke.set_color(m_KageStage.getStroke());
	m_EntryStrokeAlpha.set_text(StringHelper::unsignedIntegerToString(KageStage::stroke.getA()));
	m_EntryStrokeThickness.set_text(StringHelper::unsignedIntegerToString(KageStage::stroke.getThickness()));
}

void Kage::updateProperties() {
	m_EntryX.set_text(StringHelper::StringHelper::doubleToString(m_KageStage.propX));
	m_EntryY.set_text(StringHelper::StringHelper::doubleToString(m_KageStage.propY));
	m_EntryWidth.set_text(StringHelper::StringHelper::doubleToString(m_KageStage.propWidth));
	m_EntryHeight.set_text(StringHelper::StringHelper::doubleToString(m_KageStage.propHeight));
}

void Kage::updateNodeXY() {
	m_EntryNodeX.set_text(StringHelper::StringHelper::doubleToString(m_KageStage.nodeX));
	m_EntryNodeY.set_text(StringHelper::StringHelper::doubleToString(m_KageStage.nodeY));
}

void Kage::stackDo() {
	_undoRedoManager.stackDo(getCurrentLayer(), KageFramesManager::currentFrame, getFrameData().getVectorData());
}
void Kage::onButtonClick() {
	hide();
}

void Kage::addDataToFrame(VectorDataManager v) {
	if (m_KageLayerManager.getLayer()->isLocked() == false) {
		KageFrame *l_frame = m_KageFramesManager.getFrame();
		if (l_frame) {
			l_frame->vectorsData.push(v);
			l_frame->forceRender();
		}
	}
}
VectorDataManager Kage::getFrameData() {
	if (m_KageLayerManager.getLayer()->isVisible()) {
		KageFrame *l_frame = m_KageFramesManager.getFrame();
		if (l_frame) {
			return l_frame->vectorsData;
		} else {
			VectorDataManager l_vectorDataManager;
			return l_vectorDataManager;
		}
	} else {
		VectorDataManager l_vectorDataManager;
		return l_vectorDataManager;
	}
}

void Kage::setFrameData(VectorDataManager p_vectorsData) {
	if (m_KageLayerManager.getLayer()->isLocked() == false) {
		KageFrame *l_frame = m_KageFramesManager.getFrame();
		if (l_frame) {
			l_frame->vectorsData.clear();
			l_frame->vectorsData = p_vectorsData.clone();
		}
	}
}

void Kage::forceRenderFrames() {
	m_KageStage.render();
	renderFrames();
}
/**
	renders all frameN in all layers, where frameN is the currentFrame
*/
void Kage::renderFrames() {
	unsigned int l_layerCount = m_KageLayerManager.layerCount();
	m_KageStage.clearScreen(m_KageStage.cr);
	unsigned int l_currentLayer = getCurrentLayer();
		for (unsigned int i = 1; i <= l_layerCount; ++i) {
			m_KageFramesManager.setCurrentLayer(i);
			m_KageStage.renderFrame(m_KageStage.cr);
		}
	setCurrentLayer(l_currentLayer);
}

void Kage::renderFramesBelowCurrentLayer() {
	m_KageStage.clearScreen(m_KageStage.cr);
	unsigned int l_currentLayer = getCurrentLayer();
		for (unsigned int i = 1; i < l_currentLayer; ++i) {
			m_KageFramesManager.setCurrentLayer(i);
			m_KageStage.renderFrame(m_KageStage.cr);
		}
	setCurrentLayer(l_currentLayer);
}
void Kage::renderFramesAboveCurrentLayer() {
	unsigned int l_layerCount = m_KageLayerManager.layerCount();
	unsigned int l_currentLayer = getCurrentLayer();
		for (unsigned int i = (l_currentLayer + 1); i < l_layerCount; ++i) {
			m_KageFramesManager.setCurrentLayer(i);
			m_KageStage.renderFrame(m_KageStage.cr);
		}
	setCurrentLayer(l_currentLayer);
}

bool Kage::isLayerLocked() {
	return m_KageLayerManager.isLayerLocked();
}
unsigned int Kage::getCurrentLayer() {
	return m_KageLayerManager.getCurrentLayer();
}

void Kage::setCurrentLayer(unsigned int p_layer) {
	return m_KageLayerManager.setCurrentLayer(p_layer);
}
unsigned int Kage::getCurrentFrame() {
	return m_KageFramesManager.getCurrentFrame();
}

void Kage::setCurrentFrame(unsigned int p_layer) {
	return m_KageFramesManager.setCurrentFrame(p_layer);
}

void Kage::New_onClick() {
	propStageSetVisible(true);
	propFillStrokeSetVisible(false);
	propLocationSizeSetVisible(false);
	propNodeXYSetVisible(false);

	m_KageLayerManager.removeAllLayers();
	m_KageLayerManager.addLayer();
	
	m_KageFramesManager.removeAllFrames();
	//m_KageFramesManager.addFrame();
	
	_undoRedoManager.clear();
	stackDo();
	
	show_all();
	
	m_KageFramesManager.setCurrentFrame(1);
	m_KageFramesManager.setCurrentLayer(1);
	
	currentTool = toggleButtons[0];
	currentTool->set_active(true);
	
	m_KageStage.cleanSlate();
	
	m_EntryStageWid.set_text(StringHelper::StringHelper::doubleToString(m_KageStage.stageWidth));
	m_EntryStageHgt.set_text(StringHelper::StringHelper::doubleToString(m_KageStage.stageHeight));
	m_EntryStageFPS.set_text(StringHelper::unsignedIntegerToString(m_KageStage.fps));
	
	m_KageStage.render();
	
	updateStatus("Ready");
}

void Kage::OpenKSF_onClick() {
	Gtk::FileChooserDialog dialog("Open File", Gtk::FILE_CHOOSER_ACTION_OPEN);
	dialog.set_transient_for( * this);
		dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
		dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);
	auto filter_kage = Gtk::FileFilter::create();
		filter_kage->set_name("Kage Studio Files");
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
			
			string l_ksfContent = BasicXml::openXMLFile(ksfPath);
			
			//cout << "Loaded... \n" << l_ksfContent << endl;
			parseKSF(l_ksfContent);
			_undoRedoManager.clear();
			stackDo();
			
			break;
//		default:
//			std::cout << "clicked " << result << endl;
	}
}
void Kage::Save_onClick() {
	Gtk::FileChooserDialog dialog("Save File", Gtk::FILE_CHOOSER_ACTION_SAVE);
	dialog.set_transient_for( * this);
		dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
		dialog.add_button(Gtk::Stock::SAVE, Gtk::RESPONSE_OK);
	//Add filters, so that only certain file types can be selected:
	auto filter_kage = Gtk::FileFilter::create();
		filter_kage->set_name("Kage Studio Files");
		filter_kage->add_mime_type("text/x-kage");
		filter_kage->add_pattern("*.ksf");
			dialog.add_filter(filter_kage);
	//Show the dialog and wait for a user response:
	int result = dialog.run();
	
	//Handle the response:
	switch (result) {
		case Gtk::RESPONSE_OK:
			ksfPath = dialog.get_filename();
			cout << "uri:" << dialog.get_uri() << endl;
			ksfInited = false;
			ksfFile.close();
			
			unsigned int l_lMax = m_KageLayerManager.layerCount();
			unsigned int i;
				unsigned int l_fMax = m_KageFramesManager.frameCount();
				unsigned int j;
					unsigned int l_currentLayer;
					unsigned int l_currentFrame;
			
			saveKageStudio(ksfPath, "<KageStudio version=\"2019.10.14\">");
			saveKageStudio(ksfPath, "<stage width=\"" + StringHelper::StringHelper::doubleToString(m_KageStage.stageWidth) + "\" height=\"" + StringHelper::StringHelper::doubleToString(m_KageStage.stageHeight) + "\" " +
			                        "background=\"rgb(" + StringHelper::integerToString(m_KageStage.stageBG.getR()) + ", " + StringHelper::integerToString(m_KageStage.stageBG.getG()) + ", " + StringHelper::integerToString(m_KageStage.stageBG.getB()) + ")\" " +
			                        "fps=\"" + StringHelper::integerToString(m_KageStage.fps) + "\" " +
			                        "layers=\"" + StringHelper::integerToString(l_lMax) + "\" " +
			                        "frames=\"" + StringHelper::integerToString(l_fMax) + "\" />");
				l_currentLayer = getCurrentLayer();
				l_currentFrame = m_KageFramesManager.getCurrentFrame();
					for (i = 1; i <= l_lMax; i++) {
						m_KageFramesManager.setCurrentLayer(i);
						saveKageStudio(ksfPath, "<layer" + StringHelper::unsignedIntegerToString(i) + ">");
						for (j = 1; j <= l_fMax; ++j) {
							saveKageStudio(ksfPath, "<frame" + StringHelper::unsignedIntegerToString(j) + ">");
							m_KageFramesManager.setCurrentFrame(j);
							saveKageStudio(ksfPath, saveFrame());
							saveKageStudio(ksfPath, "</frame" + StringHelper::unsignedIntegerToString(j) + ">");
						}
						saveKageStudio(ksfPath, "</layer" + StringHelper::unsignedIntegerToString(i) + ">");
					}
				setCurrentLayer(l_currentLayer);
				m_KageFramesManager.setCurrentFrame(l_currentFrame);
			saveKageStudio(ksfPath, "</KageStudio>");
			updateStatus("Saved to " + ksfPath);
			//TODO: zip saved file -- should it be zipped?
			break;
//		default:
//			std::cout << "clicked " << result << endl;
	}
}

void Kage::ExportKS_onClick() {
	Gtk::FileChooserDialog dialog("Export File", Gtk::FILE_CHOOSER_ACTION_SAVE);
	dialog.set_transient_for( * this);
		//Add response buttons the the dialog:
		dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
		dialog.add_button(Gtk::Stock::SAVE, Gtk::RESPONSE_OK);
	
	auto filter_ks = Gtk::FileFilter::create();
		filter_ks->set_name("KonsolScript files");
		filter_ks->add_mime_type("text/konsolscript");
		filter_ks->add_pattern("*.ks");
			dialog.add_filter(filter_ks);
	int result = dialog.run();
	
	switch (result) {
		case Gtk::RESPONSE_OK:
			ksfPath = dialog.get_filename();
			ksfInited = false;
			ksfFile.close();
			
			exportKonsolScript(ksfPath, "Var:Number bgcolor;\n");
			exportKonsolScript(ksfPath, "function kagestudio_screencls() {");
			exportKonsolScript(ksfPath, "\tDraw:RectFill(0, 0, " + StringHelper::StringHelper::doubleToString(m_KageStage.stageWidth) + ", " + StringHelper::StringHelper::doubleToString(m_KageStage.stageHeight) + ", bgcolor, screen)");
			exportKonsolScript(ksfPath, "}");
			exportKonsolScript(ksfPath, "function render() {");
			exportKonsolScript(ksfPath, "\tkagestudio_screencls()");
			exportKonsolScript(ksfPath, "\tks_f1()");
			exportKonsolScript(ksfPath, "}");
			exportKonsolScript(ksfPath, "function ks_f1() {");
			exportKonsolScript(ksfPath, "\t" + dumpFrame(true));
			exportKonsolScript(ksfPath, "}");
			exportKonsolScript(ksfPath, "function main() {");
			exportKonsolScript(ksfPath, "\t//add variable initialization...");
			exportKonsolScript(ksfPath, "\tKonsol:RGB(" + StringHelper::integerToString(m_KageStage.stageBG.getR()) + ", " + StringHelper::integerToString(m_KageStage.stageBG.getG()) + "," + StringHelper::integerToString(m_KageStage.stageBG.getB()) + ", bgcolor)");
			exportKonsolScript(ksfPath, "}");
			updateStatus("Exported to " + ksfPath);
			break;
	}
}
void Kage::ExportHTML5_onClick() {
	Gtk::FileChooserDialog dialog("Export File", Gtk::FILE_CHOOSER_ACTION_SAVE);
	dialog.set_transient_for( * this);
		//Add response buttons the the dialog:
		dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
		dialog.add_button(Gtk::Stock::SAVE, Gtk::RESPONSE_OK);
	
	auto filter_html5 = Gtk::FileFilter::create();
		filter_html5->set_name("HTML5");
		filter_html5->add_mime_type("text/html");
			dialog.add_filter(filter_html5);
	int result = dialog.run();
	
	switch (result) {
		case Gtk::RESPONSE_OK:
			expPath = dialog.get_filename();
			cout << "uri:" << dialog.get_uri() << endl;
			expInited = false;
			expFile.close();
			
			unsigned int l_lMax = m_KageLayerManager.layerCount();
			unsigned int i;
				unsigned int l_fMax = m_KageFramesManager.frameCount();
				unsigned int j;
					unsigned int t;
			
			exportHtml5(expPath, "<html>\n<head>\n<title>Exported from Kage Studio</title>\n<script type='text/javascript'>\n");
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
				unsigned int f = KageFramesManager::currentFrame;
					for (j = 1; j <= l_fMax; ++j) {
						m_KageFramesManager.setCurrentFrame(j);
						exportHtml5(expPath, "function ks_f" + StringHelper::unsignedIntegerToString(j) + "() {");
						for (i = 1; i <= l_lMax; i++) {
							m_KageFramesManager.setCurrentLayer(i);
							exportHtml5(expPath, dumpFrame(false));
						}
						exportHtml5(expPath, "}\n");
					}
				KageFramesManager::currentFrame = f;
				setCurrentLayer(t);
			exportHtml5(expPath, "function main() {\n\t//add variable initialization...\n}");
			exportHtml5(expPath, "</script>\n</head>\n<body align='center' onload='kagestudio();' bgcolor='#101010'>\n<canvas id='screen' width='" + StringHelper::doubleToString(m_KageStage.stageWidth) + "' height='" + StringHelper::doubleToString(m_KageStage.stageHeight) + "' style='display: block; margin: auto;'></canvas>\n</body>\n</html>");
			updateStatus("Exported to " + expPath);
			break;
	}
}

void Kage::ExportPNG_onClick() {
	Gtk::FileChooserDialog dialog("Export File", Gtk::FILE_CHOOSER_ACTION_SAVE);
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
			
			int l_len = strlen(l_pngPath.c_str()) - 4;
			if (StringHelper::toLower(l_pngPath).substr(l_len, 4) != ".png") {
				l_pngPath = l_pngPath + ".png";
			}
			
			m_KageStage.renderToPNG(l_pngPath, false);
			updateStatus("Exported to " + l_pngPath);
			break;
	}
}

void Kage::ExportPNGTransparent_onClick() {
	Gtk::FileChooserDialog dialog("Export File", Gtk::FILE_CHOOSER_ACTION_SAVE);
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
			
			int l_len = strlen(l_pngPath.c_str()) - 4;
			if (StringHelper::toLower(l_pngPath).substr(l_len, 4) != ".png") {
				l_pngPath = l_pngPath + ".png";
			}
			
			m_KageStage.renderToPNG(l_pngPath, true);
			updateStatus("Exported to " + l_pngPath);
			break;
	}
}

void Kage::ExportPNGSequence_onClick() {
	Gtk::FileChooserDialog dialog("Export File", Gtk::FILE_CHOOSER_ACTION_SAVE);
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
			
			unsigned int l_lMax = m_KageLayerManager.layerCount();
			unsigned int i;
				unsigned int l_fMax = m_KageFramesManager.frameCount();
				unsigned int j;
					unsigned int t;
					unsigned int f;
				
				
				GdkPoint l_tempOrigin;
				l_tempOrigin.x = m_KageStage.origin.x;
				l_tempOrigin.y = m_KageStage.origin.y;
				
				m_KageStage.origin.x = 0;
				m_KageStage.origin.y = 0;
						
				t = getCurrentLayer();
				f = KageFramesManager::currentFrame;
					
					for (j = 1; j <= l_fMax; ++j) {
						m_KageFramesManager.setCurrentFrame(j);
						
						Cairo::RefPtr<Cairo::ImageSurface> surface = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, m_KageStage.stageWidth, m_KageStage.stageHeight);
						Cairo::RefPtr<Cairo::Context> l_context = Cairo::Context::create(surface);
						
							for (i = 1; i <= l_lMax; i++) {
								m_KageFramesManager.setCurrentLayer(i);
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
						surface->write_to_png(l_pngSequencePath);
					}
				KageFramesManager::currentFrame = f;
				setCurrentLayer(t);
			
				m_KageStage.origin.x = l_tempOrigin.x;
				m_KageStage.origin.y = l_tempOrigin.y;
			
			updateStatus("Exported to " + l_pngSequencePath);
			break;
	}
}

void Kage::ExportAVI_onClick() {
	Gtk::FileChooserDialog dialog("Export File", Gtk::FILE_CHOOSER_ACTION_SAVE);
	dialog.set_transient_for( * this);
		//Add response buttons the the dialog:
		dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
		dialog.add_button(Gtk::Stock::SAVE, Gtk::RESPONSE_OK);
	
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
				updateStatus("FFMPEG is not installed.");
				openWebsite("https://www.ffmpeg.org/download.html");
					return;
			}
			
			unsigned int l_lMax = m_KageLayerManager.layerCount();
			unsigned int i;
				unsigned int l_fMax = m_KageFramesManager.frameCount();
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
				f = KageFramesManager::currentFrame;
					
					for (j = 1; j <= l_fMax; ++j) {
						m_KageFramesManager.setCurrentFrame(j);
						
						Cairo::RefPtr<Cairo::ImageSurface> surface = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, m_KageStage.stageWidth, m_KageStage.stageHeight);
						Cairo::RefPtr<Cairo::Context> l_context = Cairo::Context::create(surface);
						
							for (i = 1; i <= l_lMax; i++) {
								m_KageFramesManager.setCurrentLayer(i);
								m_KageStage.clearScreen(l_context);
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
						surface->write_to_png(l_pngSequencePath);
					}
					//workaround to FFMPEG-bug: see https://sourceforge.net/p/kage/tickets/25/
						for (j = 1; j <= (m_KageStage.fps); ++j) {
							m_KageFramesManager.setCurrentFrame(l_fMax);
							
							Cairo::RefPtr<Cairo::ImageSurface> surface = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, m_KageStage.stageWidth, m_KageStage.stageHeight);
							Cairo::RefPtr<Cairo::Context> l_context = Cairo::Context::create(surface);
							
								m_KageStage.clearScreen(l_context);
								m_KageStage.renderFrame(l_context);
							
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
							surface->write_to_png(l_pngSequencePath);
						}
					//end of workaround
				KageFramesManager::currentFrame = f;
				setCurrentLayer(t);
			
				m_KageStage.origin.x = l_tempOrigin.x;
				m_KageStage.origin.y = l_tempOrigin.y;
			
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
			if (runExternal(".\\ffmpeg", "-framerate " + StringHelper::unsignedIntegerToString(m_KageStage.fps) + " -i \"" + l_pngPath + "%05d.png\" -b:v 2048k \"" + l_pngPath + ".avi\"")) {
				if (runExternal("del", "/f \"" + l_pngPath + "*.png\"")) {
#else
			if (runExternal("ffmpeg", "-framerate " + StringHelper::unsignedIntegerToString(m_KageStage.fps) + " -i '" + l_pngPath + "%05d.png' -b:v 2048k '" + l_pngPath + ".avi'")) {
				if (runExternal("rm", "-f '" + l_pngPath + "*.png'")) {
#endif
					updateStatus("Exported to " + l_pngPath + ".avi");
					return;
				}
			}
			updateStatus("Unable to export " + l_pngPath + ".avi");
			break;
	}
}

void Kage_ExportHTML5_onClick() {
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

void Kage::Play_onClick() {
	std::cout << " Kage::Play_onClick" << std::endl;
	tickCounter = 0;
	frameCounter = 0;
	m_tick_id = m_KageStage.add_tick_callback( sigc::mem_fun(*this, &Kage::on_tick) );
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
		if (frameCounter > m_KageFramesManager.frameCount()) {
			frameCounter = m_KageFramesManager.frameCount();
			
			m_KageStage.remove_tick_callback(m_tick_id);
		}
		//
		m_KageFramesManager.setCurrentFrame(frameCounter);
		renderFrames();
	}
	
	return true;
}
void Kage::EntryFillAlpha_onEnter() {
	unsigned int l_uint = StringHelper::toUnsignedInteger(m_EntryFillAlpha.get_text());
	KageStage::fillColor.setA(l_uint % 256);
	m_EntryFillAlpha.set_text(StringHelper::unsignedIntegerToString(KageStage::fillColor.getA()));
	m_KageStage.updateShapeColor();
}

void Kage::EntryStrokeAlpha_onEnter() {
	unsigned int l_uint = StringHelper::toUnsignedInteger(m_EntryStrokeAlpha.get_text());
	KageStage::stroke.setA(l_uint % 256);
	m_EntryStrokeAlpha.set_text(StringHelper::unsignedIntegerToString(KageStage::stroke.getA()));
	m_KageStage.updateShapeColor();
}

void Kage::EntryStrokeThickness_onEnter() {
	unsigned int l_uint = StringHelper::toUnsignedInteger(m_EntryStrokeThickness.get_text());
	KageStage::stroke.setThickness(l_uint);
	m_EntryStrokeThickness.set_text(StringHelper::unsignedIntegerToString(KageStage::stroke.getThickness()));
	m_KageStage.updateShapeColor();
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
	Kage::timestamp();
	std::cout << " Kage::ToggleLine_onClick" << std::endl;
	if (m_KageStage.toggleLineSelectedNodes() == true) {
		forceRenderFrames();
	}
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
			(*toggleButtons.back()).signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &Kage::onToolButtonsClick), toggleButtons.back()));
}

void Kage::btnDebug_onClick() {
	//print vectors in Stage
	m_KageStage.printVectors();
}

void Kage::btnAbout_onClick() {
	AboutDialog* pDialog = new AboutDialog(*this);
		pDialog->run();
	delete pDialog;
}

void Kage::CheckUpdate_onClick() {
	///TODO: launch kagestudio page
}

void Kage::Website_onClick() {
	openWebsite("http://konsolscript.sf.net/web/?s=kage+studio");
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
	m_KageStage.render();
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
	m_Color = m_ColorButtonFill.get_color();
	m_KageStage.setFill(m_Color);
	m_KageStage.updateShapeColor();
}
void Kage::ColorButtonStroke_onClick() {
	m_Color = m_ColorButtonStroke.get_color();
	m_KageStage.setStroke(m_Color);
	m_KageStage.updateShapeColor();
}

void Kage::updateSelectedShapeColor(bool p_doFill, bool p_doStroke) {
	unsigned int l_uint;
	if (p_doFill == true) {
		m_Color = m_ColorButtonFill.get_color();
		m_KageStage.setFill  (m_Color);
		
		l_uint = StringHelper::toUnsignedInteger(m_EntryFillAlpha.get_text());
		l_uint %= 256;
		m_EntryFillAlpha.set_text(StringHelper::unsignedIntegerToString(l_uint));
		m_KageStage.fillColor.setA(l_uint % 256);
	}
	if (p_doStroke == true) {
		m_Color = m_ColorButtonStroke.get_color();
		m_KageStage.setStroke(m_Color);
		
		l_uint = StringHelper::toUnsignedInteger(m_EntryStrokeAlpha.get_text());
		l_uint %= 256;
		m_EntryStrokeAlpha.set_text(StringHelper::unsignedIntegerToString(l_uint));
		m_KageStage.fillColor.setA(l_uint);
		
		l_uint = StringHelper::toUnsignedInteger(m_EntryStrokeThickness.get_text());
		KageStage::stroke.setThickness(l_uint);
	}
	m_KageStage.updateShapeColor(p_doFill, p_doStroke);
}

string Kage::saveFrame() {
	vector<VectorData> v = getFrameData().getVectorData();
	
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
				l_ostringstream << "<closepath/>\n";
				break;
			case VectorData::TYPE_INIT:
				l_ostringstream << "<init/>\n";// " << "\n";
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
				l_ostringstream << "<stroke color=\"rgba" << ColorData(v[i].stroke).toString() << "\" thickness=\"" + StringHelper::doubleToString(v[i].stroke.getThickness()) + "\"/>\n";
				scolor = v[i].stroke;
					if (!scolorPrev.equalTo(scolor) || scolorPrev.getThickness() != scolor.getThickness()) {
						doStroke = true;
						scolorPrev = scolor.clone();
					}
				break;
			case VectorData::TYPE_MOVE:
				l_ostringstream << "<move>" << v[i].points[0].x << " " << v[i].points[0].y << "</move>\n";
				
				p.x = v[i].points[0].x;
				p.y = v[i].points[0].y;
				break;
			case VectorData::TYPE_LINE: //currently not used
				l_ostringstream << "screen.lineTo(" << v[i].points[0].x << ", " << v[i].points[0].y << ");\n";
				
				p.x = v[i].points[0].x;
				p.y = v[i].points[0].y;
				break;
			case VectorData::TYPE_CURVE_QUADRATIC: //currently not used
				l_ostringstream << "screen.quadraticCurveTo(" << p.x << ", " << p.y << ", " << v[i].points[0].x << ", " << v[i].points[0].y << ", " << v[i].points[1].x << ", " << v[i].points[1].y << ");\n";
				
				p.x = v[i].points[1].x;
				p.y = v[i].points[1].y;
				break;
			case VectorData::TYPE_CURVE_CUBIC:
				l_ostringstream << "<cubiccurve>" << v[i].points[0].x << " " << v[i].points[0].y << " " << v[i].points[1].x << " " << v[i].points[1].y << " " << v[i].points[2].x << " " << v[i].points[2].y << "</cubiccurve>\n";
				break;
			case VectorData::TYPE_IMAGE:
				//2 '1st is for X/Y, 2nd is for width/height  -- ?!?
				break;
		}
	}
	return l_ostringstream.str();
}

string Kage::dumpFrame(bool bKS) {
	vector<VectorData> v = getFrameData().getVectorData();
	
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

string Kage::openTextFile(string p_path) {
	char ch;
	string l_string = "";
	fstream fin(p_path, fstream::in);
	while (fin >> noskipws >> ch) {
		l_string += ch;
	}
	return l_string;
}

string Trim(string p_source) {
	string l_source = "";
	int i;
	int l_len;
	//remove "space" before
	l_len = strlen(p_source.c_str());
	for (i = 0; i < l_len; ++i) {
		if (p_source[i] == ' ') {
			//skip to next...
		}
		else {
			l_source = p_source.substr(i, l_len - i);
			break;
		}
	}

	//remove "space" after
	l_len = strlen(l_source.c_str());
	for (i = l_len - 1; i > 0; --i) {
		if (l_source[i] == ' ') {
			//skip to next...
		}
		else {
			l_source = l_source.substr(0, i + 1);
			break;
		}
	}
	return l_source;
}

vector<string> split(const string &s, const string &delim, const bool keep_empty) {
	vector<string> l_rets;
	if (delim.empty()) {
		l_rets.push_back(s);
		return l_rets;
	}
	string::const_iterator substart = s.begin(), subend;
	while (true) {
		subend = search(substart, s.end(), delim.begin(), delim.end());
		string temp(substart, subend);
		if (!temp.empty()) {
			l_rets.push_back(Trim(temp));
		}
		if (subend == s.end()) {
			break;
		}
		substart = subend + delim.size();
	}
	return l_rets;
}
vector<double> Kage::parseNumbers(string p_numbers) {
	vector<string> l_XYs = split(p_numbers, " ", false);
	vector<double> l_numbers;
	for (unsigned int i = 0; i < l_XYs.size(); ++i) {
		l_numbers.push_back(StringHelper::toDouble(l_XYs[i]));
	}
	return l_numbers;
}
vector<int> Kage::parseColorString(string p_color) {
	vector<int> l_colors;
	if (p_color.length() > 4 && p_color.substr(0, 5) == "rgba(") {
		vector<string> l_rgba = split(p_color.substr(5), ", ", false);
		l_colors.push_back(StringHelper::toInteger(l_rgba[0]));
		l_colors.push_back(StringHelper::toInteger(l_rgba[1]));
		l_colors.push_back(StringHelper::toInteger(l_rgba[2]));
		l_colors.push_back(StringHelper::toInteger(split(l_rgba[3], ")", false)[0]));
	} else if (p_color.length() > 3 && p_color.substr(0, 4) == "rgb(") {
		vector<string> l_rgb = split(p_color.substr(4), ", ", false);
		l_colors.push_back(StringHelper::toInteger(l_rgb[0]));
		l_colors.push_back(StringHelper::toInteger(l_rgb[1]));
		l_colors.push_back(StringHelper::toInteger(split(l_rgb[2], ")", false)[0]));
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
			cout << "\t\t\t\t\tl_tagname LAYER\t" << l_layer << endl;
			while (l_layer > m_KageFramesManager.layerCount()) {
				m_KageLayerManager.addLayer();
				m_KageFramesManager.addFrameManager(m_KageLayerManager.layerCount());
			}
			m_KageFramesManager.setCurrentLayer(l_layer);
		} else if (l_tagname.substr(0, 5) == "frame") {
			unsigned int l_frame = StringHelper::toUnsignedInteger(l_tagname.substr(5));
			while (l_frame > m_KageFramesManager.frameCount()) {
				m_KageFramesManager.addFrame();
			}
			m_KageFramesManager.setCurrentFrame(l_frame);
		} else if (l_tagname == "init") {
			VectorDataManager v;
				v.addInit();
			addDataToFrame(v);
		} else if (l_tagname == "fill") {
			VectorDataManager v;
			vector<int> l_colors = parseColorString(l_properties[0].getValue()); //color
				v.addFill(ColorData(l_colors[0], l_colors[1], l_colors[2], l_colors[3]));
			addDataToFrame(v);
		} else if (l_tagname == "stroke") {
			VectorDataManager v;
			vector<int> l_colors = parseColorString(l_properties[0].getValue()); //color
				StrokeColorData l_stroke = StrokeColorData(l_colors[0], l_colors[1], l_colors[2], l_colors[3]);
				l_stroke.setThickness(StringHelper::toDouble(l_properties[1].getValue()));//thickness
				v.addLineStyle(l_stroke);
			addDataToFrame(v);
		} else if (l_tagname == "move") {
			VectorDataManager v;
			vector<double> l_numbers = parseNumbers(p_children[i]._value); //XYs
				v.addMove(PointData(l_numbers[0], l_numbers[1]));
			addDataToFrame(v);
		} else if (l_tagname == "cubiccurve") {
			VectorDataManager v;
			vector<double> l_numbers = parseNumbers(p_children[i]._value); //XYs
				v.addCubic(
					PointData(l_numbers[0], l_numbers[1]),
					PointData(l_numbers[2], l_numbers[3]),
					PointData(l_numbers[4], l_numbers[5]));
			addDataToFrame(v);
		} else if (l_tagname == "closepath") {
			VectorDataManager v;
				v.addClosePath();
			addDataToFrame(v);
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
			addDataToFrame(v);
		}
	}
	m_KageStage.render();
	show_all();
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
						&& l_xmlTagProperties[0].getValue() == "2019.10.14") {					
					parseKSF_Children(l_root._children);
					updateStatus("Loaded " + ksfPath);
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
//			return false; /*command does not exists*/
		} else if (i == 32512) { //GNU
			return false; /* p_cmd not found */
		}
		return true;
	}
	return false;
}

void Kage::timestamp() {
	time_t rawtime;
	time(&rawtime);
	struct tm * timeinfo = localtime (&rawtime);
	
	std::cout << timeinfo->tm_hour << ":" << timeinfo->tm_min << ":" << timeinfo->tm_sec; // << " " << *timeinfo;
}
