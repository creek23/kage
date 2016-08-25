
#include "kage.h"
#include "kage/timeline/layermanager.cpp"
//#include "kage/timeline/layer.cpp"
#include "kage/timeline/framesmanager.cpp"
//#include "kage/timeline/timeline.cpp"
#include "kage/stage.cpp"
#include "about.cpp"

const Glib::ustring app_title = "Kage Studio";

bool KageFrame::mouseIsDown = false;
ColorData KageStage::stageBG(255, 255, 255);
ColorData KageStage::fillColor(0, 0, 255, 255);
StrokeColorData KageStage::stroke(255, 0, 0, 255);

Kage::Kage() : m_LabelStageWid("Width"),
			   m_LabelStageHgt("Height"),
			   m_LabelStageBG("Background"),
			   m_LabelFill("Fill"),
			   m_LabelStroke("Stroke"),
			   m_LabelStrokeThickness("Thickness"),
			   m_LabelProp("Properties"),
			   m_LblHolder_Toolbar("Toolbar"),
			   m_KageLayerManager(),
			   m_KageFramesManager(this),
			   m_KageStage(this) {
	m_ContextId = m_Statusbar.get_context_id(app_title);
	
	set_title("Kage Studio");
	set_icon_from_file("shared/icons/default.png"); //TODO: change this place-holder
	set_border_width(0);
	
	//Define the actions:
	m_refActionGroup = Gtk::ActionGroup::create("Actions"); //It also works with no name, which is probably better if there is only one.
	
	//In real life, the details would not be supplied _in addition _ the stock IDs.
	
	//Add normal Actions:
	m_refActionGroup->add( Gtk::Action::create("FileMenu", "_File") ); 
	m_refActionGroup->add( Gtk::Action::create("EditMenu", "_Edit") );
	m_refActionGroup->add( Gtk::Action::create("TimelineMenu", "_Timeline") );
	m_refActionGroup->add( Gtk::Action::create("ColorMenu", "_Color") );
	m_refActionGroup->add( Gtk::Action::create("ShapeMenu", "_Shape") );
	m_refActionGroup->add( Gtk::Action::create("HelpMenu", "_Help") );
	m_refActionGroup->add(
		Gtk::Action::create("New", Gtk::Stock::NEW, "_New", "Create a new file"),
		sigc::mem_fun(*this, &Kage::onActionActivate)
	);
	m_refActionGroup->add(
		Gtk::Action::create("Open", Gtk::Stock::OPEN, "_Open", "Open a file"),
		sigc::mem_fun(*this, &Kage::onActionActivate)
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
		Gtk::Action::create("Quit", Gtk::Stock::QUIT, "_Quit", "Quit"),
		sigc::mem_fun(*this, &Kage::onButtonClick)
	);
	//TODO: This StockID does not seem to be registered in the C version either, but it is in appwindow.c:
	m_refActionGroup->add(
		Gtk::Action::create("Logo", Gtk::StockID("demo-gtk-logo"), "", "GTK+"),
		sigc::mem_fun(*this, &Kage::onActionActivate)
	);
	
	//Add Toggle Actions:
	m_refActionGroup->add(
		Gtk::ToggleAction::create("Bold", Gtk::Stock::BOLD, "_Bold", "Bold", true /* is_active */),
		sigc::mem_fun(*this, &Kage::onActionActivate)
	);
	
	//Add Radio Actions:
	Gtk::RadioAction::Group group_colors;
	m_refActionGroup->add(
		Gtk::RadioAction::create(group_colors, "Red", "_Red", "Blood"),
		Gtk::AccelKey("<control>R"),
		sigc::bind<-1>( sigc::mem_fun(*this, &Kage::on_radio_action_color_activated), COLOR_RED )
	);
	m_refActionGroup->add(
		Gtk::RadioAction::create(group_colors, "Green", "_Green", "Grass"),
		Gtk::AccelKey("<control>G"),
		sigc::bind<-1>( sigc::mem_fun(*this, &Kage::on_radio_action_color_activated), COLOR_GREEN )
	);
	m_refActionGroup->add( Gtk::RadioAction::create(group_colors, "Blue", "_Blue", "Sky"),
	Gtk::AccelKey("<control>B"),
	sigc::bind<-1>( sigc::mem_fun(*this, &Kage::on_radio_action_color_activated), COLOR_BLUE ) );
	
	Gtk::RadioAction::Group group_shapes;
	m_refActionGroup->add(
		Gtk::RadioAction::create(group_shapes, "Square", "_Square", "Square"),
		Gtk::AccelKey("<control>S"),
		sigc::bind<-1>( sigc::mem_fun(*this, &Kage::on_radio_action_shape_activated), SHAPE_SQUARE )
	);
	m_refActionGroup->add(
		Gtk::RadioAction::create(group_shapes, "Rectangle", "_Rectangle", "Rectangle"),
		Gtk::AccelKey("<control>R"),
		sigc::bind<-1>( sigc::mem_fun(*this, &Kage::on_radio_action_shape_activated), SHAPE_RECTANGLE )
	);
	m_refActionGroup->add(
		Gtk::RadioAction::create(group_shapes, "Oval", "_Oval", "Egg"),
		Gtk::AccelKey("<control>O"),
		sigc::bind<-1>( sigc::mem_fun(*this, &Kage::on_radio_action_shape_activated), SHAPE_OVAL )
	);
	m_refActionGroup->add(
		Gtk::Action::create("Play", "Play", "Play Animation"),
		Gtk::AccelKey("F9"),
		sigc::mem_fun(*this, &Kage::Play_onClick)
	);
	m_refActionGroup->add(
		Gtk::Action::create("PreviousFrame", "_Previous", "Previous Frame"),
//		Gtk::AccelKey("["),
		sigc::mem_fun(*this, &Kage::onActionActivate)
	);
	m_refActionGroup->add(
		Gtk::Action::create("NextFrame", "_Next", "Next Frame"),
//		Gtk::AccelKey("]"),
		sigc::mem_fun(*this, &Kage::onActionActivate)
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
		sigc::mem_fun(*this, &Kage::onActionActivate)
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
		"			<menuitem action='Open'/>"
		"			<separator/>"
		"			<menuitem action='Save'/>"
		"			<separator/>"
		"			<menuitem action='ExportHTML5'/>"
		"			<menuitem action='ExportKS'/>"
		"			<separator/>"
		"			<menuitem action='Quit'/>"
		"		</menu>"
		"		<menu action='EditMenu'>"
		"			<menu action='ColorMenu'>"
		"			<menuitem action='Red'/>"
		"			<menuitem action='Green'/>"
		"			<menuitem action='Blue'/>"
		"		</menu>"
		"		<menu action='ShapeMenu'>"
		"			<menuitem action='Square'/>"
		"			<menuitem action='Rectangle'/>"
		"			<menuitem action='Oval'/>"
		"		</menu>"
		"			<menuitem action='Bold'/>"
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
		"		<menu action='HelpMenu'>"
		"			<menuitem action='HelpTopic'/>"
		"			<menuitem action='Website'/>"
		"			<separator/>"
		"			<menuitem action='CheckUpdate'/>"
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
		addToolButton("Anchor");
		addToolButton("Marquee");
		m_VBoxToolbar_Holder.pack_start(m_Separator_Toolbar2, Gtk::PACK_SHRINK);
		addToolButton("Text");
		addToolButton("Poly");
		addToolButton("Oval");
		addToolButton("Rectangle");
		m_VBoxToolbar_Holder.pack_start(m_Separator_Toolbar3, Gtk::PACK_SHRINK);
		addToolButton("Stroke");
		addToolButton("Fill");
		addToolButton("Eyedrop"); //gtk-color-picker
		addToolButton("Eraser");
		m_VBoxToolbar_Holder.pack_start(m_Separator_Toolbar4, Gtk::PACK_SHRINK);
		addToolButton("Zoom"); //gtk-zoom-in
		currentTool = toggleButtons[0];
		currentTool->set_active(true);
		
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
							m_Timeline_Label.set_label("Timeline");
						m_Timeline_Layer_Add_HBox.pack_start(m_Timeline_Add_Button, Gtk::PACK_SHRINK);
							m_Timeline_Add_Button.set_label("+");
							m_Timeline_Add_Button.set_size_request(20, 20);
							m_Timeline_Add_Button.signal_clicked().connect(sigc::mem_fun(*this, &Kage::LayerAdd_onClick));
						m_Timeline_Layer_Add_HBox.pack_start(m_Timeline_Del_Button, Gtk::PACK_SHRINK);
							m_Timeline_Del_Button.set_label("-");
							m_Timeline_Del_Button.set_size_request(20, 20);
							m_Timeline_Del_Button.signal_clicked().connect(sigc::mem_fun(*this, &Kage::LayerDel_onClick));
				m_Timeline_HPaned.add2(m_Timeline_Frame_VBox);
					m_Timeline_Frame_VBox.pack_start(m_Timeline_Frame_ScrolledWindow);//, Gtk::PACK_SHRINK);
						m_Timeline_Frame_ScrolledWindow.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_NEVER);
							m_Timeline_Frame_ScrolledWindow.set_border_width(0);
							m_Timeline_Frame_ScrolledWindow.add(m_KageFramesManager);
							m_Timeline_Frame_ScrolledWindow.set_shadow_type(Gtk::SHADOW_NONE);
					m_Timeline_Frame_VBox.pack_start(m_Timeline_HScrollbar, Gtk::PACK_SHRINK);
						m_Timeline_HScrollbar.set_adjustment(*m_Timeline_Frame_ScrolledWindow.get_hadjustment());
					
			m_Timeline_HBox.pack_start(m_Timeline_VScrollbar, Gtk::PACK_SHRINK);
				m_Timeline_VScrollbar.set_adjustment(*m_Timeline_Frame_ScrolledWindow.get_vadjustment());
					m_Timeline_Layer_ScrolledWindow.set_vadjustment(*m_Timeline_Frame_ScrolledWindow.get_vadjustment());
					
		m_VPane_Timeline.add2(m_HPane_DrawingArea);
			m_HPane_DrawingArea.add1(m_KageStage);
			m_KageStage.set_size_request(640, 480);
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
						m_EntryStageWid.set_size_request(50, 24);
						m_EntryStageWid.set_max_length(4);
						m_EntryStageWid.set_text(uintToString(m_KageStage.stageWidth));
							m_EntryStageWid.signal_activate().connect(
								sigc::mem_fun(*this, &Kage::EntryStageWid_onEnter) );
						m_EntryStageHgt.set_size_request(50, 24);
						m_EntryStageHgt.set_max_length(4);
						m_EntryStageHgt.set_text(uintToString(m_KageStage.stageHeight));
							m_EntryStageHgt.signal_activate().connect(
								sigc::mem_fun(*this, &Kage::EntryStageHgt_onEnter) );
						m_ColorButtonStage.set_size_request(32, 32);
						//m_ColorButtonStage.set_use_alpha(true);
						m_ColorButtonStage.set_title("Choose Stage Background Color");
						m_ColorButtonStage.set_color(m_KageStage.getStageBG());
							m_ColorButtonStage.signal_color_set().connect(sigc::mem_fun(* this, &Kage::ColorButtonStage_onClick) );
						m_EntryStageFPS.set_size_request(50, 24);
						m_EntryStageFPS.set_max_length(3);
						m_EntryStageFPS.set_text(uintToString(m_KageStage.fps));
							m_EntryStageFPS.signal_activate().connect(
								sigc::mem_fun(*this, &Kage::EntryStageFPS_onEnter) );
			//Fill
			m_Box1.pack_start(m_PropFill, Gtk::PACK_SHRINK);
				m_PropFill.set_border_width(10);
				m_PropFill.pack_start(m_PropFillV1);
					m_PropFillV1.set_border_width(4);
					m_PropFillV1.set_spacing(4);
					m_PropFillV1.pack_start(m_LabelFill);
				m_PropFill.pack_start(m_PropFillV2);
					m_PropFillV2.set_border_width(4);
					m_PropFillV2.set_spacing(4);
					m_PropFillV2.pack_start(m_ColorButtonFill);
						m_ColorButtonFill.set_size_request(32, 32);
						m_ColorButtonFill.set_use_alpha(true);
						m_ColorButtonFill.set_title("Choose Fill Color");
							m_ColorButtonFill.set_color(m_KageStage.getStageBG());
						m_ColorButtonFill.signal_color_set().connect(sigc::mem_fun(* this, &Kage::ChooseFill_onClick) );
				
			//Stroke
			m_Box1.pack_start(m_PropStroke, Gtk::PACK_SHRINK);
				m_PropStroke.set_border_width(10);
				m_PropStroke.pack_start(m_PropStrokeV1);
					m_PropStrokeV1.set_border_width(4);
					m_PropStrokeV1.set_spacing(4);
					m_PropStrokeV1.pack_start(m_LabelStroke);
					m_PropStrokeV1.pack_start(m_LabelStrokeThickness);
				m_PropStroke.pack_start(m_PropStrokeV2);;
					m_PropStrokeV2.set_border_width(4);
					m_PropStrokeV2.set_spacing(4);
					m_PropStrokeV2.pack_start(m_ColorButtonStroke);
						m_ColorButtonStroke.set_size_request(32, 32);
						m_ColorButtonStroke.set_use_alpha(true);
						m_ColorButtonStroke.set_title("Choose Stroke Color");
							m_ColorButtonStroke.set_color(m_KageStage.getStageBG());
						m_ColorButtonStroke.signal_color_set().connect(sigc::mem_fun(* this, &Kage::ChooseStroke_onClick) );
					m_PropStrokeV2.pack_start(m_EntryStrokeThickness, Gtk::PACK_SHRINK);
						m_EntryStrokeThickness.set_size_request(50, 24);
						m_EntryStrokeThickness.set_max_length(2);
						m_EntryStrokeThickness.set_text(intToString((int)KageStage::stroke.getThickness()));
						m_EntryStrokeThickness.signal_activate().connect(
							sigc::mem_fun(*this, &Kage::EntryStrokeThickness_onEnter) );
				
	show_all();
	m_PropFill.set_visible(false);
	m_PropStroke.set_visible(false);
	m_KageFramesManager.setCurrentFrame(1);
	m_KageFramesManager.setCurrentLayer(1);
	updateStatus("Ready");
}

