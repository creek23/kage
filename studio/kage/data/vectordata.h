
#ifndef GTKMM_KAGE_DATA_VECTOR_H
	#define GTKMM_KAGE_DATA_VECTOR_H
	
	#include <vector>
	#include "color.h"
	#include "point.h"
	#include "strokecolor.h"
	
	using namespace std;
	
	class VectorData {
		public:
			enum type {
				TYPE_INIT,            //holds initial fill/stroke; no vectors to use
				TYPE_FILL,            //holds fillColor; no vectors to use
				TYPE_CLOSE_PATH,      //no vectors to use
				TYPE_ENDFILL,         //holds transparent fillColor; no vectors to use
				TYPE_STROKE,          //holds strokeColor
				TYPE_MOVE,            //moves p0 to p1; p1 is to be stored
				TYPE_LINE,            //draws a line from p0 to p1
				TYPE_CURVE_QUADRATIC, //draws a curve from p0 with kink to p1 before ending at p2 then; p1 and p2 is to be stored
				TYPE_CURVE_CUBIC,     //draws a curve from p0 with kink to p1 before ending at p2 and another from p2 with kink to p3 then ends at p4
				TYPE_TEXT,            //TODO: handle string content
				TYPE_IMAGE            //TODO: think of how to handle blits from imageB to imageA
			};
			VectorData();
			VectorData(type p_type);
			virtual ~VectorData();
			
			void setType(type p_type);
			type getType() const;
			void setPoints(vector<PointData> p_points);
			vector<PointData> getPoints();
			StrokeColorData stroke;
			ColorData fillColor;
			unsigned int count;
			
			type vectorType;
			vector<PointData> points;
			
			VectorData clone();
		protected:
			
			long lImg;  //index reference to array of typeImage
	};
#endif //GTKMM_KAGE_DATA_VECTOR_H
