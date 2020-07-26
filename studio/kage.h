/*
 * Kage Studio - a simple free and open source vector-based 2D animation software
 * Copyright (C) 2011~2020  Mj Mendoza IV
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
	#include <gtkmm/colorchooser.h>
	#include <gtkmm/comboboxtext.h>
	#include <gtkmm/liststore.h>
	#include <gtkmm/scale.h>
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
	#include "util/cairo/cairo_kage.h"
	
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
			VectorDataManager getFrameDataAt(unsigned int p_frame);
			void setFrameData(VectorDataManager p_vectorsData);
			void setFrameData(VectorDataManager p_vectorsData, bool p_force);
			bool isFrameEmpty();
			void forceRenderFrames();
			void renderOnionFrames();
			void renderFrames();
			void renderFramesBelowCurrentLayer();
			void renderFramesAboveCurrentLayer();
			string saveFrame();
			string dumpFrame(bool bKS);
			
			unsigned int getTween();
			
			string int255ToHex(unsigned int p);
			char int15ToHex(unsigned int p);
			
			KageLayerManager _layerManager;
			KageFramesetManager _framesetManager;
			
		protected:
			//Signal handlers:
			void updateStatus(Glib::ustring status_msg);
			void Quit_onClick();
			void LayerAdd_onClick();
			void LayerRename_onClick();
			void ShowHideLayer_onClick();
			void LockUnlockLayer_onClick();
			void LayerDel_onClick();
			void LayerMoveTop_onClick();
			void LayerMoveUp_onClick();
			void LayerMoveDown_onClick();
			void LayerMoveBottom_onClick();
			void ToggleOnion_onClick();
			void ToolButtons_onClick(Gtk::ToggleButton *p_sourceButton);
			void onActionActivate();
			void Undo_onClick();
			void Redo_onClick();
			void Cut_onClick();
			void Copy_onClick();
			void Paste_onClick();
			void Delete_onClick();
			void Duplicate_onClick();
			void SelectAll_onClick();
			void Deselect_onClick();
			void ToggleTimeline_onClick();
			void ToggleProperties_onClick();
			void ShapeGroup_onClick();
			void ShapeUngroup_onClick();
			void Raise_onClick();
			void Lower_onClick();
			void RaiseToTop_onClick();
			void LowerToBottom_onClick();
			void FlipHorizontal_onClick();
			void FlipVertical_onClick();
			
			void RecenterRotationPoint_onClick();
			
			void AddFrame_onClick();
			void ExtendFrame_onClick();
			void DuplicateFrame_onClick();
			void RemoveFrame_onClick();
			void CutFrame_onClick();
			void CopyFrame_onClick();
			void PasteFrame_onClick();
			void DeleteFrame_onClick();
			void TweenFrame_onClick();
			void RemoveTweenFrame_onClick();
//			void New_onClick();
			void OpenKSF_onClick();
			void Save_onClick();
			void SaveAs_onClick();
			void ExportKS_onClick();
			void ExportHTML5_onClick();
			void ExportPNG_onClick();
			void ExportPNGTransparent_onClick();
			void ExportPNGSequence_onClick();
			void ExportAVI_onClick();
			void ProjectSave_onClick(); //place holder
			
			virtual bool on_delete_event(GdkEventAny* any_event);
			
			void doSaveDialog(string p_title);
			void doSave(string p_filename);
			
			void doExportPNGDialog(string p_title, bool p_transparent);
			
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
			Gtk::Button _btnLayerMoveTop;
				Gtk::Image                _btnLayerMoveTop_img;
				Glib::RefPtr<Gdk::Pixbuf> _btnLayerMoveTop_pixbuf;
			Gtk::Button _btnLayerMoveUp;
				Gtk::Image                _btnLayerMoveUp_img;
				Glib::RefPtr<Gdk::Pixbuf> _btnLayerMoveUp_pixbuf;
			Gtk::Button _btnLayerMoveDown;
				Gtk::Image                _btnLayerMoveDown_img;
				Glib::RefPtr<Gdk::Pixbuf> _btnLayerMoveDown_pixbuf;
			Gtk::Button _btnLayerMoveBottom;
				Gtk::Image                _btnLayerMoveBottom_img;
				Glib::RefPtr<Gdk::Pixbuf> _btnLayerMoveBottom_pixbuf;
			Gtk::ToggleButton _toggleOnion;
				Gtk::Image                _toggleOnion_img;
				Glib::RefPtr<Gdk::Pixbuf> _toggleOnion_pixbuf;
			Gtk::ToggleButton _toggleOnionLayer;
				Gtk::Image                _toggleOnionLayer_img;
				Glib::RefPtr<Gdk::Pixbuf> _toggleOnionLayer_pixbuf;
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
//				Gtk::ColorChooser m_ColorChooser;
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
				Glib::RefPtr<Gtk::Adjustment> _adjustFill;
				Glib::RefPtr<Gtk::Adjustment> _adjustStroke;
				Gtk::Scale _scaleFillAplha;
					void FillAlpha_onChange();
				
				Gtk::Label m_LabelStroke;
				Gtk::Label m_LabelStrokeThickness;
				Gtk::Entry m_EntryStrokeThickness;
					void EntryStrokeThickness_onEnter();
				Gtk::Label m_LabelStrokeAlpha;
				Gtk::Scale _scaleStrokeAlpha;
					void StrokeAlpha_onChange();
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
				Gtk::Button _btnToggleLine;
					Gtk::Image                _btnToggleLine_img;
					Glib::RefPtr<Gdk::Pixbuf> _btnToggleLine_pixbuf;
					void ToggleLine_onClick();
			
			//Frame's properties
				Gtk::HBox m_propFrameTween;
				Gtk::VBox m_propFrameTweenV1;
				Gtk::VBox m_propFrameTweenV2;
				Gtk::Label m_LabelTweenX;
				Gtk::ComboBoxText m_ComboX;
				Gtk::Label m_LabelTweenY;
				Gtk::ComboBoxText m_ComboY;
					void FrameTween_onChange();
			
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
				Gtk::Label _labelStatusMouseXY;
			
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
			void PlayFrame_onClick();
			void Stop_onClick();
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
			bool _isPlaying;
			
			vector<VectorData> _undoBase;
			
			bool continueNewFileWithUnsavedWork();
			
			void refreshUI();
			
			int _area_timeline_pane;
			int _area_properties_pane1;
			int _area_properties_pane2;
			
		public:
			void toolsButtonToggle(string p_toolTip);
			
			void ToolSelect_onClick();
			
			static void timestamp();
			
			void switchToPreviousFrame();
			void switchToNextFrame();
			void updateFrameLabel();
			
			void propStageSetVisible(bool p_visible);
			void propFillStrokeSetVisible(bool p_visible);
			void propShapePropertiesSetVisible(bool p_visible);
			void propNodeXYSetVisible(bool p_visible);
			void propFrameTweenSetVisible(bool p_visible);
			void updateColors();
			void updateShapeProperties();
			void updateNodeXY();
			void updateSelectedShapeColor(bool p_doFill = true, bool p_doStroke = true);
			
			UnRe _undoRedoManager;
			void stackDo();
			void stackDoZoom(PointData p_originBefore, PointData p_originAfter, PointData p_zoomReference, double p_zoomRatio);
			
			void New_onClick();
			bool doDeleteFrame();
			
			bool isLayerLocked();
			unsigned int getCurrentLayer();
			void setCurrentLayer(unsigned int p_layer);
			unsigned int getCurrentFrame();
			void setCurrentLayerByID(unsigned int p_layerID);
			void setCurrentFrame(unsigned int p_layer);
			
			void displayMouseXY(double p_x, double p_y);
			
			void registerPropertiesPane();
	};

#endif //GTKMM_KAGE_H
