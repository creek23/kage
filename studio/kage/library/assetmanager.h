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
 * MA 02110-1301, USA.  Or, see <https://www.gnu.org/licenses/>.
 * 
 */

#ifndef GTKMM_KAGE_ASSET_MANAGER_H
	#define GTKMM_KAGE_ASSET_MANAGER_H
	
	#include <gtkmm/box.h>
	#include <gtkmm/label.h>
	#include <gtkmm/button.h>
	#include "asset.h"
	#include "../../label_rename.h"
	#include "../../util/string/stringhelper.h"
	
	#include <filesystem>
	
	class Kage;
	
	class KageAssetManager : public Gtk::VBox {
		public:
			KageAssetManager(Kage *p_kage);
			virtual ~KageAssetManager();
			unsigned int assetCount();
			unsigned int addAsset(std::string p_name = "");
			unsigned int addImageAsset(std::string p_name = "");
			void deleteAsset();
			bool removeAllAssets();
			void setSelected(KageAsset *p_asset);
			void setAboveAssetSelected(KageAsset *p_asset);
			void setBelowAssetSelected(KageAsset *p_asset);
			KageAsset *getAsset();
			unsigned int getCurrentAsset();
			void setCurrentAsset(unsigned int p_asset);
			void setCurrentAssetByID(unsigned int p_assetID);
			void renderStage();
			
			KageAsset::AssetType getAssetType();
			void setAssetType(KageAsset::AssetType p_assetType);
			
			void render(unsigned int p_renderID);
			void forceRender();
			
			void renameAsset(KageAsset *p_asset);
			void renameAsset();
			void setLabel(std::string p_label);
			std::string getLabel();
			
			bool moveToTop();
			bool moveUp();
			bool moveDown();
			bool moveToBottom();

			bool exists(std::string p_filePath);
			void setAssetHash(std::string p_assetHash);
			
			std::string _KageStudioAsset;
			std::string getImagePathByID(unsigned int p_index);
			std::string getImageByID(unsigned int p_id);
			std::string getFilePathAt(unsigned int p_index);
			std::string getFileNameAt(unsigned int p_index);
			std::string getLabelAt(unsigned int p_index);
			std::string getHashAt(unsigned int p_index);
			std::string getIDAt(unsigned int p_index);
			bool setFilePathAt(unsigned int p_index, std::string p_filePath);
			bool setFileNameAt(unsigned int p_index, std::string p_fileName);
			bool setHashAt(unsigned int p_index, std::string p_hash);
			
			std::string saveAssetsTo(std::string p_path);
			

			void renderLibrary(unsigned int p_ID);
			
			Kage *_kage;

			unsigned int _selectedAssetID;
		protected:
			unsigned int assetCtr;
			std::vector<Gtk::Button*> labels;
			std::vector<KageAsset*> assets;
			unsigned int _currentAssetID;
			unsigned int _currentAssetIndex;
	};
#endif // GTKMM_KAGE_ASSET_MANAGER_H
