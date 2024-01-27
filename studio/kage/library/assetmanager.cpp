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

#include "assetmanager.h"
#include "../../kage.h"

KageAssetManager::KageAssetManager(Kage *p_kage) {
	_kage = p_kage;
	assetCtr = 0;

	_KageStudioAsset = StringHelper::kHash("Kage Studio Asset", 24);

	assets.clear();
}

KageAssetManager::~KageAssetManager() {
	assets.clear();
}

unsigned int KageAssetManager::addAsset(string p_name) {
	string l_filePath;
	string l_fileName;
	
	/**
	 * TODO: 
	 * 1. [DONE] check p_name exists -- done in Kage with AssetManager
	 * 2. [DONE]if project is saved, copy file to project path
	 *    [DONE] else, copy file to temporary
	 * 3. [DONE] create hash for path and filesize for future check by assetManager if already existing in Library
	 *    use kHash from Quixie
	 */
	
	string l_hash = StringHelper::kHash(p_name, 24);
	int l_len = strlen(p_name.c_str()) - 4;
	if (p_name == "") {
		//p_name = Glib::ustring::compose("Asset %1", assetCtr);
		std::cout << "NOTE: creating Kage Asset will have no existing file unless User save the Project";
		//TODO: make this create Asset for KSF
		return UINT_MAX;
	} else if (StringHelper::toLower(p_name).substr(l_len, 4) == ".ksf") {
		//TODO: use Kage to read KSF then add to _document.Scenes 
	} else if (StringHelper::toLower(p_name).substr(l_len, 4) == ".png") {
		/**
		 * TODO:
		 * 1. [DONE] check if file p_name exists
		 * 2. [DONE] if existing, copy to temp path
		 * 3. [DONE] load copied file -- use original path as hash
		 * 4. [DONE] select image on stage
		 * 5. draw rectangle on selected image on stage
		 * 6. resize image on stage
		 * 7. [DONE] delete image on stage
		 * 8. [DONE] add image on stage FROM LIBRARY (double click)
		 * 9. [DONE] add image on stage FROM ASSET (double click)
		 * 10. add image on stage FROM LIBRARY (drag-n-drop)
		 * 11. add image on stage FROM ASSET (drag-n-drop)
		 */
		
		filesystem::path l_sourceFile = p_name;
		if (filesystem::exists(l_sourceFile) == true) {
			l_fileName = l_sourceFile.filename().u8string();
			l_filePath = filesystem::path(p_name).remove_filename().u8string();

			filesystem::path l_currentPath = filesystem::current_path();
			filesystem::current_path(filesystem::temp_directory_path());
			
				filesystem::create_directories(_KageStudioAsset);
				//const auto copyOptions = filesystem::copy_options::update_existing;
									//| filesystem::copy_options::recursive; <-- use in future when importing project file which will import ALL assets
				filesystem::path l_assetFile = filesystem::temp_directory_path() / _KageStudioAsset / l_hash;
				if (filesystem::exists(l_assetFile) == true) {
					//delete l_assetFile
					const char* filename = l_assetFile.u8string().c_str();

					//if (filesystem::remove_all(l_assetFile) == false) { //it's not deleting somehow
					if (std::remove(filename) != 0) {
						std::perror("Error deleting file");
						return UINT_MAX;
					} else {
						std::puts("File successfully deleted");
					}
				}
				
				//filesystem::copy(l_sourceFile, l_assetFile, filesystem::copy_options::update_existing);
				//as above code is not working with GCC 13
				std::ifstream src(l_sourceFile.u8string(), std::ios::binary);
				std::ofstream dst( l_assetFile.u8string(), std::ios::binary);
					dst << src.rdbuf();
			
			filesystem::current_path(l_currentPath);
			
			p_name = l_fileName;
		} else {
			return UINT_MAX;
		} 
	}
	
	assets.push_back(Gtk::manage(new KageAsset(this, assetCtr)));
		pack_end(*assets.back(), Gtk::PACK_SHRINK);
			(*assets.back()).setLabel(p_name);
			(*assets.back()).set_size_request(100, 23);
			(*assets.back()).set_focus_on_click(false);
			(*assets.back()).setFilePath(l_filePath); //is it still relevant if we have the hash version?
			(*assets.back()).setFileName(l_fileName);
			(*assets.back()).setAssetHash(l_hash);
	
	_currentAssetID = (*assets.back()).assetID;
	_currentAssetIndex = assets.size()-1;
	
	return assetCtr++;
}