Kage::~Kage() {
	//
}

void Kage::updateStatus(Glib::ustring status_msg) {
	m_Statusbar.push(status_msg, m_ContextId);
}

void Kage::AddFrame_onClick() {
	m_KageFramesManager.addFrame();
	m_KageFramesManager.setCurrentFrame(KageFramesManager::currentFrame + 1);
	show_all();
}

void Kage::LayerAdd_onClick() {
	m_KageFramesManager.addFrameManager(m_KageLayerManager.layerCount());
	m_KageLayerManager.addLayer();
	std::cout << "Layer Count: " << m_KageLayerManager.layerCount() << std::endl;
	show_all();
	updateStatus("New Layer Added");
}
void Kage::LayerDel_onClick() {
	std::cout << "Layer Delete Button clicked." << std::endl;
}

void Kage::onToolButtonsClick(Gtk::ToggleButton *p_sourceButton) {
	if (p_sourceButton->get_active() == 1) {
		if (currentTool != p_sourceButton) {
			currentTool->set_active(false);
		}
		currentTool = p_sourceButton;
		if (p_sourceButton->get_tooltip_text() == "Select Tool") {
			m_PropFill.set_visible(false);
			m_PropStroke.set_visible(false);
			m_PropStage.set_visible(true);
			KageStage::toolMode = KageStage::MODE_SELECT;
		} else if (p_sourceButton->get_tooltip_text() == "Rectangle Tool") {
			m_PropStage.set_visible(false);
			m_PropFill.set_visible(true);
			m_PropStroke.set_visible(true);
			m_ColorButtonFill.set_color(m_KageStage.getFill());
			m_ColorButtonStroke.set_color(m_KageStage.getStroke());
			KageStage::toolMode = KageStage::MODE_DRAW_RECT;
		} else if (p_sourceButton->get_tooltip_text() == "Poly Tool") {
			m_PropStage.set_visible(false);
			m_PropFill.set_visible(false);
			m_PropStroke.set_visible(true);
			m_ColorButtonStroke.set_color(m_KageStage.getStroke());
			KageStage::toolMode = KageStage::MODE_DRAW_POLY;
		} else {
			std::cout << p_sourceButton->get_tooltip_text() << ".active " << p_sourceButton->get_active() << " Button clicked." << std::endl;
		}
	}
}

