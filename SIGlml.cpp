/*-----------------------------------------------------------------------

  File        : SIGlml.cpp

  Description : Synthetic particle Image Generator program of the Laboratory of Mechanic
s in Lille (LML)

  Author      : Adrien TESSIER, Sebastien COUDERT
  
  --------------------------------------------------------------------*/

/*Additional documentation for the generation of the reference page (using doxyg
en)*/
/** \mainpage
 *
 *  index:
 *  \li \ref sectionCommandLine
 *  \li \ref sectionSIGlmlDocumentation
 *  \li \ref sectionDoxygenSyntax
 *
 *  \section sectionCommandLine command line options
 *  \verbinclude "SIGlml.help.output"
 *
 *  \section sectionSIGlmlDocumentation documentation outline
 *  This is the reference documentation of <a href="http://www.meol.cnrs.fr/">PI
Vlml</a>, from the <a href="http://www.univ-lille1.fr/lml/">LML</a>.\n\n
 *  PIVlml is a PIV image processing software. The program begins in the main fu
nction in the <a href="_p_i_vlml_8cpp.html#a1">PIVlml.cpp</a> source file.\n\n
 *  This documentation has been automatically generated from the PIVlml sources,
 
 *  using the tool <a href="http://www.doxygen.org">doxygen</a>. It should be re
aded as HTML, LaTex and man page.\n
 *  It contains both
 *  \li a detailed description of all classes and functions
 *  \li a user guide (cf. \ref pages.html "related pages")
 *
 *  that as been documented in the sources.
 *
 *  \par Additional needed libraries:
 *
 *  \li <a href="http://cimg.sourceforge.net">the CImg Library</a> using <a href
="http://www.imagemagick.org/">ImageMagick</a> for a few image format
 *
 *
 *  \section sectionDoxygenSyntax make documentation using Doxygen syntax
 *  Each function in the source code should be commented using \b doxygen \b syn
tax in the same file.
 *  The documentation need to be written before the function.
 *  The basic syntax is presented in this part.
 *  \verbinclude "doxygen.example1.txt"
 *
 *  Two kind of comments are needed for \b declaration and \b explanation \b par
ts of the function:
 *  Standart documentation should the following (\b sample of code documentation
):
 *  \verbinclude "doxygen.example2.txt"
 *
 *  In both declaration and explanation part, \b writting and \b highlithing syn
tax can be the following:\n\n
 *  \li \c \\code to get\n
 *
 *  \li \c \\n    a new line
 *  \li \c \\li   a list (dot list)
 *
 *  \li \c \\b    bold style
 *  \li \c \\c    code style
 *  \li \c \\e    enhanced style (italic)
 *
 *  For making \b shortcut please use:\n
 *  \li \c \\see to make a shortcut to a related function or variable
 *  \li \c \\link to make a shortcut to a file or a function
 *  \note this keyword needs to be closed using \c \\end*
 *
 *  \li \c \\todo to add a thing to do in the list of <a href="./todo.html">oDo</
a> for the whole program
 *
 *  In explanation part, \b paragraph style can be the following:\n
 *  \li \c \\code for an example of the function use
 *  \li \c \\note to add a few notes
 *  \li \c \\attention for SOMETHING NOT FULLY DEFINED YET
 *  \li \c \\warning to give a few warning on the function
 *  \note these keywords need to be closed using \c \\end*
 *
 *  \verbinclude "doxygen.example3.txt"
 *
 *  Many other keywords are defined, so please read the documentation of <a href
="http://www.doxygen.org/commands.html">doxygen</a>.
 *
**/

#include<iostream>

//debug display (0- quiet, 1- only final image, 2- image step by step)
//#define cimg_debug 0
//#define cimg_debug 1
#define cimg_debug 2
#define cimg_plugin "../CImg.PlugIn/PlugIn.CImg.sprite_multi_layer.h"

#include "../CImg/CImg.h"
using namespace cimg_library;
using namespace std;

//The lines below are necessary when using a non-standard compiler as visualcpp6.
#ifdef cimg_use_visualcpp6
#define std
#endif
#ifdef min
#undef min
#undef max
#endif
//end visualcpp6

//create random position and size particles
template<typename T> int create_particles(int &n,CImg<T> &particles,/*gaussian particles: position x,y; size; level*/
  T x_min,T x_max,T y_min,T y_max,
  T sigma_min,T sigma_max,
  T level_min,T level_max)
{
  //parameter arrays
  CImg<T> coord_x(n),coord_y(n),psigma(n),color(n);// matrices de coordonnées des points plus sigma et color
//create random particle parameters: 2D gaussian function
  ///2D position of the maximum (i.e. particle position)
  coord_x.rand(x_min,x_max);coord_y.rand(y_min,y_max);
  ///sigma of the gaussian curve (i.e. particle size)
  psigma.rand(sigma_min,sigma_max); 
  ///maximum level (i.e. particle brightness)
  color.rand(level_min,level_max);
//set particle parameters in only one structure (i.e. one CImg object)
  particles.assign(n,1,1,4);
  particles.draw_image(0,0,0,0,coord_x);
  particles.draw_image(0,0,0,1,coord_y);
  particles.draw_image(0,0,0,2,psigma);
  particles.draw_image(0,0,0,3,color);
  return 0;
}