/**
 * @brief Used by Kage when opening KAGE files with PNG Assets
 * Blindly accepts that p_name is actually a PNG file.
 * 
 * @param p_name Image file with full path
 * @return unsigned int 
 */
unsigned int KageAssetManager::addImageAsset(string p_name) {
	string l_filePath;
	string l_fileName;
	
	string l_hash = StringHelper::kHash(p_name, 24);
	if (p_name == "") {
		return UINT_MAX;
	} else {
		filesystem::path l_sourceFile = p_name;
		if (filesystem::exists(l_sourceFile) == true) {
			l_fileName = l_sourceFile.filename().u8string();
			l_filePath = filesystem::path(p_name).remove_filename().u8string();
			l_hash = l_fileName;
			std::cout << "l_fileName " << l_fileName << std::endl;
			std::cout << "l_filePath " << l_filePath << std::endl;
			filesystem::path l_currentPath = filesystem::current_path();
			filesystem::current_path(filesystem::temp_directory_path());
			
				filesystem::create_directories(_KageStudioAsset);
				//const auto copyOptions = filesystem::copy_options::update_existing;
									//| filesystem::copy_options::recursive; <-- use in future when importing project file which will import ALL assets
				filesystem::path l_assetFile = filesystem::temp_directory_path() / _KageStudioAsset / l_hash;
				if (filesystem::exists(l_assetFile) == true) {
					//delete l_assetFile
					const char* filename = l_assetFile.u8string().c_str();

					//if (filesystem::remove_all(l_assetFile) == false) { //it's not deleting somehow
					if (std::remove(filename) != 0) {
						std::perror("Error deleting file");
						return UINT_MAX;
					} else {
						std::puts("File successfully deleted");
					}
				}
				
				//filesystem::copy(l_sourceFile, l_assetFile, filesystem::copy_options::update_existing);
				//as above code is not working with GCC 13
				std::ifstream src(l_sourceFile.u8string(), std::ios::binary);
				std::ofstream dst( l_assetFile.u8string(), std::ios::binary);
					dst << src.rdbuf();
			
			filesystem::current_path(l_currentPath);
			
			p_name = l_fileName;
		} else {
			return UINT_MAX;
		} 
	}
	
	assets.push_back(Gtk::manage(new KageAsset(this, assetCtr)));
		pack_end(*assets.back(), Gtk::PACK_SHRINK);
			(*assets.back()).setLabel(p_name);
			(*assets.back()).set_size_request(100, 23);
			(*assets.back()).set_focus_on_click(false);
			(*assets.back()).setFilePath(l_filePath); //is it still relevant if we have the hash version?
			(*assets.back()).setFileName(l_fileName);
			(*assets.back()).setAssetHash(l_hash);
	
	_currentAssetID = (*assets.back()).assetID;
	_currentAssetIndex = assets.size()-1;
	
	return assetCtr++;
}

void KageAssetManager::deleteAsset() {
	if (_currentAssetIndex < assetCount() && assets[_currentAssetIndex]->assetID == _currentAssetID) {
		if (assets[_currentAssetIndex]->isSelected()) {
			std::cout << " deleting; index " << _currentAssetIndex << std::endl;
			remove(*assets[_currentAssetIndex]);
			delete assets[_currentAssetIndex];
			assets.erase (assets.begin() + (_currentAssetIndex));
			if (_currentAssetIndex < assets.size()) {
				//do nothing
			} else if (_currentAssetIndex-1 >= 0) {
				--_currentAssetIndex;
			}
			_currentAssetID = assets[_currentAssetIndex]->assetID;
			assets[_currentAssetIndex]->setSelected(true);
		} else {
			std::cout << " asset not selected" << std::endl;
		}
	} else {
		std::cout << " unknown asset; identifying..." << std::endl;
		for (unsigned int i = 0; i < assetCount(); ++i) {
			if (assets[i]->assetID == _currentAssetID) {
				if (assets[i]->isSelected()) {
					std::cout << " deleting; index " << i << std::endl;
					remove(*assets[i]);
					delete assets[i];
					assets.erase (assets.begin() + (i-1));
					if (_currentAssetIndex < assets.size()) {
						//do nothing
					} else if (_currentAssetIndex-1 >= 0) {
						--_currentAssetIndex;
					}
					_currentAssetID = assets[_currentAssetIndex]->assetID;
					assets[_currentAssetIndex]->setSelected(true);
					return;
				}
				break;
			}
		}
	}
}

