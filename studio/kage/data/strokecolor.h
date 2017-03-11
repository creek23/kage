
#ifndef GTKMM_KAGE_DATA_STROKECOLOR_H
	#define GTKMM_KAGE_DATA_STROKECOLOR_H
	
	#include "color.h"

	class StrokeColorData: public ColorData {
		public:
			StrokeColorData();
			StrokeColorData(unsigned int p_r, unsigned int p_g, unsigned int p_b);
			StrokeColorData(unsigned int p_r, unsigned int p_g, unsigned int p_b, unsigned int p_a);
			StrokeColorData(double p_thickness);
			virtual ~StrokeColorData();
			
			//ColorData color;
			void setThickness(double p_thickness);
			double getThickness();
			StrokeColorData clone();
			std::string toString() const;
		protected:
			double thickness;
	};
#endif //GTKMM_KAGE_DATA_STROKECOLOR_H