//read particle position from stdin
template<typename T> int get_particles(int &n,CImg<T> &particles,T sigma_min=1.5,T sigma_max=2.5,
  T level_min=200,T level_max=255)
{
  if(n<1) n=512;
  //x and y parameter arrays
  CImg<T> coord_x(n),coord_y(n);
//get particle parameters from stdin
  int p=-1;n--;
  T x,y;
  while(cin>>x>>y)
  {
//cout<<endl<<"("<<x<<","<<y<<")";
    p++;if(p>n) {cout<<endl<<"information: not enough space in the particle array (i.e. reallocating more space), set -n option to speed up execution (n should be higher than "<<n+1<<")."<<flush;n+=512;coord_x.resize(n);coord_y.resize(n);n--;}
    coord_x(p)=x;
    coord_y(p)=y;
  }
  if(p<0) return -1;
  //set number of particles
  n=p+1;
  //resize x and y arrays
  coord_x.crop(0,n-1);coord_y.crop(0,n-1);
  //others parameter arrays
  CImg<T> psigma(n),color(n);// matrices de coordonnées des points plus sigma et color
//create random particle parameters: 2D gaussian function
  ///sigma of the gaussian curve (i.e. particle size)
  psigma.rand(sigma_min,sigma_max);
  ///maximum level (i.e. particle brightness)
  color.rand(level_min,level_max);
//set particle parameters in only one structure (i.e. one CImg object)
  particles.assign(n,1,1,4);
  particles.draw_image(0,0,0,0,coord_x);
  particles.draw_image(0,0,0,1,coord_y);
  particles.draw_image(0,0,0,2,psigma);
  particles.draw_image(0,0,0,3,color);
  return 0;
}

//! add particles within the image
/* create grey level particles and add it within the image.
 * gaussian particle parameters: position x and y, sigma and maximum
 * (random level for parameters)
 * \param[in/out] ima: particle image
**/
template<typename T, typename t> int add_particles(CImg<T> &ima, CImg<t> &particles,bool test_non_hiden_particles_center,CImg<t> &non_hiden_particles/*gaussian particles: position x,y; size; level*/)
{
  int sigma_max=(int)particles.get_channel(2).max();
  int i=0;
  CImg<T> tmp(2*(int)(2*sigma_max),2*(int)(2*sigma_max));
non_hiden_particles.assign(1,1,1,4);
//draw gaussian particles on the image
 cimg_forX(particles,p)
  {
    const int x=(int)particles(p,0);
    const int y=(int)particles(p,1);
    const float sx=(float)particles(p,0)-(float)x;
    const float sy=(float)particles(p,1)-(float)y;
    const float q=(float)particles(p,2);
    const T w=(T)particles(p,3);
//ima((int)x,(int)y)=255;
 //create particle on a temporary image (i.e. single particle image)
    tmp.draw_gaussian((int)(2*sigma_max)+sx,(int)(2*sigma_max)+sy,q,&w); 
	tmp.mul(tmp.get_threshold(35,false,true));
// add particale to image
 // extraction of data on the non occluded center
if(test_non_hiden_particles_center)
	if(ima(x,y)==0) { 
	
	non_hiden_particles(i,0,0,0)=particles(p,0,0,0);
	non_hiden_particles(i,0,0,1)=particles(p,0,0,1);
	non_hiden_particles(i,0,0,2)=particles(p,0,0,2);
	non_hiden_particles(i,0,0,3)=particles(p,0,0,3);
	
	 i++;non_hiden_particles.resize( i+1,1,1,4,0,-1,false); 
	 
	 }  
	

    ima.add_sprite(tmp,x-(int)(2*sigma_max),y-(int)(2*sigma_max),0,0,2);

 
  }
 //ima.display("image ima");


return 0;
}


//! add random gray level noise
/* create grey level noise and add it within the image.
 * \param[in/out] ima: particle image
**/
template<typename T> int add_noise(CImg<T> &ima,int noise_min,int noise_max)
{
  CImg<unsigned char> noise(ima.dimx(),ima.dimy(),1,1,0);
  //create random gray level (i.e. noise)
  noise.rand(noise_min,noise_max);
  //add noise image to image
  ima+=noise;
  return 0;
}

//! add background illumination ramp
/* create linear grey level background and add it within the image.
 * ramp function: v=Ax+B
 * \param[in/out] ima: particle image
 * \param[in] ramp_const: B constant at x=0 of the ramp function
 * \param[in] ramp_slope: A ramp slope of the ramp function
**/
template<typename T> int add_ramp(CImg<T> &ima,float ramp_const,float ramp_slope)
{
  CImg<T> ramp(ima.dimx(),ima.dimy(),1,1,0);
  //create ramp image
  cimg_forXY(ramp,x,y){ramp(x,y)=(T)(ramp_slope*(float)x+ramp_const);} //rampe linéaire 
  //add ramp image to image
  ima+=ramp;
  return 0;
}