KageAsset::AssetType KageAssetManager::getAssetType() {
	if (_currentAssetIndex < assetCount() && assets[_currentAssetIndex]->assetID == _currentAssetID) {
		return assets[_currentAssetIndex]->getAssetType();
	} else {
		for (unsigned int i = 0; i < assetCount(); ++i) {
			if (assets[i]->assetID == _currentAssetID) {
				_currentAssetIndex = i;
				return assets[i]->getAssetType();
			}
		}
	}
	return KageAsset::AssetType::ASSET_NULL;
}
/** Set the AssetType of the selected Asset
 * \param p_assetType which AssetType to set
 */
void KageAssetManager::setAssetType(KageAsset::AssetType p_assetType) {
	if (_currentAssetIndex < assetCount() && assets[_currentAssetIndex]->assetID == _currentAssetID) {
		assets[_currentAssetIndex]->setAssetType(p_assetType);
	} else {
		for (unsigned int i = 0; i < assetCount(); ++i) {
			if (assets[i]->assetID == _currentAssetID) {
				_currentAssetIndex = i;
				assets[i]->setAssetType(p_assetType);
				break;
			}
		}
	}
}

void KageAssetManager::render(unsigned int p_renderID) {
	if (_currentAssetIndex < assetCount() && assets[_currentAssetIndex]->assetID == _currentAssetID) {
		assets[_currentAssetIndex]->render(p_renderID);
	} else {
		for (unsigned int i = 0; i < assetCount(); ++i) {
			if (assets[i]->assetID == _currentAssetID) {
				_currentAssetIndex = i;
				assets[i]->render(p_renderID);
				break;
			}
		}
	}
}

/**
 * @brief 
 * 
 */
void KageAssetManager::renderLibrary(unsigned int p_ID) {
	_kage->_library.render(p_ID);
}
bool KageAssetManager::removeAllAssets() {
	for (unsigned int i = 0; i < assetCount(); ++i) {
		remove(*assets[i]);
	}
	assetCtr = 0;
	assets.clear();
	return true;
}

/** For use of KageAsset.  When a KageAsset is clicked, previously clicked
 * KageAsset should be un-selected.
 * \param p_asset is pointer to KageAsset who called this function
 */
void KageAssetManager::setSelected(KageAsset *p_asset) {
	if (_currentAssetIndex < assetCount() && assets[_currentAssetIndex]->assetID == _currentAssetID) {
		assets[_currentAssetIndex]->setSelected(false);
	} else {
		for (unsigned int i = 0; i < assetCount(); ++i) {
			if (assets[i]->assetID == _currentAssetID) {
				assets[i]->setSelected(false);
				break;
			}
		}
	}
	for (unsigned int i = 0; i < assetCount(); ++i) {
		if (assets[i]->assetID == p_asset->assetID) {
			_currentAssetIndex = i;
			break;
		}
	}
	p_asset->setSelected(true);
	_currentAssetID = p_asset->assetID;
	
//	_kage->setDocumentSceneLayerCurrentFrame(_kage->getDocumentSceneLayerCurrentFrame(), false);
}

/**
 * @brief For use by Asset when Arrow Up is pressed
 * 
 * @param p_asset source asset where Key Press happened
 */
void KageAssetManager::setAboveAssetSelected(KageAsset *p_asset) {
	for (unsigned int i = 0; i < assetCount(); ++i) {
		if (assets[i]->assetID == p_asset->assetID && i < assetCount()-1) {
			p_asset->setSelected(false);
			_currentAssetIndex = i+1;
			_currentAssetID = assets[_currentAssetIndex]->assetID;
			assets[_currentAssetIndex]->setSelected(true);
			assets[_currentAssetIndex]->render();
			break;
		}
	}
}

/**
 * @brief For use by Asset when Arrow Down is pressed
 * 
 * @param p_asset source asset where Key Press happened
 */
void KageAssetManager::setBelowAssetSelected(KageAsset *p_asset) {
	for (unsigned int i = 0; i < assetCount(); ++i) {
		if (assets[i]->assetID == p_asset->assetID && i > 0) {
			p_asset->setSelected(false);
			_currentAssetIndex = i-1;
			_currentAssetID = assets[_currentAssetIndex]->assetID;
			assets[_currentAssetIndex]->setSelected(true);
			assets[_currentAssetIndex]->render();
			break;
		}
	}
}

