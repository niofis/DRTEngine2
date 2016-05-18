#include "DRTEngine.h"
/*#include "Vector.h"
#include "Ray.h"
//#include "NList.h"

#include "debug.h"
#include "Object.h"
#include "Light.h"
*/

#include <stdlib.h>
using namespace DRTE;

int g,m;

DRTEngine::DRTEngine()
{
	matrix=NULL;
	//lights=new Upp::Vector<Light *>();
	prev_frame=-1;
}
DRTEngine::~DRTEngine()
{
	//if(matrix!=NULL)
	//delete [] matrix;
	//delete lights;
	//delete _scene;
}

int DRTEngine::Render(char * pixels)
{
	
	
	if(!width || !height)
		return 1;
	if(this->_scene==NULL)
		return 2;
	this->matrix=pixels;
	/*if(matrix==NULL)
	{
		matrix=new char[width*height*3];
	}*/
	/*
	if(prev_frame!=work->frame_id)
	{
		prev_frame=work->frame_id;
		if((work->options & Work::PPM))
		{
			std::cout<<"Generando Luces...";
			std::cout<<std::dec<<"Total: "<<this->PPhotonMapping()<<"\n";
		}
	}*/
	//std::cout<<"Tamano "<<std::dec<<work->sector_width<<"x"<<work->sector_height<<" Y Offset: "<<work->sector_y<<"\n";
		//Tama�o para las c�maras)

	//Ahora vamos a barrer todo el buffer (matrix) y calculamos cada uno de los puntos para la camara
	//con ellos hacemos las pruebas de interseccion para todos los objetos en la escena
	Ray *ray=new Ray();
	Point *pt=new Point();
	#ifdef PLATFORM_WINCE
		DRTE::Color *cl=NULL;
	#endif
	int x;
	int k=0;
	int z=0;
	int total=width*height*3;

	/**********Variables para calcular el tiempo de renderizado***********/
	/*
	#ifdef PLATFORM_WIN32
		LARGE_INTEGER inicio;
		LARGE_INTEGER fin;
		LARGE_INTEGER freq;
	#else
		clock_t inicio,fin,freq;
	#endif

	long long i,f,q;
	#ifdef PLATFORM_WIN32
		QueryPerformanceCounter(&inicio);
	#else
		i=inicio=clock();
	#endif
	*/
	/**********Variables para calcular el tiempo de renderizado***********/
	
	Camera *camera=this->_scene->cameras[0];

	//A la c�mara se le establecen los valores de la resoluci�n requerida
	this->_scene->cameras[0]->SetDisplayDims(width,height);
	
	//Se crea en memoria el arreglo para la im�gen generada, del tama�o del sector requerido
	
	//matrix=new char[total];
	#ifdef PLATFORM_WIN32
		ZeroMemory(matrix,total);	//Y se pone todo en ceros
	#else
		memset(matrix,0,total);
	#endif


	//Se establece en la c�mara cuales son las coordenadas del sector a renderizar, esto es para que 
	//la c�mara genere los rayos de una sola vez y no estarlos generando uno por uno
	camera->SetSectorDims(0,0,width,height);
	
	
	
	//std::cout<<"Threads: "<<num_threads<<"\n";
	
	k=0;
	z=0;
	int total_pixels=width * height;
	int parts=1;
	
	if(threads>0)
		parts=threads;
	
	
	int pwidth=total_pixels/parts;
	int start=0;
	threads_done=0;
	for(x=0;x<parts-1;x++)
	{
		//threads.Add(0);
		CallWorker(x,start,pwidth);
		//RenderWorker(x,start,width);
		start+=pwidth;
	}
	//threads.Add(0);
	CallWorker(parts-1,start,total_pixels-start-1);
	//RenderWorker(parts-1,start,total_pixels-start);
		
	while(threads_done<parts)
	{
		#ifdef PLATFORM_WIN32
				Sleep(2);
		#else
				usleep(2000);
		#endif
	}
	
	/*
	//for(y=0; y<work->sector_height; y++)
	//{
		
		for(x=0; x<work->sector_width * work->sector_height; x++)
		{
			//Primer punto a optimizar, c�mara, para no estarla direccionando cada vez, la pongo en una variable antes de comenzar.
			//Segundo punto, un solo rayo, no mas generaci�n de rayos por cada vez, solo se obtiene cada vez la nueva direcci�n

			//Como el rayo es modificado varias veces durante el trazado, se le reestablecen los valores del origen
			ray->origin->u=camera->eye->u;
			ray->origin->v=camera->eye->v;
			ray->origin->w=camera->eye->w;
			//Se le establece la direcci�n del rayo, tal vez funcione m�s r�pido con un CopyMemory(), despues pruebo
			ray->direction->u=camera->directions[k].u;//Estos
			ray->direction->v=camera->directions[k].v;//tres
			ray->direction->w=camera->directions[k].w;//10.3766 ms

			//Se establece el grupo del punto como -1, que significa, que no pertenece al reflejo ni refraxi�n de nada
			pt->group_id=-1;
			//Comienza el trazado del rayo, el 10 significa que tantos rayos secundarios se pueden generar
			if(!Trace(pt,ray,10))
			{
				//Si no encuentra nada, el color devuelto es el del mundo
				#ifdef PLATFORM_WINCE
					cl=this->_scene->world->world_color;
				#else
					color=this->_scene->world->world_color->ToRGB();
				#endif
			}
			else
			{
				#ifdef PLATFORM_WINCE
					cl=pt->color;
				#else
					color=pt->color->ToRGB();			
				#endif
			}
			//Se guarda el color
			#ifdef PLATFORM_WINCE
				//std::cout<<"Guardando Color z="<<std::dec<<z<<", color="<<std::hex<<color<<", k="<<std::dec<<k<<"\n";
			#endif	
			#ifdef PLATFORM_WINCE
				matrix[z]=(char)(int)(255*((cl->b>1)?1:cl->b));
				matrix[z+1]=(char)(int)(255*((cl->g>1)?1:cl->g));
				matrix[z+2]=(char)(int)(255*((cl->r>1)?1:cl->r));
				
			#else
				*((int*)(matrix+z))+=color;
			#endif
			#ifdef PLATFORM_WINCE
				//std::cout<<"Guardando Color End\n";
			#endif	
			z+=3;										 
			k++;
			#ifdef PLATFORM_WINCE
				//std::cout<<"Pausa "<<std::dec<<k<<" Color= "<<std::hex<<color<<"\n";
			#endif
		}
	//}
	*/


	//Termina el trazado de los rayos
	//work->image_size=total;
	//work->image=(char*)matrix;
	//CopyMemory(pixels,matrix,width*height*3);
	
	/***********Calculos del tiempo de renderizado************************/
	/*
	#ifdef PLATFORM_WIN32
		QueryPerformanceCounter(&fin);
		QueryPerformanceFrequency(&freq);
		i=inicio.HighPart;
		i<<=32;
		i+=inicio.LowPart;
		f=fin.HighPart;
		f<<=32;
		f+=fin.LowPart;
		q=freq.HighPart;
		q<<=32;
		q+=freq.LowPart;
	#else
		f=fin=clock();
		q=freq=CLOCKS_PER_SEC;
	#endif
	*/
	//std::cout<<"Tiempo de renderizado: "<<(double)((double)(f-i)/(double)q)*1000<<" ms\n";

	/*******Tiempos:************/
	//Demo
	//T1: P4 43156 ms -> Optimizaci�n: Nada
	//T2: P4 42377.4 ms -> 1.8041% T1   Optimizaci�n: Variable global para c�mara.
	//T3: P4 40351 ms -> 6.499% T1 Op: Solo se genera la direcci�n del rayo cada vez, y no el rayo completo.
	// 
	//Cubo 12 Triangulos 2 Luces
	//T1: P4 7706.99 ms
	//T2: p4 1659.03 ms -> 78.47% T1 Op: Cambio de funcion de intersecci�n en tri�ngulos
	//T3: P4 1025.13 ms -> 86.69% T1 Op: Matriz de direcciones en c�mara para no generar continuamente, eliminado normalizaci�n de direcciones
	/***********Calculos del tiempo de renderizado************************/

	delete ray;
	delete pt;

	return 0;
}