void Kage::onButtonClick() {
	hide();
}

void Kage::addDataToFrame(VectorDataManager v) {
	m_KageFramesManager.getFrame()->vectorsData.push(v);
//	m_KageFramesManager.getFrame()->render();
}
VectorDataManager Kage::getFrameData() {
	cout << "Kage::getFrameData() F " << KageFramesManager::currentFrame << " L " << KageFramesManager::currentLayer << endl;
	return m_KageFramesManager.getFrame()->vectorsData;
}

/**
	renders all frameN in all layers, where frameN is the currentFrame
*/
void Kage::renderFrames() {
	unsigned int c = m_KageLayerManager.layerCount();
	unsigned int i;
	m_KageStage.clearScreen();
	unsigned int t = KageFramesManager::currentLayer;
		for (i = 1; i <= c; i++) {
			m_KageFramesManager.setCurrentLayer(i);
			m_KageStage.renderFrame();
		}
	KageFramesManager::currentLayer = t;
}

void Kage::Save_onClick() {
	Gtk::FileChooserDialog dialog("Save File", Gtk::FILE_CHOOSER_ACTION_SAVE);
	dialog.set_transient_for( * this);
		dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
		dialog.add_button(Gtk::Stock::SAVE, Gtk::RESPONSE_OK);
	//Add filters, so that only certain file types can be selected:
	Gtk::FileFilter filter_kage;
		filter_kage.set_name("KAGE");
		filter_kage.add_mime_type("text/plain");
			dialog.add_filter(filter_kage);
//	Gtk::FileFilter filter_text;
//		filter_text.set_name("Text files");
//		filter_text.add_mime_type("text/plain");
//			dialog.add_filter(filter_text);
//	Gtk::FileFilter filter_cpp;
//		filter_cpp.set_name("C/C++ files");
//		filter_cpp.add_mime_type("text/x-c");
//		filter_cpp.add_mime_type("text/x-c++");
//		filter_cpp.add_mime_type("text/x-c-header");
//			dialog.add_filter(filter_cpp);
//	Gtk::FileFilter filter_any;
//		filter_any.set_name("Any files");
//		filter_any.add_pattern("* ");
//			dialog.add_filter(filter_any);
	//Show the dialog and wait for a user response:
	int result = dialog.run();
	
	//Handle the response:
	switch (result) {
		case Gtk::RESPONSE_OK:
			ksfPath = dialog.get_filename();
			ksfInited = false;
			
			//TODO: save file then zip.
			break;
	}
}