/** Not to be confused of getCurrentAsset().  This function returns the
 * pointer to active KageAsset Object to caller
 * \return pointer to registered active asset
 */
KageAsset *KageAssetManager::getAsset() {
	if (_currentAssetIndex < assetCount() && assets[_currentAssetIndex]->assetID == _currentAssetID) {
		return assets[_currentAssetIndex];
	} else {
		for (unsigned int i = 0; i < assetCount(); ++i) {
			if (assets[i]->assetID == _currentAssetID) {
				_currentAssetIndex = i;
				return assets[i];
			}
		}
	}
	return NULL;
}

/** For use of Kage.  To keep track of active Asset in-check of all
 * assets in KageAssetManager.
 * \return index of registered active asset
 * \sa setCurrentAsset()
 */
unsigned int KageAssetManager::getCurrentAsset() {
	if (_currentAssetIndex < assetCount() && assets[_currentAssetIndex]->assetID == _currentAssetID) {
		return _currentAssetIndex+1;
	} else {
		for (unsigned int i = 0; i < assetCount(); ++i) {
			if (assets[i]->assetID == _currentAssetID) {
				_currentAssetIndex = i;
				return i+1; //temporarily add +1 as we integrate code to previous implementation
			}
		}
	}
	return assets.size(); //temporarily DONT -1 as we integrate code to previous implementation
}
/** For use of Kage.  When a KageFrame is clicked, KageFramesetManager will
 * call this function via Kage then sets currently active Asset via Index
 * \param p_asset is index of Asset known to KageFrame
 * \sa getCurrentAsset()
 */
void KageAssetManager::setCurrentAsset(unsigned int p_asset) {
	if (_currentAssetIndex < assetCount() && assets[_currentAssetIndex]->assetID == _currentAssetID) {
		assets[_currentAssetIndex]->setSelected(false);
	} else {
		for (unsigned int i = 0; i < assetCount(); ++i) {
			if (assets[i]->assetID == _currentAssetID) {
				assets[i]->setSelected(false);
				break;
			}
		}
	}
	
	unsigned int l_count = assetCount();
	if (p_asset > l_count) {
		p_asset = l_count;
	}
	if (p_asset < 1) {
		p_asset = 1;
	}
	_currentAssetIndex = p_asset-1;
	_currentAssetID = assets[_currentAssetIndex]->assetID;
	assets[_currentAssetIndex]->setSelected(true);
}

/** For use of Kage.  When a KageFrame is clicked, KageFramesetManager will
 * call this function via Kage then sets currently active Asset via Asset ID
 * \param p_assetID is assetID of Asset known to KageFrame
 * \sa getCurrentAsset()
 */
void KageAssetManager::setCurrentAssetByID(unsigned int p_assetID) {
	//unselect current asset
	if (_currentAssetIndex < assetCount() && assets[_currentAssetIndex]->assetID == _currentAssetID) {
		assets[_currentAssetIndex]->setSelected(false);
	} else {
		for (unsigned int i = 0; i < assetCount(); ++i) {
			if (assets[i]->assetID == _currentAssetID) {
				_currentAssetID = p_assetID;
				assets[i]->setSelected(false);
				break;
			}
		}
	}
	//select new asset by ID	
	for (unsigned int i = 0; i < assetCount(); ++i) {
		if (assets[i]->assetID == p_assetID) {
			_currentAssetIndex = i;
			_currentAssetID = p_assetID;
			assets[_currentAssetIndex]->setSelected(true);
			break;
		}
	}
}


/** For use of KageAsset.  When a KageAsset's visibility is clicked,
 * this function will be called to reflect Asset's visibility on stage.
 */
void KageAssetManager::renderStage() {
	if (KageScene::LOADING_MODE == true) {
		return;
	}
	Kage::timestamp_IN();
	std::cout << " KageAssetManager::renderStage <" << std::endl;
//	_kage->forceRenderFrames();
//	_kage->refreshUI();
	Kage::timestamp_OUT();
}

/** For use of Kage when exporting/saving to file.
 * \return size of array of KageAsset
 */
unsigned int KageAssetManager::assetCount() {
	return assets.size();
}

/** For use of KageAsset when User double-clicked Asset
 * \sa renameAsset()
 */
