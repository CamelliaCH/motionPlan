#pragma once
#ifndef WOLF_PACK_ALGORITHM
#define WOLF_PACK_ALGORITHM
#endif
#include <iostream>
#include <vector>
#include "Eigen/Dense"
#include <ctime>

#define SELF_PI 3.1415926

using namespace std;
using namespace Eigen;

class WPA
{
public:
	WPA();
	~WPA();
	void process(int num, int dim, double max_Bound, double min_Bound, int problem_Type, int gen_Num, int scout_Num,double &final_Res);

private:
	void init_Wolf();//��Ⱥ��ʼ��
	void pick_Head();//ѡ��ͷ��
	void ac_Wolfscout();//������Ϊ
	void ac_Summon();//�ٻ���Ϊ
	void ac_Beleaguer(int wolf_Ind);//Χ����Ϊ
	void ac_Quarryment();//Ⱥ�����
	double cal_Fitness(MatrixXd wolf);//������Ӧ��
	double cal_Dist(MatrixXd wolf_A, MatrixXd wolf_B, int cal_Mode);//������ƥ�ǵľ���

	vector<MatrixXd> wolves;
	vector<int>head_Wolf;//ͷ�Ƕ�Ӧ�����������ڿ����ж��������������,ĿǰΪ��ͷ��
	int wolf_Num, wolf_Dim;//��Ⱥ������ά��
	int scout_Times;//���ߵ�������
	int prob_Ind;//��ǰ�����Ӧ����
	int param_H;//̽��Ҫ��Ѱ�ķ�����,������������������б���һ����ֵ�����ڿɸ�ɶ�̬��
	int param_R;//ÿ�ε�������Ҫ��̭���ǵ�����
	double max_Val, min_Val;
	//double* wolf_Fit;//��Ⱥ��Ӧ�ȵĶ�̬����ָ��
	vector<double> wolf_Fit;
	double lead_Val;//ͷ�ǵ���Ӧ��
	double scout_Step,rush_Step,atk_Step;//���ߡ���Ϯ������ʱ�Ĳ���
	double d_Near;//�ɱ�Ϯת��Χ���ľ�����ֵ
};