void Kage::ExportKS_onClick() {
	Gtk::FileChooserDialog dialog("Export File", Gtk::FILE_CHOOSER_ACTION_SAVE);
	dialog.set_transient_for( * this);
		//Add response buttons the the dialog:
		dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
		dialog.add_button(Gtk::Stock::SAVE, Gtk::RESPONSE_OK);
	
	Gtk::FileFilter filter_ks;
		filter_ks.set_name("KonsolScript");
		filter_ks.add_mime_type("text/x-konsolscript");
			dialog.add_filter(filter_ks);
	int result = dialog.run();
	
	switch (result) {
		case Gtk::RESPONSE_OK:
			ksfPath = dialog.get_filename();
			ksfInited = false;
			
			exportKonsolScript(ksfPath, "Var:Number bgcolor;\n");
			exportKonsolScript(ksfPath, "function kagestudio_screencls() {");
			exportKonsolScript(ksfPath, "\tDraw:RectFill(0, 0, 800, 600, bgcolor, screen)");
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
			exportKonsolScript(ksfPath, "\tKonsol:RGB(" + intToString(m_KageStage.stageBG.getR()) + ", " + intToString(m_KageStage.stageBG.getG()) + "," + intToString(m_KageStage.stageBG.getB()) + ", bgcolor)");
			exportKonsolScript(ksfPath, "}");
			break;
	}
}
void Kage::ExportHTML5_onClick() {
	Gtk::FileChooserDialog dialog("Export File", Gtk::FILE_CHOOSER_ACTION_SAVE);
	dialog.set_transient_for( * this);
		//Add response buttons the the dialog:
		dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
		dialog.add_button(Gtk::Stock::SAVE, Gtk::RESPONSE_OK);
	
	Gtk::FileFilter filter_html5;
		filter_html5.set_name("HTML5");
		filter_html5.add_mime_type("text/html");
			dialog.add_filter(filter_html5);
	int result = dialog.run();
	
	switch (result) {
		case Gtk::RESPONSE_OK:
			expPath = dialog.get_filename();
			cout << "uri:" << dialog.get_uri() << endl;
			expInited = false;
			
			unsigned int l_lMax = m_KageLayerManager.layerCount();
			unsigned int i;
				unsigned int l_fMax = m_KageFramesManager.frameCount();
				unsigned int j;
					unsigned int t;
			
			exportHtml5(expPath, "<html>\n<head>\n<title>Exported from Kage Studio</title>\n<script type='text/javascript'>\n");
			exportHtml5(expPath, "var screen;\n");
			exportHtml5(expPath, "var frame = 1;\n");
			exportHtml5(expPath, "var frameMax = " + uintToString(l_fMax) + ";\n");
			exportHtml5(expPath, "function kagestudio() {");
			exportHtml5(expPath, "\tmain();");
			exportHtml5(expPath, "\tscreen = document.getElementById('screen').getContext('2d');");
			if (l_fMax > 1) {
				exportHtml5(expPath, "\tsetInterval(kagestudio_loop, " + intToString(1000/m_KageStage.fps) + ");");
				exportHtml5(expPath, "}");
				exportHtml5(expPath, "function kagestudio_screencls() {");
				exportHtml5(expPath, "\tscreen.fillStyle = \"rgb(" + intToString(m_KageStage.stageBG.getR()) + ", " + intToString(m_KageStage.stageBG.getG()) + "," + intToString(m_KageStage.stageBG.getB()) + ")\";");
				exportHtml5(expPath, "\tscreen.fillRect(0, 0, 800, 600);");
				exportHtml5(expPath, "}");
				exportHtml5(expPath, "function kagestudio_loop() {");
				exportHtml5(expPath, "\tkagestudio_screencls()");
				exportHtml5(expPath, "\twindow['ks_f' + frame]();");
				exportHtml5(expPath, "\tframe++;");
				exportHtml5(expPath, "\tif (frame > frameMax) { frame = 1; }");
				exportHtml5(expPath, "}");
			} else {
				exportHtml5(expPath, "\tks_f1();");
				exportHtml5(expPath, "}");
			}
				t = KageFramesManager::currentLayer;
					for (j = 1; j <= l_fMax; j++) {
						m_KageFramesManager.setCurrentFrame(j);
						exportHtml5(expPath, "function ks_f" + uintToString(j) + "() {");
						for (i = 1; i <= l_lMax; i++) {
							m_KageFramesManager.setCurrentLayer(i);
							exportHtml5(expPath, dumpFrame(false));
						}
						exportHtml5(expPath, "}\n");
					}
				KageFramesManager::currentLayer = t;
			exportHtml5(expPath, "function main() {\n\t//add variable initialization...\n}");
			exportHtml5(expPath, "</script>\n</head>\n<body align='center' onload='kagestudio();'>\n<canvas id='screen' width='640' height='480'></canvas>\n</body>\n</html>");
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
	std::cout << "Play activated." << std::endl;
	unsigned int c = m_KageFramesManager.frameCount();
	unsigned int i;
	m_KageStage.clearScreen();
	for (i = 1; i <= c; i++) {
		m_KageFramesManager.setCurrentFrame(i);
		renderFrames();
		Glib::usleep(1000000/m_KageStage.fps); //usleep requires MICROseconds -- NOT MILLIseconds
	}
}

void Kage::EntryStrokeThickness_onEnter() {
	string t = m_EntryStrokeThickness.get_text();
	KageStage::stroke.setThickness(stringToUInt(t));
	m_EntryStrokeThickness.set_text(uintToString(KageStage::stroke.getThickness()));
}

void Kage::on_radio_action_color_activated(color chosen_color) {
	std::cout << "Color Radio Action selected. Item selected=" << chosen_color << std::endl;
}

void Kage::on_radio_action_shape_activated(shape chosen_shape) {
	std::cout << "Shape Radio Action selected. Item selected=" << chosen_shape << std::endl;
}

void Kage::addToolButton(const Glib::ustring &label) {
	toggleButtons.push_back(Gtk::manage(new Gtk::ToggleButton()));
		m_VBoxToolbar_Holder.pack_start(*toggleButtons.back(), Gtk::PACK_SHRINK);
			(*toggleButtons.back()).add_pixlabel("shared/icons/" + label + "_tango.png",""); //find way to make the image centered
			(*toggleButtons.back()).set_focus_on_click(false);
			(*toggleButtons.back()).signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &Kage::onToolButtonsClick), toggleButtons.back()));
	tooltips.push_back(Gtk::manage(new Gtk::Tooltips()));
		(*tooltips.back()).set_tip(*toggleButtons.back(), label + " Tool");
}