int DRTEngine::CallWorker(int threadid, int k, int size)
{
	ThreadInfo *ti=new ThreadInfo;
	ti->parent=this;
	ti->k=k;
	ti->size=size;
	ti->threadid=threadid;
	
	//Upp::Thread().Run(Upp::callback1(RenderWorker,ti));
	//DRTECreateThread(RenderWorker,ti);
	RenderWorker(ti);
	return 0;
}


THREAD DRTEngine::RenderWorker(void* param)
{
	ThreadInfo *ti=(ThreadInfo *)param;
	int k=ti->k;
	int z=k*3;
	int x;
	
	Ray *ray=new Ray();
	Point *pt=new Point();
	#ifdef PLATFORM_WINCE
		DRTE::Color *cl=NULL;
	#endif
	int color;
	int bkgnd=ti->parent->_scene->world->world_color->ToRGB();
	
	Camera *camera=ti->parent->_scene->cameras[0];
	
	for(x=0;x<ti->size;x++)
	{

		ray->origin->u=camera->eye->u;
		ray->origin->v=camera->eye->v;
		ray->origin->w=camera->eye->w;		
				//Se le establece la direcci�n del rayo, tal vez funcione m�s r�pido con un CopyMemory(), despues pruebo
		ray->direction->u=camera->directions[k].u;//Estos
		ray->direction->v=camera->directions[k].v;//tres
		ray->direction->w=camera->directions[k].w;//10.3766 ms
		
				//Se establece el grupo del punto como -1, que significa, que no pertenece al reflejo ni refraxi�n de nada
		pt->group_id=-1;
				//Comienza el trazado del rayo, el 10 significa que tantos rayos secundarios se pueden generar
		if(!ti->parent->Trace(pt,ray,10))
		{			
					//Si no encuentra nada, el color devuelto es el del mundo
			#ifdef PLATFORM_WINCE
				cl=ti->parent->_scene->world->world_color;
			#else
				color=bkgnd;
			#endif
		}
		else
		{			
			#ifdef PLATFORM_WINCE
				cl=pt->color;
			#else
				color=pt->color->ToRGB();			
			#endif
		}		
		//Se guarda el color
		#ifdef PLATFORM_WINCE
			//std::cout<<"Guardando Color z="<<std::dec<<z<<", color="<<std::hex<<color<<", k="<<std::dec<<k<<"\n";
		#endif	
		#ifdef PLATFORM_WINCE
			ti->parent->matrix[z]=(char)(int)(255*((cl->b>1)?1:cl->b));
			ti->parent->matrix[z+1]=(char)(int)(255*((cl->g>1)?1:cl->g));
			ti->parent->matrix[z+2]=(char)(int)(255*((cl->r>1)?1:cl->r));
		#else
			*((int*)(ti->parent->matrix+z))=color;
		#endif
		z+=3;
		k++;
	}

	//threads[threadid]=1;
	ti->parent->threads_done++;
	delete ray;
	delete pt;
	delete ti;
	
	return 0;
}


