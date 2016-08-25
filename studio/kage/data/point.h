
#ifndef GTKMM_KAGE_DATA_POINT_H
	#define GTKMM_KAGE_DATA_POINT_H
	
	class PointData {
		public:
			PointData();
			PointData(Gdk::Point p);
			PointData(double p_x, double p_y);
			virtual ~PointData();
			
			double x;
			double y;
		protected:
	};
#endif //GTKMM_KAGE_DATA_POINT_H