#ifndef SICKCOLOR_H
#define SICKCOLOR_H

class SickColor
{
public:
	
	SickColor();
	SickColor(float comps);
	SickColor(float r, float g, float b);
	SickColor(float r, float g, float b, float a);
	
	// basic accessors
	float r();
	float g();
	float b();
	float a();

	// hsv methods
	float hue();
	float saturation();
	float value();
	void hsv(float& h, float& s, float& v);

	float luminance();
	float length();
	float distance(SickColor other_color);

		
private:	
	float myR;
	float myG;
	float myB;
	float myA;

};

#endif
