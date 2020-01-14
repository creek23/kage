
#ifndef GTKMM_KAGE_DATA_ANCHOR_H
	#define GTKMM_KAGE_DATA_ANCHOR_H
	
	#include <gdk/gdk.h>
	#include "point.h"
	
	class AnchorData: public PointData {
		public:
			enum type {
				TYPE_NONE,
				TYPE_NORTH,
				TYPE_EAST,
				TYPE_WEST,
				TYPE_SOUTH,
				TYPE_NORTH_EAST,
				TYPE_NORTH_WEST,
				TYPE_SOUTH_EAST,
				TYPE_SOUTH_WEST,
				TYPE_MOVE,
				TYPE_ROTATE
			};
			AnchorData();
			AnchorData(GdkPoint p);
			AnchorData(double p_x, double p_y);
			virtual ~AnchorData();
			
			AnchorData clone();
			//const AnchorData& operator=( const AnchorData& p_origin );
			
			unsigned int rotation;
		protected:
	};
#endif //GTKMM_KAGE_DATA_POINT_H
