
#ifndef GTKMM_KAGE_DATA_POINT_H
	#define GTKMM_KAGE_DATA_POINT_H
	
	#include <gdk/gdk.h>
	
	class PointData {
		public:
			PointData();
			PointData(GdkPoint p);
			PointData(double p_x, double p_y);
			virtual ~PointData();
			
			PointData clone();
			const PointData& operator=( const PointData& p_origin );
			
			double x;
			double y;
			unsigned int debug_id;
			static unsigned int debug_pts;
		protected:
	};
#endif //GTKMM_KAGE_DATA_POINT_H