void Kage::btnAbout_onClick() {
	AboutDialog* pDialog = new AboutDialog(*this);
		pDialog->run();
	delete pDialog;
}

void Kage::CheckUpdate_onClick() {
	//TODO: launch kagestudio page
}

void Kage::Website_onClick() {
	GError *error = NULL;
	gtk_show_uri(NULL, "http://www.konsolscript.org", gtk_get_current_event_time(), &error);
//	gtk_show_uri(gdk_screen_get_default(), "http://www.konsolscript.org", gtk_get_current_event_time(), &error);
	if (error) {
		cout << error->message << endl;
		cout << "Launching default web browser..." << endl;
		runExternal("start", "http://www.konsolscript.org/kagestudio");
	}
}

bool Kage::fileWrite(string p_path, string p_msg, std::ofstream &p_file, bool &p_flag) {
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
	
	p_file << p_msg << endl;
	
	return true;
}

bool Kage::dump(string p_path, string p_msg) {
	return fileWrite(p_path, p_msg, ksfFile, ksfInited);
}
bool Kage::exportHtml5(string p_path, string p_msg) {
	int l_len = strlen(p_path.c_str()) - 5;
	if (strToLower(p_path).substr(l_len, 5) != ".html") {
		p_path = p_path + ".html";
	}
	
	return fileWrite(p_path, p_msg, expFile, expInited);
}

