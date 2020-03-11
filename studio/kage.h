
#ifndef GTKMM_KAGE_H
	#define GTKMM_KAGE_H
	
	#include <gtkmm/window.h>
	#include <gtkmm/button.h>
	#include <gtkmm/box.h>
	#include <gtkmm/uimanager.h>
	#include <gtkmm/stock.h>
	#include <gtkmm/label.h>
	//#include <gtkmm/action.h>
	#include <gtkmm/radioaction.h>
	//#include <gtkmm/menubar.h>
	#include <gtkmm/separator.h>
	#include <gtkmm/paned.h>
	#include <gtkmm/statusbar.h>
	#include <gtkmm/togglebutton.h>
	#include <gtkmm/messagedialog.h>
	#include <gtkmm/tooltip.h>
	#include <gtkmm/scrollbar.h>
	#include <gtkmm/scrolledwindow.h>
	#include <gtkmm/filechooserdialog.h>
	#include <gtkmm/colorbutton.h>
	#include <gtkmm/colorselection.h>
	#include "kage/timeline/layermanager.h"
	//#include "kage/timeline/layer.h"
	#include "kage/timeline/framesmanager.h"
	//#include "kage/timeline/timeline.h"
	#include "kage/stage.h"
	#include "about.h"
//	#include "register.h" -- not implemented yet; this is to gather users for support group
	
	#include "kage/unre/kagedo.h"
	#include "kage/unre/unre.h"
	
	#include "util/xml/xmltoken.h"
	#include "util/xml/xmltagproperty.h"
	#include "util/xml/xmltag.h"
	#include "util/xml/xml.h"
	#include "util/string/stringhelper.h"
	
	#include <fstream> //ofstream
	#include <iostream> //cerr
	
	#include <gdkmm/cursor.h>
	#include <gdkmm/frameclock.h>

	class Kage : public Gtk::Window {
		public:
			Kage();
			virtual ~Kage();
			
			void addDataToFrame(VectorDataManager v, bool p_force = false);
			VectorDataManager getFrameData(bool p_force = false);
			void setFrameData(VectorDataManager p_vectorsData);
			void forceRenderFrames();
			void renderFrames();
			void renderFramesBelowCurrentLayer();
			void renderFramesAboveCurrentLayer();
			string saveFrame();
			string dumpFrame(bool bKS);
			
			string int255ToHex(unsigned int p);
			char int15ToHex(unsigned int p);
			
			KageLayerManager m_KageLayerManager;
			KageFramesManager m_KageFramesManager;
			
		protected:
			enum color {
				COLOR_RED,
				COLOR_GREEN,
				COLOR_BLUE
			};
			
			enum shape {
				SHAPE_SQUARE,
				SHAPE_RECTANGLE,
				SHAPE_OVAL
			};
			
			//Signal handlers:
			virtual void updateStatus(Glib::ustring status_msg);
			virtual void onButtonClick();
			virtual void LayerAdd_onClick();
			virtual void LayerRename_onClick();
			virtual void ShowHideLayer_onClick();
			virtual void LockUnlockLayer_onClick();
			virtual void LayerDel_onClick();
			virtual void onToolButtonsClick(Gtk::ToggleButton *p_sourceButton);
			virtual void onActionActivate();
			virtual void Undo_onClick();
			virtual void Redo_onClick();
			virtual void Cut_onClick();
			virtual void Copy_onClick();
			virtual void Paste_onClick();
			virtual void Delete_onClick();
			virtual void Duplicate_onClick();
			virtual void SelectAll_onClick();
			virtual void Deselect_onClick();
			virtual void ShapeGroup_onClick();
			virtual void ShapeUngroup_onClick();
			virtual void Raise_onClick();
			virtual void Lower_onClick();
			virtual void RaiseToTop_onClick();
			virtual void LowerToBottom_onClick();
			virtual void FlipHorizontal_onClick();
			virtual void FlipVertical_onClick();
			virtual void AddFrame_onClick();
			virtual void DuplicateFrame_onClick();
//			virtual void New_onClick();
			virtual void OpenKSF_onClick();
			virtual void Save_onClick();
			virtual void ExportKS_onClick();
			virtual void ExportHTML5_onClick();
			virtual void ExportPNG_onClick();
			virtual void ExportPNGTransparent_onClick();
			virtual void ExportPNGSequence_onClick();
			virtual void ExportAVI_onClick();
			
			unsigned int undoStackCount;
			unsigned int undoStackPointer;
			std::vector<unsigned int> undoStack;
			
			//Member widgets:
			Gtk::VBox m_VBoxRoot;
			Gtk::HBox m_HBoxToolbar;
			Gtk::VBox m_VBoxTimelineLayer;
			Gtk::HPaned m_Timeline_HPaned;
			Gtk::VBox m_Timeline_Layer_VBox;
			Gtk::HBox m_Timeline_Layer_Add_HBox;
			Gtk::Label m_Timeline_Label;
			Gtk::Button m_Timeline_Add_Button;
				Gtk::Image                m_Timeline_Add_Button_img;
				Glib::RefPtr<Gdk::Pixbuf> m_Timeline_Add_Button_pixbuf;
			Gtk::Button m_Timeline_Del_Button;
				Gtk::Image                m_Timeline_Del_Button_img;
				Glib::RefPtr<Gdk::Pixbuf> m_Timeline_Del_Button_pixbuf;
			Gtk::Label m_Timeline_CurrentFrame;
			Gtk::VBox m_Timeline_Frame_VBox1;
			Gtk::Label m_TimelineFrame_Label;
			Gtk::VBox m_Timeline_Frame_VBox2;
			Gtk::VBox m_VBoxTimelineFrame; //to be deleted
			Gtk::ScrolledWindow m_Timeline_Layer_ScrolledWindow;
			Gtk::ScrolledWindow m_Timeline_Frame_ScrolledWindow;
			Gtk::VBox m_VBoxToolbar_Holder;
			Gtk::VBox m_Box1;
			Gtk::HSeparator m_Separator;
			Gtk::HSeparator m_Separator_Toolbar1;
			Gtk::HSeparator m_Separator_Toolbar2;
			Gtk::HSeparator m_Separator_Toolbar3;
			Gtk::HSeparator m_Separator_Toolbar4;
			Gtk::Label m_LabelProp;
			//Stage Property items
				Gtk::HBox m_PropStage;
				Gtk::VBox m_PropStageV1;
				Gtk::VBox m_PropStageV2;
				Gtk::Label m_LabelStageWid;
				Gtk::Label m_LabelStageHgt;
				Gtk::Label m_LabelStageBG;
				Gtk::Label m_LabelStageFPS;
				
				Gtk::Entry m_EntryStageWid;
				Gtk::Entry m_EntryStageHgt;
					void EntryStageArea_onEnter();
				Gtk::ColorButton m_ColorButtonStage;
				Gtk::ColorSelection m_ColorSelectionStage;
					void ColorButtonStage_onClick();
				Gtk::Entry m_EntryStageFPS;
					void EntryStageFPS_onEnter();
			//Fill  & Stroke Property items
				Gtk::HBox m_PropFillStroke;
				Gtk::VBox m_PropFillStrokeV1;
				Gtk::VBox m_PropFillStrokeV2;
				
				Gtk::Label m_LabelFill;
				Gtk::ColorButton m_ColorButtonFill;
					void ColorButtonFill_onClick();
				Gtk::Label m_LabelFillAlpha;
				Gtk::Entry m_EntryFillAlpha;
					void EntryFillAlpha_onEnter();
				
				Gtk::Label m_LabelStroke;
				Gtk::Label m_LabelStrokeThickness;
				Gtk::Entry m_EntryStrokeThickness;
					void EntryStrokeThickness_onEnter();
				Gtk::Label m_LabelStrokeAlpha;
				Gtk::Entry m_EntryStrokeAlpha;
					void EntryStrokeAlpha_onEnter();
				Gtk::ColorButton m_ColorButtonStroke;
					void ColorButtonStroke_onClick();
			
			//X/Y Width/Height Shape's properties
				Gtk::HBox m_propLocationSize;
				Gtk::VBox m_propLocationSizeV1;
				Gtk::VBox m_propLocationSizeV2;
				Gtk::Label m_LabelX;
				Gtk::Entry m_EntryX;
					void EntryX_onEnter();
				Gtk::Label m_LabelY;
				Gtk::Entry m_EntryY;
					void EntryY_onEnter();
				Gtk::Label m_LabelWidth;
				Gtk::Entry m_EntryWidth;
					void EntryWidth_onEnter();
				Gtk::Label m_LabelHeight;
				Gtk::Entry m_EntryHeight;
					void EntryHeight_onEnter();
					
			//X/Y Node's properties
				Gtk::HBox m_propNodeXY;
				Gtk::VBox m_propNodeXYV1;
				Gtk::VBox m_propNodeXYV2;
				Gtk::Label m_LabelNodeX;
				Gtk::Entry m_EntryNodeX;
					void EntryNodeX_onEnter();
				Gtk::Label m_LabelNodeY;
				Gtk::Entry m_EntryNodeY;
					void EntryNodeY_onEnter();
				Gtk::Label m_LabelToggleLine;
				Gtk::Button m_ToggleLine;
					void ToggleLine_onClick();
			
			Gtk::Label m_LblHolder_Toolbar;
			KageStage m_KageStage;
			Glib::RefPtr<Gtk::UIManager> m_refUIManager;
			Glib::RefPtr<Gtk::ActionGroup> m_refActionGroup;
			
			void addToolButton(const Glib::ustring &label);
			Gtk::ToggleButton m_ToggleBtnSelect;
			void btnDebug_onClick();
			void btnAbout_onClick();
			void CheckUpdate_onClick();
			void Website_onClick();
			void openWebsite(string p_url);
			std::vector<Gtk::ToggleButton*> toggleButtons;
				std::vector<Glib::RefPtr<Gdk::Pixbuf>> toggleButtons_pixbuf;
				std::vector<Gtk::Image>                toggleButtons_img;
			std::vector<Gtk::Tooltip*> tooltips;
			Gtk::ToggleButton* currentTool;
			
			Gtk::HScrollbar m_Timeline_HScrollbar;
			Gtk::VScrollbar m_Timeline_VScrollbar;
			Gtk::HBox m_Timeline_HBox;
			
			Gtk::VPaned m_VPane_Timeline;
			Gtk::HPaned m_HPane_DrawingArea;
			Gtk::Statusbar m_Statusbar;
			
			unsigned m_ContextId;
			
			bool ksfInited;
			string ksfPath;
			std::ofstream ksfFile;
			
			bool expInited;
			string expPath;
			std::ofstream expFile;
			bool dtrace(string p_msg);
			bool saveKageStudio(string p_path, string p_msg);
			bool exportHtml5(string p_path, string p_msg);
			bool exportKonsolScript(string p_path, string p_msg);
			bool dump(string p_path, string p_msg);
			bool fileWrite(string p_path, string p_msg, std::ofstream &p_file, bool &p_flag);
			
			Gdk::Color m_Color;
			
			string openTextFile(string p_path);
			vector<double> parseNumbers(string p_numbers);
			vector<int> parseColorString(string p_color);
			void parseKSF_Children(vector<XmlTag> p_children);
			void parseKSF(string p_content);
			bool runExternal(string p_cmd, string p_param);
			
			void Play_onClick();
//			void ToolSelect_onClick();
			void ToolNode_onClick();
			void ToolPoly_onClick();
			void ToolOval_onClick();
			void ToolRectangle_onClick();
			void ToolStroke_onClick();
			void ToolFill_onClick();
			void ToolEyedrop_onClick();
			void ToolZoom_onClick();
			
			bool on_tick(const Glib::RefPtr<Gdk::FrameClock>& frame_clock);
			guint m_tick_id = 0;
			gint64 m_start_time = 0;
			const gint64 cycle_time = 3000000; // microseconds
			guint tickCounter;
			guint frameCounter;
			
		public:
			void toolsButtonToggle(string p_toolTip);
			
			void ToolSelect_onClick();
			
			static void timestamp();
			
			void switchToPreviousFrame();
			void switchToNextFrame();
			void updateFrameLabel();
			
			void propStageSetVisible(bool p_visible);
			void propFillStrokeSetVisible(bool p_visible);
			void propLocationSizeSetVisible(bool p_visible);
			void propNodeXYSetVisible(bool p_visible);
			void updateColors();
			void updateProperties();
			void updateNodeXY();
			void updateSelectedShapeColor(bool p_doFill = true, bool p_doStroke = true);
			
			UnRe _undoRedoManager;
			void stackDo();
			void stackDoZoom(PointData p_originBefore, PointData p_originAfter, PointData p_zoomReference, double p_zoomRatio);
			
			virtual void New_onClick();
			
			bool isLayerLocked();
			unsigned int getCurrentLayer();
			void setCurrentLayer(unsigned int p_layer);
			unsigned int getCurrentFrame();
			void setCurrentFrame(unsigned int p_layer);
	};

#endif //GTKMM_KAGE_H
