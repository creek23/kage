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

#ifndef GTKMM_KAGE_H
	#define GTKMM_KAGE_H
	
	#include <gtkmm/togglebutton.h>
	#include <gtkmm/paned.h>
	#include <gtkmm/scrolledwindow.h>
	#include <gtkmm/separator.h>
	#include <gtkmm/scrollbar.h>
	#include <gtkmm/uimanager.h>
	#include <gtkmm/statusbar.h>
	#include <gtkmm/radioaction.h>
	#include <gtkmm/messagedialog.h>
	#include <gtkmm/filechooserdialog.h>
	#include <gtkmm/cssprovider.h>
	#include "kage/timeline/layermanager.h"
	#include "kage/timeline/timeline.h"
	#include "kage/stage/stage.h"
	#include "kage/library/assetmanager.h"
	#include "kage/library/library.h"
	#include "kage/properties/fillstroke.h"
	#include "kage/properties/frametween.h"
	#include "kage/properties/locationsize.h"
	#include "kage/properties/nodexy.h"
	#include "kage/properties/stage.h"
	#include "kage/data/frame.h"
	#include "kage/data/layer.h"
	#include "kage/data/scene.h"
	#include "kage/document.h"
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
			Kage(string p_filePath);
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
			string dumpFrameToSvg();

			unsigned int getTween();
			
			string int255ToHex(unsigned int p);
			char int15ToHex(unsigned int p);
			unsigned int hexToInt255(string p);
			unsigned int hexToInt15(char p);
			
			KageLayerManager _layerManager;
			KageAssetManager _assetManager;
			KageTimeline _timeline;
			
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
			void ToggleOnionSkin_onClick();
			void ToggleOnionLayer_onClick();
			void ToggleOnion_onClick();
			void ToolButtons_onClick(Gtk::ToggleButton *p_sourceButton);
			void onActionActivate();
			//void Undo_onClick();
			//void Redo_onClick();
			void Cut_onClick();
			void Copy_onClick();
			void Paste_onClick();
			void Delete_onClick();
			void Duplicate_onClick();
			void SelectAll_onClick();
			void Deselect_onClick();
			void ToggleTimeline_onClick();
			void ToggleProperties_onClick();
				void TogglePropertiesFill_onClick();
				void TogglePropertiesStroke_onClick();
			void ShapeGroup_onClick();
			void ShapeUngroup_onClick();
			void Raise_onClick();
			void Lower_onClick();
			void RaiseToTop_onClick();
			void LowerToBottom_onClick();
			void FlipHorizontal_onClick();
			void FlipVertical_onClick();
			
			void RecenterRotationPoint_onClick();
			void Tween_onClick();
			
			void PreviousFrame_onClick();
			void NextFrame_onClick();
			void ToggleLoop_onClick();

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
			void ExportSVG_onClick();
			void ExportPNG_onClick();
			void ExportPNGTransparent_onClick();
			void ExportPNGSpritesheet_onClick();
			void ExportPNGSequence_onClick();
			void ExportVideo_onClick();
			void ProjectSave_onClick(); //place holder
			void ImportAsset_onClick();
			
			virtual bool on_delete_event(GdkEventAny* any_event);
			virtual bool on_key_press_event(GdkEventKey *e) override;
			virtual bool on_key_release_event(GdkEventKey *e) override;
			
			void doOpen();
			void doSaveDialog(string p_title);
			void doSave(string p_filename);
			
			void doExportPNGDialog(string p_title, bool p_transparent);
			
			//Member widgets:
			Gtk::VBox m_VBoxRoot;
			Gtk::HBox m_HBoxToolbar;
			Gtk::VBox m_VBoxTimelineLayer;
			Gtk::HPaned m_Timeline_HPaned;
			Gtk::VBox m_Timeline_Layer_VBox;
			Gtk::VBox m_Timeline_Layer_VBox1;
			Gtk::VBox m_Timeline_Layer_VBox2;
			
			Gtk::HBox m_Timeline_Layer_Controls_HBox;
			Gtk::HBox m_Timeline_Controls_HBox;
			Gtk::Label m_Timeline_Label;
			Gtk::Button _btnLayerAdd;
				Gtk::Image                _btnLayerAdd_img;
				Glib::RefPtr<Gdk::Pixbuf> _btnLayerAdd_pixbuf;
			Gtk::Button _btnLayerDelete;
				Gtk::Image                _btnLayerDelete_img;
				Glib::RefPtr<Gdk::Pixbuf> _btnLayerDelete_pixbuf;
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
			
			Gtk::Button _btnTimelineRewind;
				Gtk::Image                _btnTimelineRewind_img;
				Glib::RefPtr<Gdk::Pixbuf> _btnTimelineRewind_pixbuf;
			Gtk::Button _btnTimelinePrevious;
				Gtk::Image                _btnTimelinePrevious_img;
				Glib::RefPtr<Gdk::Pixbuf> _btnTimelinePrevious_pixbuf;
			Gtk::Button _btnTimelinePlay;
				Gtk::Image                _btnTimelinePlay_img;
				Glib::RefPtr<Gdk::Pixbuf> _btnTimelinePlay_pixbuf;
			Gtk::Button _btnTimelineNext;
				Gtk::Image                _btnTimelineNext_img;
				Glib::RefPtr<Gdk::Pixbuf> _btnTimelineNext_pixbuf;
			Gtk::Button _btnTimelineStop;
				Gtk::Image                _btnTimelineStop_img;
				Glib::RefPtr<Gdk::Pixbuf> _btnTimelineStop_pixbuf;
			Gtk::ToggleButton _btnTimelineLoop;
				Gtk::Image                _btnTimelineLoop_img;
				Glib::RefPtr<Gdk::Pixbuf> _btnTimelineLoop_pixbuf;
			public:
			Gtk::ToggleButton _toggleOnion;
			protected:
				Gtk::Image                _toggleOnion_img;
				Glib::RefPtr<Gdk::Pixbuf> _toggleOnion_pixbuf;
			public:
			Gtk::ToggleButton _toggleOnionLayer;
			protected:
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
			Gtk::HBox m_Box1;
			Gtk::VBox m_PropertyBox;
			Gtk::HSeparator m_Separator;
			Gtk::HSeparator m_Separator_Toolbar1;
			Gtk::HSeparator m_Separator_Toolbar2;
			Gtk::HSeparator m_Separator_Toolbar3;
			Gtk::HSeparator m_Separator_Toolbar4;
			Gtk::HSeparator m_Separator_Library1;
			Gtk::HSeparator m_Separator_Library2;
			Gtk::Label m_LabelProp;
			Gtk::ScrolledWindow m_Property_Pane_ScrolledWindow;
			Gtk::VScrollbar     m_Property_Pane_VScrollbar;
			Gtk::Label m_LabelLibrary;			
			
			Gtk::Label m_LblHolder_Toolbar;
			Gtk::ScrolledWindow m_Stage_Pane_ScrolledWindow;
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
			
			Gtk::HScrollbar m_Timeline_Layer_HScrollbar;
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
			bool exportSvg(string p_path, string p_msg);
			bool exportKonsolScript(string p_path, string p_msg);
			bool dump(string p_path, string p_msg);
			bool fileWrite(string p_path, string p_msg, std::ofstream &p_file, bool &p_flag);
			
			string openTextFile(string p_path);
			vector<double> parseNumbers(string p_numbers);
			vector<int> parseColorString(string p_color);
			void parseKSF_Children(vector<XmlTag> p_children);
			void parseKSF(string p_content);
			bool runExternal(string p_cmd, string p_param);
			
			void Play_onClick();
			void PlayFrame_onClick();