bool Kage::exportKonsolScript(string p_path, string p_msg) {
	int l_len = strlen(p_path.c_str()) - 3;
	if (strToLower(p_path).substr(l_len, 3) != ".ks") {
		p_path = p_path + ".ks";
	}
	
	return fileWrite(p_path, p_msg, ksfFile, ksfInited);
}

bool Kage::dtrace(string p_msg) {
	return dump(ksfPath, p_msg);
}

//change each element of the string to upper case
string Kage::strToUpper(string p_str) {
	unsigned int i;
	
	for(i = 0; i < p_str.length(); i++) {
		p_str[i] = toupper(p_str[i]);
	}
	
	return p_str;
}

//change each element of the string to lower case
string Kage::strToLower(string p_str) {
	unsigned int i;
	for(i = 0; i < p_str.length(); i++) {
		p_str[i] = tolower(p_str[i]);
	}
	return p_str;
}

string Kage::intToString(int p_src) {
	if (p_src == 0) {
		return "0";
	}
	
	ostringstream l_ostringstream;
		l_ostringstream << p_src;
	return l_ostringstream.str();
}

string Kage::uintToString(unsigned int p_src) {
	if (p_src == 0) {
		return "0";
	}
	
	ostringstream l_ostringstream;
		l_ostringstream << p_src;
	return l_ostringstream.str();
}

