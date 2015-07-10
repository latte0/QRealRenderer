#ifndef COLORMODULATION
#define COLORMODULATION

float star_modulation1st[16];
float star_modulation2nd[16];
float star_modulation3rd[16];

float filmic_ghost_modulation1st[16];
float filmic_ghost_modulation2nd[16];
float camera_ghost_modulation1st[16];
float camera_ghost_modulation2nd[16];

float hori_modulation1st[16];
float hori_modulation2nd[16];
float hori_modulation3rd[16];


void ColorModulationRedShift(float* color, float r, float g, float b, int num)
{
    color[4*num] = color[4*num]*r;
    color[4*num+1] = color[4*num]*g;
    color[4*num+2] = color[4*num]*b;
    //color[4*num+3] = color[4*num]*a;
}

void ColorModulation(float* color, float r, float g, float b, int num)
{
    color[4*num] = color[4*num+0]*r;
    color[4*num+1] = color[4*num+1]*g;
    color[4*num+2] = color[4*num+2]*b;
    //color[4*num+3] = color[4*num]*a;
}


void modulateColor()
{
    //color modulation coefficients for star streak & ghost image
    for (int i=0;i<16; i++) {
       star_modulation1st[i]=0.25;
       star_modulation2nd[i]=0.25;
       star_modulation3rd[i]=0.25;
       hori_modulation1st[i]=0.5;
       hori_modulation2nd[i]=0.5;
       hori_modulation3rd[i]=0.5;
       filmic_ghost_modulation1st[i] = 1.0;
       filmic_ghost_modulation2nd[i] = 1.0;
       camera_ghost_modulation1st[i] = 1.0;
       camera_ghost_modulation2nd[i] = 1.0;
    }
    //star
    ColorModulationRedShift(star_modulation1st, 1.0, 0.95, 0.9,0);
    ColorModulationRedShift(star_modulation1st, 0.8, 1.0, 0.9,1);
    ColorModulationRedShift(star_modulation1st, 0.9, 0.9, 1.0,2);
    ColorModulationRedShift(star_modulation1st, 0.9, 1.0, 0.9,3);

    ColorModulationRedShift(star_modulation2nd, 1.0, 0.9, 0.8,0);
    ColorModulationRedShift(star_modulation2nd, 1.0, 0.6, 0.5,1);
    ColorModulationRedShift(star_modulation2nd, 0.5, 1.0, 0.6,2);
    ColorModulationRedShift(star_modulation2nd, 0.6, 0.4, 1.0,3);

    ColorModulationRedShift(star_modulation3rd, 1.0, 0.6, 0.6,1);
    ColorModulationRedShift(star_modulation3rd, 0.6, 1.0, 0.6,2);
    ColorModulationRedShift(star_modulation3rd, 0.6, 0.6, 1.0,3);

#define BLUE_SHIFT0 1.0, 1.0, 1.0
#define BLUE_SHIFT1 0.2, 0.3, 0.95
#define BLUE_SHIFT2 0.1, 0.2, 0.9
#define BLUE_SHIFT3 0.02, 0.1, 0.99

    //horizontal
    ColorModulation(hori_modulation1st, BLUE_SHIFT1,0);
    ColorModulation(hori_modulation1st, BLUE_SHIFT1,1);
    ColorModulation(hori_modulation1st, BLUE_SHIFT2,2);
    ColorModulation(hori_modulation1st, BLUE_SHIFT1,3);

    ColorModulation(hori_modulation2nd, BLUE_SHIFT1,0);
    ColorModulation(hori_modulation2nd, BLUE_SHIFT2,1);
    ColorModulation(hori_modulation2nd, BLUE_SHIFT3,2);
    ColorModulation(hori_modulation2nd, BLUE_SHIFT3,3);

    ColorModulation(hori_modulation3rd, BLUE_SHIFT0,0);
    ColorModulation(hori_modulation3rd, BLUE_SHIFT0,1);
    ColorModulation(hori_modulation3rd, BLUE_SHIFT0,2);
    ColorModulation(hori_modulation3rd, BLUE_SHIFT0,3);

    //ghost camera
    ColorModulationRedShift(camera_ghost_modulation1st, 1.0, 0.9, 0.8,0);
    ColorModulationRedShift(camera_ghost_modulation1st, 1.0, 0.6, 0.5,1);
    ColorModulationRedShift(camera_ghost_modulation1st, 0.5, 1.0, 0.6,2);
    ColorModulationRedShift(camera_ghost_modulation1st, 1.0, 0.7, 0.3,3);

    ColorModulationRedShift(camera_ghost_modulation2nd, 0.2, 0.3, 0.7,0);
    ColorModulationRedShift(camera_ghost_modulation2nd, 0.5, 0.3, 0.2,1);
    ColorModulationRedShift(camera_ghost_modulation2nd, 0.1, 0.5, 0.2,2);
    ColorModulationRedShift(camera_ghost_modulation2nd, 0.1, 0.1, 1.0,3);

    //ghost filmic
    ColorModulation(filmic_ghost_modulation1st, 0.1, 0.1, 1.0,0);
    ColorModulation(filmic_ghost_modulation1st, 0.2, 0.3, 1.0,1);
    ColorModulation(filmic_ghost_modulation1st, 0.1, 0.2, 0.6,2);
    ColorModulation(filmic_ghost_modulation1st, 0.6, 0.3, 1.0,3);

    ColorModulation(filmic_ghost_modulation2nd, 0.6, 0.2, 0.2,0);
    ColorModulation(filmic_ghost_modulation2nd, 0.2, 0.06, 0.6,1);
    ColorModulation(filmic_ghost_modulation2nd, 0.15, 0.00, 0.1,2);
    ColorModulation(filmic_ghost_modulation2nd, 0.06, 0.00, 0.55,3);
}


#endif // COLORMODULATION

