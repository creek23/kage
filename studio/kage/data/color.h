
#ifndef GTKMM_KAGE_DATA_COLOR_H
	#define GTKMM_KAGE_DATA_COLOR_H
	
	#include <string>
	
	class ColorData {
		public:
			ColorData();
			ColorData(unsigned int p_r, unsigned int p_g, unsigned int p_b);
			ColorData(unsigned int p_r, unsigned int p_g, unsigned int p_b, unsigned int p_a);
			virtual ~ColorData();
			
			void setR(unsigned int p_r);
			void setG(unsigned int p_g);
			void setB(unsigned int p_b);
			void setA(unsigned int p_a);
			unsigned int getR();
			unsigned int getG();
			unsigned int getB();
			unsigned int getA();
			ColorData clone();
			std::string toString();
			void copy(ColorData p_color);
			bool equalTo(ColorData p_color);
		protected:
			unsigned int R;
			unsigned int G;
			unsigned int B;
			unsigned int A;
	};
#endif //GTKMM_KAGE_DATA_COLOR_H