int DRTEngine::SetScene(DRTEScene *scene)
{
	this->_scene=scene;
	prev_frame=-1;
	return 0;
}


int DRTEngine::Trace(Point* pt, Ray *ray, int depth)
{
	
	unsigned int x=0;

	Object* closest_object=NULL;
	Triangle *triangle=NULL;
	Sphere *sphere=NULL;
	
	float *vert0,*vert1;

	float dist,t0,t,ta,u,v;

	dist=t=Intersection(ray,&closest_object);
	
	if(!t)
	{
		if(pt!=NULL)
		{
			pt->color->r=_scene->world->world_color->r;
			pt->color->g=_scene->world->world_color->g;
			pt->color->b=_scene->world->world_color->b;
		}
		return 0;
	}

	Material *material=NULL;
	int previous_group=pt->group_id;
	float old_color[3]={0.0f,0.0f,0.0f};
	float rx_color[3]={0.0f,0.0f,0.0f};
	float rc_color[3]={0.0f,0.0f,0.0f};

	pt->group_id=closest_object->group_id;
	//material=_scene->materials->At(_scene->groups->At(pt->group_id)->material_id);
	material=_scene->materials[_scene->groups[pt->group_id]->material_id];
	pt->color->r=material->diffuse_color->r;
	pt->color->g=material->diffuse_color->g;
	pt->color->b=material->diffuse_color->b;
	
	
	pt->u=ray->origin->u + ray->direction->u*t;
	pt->v=ray->origin->v + ray->direction->v*t;
	pt->w=ray->origin->w + ray->direction->w*t;
	


	switch(closest_object->type)
	{
	case Object::TRIANGLE:
		triangle=(Triangle*)closest_object;
		pt->normal->u=triangle->normal->u;
		pt->normal->v=triangle->normal->v;
		pt->normal->w=triangle->normal->w;
		pt->normal->Normalize();
		break;
	case Object::SPHERE:
		sphere=(Sphere*)closest_object;
		pt->normal->u=pt->u - sphere->center->u;
		pt->normal->v=pt->v - sphere->center->v;
		pt->normal->w=pt->w - sphere->center->w;
		pt->normal->Normalize();
		break;
	}

	//if(!ppm)
	Illumination(pt,ray);//13715ms
	//pt->light->r=1.0f;//3900ms
	//pt->light->g=1.0f;
	//pt->light->b=1.0f;
	/*ray->direction->Normalize();
	float l=1-((*(pt->normal)) * ray->direction);
	if(l<=0)
		l=0;
	pt->color->r*=l;
	pt->color->g*=l;
	pt->color->b*=l;*/
	
	if(depth>0)
	{

		//Refracci�n
		// Tomado de Reflections and Refractions in Ray Tracing by Bram de Greve (bram.degreve@gmail.com)
		if(material->transparency_index>0.0f)
		{
			
			vert0=(float*)(&(ray->direction->u)); //V
			vert1=(float*)(&(pt->normal->u));	//N
			
			if(previous_group!=-1)
				ta=_scene->materials[_scene->groups[previous_group]->material_id]->refraction_index;//n1
			else
				ta=1.0f;
			
			if(pt->group_id!=-1)
				t=_scene->materials[_scene->groups[closest_object->group_id]->material_id]->refraction_index;
			else
				t=1.0f;
			
			if(previous_group==pt->group_id)
			{
				vert1[0]*=-1;
				vert1[1]*=-1;
				vert1[2]*=-1;
			}
			else
			{
				ta=1.0f;
			}
			
			//t=material->refraction_index;//n2

			v = ta / t;//n
			u = V_DOT(vert0,vert1);//cosI

			t0 = (float)( v * v * (1.0 - u * u)); //sinT2

			if (t0 <= 1.0)
			{				
				//n * incident - (n + sqrt(1.0 - t0)) * normal;
				vert0[0]*=v;
				vert0[1]*=v;
				vert0[2]*=v;
				
				t=-(v + sqrt(1.0f - t0));
				vert0[0]+=t * vert1[0];
				vert0[1]+=t * vert1[1];			
				vert0[2]+=t * vert1[2];
				
				ray->origin->u=pt->u;
				ray->origin->v=pt->v;
				ray->origin->w=pt->w;
				
				ray->direction->Normalize();
				old_color[0]=pt->color->r;
				old_color[1]=pt->color->g;
				old_color[2]=pt->color->b;
				Trace(pt,ray,depth-1);
				pt->color->r=(pt->color->r * (material->transparency_index)) + (old_color[0] * (1-material->transparency_index));
				pt->color->g=(pt->color->g * (material->transparency_index)) + (old_color[1] * (1-material->transparency_index));
				pt->color->b=(pt->color->b * (material->transparency_index)) + (old_color[2] * (1-material->transparency_index));
				
			}		
		}


		switch(closest_object->type)
		{
		case Object::TRIANGLE:
			triangle=(Triangle*)closest_object;
			pt->normal->u=triangle->normal->u;
			pt->normal->v=triangle->normal->v;
			pt->normal->w=triangle->normal->w;
			pt->normal->Normalize();
			break;
		case Object::SPHERE:
			sphere=(Sphere*)closest_object;
			pt->normal->u=pt->u - sphere->center->u;
			pt->normal->v=pt->v - sphere->center->v;
			pt->normal->w=pt->w - sphere->center->w;
			pt->normal->Normalize();
			break;
		}

		//REFLEXION
		//Tomado de http://www.devmaster.net/articles/raytracing_series/part2.php
		
		if(material->reflection_index>0.0f)
		{
			vert0=(float*)(&(ray->direction->u)); //V
			vert1=(float*)(&(pt->normal->u));	//N
			t=(V_DOT(vert0,vert1)) * 2 ;
			//R = V � 2 * ( V . N ) * N;

			ray->direction->u=vert0[0] - t * vert1[0];
			ray->direction->v=vert0[1] - t * vert1[1];
			ray->direction->w=vert0[2] - t * vert1[2];

			ray->origin->u=pt->u;
			ray->origin->v=pt->v;
			ray->origin->w=pt->w;

			old_color[0]=pt->color->r;
			old_color[1]=pt->color->g;
			old_color[2]=pt->color->b;

			old_color[0]*=1-material->reflection_index;
			old_color[1]*=1-material->reflection_index;
			old_color[2]*=1-material->reflection_index;
			
			Trace(pt,ray,depth-1);

			pt->color->r=old_color[0] + pt->color->r*material->reflection_index;
			pt->color->g=old_color[1] + pt->color->g*material->reflection_index;
			pt->color->b=old_color[2] + pt->color->b*material->reflection_index;
		}
	
	}
	
	return 1;
}


