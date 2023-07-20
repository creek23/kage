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

#include "assetmanager.h"
#include "../../kage.h"

KageAssetManager::KageAssetManager(Kage *p_kage) {
	_kage = p_kage;
	assetCtr = 0;

	_KageStudioAsset = StringHelper::kHash("Kage Studio Asset");

	assets.clear();
}

KageAssetManager::~KageAssetManager() {
	assets.clear();
}

#include <filesystem>

unsigned int KageAssetManager::addAsset(Glib::ustring p_name) {
	string l_filePath;
	string l_fileName;
	
	/**
	 * TODO: 
	 * 1. [DONE] check p_name exists -- done in Kage with AssetManager
	 * 2. if project is saved, copy file to project path
	 *    [DONE] else, copy file to temporary
	 * 3. [DONE] create hash for path and filesize for future check by assetManager if already existing in Library
	 *    use kHash from Quixie
	 */
	
	string l_hash = StringHelper::kHash(p_name);
	
	if (p_name == "") {
		//p_name = Glib::ustring::compose("Asset %1", assetCtr);
		cout << "NOTE: creating Kage Asset will have no existing file unless User save the Project";
		//TODO: make this create Asset for KSF
		return UINT_MAX;
	} else {
		/**
		 * TODO:
		 * 1. [DONE] check if file p_name exists
		 * 2. [DONE] if existing, copy to temp path
		 * 3. [DONE] load copied file -- use original path as hash
		 * 4. select image on stage
		 * 5. resize image on stage
		 * 6. delete image on stage
		 * 7. add image on stage FROM LIBRARY (double click)
		 * 8. add image on stage FROM ASSET (double click)
		 * 9. add image on stage FROM LIBRARY (drag-n-drop)
		 * 10. add image on stage FROM ASSET (drag-n-dropl)
		 */
		
		string l_name = p_name;
		std::filesystem::path l_sourceFile = l_name;
		if (std::filesystem::exists(l_sourceFile) == true) {
			std::size_t found = p_name.find_last_of("/\\");
			l_filePath = p_name.substr(0,found);
			l_fileName = p_name.substr(found+1);

			std::filesystem::path l_currentPath = std::filesystem::current_path();
			cout << "Current path is " << l_currentPath << endl;
			std::filesystem::current_path(std::filesystem::temp_directory_path());
			cout << "    New path is " << std::filesystem::current_path() << endl;
			
			std::filesystem::create_directories(_KageStudioAsset);
			
			//std::ofstream("sandbox/file1.txt").put('a');
			//std::filesystem::copy("sandbox/file1.txt", "sandbox/file2.txt"); // copy file
			//const auto copyOptions = std::filesystem::copy_options::update_existing;
								//| std::filesystem::copy_options::recursive; <-- use in future when importing project file which will import assets
			std::filesystem::path l_assetFile = std::filesystem::temp_directory_path() / _KageStudioAsset / l_hash;
			if (std::filesystem::exists(l_assetFile) == true) {
				//delete l_assetFile
				if (std::filesystem::remove_all(l_assetFile) == false) {
					return UINT_MAX;
				}
			}
			
			std::filesystem::copy(l_sourceFile, l_assetFile, std::filesystem::copy_options::update_existing);
			
			static_cast<void>(std::system("tree"));
			std::filesystem::current_path(l_currentPath);

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
			cout << " deleting; index " << _currentAssetIndex << endl;
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
			cout << " asset not selected" << endl;
		}
	} else {
		cout << " unknown asset; identifying..." << endl;
		for (unsigned int i = 0; i < assetCount(); ++i) {
			if (assets[i]->assetID == _currentAssetID) {
				if (assets[i]->isSelected()) {
					cout << " deleting; index " << i << endl;
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
	return KageAsset::AssetType::ASSET_IMAGE; //prone to bug; return ASSET_NULL instead
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
				_currentAssetIndex = i;
				assets[i]->setSelected(false);
				break;
			}
		}
	}
	p_asset->setSelected(true);
	_currentAssetID = p_asset->assetID;
	
//	_kage->setCurrentFrame(_kage->getCurrentFrame());
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
	cout << " KageAssetManager::renderStage <" << endl;
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
	
	AssetRenameDialog* pDialog = new AssetRenameDialog(*_kage, p_asset);
		pDialog->run();
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
		AssetRenameDialog* pDialog = new AssetRenameDialog(*_kage, l_asset);
			pDialog->run();
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
	string l_assetHash = StringHelper::kHash(p_filePath);
	for (unsigned int i = 0; i < assetCount(); ++i) {
		cout << " " << assets[i]->getAssetHash() << " == " << l_assetHash << endl;
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
	cout << "p_index " << p_index << endl;
	std::filesystem::path path_tempPath = std::filesystem::temp_directory_path();
//	stringstream stream_tempPath;
//	stream_tempPath << path_tempPath;
//	string l_tempPath;
//	stream_tempPath >> l_tempPath;
	string l_tempPath { path_tempPath.u8string() };
	cout << "      ? " << l_tempPath << _KageStudioAsset << "\\" << assets[p_index]->getAssetHash() << endl;
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
		cout << "\n\n\ngetImagePathByID FAIL!!!\n\n\n" << endl;
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