void KageAssetManager::renameAsset(KageAsset *p_asset) {
	setSelected(p_asset);
	
	LabelRenameDialog* pDialog = new LabelRenameDialog(*_kage, p_asset->getLabel());
		pDialog->run();
	p_asset->setLabel(pDialog->getLabel());
	delete pDialog;
}

/** For use of Kage when User clicked Asset->Rename
 * \sa renameAsset(KageAsset)
 */
void KageAssetManager::renameAsset() {
	KageAsset *l_asset = NULL;
	if (_currentAssetIndex < assetCount() && assets[_currentAssetIndex]->assetID == _currentAssetID) {
		l_asset = assets[_currentAssetIndex];
	} else {
		for (unsigned int i = 0; i < assetCount(); ++i) {
			if (assets[i]->assetID == _currentAssetID) {
				_currentAssetIndex = i;
				l_asset = assets[i];
				break;
			}
		}
	}
	
	if (l_asset) {
		LabelRenameDialog* pDialog = new LabelRenameDialog(*_kage, l_asset->getLabel());
			pDialog->run();
		l_asset->setLabel(pDialog->getLabel());
		delete pDialog;
	}
}
/**
 * @brief Sets the label of current active Asset
 * 
 * @param p_label the new label for the active Asset
 */
void KageAssetManager::setLabel(string p_label) {
	if (_currentAssetIndex < assetCount() && assets[_currentAssetIndex]->assetID == _currentAssetID) {
		assets[_currentAssetIndex]->setLabel(p_label);
	} else {
		for (unsigned int i = 0; i < assetCount(); ++i) {
			if (assets[i]->assetID == _currentAssetID) {
				_currentAssetIndex = i;
				assets[i]->setLabel(p_label);
				break;
			}
		}
	}
}

string KageAssetManager::getLabel() {
	if (_currentAssetIndex < assetCount() && assets[_currentAssetIndex]->assetID == _currentAssetID) {
		return assets[_currentAssetIndex]->getLabel();
	} else {
		for (unsigned int i = 0; i < assetCount(); ++i) {
			if (assets[i]->assetID == _currentAssetID) {
				return assets[i]->getLabel();
			}
		}
	}
	
	return "(blank)";
}

/**
 * NOTE: assets are organized as index 0 as BOTTOM and last index is TOP
 * \sa moveDown() moveToBottom() moveUp()
 * \return True if successfully moved to bottom
 */
bool KageAssetManager::moveToTop() {
	bool l_return = false;
	if (_currentAssetIndex < assetCount() && assets[_currentAssetIndex]->assetID == _currentAssetID) {
		while (_currentAssetIndex < assetCount()-1) {
			l_return = moveUp();
		}
		return l_return;
	} else {
		for (unsigned int i = 0; i < assetCount(); ++i) {
			if (assets[i]->assetID == _currentAssetID) {
				_currentAssetIndex = i;
				while (_currentAssetIndex < assetCount()-1) {
					l_return = moveUp();
				}
				return l_return;
			}
		}
	}
	return false;
}
/**
 * NOTE: assets are organized as index 0 as BOTTOM and last index is TOP
 * \sa moveDown() moveToBottom() moveToTop()
 * \return True if successfully moved to bottom
 */
bool KageAssetManager::moveUp() {
	if (_currentAssetIndex < assetCount() && assets[_currentAssetIndex]->assetID == _currentAssetID) {
		if (_currentAssetIndex < assetCount()-1) {
			reorder_child(*assets[_currentAssetIndex], _currentAssetIndex+1);
			swap(assets[_currentAssetIndex], assets[_currentAssetIndex+1]);
			_currentAssetIndex = _currentAssetIndex+1;
			return true;
		}
	} else {
		for (unsigned int i = 0; i < assetCount(); ++i) {
			if (assets[i]->assetID == _currentAssetID) {
				_currentAssetIndex = i;
				if (_currentAssetIndex < assetCount()-1) {
					reorder_child(*assets[_currentAssetIndex], _currentAssetIndex+1);
					swap(assets[_currentAssetIndex], assets[_currentAssetIndex+1]);
					_currentAssetIndex = _currentAssetIndex+1;
					return true;
				}
				break;
			}
		}
	}
	return false;
}
/**
 * NOTE: assets are organized as index 0 as BOTTOM and last index is TOP
 * \sa moveToBottom() moveToTop() moveUp()
 * \return True if successfully moved to bottom
 */