int Kage::stringToInt(string strConvert) { 
	//TODO: validate string to be numerics
	return atoi(strConvert.c_str());
}

int Kage::stringToUInt(string strConvert) { 
	//TODO: validate string to be numerics
	return (unsigned)atoi(strConvert.c_str());
}

long Kage::stringToLong(string strConvert) { 
	//TODO: validate string to be numerics
	return atol(strConvert.c_str());
}

void Kage::EntryStageWid_onEnter() {
	string t = m_EntryStageWid.get_text();
	m_KageStage.wid = stringToUInt(t);
	m_EntryStageWid.set_text(uintToString(m_KageStage.wid));
	m_KageStage.render();
}
void Kage::EntryStageHgt_onEnter() {
	string t = m_EntryStageHgt.get_text();
	m_KageStage.hgt = stringToUInt(t);
	m_EntryStageHgt.set_text(uintToString(m_KageStage.hgt));
	m_KageStage.render();
}
void Kage::EntryStageFPS_onEnter() {
	string t = m_EntryStageFPS.get_text();
	cout << "m_KageStage.fps " << m_KageStage.fps << " t " << t << endl;
	m_KageStage.fps = stringToUInt(t);
	cout << "m_KageStage.fps ..." << m_KageStage.fps << " t " << t << endl;
	m_EntryStageFPS.set_text(uintToString(m_KageStage.fps));
}

void Kage::ColorButtonStage_onClick() {
	m_Color = m_ColorButtonStage.get_color();
	m_KageStage.modify_bg(Gtk::STATE_NORMAL, m_Color);
	m_KageStage.setStageBG(m_Color);
}
void Kage::ChooseFill_onClick() {
	m_Color = m_ColorButtonFill.get_color();
	m_KageStage.setFill(m_Color);
}
void Kage::ChooseStroke_onClick() {
	m_Color = m_ColorButtonStroke.get_color();
	m_KageStage.setStroke(m_Color);
}