int DRTEngine::Illumination(Point *point, Ray *i_ray)
{	

	Light *light=NULL;
	float d,dg,t;
	float *pf_vct,*pf_pnorm;
	int x=0;
	float vct[3];
	Ray ray;
	float norma=0;


	ray.origin->u=point->u;
	ray.origin->v=point->v;
	ray.origin->w=point->w;
	
	point->light->r=0;
	point->light->g=0;
	point->light->b=0;

	Light **lights;
	
	lights=_scene->lights;
	//int lc=_scene->num_lights;

	for(x=0;x<_scene->num_lights;x++)
	{

		light=lights[x];

		vct[0]=light->center->u-point->u;
		vct[1]=light->center->v-point->v;
		vct[2]=light->center->w-point->w;
		vct[0]*=vct[0];
		vct[1]*=vct[1];
		vct[2]*=vct[2];
		d= sqrt(vct[0]+vct[1]+vct[2]);
		
		if(d>light->intensity)
			continue;

		vct[0]=light->center->u - point->u;
		vct[1]=light->center->v - point->v;
		vct[2]=light->center->w - point->w;
		
		pf_vct=(float*)(&(vct));
		pf_pnorm=(float*)(&(point->normal->u));

		norma=sqrt(vct[0]*vct[0]+vct[1]*vct[1]+vct[2]*vct[2]);
		vct[0]/=norma;
		vct[1]/=norma;
		vct[2]/=norma;
		
		dg=V_DOT(pf_vct,pf_pnorm);


		if(dg<=0)
			continue;

		ray.direction->u=vct[0];
		ray.direction->v=vct[1];
		ray.direction->w=vct[2];


		//Busca objeto que obstruya la luz
		t=Intersection(&ray,NULL);
		if(t>0 && t<d)//if(!t==d)
			continue;

		//Aplica la distancia como par�metro para la intensidad de la luz
		d=(light->intensity-d)/light->intensity;
		dg*=d;
	
		point->light->r+=light->color->r*dg;
		point->light->g+=light->color->g*dg;
		point->light->b+=light->color->b*dg;

		if(point->light->r>1.0f)
			point->light->r=1.0f;
		if(point->light->b>1.0f)
			point->light->b=1.0f;
		if(point->light->g>1.0f)
			point->light->g=1.0f;

		/////////Phong Lighting////////////////////////
		/*
		material=_scene->materials->At(_scene->groups->At(point->group_id)->material_id);
		if(material->specular_index>0.0f)
		{
			float *V=(float*)(&(i_ray->direction->u)); //V
			float *N=(float*)(&(point->normal->u));
			float *L=(float*)(&(ray.direction->u));
			float t=V_DOT( L, N );
			// 2.0f * V_DOT( L, N ) * N;
			R[0] = L[0] - 2.0f*t*N[0];
			R[1] = L[1] - 2.0f*t*N[1];
			R[2] = L[2] - 2.0f*t*N[2];
			
			float dot = V_DOT( V, R );
			if (dot > 0)
			{
				float spec = powf( dot, 20 ) * material->specular_index * dg;
				// add specular component to ray color
				//a_Acc += spec * light->GetMaterial()->GetColor();
				point->color->r+=spec*light->color->r;
				point->color->g+=spec*light->color->g;
				point->color->b+=spec*light->color->b;
			}
		}*/
	}

	point->color->Multiply(point->light);

	return 0;
}