//			void Stop_onClick();
//			void ToolSelect_onClick();
			void ToolNode_onClick();
			void ToolPoly_onClick();
			void ToolPencil_onClick();
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
			
			unsigned int tickCounter;
			guint frameCounter;
			//bool _isPlaying;
			
			vector<VectorData> _undoBase;
			
			bool continueNewFileWithUnsavedWork();
			
			int _area_timeline_pane;
			int _area_properties_pane1;
			int _area_properties_pane2;
			
			bool m_LabelLibrary_onClick(GdkEventButton *event);//, gpointer user_data);
		public:
			KageStage _stage;
				bool _UPDATE_SHAPE_COLORS;
				bool _propFillVisible = false;
				bool _propStrokeVisible = false;
			PropertyStage m_PropStage;
			PropertyLocationSize m_propLocationSize;
			PropertyFillStroke m_propFillStroke;
			PropertyNodeXY m_propNodeXY;
			PropertyFrameTween m_propFrameTween;
			KageLibrary  _library;
			Gdk::Color m_Color; //fill or stroke?
			void Undo_onClick();
			void Redo_onClick();
			void Stop_onClick();
			bool _isPlaying;
			
			void toolsButtonToggle(string p_toolTip);
			
			void ToolSelect_onClick();
			
			static void timestamp();
			static void timestamp_IN();
			static void timestamp_OUT();
			static unsigned int TAB_COUNT;
			
			bool switchToPreviousFrame();
			bool switchToNextFrame();
			
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
			
			void refreshUI();
			void focusOnStage();

			KageDocument _document;
			
			void setStageBG(Gdk::Color p_Color);
			Gdk::Color getStageBG();
	};

#endif //GTKMM_KAGE_H