bool KageAssetManager::moveDown() {
	if (_currentAssetIndex < assetCount() && assets[_currentAssetIndex]->assetID == _currentAssetID) {
		if (_currentAssetIndex > 0) {
			reorder_child(*assets[_currentAssetIndex], _currentAssetIndex-1);
			swap(assets[_currentAssetIndex], assets[_currentAssetIndex-1]);
			--_currentAssetIndex;
			return true;
		}
	} else {
		for (unsigned int i = 0; i < assetCount(); ++i) {
			if (assets[i]->assetID == _currentAssetID) {
				_currentAssetIndex = i;
				if (_currentAssetIndex > 0) {
					reorder_child(*assets[_currentAssetIndex], _currentAssetIndex-1);
					swap(assets[_currentAssetIndex], assets[_currentAssetIndex-1]);
					--_currentAssetIndex;
					return true;
				}
				break;
			}
		}
	}
	return false;
}
/**
 * NOTE: assets are organized as index 0 as BOTTOM and last index is TOP
 * \sa moveDown() moveToTop() moveUp()
 * \return True if successfully moved to bottom
 */
bool KageAssetManager::moveToBottom() {
	bool l_return = false;
	if (_currentAssetIndex < assetCount() && assets[_currentAssetIndex]->assetID == _currentAssetID) {
		while (_currentAssetIndex > 0) {
			l_return = moveDown();
		}
		return l_return;
	} else {
		for (unsigned int i = 0; i < assetCount(); ++i) {
			if (assets[i]->assetID == _currentAssetID) {
				_currentAssetIndex = i;
				while (_currentAssetIndex > 0) {
					l_return = moveDown();
				}
				return l_return;
			}
		}
	}
	return false;
}

/**
 * Checks all Asset if already imported
 * @param p_filePath Asset's full path1
 * \return true if p_filePath is in AssetManager. Otherwise, false.
 */
bool KageAssetManager::exists(string p_filePath) {
	bool l_return = false;
	unsigned int l_assetCount = assetCount();
	string l_assetHash = StringHelper::kHash(p_filePath, 24);
	std::cout << "l_assetHash " << l_assetHash << std::endl;
	for (unsigned int i = 0; i < l_assetCount; ++i) {
		std::cout << " " << assets[i]->getAssetHash() << " == " << l_assetHash << std::endl;
		if (assets[i]->getAssetHash() == l_assetHash) {
			return true;
		}
	}
	return false;
}

/**
 * Set's an Asset's hash
 * @param p_assetHash
 */
void KageAssetManager::setAssetHash(string p_assetHash) {
	if (_currentAssetIndex < assetCount() && assets[_currentAssetIndex]->assetID == _currentAssetID) {
		assets[_currentAssetIndex]->setAssetHash(p_assetHash);
	} else {
		for (unsigned int i = 0; i < assetCount(); ++i) {
			if (assets[i]->assetID == _currentAssetID) {
				_currentAssetIndex = i;
				assets[_currentAssetIndex]->setAssetHash(p_assetHash);
			}
		}
	}
}

string KageAssetManager::getImagePathByID(unsigned int p_index) {
	std::cout << "p_index " << p_index << std::endl;
	filesystem::path path_tempPath = filesystem::temp_directory_path();
//	stringstream stream_tempPath;
//	stream_tempPath << path_tempPath;
//	string l_tempPath;
//	stream_tempPath >> l_tempPath;
	string l_tempPath { path_tempPath.u8string() };
	std::cout << "      ? " << l_tempPath << _KageStudioAsset << "\\" << assets[p_index]->getAssetHash() << std::endl;
	if (p_index < assetCount() && assets[p_index]->getAssetHash() != "") {
	#ifdef __linux__
		return _KageStudioAsset + "/" + assets[p_index]->getAssetHash();
	#elif _WIN32
		return l_tempPath + _KageStudioAsset + "\\" + assets[p_index]->getAssetHash();
	#elif __APPLE__
		#include <TargetConditionals.h>
		#if TARGET_OS_MAC
			return _KageStudioAsset + "/" + assets[p_index]->getAssetHash();
		#endif
	#endif
	} else {
		std::cout << "\n\n\ngetImagePathByID FAIL!!!\n\n\n" << std::endl;
		return "";
	}
}

/**
 * For use by Stage when rendering an Asset, it needs the image path (why not the loaded image object?)
 * @param p_id
 */