string Kage::dumpFrame(bool bKS) {
	vector<VectorData> v = getFrameData().getVectorData();
	
	ostringstream l_ostringstream;
	unsigned int vsize = v.size();//-1;
	unsigned int fillCtr = 0;
	unsigned int thickness = 0;
	ColorData fcolor;
	StrokeColorData scolor;
	StrokeColorData scolorPrev;
	PointData p;
	double x1;
	double y1;
	double x2;
	double y2;
	double x3;
	double y3;
	for (unsigned int i = 1; i < vsize; i++) {
		switch (v[i].vectorType) {
			case VectorData::TYPE_FILL:
				fcolor = v[i].fillColor;
				if (bKS == true) {
					l_ostringstream << "\nDraw:BeginFill(0x" << int255ToHex(fcolor.getR()) << int255ToHex(fcolor.getG()) << int255ToHex(fcolor.getB()) << ", " << fcolor.getA() << ", screen)\n";
				} else {
					l_ostringstream << "\nscreen.beginPath();\n";
				}
				fillCtr++;
				break;
			case VectorData::TYPE_ENDFILL:
				if (fillCtr > 0) {
					if (bKS == true) {
						l_ostringstream << "Draw:EndFill(screen)\n";
					} else {
						l_ostringstream << "screen.closePath();\n";
						if (fcolor.getA() == 255) {
							l_ostringstream << "screen.fillStyle = '#" << int255ToHex(fcolor.getR()) << int255ToHex(fcolor.getG()) << int255ToHex(fcolor.getB()) << "';\n";
						} else {
							l_ostringstream << "screen.fillStyle = 'rgba(" << fcolor.getR() << "," << fcolor.getG() << "," << fcolor.getB() << "," << (double)(fcolor.getA()/255) << ")';\n";
						}
						l_ostringstream << "screen.fill();\n";
					}
				}
					if (scolor.getThickness() > 0) {
						if (bKS == false) {
							if (scolorPrev.equalTo(scolor) || scolorPrev.getThickness() != scolor.getThickness()) {
								if (scolor.getA() == 255) {
									l_ostringstream << "screen.strokeStyle = '#" << int255ToHex(scolor.getR()) << int255ToHex(scolor.getG()) << int255ToHex(scolor.getB()) << "';\n";
								} else {
									l_ostringstream << "screen.strokeStyle = 'rgba(" << scolor.getR() << "," << scolor.getG() << "," << scolor.getB() << "," << (double)(scolor.getA()/255) << ")';\n";
								}
								l_ostringstream << "screen.lineWidth = " << scolor.getThickness() << ";\n";
								l_ostringstream << "screen.lineCap = 'round';\n";
								
								scolorPrev = scolor.clone();
							}
							l_ostringstream << "screen.stroke();\n";
						}
					}
				if (fillCtr > 0) {
					fillCtr--;
				}
				break;
			case VectorData::TYPE_STROKE:
				scolor = v[i].stroke;
				if (bKS == true) {
					l_ostringstream << "Draw:LineStyle(" << scolor.getThickness() << ", 0x" << int255ToHex(scolor.getR()) << int255ToHex(scolor.getG()) << int255ToHex(scolor.getB()) << ", " << scolor.getA() << ", screen)\n";
				} else {
					if (scolorPrev.equalTo(scolor) || scolorPrev.getThickness() != scolor.getThickness()) {
						l_ostringstream << "screen.beginPath();\n";
						
						scolorPrev = scolor.clone();
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
				
				p.x = x3;
				p.y = y3;
				break;
			case VectorData::TYPE_CURVE_CUBIC:
				if (bKS == true) {
					l_ostringstream << "Draw:CubicCurveTo(" << p.x << ", " << p.y << ", " << v[i].points[0].x << ", " << v[i].points[0].y << ", " << v[i].points[1].x << ", " << v[i].points[1].y << ", screen)\n";
				} else {
					l_ostringstream << "screen.bezierCurveTo(" << p.x << ", " << p.y << ", " << v[i].points[0].x << ", " << v[i].points[0].y << ", " << v[i].points[1].x << ", " << v[i].points[1].y << ");\n";
				}
				p.x = v[i].points[1].x;
				p.y = v[i].points[1].y;
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
}

bool Kage::runExternal(string p_cmd, string p_param) {
	string l_cmd = p_cmd + " " + p_param;
	
	if (system(NULL)) {
		int i = system(l_cmd.c_str());
		if (i == 0) { /*command does not exists*/}
		return true;
	}
	return false;
}
