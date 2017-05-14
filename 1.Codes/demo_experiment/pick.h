#ifndef CZQ_PICK
#define CZQ_PICK
#include <vector>
#define THRESHOLD 200
using namespace std;
struct coordinatei{
	int x;
	int y;
	int z;
};

class pickIt{
public:
	pickIt();
	int addObj(int x,int y,int z);
	float printDistance(float dir_x,float dir_y,float dir_z,float eye_x,float eye_y,float eye_z,float ball_x,float ball_y,float ball_z);//需要参数表
	int printPickedNum(float dir_x,float dir_y,float dir_z,float eye_x,float eye_y,float eye_z);
private:
	int capacity;
	vector<coordinatei> all_coordinate;

};
pickIt::pickIt(){
	capacity=0;
	all_coordinate.clear();
}

int pickIt::addObj(int x,int y,int z){
	++capacity;
	coordinatei xyz;
	xyz.x=x;
	xyz.y=y;
	xyz.z=z;
	all_coordinate.push_back(xyz);
	return capacity;
}

float pickIt::printDistance(float dir_x,float dir_y,float dir_z,float eye_x,float eye_y,float eye_z,float ball_x,float ball_y,float ball_z){
	float t,d,xc,yc,zc;
	t=(dir_x*(ball_x-eye_x)+dir_y*(ball_y-eye_y)+dir_z*(ball_z-eye_z))/(dir_x*dir_x+dir_y*dir_y+dir_z*dir_z);
	xc=dir_x*t+eye_x;
	yc=dir_y*t+eye_y;
	zc=dir_z*t+eye_z;
	d=sqrt((ball_x-xc)*(ball_x-xc)+(ball_y-yc)*(ball_y-yc)+(ball_z-zc)*(ball_z-zc));
	return d;
}

int pickIt::printPickedNum(float dir_x,float dir_y,float dir_z,float eye_x,float eye_y,float eye_z){//可选编号为1、2、3……，不选中返回0
	if(capacity==0)return 0;
	int num=0,num_pre_min=0;
	float dis=0,dis_pre_min=printDistance(dir_x,dir_y,dir_z,eye_x,eye_y,eye_z,all_coordinate[0].x,all_coordinate[0].y,all_coordinate[0].z);
	for(int i=1;i<capacity;i++){
		dis = printDistance(dir_x,dir_y,dir_z,eye_x,eye_y,eye_z,all_coordinate[i].x,all_coordinate[i].y,all_coordinate[i].z);
		if(dis<dis_pre_min){num=i;num_pre_min=i;dis_pre_min=dis;}
	}
	if(dis_pre_min<THRESHOLD)return num_pre_min+1;
	else return 0;
}

/****************************************************************************/
extern pickIt my_pick;
int COLLECTOR_PICK,MONITOR_PICK,ANEMOSCOPE_PICK,
	SCISSORS_PICK,SCREW_PICK,SHELF_PICK,OVEN_PICK,WIND_TUNNEL_PICK; 
int pick_initial(){
	COLLECTOR_PICK=my_pick.addObj(0,0,200);//collector
	MONITOR_PICK=my_pick.addObj(3000,600,0);
	ANEMOSCOPE_PICK=my_pick.addObj(800,0,-400);
	SCISSORS_PICK=my_pick.addObj(700,0,-400);
	SCREW_PICK=my_pick.addObj(850,0,-300);
	SHELF_PICK=my_pick.addObj(-350,0,200);
	OVEN_PICK=my_pick.addObj(750,0,300);
	WIND_TUNNEL_PICK=my_pick.addObj(-500,0,600);
	return 0;
}

#endif