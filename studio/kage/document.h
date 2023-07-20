/*
 * Kage Studio - a simple free and open source vector-based 2D animation software
 * Copyright (C) 2011~2023  Mj Mendoza IV
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
/*Project
	Info
		Name
		Width
		Height
		BG Color
	Assets
		asset01 Image
		asset02 Image
		asset03 SceneID
	Scene01
		layer01
			frame01
				data Move X Y
				data Line X Y
				data Color
			frame02
				data Move X Y
				data Line X Y
				data Asset ID
		layer02
			...
	Scene02
		...
	Scene03
		layer01
			frame01
				data Move X Y
				data Line X Y
				data Color
			frame02
				...
		layer02
			...
		layer03
			...
*/
#ifndef GTKMM_KAGE_DOCUMENT_H
	#define GTKMM_KAGE_DOCUMENT_H
	
	#include <vector>
	#include "data/color.h"
	#include "./data/scene.h"
	
	#include "../util/string/stringhelper.h"
	
	#include <gdkmm/color.h>
	class Kage; //forward declaration
	
	using namespace std;
	class KageProject {
		public:
			string _name;
			double _width; //made double for zoom
			double _height; //ditto
			ColorData _backgroundColor;
			unsigned int _fps;
			
			KageProject();
			virtual ~KageProject();
			string toString();
	};

	class KageDocument {
		protected:
			bool _saved;
			unsigned int _activeSceneID;
		public:
			KageProject Project;
			Kage *_kage;

			KageDocument(Kage *p_kage);
			KageDocument(Kage *p_kage, KageProject p_project);
			void init(Kage *p_kage);
			virtual ~KageDocument();
			void setProjectInformation(KageProject p_project);
			unsigned int openScene(string p_filepath);
			bool saveScene(unsigned int p_sceneID);
			bool openProject();
			bool saveProject();
			bool isSaved();
			
			enum AssetType {
				ASSET_IMAGE,
				ASSET_VIDEO,
				ASSET_KAGE
			};
			
			struct Asset {
				unsigned int ID;
				AssetType Type;
				string Path;
				string Name;
			};
			//TODO: each instance of asset will reference to this
			vector<Asset> Assets;
			
			//TODO: each Scene will be saved as separate KSF file
			vector<KageScene> Scenes;
			static unsigned int ACTIVE_SCENE;

			unsigned int getActiveSceneID();
			void setActiveSceneID(unsigned int p_activeSceneID);
			unsigned int getActiveLayerID();
			unsigned int getActiveFrameID();

			unsigned int frameCount();
			void setCurrentFrame(unsigned int p_frame);
	};
#endif //GTKMM_KAGE_DOCUMENT_H