string KageAssetManager::getImageByID(unsigned int p_id) {
	if (_currentAssetIndex < assetCount() && assets[_currentAssetIndex]->assetID == p_id) {
		return getImagePathByID(_currentAssetIndex);
	} else {
		for (unsigned int i = 0; i < assetCount(); ++i) {
			if (assets[i]->assetID == _currentAssetID) {
				_currentAssetIndex = i;
				return getImagePathByID(_currentAssetIndex);
			}
		}
	}
	return "";
}

/**
 * @brief For use by Kage when Opening
 * 
 * @param p_index 
 * @param p_filepath 
 * @return true if asset index exists
 * @return false 
 */
bool KageAssetManager::setFilePathAt(unsigned int p_index, string p_filePath) {
	if (p_index < assetCount() && assets[p_index]->getAssetHash() != "") {
		assets[p_index]->setFilePath(p_filePath);
		return true;
	}
	return false;
}
/**
 * For use by saveAssetsTo when Exporting; returns path like /home/creek23/assets/
 * @param p_index
 */
string KageAssetManager::getFilePathAt(unsigned int p_index) {
	if (p_index < assetCount() && assets[p_index]->getAssetHash() != "") {
		return assets[p_index]->getFilePath();
	}
	return "";
}
/**
 * @brief For use by Kage when Opening
 * 
 * @param p_index 
 * @param p_fileName 
 * @return true if asset index exists
 * @return false 
 */
bool KageAssetManager::setFileNameAt(unsigned int p_index, string p_fileName) {
	if (p_index < assetCount() && assets[p_index]->getAssetHash() != "") {
		assets[p_index]->setFileName(p_fileName);
		return true;
	}
	return false;
}
/**
 * For use by saveAssetsTo when Exporting; returns path like image.png
 * @param p_index
 */
string KageAssetManager::getFileNameAt(unsigned int p_index) {
	if (p_index < assetCount() && assets[p_index]->getAssetHash() != "") {
		return assets[p_index]->getFileName();
	}
	return "";
}
/**
 * For use by saveAssetsTo when Exporting; returns label as scene on AssetManager
 * @param p_index
 */
string KageAssetManager::getLabelAt(unsigned int p_index) {
	if (p_index < assetCount() && assets[p_index]->getAssetHash() != "") {
		return assets[p_index]->getLabel();
	}
	return "";
}

bool KageAssetManager::setHashAt(unsigned int p_index, string p_hash) {
	if (p_index < assetCount() && assets[p_index]->getAssetHash() != "") {
		assets[p_index]->setAssetHash(p_hash);
		return true;
	}
	return false;
}
/**
 * For use by saveAssetsTo when Exporting; returns hash of '/home/creek23/assets/image.png'
 * @param p_index
 */
string KageAssetManager::getHashAt(unsigned int p_index) {
	if (p_index < assetCount() && assets[p_index]->getAssetHash() != "") {
		return assets[p_index]->getAssetHash();
	}
	return "";
}

/**
 * For use by Kage when Exporting; returns XML string as part of .kage file
 * @param p_path
 */
string KageAssetManager::saveAssetsTo(string p_path) {
	unsigned int l_assetMax = assets.size();
	string l_assetXML = "";
	string l_hash = "";
	for (unsigned int i = 0; i < l_assetMax; ++i) {
		//ASSET_KAGE are KSF files considered as Scenes
		if (assets[i]->getAssetType() != KageAsset::AssetType::ASSET_KAGE) {
			l_hash = getHashAt(i);

			filesystem::path l_sourceFile = filesystem::temp_directory_path() / _KageStudioAsset / l_hash;
			filesystem::path l_assetFile = filesystem::path(p_path) / l_hash;
				string l_filenames = l_sourceFile.u8string() + " " + l_assetFile.u8string();
				
				//filesystem::copy(l_sourceFile, l_assetFile, filesystem::copy_options::overwrite_existing);
				//as above code is not working with GCC 13
				if (filesystem::exists(l_assetFile)) {
					filesystem::remove_all(l_assetFile);
				}
				std::ifstream  src(l_sourceFile.u8string(), std::ios::binary);
				std::ofstream  dst( l_assetFile.u8string(), std::ios::binary);
					dst << src.rdbuf();
			
			l_assetXML += ("<asset label=\"" + getLabelAt(i) + "\" hash=\"" + l_hash + "\">" + getFilePathAt(i) + getFileNameAt(i) + "</asset>\n");
		}
	}
	return l_assetXML;
}