int main(int argc,char **argv)
{
  // Usage of the program displayed at the command line
  cimg_usage("Synthetic particle Image Generator of LML");
  const bool help = cimg_option("-h",false,"Display Help");
//particles
  ///particle parameter from stdin
  const char* createParticleType= cimg_option("-i","random","particle parameters source (cimg file or) can be set to: random (internal) or stdin (external) (e.g. echo 31.234 12.345 | ./SIGlml -W 64 -H 64 -i stdin)");
  ///particle number
  int nbParticles= cimg_option("-n",10,"number of particles");
  ///particle size
  float sigma_min= cimg_option("-s",1.0,"minimum value of sigma of gaussian function (i.e. minimum size)");
  float sigma_max= cimg_option("-S",2.0,"maximum value of sigma of gaussian function (i.e. maximum size)");
  ///particle grey level dynamics
  int level_min = cimg_option("-l",200,"minimum value of amplitude of the gaussian function (i.e. minimum brightness)");
  int level_max = cimg_option("-L",255,"maximum value of amplitude of the gaussian function (i.e. maximum brightness)");
//image
  ///image file name (output)
  const char* filename= cimg_option("-o","particles.png","output image file name");
  ///image grey level dynamics
  int cutoff= cimg_option("-C",255,"gray level cut off (255 for 8 bit, >255 for 16 bit)");
  ///image size
  int width = cimg_option("-W",512,"image width");
  int height= cimg_option("-H",512,"image height");
  ///image noise
  int noise_min = cimg_option("-m",10,"noise minimum level");
  int noise_max = cimg_option("-M",20,"noise maximum level");
//background
  float ramp_slope= cimg_option("-A",0.015,"A slope of background ramp (y=Ax+B)");
  float ramp_const= cimg_option("-B",5,"B constant of background ramp (y=Ax+B)");
// non hiden particles center
bool test_non_hiden_particles_center=cimg_option("-tnhc",false,"detecting the non hiden center of particles by occlusion( set to true if  there are need to detect the non hiden center otherwise set to false");

 //const char* file_non_hiden_particles_center=cimg_option("-nhc","non_hiden_center.cimg"," file name of the non  hiden particles center");
  if(help) return 0;
  
//create empty image
  CImg<int> ima(width,height,1,1,0);
//create or get particle parameters
  CImg<float> particles;
  if (!cimg::strcmp("random",createParticleType))
  {
    cout<<"\rinformation: create particles with random parameters."<<flush;
    create_particles(nbParticles,particles,
                     (float)0,(float)ima.dimx()-1,(float)0,(float)ima.dimy()-1,
                     (float)sigma_min,(float)sigma_max,
                     (float)level_min,(float)level_max);
  }
  else if (!cimg::strcmp("stdin",createParticleType))
  {
    cout<<"\rinformation: get particles from stdin."<<flush;
    get_particles(nbParticles,particles,(float)sigma_min,(float)sigma_max,(float)level_min,(float)level_max);
  }
  else
  {
    cout<<"\rinformation: get particles from file \""<<createParticleType<<"\"."<<flush;
    int error;
    if(!(error=particles.load(createParticleType))) return error;
    if(particles.dimv()<4) {cerr<<"\nerror: needs at least 4 parameters for a gaussian particle (file \""<<createParticleType<<"\" do NOT."<<flush;return 1;}
  }
//add particles within the image
  cout<<"\rinformation: create the image of "<<particles.dimx()<<" particle"<<((particles.dimx()>1)?"s":" ")<<".\n"<<flush;
  CImg<float> non_hiden_particles;

  add_particles(ima,particles,test_non_hiden_particles_center,non_hiden_particles); 
 
  if(test_non_hiden_particles_center) {
 	// creation of the name file containing the non hiden particles from the name of the file particles name 
  char file[100];strcpy(file,filename);sprintf(file,"..%s_non_hiden_particles.cimg",strtok(file,"."));
  
 non_hiden_particles.save(file);
  }

#if cimg_debug>1
 // ima.display("particles only SIGlml");
#endif
//add grey level noise
  add_noise(ima,noise_min,noise_max);
#if cimg_debug>1
 // ima.display("particles and noise SIGlml");
#endif
//add background illumination ramp
  add_ramp(ima,ramp_const,ramp_slope);
#if cimg_debug>1
 // ima.display("particles plus noise and ramp SIGlml");
#endif
//cut of the image gray level to suite grey level dynamics
  ima.cut(0,cutoff);
#if cimg_debug>0
  //ima.display("final SIG image");
#endif
//save
///08bit
  if(cutoff<256)
  {
    CImg<unsigned char> img(ima);
    img.save(filename);
  }
///16bit
  else
  {
    CImg<unsigned short> img(ima);
    img.save(filename);
  }
 return 0;
}