float DRTEngine::Intersection(Ray* ray, Object** out_object)
{
	
	Object *object=NULL;
	Object *closest_object=NULL;
	Triangle *triangle=NULL;
	Sphere *sphere=NULL;

	float edge1[3],tvec[3],pvec[3],qvec[3];
	float *orig,*dir,*vert0;
	float det,inv_det;
	float u, v, t,ta,t0;
	float B,B2,C,I;
	
	t=0;
	ta=0;

	orig=(float*)(&(ray->origin->u));
	dir=(float*)(&(ray->direction->u));

	int x;

	//Object **objects;
	
	//objects=_scene->objects;
	//int oc=_scene->num_objects;//objects->GetCount();
	
	
	for(x=0;x<_scene->num_objects;x++)
	{
		object=_scene->objects[x];
		
			
		switch(object->type)
		{
		case Object::TRIANGLE:
			triangle=(Triangle*)object;	
		
			/* Estos re realizan en la funcion CalcEdges de cada tri�ngulo, solo la primera vez
			vert0=(float*)(&(triangle->v1->u));
			vert1=(float*)(&(triangle->v2->u));
			vert2=(float*)(&(triangle->v3->u));

			V_SUB(edge1,vert1,vert0);
			V_SUB(edge2,vert2,vert0);
			*/
			
			V_CROSS(pvec,dir,triangle->edge2);

			det=V_DOT(triangle->edge1,pvec);

			//NO CULLING

			if(det > -EPSILON && det <EPSILON)
				break;
			inv_det=1.0f/det;

			V_SUB(tvec,orig,triangle->vert0);

			u=V_DOT(tvec,pvec) * inv_det;
			if(u < 0.0f || u > 1.0f)
				break;

			V_CROSS(qvec,tvec,triangle->edge1);
			v=V_DOT(dir,qvec) * inv_det;
			if(v < 0.0f || u + v > 1.0f)
				break;

			
			t=V_DOT(triangle->edge2,qvec)*inv_det;
			
			if(t<0.01)
				break;
			
			if(t<ta || ta==0)
			{
				ta=t;
				closest_object=object;
			}
			break;

		
		case Object::SPHERE:
			sphere=(Sphere*)object;

			vert0=(float*)(&(sphere->center->u));//Vector *c=sphere->center;
			V_SUB(edge1,orig,vert0);//Vector *f= (*e)-c;
			B=-2*V_DOT(edge1,dir);//B=((*f) * d)*2;
			B2=B*B;
			C=(V_DOT(edge1,edge1)) - (sphere->r*sphere->r);// C=((*f) * f) - r2;
			I=B2 - 4*C;

			if(I<0)					//No hay intersecci�n
				break;

			t0=sqrt(I);		//C�lculos para obtener el(los) punto(s) de intersecci�n

			t=(B - t0)/2;

			if(t<0.01)
				t=(B + t0)/2;	

			if(t<0.01)
				break;
			if(t<ta || ta==0)
			{
				ta=t;
				closest_object=object;
			}
			break;
		}
	}

	if(out_object!=NULL)
	{
		*out_object=closest_object;
	}

	return ta;
}
